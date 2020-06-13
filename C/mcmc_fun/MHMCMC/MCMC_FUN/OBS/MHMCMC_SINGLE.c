#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../../../math_fun/std.c"
#include "NORMPARS.c"
#include "ADAPT_STEP_SIZE.c"
#include "STEP.c"





double *MHMCMC(
double (*MODEL_LIKELYHOOD)(DATA, PARAMETER_INFO, double *, double),
DATA DATA, PARAMETER_INFO PI, MCMC_OPTIONS MCO, double *PARS)
{

/* ***********INPUTS************
 *
 * MODEL_LIKELYHOOD: A function wholly responsible for 
 * (a) running the model given the DATA and parameters, 
 * (b) comparing it to observations,and 
 * (c) returning  the likelyhood.
 * The function will be run as MODEL_LIKELYHOOD(DATA,PI,PARS);
 * To facilitate this, ALL data can be 
 * passed to the MHMCMC function as a structure (in order to avoid 
 * repeated read/write computational time). 
 *
 * DATA: All data needed for the MODEL_LIKELYHOOD. It can include
 * drivers, observations, etc.
 *
 * PARINFO: This structure contains information on 
 * (a) pmin, pmax:	parameter ranges (compulsory)
 * (b) initpars:	parameter starting values (optional/recommended).
 * (c) npars:		number of pars
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
 * RESULTS FILE: File includes (a) results (b) likelyhood and (c) final step size
 *
 * */


/*ERASING PREVIOUS FILE IF APPEND == 0 */
if(MCO.APPEND==0){FILE *fileout=fopen(MCO.outfile,"wb");fclose(fileout);}



/*DECLARING*/
double P0,P;
int n;

COUNTERS N;
N.ACC=0;
N.ITER=0;
N.ACCLOC=0;
N.ACCRATE=0;
/*New and default parameter vectors*/
double *PARS0, *PARSALL;
PARS0=calloc(PI.npars,sizeof(double));
for (n=0;n<PI.npars;n++){PARS0[n]=PI.parini[n];}
PARS=calloc(PI.npars,sizeof(double));
/*All accepted parameters*/
PARSALL=calloc(MCO.nOUT*PI.npars,sizeof(double));



/*STEP 1 - RUN MODEL WITH INITIAL PARAMETERS*/
P0=MODEL_LIKELYHOOD(DATA,PI,PI.parini);
printf("starting log likelyhood = %e\n",P0);



/*STEP 2 - BEGIN MCMC*/
while (N.ACC < MCO.nOUT){
	/*take a step*/
	STEP(PARS0,PARS,PI);
	/*Calculate new likelyhood*/
	P=MODEL_LIKELYHOOD(DATA,PI,PARS);
	if (P-P0>log((double)random()/RAND_MAX)){
		/*storing accepted solution*/
		for (n=0;n<PI.npars;n++){
		PARSALL[N.ACC*PI.npars+n]=PARS[n];
		PARS0[n]=PARS[n];}
		N.ACC=N.ACC+1;N.ACCLOC=N.ACCLOC+1;
		P0=P;
		/*writing results*/
		if (N.ACC % MCO.nWRITE==0){WRITE_RESULTS(PARS,P,PI,MCO);}}
	
	/*Continuing in any case*/
	N.ITER=N.ITER+1;
	

	/*Adapting Step Size*/
	if (N.ITER % MCO.nADAPT==0){
		N.ACCRATE=(double)N.ACCLOC/MCO.nADAPT;
		
		/*Adapting step size*/
		if (MCO.fADAPT*(double)MCO.nOUT<(double)N.ACC){
		ADAPT_STEP_SIZE(PARSALL,PI,N,MCO);}
		
		N.ACCLOC=0;}
	
	/*Printing Info to Screen*/
	if (MCO.nPRINT>0 && N.ITER % MCO.nPRINT==0){
		printf("Total Accepted = %d out of %d\n",N.ACC,MCO.nOUT);
		printf("Local Acceptance rate %5.2f\n%",N.ACCRATE*100);
		printf("Likelyhood %e\n",P0);}

	/*END OF WHILE LOOP*/
	}
	
	
free(PARS);
free(PARS0);
printf("MHMCMC DONE\n");
return 0;	
/*END OF MHMCMC*/
}
