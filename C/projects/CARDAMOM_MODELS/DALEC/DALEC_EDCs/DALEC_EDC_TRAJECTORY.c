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
    int no_pools_to_check;
    int *pool_indices;
} DALEC_EDC_TRAJECTORY_STRUCT;


//General inequality function
double DALEC_EDC_TRAJECTORY(DATA * DATA, void * EDCstruct){
    
    
    
    
    //Casting struct
    
     DALEC_EDC_TRAJECTORY_STRUCT  E = *(DALEC_EDC_TRAJECTORY_STRUCT * ) EDCstruct;

      
 double PEDC = 0;
 
  double *TIME_INDEX=DATA->ncdf_data.TIME_INDEX.values;
  
  DALEC *DALECmodel=(DALEC *)DATA->MODEL;
  


  
  int N_timesteps=DATA->ncdf_data.TIME_INDEX.length;
  
int nopools=DALECmodel->nopools;
int nofluxes=DALECmodel->nofluxes;

  //int nopools=DATA->nopools;


  double EQF=DATA->ncdf_data.EDC_EQF;
EQF=1.1;
     //Looping through all pools
    
     //Pool inde

/*deriving mean pools here!*/
      int s,n,m,i,p;
       PEDC=0;

    for (s=0;s<E.no_pools_to_check;s++){


p = E.pool_indices[s];

double MPOOLSjan;
double MPOOLS=mean_pool(DATA->M_POOLS,p,N_timesteps+1,nopools);


double * FLUXES = DATA->M_FLUXES;
double * POOLS = DATA->M_POOLS;


/*deriving mean January pools*/
/*Assuming COMPLETE years*/

/*pool interval*/


int dint=(int)floor(N_timesteps/(TIME_INDEX[N_timesteps-1]-TIME_INDEX[0])*365.25);
/*declaring mean pool array*/

/*deriving mean jan pools*/
/*based on all jan pools except initial conditions*/

    int pidx = E.pool_indices[s];
    

for (m=0;m<(N_timesteps/dint+1);m++){
MPOOLSjan=MPOOLSjan+POOLS[nopools*(m*dint)+pidx]/(N_timesteps/dint+1);}






double *FT;
FT=calloc(nofluxes,sizeof(double));
int f=0;
for (f=0;f<nofluxes;f++){FT[f]=0;for (n=0;n<N_timesteps;n++){FT[f]+=FLUXES[n*nofluxes+f];}}



//Next step:
//Loop through all fluxes
//For each pool create "Fin" and "Fout", and add these to fluxe
double Fin=0, Fout=0;
    for (i=0;i<DALECmodel->SIOMATRIX[pidx].N_STATE_INPUT_FLUXES;i++){Fin += FT[DALECmodel->SIOMATRIX[pidx].STATE_INPUT_FLUXES[i]];}
    for (i=0;i<DALECmodel->SIOMATRIX[pidx].N_STATE_OUTPUT_FLUXES;i++){Fout += FT[DALECmodel->SIOMATRIX[pidx].STATE_OUTPUT_FLUXES[i]];}
    
free(FT);


// 
// double Fin[10];
// double Fout[10];
double Pstart;
double Pend;
/*temporary print switch*/
int psw=0;
/*exponential decay tolerance*/
double etol=0.1;

/*Inlcuding H2O pool*/
/*EDCs 7-13 - inputs, outputs and exponential tolerance*/

/*mean input/output ratio and start ratio*/
double Rm, Rs;



/*start and end pools*/
Pstart=POOLS[pidx];
Pend=POOLS[nopools*N_timesteps+pidx];
/*mean input/output*/
Rm=Fin/Fout;
/*Theoretical starting input/output*/
Rs=Rm*MPOOLSjan/Pstart;

/*if (((EDC==1 & DIAG==0) || DIAG==1 || (EDC==1 & DIAG==2 & EDCD->SWITCH[7-1+n]==1))
& ((fabs(log(Rs))>log(EQF)) || (fabs(Rs-Rm)>etol)))
{EDC=ipow(0,EDCD->SWITCH[7-1+n]);EDCD->PASSFAIL[7-1+n]=0;}*/

 PEDC+=-0.5*pow(log(Rs)/log(EQF),2) - 0.5 *pow((Rs-Rm)/etol,2);


    }
    return PEDC;
}



