#pragma once
#include "../DALEC_ALL/mean_pool.c"
#include "../DALEC_ALL/mean_annual_pool.c"
#include "../DALEC_ALL/expdecay2.c"
#include "../../../../math_fun/std.c"
#include "../../../../math_fun/ipow.c"
#include "stdlib.h"
#include "stdio.h"


int EDC2_1006(double const *pars, DATA DATA, struct EDCDIAGNOSTIC *EDCD)
{

struct DALEC_1006_PARAMETERS P=DALEC_1006_PARAMETERS;
struct DALEC_1006_FLUXES F=DALEC_1006_FLUXES;
struct DALEC_1006_POOLS S=DALEC_1006_POOLS;

/*Extract DALEC model here*/
/*Copy model pointer for brevity*/
DALEC *MODEL=(DALEC *)DATA.MODEL;

double *PREC=DATA.ncdf_data.TOTAL_PREC.values;
double *TIME_INDEX=DATA.ncdf_data.TIME_INDEX.values;
double *POOLS=DATA.M_POOLS;
double *FLUXES=DATA.M_FLUXES;
int N_timesteps=DATA.ncdf_data.TIME_INDEX.length;
double *parmax=DATA.parmax;
double meantemp = (DATA.ncdf_data.T2M_MAX.reference_mean + DATA.ncdf_data.T2M_MIN.reference_mean)/2;
/*EDCD=EDCD2;*/

/* EDC2_SUMMARY
6. Cfol:Croo
7-14. Pool trajectory EDCs
15. Ensuring that wilting point is at or below the mean H2O pool
35.
*/

/*THESE EDC2 checks are for DALEC_FIREBUCKET3d*/
int EDC=1,n=0,m=0,edc=0;
int DIAG=EDCD->DIAG;/*1 or 0*/



/*FIREBUCKET*/
int nopools=MODEL->nopools;
int noprogpools=8;//Prognostic pools only for EDCs
int nofluxes=MODEL->nofluxes;
int done=0;
int k=0;



/*deriving mean pools here!*/
double *MPOOLS;
MPOOLS=calloc(nopools,sizeof(double));
if (MPOOLS==0){printf("WARNING NULL POINTER");}
for (n=0;n<noprogpools;n++){MPOOLS[n]=mean_pool(POOLS,n,N_timesteps+1,nopools);};

/*deriving mean January pools*/
/*Assuming COMPLETE years*/
double *MPOOLSjan;
/*pool interval*/
int dint=(int)floor(N_timesteps/(TIME_INDEX[N_timesteps-1]-TIME_INDEX[0])*365.25);
/*declaring mean pool array*/
MPOOLSjan=calloc(nopools,sizeof(double));if (MPOOLSjan==0){printf("WARNING NULL POINTER");}
/*deriving mean jan pools*/
/*based on all jan pools except initial conditions*/
for (n=0;n<noprogpools;n++){
for (m=0;m<(N_timesteps/dint+1);m++){
MPOOLSjan[n]=MPOOLSjan[n]+POOLS[nopools*(m*dint)+n]/(N_timesteps/dint+1);}}
/*printing just to make sure*/
/*for (n=0;n<nopools;n++){printf("Pool = %d, janmnean=%2.2f\n",n,MPOOLSjan[n]);}*/


/***********************EDCs start here****************************/


/*EDC no 6*/
/*0.2*Cf < Cr < 5*Cf*/
/*Cfoliar : Croot = 5:1 or 1:5*/
EDCD->pEDC=EDCD->pEDC-0.5*pow(log(MPOOLS[S.C_fol]/MPOOLS[S.C_roo])/log(2),2);


/*equilibrium factor (in comparison to C_initial)*/
double EQF=DATA.ncdf_data.EDC_EQF;

/*Total fluxes*/
double *FT;
FT=calloc(nofluxes,sizeof(double));
int f=0;
for (f=0;f<nofluxes;f++){FT[f]=0;for (n=0;n<N_timesteps;n++){FT[f]+=FLUXES[n*nofluxes+f];}}
/*Total prec*/
double TOTAL_PREC=0;
for (n=0;n<N_timesteps;n++){TOTAL_PREC+=PREC[n];}


double Fin[8];
double Fout[8];
double Pstart;
double Pend;
/*temporary print switch*/
int psw=0;
/*exponential decay tolerance*/
double etol=0.1;





/*Inputs and outputs for each pool*/
/*labile*/
Fin[S.C_lab]=FT[F.lab_prod];
Fout[S.C_lab]=FT[F.lab_release]+FT[F.f_lab]+FT[F.fx_lab2lit];
/*foliar*/
Fin[S.C_fol]=FT[F.fol_prod]+FT[F.lab_release];
Fout[S.C_fol]=FT[F.fol2lit]+FT[F.f_fol]+FT[F.fx_fol2lit];
/*root*/
Fin[S.C_roo]=FT[F.root_prod];
Fout[S.C_roo]=FT[F.root2lit]+FT[F.f_roo]+FT[F.fx_roo2lit];
/*wood*/
Fin[S.C_woo]=FT[F.wood_prod];
Fout[S.C_woo]=FT[F.wood2lit]+FT[F.f_woo]+FT[F.fx_woo2som];
/*litter*/
Fin[S.C_lit]=FT[F.fol2lit]+FT[F.root2lit]+FT[F.fx_lab2lit]+FT[F.fx_fol2lit]+FT[F.fx_roo2lit];
Fout[S.C_lit]=FT[F.resp_het_lit]+FT[F.lit2som]+FT[F.f_lit]+FT[F.fx_lit2som];
/*som*/
Fin[S.C_som]=FT[F.wood2lit]+FT[F.lit2som]+FT[F.fx_woo2som]+FT[F.fx_lit2som];
Fout[S.C_som]=FT[F.resp_het_som]+FT[F.f_som];
/*PAH2O*/
Fin[S.H2O_PAW]=TOTAL_PREC;
Fout[S.H2O_PAW]=FT[F.et]+FT[F.q_paw]+FT[F.paw2puw];
/*PUH2O*/
Fin[S.H2O_PUW]=FT[F.paw2puw];
Fout[S.H2O_PUW]=FT[F.q_puw];


/*Inlcuding H2O pool*/
/*EDCs 7-13 - inputs, outputs and exponential tolerance*/

/*mean input/output ratio and start ratio*/
double Rm, Rs;



for (n=0;n<noprogpools;n++){
/*start and end pools*/
Pstart=POOLS[n];
Pend=POOLS[nopools*N_timesteps+n];
/*mean input/output*/
Rm=Fin[n]/Fout[n];
/*Theoretical starting input/output*/
Rs=Rm*MPOOLSjan[n]/Pstart;

if (Fin[n] > 0){
EDCD->pEDC=EDCD->pEDC-0.5*pow(log(Rs)/log(EQF),2) - 0.5 *pow((Rs-Rm)/etol,2);

/*storing EDCPROB: i.e. the log probability of each EDC based on a gaussian representation*/
/*of each constraint*/
EDCD->EDCPROB[7-1+n]=-0.5*pow(log(Rs)/log(EQF),2);/*-0.5*pow((Rs-Rm)/etol,2);*/
}

if (psw==1){
printf("****\n");
printf("DIAG = %i\n", DIAG);
printf("EDC = %i\n",EDC);
printf("EDCD->SWITCH[7-1+n] = %i\n",EDCD->SWITCH[7-1+n]);
printf("Pool %i EDCDPROB = %f\n",n,EDCD->EDCPROB[7-1+n]);
printf("Pool %i Fin = %f,Fout = %f\n",n,Fin[n],Fout[n]);
printf("Pool %i Pstart = %f,Pend = %f\n, Pmeanjan=%f\n",n,Pstart,Pend,MPOOLSjan[n]);
printf("fabs(log(Rs)) = %f\n",fabs(log(Rs)));
printf("fabs(log(Fin/Fout)) = %f\n",fabs(log(Fin[n]/Fout[n])));
printf("fabs(log(Pend/Pstart)) = %f\n",fabs(log(Pend/Pstart)));
printf("Rm = %f\n",Rm);
printf("Rs = %f\n",Rs);
printf("log(EQF) = %f\n",log(EQF));
printf("etol = %f\n",etol);
printf("****\n");}}


/*Ensuring that wilting point is at or below the mean H2O pool EDC14*/
EDCD->pEDC=EDCD->pEDC+log(1/(1+exp(10*(pars[P.wilting]-MPOOLS[S.H2O_PAW])/MPOOLS[S.H2O_PAW])));

/***********************EDCs done here****************************/



/*Additional faults can be stored in positions 35-40*/

/*PRIOR RANGES - ALL POOLS MUST CONFORM*/
int pidx[]={P.i_labile,P.i_foliar,P.i_root,P.i_wood,P.i_lit,P.i_som,P.i_PAW,P.i_PUW};

for (n=0;n<noprogpools;n++){if ((EDC==1 || DIAG==1) & ((MPOOLS[n])>parmax[pidx[n]])){EDC=0;EDCD->PASSFAIL[35-1]=0;EDCD->pEDC=log(0);}}


int PEDC;
/*ensuring minimum of each pool is zero & finite*/
if (EDC==1 || DIAG==1)
{double min; int nn;n=0;
while ((n<noprogpools) & (EDC==1 || DIAG==1))
{nn=0;PEDC=1;while ((nn<N_timesteps+1) & (PEDC==1))
{if ((POOLS[n+nn*nopools]<0) || isnan(POOLS[n+nn*nopools])==1)
{EDC=0;PEDC=0;EDCD->PASSFAIL[35+n]=0;EDCD->pEDC=log(0);}nn=nn+1;};	
n=n+1;
}
}
/*
if (EDC==1){printf("\n");oksofar("EDC(36-43) passed");}
*/








/*FREE MEMORY*/
free(FT);
free(MPOOLS);
free(MPOOLSjan);





/*final check confirming EDC = 1 or 0*/
int Num_EDC=100;
if (DIAG==1){for (n=0;n<Num_EDC;n++){if (EDCD->PASSFAIL[n]==0){EDC=0;}}}


/*Returning EDC */
return EDC;

}







