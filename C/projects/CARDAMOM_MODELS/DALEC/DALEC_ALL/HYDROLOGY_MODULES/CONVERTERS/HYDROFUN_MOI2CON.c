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
    double ice_sat_tv = moi * (1.0 - lf); //fraction of pore volume occupied by ice
    double effective_porosity = 1.0 - ice_sat_tv; //fraction of pore volume NOT occupied by ice

    // 2. Calculate effective liquid saturation
    double liq_sat_tv = moi * lf; //fraction of pore volume occupied by liquid water
    double moi_eff = liq_sat_tv / effective_porosity; //fraction of <pore volume NOT occupied by ice> occupied by liquid water

    // 3. Calculate unsaturated hydraulic conductivity based on available liquid space
    double con_max = k0 * pow(moi_eff, 2.0 * b + 3.0);
    double impedance = pow(10, -ice_sat_tv*6.0); // from Swenson et al., 2012 https://agupubs.onlinelibrary.wiley.com/doi/full/10.1029/2012MS000165 

    double con = con_max * impedance;
 
return con;
}