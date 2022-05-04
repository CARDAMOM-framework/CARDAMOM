#pragma once
#include <math.h>
#include "DALEC_ALL_LIKELIHOOD.c"
#include "DALEC_ALL_MLF.c"
#include "../../CARDAMOM_MODELS/DALEC/DALEC_EDCs/DALEC_EDC_FUNCTIONS.c"
#include "../../../math_fun/ipow.c"

        



double DALEC_MLF2(DATA DATA,double *PARS){

    
    //Copy "DATA.M_PARS" for convenience
    memcpy(DATA.M_PARS, PARS, DATA.nopars*sizeof(double));


/*Copy model pointer for brevity*/
DALEC *MODEL=(DALEC *)DATA.MODEL;

EDCs *EDCs=MODEL->EDCs;


//int EDC,n;
double P=0;


if (DATA.ncdf_data.EDC==1){
    //Call EDC function for prerun=1 models
    double Pedc1 = RUN_DALEC_EDCs(&DATA, EDCs, true);
P=P+Pedc1;}


/*PARAMETER LOG LIKELIHOOD*/
//P=P+LIKELIHOOD_P(DATA,PARS);



/*running model*/
MODEL->dalec(DATA, PARS);
/*storing GPP*/
//for (n=0;n<DATA.nodays;n++){DATA.M_GPP[n]=DATA.M_FLUXES[n*DATA.nofluxes];}


//Post-run EDCs
if (DATA.ncdf_data.EDC==1){
    //Call EDC function for prerun=0 edcs
    double Pedc2 = RUN_DALEC_EDCs(&DATA, EDCs, false);
P=P+Pedc2;}


/*Likelihood*/

P=P+LIKELIHOOD(DATA);




/*saving likelihood P*/
DATA.M_P[0]=P;




/*Returning the log likelihood P*/
return P;


}



double EDC_DALEC_MLF2_BINARY(DATA DATA, double *PARS){


/*Copy model pointer for brevity*/
DALEC *MODEL=(DALEC *)DATA.MODEL;


double P=0;

double ML=DATA.MLF(DATA,PARS);

/*overriding if model likelihood is zero or erroneous*/
if (( isinf(ML)==-1 || isinf(ML)==1 || isnan(ML) )){P=log(0);}


return P;}




     
int DALEC_MLF2_MODCONFIG(MLF* MLF){
    
        //Populate LS 
     LIKELIHOODinfo LIKELIHOODinfo; DALEC_ALL_LIKELIHOOD_MODCONFIG(&LIKELIHOODinfo);
    //Store LS
     MLF->nolikelihoods=LIKELIHOODinfo.nolikelihoods;
//Store pointer to function
    MLF->mlf= DALEC_MLF2;
    
    return 0;}





