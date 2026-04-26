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

if (lf<=1e-6 && moi>=0.999999){
    return 0.0;
}
    double min_cond_factor = 1e-14; // scales saturated conductivity
    // 1. Calculate effective pore volume (fraction of pore volume not occupied by ice)
    double ice_sat_tv = moi * (1.0 - lf); //fraction of pore volume occupied by ice
    double effective_porosity = 1.0 - ice_sat_tv; //fraction of pore volume NOT occupied by ice

    // 2. Calculate effective liquid saturation
    double liq_sat_tv = moi * lf; //fraction of pore volume occupied by liquid water    
    double moi_eff = liq_sat_tv / effective_porosity; //fraction of <pore volume NOT occupied by ice> occupied by liquid water

    // 3. Calculate unsaturated hydraulic conductivity based on available liquid space. 
    double con_campbell = k0 * pow(moi_eff, 2.0 * b + 3.0); /*CAMPBELL, GAYLON S. A SIMPLE METHOD FOR DETERMINING UNSATURATED CONDUCTIVITY FROM MOISTURE RETENTION DATA. Soil Science 117(6):p 311-314, June 1974.*/
    double con_min=k0*min_cond_factor; //allows nominal water movement for fully dry/frozen empty pore space
    
    double con=fmax(con_campbell, con_min);
 
return con;
}