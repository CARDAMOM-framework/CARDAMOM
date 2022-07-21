#pragma once
#include "CARDAMOM_MODULE_IDX.c"
#include "CARDAMOM_NETCDF_DATA_STRUCTURE.c"

typedef struct DATA{
//This is the netCDF data struct. See CARDAMOM_NETCDF_DATA_STRUCTURE.c for details on all the things contained therein
NETCDF_DATA ncdf_data;

/****Diagnostics****/
int edcsearch;


/*OBS*/
/*Number of non-empty points in observation timeseries*/
/*Number of indices can be stored in single obs vector in the future*/
double *M_PARS;

/*saving computational speed by allocating memory to model output*/
/*Model fluxes*/
//MODEL FIELDS: these need to be consistent with cost function terms
double *M_ABGB;
double *M_CH4;
double *M_CWOO;
double *M_DOM;
double *M_ET;
double *M_EWT;
double *M_GPP;
double *M_SIF;
double *M_LAI;
double *M_NBE;
double *M_ROFF;
double *M_SCF;
//Mean values
double M_Mean_ABGB;
double M_Mean_FIR;
double M_Mean_GPP;
double M_Mean_LAI;
//Miscelaneous
double M_ABGB_t0;
//Parameters and emergent quantities
double M_PEQ_Cefficiency;
double M_PEQ_CUE;
double M_PEQ_Vcmax25;
double M_PEQ_C3frac;
double M_PEQ_iniSnow;
double M_PEQ_iniSOM;



//Fyll state variables
double *M_FLUXES;
double *M_POOLS;
/*even though sizes are known, memory needs to be explicitly allocated*/
int *M_EDCD;
double *M_P;
double *M_LIKELIHOODS;
double *M_EDCs;
/*static data*/
// int nodays;
// double deltat;
// double LAT;
// int ID;
// int noobs;
//These quantities are autofilled based on model ID
int nopools;
int nofluxes;
int nopars;
int nolikelihoods;
int noedcs;
//



/*TO DO: include parameter info and model likelihood function fields HERE
These can then be assigned during call to CARDAMOM_MODEL_LIBRARY*/
double (*MLF)(struct DATA,double *);
double (*EMLF)(struct DATA,double *);
/*parameter & optimization info - add all required fields here*/
double *parpriors;
double *parmin;
double *parmax;
char **parname;
// int assemble_model;
/*Development in progress: this structure will contain all model-specific fields and memory declarations, making it therefore a flexible storage field for non-global "DATA" fields*/
void *MODEL;

//EDCs struct
void *EDCs;



}DATA;




