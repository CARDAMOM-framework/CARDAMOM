#pragma once
double INTERNAL_ENERGY_PER_LIQUID_H2O_UNIT_MASS(double TEMP){
  //Internal energy per mm (or kg) H2O  
    //Temp is in kelvin
    //Assumes liquid fraction is = 1
    
    
    double U= DGCM_SPECIFIC_HEAT_WATER* (TEMP - DGCM_T_LIQUID_H2O_ZERO_ENERGY);
    
    
    return U;
}

