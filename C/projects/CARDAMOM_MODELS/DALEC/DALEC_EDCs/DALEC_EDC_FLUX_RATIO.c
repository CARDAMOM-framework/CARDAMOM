#pragma once
#include "../DALEC_ALL/mean_flux.c"


/* Constraining ratios of C allocation: in v1006, there was:
"EDC CHECK NO 5: Allocation to canopy is comparable to allocation to fine roots
if (((EDC==1 & DIAG==0) || DIAG==1 || (EDC==1 & DIAG==2 & EDCD->SWITCH[5-1]==1)) & ((ffol+flab)>5*froot | (ffol+flab)*5<froot)){EDC=0;EDCD->PASSFAIL[5-1]=0;}"

Comparable formulation of ((ffol+flab)>5*froot | (ffol+flab)*5<froot)) for v1100 would be: 

(ARFLUXES.ALLOC_FOL_ACTUAL+(ARFLUXES.F_LABPROD-ARFLUXES.F_LABREL_ACTUAL))*5>ARFLUXES.ALLOC_ROO_ACTUAL || (ARFLUXES.ALLOC_FOL_ACTUAL+(ARFLUXES.F_LABPROD-ARFLUXES.F_LABREL_ACTUAL))*5<ARFLUXES.ALLOC_ROO_ACTUAL
 */

//Get indices for root and foliar C allocation fluxes
typedef struct {
    int numerator_flux_index;
    int denominator_flux_index;
}  DALEC_EDC_FLUX_RATIO_STRUCT;


double DALEC_EDC_FLUX_RATIO(DATA * DATA, void * EDCstruct){
       
    //Casting struct
    
    DALEC_EDC_FLUX_RATIO_STRUCT  E = *(DALEC_EDC_FLUX_RATIO_STRUCT * ) EDCstruct;    
 
  double *TIME_INDEX=DATA->ncdf_data.TIME_INDEX.values;
  
  DALEC *DALECmodel=(DALEC *)DATA->MODEL;

  int N_timesteps=DATA->ncdf_data.TIME_INDEX.length;
  
int nofluxes=DALECmodel->nofluxes;

//compute mean foliar and mean root allocation fluxes  

double M_FNUMERATOR=mean_flux(DATA->M_FLUXES,E.numerator_flux_index,N_timesteps,nofluxes);
double M_FDENOMINATOR=mean_flux(DATA->M_FLUXES,E.denominator_flux_index,N_timesteps,nofluxes);

double residual= log(M_FNUMERATOR/M_FDENOMINATOR)/log(2);
  
double PEDC=-0.5*residual*residual;

    return PEDC;
}




