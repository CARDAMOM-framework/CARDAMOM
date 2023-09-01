#pragma once
typedef struct {    
    struct{
 double       h2o;//mm (or kg/m2)
 double       internal_energy;} IN;//Joules
 struct {
 double TEMP;// Temperature in K
 double LF;} OUT;// Liquid fraction
}H2O_TEMP_AND_LIQUID_FRAC_STRUCT;

    

//Main function 
int H2O_TEMP_AND_LIQUID_FRAC(H2O_TEMP_AND_LIQUID_FRAC_STRUCT * S){



//Internal energy at triple pointUi3
//Volumetric heat capacity of soils per m3 [J/m3/K]


//Ui3 = (Cs z + Ws Ci) T3
// 
// lil3 Specific latent heat of fusion of water at the triple point 3.34 · 105 J kg?1
// llv3 Specific latent heat of vaporization of water at the triple point 2.5 · 10?6 J kg?1
// liv3 Specific latent heat of sublimation of ice at the triple point li`3 + l`v3 = 2.834 · 10?6 J kg?1

double UI3 = (  S->IN.h2o *DGCM_SPECIFIC_HEAT_ICE)*DGCM_T3 ; 
double UL3 = UI3 + S->IN.h2o*DGCM_LATENT_HEAT_FUSION_3; 


if (S->IN.internal_energy<UI3){
    S->OUT.TEMP = (S->IN.internal_energy/UI3)*DGCM_T3 ;
   S->OUT.LF = 0; }
        
else if (S->IN.internal_energy>UL3){
        S->OUT.TEMP = (S->IN.internal_energy + S->IN.h2o*DGCM_SPECIFIC_HEAT_WATER*DGCM_T_LIQUID_H2O_ZERO_ENERGY)/(S->IN.h2o*DGCM_SPECIFIC_HEAT_WATER) ;
      S->OUT.LF = 1;  
}
//Calculate liquid fraction
else {
    S->OUT.TEMP = DGCM_TK0C;  
     //S->OUT.LF =(S->IN.internal_energy - UI3) /(S->IN.h2o*DGCM_LATENT_HEAT_FUSION_3);  

    S->OUT.LF =  (S->IN.internal_energy/S->IN.h2o - DGCM_SPECIFIC_HEAT_ICE* DGCM_TK0C )/(DGCM_SPECIFIC_HEAT_WATER* (DGCM_TK0C - DGCM_T_LIQUID_H2O_ZERO_ENERGY) - DGCM_SPECIFIC_HEAT_ICE* DGCM_TK0C);
}
    
 
    
return 0;

}
    
    






//    S->OUT.TEMP = DGCM_TK0C;  
   //  S->OUT.LF =(S->IN.internal_energy - UI3) /(S->IN.h2o*DGCM_LATENT_HEAT_FUSION_3);  
//


//U=(1 - LF) * DGCM_SPECIFIC_HEAT_ICE* TEMP + LF * DGCM_SPECIFIC_HEAT_WATER* (TEMP - DGCM_T_LIQUID_H2O_ZERO_ENERGY);
//U= DGCM_SPECIFIC_HEAT_ICE* TEMP  -  LF*DGCM_SPECIFIC_HEAT_ICE* TEMP + LF * DGCM_SPECIFIC_HEAT_WATER* (TEMP - DGCM_T_LIQUID_H2O_ZERO_ENERGY);
//U -  DGCM_SPECIFIC_HEAT_ICE* TEMP = -  LF*DGCM_SPECIFIC_HEAT_ICE* TEMP + LF * DGCM_SPECIFIC_HEAT_WATER* (TEMP - DGCM_T_LIQUID_H2O_ZERO_ENERGY);
// DGCM_SPECIFIC_HEAT_ICE* TEMP - U = LF*(DGCM_SPECIFIC_HEAT_ICE* TEMP - DGCM_SPECIFIC_HEAT_WATER* (TEMP - DGCM_T_LIQUID_H2O_ZERO_ENERGY));
//  LF = (U - DGCM_SPECIFIC_HEAT_ICE* TEMP )/(DGCM_SPECIFIC_HEAT_WATER* (TEMP - DGCM_T_LIQUID_H2O_ZERO_ENERGY) - DGCM_SPECIFIC_HEAT_ICE* TEMP)
    
    