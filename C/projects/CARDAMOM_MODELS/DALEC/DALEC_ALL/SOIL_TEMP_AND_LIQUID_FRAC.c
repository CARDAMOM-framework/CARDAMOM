#pragma once
//***IN-OUT form structure***
typedef struct {    
struct{
    double     dry_soil_vol_heat_capacity;//J/m3/K
    double        depth;//m 
    double       soil_water;//mm (or kg/m2)
    double       internal_energy;} IN;//Joules
 struct {
     double TEMP;// Temperature in K
     double LF;} OUT;// Liquid fraction
}SOIL_TEMP_AND_LIQUID_FRAC_STRUCT;

//***Main function***
int SOIL_TEMP_AND_LIQUID_FRAC(SOIL_TEMP_AND_LIQUID_FRAC_STRUCT * S){
    
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
return 0;
}
    
    
    
    
