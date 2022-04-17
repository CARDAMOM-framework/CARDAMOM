typedef struct {    
double     dry_soil_vol_heat_capacity;//J/m3/K
double        depth;//m 
 double       soil_water;//mm (or kg/m2)
 double       internal_energy;//Joules
}SOIL_TEMP_AND_LIQUID_FRAC_STRUCT;

    

//Main function 
int SOIL_TEMP_AND_LIQUID_FRAC(SOIL_TEMP_AND_LIQUID_FRAC_STRUCT * S, double * SOIL_TEMP, double * SOIL_LF){



//Internal energy at triple pointUi3
//Volumetric heat capacity of soils per m3 [J/m3/K]


//Ui3 = (Cs z + Ws Ci) T3
// 
// lil3 Specific latent heat of fusion of water at the triple point 3.34 · 105 J kg?1
// llv3 Specific latent heat of vaporization of water at the triple point 2.5 · 10?6 J kg?1
// liv3 Specific latent heat of sublimation of ice at the triple point li`3 + l`v3 = 2.834 · 10?6 J kg?1

double dry_soil_sh=S->dry_soil_vol_heat_capacity * S->depth;
double UI3 = ( dry_soil_sh + S->soil_water *DGCM_SPECIFIC_HEAT_ICE)*DGCM_T3 ; 
double UL3 = UI3 + S->soil_water*DGCM_LATENT_HEAT_FUSION_3; 

//Default outcome if UI3<U<UL3
*SOIL_TEMP = 0;  

if (S->internal_energy<UI3){
    *SOIL_TEMP = (S->internal_energy/UI3)*DGCM_T3 - DGCM_TK0C;
   *SOIL_LF = 0; }
        
else if (S->internal_energy>UL3){
        *SOIL_TEMP = (S->internal_energy + S->soil_water*DGCM_SPECIFIC_HEAT_WATER*DGCM_TK0C)/(dry_soil_sh + S->soil_water*DGCM_SPECIFIC_HEAT_WATER)  - DGCM_TK0C;
      *SOIL_LF = 1;  
}
//Calculate liquid fraction
else {
      *SOIL_LF =(S->internal_energy - UI3) /(S->soil_water*DGCM_LATENT_HEAT_FUSION_3);  
}
    
 
    
return 0;

}
    
    
    
    