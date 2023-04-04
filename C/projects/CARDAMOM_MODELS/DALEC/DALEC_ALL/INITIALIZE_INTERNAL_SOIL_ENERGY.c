#pragma once
#include  "SOIL_TEMP_AND_LIQUID_FRAC.c"

   
//Main function 
double INITIALIZE_INTERNAL_SOIL_ENERGY(double internal_energy_per_mm_H2O, double H2O_mm, double dry_soil_vol_heat_capacity, double depth ){

// 
// double       internal_energy_per_mm_h2o; 
// double       soil_water;//mm (or kg/m2)
// double     dry_soil_vol_heat_capacity;//J/m3/K
// double        depth;//m 
// 


 SOIL_TEMP_AND_LIQUID_FRAC_STRUCT LY_H2O_TEMP;
    //1mm, no soil
    LY_H2O_TEMP.IN.dry_soil_vol_heat_capacity =0 ;//J/m3/K
    LY_H2O_TEMP.IN.depth = 0;//m 
    LY_H2O_TEMP.IN.soil_water = 1;//mm (or kg/m2)
    LY_H2O_TEMP.IN.internal_energy = internal_energy_per_mm_H2O;//Joules
        //Calculate H2O temp
    SOIL_TEMP_AND_LIQUID_FRAC(&LY_H2O_TEMP);
    //Now calculate internal energy 
    //Energy in dry soil = 
    double SOIL_E = dry_soil_vol_heat_capacity* depth*LY_H2O_TEMP.OUT.TEMP;
    //Energy in H2O = mm* E/mm;
    double H2O_E =  internal_energy_per_mm_H2O*H2O_mm;
    //Total energy
  double TOTAL_E= SOIL_E  + H2O_E;
//Calculate temp based on total energy (just to check)

// 
//     double TEMP=LY_H2O_TEMP.OUT.TEMP;
//    LY_H2O_TEMP.IN.dry_soil_vol_heat_capacity =dry_soil_vol_heat_capacity ;//J/m3/K
//     LY_H2O_TEMP.IN.depth = depth;//m 
//     LY_H2O_TEMP.IN.soil_water = H2O_mm;//mm (or kg/m2)
//     LY_H2O_TEMP.IN.internal_energy = TOTAL_E;//Joules
//      SOIL_TEMP_AND_LIQUID_FRAC(&LY_H2O_TEMP);



    
return TOTAL_E;

}
    
    