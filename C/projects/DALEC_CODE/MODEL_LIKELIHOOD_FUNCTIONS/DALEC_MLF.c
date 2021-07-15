#pragma once
#include <math.h>
#include "DALEC_ALL_LIKELIHOOD.c"
#include "../../../math_fun/ipow.c"


double DALEC_MLF(DATA DATA,double *PARS){

printf("DALEC_MLF Line 9...\n");
//copy pars to M_PARS for export
int n; for (n=0;n<DATA.nopars;n++){DATA.M_PARS[n]=PARS[n];}
printf("DALEC_MLF Line 12...\n");
//printf("DATA.nopars = %i\n",DATA.nopars);
//printf("PARS[16] = %2.2f\n",PARS[16]);
//printf("DATA.M_PARS[16] = %2.2f\n",DATA.M_PARS[16]);

/*Copy model pointer for brevity*/
DALEC *MODEL=(DALEC *)DATA.MODEL;

struct EDCDIAGNOSTIC EDCD;
/*Copy default structure*/
/*EDCD=*((DALEC *)DATA.MODEL)->EDCD;*/
EDCD=*MODEL->EDCD;
/*EDCD->nedc=100;
int n; for (n=0;n<EDCD->nedc;n++){EDCD->PASSFAIL[n]=1;}
*/
printf("DALEC_MLF Line 27...\n");
int EDC;
double P=0,P_p;

EDC=ipow(MODEL->edc1(PARS,DATA, &EDCD),DATA.EDC);
P=P+log((double)EDC);

printf("DALEC_MLF Line 34...\n");
if (EDC==1 | EDCD.DIAG==1){
/*PARAMETER LOG LIKELIHOOD*/
P=P+LIKELIHOOD_P(DATA,PARS);
P_p=P;

printf("DALEC_MLF Line 40...\n");
/*running model*/
MODEL->dalec(DATA, PARS);
printf("DALEC_MLF Line 43...\n");

/*storing GPP*/
//for (n=0;n<DATA.nodays;n++){DATA.M_GPP[n]=DATA.M_FLUXES[n*DATA.nofluxes];}

/*EDC2 check*/
EDC=MODEL->edc2(PARS, DATA, &EDCD);
EDC=ipow(EDC,DATA.EDC);
printf("DALEC_MLF Line 51...\n");

/*LIKELIHOOD*/
P=P+log((double)EDC);
printf("DALEC_MLF Line 55...\n");



if (EDC==1){P=P+LIKELIHOOD(DATA);}}
printf("DALEC_MLF Line 60...\n");



/*saving EDCD if EDCDIAG==1*/
if (DATA.EDCDIAG==1){for (n=0;n<100;n++){DATA.M_EDCD[n]=EDCD.PASSFAIL[n];}}
printf("DALEC_MLF Line 66...\n");


/*saving likelihood P*/
DATA.M_P[0]=P;
printf("DALEC_MLF Line 71...\n");


/*Returning the log likelihood P*/
return P;


}







double EDC_DALEC_MLF(DATA DATA, double *PARS){

/*Copy model pointer for brevity*/
DALEC *MODEL=(DALEC *)DATA.MODEL;

/*Independend EDCD structure: stores detailed EDC info, and provides DATA with result summary*/
struct EDCDIAGNOSTIC EDCD;
/*initialize EDCD: copy default structure*/
EDCD=*MODEL->EDCD;
/*enforce DIAGNOSTIC MODE: switches are still in place*/
EDCD.DIAG=1;
int EDC, n;
double P=0;


EDC=MODEL->edc1(PARS,DATA,&EDCD);

/*running model*/
MODEL->dalec(DATA, PARS);

/*EDC2 check*/
EDC=EDC*MODEL->edc2(PARS, DATA, &EDCD);

/*LIKELIHOOD (log likelihood)*/
/*EDCs are individually counted*/
/*Only counted if EDCSWITCH is on*/
int tot_exp=0;
for (n=0;n<EDCD.nedc;n++){
tot_exp+=1-ipow(EDCD.PASSFAIL[n],EDCD.SWITCH[n]);}

P=-0.5*((double)tot_exp*10)*DATA.EDC;



/*overriding if model likelihood is zero or erroneous*/
double ML=DATA.MLF(DATA,PARS);
if (( isinf(ML)==-1 || isinf(ML)==1 || isnan(ML) )){
P=P-0.5*10;}
/*if (DATA->EDC==0 && (isinf(ML)==-1 || isnan(ML))){P=P-0.5*10;}
*/

return P;

}




