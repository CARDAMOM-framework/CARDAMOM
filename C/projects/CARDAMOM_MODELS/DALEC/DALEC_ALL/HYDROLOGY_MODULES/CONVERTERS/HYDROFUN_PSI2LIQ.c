#pragma once
#include <math.h>
/*
Convert effective water potential (MPa) to volumetric soil liquid (m3/m3) relative to pore space not occupied by ice
Inputs:
psi_eff - effective water potential (MPa) of the liquid-saturated state (i.e. corrected for ice volume)
psi_porosity - porosity potential (MPa)
b - retention parameter

***NOTE this is mathematically the same as previous PSI2MOI; rename PSI2LIQ intended to clarify its use case in DRAINAGE.c, 
the only place that it appears. 
*/

double HYDROFUN_PSI2LIQ(double psi, double psi_porosity, double b){

double moi = pow(psi/psi_porosity,(-1.0/b));
 
return moi;
}