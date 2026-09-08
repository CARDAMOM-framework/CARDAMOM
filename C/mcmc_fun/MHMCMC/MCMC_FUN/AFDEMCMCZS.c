#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../../../math_fun/std.c"
#include "NORMPARS.c"
#include "STEP_AFDEMCMC.c"
#include "STEP_DEMCMCZS.c"
#include "WRITE_DEMCMC_RESULTS.c"

/*here including additional functions needed to initialise and clear memory*/
#include "INITIALIZE_MCMC_OUTPUT.c"

/*Hybrid affine-invariant / DE-MCZS sampler.
 *
 * For the first fADAPT fraction of the run, this follows AFDEMCMC and uses
 * STEP_AFDEMCMC. After that switch point, it uses DEMCMCZS proposals from an
 * archive. The archive is initialized like DEMCMCZS and is grown throughout
 * the whole run, including the affine phase, so DEMCMCZS can use the affine
 * exploration history after handoff.
 */
double *AFDEMCMCZS(
double (MODEL_LIKELIHOOD)(DATA, double *),
DATA DATA, PARAMETER_INFO PI, MCMC_OPTIONS MCO, MCMC_OUTPUT *MCOUT){

printf("CARDAMOM: Successfully entered AFDEMCMCZS!\n"); fflush(stdout);

/*ERASING PREVIOUS FILE IF APPEND == 0 */
if(MCO.APPEND==0 && MCO.nWRITE>0){FILE *fileout=fopen(MCO.outfile,"wb");fclose(fileout);}

int NC=MCO.nchains;
int initialNC=NC;
int demNC=10;
if (demNC>initialNC){demNC=initialNC;}

/*DEMCMCZS archive constants, matching DEMCMCZS.c*/
int M0=10*PI.npars;
if (M0<NC){M0=NC;}
int K=10;
double psnooker=0.10;

int switch_iter=(int)((double)MCO.nOUT*MCO.fADAPT);
if (switch_iter<0){switch_iter=0;}
if (switch_iter>MCO.nOUT){switch_iter=MCO.nOUT;}

/*The archive keeps all affine-phase 400-chain history, then only appends the
 *10 live DEMCMCZS chains after the switch.*/
int max_affine_appends=switch_iter/K+2;
int max_demcmczs_appends=(MCO.nOUT-switch_iter)/K+2;
int maxM=M0+initialNC*max_affine_appends+demNC*max_demcmczs_appends;

double *Z=calloc((size_t)maxM*PI.npars,sizeof(double));
double *PARS=calloc(PI.npars*NC,sizeof(double));
double *pars_new=calloc(PI.npars,sizeof(double));
double *P=calloc(NC,sizeof(double));
double *BESTPARS=calloc(PI.npars*NC,sizeof(double));
double *BESTP=calloc(NC,sizeof(double));

int n,nn,M=M0;
double par;

/*Initialize live chains from PI.parini, same convention as AFDEMCMC/DEMCMCZS.*/
for (nn=0;nn<NC;nn++){
for (n=0;n<PI.npars;n++){
if (MCO.randparini==1 && PI.parfix[n]!=1){
par=nor2par((double)random()/(double)RAND_MAX,PI.parmin[n],PI.parmax[n]);
}else{
par=PI.parini[n+nn*PI.npars];
if (par>PI.parmax[n] || par<PI.parmin[n]){printf("Warning, prescribed initial parameters are out of range\n");}
}
PARS[nn*PI.npars+n]=par;
Z[nn*PI.npars+n]=par;
}}

/*Fill the rest of the initial archive with random prior draws, matching DEMCMCZS.c.*/
for (nn=NC;nn<M0;nn++){
for (n=0;n<PI.npars;n++){
if (PI.parfix[n]==1){par=PI.parini[n];}
else{par=nor2par((double)random()/(double)RAND_MAX,PI.parmin[n],PI.parmax[n]);}
Z[nn*PI.npars+n]=par;
}}

oksofar("Established PI.parini - beginning AFDEMCMCZS now");

memcpy(BESTPARS,PARS,PI.npars*NC*sizeof(double));

/*STEP 1 - RUN MODEL WITH INITIAL PARAMETERS*/
for (nn=0;nn<NC;nn++){
P[nn]=MODEL_LIKELIHOOD(DATA,&PARS[nn*PI.npars]);
if (isnan(P[nn])){printf("Warning: MLF generated NaN... treating as -Inf");P[nn]=log(0);}
if (isinf(P[nn])==-1){printf("WARNING! P(0)=-inf - AFDEMCMCZS may get stuck - if so, please check your initial conditions\n");}
}
memcpy(BESTP,P,NC*sizeof(double));

double P_new,lr,gratio;
int withinrange,wrlocal=0;

COUNTERS N;
N.ACC=0;
N.ITER=MCO.nSTART;
N.ACCLOC=0;
N.ACCRATE=0;

printf("AFDEMCMCZS: affine phase ends at iteration %d out of %d\n",switch_iter,MCO.nOUT);
printf("AFDEMCMCZS: switching from %d affine chains to %d DEMCMCZS chains\n",initialNC,demNC);

int activeNC=initialNC;
int switched_to_demcmczs=0;

/*STEP 2 - BEGIN MCMC*/
for ( ;N.ITER<MCO.nOUT;N.ITER++){

if (!switched_to_demcmczs && N.ITER>=switch_iter){
double *PARS_SMALL=calloc(PI.npars*demNC,sizeof(double));
double *BESTPARS_SMALL=calloc(PI.npars*demNC,sizeof(double));
double *P_SMALL=calloc(demNC,sizeof(double));
double *BESTP_SMALL=calloc(demNC,sizeof(double));
int *selected=calloc(demNC,sizeof(int));
double *rankP=calloc(initialNC,sizeof(double));
for (nn=0;nn<initialNC;nn++){rankP[nn]=P[nn];}

int k,ii,best_idx;
for (k=0;k<demNC;k++){
best_idx=0;
double best_val=rankP[0];
for (ii=1;ii<initialNC;ii++){
if (rankP[ii]>best_val){best_val=rankP[ii];best_idx=ii;}}
selected[k]=best_idx;
P_SMALL[k]=P[best_idx];
BESTP_SMALL[k]=BESTP[best_idx];
for (n=0;n<PI.npars;n++){
PARS_SMALL[k*PI.npars+n]=PARS[best_idx*PI.npars+n];
BESTPARS_SMALL[k*PI.npars+n]=BESTPARS[best_idx*PI.npars+n];}
rankP[best_idx]=log(0);
}

for (k=0;k<demNC;k++){
P[k]=P_SMALL[k];
BESTP[k]=BESTP_SMALL[k];
for (n=0;n<PI.npars;n++){
PARS[k*PI.npars+n]=PARS_SMALL[k*PI.npars+n];
BESTPARS[k*PI.npars+n]=BESTPARS_SMALL[k*PI.npars+n];}
}

printf("AFDEMCMCZS: selected DEMCMCZS live-chain source indices:");
for (k=0;k<demNC;k++){printf(" %d(%2.1f)",selected[k],P_SMALL[k]);}
printf("\n");

free(PARS_SMALL);
free(BESTPARS_SMALL);
free(P_SMALL);
free(BESTP_SMALL);
free(selected);
free(rankP);

activeNC=demNC;
switched_to_demcmczs=1;
}

for (nn=0;nn<activeNC;nn++){

gratio=0;
if (N.ITER<switch_iter){
withinrange=STEP_AFDEMCMC(PARS,pars_new,PI,nn,activeNC,&gratio);
}else{
if ((double)random()/(double)RAND_MAX<psnooker){
withinrange=STEP_DEMCMCZ_SNOOKER(&PARS[nn*PI.npars],Z,M,pars_new,PI,&gratio);
}else{
withinrange=STEP_DEMCMCZ_PARALLEL(&PARS[nn*PI.npars],Z,M,pars_new,PI);
}
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
for (n=0;n<PI.npars;n++){PARS[n+nn*PI.npars]=pars_new[n];}
if (P_new>=BESTP[nn]){
for (n=0;n<PI.npars;n++){BESTPARS[n+nn*PI.npars]=pars_new[n];}
BESTP[nn]=P_new;}
P[nn]=P_new;
}
}

/*Regularly write results.*/
if (MCO.nWRITE>0 && (N.ITER % MCO.nWRITE)==0){
MCMC_OPTIONS MCO_WRITE=MCO;
MCO_WRITE.nchains=activeNC;
WRITE_DEMCMC_RESULTS(PARS,PI,MCO_WRITE,N.ITER);}

/*Regularly write restart file.*/
if (MCO.nWRITE>0 && (N.ITER % 1000)==0){
MCMC_OPTIONS MCO_WRITE=MCO;
MCO_WRITE.nchains=activeNC;
WRITE_DEMCMC_RESTART(PARS,PI,MCO_WRITE,N.ITER);}

/*Append current chain states to the DEMCMCZS archive during both phases.*/
if ((N.ITER+1) % K==0 && M+activeNC<=maxM){
for (nn=0;nn<activeNC;nn++){
for (n=0;n<PI.npars;n++){Z[(M+nn)*PI.npars+n]=PARS[nn*PI.npars+n];}}
M=M+activeNC;
}

/*Printing Info*/
if (MCO.nPRINT>0 && N.ITER % MCO.nPRINT==0){
printf("%d out of %d iterations (archive size = %d out of %d)\n",N.ITER,MCO.nOUT,M,maxM);
printf("AFDEMCMCZS phase = %s\n",(N.ITER<switch_iter) ? "affine" : "DEMCMCZS");
printf("active chains = %d\n",activeNC);
printf("within range = %2.2f%%\n",wrlocal/((double)(N.ITER+1)*activeNC)*100);
printf("Local Acceptance rate %5.1f%%\n",100*(double)N.ACC/((double)(N.ITER+1)*activeNC));
printf("Log Likelihoods: ");
for (nn=0;nn<activeNC;nn++){printf("%2.1f ",P[nn]);}
printf("\n");
}

}

/*filling in MCOUT details*/
for (n=0;n<PI.npars*activeNC;n++){MCOUT->best_pars[n]=BESTPARS[n];}
MCOUT->complete=1;

free(BESTPARS);
free(BESTP);
free(Z);
free(PARS);
free(pars_new);
free(P);
printf("AFDEMCMCZS DONE\n");

return 0;
}
