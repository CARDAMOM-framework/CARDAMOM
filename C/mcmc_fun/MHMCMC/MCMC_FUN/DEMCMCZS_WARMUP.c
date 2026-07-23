#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "NORMPARS.c"
#include "STEP_DEMCMCZS.c"

/*Self-contained, I/O-free core of DEMCMCZS.c (ter Braak & Vrugt 2008), used by
 *mode 6 as an intermediate "warm-up" phase between the 400-chain EDC search
 *and AFDEMCMC production. Runs niter generations on NC chains, overwriting
 *parini in place with the live chain state at the end. No output writing, no
 *restart support - this is only ever an intermediate step feeding into a
 *subsequent sampler, not a saved result in its own right.*/
void RUN_DEMCMCZS_WARMUP(
double (MODEL_LIKELIHOOD)(DATA, double *),
DATA DATA, PARAMETER_INFO PI, int NC, int niter, double *parini){

int M0=10*PI.npars;
if (M0<NC){M0=NC;}
int K=10;
double psnooker=0.10;

int maxappends=niter/K;
int maxM=M0+NC*maxappends;

double *Z=calloc((size_t)maxM*PI.npars,sizeof(double));
double *X=calloc(PI.npars*NC,sizeof(double));
double *pars_new=calloc(PI.npars,sizeof(double));
double *P=calloc(NC,sizeof(double));

int n,nn,M=M0;

/*seeding the archive's first NC rows (= the live chains) from the caller-supplied parini*/
for (nn=0;nn<NC;nn++){
for (n=0;n<PI.npars;n++){
Z[nn*PI.npars+n]=parini[n+nn*PI.npars];
X[nn*PI.npars+n]=parini[n+nn*PI.npars];
}}

/*scattering the remaining archive rows randomly, same as DEMCMCZS.c*/
for (nn=NC;nn<M0;nn++){
for (n=0;n<PI.npars;n++){
if (PI.parfix[n]==1){Z[nn*PI.npars+n]=parini[n];}
else{Z[nn*PI.npars+n]=nor2par((double)random()/(double)RAND_MAX,PI.parmin[n],PI.parmax[n]);}
}}

for (nn=0;nn<NC;nn++){
P[nn]=MODEL_LIKELIHOOD(DATA,&X[nn*PI.npars]);
if (isnan(P[nn])){P[nn]=log(0);}
}

double P_new,lr,gratio;
int withinrange,iter;

for (iter=0;iter<niter;iter++){

for (nn=0;nn<NC;nn++){

gratio=0;
if ((double)random()/(double)RAND_MAX<psnooker){
withinrange=STEP_DEMCMCZ_SNOOKER(&X[nn*PI.npars],Z,M,pars_new,PI,&gratio);
}else{
withinrange=STEP_DEMCMCZ_PARALLEL(&X[nn*PI.npars],Z,M,pars_new,PI);
}

lr=log((double)random()/(double)RAND_MAX);

if (withinrange==1){
P_new=MODEL_LIKELIHOOD(DATA,pars_new);
}else{
P_new=log(0);
}
if (isnan(P_new)){P_new=log(0);}

if (P_new-P[nn]+gratio>lr || (isinf(P[nn]) && withinrange==1)){
for (n=0;n<PI.npars;n++){X[n+nn*PI.npars]=pars_new[n];}
P[nn]=P_new;
}
}

/*appending the current chain states to the archive every K generations, same as DEMCMCZS.c*/
if ((iter+1) % K==0 && M+NC<=maxM){
for (nn=0;nn<NC;nn++){
for (n=0;n<PI.npars;n++){Z[(M+nn)*PI.npars+n]=X[nn*PI.npars+n];}}
M=M+NC;
}

}

for (nn=0;nn<NC;nn++){
for (n=0;n<PI.npars;n++){parini[n+nn*PI.npars]=X[nn*PI.npars+n];}}

free(Z);free(X);free(pars_new);free(P);
}
