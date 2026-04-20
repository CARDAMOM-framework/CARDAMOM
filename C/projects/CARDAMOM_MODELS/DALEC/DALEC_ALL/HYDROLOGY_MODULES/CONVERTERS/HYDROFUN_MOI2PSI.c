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

double HYDROFUN_MOI2PSI(double moi, double psi_porosity, double b, double lf, double soil_temp){

    // Safety check to prevent divide-by-zero 
    if (lf<=1e-4) return -9999.0;

    // 1. Calculate effective pore volume (fraction of por volume not occupied by ice)
    double ice_sat_tv= moi * (1.0 - lf); // Ice fraction of the total pore volume
    double effective_porosity = 1.0 - ice_sat_tv; // ice-adjusted pore volume as fraction of total pore volume

    // 2. Calculate effective liquid saturation
    double liq_sat_tv = moi * lf; // liquid water fraction of the total pore space volume
    double moi_eff = liq_sat_tv / effective_porosity; // liquid water fraction of the ice-adjusted pore space 

    // 3. Calculate effective psi based on capillary matric potential
    double psi_capillary=psi_porosity*(pow((1.0/(moi_eff)),b));

    // 4. Calculate cryosuction (Clapeyron equation) if frozen
    double psi_cryo = 0.0;
    if (lf < 1.0 && soil_temp < 273.15) {
        // Latent heat of fusion = 3.34e5 J/kg. Equation yields MPa.
        // As temperature drops further below 273.15, suction becomes more strongly negative
        psi_cryo = (3.34e5 * (soil_temp - 273.15)) / (1000.0 * soil_temp);
    }
    double psi = fmin(psi_capillary, psi_cryo);
 
return psi;
}