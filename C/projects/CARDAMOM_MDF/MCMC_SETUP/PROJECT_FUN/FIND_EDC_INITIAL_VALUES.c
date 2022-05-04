
#pragma once
#include "../../../COST_FUNCTION/MODEL_LIKELIHOOD_FUNCTIONS/DALEC_MLF.c"
//#include "../../../COST_FUNCTION/MODEL_LIKELIHOOD_FUNCTIONS/DALEC_MLF_beta.c"
#include "../../../../mcmc_fun/MHMCMC/MCMC_FUN/MHMCMC_119.c"
#include "../../../../mcmc_fun/MHMCMC/MCMC_FUN/DEMCMC.c"
#include "../../../../mcmc_fun/MHMCMC/MCMC_FUN/ADEMCMC.c"
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
MCOPT.nOUT=2000;/*1;was 2000*/
MCOPT.nPRINT=2000;/*1;was 2000*/
MCOPT.minstepsize=1e-5;
MCOPT.nchains=200;
MCOPT.fixedpars=0;
MCOPT.fADAPT=0;
//declaring best_pars
MCOUT.best_pars=calloc(MCOPT.nchains*PI->npars,sizeof(double));}


int OK=INITIALIZE_MCMC_OUTPUT(*PI,&MCOUT,MCOPT);
printf("C/projects/CARDAMOM_MDF/MCMC_SETUP/PROJECT_FUN/FIND_EDC_INITIAL_VALUES.c: MCOUT structure initialized\n");



int n;

printf("PI->npars (INSIDE FIND_EDC_INITIAL_VALUES.c)= %d\n",PI->npars);




for (n=0;n<PI->npars;n++){
PI->stepsize[n]=0.02;
/*PI->stepsize[n]=0.00005;*/
PI->parini[n]=DEFAULT_DOUBLE_VAL;
PI->parfix[n]=0;
/*
if (PI->parini[n]!=-9999 & CARDADATA.edc_random_search<1) {PI->parfix[n]=1;}*/}




/*Reading file (if available)*/
/*if number of parameters x number of chains available are contained in file, then OK*/
/*Otherwise search for new parameters*/

FILE *fileout0=fopen(MCOPT_CARDAMOM->startfile,"r");
int filelength;
if (fileout0!=NULL){
fseek(fileout0, 0, SEEK_END);filelength = ftell(fileout0)/sizeof(double);fclose(fileout0);}
else{filelength=0;}
/*Two choices:
(1) read parameters from file if these are sufficient
(2) sample/store parameter vectors from file otherwise*/


/*Sampling M=N-N0 chains, where N0 is the number of existing chains in the file*/
int m,M=MCOPT_CARDAMOM->nchains - filelength/PI->npars;
printf("Number of starting parameter vectors saved in file: %i\n",filelength/PI->npars);
printf("Number of required starting parameters vectors: %i\n", MCOPT_CARDAMOM->nchains);
printf("Number of starting parameter vectors to be sampled here: %i\n", int_max(M,0));



/*if (PI->npars*MCOPT_CARDAMOM->nchains>filelength){*/
if (PI->npars>filelength){
/*done*/
/*PEDC is the log likelihood*/
/*for (m=0;m<M;m++){
*/
for (m=0;m<1;m++){
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
	printf("*******\n");
	printf("*******\n");

	
	count=count+1;
	
	if (MCOPT.mcmcid==2 && PEDCC>MCOPT.nchains){PEDC=0;}
	//Guarantee that at least half of chains have non-zero starting probabilities
	if (MCOPT.mcmcid==3){if (PEDCC>nstartchains){PEDC=0;}else{PEDC=-1;}}
	if (MCOPT.mcmcid==2 || MCOPT.mcmcid==3){MCOPT.randparini=0;}	
	/*Hard coding*/
	
	/*in case one EDC missing*/
	if (MCOPT.mcmcid==119 && PEDC!=0 && count%3==0){for (n=0;n<PI->npars;n++){PI->parini[n]=CARDADATA.parpriors[n];}}

}

/*Writing parameters to file*/
FILE *fileout=fopen(MCOPT_CARDAMOM->startfile,"ab");
for (n=0;n<PI->npars*MCOPT.nchains;n++){fwrite(&MCOUT.best_pars[n],1,sizeof(double),fileout);}
fclose(fileout);
oksofar("Starting solution found & written to file");
printf("filename = %s\n",MCOPT_CARDAMOM->startfile);
}}



printf("Initial parameters already sampled & saved");
/*Read file values into PARS chains (only as many as needed)*/
fileout0=fopen(MCOPT_CARDAMOM->startfile,"r");
fread(PI->parini,sizeof(double),PI->npars*MCOPT_CARDAMOM->nchains,fileout0);
fclose(fileout0);
/*Sampling new/more parameters*/

	/*
	for (n=0;n<PI->npars;n++){printf("%8.6f  ",PI->parini[n]);}printf("\n");
	printf("EDC Probability of starting parameters = %4.4f\n",EMLF(CARDADATA, PI->parini));
	printf("Probability of starting parameters = %4.4f\n",CARDADATA.MLF(CARDADATA, PI->parini));
	*/
	/*for (n=0;n<PI->npars;n++){PI->stepsize[n]=0.01;}*/
	
	/*SOON-TO-BE-OBSOLETE: resetting fixed pars to zero for main r*/
	for (n=0;n<PI->npars;n++){PI->parfix[n]=0;}

/*clearing MCOUT fields*/
free(MCOUT.best_pars);
/*Done either (a) reading parameters from file, or (b) sampling parameters
*/

return 0;}

