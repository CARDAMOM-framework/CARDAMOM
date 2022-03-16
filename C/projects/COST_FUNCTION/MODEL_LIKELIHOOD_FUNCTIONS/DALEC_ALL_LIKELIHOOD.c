#pragma once
#include "../../../math_fun/std.c"
#include "../../../math_fun/mean.c"
#include "../../../math_fun/max.c"
#include "../CARDAMOM_LIKELIHOOD_FUNCTION.c"
#include "../../CARDAMOM_MODELS/DALEC/DALEC_OBSERVATION_OPERATORS/DALEC_OBSERVATION_OPERATORS.c"



struct PROBABILITY_INDICES{
        //Time varying obs
        int ABGB;
        int CH4;
        int ET;
        int ROFF;
        int EWT;
        int GPP;
        int LAI;
        int NBE;
        int DOM;
        //Mean OBS
        int Mean_ABGB;
        int Mean_FIR;
        int Mean_GPP;
        int Mean_LAI;
        //Parameters
        int PEQ_Cefficiency;
        int PEQ_CUE;
        int PEQ_iniSnow;
        int PEQ_C3frac;} PROBABILITY_INDICES={
     0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    10,11,12,13,14,15,16};




/*Any likelihood functions used in multiple MLF functions are kept here!*/



int EDCSETUP(DATA DATA, struct EDCDIAGNOSTIC ** EDCD){

/*TO DO HERE: set EDCD.EDCSWITCHES to DATA.OTHERPRIORS fields 30 to 50*/
/*DON'T FORGET TO DO THE SAME IN CDEA files*/
/*see if it is possible to initiate both using the same function*/
/*E.g. INITIATE_EDC_SWITCHES*/

static struct EDCDIAGNOSTIC EDCDmem;
EDCDmem.DIAG=DATA.ncdf_data.EDCDIAG;
int n;
/*EDC switches are stored in DATA->parpriors (positions 31-50);*/
/*switch all on if EDCDIAG<2 , otherwise, switch on/off according to EDCSWITCH*/
/*EDCSETUP function will be included in DALEC_ALL_LIKELIHOOD.c*/

/*
printf("*****")
printf("*****Note on status of DALEC EDC settings*******\n");
printf("*****")
printf("*****")
printf("*****")
printf("******Only default values used for now, introduce EDC settings via netcdf system when needed\n");
printf("*****")
*/


for (n=0;n<100;n++){EDCDmem.SWITCH[n]=1;}
// if (EDCDmem.DIAG==2){for (n=0;n<20;n++){EDCDmem.SWITCH[n]=DATA.otherpriors[n+30];}}

//if (EDCDmem.DIAG==2){for (n=0;n<20;n++){EDCDmem.SWITCH[n]=0;}}


/*EQF is stored in the "DATA.otherpriorunc" fields associated with EDCs 7-12*/
/*default value is 2*/
EDCDmem.EQF=DATA.ncdf_data.EDC_EQF;

printf("EDCD->EQF* = %2.2f\n",EDCDmem.EQF);


/*Default structure has all EDC=1, and 100 EDCs*/
EDCDmem.nedc=100;
for (n=0;n<EDCDmem.nedc;n++){EDCDmem.PASSFAIL[n]=1;}


/*Double pointer = contents of EDCD reassigned to static struct*/
*EDCD=&EDCDmem;


return 0;
}


double LIKELIHOOD_P(DATA DATA,double *PARS)
{
/*remember - LOG likelihood*/
double p=0;

return p;}

/*for any other priors, explicitly define functions based on values in DATA.otherpriors*/

/*total biomass (pools 1:4) defined here - using first space of pappriors for total biomass*/


double LIKELIHOOD(DATA D){

//Step 1. Implement observarion operator on 


double P=0;

OBSOPE *O=&((DALEC *)D.MODEL)->OBSOPE;

//printf("O->SUPPORT_LAI_OBS = %d\n",O->SUPPORT_LAI_OBS);


//Observation operator on DALEC variables.
DALEC_OBSOPE(&D,O);

//printf("About to calculate likelihoods...\n");



//printf("O->SUPPORT_LAI_OBS = %d\n",O->SUPPORT_LAI_OBS);

//if (O->SUPPORT_ET_OBS){   P=P+DALEC_LIKELIHOOD_ET(D);}
if (O->SUPPORT_ABGB_OBS){   P=P+CARDAMOM_TIMESERIES_OBS_LIKELIHOOD(&D.ncdf_data.ABGB, D.M_ABGB);};
if (O->SUPPORT_CH4_OBS){   P=P+CARDAMOM_TIMESERIES_OBS_LIKELIHOOD(&D.ncdf_data.CH4, D.M_CH4);};
if (O->SUPPORT_ET_OBS){   P=P+CARDAMOM_TIMESERIES_OBS_LIKELIHOOD(&D.ncdf_data.ET, D.M_ET);};
if (O->SUPPORT_ROFF_OBS){   P=P+CARDAMOM_TIMESERIES_OBS_LIKELIHOOD(&D.ncdf_data.ROFF, D.M_ROFF);};
if (O->SUPPORT_EWT_OBS){   P=P+CARDAMOM_TIMESERIES_OBS_LIKELIHOOD(&D.ncdf_data.EWT, D.M_EWT);};
if (O->SUPPORT_GPP_OBS){   P=P+CARDAMOM_TIMESERIES_OBS_LIKELIHOOD(&D.ncdf_data.GPP, D.M_GPP);};
if (O->SUPPORT_LAI_OBS ){P=P+CARDAMOM_TIMESERIES_OBS_LIKELIHOOD(&D.ncdf_data.LAI, D.M_LAI);};
if (O->SUPPORT_NBE_OBS){   P=P+CARDAMOM_TIMESERIES_OBS_LIKELIHOOD(&D.ncdf_data.NBE, D.M_NBE);};
if (O->SUPPORT_DOM_OBS){   P=P+CARDAMOM_TIMESERIES_OBS_LIKELIHOOD(&D.ncdf_data.DOM, D.M_DOM);};
if ( O->SUPPORT_SCF_OBS){P=P+CARDAMOM_TIMESERIES_OBS_LIKELIHOOD(&D.ncdf_data.SCF, D.M_SCF);};

//Mean OBS


if (O->SUPPORT_ABGB_OBS){   P=P+CARDAMOM_SINGLE_OBS_LIKELIHOOD(&D.ncdf_data.Mean_ABGB, D.M_Mean_ABGB);};
if (O->SUPPORT_FIR_OBS){   P=P+CARDAMOM_SINGLE_OBS_LIKELIHOOD(&D.ncdf_data.Mean_FIR, D.M_Mean_FIR);};
if (O->SUPPORT_GPP_OBS){   P=P+CARDAMOM_SINGLE_OBS_LIKELIHOOD(&D.ncdf_data.Mean_GPP, D.M_Mean_GPP);};
if (O->SUPPORT_LAI_OBS){   P=P+CARDAMOM_SINGLE_OBS_LIKELIHOOD(&D.ncdf_data.Mean_LAI, D.M_Mean_LAI);};

//Parameters and emergent quantities
if (O->SUPPORT_Cefficiency_OBS){   P=P+CARDAMOM_SINGLE_OBS_LIKELIHOOD(&D.ncdf_data.PEQ_Cefficiency, D.M_PEQ_Cefficiency);};
if (O->SUPPORT_CUE_OBS){   P=P+CARDAMOM_SINGLE_OBS_LIKELIHOOD(&D.ncdf_data.PEQ_CUE, D.M_PEQ_CUE);};
if (O->SUPPORT_iniSnow_OBS){   P=P+CARDAMOM_SINGLE_OBS_LIKELIHOOD(&D.ncdf_data.PEQ_iniSnow, D.M_PEQ_iniSnow);};
if (O->SUPPORT_C3frac_OBS){   P=P+CARDAMOM_SINGLE_OBS_LIKELIHOOD(&D.ncdf_data.PEQ_C3frac, D.M_PEQ_C3frac);};






/*Note: only use with model ID = 806*/
//if (D.ID==806){P = P + DALEC_806_MFCF(D);}
//if (D.ID==807){P = P + DALEC_807_MFCF(D);}
//if (D.ID==808){P = P + DALEC_807_MFCF(D);}

/*Constrain CMS disturbance fluxes*/
//if (D.otherpriors[6]>-9999){mnpdf=0;
/*Step 1. Sum of biomass -> litter emissions (including fire)*/
//for (n=0;n<D.nodays;n++){
//for (f=0;f<7;f++){mnpdf+=D.M_FLUXES[n*D.nofluxes+npdfi[f]];}}
/*Normalize npdf constraint to daily mean flux*/
//mnpdf=mnpdf/((double)D.nodays);
/*Step 2. Constrain against npdf*/
/*Only constrain if model flux < observed flux (otherwise mortality can explain internal cardamom fluxes)*/
//if (mnpdf<D.otherpriors[6]) {
//P=P-0.5*pow(log(mnpdf/D.otherpriors[6])/log(D.otherpriorunc[6]),2);
//}}
if (isnan(P)){P=log(0);}
return P;}

