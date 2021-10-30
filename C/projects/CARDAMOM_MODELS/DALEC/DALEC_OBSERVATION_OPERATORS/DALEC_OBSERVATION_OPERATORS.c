#pragma once
#include <stdbool.h>

///


//observation operator structure//
typedef struct OBSOPE{
bool SUPPORT_CH4_OBS;
//Need to fill this out...
int CH4_flux;
bool SUPPORT_GPP_OBS;
int GPP_flux;
bool SUPPORT_LAI_OBS;
int LAI_foliar_pool;
int LAI_LCMA;
bool SUPPORT_ET_OBS;
int ET_flux;
bool SUPPORT_NBE_OBS;
int * NBE_fluxes;
double *NBE_flux_signs;
int NBE_n_fluxes;
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
//
//Parameters and emergen quantities
bool SUPPORT_Cefficiency_OBS;
int Cefficiency_PARAM;///This is assuming it's a single parameter

bool SUPPORT_CUE_OBS;
int CUE_PARAM;//This is assuming it's a single parameter
//Can add more parameters OR options
        
bool SUPPORT_iniSOM_OBS;
int iniSOM_PARAM;//This is assuming it's a single parameter
//Can add more parameters OR options


}OBSOPE;


int INITIALIZE_OBSOPE_SUPPORT(OBSOPE * OBSOPE){

OBSOPE->SUPPORT_CH4_OBS=false;
OBSOPE->SUPPORT_GPP_OBS=false;
OBSOPE->SUPPORT_LAI_OBS=false;
OBSOPE->SUPPORT_ET_OBS=false;
OBSOPE->SUPPORT_NBE_OBS=false;
OBSOPE->SUPPORT_ABGB_OBS=false;
OBSOPE->SUPPORT_DOM_OBS=false;
OBSOPE->SUPPORT_EWT_OBS=false;
OBSOPE->SUPPORT_FIR_OBS=false;


OBSOPE->SUPPORT_Cefficiency_OBS=false;
OBSOPE->SUPPORT_CUE_OBS=false;
OBSOPE->SUPPORT_iniSOM_OBS=false;



return 0;
}





//LAI observation operator
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
if (SOBS.value!=DEFAULT_DOUBLE_VAL){int n;D->M_Mean_ABGB=0;for (n=0;n<N;n++){D->M_Mean_ABGB+=D->M_ABGB[n];};D->M_Mean_ABGB=D->M_Mean_ABGB/(double)N;};


return 0;}


int DALEC_OBSOPE_CH4(DATA * D, OBSOPE * O){
return 0;}

//ET observation operator, assuming one flux


int DALEC_OBSOPE_ET(DATA * D, OBSOPE * O){

int N=D->ncdf_data.TIME_INDEX.length;
TIMESERIES_OBS_STRUCT TOBS=D->ncdf_data.ET;


if (TOBS.valid_obs_length>0){int n;for (n=0;n<N;n++){D->M_ET[n]=D->M_FLUXES[D->nofluxes*n+O->ET_flux];}};


return 0;}




//GRACE H2O timeseries
int DALEC_OBSOPE_EWT(DATA * D, OBSOPE * O){

    
TIMESERIES_OBS_STRUCT TOBS=D->ncdf_data.EWT;


if (TOBS.valid_obs_length>0){int n,nn;

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

if (SOBS.value!=DEFAULT_DOUBLE_VAL){int n;D->M_Mean_FIR=0;for (n=0;n<N;n++){D->M_Mean_FIR+=D->M_FLUXES[n*D->nofluxes+O->FIR_flux];};D->M_Mean_FIR=D->M_Mean_FIR/(double)N;}


return 0;}





int DALEC_OBSOPE_GPP(DATA * D, OBSOPE * O){

//GPP timeseries length
int N=D->ncdf_data.TIME_INDEX.length;

//Time varying GPP 
TIMESERIES_OBS_STRUCT TOBS=D->ncdf_data.GPP;
if (TOBS.valid_obs_length>0){int n;for (n=0;n<N;n++){D->M_GPP[n]=D->M_FLUXES[D->nofluxes*n+O->GPP_flux];}};


//Mean GPP 
SINGLE_OBS_STRUCT SOBS=D->ncdf_data.Mean_GPP;
if (SOBS.value!=DEFAULT_DOUBLE_VAL){int n;D->M_Mean_GPP=0;for (n=0;n<N;n++){D->M_Mean_GPP+=D->M_FLUXES[n*D->nofluxes+O->GPP_flux];};D->M_Mean_GPP=D->M_Mean_GPP/(double)N;}

return 0;}





//NBE observation operator
// R
int DALEC_OBSOPE_NBE(DATA * D, OBSOPE * O){
 

int N=D->ncdf_data.TIME_INDEX.length;
TIMESERIES_OBS_STRUCT TOBS=D->ncdf_data.NBE;


if (TOBS.valid_obs_length>0){
int n,nn;
for (n=0;n<N;n++){
D->M_NBE[n]=0;
for (nn=0;nn<O->NBE_n_fluxes;nn++){
D->M_NBE[n]+=D->M_FLUXES[D->nofluxes*n+O->NBE_fluxes[nn]]*O->NBE_flux_signs[nn];}}};




return 0;}




//LAI observation operator
int DALEC_OBSOPE_LAI(DATA * D, OBSOPE * O){
//int folc_pool,double lcma_pars_index
    
//GPP timeseries length

int N=D->ncdf_data.TIME_INDEX.length;



//Time varying GPP and mean GPP
TIMESERIES_OBS_STRUCT TOBS=D->ncdf_data.LAI;
SINGLE_OBS_STRUCT SOBS=D->ncdf_data.Mean_LAI;

if (TOBS.valid_obs_length>0 || SOBS.value!=DEFAULT_DOUBLE_VAL){int n;
for (n=0;n<N;n++){
    D->M_LAI[n]=(D->M_POOLS[D->nopools*n+O->LAI_foliar_pool]+D->M_POOLS[D->nopools*(n+1)+O->LAI_foliar_pool])*0.5/D->M_PARS[O->LAI_LCMA];
}}

//Mean LAI
if (SOBS.value!=DEFAULT_DOUBLE_VAL){int n;D->M_Mean_LAI=0;for (n=0;n<N;n++){D->M_Mean_LAI+=D->M_LAI[n];}D->M_Mean_LAI=D->M_Mean_LAI/(double)N;}


return 0;}










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
if  (SOBS.value!=DEFAULT_DOUBLE_VAL){D->M_PEQ_Cefficiency=D->M_PARS[O->Cefficiency_PARAM];}
return 0;

}

int DALEC_OBSOPE_CUE(DATA * D, OBSOPE * O){
    SINGLE_OBS_STRUCT SOBS=D->ncdf_data.PEQ_CUE;
if  (SOBS.value!=DEFAULT_DOUBLE_VAL){D->M_PEQ_CUE=D->M_PARS[O->CUE_PARAM];}
return 0;

}

int DALEC_OBSOPE_iniSOM(DATA * D, OBSOPE * O){
    SINGLE_OBS_STRUCT SOBS=D->ncdf_data.PEQ_iniSOM;
if  (SOBS.value!=DEFAULT_DOUBLE_VAL){D->M_PEQ_iniSOM=D->M_PARS[O->iniSOM_PARAM];}
return 0;

}



///Full observation operator
int DALEC_OBSOPE(DATA * D, OBSOPE * O){

if (O->SUPPORT_CH4_OBS){DALEC_OBSOPE_CH4(D, O);}
if (O->SUPPORT_GPP_OBS){DALEC_OBSOPE_GPP(D, O);}
if (O->SUPPORT_LAI_OBS){DALEC_OBSOPE_LAI(D, O);}
if (O->SUPPORT_ET_OBS){DALEC_OBSOPE_ET(D, O);}
if (O->SUPPORT_NBE_OBS){DALEC_OBSOPE_NBE(D, O);}
if (O->SUPPORT_ABGB_OBS){DALEC_OBSOPE_ABGB(D, O);}
if (O->SUPPORT_DOM_OBS){DALEC_OBSOPE_DOM(D, O);}
if (O->SUPPORT_EWT_OBS){DALEC_OBSOPE_EWT(D, O);}
if (O->SUPPORT_FIR_OBS){DALEC_OBSOPE_FIR(D, O);}

//Parameters

if (O->SUPPORT_Cefficiency_OBS){DALEC_OBSOPE_Cefficiency(D, O);}
if (O->SUPPORT_CUE_OBS){DALEC_OBSOPE_CUE(D, O);}
if (O->SUPPORT_iniSOM_OBS){DALEC_OBSOPE_iniSOM(D, O);}


return 0;}  




//













