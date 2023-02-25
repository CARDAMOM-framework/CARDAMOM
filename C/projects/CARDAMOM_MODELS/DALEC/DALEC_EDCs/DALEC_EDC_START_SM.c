
#pragma once 
#include "../DALEC_ALL/HYDROLOGY_MODULES/CONVERTERS/HYDROFUN_EWT2MOI.c"



typedef struct {
    int z_idx;
    int por_idx;
    int i_H2O_idx;}DALEC_EDC_START_SM_STRUCT;

  
   


//General inequality function
double DALEC_EDC_START_SM(DATA * DATA, void * EDCstruct){



       DALEC_EDC_START_SM_STRUCT  E = *(DALEC_EDC_START_SM_STRUCT * ) EDCstruct;



double SM=HYDROFUN_EWT2MOI(DATA->M_PARS[E.i_H2O_idx],DATA->M_PARS[E.por_idx],DATA->M_PARS[E.z_idx]); 

    

   //loop through all timesteps

    double PEDC=0;
    
         if (SM>1){PEDC = -INFINITY;}
          



  
    return PEDC;
}

    
    
    