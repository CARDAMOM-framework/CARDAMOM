#pragma once
#include "CARDAMOM_MODULE_IDX.c"
typedef struct DATA{
/*DRIVERS*/
double *MET;
double meanprec;
double meantemp;
double meanrad;
/*OBS*/
double *GPP;
double *NBE;/*NBE uncertinaty*/
double *LAI;
double *ABGB;
double *ET;
double *EWT;
double *BAND4;
double *BAND3;
double *BAND2;
double *BAND1;
double *SOM;
double *NBEunc; /*NBE uncertainty*/
double *CH4; /*shuang*/
/*Indices of non-empty points in observation timeseries*/
int *gpppts;
int *nbepts;
int *laipts;
int *abgbpts;
int *etpts;
int *ewtpts;
int *band1pts;
int *band2pts;
int *band3pts;
int *band4pts;
int *sompts;
int *nbeuncpts;
int *ch4pts; /*shuang*/
/*Number of non-empty points in observation timeseries*/
/*Number of indices can be stored in single obs vector in the future*/
int ngpp;
int nnbe;
int nlai;
int nabgb;
int net;
int newt;
int nband1;
int nband2;
int nband3;
int nband4;
int nsom;
int nnbeunc;
int nch4;/*shuang*/
/*saving computational speed by allocating memory to model output*/
double *M_PARS; /*Prescribing these here since they need to be carried across functions with a "DATA"-only interface*/
/*Model fluxes*/
//MODEL FIELDS: these need to be consistent with cost function terms
double *M_GPP;
double *M_NBE;
double *M_LAI;
double *M_ET;
double *M_EWT;
double *M_CH4;
double *M_ABGB_t0;
double *M_ABGB;
double *M_SOM;
double *M_MGPP;
double *M_MFIRE;
//
double *M_FLUXES;
double *M_POOLS;
double *C_POOLS; /*Do we even use this?*/
/*even though sizes are known, memory needs to be explicitly allocated*/
int *M_EDCD;
double *M_P;
double *M_leo;
/*static data*/
int nodays;
double deltat;
double LAT;
int ID;
int noobs;
int nomet;
int nopools;
int nofluxes;
int nopars;
int EDC;
int EDCDIAG;
int edc_random_search;
int gppabs;
int gppiav;
int laiiav;
int etiav;
int ch4iav;/*shuang*/
double nbe_annual_unc;
double nbe_obs_unc;
double et_annual_unc;
double et_obs_unc;
double ewt_annual_unc;
double ewt_obs_unc;
double gpp_annual_unc;
double gpp_obs_unc;
double et_obs_threshold;
double gpp_obs_threshold;
double ch4_annual_unc;/*shuang*/
double ch4_obs_unc;/*shuang*/
double ch4_obs_threshold;/*shuang*/

/*priors*/
double parpriors[50];
double parpriorunc[50];
double otherpriors[50];
double otherpriorunc[50];
int PCrotate;
/*TO DO: include parameter info and model likelihood function fields HERE
These can then be assigned during call to CARDAMOM_MODEL_LIBRARY*/
double (*MLF)(struct DATA,double *);
/*parameter & optimization info - add all required fields here*/
double *parmin;
double *parmax;
char **parname;
int assemble_model;
/*Development in progress: this structure will contain all model-specific fields and memory declarations, making it therefore a flexible storage field for non-global "DATA" fields*/
void *MODEL;
/*MODULES: can be used to index user-defined module*/
struct MODULE_IDX MODULE_IDX;

}DATA;




