
#pragma once
#include "../../../COST_FUNCTION/MODEL_LIKELIHOOD_FUNCTIONS/DALEC_MLF.c"
//#include "../../../COST_FUNCTION/MODEL_LIKELIHOOD_FUNCTIONS/DALEC_MLF_beta.c"
#include "../../../../mcmc_fun/MHMCMC/MCMC_FUN/MHMCMC_119.c"
#include "../../../../mcmc_fun/MHMCMC/MCMC_FUN/DEMCMC.c"
#include "../../../../mcmc_fun/MHMCMC/MCMC_FUN/ADEMCMC.c"
#include "../../../../mcmc_fun/MHMCMC/MCMC_FUN/AFDEMCMC.c"
#include "../../../../math_fun/int_max.c"

int FIND_EDC_INITIAL_VALUES(DATA CARDADATA,PARAMETER_INFO *PI, MCMC_OPTIONS *MCOPT_CARDAMOM){

/*First: choosing the correct EDC MODEL LIKELIHOOD FUNCTION (EMLF)*/


    printf("*********made it to here FIND_EDC_INITIAL_VALUES********\n");

//double (*EMLF)(DATA, double *);
//double (*MLF)(DATA, double *);


//EMLF=EDC_DALEC_MLF;
//MLF=DALEC_MLF;

/*This MCMC is designed to find the best-fit DALEC parameters ONLY*/

MCMC_OPTIONS MCOPT;
MCMC_OUTPUT MCOUT;


int PEDCC,nn;


//option for mcmcid = 3
int nstartchains=10;
        
        
MCOPT.APPEND=0;
MCOPT.nADAPT=10;/*was 20*/
MCOPT.fADAPT=0.5;
MCOPT.nOUT=1000;/*was 2000*/
MCOPT.nPRINT=100;/*was*/
MCOPT.nWRITE=0;
MCOPT.nSTART=0;
/*randparini = 0*/
/*this means all PI.parini values must either be given values or entered as -9999*/
MCOPT.randparini=1;
MCOPT.returnpars=1;
/*setting fixedpars option to 1*/
MCOPT.fixedpars=1;
MCOPT.mcmcid=119;/*Using metropolis-hastings to find initial parameters*/
MCOPT.nchains=1;
MCOPT.minstepsize=1e-2;


if (MCOPT_CARDAMOM->mcmcid==3){
MCOPT.mcmcid=3;
default_int_value(&CARDADATA.ncdf_data.MCMCID.nSAMPLES_EDC_SEARCH ,200000);
MCOPT.nOUT=CARDADATA.ncdf_data.MCMCID.nSAMPLES_EDC_SEARCH ;/*Default =  20000*/
MCOPT.nPRINT=2000;/*1;was 2000*/
MCOPT.minstepsize=1e-5;
MCOPT.nchains=400;
MCOPT.fixedpars=0;
MCOPT.fADAPT=0;
//declaring best_pars
MCOUT.best_pars=calloc(MCOPT.nchains*PI->npars,sizeof(double));}


if (MCOPT_CARDAMOM->mcmcid==4){
MCOPT.mcmcid=4;
default_int_value(&CARDADATA.ncdf_data.MCMCID.nSAMPLES_EDC_SEARCH ,200000);
MCOPT.nOUT=CARDADATA.ncdf_data.MCMCID.nSAMPLES_EDC_SEARCH ;/*Default =  20000*/
MCOPT.nPRINT=2000;/*1;was 2000*/
MCOPT.minstepsize=1e-5;
MCOPT.nchains=400;
MCOPT.fixedpars=0;
MCOPT.fADAPT=0;
//declaring best_pars
MCOUT.best_pars=calloc(MCOPT.nchains*PI->npars,sizeof(double));}


/*mode 5 (DEMCMCZS): reuse AFDEMCMC (a proven multi-chain searcher, same as
 *modes 3/4) for the EDC search rather than DEMCMCZS itself, since EDCs are
 *hard feasibility constraints, not the smooth-posterior problem DEMCMCZS is
 *designed for. Uses 400 chains like modes 3/4 - a real production run showed
 *even 400 chains with a lenient threshold took ~84 minutes to converge here,
 *so matching production's 3 chains directly would be far too chain-starved.
 *Since production nchains (e.g. 3) is smaller than the search's 400,
 *PI->parini (allocated by the caller at only npars*production_nchains) is
 *temporarily grown to npars*400 for the duration of the search, then shrunk
 *back down after picking the best production_nchains (by real likelihood,
 *among EDC-passing chains) - see the selection block just after the while
 *loop below.*/
if (MCOPT_CARDAMOM->mcmcid==5){
MCOPT.mcmcid=5;
default_int_value(&CARDADATA.ncdf_data.MCMCID.nSAMPLES_EDC_SEARCH ,200000);
MCOPT.nOUT=CARDADATA.ncdf_data.MCMCID.nSAMPLES_EDC_SEARCH ;
MCOPT.nPRINT=2000;
MCOPT.minstepsize=1e-5;
MCOPT.nchains=400;
MCOPT.fixedpars=0;
MCOPT.fADAPT=0;
//declaring best_pars
MCOUT.best_pars=calloc(MCOPT.nchains*PI->npars,sizeof(double));
//growing PI->parini to fit 400 chains for the search (caller only allocated
//enough for MCOPT_CARDAMOM->nchains) - shrunk back after selection, below
free(PI->parini);
PI->parini=calloc(MCOPT.nchains*PI->npars,sizeof(double));}



int OK=INITIALIZE_MCMC_OUTPUT(*PI,&MCOUT,MCOPT);
printf("C/projects/CARDAMOM_MDF/MCMC_SETUP/PROJECT_FUN/FIND_EDC_INITIAL_VALUES.c: MCOUT structure initialized\n");



int n,nnn;

printf("PI->npars (INSIDE FIND_EDC_INITIAL_VALUES.c)= %d\n",PI->npars);




for (n=0;n<PI->npars;n++){
PI->stepsize[n]=0.02;
/*PI->stepsize[n]=0.00005;*/
PI->parini[n]=DEFAULT_DOUBLE_VAL;
PI->parfix[n]=0;
/*
if (PI->parini[n]!=-9999 & CARDADATA.edc_random_search<1) {PI->parfix[n]=1;}*/}




/*done*/

double PEDC=log(0);
int count=0;
while (PEDC!=0){
	printf("EDC Attempt no %d\n",count);oksofar("---");

	for (n=0;n<PI->npars;n++){PI->stepsize[n]=0.0005;}
	/*insert prior value option here!*/

	oksofar("Running short MCMC to find x_{EDC} = 1");
    
	if (MCOPT.mcmcid==119){MHMCMC_119(CARDADATA.EMLF,CARDADATA,*PI,MCOPT,&MCOUT);};
        if (MCOPT.mcmcid==2){DEMCMC(CARDADATA.EMLF,CARDADATA,*PI,MCOPT,&MCOUT);};
        if (MCOPT.mcmcid==3){ADEMCMC(CARDADATA.EMLF,CARDADATA,*PI,MCOPT,&MCOUT);};
        if (MCOPT.mcmcid==4){ADEMCMC(CARDADATA.EMLF,CARDADATA,*PI,MCOPT,&MCOUT);};
        if (MCOPT.mcmcid==5){AFDEMCMC(CARDADATA.EMLF,CARDADATA,*PI,MCOPT,&MCOUT);};

	/*if (MCOPT.mcmcid==2){DEMCMC(EMLF,CARDADATA,*PI,MCOPT,&MCOUT);};
	*/
	oksofar("Short MCMC complete");
	for (n=0;n<PI->npars*MCOPT.nchains;n++){PI->parini[n]=MCOUT.best_pars[n];}

	PEDCC=0;
	for (nn=0;nn<MCOPT.nchains;nn++){
	PEDC=CARDADATA.EMLF(CARDADATA, PI->parini + nn*PI->npars);double P;
	P=CARDADATA.MLF(CARDADATA, PI->parini + nn*PI->npars);
	printf("PEDC for chain %i = %2.1f (%2.1f)\n",nn,PEDC,P);
	if (PEDC==0){PEDCC=PEDCC+1;}}

	
	printf("*******\n");
	printf("*******\n");
	printf("%i out of %i chains have non-zero prob\n",PEDCC,MCOPT.nchains);
    printf("EDC stats\n");
    for (nnn=0;nnn<CARDADATA.noedcs;nnn++){
        double prcnt=100*(double)CARDADATA.EDC_PASS_COUNTER[nnn]/(double)CARDADATA.EDC_INSTANCE_COUNTER[nnn];
printf("EDC no %i; attempts = %i; passes = %i (%2.2f%%);\n",nnn,CARDADATA.EDC_INSTANCE_COUNTER[nnn],CARDADATA.EDC_PASS_COUNTER[nnn],prcnt );
    }
	printf("*******\n");
	printf("*******\n");

	
	count=count+1;
	
	if (MCOPT.mcmcid==2 && PEDCC>MCOPT.nchains){PEDC=0;}
	//Guarantee that at least half of chains have non-zero starting probabilities
	if (MCOPT.mcmcid==3){if (PEDCC>nstartchains){PEDC=0;}else{PEDC=-1;}}
	if (MCOPT.mcmcid==4){if (PEDCC>nstartchains){PEDC=0;}else{PEDC=-1;}}
	//mode 5 uses 400 chains for the search (same as 3/4), so use the same
	//convergence threshold - only a handful need to actually pass; the best
	//MCOPT_CARDAMOM->nchains of them get selected for production below
	if (MCOPT.mcmcid==5){if (PEDCC>nstartchains){PEDC=0;}else{PEDC=-1;}}
	if (MCOPT.mcmcid==2 || MCOPT.mcmcid==3 || MCOPT.mcmcid==4 || MCOPT.mcmcid==5){MCOPT.randparini=0;}
	/*Hard coding*/
	
	/*in case one EDC missing*/
	if (MCOPT.mcmcid==119 && PEDC!=0 && count%3==0){for (n=0;n<PI->npars;n++){PI->parini[n]=CARDADATA.parpriors[n];}}

}

/*mode 5: pick the best MCOPT_CARDAMOM->nchains (production chain count) chains
 *out of the 400 searched, ranked by real likelihood among those passing EDCs,
 *then shrink PI->parini back down to that size - the production DEMCMCZS run
 *only ever sees this final, small, EDC-satisfying set of starting points.*/
if (MCOPT_CARDAMOM->mcmcid==5){
int nprod=MCOPT_CARDAMOM->nchains;
double *chainP=calloc(MCOPT.nchains,sizeof(double));
int nn2;
for (nn2=0;nn2<MCOPT.nchains;nn2++){
double edc_p=CARDADATA.EMLF(CARDADATA, PI->parini + nn2*PI->npars);
if (edc_p==0){chainP[nn2]=CARDADATA.MLF(CARDADATA, PI->parini + nn2*PI->npars);}
else{chainP[nn2]=log(0);}
}

double *parini_small=calloc(nprod*PI->npars,sizeof(double));
int k,best_idx;
for (k=0;k<nprod;k++){
best_idx=0;
double best_val=chainP[0];
for (nn2=1;nn2<MCOPT.nchains;nn2++){
if (chainP[nn2]>best_val){best_val=chainP[nn2];best_idx=nn2;}}
for (n=0;n<PI->npars;n++){parini_small[k*PI->npars+n]=PI->parini[best_idx*PI->npars+n];}
chainP[best_idx]=log(0);
}

free(chainP);
free(PI->parini);
PI->parini=parini_small;
MCOPT.nchains=nprod;
}else{
//Probs would make more sense as a memcpy but I am keeping it like this for now
for (n=0;n<PI->npars*MCOPT.nchains;n++){

	PI->parini[n]=MCOUT.best_pars[n];
}
}



/*Sampling new/more parameters*/

	/*
	for (n=0;n<PI->npars;n++){printf("%8.6f  ",PI->parini[n]);}printf("\n");
	printf("EDC Probability of starting parameters = %4.4f\n",EMLF(CARDADATA, PI->parini));
	printf("Probability of starting parameters = %4.4f\n",CARDADATA.MLF(CARDADATA, PI->parini));
	*/
	/*for (n=0;n<PI->npars;n++){PI->stepsize[n]=0.01;}*/
	
	/*SOON-TO-BE-OBSOLETE: resetting fixed pars to zero for main r*/
	//for (n=0;n<PI->npars;n++){PI->parfix[n]=0;}
	//THIS WAS MOVED TO MCMC_MODULES!

/*clearing MCOUT fields*/
free(MCOUT.best_pars);
/*Done either (a) reading parameters from file, or (b) sampling parameters
*/

return 0;}

