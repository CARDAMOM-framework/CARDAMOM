#include "DALEC_1004.c"
#include "EDC1_1004.c"
#include "EDC2_1004.c"
#include "../../../DALEC_CODE/MODEL_LIKELIHOOD_FUNCTIONS/DALEC_MLF.c"


int MODEL_INFO_1004(DATA * DATA){

/*Step 1. Declare structure*/
/*"static" ensures that the memory is declared in one instance and visible to all functions (I think)*/
static DALEC DALECmodel;

/*Step 2: Fill structure with model-specific info*/
DALECmodel.nopools=8;
DALECmodel.nomet=9;/*This should be compatible with CBF file, if not then disp error*/
DALECmodel.nofluxes=32;
/****CONTINUE FROM HERE: "MODULES" is a generic*/
/*DATA.MODULES=*/

/*Short-term: copy quantities into DATA structure to reduce dependencies in CARDAMOM_MODEL_LIBRARY.c*/
/*Long-term: remove dependencies on DATA.nofluxes... etc. in CARDAMOM_READ_BINARY_DATA and DALEC_ALL_LIKELIHOOD.c*/
DATA->nopools=DALECmodel.nopools;
DATA->nofluxes=DALECmodel.nofluxes;

/*All model functions*/
/*User is able to add further functions as deemed necessary*/
/*Function names are declared in ../DALEC_ALL/DALEC_MODULE.c*/
/*Consider starting new module for radically different model structures*/
DALECmodel.dalec=DALEC_1004;

/*Initialize parameter fields*/
/*initializing parmin and parmax fields*/
/*Currently assigned to "DATA", since MCMC needs info separately*/

/*DALECmodel.nopars = potentially obsolete as modules mounted directly onto DATA stack */
DALECmodel.nopars=0;

DATA->parmin=NULL; /*calloc(DATA->nopars,sizeof(double));*/
DATA->parmax=NULL;/*calloc(DATA->nopars,sizeof(double));*/


/*Mount DALEC 1004 module parameters*/
MOUNT_DALEC_1004_PARS(DATA);


printf("DALECmodel.nopars = %i\n",DALECmodel.nopars);

/*DALECmodel.nopars = potentially obsolete as modules mounted directly onto DATA stack */

oksofar("about to declare EDCD");
printf("DALECmodel.EDCD = %p\n",DALECmodel.EDCD);
/*Initialize the EDCD structure*/
EDCSETUP(*DATA,&DALECmodel.EDCD);
oksofar("done with declaration");


/*initializing model*/
DATA->MODEL=DALEC_1004;
DATA->MLF=DALEC_MLF_beta;




return 0;}
