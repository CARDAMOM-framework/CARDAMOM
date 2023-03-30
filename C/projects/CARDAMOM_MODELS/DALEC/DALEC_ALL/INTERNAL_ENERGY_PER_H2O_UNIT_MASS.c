#pragma once
double INTERNAL_ENERGY_PER_H2O_UNIT_MASS(double TEMP, double LF){
  //Internal energy per mm (or kg) H2O  
    //Temp is in kelvin
    
    
    U=(1 - LF) * DGCM_SPECIFIC_HEAT_ICE* TEMP + LF * DGCM_SPECIFIC_HEAT_WATER* (TEMP - DGCM_T_LIQUID_H2O_ZERO_ENERGY);
    
    
    return U;
}

