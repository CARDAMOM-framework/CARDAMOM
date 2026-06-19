#pragma once
#include "../../../DALEC_CODE/DALEC_ALL/mean_pool.c"
#include "../../../DALEC_CODE/DALEC_ALL/mean_annual_pool.c"
#include "../../../DALEC_CODE/DALEC_ALL/expdecay2.c"
#include "../../../../math_fun/std.c"
#include "../../../../math_fun/ipow.c"
#include "stdlib.h"
#include "stdio.h"


int EDC2_1111(double const *pars, DATA DATA, struct EDCDIAGNOSTIC *EDCD)
{

struct DALEC_1111_PARAMETERS P=DALEC_1111_PARAMETERS;
struct DALEC_1111_FLUXES F=DALEC_1111_FLUXES;
struct DALEC_1111_POOLS S=DALEC_1111_POOLS;

DALEC *MODEL=(DALEC *)DATA.MODEL;

double *PREC=DATA.ncdf_data.TOTAL_PREC.values;
double *SNOWFALL=DATA.ncdf_data.SNOWFALL.values;
double *TIME_INDEX=DATA.ncdf_data.TIME_INDEX.values;
double *POOLS=DATA.M_POOLS;
double *FLUXES=DATA.M_FLUXES;
int N_timesteps=DATA.ncdf_data.TIME_INDEX.length;
double *parmax=DATA.parmax;

double meantemp = (DATA.ncdf_data.T2M_MAX.reference_mean + DATA.ncdf_data.T2M_MIN.reference_mean)/2;

int EDC=1,n=0,m=0,edc=0;
int DIAG=EDCD->DIAG;

int nopools=MODEL->nopools;
int nofluxes=MODEL->nofluxes;
int done=0;
int k=0;

/* Mean pools over all timesteps */
double *MPOOLS;
MPOOLS=calloc(nopools,sizeof(double));
if (MPOOLS==0){printf("WARNING NULL POINTER");}
for (n=0;n<nopools;n++){MPOOLS[n]=mean_pool(POOLS,n,N_timesteps+1,nopools);};

/* Mean January pools */
double *MPOOLSjan;
int dint=(int)floor(N_timesteps/(TIME_INDEX[N_timesteps-1]-TIME_INDEX[0])*365.25);
MPOOLSjan=calloc(nopools,sizeof(double));
if (MPOOLSjan==0){printf("WARNING NULL POINTER");}
for (n=0;n<nopools;n++){
for (m=0;m<(N_timesteps/dint+1);m++){
MPOOLSjan[n]=MPOOLSjan[n]+POOLS[nopools*(m*dint)+n]/(N_timesteps/dint+1);}}

/******* EDCs start here *******/

/* EDC 6: C_fol : C_roo ratio */
EDCD->pEDC=EDCD->pEDC-0.5*pow(log(MPOOLS[S.C_fol]/MPOOLS[S.C_roo])/log(2),2);

double EQF=EDCD->EQF;

/* Total fluxes */
double *FT;
FT=calloc(nofluxes,sizeof(double));
int f=0;
for (f=0;f<nofluxes;f++){FT[f]=0;for (n=0;n<N_timesteps;n++){FT[f]+=FLUXES[n*nofluxes+f];}}

double TOTAL_PREC=0;
double TOTAL_SNOW=0;
for (n=0;n<N_timesteps;n++){TOTAL_PREC+=PREC[n];TOTAL_SNOW+=SNOWFALL[n];}

/* Pool flux balance: Fin and Fout (12 pools for DALEC_1111) */
double Fin[12];
double Fout[12];
double Pstart;
double Pend;
int psw=0;
double etol=0.1;

/* Labile */
Fin[S.C_lab]   = FT[F.lab_prod];
Fout[S.C_lab]  = FT[F.lab_release]+FT[F.f_lab]+FT[F.fx_lab2lit];
/* Foliar */
Fin[S.C_fol]   = FT[F.fol_prod]+FT[F.lab_release];
Fout[S.C_fol]  = FT[F.fol2lit]+FT[F.f_fol]+FT[F.fx_fol2lit];
/* Root */
Fin[S.C_roo]   = FT[F.root_prod];
Fout[S.C_roo]  = FT[F.root2lit]+FT[F.f_roo]+FT[F.fx_roo2lit];
/* Wood
 * Note: non-combusted fire wood now splits into CWD and PyC.
 * Total woody fire structural loss = f_woo + fx_woo2cwd + fx_woo2PyC.
 */
Fin[S.C_woo]   = FT[F.wood_prod];
Fout[S.C_woo]  = FT[F.wood2cwd]+FT[F.f_woo]+FT[F.fx_woo2cwd]+FT[F.fx_woo2PyC];
/* CWD: now receives only (1-alpha_eff) fraction of non-combusted wood */
Fin[S.C_cwd]   = FT[F.wood2cwd]+FT[F.fx_woo2cwd];
Fout[S.C_cwd]  = FT[F.resp_het_cwd]+FT[F.cwd2som]+FT[F.f_cwd]+FT[F.fx_cwd2som];
/* Litter */
Fin[S.C_lit]   = FT[F.fol2lit]+FT[F.root2lit]+FT[F.fx_lab2lit]+FT[F.fx_fol2lit]+FT[F.fx_roo2lit];
Fout[S.C_lit]  = FT[F.resp_het_lit]+FT[F.lit2som]+FT[F.f_lit]+FT[F.fx_lit2som];
/* SOM */
Fin[S.C_som]   = FT[F.cwd2som]+FT[F.lit2som]+FT[F.fx_cwd2som]+FT[F.fx_lit2som];
Fout[S.C_som]  = FT[F.resp_het_som]+FT[F.f_som];
/* PAW */
Fin[S.H2O_PAW] = TOTAL_PREC-FT[F.q_surf]-TOTAL_SNOW+FT[F.melt];
Fout[S.H2O_PAW]= FT[F.et]+FT[F.q_paw]+FT[F.paw2puw];
/* PUW */
Fin[S.H2O_PUW] = FT[F.paw2puw];
Fout[S.H2O_PUW]= FT[F.q_puw];
/* SWE */
Fin[S.H2O_SWE] = TOTAL_SNOW;
Fout[S.H2O_SWE]= FT[F.melt];
/* Labile PyC: input = non-combusted wood * alpha_eff * (1-beta_stab) */
Fin[S.C_PyC_L] = FT[F.fx_woo2PyC]*(1.0-pars[P.beta_stab]);
Fout[S.C_PyC_L]= FT[F.f_PyC_L_dec];
/* Refractory PyC: input = non-combusted wood * alpha_eff * beta_stab */
Fin[S.C_PyC_R] = FT[F.fx_woo2PyC]*pars[P.beta_stab];
Fout[S.C_PyC_R]= FT[F.f_PyC_R_dec];

/* EDCs 7-18: pool trajectory checks (nopools=12 now) */
double Rm, Rs;
for (n=0;n<nopools;n++){
Pstart=POOLS[n];
Pend=POOLS[nopools*N_timesteps+n];
Rm=Fin[n]/Fout[n];
Rs=Rm*MPOOLSjan[n]/Pstart;
if (Fin[n] > 0){
EDCD->pEDC=EDCD->pEDC-0.5*pow(log(Rs)/log(EQF),2)-0.5*pow((Rs-Rm)/etol,2);
EDCD->EDCPROB[7-1+n]=-0.5*pow(log(Rs)/log(EQF),2);
}
if (psw==1){
printf("Pool %i Fin = %f,Fout = %f\n",n+1,Fin[n],Fout[n]);
printf("Pool %i Pstart = %f,Pend = %f\n Pmeanjan=%f\n",n+1,Pstart,Pend,MPOOLSjan[n]);
printf("Rm = %f\n",Rm);printf("Rs = %f\n",Rs);}}

/* EDC 14/15: wilting point below mean H2O pool */
EDCD->pEDC=EDCD->pEDC+log(1/(1+exp(10*(pars[P.wilting]-MPOOLS[S.H2O_PAW])/MPOOLS[S.H2O_PAW])));

/* EDC PyC-2: refractory PyC pool not larger than SOM */
if (((EDC==1 & DIAG==0) || DIAG==1) & (MPOOLS[S.C_PyC_R]>MPOOLS[S.C_som])){
    EDCD->pEDC=EDCD->pEDC+log(0.01);}

/* Prior range check for all initial pool values */
int pidx[]={P.i_labile,P.i_foliar,P.i_root,P.i_wood,P.i_cwd,P.i_lit,P.i_soil,P.i_PAW,P.i_PUW,P.i_SWE,P.i_PyC_L,P.i_PyC_R};
for (n=0;n<nopools;n++){if ((EDC==1 || DIAG==1) & ((MPOOLS[n])>parmax[pidx[n]])){EDC=0;EDCD->PASSFAIL[35-1]=0;EDCD->pEDC=log(0);}}

/* Non-negativity and finiteness check for all pools */
int PEDC;
if (EDC==1 || DIAG==1)
{double min; int nn;n=0;
while ((n<nopools) & (EDC==1 || DIAG==1))
{nn=0;PEDC=1;while ((nn<N_timesteps+1) & (PEDC==1))
{if ((POOLS[n+nn*nopools]<0) || isnan(POOLS[n+nn*nopools])==1)
{EDC=0;PEDC=0;EDCD->PASSFAIL[35+n]=0;EDCD->pEDC=log(0);}nn=nn+1;};
n=n+1;
}
}

free(FT);
free(MPOOLS);
free(MPOOLSjan);

int Num_EDC=100;
if (DIAG==1){for (n=0;n<Num_EDC;n++){if (EDCD->PASSFAIL[n]==0){EDC=0;}}}

return EDC;
}
