#pragma once
#include "DALEC_1004.c"
#include "EDC1_1004.c"
#include "EDC2_1004.c"
#include "../../../DALEC_CODE/MODEL_LIKELIHOOD_FUNCTIONS/DALEC_MLF.c"
#include "../../../DALEC_CODE/MODEL_LIKELIHOOD_FUNCTIONS/DALEC_MLF_beta.c"


int MODEL_INFO_1004(DATA * DATA){

/*Step 1. Declare structure*/
/*"static" ensures that the memory is declared in one instance and visible to all functions (I think)*/
static DALEC DALECmodel;


DATA->assemble_model=1;

/*Step 2: Fill structure with model-specific info*/
DALECmodel.nopools=8;
DALECmodel.nomet=9;/*This should be compatible with CBF file, if not then disp error*/
/*DALECmodel.nopars=36;*/
DALECmodel.nofluxes=32;

/*Short-term: copy quantities into DATA structure to reduce dependencies in CARDAMOM_MODEL_LIBRARY.c*/
/*Long-term: remove dependencies on DATA.nofluxes... etc. in CARDAMOM_READ_BINARY_DATA and DALEC_ALL_LIKELIHOOD.c*/
DATA->nopools=DALECmodel.nopools;
/*DATA->nopars=DALECmodel.nopars;*/
DATA->nofluxes=DALECmodel.nofluxes;

/*All model functions*/
/*User is able to add further functions as deemed necessary*/
/*Function names are declared in ../DALEC_ALL/DALEC_MODULE.c*/
/*Consider starting new module for radically different model structures*/
DALECmodel.dalec=DALEC_1004;
/*THESE NEED TO BE LEFT HERE UNTIL EDC_DALEC_MLF and/or EDC_DALEC_MLF_beta made obsolete*/
DALECmodel.edc1=EDC1_1004;
DALECmodel.edc2=EDC2_1004;

/*Initialize parameter fields*/
/*initializing parmin and parmax fields*/
/*Currently assigned to "DATA", since MCMC needs info separately*/

/*
DATA->parmin=calloc(DATA->nopars,sizeof(double));
DATA->parmax=calloc(DATA->nopars,sizeof(double));
*/

/*Mount EDC checks here*/
EDCSETUP(*DATA,&DALECmodel.EDCD);



DALECmodel.nopars=0;
DATA->nopars=0;
DATA->parmin=NULL; 
/*calloc(DATA->nopars,sizeof(double));*/
DATA->parmax=NULL;
DATA->parname=NULL;
/*calloc(DATA->nopars,sizeof(double));*/

/*allocate zero-size memory, for realloc later*/
DATA->parmin=calloc(DATA->nopars,sizeof(double));
DATA->parmax=calloc(DATA->nopars,sizeof(double));


/*Mount DALEC 1004 module parameters*/
MOUNT_DALEC_1004_PARS(DATA);DALECmodel.nopars=DATA->nopars;

printf("DATA->nopars = %i\n",DALECmodel.nopars);
/*Write pars to file*/




/*initializing model*/
DATA->MODEL=&DALECmodel;
DATA->MLF=DALEC_MLF_beta;



return 0;}
