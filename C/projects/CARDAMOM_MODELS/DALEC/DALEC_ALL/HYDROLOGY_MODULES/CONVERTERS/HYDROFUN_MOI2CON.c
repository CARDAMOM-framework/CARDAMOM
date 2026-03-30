#pragma once

/*
Convert volumetric soil moisture (m3/m3) to conductivity (m/s)
Inputs:
moi - Soil moisture (m3/m3)
k0 - saturated hydraulic conductivity (m/s)
b - retention parameter
lf - liquid fraction 
*/

double HYDROFUN_MOI2CON(double moi, double k0, double b, double lf){

if (lf<=1e-4){
    return 0.0;
}

    // 1. Calculate effective pore volume (fraction of pore volume not occupied by ice)
    double ice_sat_tv = moi * (1.0 - lf);
    double effective_porosity = 1.0 - ice_sat_tv; 

    // 2. Calculate effective liquid saturation
    double liq_sat_tv = moi * lf; 
    double moi_eff = liq_sat_tv / effective_porosity; 

    // 3. Calculate unsaturated hydraulic conductivity based on available liquid space
    double con = k0 * pow(moi_eff, 2.0 * b + 3.0);
 
return con;
}