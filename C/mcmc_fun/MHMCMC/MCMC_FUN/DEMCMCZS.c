#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../../../math_fun/std.c"
#include "NORMPARS.c"
#include "STEP_DEMCMCZS.c"
#include "WRITE_DEMCMC_RESULTS.c"

/*here including additional functions needed to initialise and clear memory*/
#include "INITIALIZE_MCMC_OUTPUT.c"

/*DE-MCZS: Differential Evolution Markov Chain sampling difference vectors
 * from an archive of past states (DE-MCZ, ter Braak 2006), mixed with a 10%
 * snooker update (ter Braak & Vrugt 2008, Stat Comput). Designed to work
 * well with very few live chains (default N=3) by exploiting information
 * from the chains' own past instead of the other live chains.
 *
 * Unlike DEMCMC/AFDEMCMC, proposals are NOT drawn from the other live
 * chains. Instead, an archive Z of past states is grown by appending the
 * current chain states every K generations; proposals are drawn from Z,
 * which is frozen between appends. This is what allows so few live chains
 * to work well.
 *
 * v1 scope: no restart/append support (MCO.nSTART is not honoured here,
 * matching DEMCMC.c's current scope).
 * */

double *DEMCMCZS(
double (MODEL_LIKELIHOOD)(DATA, double *),
DATA DATA, PARAMETER_INFO PI, MCMC_OPTIONS MCO, MCMC_OUTPUT *MCOUT){

/*ERASING PREVIOUS FILE IF APPEND == 0 */
if(MCO.APPEND==0 && MCO.nWRITE>0){FILE *fileout=fopen(MCO.outfile,"wb");fclose(fileout);}

int NC=MCO.nchains;

/*Algorithm constants (ter Braak & Vrugt 2008 defaults: M0=10*npars, K=10, 10% snooker updates)*/
int M0=10*PI.npars;
if (M0<NC){M0=NC;}
int K=10;
double psnooker=0.10;

/*Z is known to be bounded in size, since nOUT (total generations) is fixed - preallocate rather than realloc*/
int maxappends=MCO.nOUT/K;
int maxM=M0+NC*maxappends;

double *Z=calloc((size_t)maxM*PI.npars,sizeof(double));
double *X=calloc(PI.npars*NC,sizeof(double));
double *pars_new=calloc(PI.npars,sizeof(double));
double *P=calloc(NC,sizeof(double));
double *BESTPARS=calloc(PI.npars*NC,sizeof(double));
double *BESTP=calloc(NC,sizeof(double));

int n,nn,M=M0;
double par;

/*Initializing the first NC rows of the archive (= the live chains) from PI.parini,
 *same convention as DEMCMC.c/AFDEMCMC.c*/
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

/*Initializing the remaining archive rows (NC..M0-1) with a wide random scatter, so the
 *archive is well-diversified from the start (ter Braak & Vrugt 2008, sect. 2.2)*/
for (nn=NC;nn<M0;nn++){
for (n=0;n<PI.npars;n++){
if (PI.parfix[n]==1){par=PI.parini[n];}
else{par=nor2par((double)random()/(double)RAND_MAX,PI.parmin[n],PI.parmax[n]);}
Z[nn*PI.npars+n]=par;
}}

oksofar("Established PI.parini - beginning DEMCMCZS now");

memcpy(BESTPARS,X,PI.npars*NC*sizeof(double));

/*STEP 1 - RUN MODEL WITH INITIAL PARAMETERS*/
for (nn=0;nn<NC;nn++){
P[nn]=MODEL_LIKELIHOOD(DATA,&X[nn*PI.npars]);
if (isnan(P[nn])){printf("Warning: MLF generated NaN... treating as -Inf");P[nn]=log(0);}
if (isinf(P[nn])==-1){printf("WARNING! P(0)=-inf - DEMCMCZS may get stuck - if so, please check your initial conditions\n");}
}
memcpy(BESTP,P,NC*sizeof(double));

double P_new,lr,gratio;
int withinrange,wrlocal=0;

COUNTERS N;
N.ACC=0;N.ITER=0;N.ACCLOC=0;N.ACCRATE=0;

/*STEP 2 - BEGIN MCMC*/
for (N.ITER=0;N.ITER<MCO.nOUT;N.ITER++){

for (nn=0;nn<NC;nn++){

gratio=0;
if ((double)random()/(double)RAND_MAX<psnooker){
withinrange=STEP_DEMCMCZ_SNOOKER(&X[nn*PI.npars],Z,M,pars_new,PI,&gratio);
}else{
withinrange=STEP_DEMCMCZ_PARALLEL(&X[nn*PI.npars],Z,M,pars_new,PI);
}

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

/*Printing Info to Screen*/
if (MCO.nPRINT>0 && N.ITER % MCO.nPRINT==0){
printf("%d out of %d generations (archive size = %d out of %d)\n",N.ITER,MCO.nOUT,M,maxM);
printf("within range = %2.2f%%\n",wrlocal/((double)(N.ITER+1)*NC)*100);
printf("Local Acceptance rate %5.1f%%\n",100*(double)N.ACC/((double)(N.ITER+1)*NC));
printf("Log Likelihoods: ");
for (nn=0;nn<NC;nn++){printf("%2.1f ",P[nn]);}
printf("\n");
}

/*Appending the current chain states to the archive every K generations*/
if ((N.ITER+1) % K==0 && M+NC<=maxM){
for (nn=0;nn<NC;nn++){
for (n=0;n<PI.npars;n++){Z[(M+nn)*PI.npars+n]=X[nn*PI.npars+n];}}
M=M+NC;
}

/*END OF WHILE LOOP*/
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
printf("DEMCMCZS DONE\n");

return 0;

/*END OF DEMCMCZS*/
}
