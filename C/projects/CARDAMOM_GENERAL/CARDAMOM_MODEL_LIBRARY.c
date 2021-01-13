#pragma once

int INITIALIZE_PARAMETER_FIELDS(DATA *DATA){
/*initializing parmin and parmax fields*/
DATA->parmin=NULL;
DATA->parmax=NULL;
DATA->parname=NULL;
DATA->parmin=calloc(DATA->nopars,sizeof(double));
DATA->parmax=calloc(DATA->nopars,sizeof(double));
DATA->parname=calloc(DATA->nopars,sizeof(char *));
return 0;}

#include "ADD_PARAMETER_TO_STACK.c"
#include "WRITE_MODEL_STACKS_TO_FILE.c"
/*Including all model PARS_INFO_MMMM (model) files*/
#include "../CARDAMOM_MODELS/DALEC/DALEC_1000/MODEL_INFO_1000.c"
#include "../CARDAMOM_MODELS/DALEC/DALEC_1004/MODEL_INFO_1004.c"

/*This function attributes model specific variables based
 * on the ID number stored in DATA->ID*/
/*General note: The code IDxMA (where x is the model ID) is kept above the model attribute
line. The reason for this is to ensure that Matlab can easily access this code*/



int CARDAMOM_MODEL_LIBRARY(DATA *DATA){


int status=0;
if (DATA->ID==1000  ){MODEL_INFO_1000(DATA);}
if (DATA->ID==1004  ){MODEL_INFO_1004(DATA);}
else {status=1;}



return status;}


