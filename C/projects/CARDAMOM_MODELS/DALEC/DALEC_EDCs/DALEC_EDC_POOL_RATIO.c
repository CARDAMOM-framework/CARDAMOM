#pragma once
#include "../DALEC_ALL/DALEC_MODULE.c"//Includes all calls to DALEC-compatible EDC functions
#include "../DALEC_ALL/mean_pool.c"
#include "../DALEC_ALL/mean_annual_pool.c"
#include "../DALEC_ALL/expdecay2.c"
#include "../../../../math_fun/std.c"
#include "../../../../math_fun/ipow.c"
#include "stdlib.h"
#include "stdio.h"

//***************Constrain ratio of Croot:Cfol to be within 1:5 to 5:1 ******************
  

//Get indices for root and foliar C pools
typedef struct {
    int numerator_index;
    int denominator_index;
}  DALEC_EDC_POOL_RATIO_STRUCT;


double DALEC_EDC_POOL_RATIO(DATA * DATA, void * EDCstruct){
       
    //Casting struct
    
    DALEC_EDC_POOL_RATIO_STRUCT  E = *(DALEC_EDC_POOL_RATIO_STRUCT * ) EDCstruct;

 
  double *TIME_INDEX=DATA->ncdf_data.TIME_INDEX.values;
  
  DALEC *DALECmodel=(DALEC *)DATA->MODEL;
  

  int N_timesteps=DATA->ncdf_data.TIME_INDEX.length;
  
int nopools=DALECmodel->nopools;

//compute mean foliar and mean root pool sizes  

double M_NUMERATOR=mean_pool(DATA->M_POOLS,E.numerator_index,N_timesteps+1,nopools);
double M_DENOMINATOR=mean_pool(DATA->M_POOLS,E.denominator_index,N_timesteps+1,nopools);

double  residual= log(M_NUMERATOR/M_DENOMINATOR)/log(2);
  
double PEDC=-0.5*residual*residual;

    return PEDC;
}



