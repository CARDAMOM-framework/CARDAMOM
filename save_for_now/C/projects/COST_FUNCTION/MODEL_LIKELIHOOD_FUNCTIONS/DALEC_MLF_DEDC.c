#pragma once
#include <math.h>
#include "DALEC_ALL_LIKELIHOOD.c"
#include "DALEC_ALL_MLF.c"
#include "../../../math_fun/ipow.c"



double DALEC_MLF_DEDC(DATA DATA,double *PARS){

    memcpy(DATA.M_PARS, PARS, DATA.nopars*sizeof(double));

 

/*Copy model pointer for brevity*/
DALEC *MODEL=(DALEC *)DATA.MODEL;

struct EDCDIAGNOSTIC EDCD;
/*Copy default structure*/
/*EDCD=*((DALEC *)DATA.MODEL)->EDCD;*/
EDCD=*MODEL->EDCD;

int EDC,n;
double P=0,P_p;
EDCD.pEDC=0;



if (DATA.ncdf_data.EDC==1){
MODEL->edc1(PARS,DATA, &EDCD);
P=P+EDCD.pEDC;}


/*PARAMETER LOG LIKELIHOOD*/
//P=P+LIKELIHOOD_P(DATA,PARS);
P_p=P;


/*running model*/
MODEL->dalec(DATA, PARS);
/*storing GPP*/
//for (n=0;n<DATA.nodays;n++){DATA.M_GPP[n]=DATA.M_FLUXES[n*DATA.nofluxes];}


/*EDC2 check*/
if (DATA.ncdf_data.EDC==1){

    MODEL->edc2(PARS, DATA, &EDCD);
P=P+EDCD.pEDC;}

/*Likelihood*/

P=P+LIKELIHOOD(DATA);




/*saving likelihood P*/
DATA.M_P[0]=P;




/*Returning the log likelihood P*/
return P;


}


     
int DALEC_MLF_DEDC_MODCONFIG(MLF* MLF){
    
   //Populate LS 
    LIKELIHOODinfo LIKELIHOODinfo; DALEC_ALL_LIKELIHOOD_MODCONFIG(&LIKELIHOODinfo);
   //Store LS
    MLF->nolikelihoods=LIKELIHOODinfo.nolikelihoods;
    MLF->mlf= DALEC_MLF_DEDC;
    
    return 0;}





