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
bool SUPPORT_SOM_OBS;
int * SOM_pools;
int SOM_n_pools;
bool SUPPORT_GRACE_EWT_OBS;
int * GRACE_EWT_h2o_pools;
int GRACE_EWT_n_h2o_pools;
bool SUPPORT_FIR_OBS;
int FIR_flux;
}OBSOPE;


int INITIALIZE_OBSOPE_SUPPORT(OBSOPE * OBSOPE){

OBSOPE->SUPPORT_CH4_OBS=false;
OBSOPE->SUPPORT_GPP_OBS=false;
OBSOPE->SUPPORT_LAI_OBS=false;
OBSOPE->SUPPORT_ET_OBS=false;
OBSOPE->SUPPORT_NBE_OBS=false;
OBSOPE->SUPPORT_ABGB_OBS=false;
OBSOPE->SUPPORT_SOM_OBS=false;
OBSOPE->SUPPORT_GRACE_EWT_OBS=false;
OBSOPE->SUPPORT_FIR_OBS=false;

printf("So far so good (false declarations)\n");
return 0;
}





int DALEC_OBSOPE_GPP(DATA * D, OBSOPE * O){

//Time varying GPP 
if (D->ngpp>0){int n;for (n=0;n<D->nodays;n++){D->M_GPP[n]=D->M_FLUXES[D->nofluxes*n+O->GPP_flux];}};

//Mean GPP 
if (D->otherpriors[5]>-9999){int n;D->M_MGPP[0]=0;for (n=0;n<D->nodays;n++){D->M_MGPP[0]+=D->M_FLUXES[n*D->nofluxes+O->GPP_flux];};D->M_MGPP[0]=D->M_MGPP[0]/(double)D->nodays;}

printf("So far so good (inside GPP cost function)\n");

return 0;}


int DALEC_OBSOPE_CH4(DATA * D, OBSOPE * O){
return 0;
}

//Mean fire co2 flux
int DALEC_OBSOPE_FIR(DATA * D, OBSOPE * O){

if (D->otherpriors[2]>-9999){int n;D->M_MFIRE[0]=0;for (n=0;n<D->nodays;n++){D->M_MFIRE[0]+=D->M_FLUXES[n*D->nofluxes+O->FIR_flux];};D->M_MFIRE[0]=D->M_MFIRE[0]/(double)D->nodays;}


printf("So far so good (inside FIRE cost function)\n");

return 0;}





//NBE observation operator
// R
int DALEC_OBSOPE_NBE(DATA * D, OBSOPE * O){
  
if (D->nnbe>0){
int n,nn;
for (n=0;n<D->nodays;n++){
D->M_NBE[n]=0;
for (nn=0;nn<O->NBE_n_fluxes;nn++){
D->M_NBE[n]+=D->M_FLUXES[D->nofluxes*n+O->NBE_fluxes[nn]]*O->NBE_flux_signs[nn];}}};



printf("So far so good (inside NBE cost function)\n");

return 0;}

//ET observation operator, assuming one flux
int DALEC_OBSOPE_ET(DATA * D, OBSOPE * O){
  
if (D->net>0){int n;for (n=0;n<D->nodays;n++){D->M_ET[n]=D->M_FLUXES[D->nofluxes*n+O->ET_flux];}};


printf("So far so good (inside ET cost function)\n");

return 0;}


//LAI observation operator
int DALEC_OBSOPE_LAI(DATA * D, OBSOPE * O){
//int folc_pool,double lcma_pars_index
printf("inside LAI OBSOPE\n");

int n=0;
printf("(D->M_POOLS[D->nopools*n+O->LAI_foliar_pool] = %2.2f\n",(D->M_POOLS[D->nopools*n+O->LAI_foliar_pool]));
if (D->otherpriors[4]>0 | D->nlai>0){int n;for (n=0;n<D->nodays;n++){D->M_LAI[n]=(D->M_POOLS[D->nopools*n+O->LAI_foliar_pool]+D->M_POOLS[D->nopools*(n+1)+O->LAI_foliar_pool])*0.5/D->M_PARS[O->LAI_LCMA];}};


printf("done with LAI OBSOPE\n");
return 0;}


//GRACE H2O timeseries
int DALEC_OBSOPE_GRACE_EWT(DATA * D, OBSOPE * O){
//CONTINUR FROM HERE
if (D->newt>0){int n,nn;
int * h2op=O->GRACE_EWT_h2o_pools;

for (n=0;n<D->nodays;n++){D->M_EWT[n]=0;
for (nn=0;nn<O->GRACE_EWT_n_h2o_pools;nn++){
D->M_EWT[n]+=(D->M_POOLS[D->nopools*n+h2op[nn]] +D->M_POOLS[D->nopools*(n+1)+h2op[nn]])*0.5;}}}
return 0;}



int DALEC_OBSOPE_ABGB(DATA * D, OBSOPE * O){



if (D->otherpriors[0]>-9999){

int nn;
D->M_ABGB_t0[0]=0;
for (nn=0;nn<O->ABGB_n_pools;nn++){D->M_ABGB_t0[0]+=D->M_POOLS[O->ABGB_pools[nn]];}
}


if (D->nabgb>0){
//declarations
int p,nxp,n,nn;

for (n=0;n<D->nodays;n++){
//indices
nxp=D->nopools*(n+1);p=D->nopools*n;

//inialize
D->M_ABGB[n]=0;
//loop through pools
for (nn=0;nn<O->ABGB_n_pools;nn++){
D->M_ABGB[n]+=(D->M_POOLS[p+O->ABGB_pools[nn]]+D->M_POOLS[nxp+O->ABGB_pools[nn]])*0.5;}

}}


return 0;
}







int DALEC_OBSOPE_SOM(DATA * D, OBSOPE * O){



if (D->nsom>0){
//declare constanta
int p,nxp,n,nn;
//loop through days
for (n=0;n<D->nodays;n++){
//declarations

//indices
nxp=D->nopools*(n+1);p=D->nopools*n;

//inialize
D->M_SOM[n]=0;
//loop through pools
for (nn=0;nn<O->SOM_n_pools;nn++){
D->M_SOM[n]+=(D->M_POOLS[p+O->SOM_pools[nn]]+D->M_POOLS[nxp+O->SOM_pools[nn]])*0.5;}

}}


return 0;
}



///Full observation operator
int DALEC_OBSOPE(DATA * D, OBSOPE * O){

if (O->SUPPORT_CH4_OBS){DALEC_OBSOPE_CH4(D, O);}
if (O->SUPPORT_GPP_OBS){DALEC_OBSOPE_GPP(D, O);}
printf("Done with GPP OBSOPE\n");
if (O->SUPPORT_LAI_OBS){DALEC_OBSOPE_LAI(D, O);}
printf("Done with LAI OBSOPE\n");
if (O->SUPPORT_ET_OBS){DALEC_OBSOPE_ET(D, O);}
if (O->SUPPORT_NBE_OBS){DALEC_OBSOPE_NBE(D, O);}
if (O->SUPPORT_ABGB_OBS){DALEC_OBSOPE_ABGB(D, O);}
if (O->SUPPORT_SOM_OBS){DALEC_OBSOPE_SOM(D, O);}
if (O->SUPPORT_GRACE_EWT_OBS){DALEC_OBSOPE_GRACE_EWT(D, O);}
if (O->SUPPORT_FIR_OBS){DALEC_OBSOPE_FIR(D, O);}

return 0;}




//













