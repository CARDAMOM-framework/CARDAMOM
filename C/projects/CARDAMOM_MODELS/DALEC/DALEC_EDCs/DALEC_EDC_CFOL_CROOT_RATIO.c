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
    int cfol_pool_index;
    int croot_pool_index;
}  DALEC_EDC_CFOL_CROOT_RATIO_STRUCT;


double DALEC_EDC_CFOL_CROOT_RATIO(DATA * DATA, void * EDCstruct){
       
    //Casting struct
    
    DALEC_EDC_CFOL_CROOT_RATIO_STRUCT  E = *(DALEC_EDC_CFOL_CROOT_RATIO_STRUCT * ) EDCstruct;

      
 
  double *TIME_INDEX=DATA->ncdf_data.TIME_INDEX.values;
  
  DALEC *DALECmodel=(DALEC *)DATA->MODEL;
  


  
  int N_timesteps=DATA->ncdf_data.TIME_INDEX.length;
  
int nopools=DALECmodel->nopools;

//compute mean foliar and mean root pool sizes  

double M_CFOL=mean_pool(DATA->M_POOLS,E.cfol_pool_index,N_timesteps+1,nopools);
double M_CROOT=mean_pool(DATA->M_POOLS,E.croot_pool_index,N_timesteps+1,nopools);

double ratio = M_CFOL / M_CROOT;
double residual = 0; 
if (ratio >= 0.2 && ratio <= 5) {// The ratio is within the desired range
    residual = 0;
} else {// The ratio is outside the desired range
    residual= log(M_CFOL/M_CROOT)/log(2);
}
  
double PEDC=-0.5*residual*residual;

    return PEDC;
}



