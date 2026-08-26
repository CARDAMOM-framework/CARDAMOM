#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../../../math_fun/std.c"
#include "NORMPARS.c"
#include "STEP_HYBRID_AIDE.c"
#include "WRITE_DEMCMC_RESULTS.c"

/*here including additional functions needed to initialise and clear memory*/
#include "INITIALIZE_MCMC_OUTPUT.c"

/*HYBRID_AIDE: experimental live-ensemble sampler combining CARDAMOM DEMCMC
 *translation moves and affine stretch moves inside each proposal. No archive is
 *used; the proposal source is the live ensemble, like modes 3 and 4.*/
double *HYBRID_AIDE(
double (MODEL_LIKELIHOOD)(DATA, double *),
DATA DATA, PARAMETER_INFO PI, MCMC_OPTIONS MCO, MCMC_OUTPUT *MCOUT){

printf("CARDAMOM: Successfully entered HYBRID_AIDE!\n"); fflush(stdout);

/*ERASING PREVIOUS FILE IF APPEND == 0 */
if(MCO.APPEND==0 && MCO.nWRITE>0){FILE *fileout=fopen(MCO.outfile,"wb");fclose(fileout);}

int NC=MCO.nchains;

double *P=calloc(NC,sizeof(double));
double *PARS=calloc(PI.npars*NC,sizeof(double));
double *pars_new=calloc(PI.npars,sizeof(double));
double *BESTPARS=calloc(PI.npars*NC,sizeof(double));
double *BESTP=calloc(NC,sizeof(double));

int n,nn,withinrange,wrlocal=0;
double par,P_new,gratio,lr;

COUNTERS N;
N.ACC=0;
N.ITER=MCO.nSTART;
N.ACCLOC=0;
N.ACCRATE=0;

/*Initialize live chains from PI.parini, same convention as other ensemble samplers.*/
for (nn=0;nn<NC;nn++){
for (n=0;n<PI.npars;n++){
if (MCO.randparini==1 && PI.parfix[n]!=1){
PARS[n+nn*PI.npars]=nor2par((double)random()/(double)RAND_MAX,PI.parmin[n],PI.parmax[n]);
}else{
par=PI.parini[n+nn*PI.npars];
PARS[n+nn*PI.npars]=par;
if (par>PI.parmax[n] || par<PI.parmin[n]){printf("Warning, prescribed initial parameters are out of range\n");}
}
}}

oksofar("Established PI.parini - beginning HYBRID_AIDE now");

memcpy(BESTPARS,PARS,PI.npars*NC*sizeof(double));

/*STEP 1 - RUN MODEL WITH INITIAL PARAMETERS*/
for (nn=0;nn<NC;nn++){
P[nn]=MODEL_LIKELIHOOD(DATA,&PARS[nn*PI.npars]);
if (isnan(P[nn])){printf("Warning: MLF generated NaN... treating as -Inf");P[nn]=log(0);}
if (isinf(P[nn])==-1){printf("WARNING! P(0)=-inf - HYBRID_AIDE may get stuck - if so, please check your initial conditions\n");}
}
memcpy(BESTP,P,NC*sizeof(double));

/*STEP 2 - BEGIN MCMC*/
for ( ;N.ITER<MCO.nOUT;N.ITER++){

for (nn=0;nn<NC;nn++){

gratio=0;
withinrange=STEP_HYBRID_AIDE(PARS,pars_new,PI,nn,NC,&gratio);

lr=log((double)random()/(double)RAND_MAX);
if (withinrange==1){
wrlocal=wrlocal+1;
P_new=MODEL_LIKELIHOOD(DATA,pars_new);
}else{
P_new=log(0);
}
if (isnan(P_new)){P_new=log(0);}

if (P_new-P[nn]+gratio>lr || (isinf(P[nn]) && withinrange==1)){
N.ACC=N.ACC+1;
for (n=0;n<PI.npars;n++){PARS[n+nn*PI.npars]=pars_new[n];}
if (P_new>=BESTP[nn]){
for (n=0;n<PI.npars;n++){BESTPARS[n+nn*PI.npars]=pars_new[n];}
BESTP[nn]=P_new;}
P[nn]=P_new;
}
}

/*regularly write results*/
if (MCO.nWRITE>0 && (N.ITER % MCO.nWRITE)==0){
WRITE_DEMCMC_RESULTS(PARS,PI,MCO,N.ITER);}

/*regularly write restart file*/
if (MCO.nWRITE>0 && (N.ITER % 1000)==0){
WRITE_DEMCMC_RESTART(PARS,PI,MCO,N.ITER);}

/*Printing Info to Screen*/
if (MCO.nPRINT>0 && N.ITER % MCO.nPRINT==0){
printf("%d out of %d iterations\n",N.ITER,MCO.nOUT);
printf("within range = %2.2f%%\n",wrlocal/((double)(N.ITER+1)*NC)*100);
printf("Local Acceptance rate %5.1f%%\n",100*(double)N.ACC/((double)(N.ITER+1)*NC));
printf("Log Likelihoods: ");
for (nn=0;nn<NC;nn++){printf("%2.1f ",P[nn]);}
printf("\n");
}

}

/*filling in MCOUT details*/
for (n=0;n<PI.npars*NC;n++){MCOUT->best_pars[n]=BESTPARS[n];}
MCOUT->complete=1;

free(BESTPARS);
free(BESTP);
free(PARS);
free(pars_new);
free(P);
printf("HYBRID_AIDE DONE\n");

return 0;
}
