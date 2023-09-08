#pragma once
#include "ACM.c"
#include "offset.c"
#include "../DALEC_OBSERVATION_OPERATORS/DALEC_OBSERVATION_OPERATORS.c"
#include "DALEC_EDCS_OLD.c"
typedef struct {
double (*function) (DATA * , void *);
     void *    data;
     bool prerun;
}EDCs;



typedef struct DALEC_FLUX_SOURCE_SINK_MATRIX{
    int *SOURCE;
    int *SINK;
}DALEC_FLUX_SOURCE_SINK_MATRIX;


   typedef struct DALEC_STATE_SOURCE_SINK_MATRIX{ 
  int N_STATE_INPUT_FLUXES;
  int *STATE_INPUT_FLUXES;
  int N_STATE_OUTPUT_FLUXES;
  int *STATE_OUTPUT_FLUXES;}DALEC_STATE_SOURCE_SINK_MATRIX;
  

typedef struct DALEC{
int nopools;
int nopars;
int nofluxes;
int nomet;
int noedcs;
int * testvec;//Flag for deletion (isn't supposed to do anything)
struct PARS_INFO {
   double * parmin;
   double * parmax;} PARS_INFO;
//struct POOL_INFO;
struct PARS_META_STRUCT {
   char ** NAME;
   char ** ABBREVIATION;
   char ** UNITS;
   char ** DESCRIPTION;} PARS_META;
   
struct FLUX_META_STRUCT {
   char ** NAME;
   char ** ABBREVIATION;
   char ** UNITS;
   char ** DESCRIPTION;} FLUX_META;

int (*dalec)(DATA,const double *);
//To become obsolete, once EDCs refactored
int (*edc1)(const double *, DATA, struct EDCDIAGNOSTIC * EDCD);
int (*edc2)(const double *, DATA, struct EDCDIAGNOSTIC * EDCD);
/*contains all the EDCD relevant info*/
struct EDCDIAGNOSTIC * EDCD;

DALEC_FLUX_SOURCE_SINK_MATRIX FIOMATRIX;
DALEC_STATE_SOURCE_SINK_MATRIX * SIOMATRIX;
OBSOPE OBSOPE;
EDCs * EDCs;//Pointer, as each element is a unique EDC
    void * MODEL_DATA;
} DALEC;

#include "../DALEC_EDCs/DALEC_EDC_FUNCTIONS.c"
#include "DALEC_STATE_SOURCE_SINK_MATRIX_CONFIG.c"



