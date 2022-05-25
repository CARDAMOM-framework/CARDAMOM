#pragma once
#include "../DALEC_ALL/mean_pool.c"
#include "../DALEC_ALL/mean_annual_pool.c"
#include "../DALEC_ALL/expdecay2.c"
#include "../../../../math_fun/std.c"
#include "../../../../math_fun/ipow.c"
#include "stdlib.h"
#include "stdio.h"

//***************General inequality EDC******************
//***************DALEC_EDC_PARS_INEQUALITY*********
typedef struct {
    int nopools;
    int *nfin;
    int ** FIN;
    int *nfout;
    int ** FOUT;
} DALEC_EDC_TRAJECTORY_STRUCT;


//General inequality function
double DALEC_EDC_TRAJECTORY(DATA * DATA, void * EDCstruct){
    
    //Casting struct
    
     DALEC_EDC_TRAJECTORY_STRUCT  E = *(DALEC_EDC_TRAJECTORY_STRUCT * ) EDCstruct;

     int p;
      
 double PEDC = 0;
 
  double *TIME_INDEX=DATA->ncdf_data.TIME_INDEX.values;
  
  DALEC *MODEL=(DALEC *)DATA->MODEL;
    
  
  int N_timesteps=DATA.ncdf_data.TIME_INDEX.length;
  
  double EQF=DATA->ncdf_data.EDC_EQF;

     //Looping through all pools
     
     for (p=0;p<E->nopools;p++){
     //Pool inde

/*deriving mean pools here!*/
double MPOOLS;

MPOOLS=mean_pool(DATA->M_POOLS,p,N_timesteps+1,nopools);



/*deriving mean January pools*/
/*Assuming COMPLETE years*/
double MPOOLSjan;
/*pool interval*/


int dint=(int)floor(N_timesteps/(TIME_INDEX[N_timesteps-1]-TIME_INDEX[0])*365.25);
/*declaring mean pool array*/

/*deriving mean jan pools*/
/*based on all jan pools except initial conditions*/
for (n=0;n<noprogpools;n++){
for (m=0;m<(N_timesteps/dint+1);m++){
MPOOLSjan[n]=MPOOLSjan[n]+POOLS[nopools*(m*dint)+n]/(N_timesteps/dint+1);}}
/*printing just to make sure*/
/*for (n=0;n<nopools;n++){printf("Pool = %d, janmnean=%2.2f\n",n,MPOOLSjan[n]);}*/


/***********************EDCs start here****************************/



/*Total fluxes*/



//********CONTINUE FROM HERE********** sum all fluxes

FLUXES = DATA->M_FLUXES;



double *FT;
FT=calloc(nofluxes,sizeof(double));
int f=0;
for (f=0;f<nofluxes;f++){FT[f]=0;for (n=0;n<N_timesteps;n++){FT[f]+=FLUXES[n*nofluxes+f];}}
/*Total prec*/
double TOTAL_PREC=0;
double TOTAL_SNOW=0;
for (n=0;n<N_timesteps;n++){
    for (i=0;i<E->nfin;i++)
    Fin = FLUXES[E->
            Fout = 
    
    TOTAL_PREC+=PREC[n];TOTAL_SNOW+=SNOWFALL[n];}


Fi

// 
// double Fin[10];
// double Fout[10];
double Pstart;
double Pend;
/*temporary print switch*/
int psw=0;
/*exponential decay tolerance*/
double etol=0.1;

// 
// 
// 
// 
// /*Inputs and outputs for each pool*/
// /*labile*/
// Fin[S.C_lab]=FT[F.lab_prod];
// Fout[S.C_lab]=FT[F.lab_release]+FT[F.f_lab]+FT[F.fx_lab2lit];
// /*foliar*/
// Fin[S.C_fol]=FT[F.lab_release];
// Fout[S.C_fol]=FT[F.fol2lit]+FT[F.f_fol]+FT[F.fx_fol2lit];
// /*root*/
// Fin[S.C_roo]=FT[F.root_prod];
// Fout[S.C_roo]=FT[F.root2lit]+FT[F.f_roo]+FT[F.fx_roo2lit];
// /*wood*/
// Fin[S.C_woo]=FT[F.wood_prod];
// Fout[S.C_woo]=FT[F.wood2cwd]+FT[F.f_woo]+FT[F.fx_woo2cwd];
// /*CWD*/
// Fin[S.C_cwd]=FT[F.wood2cwd]+FT[F.fx_woo2cwd];
// Fout[S.C_cwd]=FT[F.ae_rh_cwd]+FT[F.an_rh_cwd]+FT[F.cwd2som]+FT[F.f_cwd]+FT[F.fx_cwd2som];
// /*litter*/
// Fin[S.C_lit]=FT[F.fol2lit]+FT[F.root2lit]+FT[F.fx_lab2lit]+FT[F.fx_fol2lit]+FT[F.fx_roo2lit];
// Fout[S.C_lit]=FT[F.ae_rh_lit]+FT[F.an_rh_lit]+FT[F.lit2som]+FT[F.f_lit]+FT[F.fx_lit2som];
// /*som*/
// Fin[S.C_som]=FT[F.cwd2som]+FT[F.lit2som]+FT[F.fx_cwd2som]+FT[F.fx_lit2som];
// Fout[S.C_som]=FT[F.ae_rh_som]+FT[F.an_rh_som]+FT[F.f_som];
// /*PAH2O*/
// Fin[S.H2O_PAW]=TOTAL_PREC-FT[F.q_surf]-TOTAL_SNOW+FT[F.melt];
// Fout[S.H2O_PAW]=FT[F.et]+FT[F.q_paw]+FT[F.paw2puw];
// /*PUH2O*/
// Fin[S.H2O_PUW]=FT[F.paw2puw];
// Fout[S.H2O_PUW]=FT[F.q_puw];
// /*SWE*/
// Fin[S.H2O_SWE]=TOTAL_SNOW;
// Fout[S.H2O_SWE]=FT[F.melt];



/*Inlcuding H2O pool*/
/*EDCs 7-13 - inputs, outputs and exponential tolerance*/

/*mean input/output ratio and start ratio*/
double Rm, Rs;



/*start and end pools*/
Pstart=POOLS[pidx];
Pend=POOLS[nopools*N_timesteps+piex];
/*mean input/output*/
Rm=Fin/Fout;
/*Theoretical starting input/output*/
Rs=Rm*MPOOLSjan[n]/Pstart;

/*if (((EDC==1 & DIAG==0) || DIAG==1 || (EDC==1 & DIAG==2 & EDCD->SWITCH[7-1+n]==1))
& ((fabs(log(Rs))>log(EQF)) || (fabs(Rs-Rm)>etol)))
{EDC=ipow(0,EDCD->SWITCH[7-1+n]);EDCD->PASSFAIL[7-1+n]=0;}*/
if (Fin[n] > 0){
 PEDC=EDCD->pEDC-0.5*pow(log(Rs)/log(EQF),2) - 0.5 *pow((Rs-Rm)/etol,2);


}

 
 
 
 
 

    
    return PEDC;
}



