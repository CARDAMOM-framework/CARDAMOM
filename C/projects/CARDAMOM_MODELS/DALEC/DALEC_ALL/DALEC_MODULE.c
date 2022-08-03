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
//struct FLUX_INFO;
int (*dalec)(DATA,const double *);
//To become obsolete, once EDCs refactored
int (*edc1)(const double *, DATA, struct EDCDIAGNOSTIC * EDCD);
int (*edc2)(const double *, DATA, struct EDCDIAGNOSTIC * EDCD);
/*contains all the EDCD relevant info*/
struct EDCDIAGNOSTIC * EDCD;

OBSOPE OBSOPE;
EDCs * EDCs;//Pointer, as each element is a unique EDC
}DALEC;

#include "../DALEC_EDCs/DALEC_EDC_FUNCTIONS.c"

