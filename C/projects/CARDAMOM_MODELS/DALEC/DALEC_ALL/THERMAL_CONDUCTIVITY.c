#pragma once
//***Main function***
typedef struct {    
    struct {double surf_por;
            double mid_por;
            double deep_por;
            double soil_VWC_surf; 
            double soil_VWC_mid; 
            double soil_VWC_deep; 
            double LF_surf; 
            double LF_mid; 
            double LF_deep; 
            double soil_thermal_conductivity_surf;
            double soil_thermal_conductivity_middeep;} IN;
    struct {double THERMAL_COND_SURF;
            double THERMAL_COND_MID;
            double THERMAL_COND_DEEP;} OUT;
            }Thermal_struct;

int THERMAL_COND(Thermal_struct * S){

const double air_cond=0.026;//Conductivity of ar at 0C
const double water_cond=0.57;//Conductivity of water at 0C
const double ice_cond=2.22;//Conductivity of water at 0C
    
S->OUT.THERMAL_COND_SURF =0;
S->OUT.THERMAL_COND_MID =0;
S->OUT.THERMAL_COND_DEEP =0;

S->OUT.THERMAL_COND_SURF=((S->IN.surf_por-S->IN.soil_VWC_surf*S->IN.surf_por)*air_cond + // Air contribution
                 S->IN.surf_por*S->IN.soil_VWC_surf*S->IN.LF_surf*water_cond + // water contribution
                 S->IN.surf_por*S->IN.soil_VWC_surf*(1-S->IN.LF_surf)*ice_cond + // ice contribution
                 (1-S->IN.surf_por)*S->IN.soil_thermal_conductivity_surf); // soil contribution

S->OUT.THERMAL_COND_MID=((S->IN.mid_por-S->IN.soil_VWC_mid*S->IN.mid_por)*air_cond + // Air contribution
                 S->IN.mid_por*S->IN.soil_VWC_mid*S->IN.LF_mid*water_cond + // water contribution
                 S->IN.mid_por*S->IN.soil_VWC_mid*(1-S->IN.LF_mid)*ice_cond + // ice contribution
                 (1-S->IN.mid_por)*S->IN.soil_thermal_conductivity_middeep); // soil contribution

S->OUT.THERMAL_COND_DEEP=((S->IN.deep_por-S->IN.soil_VWC_deep*S->IN.deep_por)*air_cond + // Air contribution
                 S->IN.deep_por*S->IN.soil_VWC_deep*S->IN.LF_deep*water_cond + // water contribution
                 S->IN.deep_por*S->IN.soil_VWC_deep*(1-S->IN.LF_deep)*ice_cond + // ice contribution
                 (1-S->IN.deep_por)*S->IN.soil_thermal_conductivity_middeep); // soil contribution

   return 0;
}
    
