#pragma once
#include <math.h>

/*
Convert volumetric soil moisture (m3/m3) to potential (MPa)
Inputs:
moi - Soil moisture (m3/m3)
psi_porosity - porosity potential (MPa)
b - retention parameter
lf - liquid fraction
*/

double HYDROFUN_MOI2PSI(double moi, double psi_porosity, double b, double lf){

    // Safety check to prevent divide-by-zero 
    if (lf<=1e-4) return -9999.0;

    // 1. Calculate effective pore volume (fraction of por volume not occupied by ice)
    double ice_sat_tv= moi * (1.0 - lf); // Ice fraction of the total pore volume
    double effective_porosity = 1.0 - ice_sat_tv; // ice-adjusted pore volume as fraction of total pore volume

    // 2. Calculate effective liquid saturation
    double liq_sat_tv = moi * lf; // liquid water fraction of the total pore space volume
    double moi_eff = liq_sat_tv / effective_porosity; // liquid water fraction of the ice-adjusted pore space 

// 3. Calculate effective psi
double psi=psi_porosity*(pow((1.0/(moi_eff)),b));
 
return psi;
}