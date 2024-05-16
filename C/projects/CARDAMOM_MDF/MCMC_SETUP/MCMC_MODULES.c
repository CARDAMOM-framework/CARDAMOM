

#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../CARDAMOM_GENERAL/CARDAMOM_READ_BINARY_DATA.c"
#include "PROJECT_FUN/FIND_EDC_INITIAL_VALUES.c"
#include "../../../math_fun/declare_matrix.c"
#include "../../CARDAMOM_GENERAL/NETCDF_AUXILLIARY_FUNCTIONS.c"


////TODO: break this into it's own shared file! See cardamom_run_model.c
/* Handle netCDF library errors by printing an error message and exiting with a
 * non-zero status.*/
#define ERREXITCODE 2
#define NCDFERR(e) {printf("Error in %s at %d: %s\n", __FILE__, __LINE__, nc_strerror(e));}
#define FILE_NAME_MAX_LEN 1000

//This is a tiny macro used to do error handeling for netCDF methods with the standard format of returning a result code.
//It helps make the code look less insane by allowing one-line calls
//This as a macro because we want the __FILE__ and __LINE__ macros to work
#define FAILONERROR(ncretval) if (ncretval != NC_NOERR) {NCDFERR(ncretval); exit(ERREXITCODE);}
//This variant will not die, but will still thow a message at the user about the problem.
#define WARNONERROR(ncretval) if (ncretval != NC_NOERR) {NCDFERR(ncretval);}

/*DALEC_SYNTHETIC SETUP*/










int READ_PARI_DATA(PARAMETER_INFO *PI, DATA *DATA,MCMC_OUTPUT *MCOUT,MCMC_OPTIONS *MCOPT, char *CLA[]){
/*READING IN DALEC_SYNTHETIC DATA*/
/*opening file*/


/*CHANGE4EDDIE*/
/*this only applies to the native environment - change accordingly to add default file!*/
char filename[1000];strcpy(filename,CLA[1]);



printf("MCMC_MODULES.c READ_PARI_DATA(): file read and copied\n");



/*defining initial values*
 * Need to perform MCMC run to determine this, or we need a startfile. Either:
 * -netcdf start file is present and has right amount of data, use data inside that file
 * -NO FILE is present at all, just build a new solution and store it
 */







/*Reading file (if available)*/
/*if number of parameters x number of chains available are contained in file, then OK*/
/*Otherwise search for new parameters*/
/*
FILE *fileout0=fopen(MCOPT_CARDAMOM->startfile,"r");
int filelength;
if (fileout0!=NULL){
fseek(fileout0, 0, SEEK_END);filelength = ftell(fileout0)/sizeof(double);fclose(fileout0);}
else{filelength=0;}
*/
/*Two choices:
(1) read parameters from file if these are sufficient
(2) sample/store parameter vectors from file otherwise*/


/*Sampling M=N-N0 chains, where N0 is the number of existing chains in the file*/
/*
int m,M=MCOPT_CARDAMOM->nchains - filelength/PI->npars;
printf("Number of starting parameter vectors saved in file: %i\n",filelength/PI->npars);
printf("Number of required starting parameters vectors: %i\n", MCOPT_CARDAMOM->nchains);
printf("Number of starting parameter vectors to be sampled here: %i\n", int_max(M,0));

if (PI->npars>filelength){
    printf("Initial parameters already sampled & saved");
 */
/*Read file values into PARS chains (only as many as needed)*/
/*
fileout0=fopen(MCOPT_CARDAMOM->startfile,"r");
fread(PI->parini,sizeof(double),PI->npars*MCOPT_CARDAMOM->nchains,fileout0);
fclose(fileout0);
*/


int ncid = 0;
int ncRetVal = nc_open(MCOPT->startfile,NC_NOWRITE, &ncid );
if (ncRetVal != NC_NOERR) {
    printf("%s at %d NOTE: got error %s when opening the startfile, ignoring it.\n", __FILE__, __LINE__, nc_strerror(ncRetVal));
}else{
    int paramDimID,sampleDimID; //Dim ID number, must be populated each invocation
    int parsVarID; // Variable ID numbers, also must be repopulated each invocation

    FAILONERROR(nc_inq_dimid(ncid,"Parameter",&paramDimID));
    FAILONERROR(nc_inq_dimid(ncid,"Sample", &sampleDimID));
    FAILONERROR(nc_inq_varid(ncid,"Parameters",&parsVarID));

    FAILONERROR(nc_get_vara_double(ncid,parsVarID,(const size_t[]){0,0}, (const size_t[]){MCOPT->nchains,PI->npars},PI->parini ));
    FAILONERROR(nc_close(ncid));
}

FIND_EDC_INITIAL_VALUES(*DATA,PI,MCOPT);


/*Originally in FIND_EDC_INITAL_VALUES, had comment that follows. Anthony confirmed this is needed for legacy support.
SOON-TO-BE-OBSOLETE: resetting fixed pars to zero for main r*/
int n;
for (n=0;n<PI->npars;n++){PI->parfix[n]=0;}
/*resetting PI-stepsize (as this has been changed)*/
for (n=0;n<PI->npars;n++){PI->stepsize[n]=0.0001;}

printf("CARDAMOM_MDF/MCMC_SETUP/MCMC_MODULES.c: Done with initial parameters");


/*This function is in the MCMC folder*/
INITIALIZE_MCMC_OUTPUT(*PI,MCOUT,*MCOPT);

for (n=0;n<PI->npars;n++){PI->stepsize[n]=0.0001;}

return 0;
}


// /*MCMC OPTIONS*/
// int READ_MCOPT_OBS(MCMC_OPTIONS *MCOPT, char *CLA[]){
// /*number of command line imputs*/
// int ncli=atoi(CLA[0]);
// 
// /*defining MCMC_OPTIONS structure*/
// MCOPT->APPEND=0;
// MCOPT->nADAPT=100;
// MCOPT->fADAPT=0.5;
// /*command line (or default) values*/
// if (ncli<3){MCOPT->nOUT=1000;}else{MCOPT->nOUT=atoi(CLA[3]);};
// if (ncli<4){MCOPT->nPRINT=1000;}else{MCOPT->nPRINT=atoi(CLA[4]);};
// if (ncli<5){MCOPT->nWRITE=10;}else{MCOPT->nWRITE=atoi(CLA[5]);};
// if (ncli<6){MCOPT->minstepsize=1e-7;}else{MCOPT->minstepsize=atof(CLA[6]);};
// if (ncli<7){MCOPT->mcmcid=1;}else{MCOPT->mcmcid=atof(CLA[7]);};
// if (ncli<8){MCOPT->nADAPT=100;}else{MCOPT->nADAPT=atoi(CLA[8]);};
// 
// MCOPT->randparini=0;
// MCOPT->returnpars=0;
// MCOPT->fixedpars=0;
// char outfile[200], stepfile[200],startfile[200];
// if (ncli<2){strcpy(outfile,"MOUT_");strcpy(stepfile,"MOUT_");}
// else{strcpy(outfile,CLA[2]); strcpy(stepfile,CLA[2]); strcpy(startfile,CLA[2]);}
// /*STEP file comes with SUFFIX*/
// /*PARS file no longer does!*/
// /*strcat(outfile,"PARS");*/
// strcat(stepfile,"STEP");
// strcat(startfile,"START");
// /*directory*/
// strcpy(MCOPT->outfile,outfile);
// strcpy(MCOPT->stepfile,stepfile);
// strcpy(MCOPT->startfile,startfile);
// 
// return 0;
// 
// }




// #define DEFAULT_DOUBLE_VAL -9999.0
// #define DEFAULT_INT_VAL -9999



/*Enter all fields originally defined with MALLOC*/
int MEMORY_CLEANUP(DATA DATA, PARAMETER_INFO PI, MCMC_OPTIONS MCOPT, MCMC_OUTPUT MCOUT){

free(PI.parmin);
free(PI.parmax);
free(PI.parini);
free(PI.parfix);
free(PI.stepsize);
free(PI.transform);

FREE_DATA_STRUCT(DATA);
/*
free(DATA.MET);
free(DATA.LAI);
free(DATA.NEE);
free(DATA.WOO);
free(DATA.GPP);


free(DATA.M_FLUXES);
free(DATA.M_LAI);
free(DATA.M_NEE);
free(DATA.M_POOLS);
free(DATA.M_GPP);

free(DATA.parmin);
free(DATA.parmax);

if (DATA.ngpp>0){free(DATA.gpppts);}
if (DATA.nlai>0){free(DATA.laipts);}
if (DATA.nnee>0){free(DATA.neepts);}
if (DATA.nwoo>0){free(DATA.woopts);}

*/
free(MCOUT.best_pars);


return 0;}




/*this function initializes the PI fields
It is called from DALEC_ALL_TEMPLATE or equivalent higher level function*/
int INITIALIZE_PI_STRUCT(PARAMETER_INFO * PI, DATA * DATA, MCMC_OPTIONS *MCO){
oksofar("CARDAMOM_MDF/MCMC_SETUP/MCMC_MODULES.c INITIALIZE_PI_STRUCT():  initializing PI stucture");
/*contains 6 fields with min max log for par and par*/
PI->parmin=calloc(DATA->nopars,sizeof(double));
PI->parmax=calloc(DATA->nopars,sizeof(double));
PI->parini=calloc(DATA->nopars*MCO->nchains,sizeof(double));
PI->parfix=calloc(DATA->nopars,sizeof(double));
PI->stepsize=calloc(DATA->nopars,sizeof(double));
PI->transform=calloc(DATA->nopars,sizeof(int));
/*MAtrix double-pointer allocation*/
oksofar("CARDAMOM_MDF/MCMC_SETUP/MCMC_MODULES.c INITIALIZE_PI_STRUCT(): about to declare matrix");


/*defining step size*/

int n,m;
PI->npars=DATA->nopars;
for (n=0;n<DATA->nopars;n++){
/*copying minimum and maximum parameter values from DATA*/
PI->parmin[n]=DATA->parmin[n];
PI->parmax[n]=DATA->parmax[n];
PI->stepsize[n]=0.01;}

/*Try first hardcoding transform*/
/*PI->transform[11]=2;
PI->transform[14]=2;
*/


oksofar("CARDAMOM_MDF/MCMC_SETUP/MCMC_MODULES.c INITIALIZE_PI_STRUCT(): stepsizes initialized");
return 0;}



