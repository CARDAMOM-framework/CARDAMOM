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
    int no_other_pool_indices;
    int *other_pool_indices;
    int nsc_pool_index;
}  DALEC_EDC_NSC_ABGB_RATIO_STRUCT;


//General inequality function
double DALEC_EDC_NSC_ABGB_RATIO(DATA * DATA, void * EDCstruct){
    
    
    
    
    //Casting struct
    
    DALEC_EDC_NSC_ABGB_RATIO_STRUCT  E = *(DALEC_EDC_NSC_ABGB_RATIO_STRUCT * ) EDCstruct;

      
 
  double *TIME_INDEX=DATA->ncdf_data.TIME_INDEX.values;
  
  DALEC *DALECmodel=(DALEC *)DATA->MODEL;
  


  
  int N_timesteps=DATA->ncdf_data.TIME_INDEX.length;
  
int nopools=DALECmodel->nopools;
int nofluxes=DALECmodel->nofluxes;

  //int nopools=DATA->nopools;



     //Looping through all pools
    
     //Pool inde


double M_NSC=mean_pool(DATA->M_POOLS,E.nsc_pool_index,N_timesteps+1,nopools);

    double M_ALL_ELSE=0;

int n;
    for (n=0;n< E.no_other_pool_indices;n++){M_ALL_ELSE+=mean_pool(DATA->M_POOLS,E.other_pool_indices[n],N_timesteps+1,nopools);}

  
    double residual = (M_NSC/(M_NSC+M_ALL_ELSE) - 0.1)/0.05;
double PEDC=-0.5*residual*residual;

//printf(" M_NSC = %2.2f, M_ALL_ELSE = %2.2f, PEDC = %2.2f\n", M_NSC,M_ALL_ELSE,PEDC);

    return PEDC;
}



