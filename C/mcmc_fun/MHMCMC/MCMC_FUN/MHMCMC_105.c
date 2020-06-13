#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../../../math_fun/std.c"
#include "NORMPARS.c"
#include "ADAPT_STEP_SIZE_105.c"
#include "STEP.c"
#include "STEP_WITH_BOUNCE.c"
#include "WRITERESULTS.c"

/*here including additional functions needed to initialise and clear memory*/
#include "INITIALIZE_MCMC_OUTPUT.c"



double *MHMCMC_105(
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
double P0;
/*initialising P as -inf */
double P=log(0);
int n=0,m=0;

COUNTERS N;
N.ACC=0;
N.ITER=0;
N.ACCLOC=0;
N.ACCRATE=0;
/*New and default parameter vectors*/
double *PARS,*PARS0, *PARSALL,*BESTPARS;
PARS0=calloc(PI.npars,sizeof(double));
PARS=calloc(PI.npars,sizeof(double));
BESTPARS=calloc(PI.npars,sizeof(double));
/*All accepted parameters*/
/*This is now the last N parameter vectors
 * where N is the adaptation frequency*/
/*PARSALL is only used for adaptation*/
PARSALL=calloc(MCO.nADAPT*PI.npars,sizeof(double));


double meanstepsize;

/*Random starting parameters if MCO.randparini*/
for (n=0;n<PI.npars;n++){
/*if MCO.fixedpars=0*/
if (MCO.fixedpars!=1){PI.parfix[n]=0;}
/*ONLY assigning randompars if (a) randparini==1 or (b) PI.parini[n]=-9999*/
/*BUG IS HERE!!!4-9-2013*/
if (MCO.randparini==1 && PI.parfix[n]!=1){
/*random parameter if PI.parini = -9999*/
PI.parini[n]=nor2par((double)random()/RAND_MAX,PI.parmin[n],PI.parmax[n]);}
/*printing parameter values*/
printf("log10(p%d)=%2.4f ",n+1,log10(PI.parini[n]));
if ((n+1) % 3==0){printf("\n");}
}


/*Defining default PCArotation here*/
/*if (PI.PCArotation==NULL){
for (m=0;m<PI.npars;m++){
for (m=0;m<PI.npars;m++){
PI.PCArotation[m][n]
}
}
} */

oksofar("Established PI.parini - begining MHMCMC now");

for (n=0;n<PI.npars;n++){PARS0[n]=PI.parini[n];}
memcpy(BESTPARS,PARS0,PI.npars*sizeof(double));

/*STEP 1 - RUN MODEL WITH INITIAL PARAMETERS*/
P0=MODEL_LIKELIHOOD(DATA,PI.parini);
/*treating NaN as -inf*/
if isnan(P0){printf("Warning: MLF generated NaN... treating as -Inf");P0=log(0);}

printf("starting likelihood = %e\n",P0);

if (isinf(P0)==-1){printf("WARNING! P0=-inf - MHMCMC may get stuck - if so, please check your initial conditions\n");}



/*STEP 2 - BEGIN MCMC*/
while (N.ACC < MCO.nOUT && (P<0 || MCO.nWRITE>0 || N.ACC==0)){
	/*take a step*/
	STEP_WITH_BOUNCE(PARS0,PARS,PI);
	/*Calculate new likelihood*/
	P=MODEL_LIKELIHOOD(DATA,PARS);
	/*treating nans as -inf*/
	if isnan(P){printf("Warning: MLF generated NaN... treating as -Inf");P=log(0);}


	if (P-P0>log((double)random()/RAND_MAX)){
		/*storing accepted solution*/
		for (n=0;n<PI.npars;n++){
		PARSALL[N.ACCLOC*PI.npars+n]=PARS[n];
		PARS0[n]=PARS[n];
		if (P>P0){BESTPARS[n]=PARS[n];}}
		
		N.ACC=N.ACC+1;N.ACCLOC=N.ACCLOC+1;
		P0=P;
		/*writing results: parameters and log likelihood*/
                /*writing ALL results! Changed on Sat 30 Nov 2013*/
		if (MCO.nWRITE>0 && (N.ACC % MCO.nWRITE)==0){WRITE_RESULTS(PARS,P,PI,MCO);}}
	
	/*Continuing in any case*/
	N.ITER=N.ITER+1;

	/*Adapting Step Size*/
	if (N.ITER % MCO.nADAPT==0){
		N.ACCRATE=(double)N.ACCLOC/(double)MCO.nADAPT;
		
		/*Adapting step size*/
		if (MCO.fADAPT*(double)MCO.nOUT>(double)N.ACC){
		ADAPT_STEP_SIZE_105(PARSALL,PI,N,MCO);}

		/*counting loops with zero accepted parameters*/
		
		
		N.ACCLOC=0;}
	/*Printing Info to Screen*/
	if (MCO.nPRINT>0 && N.ITER % MCO.nPRINT==0){
		printf("Total Accepted = %d out of %d\n",N.ACC,MCO.nOUT);
		printf("Local Acceptance rate %5.1f\n %%",N.ACCRATE*100);
		printf("Log Likelihood %e\n",P0);
		meanstepsize=0;
		for (n=0;n<PI.npars;n++){meanstepsize+=PI.stepsize[n]/(double)(PI.npars);}
		printf("Mean step size = %5.5f\n",meanstepsize);
		}
	/*END OF WHILE LOOP*/
	
	if (P0==0){printf("Found P=1 solution");break;}

	
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
free(PARSALL);
printf("MHMCMC DONE\n");


return 0;

/*END OF MHMCMC*/
}
