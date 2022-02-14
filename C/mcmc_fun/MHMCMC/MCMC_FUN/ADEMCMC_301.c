#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../../../math_fun/std.c"
#include "NORMPARS.c"
#include "STEP_DEMCMC.c"
#include "STEP_ADEMCMC.c"
#include "WRITE_DEMCMC_RESULTS.c"

/*here including additional functions needed to initialise and clear memory*/
#include "INITIALIZE_MCMC_OUTPUT.c"
/*Same as MHMCMC, except following differences:*/
/*N chains (N>=3)*/

double *ADEMCMC_301(
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
double *P, P_new;
int NC=MCO.nchains;/*[DEMCMC] Number of chains*/
P=calloc(NC,sizeof(double));

NC=MCO.nchains;

/*initialising P as -inf */
double Pmin=0;
int n=0,nn=0,m=0,withinrange,wrlocal=0;

COUNTERS N;
N.ACC=0;
N.ITER=0;
N.ACCLOC=0;
N.ACCRATE=0;
/*New and default parameter vectors*/
double *PARS,*pars_new,*BESTPARS, *BESTP;
PARS=calloc(PI.npars*NC,sizeof(double));
pars_new=calloc(PI.npars,sizeof(double));
BESTPARS=calloc(PI.npars*NC,sizeof(double));
PARS0=calloc(PI.npars*NC,sizeof(double));
BESTP=calloc(NC,sizeof(double));
/*All accepted parameters*/
/*This is now the last N parameter vectors
 * where N is the adaptation frequency*/
/*PARSALL is only used for adaptation*/
/*PARSALL=calloc(MCO.nADAPT*PI.npars*NC,sizeof(double));*//*[DEMCMC: can comment this out]*/

/*Random starting parameters if MCO.randparini*/
double par;
for (nn=0;nn<NC;nn++){
for (n=0;n<PI.npars;n++){
/*if MCO.fixedpars=0*/
/*if (MCO.fixedpars!=1){PI.parfix[n]=0;}*/
/*ONLY assigning randompars if (a) randparini==1 or (b) PI.parini[n]=-9999*/

if (MCO.randparini==1 && PI.parfix[n]!=1){
/*random parameter if PI.parini = -9999*/
PARS[n+nn*PI.npars]=nor2par((double)random()/RAND_MAX,PI.parmin[n],PI.parmax[n]);}
else{par=PI.parini[n+nn*PI.npars];
PARS[n+nn*PI.npars]=par;
if (par>PI.parmax[n] | par<PI.parmin[n]){printf("Warning, prescribed initial parameters are out of range");}
}
}}


double gratio=0,lr;

/*
for (nn=0;nn<NC;nn++){
for (n=0;n<PI.npars;n++){
printf("%1.1e ",PARS[n+nn*PI.npars]);}
printf("\n");}
*/

oksofar("Established PI.parini - begining MHMCMC now");

memcpy(BESTPARS,PARS,PI.npars*NC*sizeof(double));

/*STEP 1 - RUN MODEL WITH INITIAL PARAMETERS*/
for (nn=0;nn<NC;nn++){
/*NOTE: passing pointer of PARS0 + N-chains: also use *(P+N) format if this one won't work*/
P[nn]=MODEL_LIKELIHOOD(DATA,&PARS[nn*PI.npars]);
/*treating NaN as -inf*/
if isnan(P[nn]){printf("Warning: MLF generated NaN... treating as -Inf");
P[nn]=log(0);}
if (Pmin>P[nn]){Pmin=P[nn];}


memcpy(BESTP,P,NC*sizeof(double));


//printf("starting likelihood for chain %i = %e\n",nn,P[nn]);

if (isinf(P[nn])==-1){printf("WARNING! P(0)=-inf - MHMCMC may get stuck - if so, please check your initial conditions\n");}}

/*STEP 2 - BEGIN MCMC*/
for (N.ITER=0;N.ITER<MCO.nOUT;N.ITER++){
	/*Looping through each chain*/
	/*UPDATE: retaining parameter vector (as done in ter Braak, 2006) and moving on to next chain if metropolis ratio is rejected*/
	
	//PI.stepsize[0]=1e-1-(1e-1-1)*(double)(n % 10 == 0);
	
    memcpy(PARS0,PARS,PI.npars*NC*sizeof(double));

    
	for (nn=0;nn<NC;nn++){

	//ADEMCMC
	if ((double)N.ITER<(double)MCO.nOUT*MCO.fADAPT){
        withinrange=STEP_ADEMCMC(PARS0,pars_new,PI,nn,NC,&gratio);
	}
	//Standard DEMCMC
	else {
	/*Step size is 1 wigth 10% prob iterations*/
        PI.stepsize[0]=1 - (1-2.38/sqrt(2*PI.npars)/10)*(double)((double)(random()/RAND_MAX)<0.9);
	/*take a step (DE-MCMC style)*/
	PI.stepsize[0]=PI.stepsize[0];
	withinrange=STEP_DEMCMC(PARS0,pars_new,PI,nn,NC);
	gratio=0;
	}

	
	lr=log((double)random()/RAND_MAX);
	/*p(x) = 0 if parameters outside bounds*/
	if (withinrange==1 & -P[nn]+gratio>lr){
wrlocal=wrlocal+1;
	/*Calculate new likelihood*/
	P_new=MODEL_LIKELIHOOD(DATA,pars_new);}
        else
	{P_new=log(0);}
        //printf("P_new = %2.2f\n, PI.stepsize = %2.2f\n",P_new, PI.stepsize[0]);
	/*if (isinf(P_new)==0){oksofar("Found non-inf solution");}
	*/
	/*treating nans as -inf*/
	if isnan(P_new){P_new=log(0);}
if (P_new-P[nn]+gratio>lr || (isinf(P_new)==0 && isinf(P[nn]) && withinrange==1)){N.ACC=N.ACC+1;
	if (isinf(P_new)==0 && isinf(P[nn])){printf("pnew = %2.1f, p = %2.1f, (P_new-P[nn]) = %2.1f\n",P_new,P[nn],P_new-P[nn]);}


		for (n=0;n<PI.npars;n++){
		PARS[n+nn*PI.npars]=pars_new[n];}
	P[nn]=P_new;
		if (P_new>=BESTP[nn]){for (n=0;n<PI.npars;n++){BESTPARS[n + nn*PI.npars]=pars_new[n];BESTP[nn]=P_new;}
		if (P_new==0 && P_new>P[nn] ){printf("Found bestpars, prob = %2.1f, chain = %i\n",BESTP[nn],nn);}
		}
	}
	}
	
	/*regularly write results*/
	if (MCO.nWRITE>0 && (N.ITER % MCO.nWRITE)==0){
WRITE_DEMCMC_RESULTS(PARS,PI,MCO);}
	
	/*Printing Info to Screen*/
	if (MCO.nPRINT>0 && N.ITER % MCO.nPRINT==0){
		printf("%d out of %d iterations)\n",N.ITER,MCO.nOUT);
		printf("within range = %2.2f\n",wrlocal/((double)N.ITER*NC)*100);
		printf("Local Acceptance rate %5.1f\n %%",100*(double)N.ACC/((double)N.ITER*NC));
		printf("PI.stepsize = %5.5f\n",PI.stepsize[0]);
		printf("Log Likelihoods: ");
		for (nn=0;nn<NC;nn++){printf("%2.1f ",P[nn]);}
		printf("\n");
	}

/*End of chain loop*/
	/*END OF WHILE LOOP*/
	


	
	}
	
	

/*filling in MCOUT details*/
/*best parameter combination*/
//printf("bestpars\n");
for (n=0;n<PI.npars*NC;n++){MCOUT->best_pars[n]=BESTPARS[n];
//printf("MCOUT->best_pars[%i]=%2.1f",n,MCOUT->best_pars[n]);
}
/*MCMC completed*/
MCOUT->complete=1;
/*done with MCMC completion*/

free(BESTPARS);
free(BESTP);
free(PARS);
free(PARS0);
free(pars_new);
free(P);
printf("DEMCMC DONE\n");


return 0;

/*END OF MHMCMC*/
}
