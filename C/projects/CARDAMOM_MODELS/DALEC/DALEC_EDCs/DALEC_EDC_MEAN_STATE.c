#pragma once
#include "../DALEC_ALL/DALEC_MODULE.c"//Includes all calls to DALEC-compatible EDC functions
#include "../DALEC_ALL/mean_pool.c"
#include "../DALEC_ALL/mean_annual_pool.c"
#include "../DALEC_ALL/expdecay2.c"
#include "../../../../math_fun/std.c"
#include "../../../../math_fun/ipow.c"
#include "stdlib.h"
#include "stdio.h"

//***************General inequality EDC******************
//***************DALEC_EDC_PARS_INEQUALITY*********


  
  
  

//Doing all pools
typedef struct {
    int state_index;
    double mean_state;
    double mean_state_unc;
}  DALEC_EDC_MEAN_STATE_STRUCT;


//General inequality function
double DALEC_EDC_MEAN_STATE(DATA * DATA, void * EDCstruct){

    
    //Casting struct
    
    DALEC_EDC_MEAN_STATE_STRUCT  E = *(DALEC_EDC_MEAN_STATE_STRUCT  * ) EDCstruct;

      
 
  double *TIME_INDEX=DATA->ncdf_data.TIME_INDEX.values;
  
  DALEC *DALECmodel=(DALEC *)DATA->MODEL;
  


  
int N_timesteps=DATA->ncdf_data.TIME_INDEX.length;
  
int nopools=DALECmodel->nopools;




double MEAN_STATE=mean_pool(DATA->M_POOLS,E.state_index,N_timesteps+1,nopools);


    double residual = (MEAN_STATE - E.mean_state)/E.mean_state_unc;
double PEDC=-0.5*residual*residual;

//printf(" M_NSC = %2.2f, M_ALL_ELSE = %2.2f, PEDC = %2.2f\n", M_NSC,M_ALL_ELSE,PEDC);

    return PEDC;
}



