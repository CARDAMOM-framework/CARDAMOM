#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../../../math_fun/std.c"
#include "NORMPARS.c"
#include "ADAPT_DEMCMC_STEP_SIZE.c"
#include "STEP_DEMCMC.c"
#include "WRITE_DEMCMC_RESULTS.c"

/*here including additional functions needed to initialise and clear memory*/
#include "INITIALIZE_MCMC_OUTPUT.c"
/*Same as MHMCMC, except following differences:*/
/*N chains (N>=3)*/

double *ECMCMC(
double (MODEL_LIKELIHOOD)(DATA, double *),
DATA DATA, PARAMETER_INFO PI, MCMC_OPTIONS MCO, MCMC_OUTPUT *MCOUT){

/* ***********INPUTS************
 *
 * MODEL_LIKELIHOOD: A function wholly responsible for 
 * (a) running the model given the DATA and parameters, 
 * (b) comparing it to observations,and 
 * (c) returning  the (log) likelihood.
 * The function will be run as MODEL_LIKELIHOOD(DATA,PARS);
 * To facilitate this, ALL data can be 
 * passed to the MHMCMC function as a structure (in order to avoid 
 * repeated read/write computational time). 
 *
 * DATA: All data needed for the MODEL_LIKELIHOOD. It can include
 * drivers, observations, etc.
 *
 * PARINFO: This structure contains information on 
 * (a) pmin, pmax:	parameter ranges (compulsory)
 * (b) initpars:	parameter starting values (optional/recommended).
 * (c) npars:		number of pars (compulsory)
 *
 * MCO: This structure contains option values for the MCMC run.
 * These will be set to default values if empty. Options include:
 * (a) number of runs
 * (b) filename for writing file with results
 * (c) step adaptation frequency
 * (d) initial step size
 * */

/* **************OUTPUTS*************
 * 
 * RESULTS FILE: File includes (a) results (b) likelihood and (c) final step size
 *
 * */



/*NOTE: seeding must happen outside of the MHMCMC function*/
/*if internal seeding is needed, use srandom(time(0));*/
/*however this may result in repeat numbers over short timespan*/


/*ERASING PREVIOUS FILE IF APPEND == 0 */
if(MCO.APPEND==0 && MCO.nWRITE>0){FILE *fileout=fopen(MCO.outfile,"wb");fclose(fileout);}



/*DECLARING*/
double *P0, *P;
int NC=MCO.nchains;/*[DEMCMC] Number of chains*/
P0=calloc(NC,sizeof(double));
P=calloc(NC,sizeof(double));

NC=MCO.nchains;

/*initialising P as -inf */
double Pmin=0;
int n=0,nn=0,m=0;

COUNTERS N;
N.ACC=0;
N.ITER=0;
N.ACCLOC=0;
N.ACCRATE=0;
/*New and default parameter vectors*/
double *PARS,*PARS0,*BESTPARS;
PARS0=calloc(PI.npars*NC,sizeof(double));
PARS=calloc(PI.npars*NC,sizeof(double));
BESTPARS=calloc(PI.npars*NC,sizeof(double));
/*All accepted parameters*/
/*This is now the last N parameter vectors
 * where N is the adaptation frequency*/
/*PARSALL is only used for adaptation*/
/*PARSALL=calloc(MCO.nADAPT*PI.npars*NC,sizeof(double));*//*[DEMCMC: can comment this out]*/

/*Random starting parameters if MCO.randparini*/
for (nn=0;nn<NC;nn++){
for (n=0;n<PI.npars;n++){
/*if MCO.fixedpars=0*/
if (MCO.fixedpars!=1){PI.parfix[n]=0;}
/*ONLY assigning randompars if (a) randparini==1 or (b) PI.parini[n]=-9999*/

if (MCO.randparini==1 && PI.parfix[n]!=1){
/*random parameter if PI.parini = -9999*/
PARS0[n+nn*PI.npars]=nor2par((double)random()/RAND_MAX,PI.parmin[n],PI.parmax[n]);}
else
{PARS0[n+nn*PI.npars]=PI.parini[n+nn*PI.npars];}}}


/*Defining default PCArotation here*/
/*if (PI.PCArotation==NULL){
for (m=0;m<PI.npars;m++){
for (m=0;m<PI.npars;m++){
PI.PCArotation[m][n]
}
}
} */

oksofar("Established PI.parini - begining MHMCMC now");

memcpy(BESTPARS,PARS0,PI.npars*sizeof(double));

/*STEP 1 - RUN MODEL WITH INITIAL PARAMETERS*/
for (nn=0;nn<NC;nn++){
/*NOTE: passing pointer of PARS0 + N-chains: also use *(P+N) format if this one won't work*/
P0[nn]=MODEL_LIKELIHOOD(DATA,&PARS0[nn*PI.npars]);
/*treating NaN as -inf*/
if isnan(P0[nn]){printf("Warning: MLF generated NaN... treating as -Inf");
P0[nn]=log(0);}
if (Pmin>P0[nn]){Pmin=P0[nn];}




printf("starting likelihood for chain %i = %e\n",nn,P0[nn]);

if (isinf(P0[nn])==-1){printf("WARNING! P0=-inf - MHMCMC may get stuck - if so, please check your initial conditions\n");}}

/*STEP 2 - BEGIN MCMC*/
while (N.ACC < MCO.nOUT && (Pmin<0 || MCO.nWRITE>0 || N.ACC==0)){

	/*Looping through each chain*/
	nn=0;
	while (nn<NC){
	/*take a step (DE-MCMC style)*/
	/*CONTINUE FROM HERE [ensure DE_STEP works]*/
	STEP_DEMCMC(PARS0,PARS,PI,nn,NC);
	/*Calculate new likelihood*/
	P[nn]=MODEL_LIKELIHOOD(DATA,PARS+nn*PI.npars);/*
	printf("nn = %i, P[nn] = %2.2e, P0[nn] = %2.2e, PARS+nn*PI.npars = %2.2e\n",nn,P[nn],P0[nn],*(PARS+nn*PI.npars));
	*/
	/*treating nans as -inf*/
	if isnan(P[nn]){printf("Warning: MLF generated NaN... treating as -Inf");P[nn]=log(0);}
	if (P[nn]-P0[nn]>log((double)random()/RAND_MAX)){nn=nn+1;N.ACC=N.ACC+1;}

	/*Continuing in any case*/
	N.ITER=N.ITER+1;
	
	}
	
	/*Done with sampling all chains*/
		
	/*storing accepted solution*/
	/*for DEMCMC, that's each sample from each chain*/
	for (nn=0;nn<NC;nn++){
	for (n=0;n<PI.npars;n++){
	PARS0[n+nn*PI.npars]=PARS[n+nn*PI.npars];}
	if (P[nn]>P0[nn]){BESTPARS[n]=PARS[n];}
	P0[nn]=P[nn];}
	
		/*writing results: parameters and log likelihood*/
                /*writing ALL results! Changed on Sat 30 Nov 2013*/
	if (MCO.nWRITE>0 && (N.ACC % MCO.nWRITE)==0){WRITE_DEMCMC_RESULTS(PARS,PI,MCO);}
	
	/*Continue from here*/
	/*Adapting Step Size*/
	if (N.ACC % MCO.nADAPT==0){
		N.ACCRATE=(double)N.ACC/(double)N.ITER;
		/*Adapting step size*/
		if (MCO.fADAPT*(double)MCO.nOUT>(double)N.ACC){
		ADAPT_DEMCMC_STEP_SIZE(PI, N);}

		/*counting loops with zero accepted parameters*/
		
		
		N.ACCLOC=0;}
	/*Printing Info to Screen*/
	if (MCO.nPRINT>0 && N.ACC % MCO.nPRINT==0){
		printf("Total Accepted = %d out of %d\n",N.ACC,MCO.nOUT);
		printf("Local Acceptance rate %5.1f\n %%",N.ACCRATE*100);
		printf("PI.stepsize = %5.5f\n",PI.stepsize[0]);
		printf("Log Likelihoods: ");
		for (nn=0;nn<NC;nn++){printf("%2.3f",P0[nn]);}
		printf("\n");



/*End of chain loop*/
}

	/*END OF WHILE LOOP*/
	


	
	}
	
	

/*filling in MCOUT details*/
/*best parameter combination*/
for (n=0;n<PI.npars;n++){MCOUT->best_pars[n]=BESTPARS[n];}
/*MCMC completed*/
MCOUT->complete=1;
/*done with MCMC completion*/

free(BESTPARS);
free(PARS);
free(PARS0);
free(P);
free(P0);
printf("MHMCMC DONE\n");


return 0;

/*END OF MHMCMC*/
}
