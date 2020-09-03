#pragma once
#include "CARDAMOM_MODULE_IDX.c"
typedef struct ONE_DIM_VAR{
double* values; //NULL if not present
size_t length; //Set to 0 if not present
}ONE_DIM_VAR;


typedef struct GPP_STRUCT{
double* values; //NULL if not present
size_t length; //Set to 0 if not present
double Annual_Uncertainty;

}GPP_STRUCT;


typedef struct TWO_DIM_VAR{
double** values;
size_t* dimensions; //NOTE: This is NOT a length per row, but a length per dimension. This means this array will ALLWAYS have 2 elements for a 2D array.
}TWO_DIM_VAR;

typedef struct NETCDF_DATA{
double ABGB;
double CH4;
double EDC;
double EDCDIAG;
double ET;
double EWT;
ONE_DIM_VAR GPP;
double ID;
ONE_DIM_VAR LAI;
double LAT;
double mean_biomass;
double mean_fire;
double mean_GPP;
double mean_LAI;
TWO_DIM_VAR MET;
double NBE;
ONE_DIM_VAR PARPRIORS;
ONE_DIM_VAR PARPRIORUNC;
double SOM;

}NETCDF_DATA;
