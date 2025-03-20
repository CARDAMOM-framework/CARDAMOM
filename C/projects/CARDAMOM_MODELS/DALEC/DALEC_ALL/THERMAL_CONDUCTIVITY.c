#pragma once


//***Main function***
double THERMAL_CONDUCTIVITY(double       porosity,double       soil_water, double       soil_liquid_fraction, double       soil_temp, double soil_thermal_conductivity){

    double THERMAL_COND;
double dry_soil_sh=S->IN.dry_soil_vol_heat_capacity * S->IN.depth;
double UI3 = ( dry_soil_sh + S->IN.soil_water *DGCM_SPECIFIC_HEAT_ICE)*DGCM_T3 ; 
double UL3 = UI3 + S->IN.soil_water*DGCM_LATENT_HEAT_FUSION_3; 

if (S->IN.internal_energy<UI3){
    S->OUT.TEMP = (S->IN.internal_energy/UI3)*DGCM_T3 ;S->OUT.LF = 0;}
else if (S->IN.internal_energy>UL3){
        S->OUT.TEMP = (S->IN.internal_energy + S->IN.soil_water*DGCM_SPECIFIC_HEAT_WATER*DGCM_T_LIQUID_H2O_ZERO_ENERGY)/(dry_soil_sh + S->IN.soil_water*DGCM_SPECIFIC_HEAT_WATER) ;
      S->OUT.LF = 1;}
else {
    S->OUT.TEMP = DGCM_TK0C;  
     S->OUT.LF =(S->IN.internal_energy - UI3) /(S->IN.soil_water*DGCM_LATENT_HEAT_FUSION_3);  
}
return THERMAL_COND;
}
    
