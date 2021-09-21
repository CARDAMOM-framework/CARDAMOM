#pragma once
#include "CARDAMOM_MODULE_IDX.c"
#include "../COST_FUNCTION/CARDAMOM_LIKELIHOOD_FUNCTION.c"
//This is a generic struct for a 1D var.
typedef struct ONE_DIM_VAR_STRUCT{
double* values; //NULL if not present
size_t length; //Set to 0 if not present
}ONE_DIM_VAR_STRUCT;

typedef struct TIMESERIES_DRIVER_STRUCT{
double* values; //NULL if not present
size_t length; //Set to 0 if not present
double reference_mean;}TIMESERIES_DRIVER_STRUCT;

//This is a generic struct that contains an additional uncertainty value, which is a common form for Cardamom variables
typedef struct ONE_DIM_VAR_WITH_UNCERTAINTY_STRUCT{
double* values; //NULL if not present
size_t length; //Set to 0 if not present
double Uncertainty;
}ONE_DIM_VAR_WITH_UNCERTAINTY_STRUCT;

//This is a generic struct for a 2D var
typedef struct TWO_DIM_VAR_STRUCT{
double** values;
size_t dimensions[2]; //NOTE: This is NOT a length per row, but a length per dimension. This means this array will ALLWAYS have 2 elements for a 2D array.
}TWO_DIM_VAR_STRUCT;




typedef struct NETCDF_DATA{
TIMESERIES_DRIVER_STRUCT BURNED_AREA;
TIMESERIES_DRIVER_STRUCT CO2;
TIMESERIES_DRIVER_STRUCT DOY;

TIMESERIES_OBS_STRUCT ABGB;
TIMESERIES_OBS_STRUCT CH4;
TIMESERIES_OBS_STRUCT ET;
TIMESERIES_OBS_STRUCT EWT;
TIMESERIES_OBS_STRUCT GPP;
TIMESERIES_OBS_STRUCT LAI;
TIMESERIES_OBS_STRUCT NBE;
TIMESERIES_OBS_STRUCT DOM;
//
SINGLE_OBS_STRUCT Mean_ABGB;
SINGLE_OBS_STRUCT Mean_FIR;
SINGLE_OBS_STRUCT Mean_GPP;
SINGLE_OBS_STRUCT Mean_LAI;
//Parameters and emergent quantities
SINGLE_OBS_STRUCT PEQ_Cefficiency;
SINGLE_OBS_STRUCT PEQ_CUE;


//Drivers
TIMESERIES_DRIVER_STRUCT  SSRD;
TIMESERIES_DRIVER_STRUCT  T2M_MAX;
TIMESERIES_DRIVER_STRUCT  T2M_MIN;
TIMESERIES_DRIVER_STRUCT  TIME_INDEX;
TIMESERIES_DRIVER_STRUCT TOTAL_PREC;
TIMESERIES_DRIVER_STRUCT VPD;
TIMESERIES_DRIVER_STRUCT SNOWFALL;
//Summary variables
int EDC;
int EDCDIAG;
double EDC_EQF;
int ID;
double LAT;
//Derived variables (not prescribed)
int Ntimesteps;
double deltat;
double meantemp;



}NETCDF_DATA;
