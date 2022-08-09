#pragma once
#include <stdbool.h>

///


//observation operator structure//
typedef struct OBSOPE{
bool SUPPORT_CH4_OBS;
//Need to fill this out...
int CH4_flux;
//Woody C (CWOO)
bool SUPPORT_CWOO_OBS;
int * CWOO_pools;
int CWOO_n_pools;
//Gross primary productivity (GPP)
bool SUPPORT_GPP_OBS;
int GPP_flux;
// shuang added SIF operator so that both GPP and SIF can be compared at the same time, with different observation datasets, eg. TROPOMI SIF and FluxSat GPP
bool SUPPORT_SIF_OBS;
bool SUPPORT_LAI_OBS;
int LAI_pool;
bool SUPPORT_ET_OBS;
int ET_flux;
bool SUPPORT_ROFF_OBS;
int * ROFF_fluxes;
double *ROFF_flux_signs;
int ROFF_n_fluxes;
bool SUPPORT_NBE_OBS;
int * NBE_fluxes;
double *NBE_flux_signs;
int NBE_n_fluxes;
//ABove and below ground biomass
bool SUPPORT_ABGB_OBS;
int * ABGB_pools;
int ABGB_n_pools;
//Total dead organic matter
bool SUPPORT_DOM_OBS;
int * DOM_pools;
int DOM_n_pools;
//Total water storage (equivalent water thickness)
bool SUPPORT_EWT_OBS;
int * EWT_h2o_pools;
int EWT_n_h2o_pools;
//Total fire C emissions
bool SUPPORT_FIR_OBS;
int FIR_flux;
bool SUPPORT_SCF_OBS;
int SCF_pool;


//
//Parameters and emergen quantities
bool SUPPORT_Cefficiency_OBS;
int Cefficiency_PARAM;///This is assuming it's a single parameter

bool SUPPORT_CUE_OBS;
int CUE_PARAM;//This is assuming it's a single parameter
//Can add more parameters OR options
        
bool SUPPORT_C3frac_OBS;
int C3frac_PARAM;//This is assuming it's a single parameter
//Can add more parameters OR options
        
bool SUPPORT_Vcmax25_OBS;
int Vcmax25_PARAM;//This is assuming it's a single parameter
//Can add more parameters OR options
        
bool SUPPORT_iniSnow_OBS;
int iniSnow_PARAM;//This is assuming it's a single parameter
//Can add more parameters OR options
        
bool SUPPORT_iniSOM_OBS;
int iniSOM_PARAM;//This is assuming it's a single parameter
//Can add more parameters OR options




}OBSOPE;


int INITIALIZE_OBSOPE_SUPPORT(OBSOPE * OBSOPE){

OBSOPE->SUPPORT_ABGB_OBS=false;
OBSOPE->SUPPORT_CH4_OBS=false;
OBSOPE->SUPPORT_CWOO_OBS=false;
OBSOPE->SUPPORT_DOM_OBS=false;
OBSOPE->SUPPORT_ET_OBS=false;
OBSOPE->SUPPORT_EWT_OBS=false;
OBSOPE->SUPPORT_FIR_OBS=false;
OBSOPE->SUPPORT_GPP_OBS=false;
OBSOPE->SUPPORT_SIF_OBS=false;
OBSOPE->SUPPORT_LAI_OBS=false;
OBSOPE->SUPPORT_NBE_OBS=false;
OBSOPE->SUPPORT_ROFF_OBS=false;
OBSOPE->SUPPORT_SCF_OBS=false;


OBSOPE->SUPPORT_Cefficiency_OBS=false;
OBSOPE->SUPPORT_CUE_OBS=false;
OBSOPE->SUPPORT_C3frac_OBS=false;
OBSOPE->SUPPORT_Vcmax25_OBS=false;
OBSOPE->SUPPORT_iniSnow_OBS=false;
OBSOPE->SUPPORT_iniSOM_OBS=false;
//In-built observation operators


return 0;
}





//ABGB observation operator
int DALEC_OBSOPE_ABGB(DATA * D, OBSOPE * O){
//int folc_pool,double lcma_pars_index
    
//GPP timeseries length
int N=D->ncdf_data.TIME_INDEX.length;

//Time varying GPP and mean GPP
TIMESERIES_OBS_STRUCT TOBS=D->ncdf_data.ABGB;
SINGLE_OBS_STRUCT SOBS=D->ncdf_data.Mean_ABGB;

//Calculating full time series in case needed for tim
if (TOBS.valid_obs_length>0 || SOBS.value!=DEFAULT_DOUBLE_VAL){
    //declarations
int p,nxp,n,nn;
//Looping through timesteps
for (n=0;n<N;n++){
//indices
nxp=D->nopools*(n+1);p=D->nopools*n;
//inialize
D->M_ABGB[n]=0;
//loop through pools
for (nn=0;nn<O->ABGB_n_pools;nn++){
D->M_ABGB[n]+=(D->M_POOLS[p+O->ABGB_pools[nn]]+D->M_POOLS[nxp+O->ABGB_pools[nn]])*0.5;}  
}}

//Mean ABGB
if (SOBS.validobs){int n;D->M_Mean_ABGB=0;for (n=0;n<N;n++){D->M_Mean_ABGB+=D->M_ABGB[n];};D->M_Mean_ABGB=D->M_Mean_ABGB/(double)N;};


return 0;}


int DALEC_OBSOPE_CH4(DATA * D, OBSOPE * O){

int N=D->ncdf_data.TIME_INDEX.length;
TIMESERIES_OBS_STRUCT TOBS=D->ncdf_data.CH4;


if (TOBS.validobs){int n;for (n=0;n<N;n++){D->M_CH4[n]=D->M_FLUXES[D->nofluxes*n+O->CH4_flux];}};

return 0;}

//ET observation operator, assuming one flux


int DALEC_OBSOPE_ET(DATA * D, OBSOPE * O){

int N=D->ncdf_data.TIME_INDEX.length;
TIMESERIES_OBS_STRUCT TOBS=D->ncdf_data.ET;


if (TOBS.validobs){int n;for (n=0;n<N;n++){D->M_ET[n]=D->M_FLUXES[D->nofluxes*n+O->ET_flux];}};


return 0;}

// Runoff operator

int DALEC_OBSOPE_ROFF(DATA * D, OBSOPE * O){

int N=D->ncdf_data.TIME_INDEX.length;
TIMESERIES_OBS_STRUCT TOBS=D->ncdf_data.ROFF;


if (TOBS.validobs){
int n,nn;
for (n=0;n<N;n++){
D->M_ROFF[n]=0;
for (nn=0;nn<O->ROFF_n_fluxes;nn++){
D->M_ROFF[n]+=D->M_FLUXES[D->nofluxes*n+O->ROFF_fluxes[nn]];}}};


return 0;}



//GRACE H2O timeseries
int DALEC_OBSOPE_EWT(DATA * D, OBSOPE * O){

    
TIMESERIES_OBS_STRUCT TOBS=D->ncdf_data.EWT;


if (TOBS.validobs){int n,nn;

int N=D->ncdf_data.TIME_INDEX.length;
int * h2op=O->EWT_h2o_pools;



for (n=0;n<N;n++){D->M_EWT[n]=0;
for (nn=0;nn<O->EWT_n_h2o_pools;nn++){
D->M_EWT[n]+=(D->M_POOLS[D->nopools*n+h2op[nn]] +D->M_POOLS[D->nopools*(n+1)+h2op[nn]])*0.5;}}}


return 0;}




//Mean fire co2 flux
int DALEC_OBSOPE_FIR(DATA * D, OBSOPE * O){

int N=D->ncdf_data.TIME_INDEX.length;

SINGLE_OBS_STRUCT SOBS=D->ncdf_data.Mean_FIR;

if (SOBS.validobs){int n;D->M_Mean_FIR=0;for (n=0;n<N;n++){D->M_Mean_FIR+=D->M_FLUXES[n*D->nofluxes+O->FIR_flux];};D->M_Mean_FIR=D->M_Mean_FIR/(double)N;}


return 0;}





int DALEC_OBSOPE_GPP(DATA * D, OBSOPE * O){

//GPP timeseries length
int N=D->ncdf_data.TIME_INDEX.length;

//Time varying GPP 
TIMESERIES_OBS_STRUCT TOBS=D->ncdf_data.GPP;
if (TOBS.validobs){int n;for (n=0;n<N;n++){D->M_GPP[n]=D->M_FLUXES[D->nofluxes*n+O->GPP_flux];}};


//Mean GPP 
SINGLE_OBS_STRUCT SOBS=D->ncdf_data.Mean_GPP;
if (SOBS.validobs){int n;D->M_Mean_GPP=0;for (n=0;n<N;n++){D->M_Mean_GPP+=D->M_FLUXES[n*D->nofluxes+O->GPP_flux];};D->M_Mean_GPP=D->M_Mean_GPP/(double)N;}

return 0;}


// SIF operator is made the same as GPP, be careful with its opt_filter and opt_normalization set up - shuang
// Mean_SIF is not available since current model does not yet simulate SIF - shuang

int DALEC_OBSOPE_SIF(DATA * D, OBSOPE * O){

//SIF timeseries length, here DATA refer to input obs, OBSOPE refer to model output mapped to mathch data
int N=D->ncdf_data.TIME_INDEX.length;

//Time varying SIF 
TIMESERIES_OBS_STRUCT TOBS=D->ncdf_data.SIF;
if (TOBS.validobs){int n;for (n=0;n<N;n++){D->M_SIF[n]=D->M_FLUXES[D->nofluxes*n+O->GPP_flux];}};


return 0;}



//NBE observation operator
// R
int DALEC_OBSOPE_NBE(DATA * D, OBSOPE * O){
 

int N=D->ncdf_data.TIME_INDEX.length;
TIMESERIES_OBS_STRUCT TOBS=D->ncdf_data.NBE;


if (TOBS.validobs){
int n,nn;
for (n=0;n<N;n++){
D->M_NBE[n]=0;
for (nn=0;nn<O->NBE_n_fluxes;nn++){
D->M_NBE[n]+=D->M_FLUXES[D->nofluxes*n+O->NBE_fluxes[nn]]*O->NBE_flux_signs[nn];}}};




return 0;}




//LAI observation operator
int DALEC_OBSOPE_LAI(DATA * D, OBSOPE * O){

//int folc_pool,double lcma_pars_index
    
//Run length
int N=D->ncdf_data.TIME_INDEX.length;

//Time varying GPP and mean GPP
TIMESERIES_OBS_STRUCT TOBS=D->ncdf_data.LAI;
SINGLE_OBS_STRUCT SOBS=D->ncdf_data.Mean_LAI;


if ((TOBS.valid_obs_length>0 || SOBS.value!=DEFAULT_DOUBLE_VAL) ){int n;
for (n=0;n<N;n++){
    D->M_LAI[n]=(D->M_POOLS[D->nopools*n+O->LAI_pool]+D->M_POOLS[D->nopools*(n+1)+O->LAI_pool])*0.5;
}}

//Mean LAI
if (SOBS.validobs){int n;D->M_Mean_LAI=0;for (n=0;n<N;n++){D->M_Mean_LAI+=D->M_LAI[n];}D->M_Mean_LAI=D->M_Mean_LAI/(double)N;}

return 0;}




//SCF observation operator
int DALEC_OBSOPE_SCF(DATA * D, OBSOPE * O){
//int folc_pool,double lcma_pars_index

    
//Run length
int N=D->ncdf_data.TIME_INDEX.length;

//Time varying GPP and mean GPP
TIMESERIES_OBS_STRUCT TOBS=D->ncdf_data.SCF;


if ((TOBS.valid_obs_length>0) ){int n;
for (n=0;n<N;n++){

    D->M_SCF[n]=(D->M_POOLS[D->nopools*n+O->SCF_pool]+D->M_POOLS[D->nopools*(n+1)+O->SCF_pool])*0.5;
}}


return 0;}






int DALEC_OBSOPE_CWOO(DATA * D, OBSOPE * O){

int N=D->ncdf_data.TIME_INDEX.length;
TIMESERIES_OBS_STRUCT TOBS=D->ncdf_data.CWOO;



if (TOBS.valid_obs_length>0){
//declare constanta
int p,nxp,n,nn;
//loop through days
for (n=0;n<N;n++){
//declarations

//indices
nxp=D->nopools*(n+1);p=D->nopools*n;

//inialize
D->M_CWOO[n]=0;
//loop through pools
for (nn=0;nn<O->CWOO_n_pools;nn++){
D->M_CWOO[n]+=(D->M_CWOO[p+O->CWOO_pools[nn]]+D->M_CWOO[nxp+O->CWOO_pools[nn]])*0.5;}
}}


return 0;
}







int DALEC_OBSOPE_DOM(DATA * D, OBSOPE * O){

int N=D->ncdf_data.TIME_INDEX.length;
TIMESERIES_OBS_STRUCT TOBS=D->ncdf_data.DOM;



if (TOBS.valid_obs_length>0){
//declare constanta
int p,nxp,n,nn;
//loop through days
for (n=0;n<N;n++){
//declarations

//indices
nxp=D->nopools*(n+1);p=D->nopools*n;

//inialize
D->M_DOM[n]=0;
//loop through pools
for (nn=0;nn<O->DOM_n_pools;nn++){
D->M_DOM[n]+=(D->M_POOLS[p+O->DOM_pools[nn]]+D->M_POOLS[nxp+O->DOM_pools[nn]])*0.5;}
}}


return 0;
}




//Parameter and emergent quantity observation operators

int DALEC_OBSOPE_Cefficiency(DATA * D, OBSOPE * O){
    SINGLE_OBS_STRUCT SOBS=D->ncdf_data.PEQ_Cefficiency;
if  (SOBS.validobs){D->M_PEQ_Cefficiency=D->M_PARS[O->Cefficiency_PARAM];}
return 0;

}

int DALEC_OBSOPE_CUE(DATA * D, OBSOPE * O){
    SINGLE_OBS_STRUCT SOBS=D->ncdf_data.PEQ_CUE;
if  (SOBS.validobs){D->M_PEQ_CUE=1-D->M_PARS[O->CUE_PARAM];} // modeled CUE = 1-par(f_auto)
return 0;

}

int DALEC_OBSOPE_C3frac(DATA * D, OBSOPE * O){
    SINGLE_OBS_STRUCT SOBS=D->ncdf_data.PEQ_C3frac;
if  (SOBS.validobs){D->M_PEQ_C3frac=D->M_PARS[O->C3frac_PARAM];}
return 0;

}

int DALEC_OBSOPE_Vcmax25(DATA * D, OBSOPE * O){
    SINGLE_OBS_STRUCT SOBS=D->ncdf_data.PEQ_Vcmax25;
if  (SOBS.validobs){D->M_PEQ_Vcmax25=D->M_PARS[O->Vcmax25_PARAM];}
return 0;

}

int DALEC_OBSOPE_iniSnow(DATA * D, OBSOPE * O){
    SINGLE_OBS_STRUCT SOBS=D->ncdf_data.PEQ_iniSnow;
if  (SOBS.validobs){D->M_PEQ_iniSnow=D->M_PARS[O->iniSnow_PARAM];}
return 0;

}
int DALEC_OBSOPE_iniSOM(DATA * D, OBSOPE * O){
    SINGLE_OBS_STRUCT SOBS=D->ncdf_data.PEQ_iniSOM;
if  (SOBS.validobs){D->M_PEQ_iniSOM=D->M_PARS[O->iniSOM_PARAM];}
return 0;

}



///Full observation operator
int DALEC_OBSOPE(DATA * D, OBSOPE * O){

if (O->SUPPORT_ABGB_OBS){DALEC_OBSOPE_ABGB(D, O);}
if (O->SUPPORT_CH4_OBS){DALEC_OBSOPE_CH4(D, O);}
if (O->SUPPORT_CWOO_OBS){DALEC_OBSOPE_CWOO(D, O);}
if (O->SUPPORT_ET_OBS){DALEC_OBSOPE_ET(D, O);}
if (O->SUPPORT_EWT_OBS){DALEC_OBSOPE_EWT(D, O);}
if (O->SUPPORT_FIR_OBS){DALEC_OBSOPE_FIR(D, O);}
if (O->SUPPORT_GPP_OBS){DALEC_OBSOPE_GPP(D, O);}
if (O->SUPPORT_LAI_OBS ){DALEC_OBSOPE_LAI(D, O);}
if (O->SUPPORT_NBE_OBS){DALEC_OBSOPE_NBE(D, O);}
if (O->SUPPORT_ROFF_OBS){DALEC_OBSOPE_ROFF(D, O);}
if (O->SUPPORT_SCF_OBS ){DALEC_OBSOPE_SCF(D, O);}

//Parameters

if (O->SUPPORT_Cefficiency_OBS){DALEC_OBSOPE_Cefficiency(D, O);}
if (O->SUPPORT_CUE_OBS){DALEC_OBSOPE_CUE(D, O);}
if (O->SUPPORT_C3frac_OBS){DALEC_OBSOPE_C3frac(D, O);}
if (O->SUPPORT_Vcmax25_OBS){DALEC_OBSOPE_Vcmax25(D, O);}
if (O->SUPPORT_iniSnow_OBS){DALEC_OBSOPE_iniSnow(D, O);}
if (O->SUPPORT_iniSOM_OBS){DALEC_OBSOPE_iniSOM(D, O);}


return 0;}  




//













