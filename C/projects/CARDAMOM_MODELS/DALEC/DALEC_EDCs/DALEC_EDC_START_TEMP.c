
#pragma once 
#include "../DALEC_ALL/SOIL_TEMP_AND_LIQUID_FRAC.c"


typedef struct {
    double min_temp;
    double max_temp;
   int vhc_idx;
    int z_idx;
    int i_H2O_idx;
    int i_E_idx;}DALEC_EDC_START_TEMP_STRUCT;

  
   


//General inequality function
double DALEC_EDC_START_TEMP(DATA * DATA, void * EDCstruct){



       DALEC_EDC_START_TEMP_STRUCT  E = *(DALEC_EDC_START_TEMP_STRUCT * ) EDCstruct;

    //Declare stryct
    SOIL_TEMP_AND_LIQUID_FRAC_STRUCT SOILTEMP;
  //Populate with run-specific constrants
    //PAW
    SOILTEMP.IN.dry_soil_vol_heat_capacity=DATA->M_PARS[E.vhc_idx] ;//J/m3/K
    SOILTEMP.IN.depth = DATA->M_PARS[E.z_idx];//m 
    SOILTEMP.IN.soil_water = DATA->M_PARS[E.i_H2O_idx];//mm (or kg/m2)
    SOILTEMP.IN.internal_energy =DATA->M_PARS[E.i_E_idx];//Joules
    //Pass pointer to function 
    SOIL_TEMP_AND_LIQUID_FRAC(&SOILTEMP);  //Outputs are in K

    

   //loop through all timesteps

    double PEDC=0;
    
         if (SOILTEMP.OUT.TEMP<E.min_temp | SOILTEMP.OUT.TEMP>E.max_temp){PEDC = -INFINITY;}
          



  
    return PEDC;
}

    
    
    