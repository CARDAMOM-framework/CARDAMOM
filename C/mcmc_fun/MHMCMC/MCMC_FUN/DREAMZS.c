#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../../../math_fun/std.c"
#include "NORMPARS.c"
#include "STEP_DREAMZS.c"
#include "STEP_DEMCMCZS.c"
#include "WRITE_DEMCMC_RESULTS.c"

/*here including additional functions needed to initialise and clear memory*/
#include "INITIALIZE_MCMC_OUTPUT.c"

/*DREAM(ZS)-lite: archive-based DE-MCZS with a DREAM-style crossover mask.
 *
 * This keeps the DEMCMCZS archive and snooker mechanics, but the standard
 * parallel proposal only updates a random subset of parameters. The CR menu is
 * fixed rather than adaptively weighted, keeping this mode interpretable as a
 * first-pass DREAM(ZS) experiment.
 */
double *DREAMZS(
double (MODEL_LIKELIHOOD)(DATA, double *),
DATA DATA, PARAMETER_INFO PI, MCMC_OPTIONS MCO, MCMC_OUTPUT *MCOUT){

printf("CARDAMOM: Successfully entered DREAMZS!\n"); fflush(stdout);

/*ERASING PREVIOUS FILE IF APPEND == 0 */
if(MCO.APPEND==0 && MCO.nWRITE>0){FILE *fileout=fopen(MCO.outfile,"wb");fclose(fileout);}

int NC=MCO.nchains;

/*Algorithm constants, matching DEMCMCZS where possible.*/
int M0=10*PI.npars;
if (M0<NC){M0=NC;}
int K=10;
double psnooker=0.10;
double CRvals[4]={0.1,0.2,0.5,1.0};
int nCR=4;

/*Z is known to be bounded in size, since nOUT is fixed.*/
int maxappends=MCO.nOUT/K+1;
int maxM=M0+NC*maxappends;

double *Z=calloc((size_t)maxM*PI.npars,sizeof(double));
double *X=calloc(PI.npars*NC,sizeof(double));
double *pars_new=calloc(PI.npars,sizeof(double));
double *P=calloc(NC,sizeof(double));
double *BESTPARS=calloc(PI.npars*NC,sizeof(double));
double *BESTP=calloc(NC,sizeof(double));

int n,nn,M=M0;
double par;

/*Initializing the first NC rows of the archive (= the live chains) from PI.parini.*/
for (nn=0;nn<NC;nn++){
for (n=0;n<PI.npars;n++){
if (MCO.randparini==1 && PI.parfix[n]!=1){
par=nor2par((double)random()/(double)RAND_MAX,PI.parmin[n],PI.parmax[n]);
}else{
par=PI.parini[n+nn*PI.npars];
if (par>PI.parmax[n] || par<PI.parmin[n]){printf("Warning, prescribed initial parameters are out of range\n");}
}
Z[nn*PI.npars+n]=par;
X[nn*PI.npars+n]=par;
}}

/*Initializing the remaining archive rows with a wide random scatter.*/
for (nn=NC;nn<M0;nn++){
for (n=0;n<PI.npars;n++){
if (PI.parfix[n]==1){par=PI.parini[n];}
else{par=nor2par((double)random()/(double)RAND_MAX,PI.parmin[n],PI.parmax[n]);}
Z[nn*PI.npars+n]=par;
}}

oksofar("Established PI.parini - beginning DREAMZS now");

memcpy(BESTPARS,X,PI.npars*NC*sizeof(double));

/*STEP 1 - RUN MODEL WITH INITIAL PARAMETERS*/
for (nn=0;nn<NC;nn++){
P[nn]=MODEL_LIKELIHOOD(DATA,&X[nn*PI.npars]);
if (isnan(P[nn])){printf("Warning: MLF generated NaN... treating as -Inf");P[nn]=log(0);}
if (isinf(P[nn])==-1){printf("WARNING! P(0)=-inf - DREAMZS may get stuck - if so, please check your initial conditions\n");}
}
memcpy(BESTP,P,NC*sizeof(double));

double P_new,lr,gratio,CR;
int withinrange,wrlocal=0,nupdate=0,totalupdates=0;

COUNTERS N;
N.ACC=0;
N.ITER=MCO.nSTART;
N.ACCLOC=0;
N.ACCRATE=0;

/*STEP 2 - BEGIN MCMC*/
for ( ;N.ITER<MCO.nOUT;N.ITER++){

for (nn=0;nn<NC;nn++){

gratio=0;
if ((double)random()/(double)RAND_MAX<psnooker){
withinrange=STEP_DEMCMCZ_SNOOKER(&X[nn*PI.npars],Z,M,pars_new,PI,&gratio);
nupdate=PI.npars;
}else{
int cridx=(int)(((double)random()/((double)RAND_MAX))*nCR);
if (cridx>=nCR){cridx=nCR-1;}
CR=CRvals[cridx];
withinrange=STEP_DREAMZS_PARALLEL(&X[nn*PI.npars],Z,M,pars_new,PI,CR,&nupdate);
}
totalupdates=totalupdates+nupdate;

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
for (n=0;n<PI.npars;n++){X[n+nn*PI.npars]=pars_new[n];}
if (P_new>=BESTP[nn]){
for (n=0;n<PI.npars;n++){BESTPARS[n+nn*PI.npars]=pars_new[n];}
BESTP[nn]=P_new;}
P[nn]=P_new;
}
}

/*regularly write results*/
if (MCO.nWRITE>0 && (N.ITER % MCO.nWRITE)==0){
WRITE_DEMCMC_RESULTS(X,PI,MCO,N.ITER);}

/*regularly write restart file*/
if (MCO.nWRITE>0 && (N.ITER % 1000)==0){
WRITE_DEMCMC_RESTART(X,PI,MCO,N.ITER);}

/*Printing Info*/
if (MCO.nPRINT>0 && N.ITER % MCO.nPRINT==0){
printf("%d out of %d generations (archive size = %d out of %d)\n",N.ITER,MCO.nOUT,M,maxM);
printf("within range = %2.2f%%\n",wrlocal/((double)(N.ITER+1)*NC)*100);
printf("Local Acceptance rate %5.1f%%\n",100*(double)N.ACC/((double)(N.ITER+1)*NC));
printf("Mean updated dims = %2.2f out of %d\n",(double)totalupdates/((double)(N.ITER+1)*NC),PI.npars);
printf("Log Likelihoods: ");
for (nn=0;nn<NC;nn++){printf("%2.1f ",P[nn]);}
printf("\n");
}

/*Appending the current chain states to the archive Z mat. every K generations*/
if ((N.ITER+1) % K==0 && M+NC<=maxM){
for (nn=0;nn<NC;nn++){
for (n=0;n<PI.npars;n++){Z[(M+nn)*PI.npars+n]=X[nn*PI.npars+n];}}
M=M+NC;
}

}

/*filling in MCOUT details*/
for (n=0;n<PI.npars*NC;n++){MCOUT->best_pars[n]=BESTPARS[n];}
MCOUT->complete=1;

free(BESTPARS);
free(BESTP);
free(Z);
free(X);
free(pars_new);
free(P);
printf("DREAMZS DONE\n");

return 0;
}
