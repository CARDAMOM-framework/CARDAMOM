#include <assert.h>
#include "../../auxi_fun/oksofar.c"
#include "../../auxi_fun/okcheck.c"
#include "../../auxi_fun/seedrandomnumber.c"

/*defines all the structures, i.e. DATA, MCOPT, PI*/

#include "../../mcmc_fun/MHMCMC/MCMC_FUN/MCMCOPT.c"
#include "MCMC_SETUP/MCMC_MODULES.c"


/*Temporarily de-activating to write EDC sampler*/
#include "../../mcmc_fun/MHMCMC/MCMC_FUN/DEMCMC.c"
#include "../../mcmc_fun/MHMCMC/MCMC_FUN/ADEMCMC.c"
#include "../../mcmc_fun/MHMCMC/MCMC_FUN/MHMCMC_119.c"



/*MCMC OPTIONS*/
int READ_MCOPT(MCMC_OPTIONS *MCOPT, DATA DATA, char *outfile){
/*number of command line imputs*/

/*defining MCMC_OPTIONS structure*/
MCOPT->APPEND=0;
//MCOPT->nADAPT=100;
MCOPT->fADAPT=0.5;


MCOPT->nOUT=DATA.ncdf_data.MCMCID.nITERATIONS;
MCOPT->nPRINT=DATA.ncdf_data.MCMCID.nPRINT;
MCOPT->minstepsize=DATA.ncdf_data.MCMCID.minstepsize;
MCOPT->mcmcid=DATA.ncdf_data.MCMCID.value;
MCOPT->nADAPT=DATA.ncdf_data.MCMCID.nADAPT;
MCOPT->fADAPT=DATA.ncdf_data.MCMCID.fADAPT;

if (DATA.ncdf_data.MCMCID.nSAMPLES==DEFAULT_INT_VAL){DATA.ncdf_data.MCMCID.nSAMPLES=2000;}



if (MCOPT->nOUT==DEFAULT_INT_VAL){MCOPT->nOUT=10000;}
if (MCOPT->nPRINT==DEFAULT_INT_VAL){MCOPT->nPRINT=1000;}
if (MCOPT->minstepsize==DEFAULT_DOUBLE_VAL){MCOPT->minstepsize=1e-5;}
if (MCOPT->mcmcid==DEFAULT_INT_VAL){MCOPT->mcmcid=119;}
if (MCOPT->nADAPT==DEFAULT_INT_VAL){MCOPT->nADAPT=100;}
if (MCOPT->fADAPT==DEFAULT_DOUBLE_VAL){MCOPT->fADAPT=0.05;}

//Derive nWRITE from fields
MCOPT->nWRITE=MCOPT->nOUT/DATA.ncdf_data.MCMCID.nSAMPLES;


printf("**********MCMCOPT SUMMARY*******\n");

printf("Number of iterations = %i\n",MCOPT->nOUT);
printf("Print status every %i iterations \n",MCOPT->nOUT);
printf("Write parameters every %i iterations \n",MCOPT->nWRITE);
printf("Number of samples requested (including burn-in) = %i\n",DATA.ncdf_data.MCMCID.nSAMPLES);
printf("Burn in fraction = %2.2f\n",MCOPT->fADAPT);
printf("MCMC ID= %i\n",MCOPT->mcmcid);


printf("***********************************\n");

MCOPT->randparini=0;
MCOPT->returnpars=0;
MCOPT->fixedpars=0;
char stepfile[1000],startfile[1000];
strcpy(stepfile,outfile); strcpy(startfile,outfile);
/*STEP file comes with SUFFIX*/
/*PARS file no longer does!*/
/*strcat(outfile,"PARS");*/
strcat(stepfile,"STEP");
strcat(startfile,"START");
/*directory*/
strcpy(MCOPT->outfile,outfile);
strcpy(MCOPT->stepfile,stepfile);
strcpy(MCOPT->startfile,startfile);



return 0;

}



int main(int argc,char *CLA[]){
/*To correctly set-up the MHMCMC*/

/*inputs*/
/*1. met file in*/
/*2. results file out*/
    //Rest is now obsolete



/*3. number of MCMC solutions requested*/
/*4. print-to-screen frequency*/
/*5. write-to-file frequency*/


/*OK is output flag from all functions*/
int OK = 0; //Need to initilize!


/*SETTING number of command line inputs as char in CLA[0]*/
sprintf(CLA[0],"%d",argc-1);
/*declaring CARDAMOM Binary Format (.cbf) file*/
char CBFfile[1000], CBRfile[1000];
//Setting CBF file to equal first argument
strcpy(CBFfile,CLA[1]);
strcpy(CBRfile,CLA[2]);



/***********CARDATA STRUCTURE*************/

/*defining data structure*/
DATA DATA;
/*Initialize data structure - this function is found in CARDAMOM_READ_BINARY_DATA*/
//OK=INITIALIZE_DATA_STRUCT(&DATA);
//okcheck(OK,"Main data structure initialized");

/*read cardamom data from file*/
/*Function also performs and displays basic checks*/
CARDAMOM_READ_BINARY_DATA(CBFfile,&DATA);
//okcheck(OK,"Main data structure read successfully");

//********************************************//



/*defining MCMC_OPTIONS structure*/
MCMC_OPTIONS MCOPT;

/*ID=1 adaptive MHMCMC*/
/*ID=2 DE-MCMC*/
/*Hard-coding number of chains for now (for DEMCMC)*/

READ_MCOPT(&MCOPT,DATA, CBRfile);

if (MCOPT.mcmcid==119){MCOPT.nchains=1;}
if (MCOPT.mcmcid==3){MCOPT.nchains=200;}
else if (MCOPT.mcmcid==2){MCOPT.nchains=100;}


printf("MDF options structure read successfully");


printf("CARDAMOM_MDF.c: CARDAMOM MODEL ID = %i\n",DATA.ncdf_data.ID);
printf("CARDAMOM_MDF.c: MCMC ID = %i\n",MCOPT.mcmcid);

/*defining the MCMC output structure*/
MCMC_OUTPUT MCOUT;

/*These lines guarantee high frequency random generator seeding*/

//if (argc-1<2){seedrandomnumber(CBFfile);}else{seedrandomnumber(CLA[2]);}

if (DATA.ncdf_data.MCMCID.seed_number==DEFAULT_INT_VAL){DATA.ncdf_data.MCMCID.seed_number=0;}
srandom(DATA.ncdf_data.MCMCID.seed_number);
printf("*******RANDOM NUMBER SEED**********\n");
printf("Read in from DATA.ncdf_data.MCMCID.seed_number\n");
printf("(Default value = 0)\n");
printf("*******Seed = %i**********\n",DATA.ncdf_data.MCMCID.seed_number);
printf("*******DONE WITH RANDOM NUMBER SEED**********\n");


/*Defining all MCMC components*/
/*USER DEFINED: SETUP MCMC - templates provides*/
/*NOTE  : READ_PARI_DATA function is stored in DALEC_CDEA_TEMPLATE/MCMC_SETUP/MCMC_MODULES.c*/
/*TO DO : (a) read DATA first - note that this includes model specific fields, such as nomet, nopars, etc.
        these are all loaded via the CARDAMOM_MODEL_LIBRARY(DATA) function*/
/*      : (b) read PI based on DATA*/




/***************PI STRUCTURE AND MLF*****************/


/*defining parameter structure*/
/*this structure is defined as part of the MCMC
Full function can be found in mcmc_fun/MHMCMC/MCMC_FUN/MCMC_OPT.c*/
PARAMETER_INFO PI;

/*initializing structure with correct PI fields (as required by MHMCMC)*/
/*Function is in MCMC_MODULES.c*/
INITIALIZE_PI_STRUCT(&PI,&DATA,&MCOPT);
printf("CARDAMOM_MDF.c: Parameter info structure initialized\n");


/*choose model likelihood here*/
/*the MLF function determines the probability of any model output based on data and/or other constraints*/
/*In future versions: this should be done inside CARDAMOM_READ_BINARY_DATA*/



/*READ_PARI_DATA and READ_MCOPT should now be generic for all model types*/
/*CONTAINS "FIND_EDC_INITIAL_VALUES(*DATA,PI);"*/
DATA.edcsearch=1;
READ_PARI_DATA(&PI, &DATA, &MCOUT, &MCOPT,CLA);
printf("CARDAMOM_MDF.c: READ_PARI_DATA successfully executed\n");


/*calling the MHMCMC here*/
DATA.edcsearch=0;

printf("CARDAMOM_MDF.c: about to start MCMC\n");
printf("CARDAMOM_MDF.c: Prescribed option = %i\n",MCOPT.mcmcid);
switch (MCOPT.mcmcid){

case 119:
printf("CARDAMOM_MDF.c: about to start MHMCMC (id = 119)\n");
MHMCMC_119(DATA.MLF,DATA,PI,MCOPT,&MCOUT);
printf("CARDAMOM_MDF.c: completed MHMCMC 119\n");
break;
case 2:
printf("CARDAMOM_MDF.c: about to start DEMCMC\n");
DEMCMC(DATA.MLF,DATA,PI,MCOPT,&MCOUT);
break;
case 3:
//MCOPT.fADAPT=0.05;
    //MCOPT.fADAPT=1;
printf(" CARDAMOM_MDF.c: about to start ADEMCMC\n");
ADEMCMC(DATA.MLF,DATA,PI,MCOPT,&MCOUT);
break;


/*printf("CARDAMOM_MDF.c: DEMCMC temporarily disconnected, need to de-bug, correct and re-introduce");
printf("CARDAMOM_MDF.c: completed DEMCMC\n");
break;*/
default:
printf("CARDAMOM_MDF.c: Error: no valid mcmcid value prescribed...\n");

}
// printf("CARDAMOM_MDF.c: MCMC complete\n");
// printf("DATA.M_P[0] = %2.2f\n",DATA.M_P[0]);
// printf("DATA.LAI.values[0] = %2.2f\n",DATA.ncdf_data.LAI.values[0]);
// printf("DATA.LAI.values[24] = %2.2f\n",DATA.ncdf_data.LAI.values[24]);
// printf("DATA.M_POOLS[1] = %2.2f\n",DATA.M_POOLS[1]);
// printf("DATA.ncdf_data.LAI.opt_unc_type=%i\n",DATA.ncdf_data.LAI.opt_unc_type);
// printf("DATA.ncdf_data.LAI.opt_normalization=%i\n",DATA.ncdf_data.LAI.opt_normalization);
// printf("DATA.ncdf_data.LAI.opt_filter=%i\n",DATA.ncdf_data.LAI.opt_filter);
// printf("DATA.ncdf_data.LAI.min_threshold=%2.2f\n",DATA.ncdf_data.LAI.min_threshold);
// printf("DATA.ncdf_data.LAI.single_monthly_unc=%2.2f\n",DATA.ncdf_data.LAI.single_monthly_unc);
// printf("DATA.ncdf_data.LAI.single_annual_unc=%2.2f\n",DATA.ncdf_data.LAI.single_annual_unc);
// printf("DATA.ncdf_data.LAI.single_mean_unc=%2.2f\n",DATA.ncdf_data.LAI.single_mean_unc);
// printf("DATA.ncdf_data.LAI.single_unc=%2.2f\n",DATA.ncdf_data.LAI.single_unc);
// printf("DATA.ncdf_data.LAI.structural_unc=%2.2f\n",DATA.ncdf_data.LAI.structural_unc);
// printf("DATA.ncdf_data.LAI.unc[24]=%f\n",DATA.ncdf_data.LAI.unc[24]);
// printf("DATA.ncdf_data.LAI.length=%i\n",(int)DATA.ncdf_data.LAI.length);
// printf("DATA.ncdf_data.LAI.unc_length=%i\n",(int)DATA.ncdf_data.LAI.unc_length);
// printf("DATA.ncdf_data.LAI.valid_obs_length=%i\n",(int)DATA.ncdf_data.LAI.valid_obs_length);
// double * values;//Timeseries of observation values


/*???????*/
/*User Defined function needed to clean up memory*/
MEMORY_CLEANUP(DATA,PI,MCOPT,MCOUT);


return 0;

}
