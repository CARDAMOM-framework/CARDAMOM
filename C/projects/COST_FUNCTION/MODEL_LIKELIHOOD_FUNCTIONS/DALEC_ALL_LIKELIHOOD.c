#pragma once
#include "../../../math_fun/std.c"
#include "../../../math_fun/mean.c"
#include "../../../math_fun/max.c"
#include "../CARDAMOM_LIKELIHOOD_FUNCTION.c"
#include "../../CARDAMOM_MODELS/DALEC/DALEC_OBSERVATION_OPERATORS/DALEC_OBSERVATION_OPERATORS.c"




        //Time varying obs
        //Mean obs
        //PEQs

typedef struct LIKELIHOODinfo{
            int nolikelihoods;
}LIKELIHOODinfo;
    
            
            
struct LIKELIHOOD_INDICES{
        int ABGB;
        int CH4;
        int DOM;
        int ET;
        int EWT;
        int GPP;
        int SIF;
        int LAI;
        int NBE;
        int ROFF;
        int SCF;
        int Mean_ABGB;      
        int Mean_FIR;
        int Mean_GPP;
        int Mean_LAI;
        int PEQ_Cefficiency;       
        int PEQ_CUE;
        int PEQ_iniSnow;
        int PEQ_iniSOM;
        int PEQ_C3frac;
        int PEQ_Vcmax25;} LIKELIHOOD_INDICES={
     0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    10,11,12,13,14,15,16,17,18,19,
    20};
    
  
    
    int DALEC_ALL_LIKELIHOOD_MODCONFIG(LIKELIHOODinfo * LI){
        LI->nolikelihoods=21;
        return 0;}
            
            
            
            




/*Any likelihood functions used in multiple MLF functions are kept here!*/






/*for any other priors, explicitly define functions based on values in DATA.otherpriors*/

/*total biomass (pools 1:4) defined here - using first space of pappriors for total biomass*/


double LIKELIHOOD(DATA D){

//Step 1. Implement observarion operator on 
    




OBSOPE *O=&((DALEC *)D.MODEL)->OBSOPE;

//printf("O->SUPPORT_LAI_OBS = %d\n",O->SUPPORT_LAI_OBS);


//Observation operator on DALEC variables.
DALEC_OBSOPE(&D,O);

//printf("About to calculate likelihoods...\n");

//Abstracted likelihood indices
struct LIKELIHOOD_INDICES LI = LIKELIHOOD_INDICES;
double * ML=D.M_LIKELIHOODS;


//printf("O->SUPPORT_LAI_OBS = %d\n",O->SUPPORT_LAI_OBS);

//if (O->SUPPORT_ET_OBS){   P=DALEC_LIKELIHOOD_ET(D);}
// shuang added SIF operator, to enable using two seperate observations to constrain GPP and SIF at the same time, evaluating performance
if (O->SUPPORT_ABGB_OBS){ML[LI.ABGB]=CARDAMOM_TIMESERIES_OBS_LIKELIHOOD(&D.ncdf_data.ABGB, D.M_ABGB);};
if (O->SUPPORT_CH4_OBS){  ML[LI.CH4]=CARDAMOM_TIMESERIES_OBS_LIKELIHOOD(&D.ncdf_data.CH4, D.M_CH4);};
if (O->SUPPORT_DOM_OBS){   ML[LI.DOM]=CARDAMOM_TIMESERIES_OBS_LIKELIHOOD(&D.ncdf_data.DOM, D.M_DOM);};
if (O->SUPPORT_ET_OBS){   ML[LI.ET]=CARDAMOM_TIMESERIES_OBS_LIKELIHOOD(&D.ncdf_data.ET, D.M_ET);};
if (O->SUPPORT_EWT_OBS){   ML[LI.EWT]=CARDAMOM_TIMESERIES_OBS_LIKELIHOOD(&D.ncdf_data.EWT, D.M_EWT);};
if (O->SUPPORT_GPP_OBS){   ML[LI.GPP]=CARDAMOM_TIMESERIES_OBS_LIKELIHOOD(&D.ncdf_data.GPP, D.M_GPP);};
if (O->SUPPORT_SIF_OBS){   ML[LI.SIF]=CARDAMOM_TIMESERIES_OBS_LIKELIHOOD(&D.ncdf_data.SIF, D.M_GPP);}; // shuang
if (O->SUPPORT_LAI_OBS ){ML[LI.LAI]=CARDAMOM_TIMESERIES_OBS_LIKELIHOOD(&D.ncdf_data.LAI, D.M_LAI);};
if (O->SUPPORT_NBE_OBS){   ML[LI.NBE]=CARDAMOM_TIMESERIES_OBS_LIKELIHOOD(&D.ncdf_data.NBE, D.M_NBE);};
if (O->SUPPORT_ROFF_OBS){   ML[LI.ROFF]=CARDAMOM_TIMESERIES_OBS_LIKELIHOOD(&D.ncdf_data.ROFF, D.M_ROFF);};
if ( O->SUPPORT_SCF_OBS){ML[LI.SCF]=CARDAMOM_TIMESERIES_OBS_LIKELIHOOD(&D.ncdf_data.SCF, D.M_SCF);};

//Mean OBS


if (O->SUPPORT_ABGB_OBS){   ML[LI.Mean_ABGB]=CARDAMOM_SINGLE_OBS_LIKELIHOOD(&D.ncdf_data.Mean_ABGB, D.M_Mean_ABGB);};
if (O->SUPPORT_FIR_OBS){   ML[LI.Mean_FIR]=CARDAMOM_SINGLE_OBS_LIKELIHOOD(&D.ncdf_data.Mean_FIR, D.M_Mean_FIR);};
if (O->SUPPORT_GPP_OBS){   ML[LI.Mean_GPP]=CARDAMOM_SINGLE_OBS_LIKELIHOOD(&D.ncdf_data.Mean_GPP, D.M_Mean_GPP);};
if (O->SUPPORT_LAI_OBS){   ML[LI.Mean_LAI]=CARDAMOM_SINGLE_OBS_LIKELIHOOD(&D.ncdf_data.Mean_LAI, D.M_Mean_LAI);};

//Parameters and emergent quantities
if (O->SUPPORT_Cefficiency_OBS){   ML[LI.PEQ_Cefficiency]=CARDAMOM_SINGLE_OBS_LIKELIHOOD(&D.ncdf_data.PEQ_Cefficiency, D.M_PEQ_Cefficiency);};
if (O->SUPPORT_CUE_OBS){   ML[LI.PEQ_CUE]=CARDAMOM_SINGLE_OBS_LIKELIHOOD(&D.ncdf_data.PEQ_CUE, D.M_PEQ_CUE);};
if (O->SUPPORT_iniSnow_OBS){   ML[LI.PEQ_iniSnow]=CARDAMOM_SINGLE_OBS_LIKELIHOOD(&D.ncdf_data.PEQ_iniSnow, D.M_PEQ_iniSnow);};
if (O->SUPPORT_iniSOM_OBS){   ML[LI.PEQ_iniSOM]=CARDAMOM_SINGLE_OBS_LIKELIHOOD(&D.ncdf_data.PEQ_iniSOM, D.M_PEQ_iniSOM);};
if (O->SUPPORT_Vcmax25_OBS){   ML[LI.PEQ_Vcmax25]=CARDAMOM_SINGLE_OBS_LIKELIHOOD(&D.ncdf_data.PEQ_Vcmax25, D.M_PEQ_Vcmax25);};
if (O->SUPPORT_C3frac_OBS){   ML[LI.PEQ_C3frac]=CARDAMOM_SINGLE_OBS_LIKELIHOOD(&D.ncdf_data.PEQ_C3frac, D.M_PEQ_C3frac);};


//Calculate sum here;

int n;
double P=0;
for (n=0;n<D.nolikelihoods;n++){P=P+ML[n];}



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

