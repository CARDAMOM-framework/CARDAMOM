#pragma once
#include "../DALEC_ALL/DALEC_MODULE.c"//Includes all calls to DALEC-compatible EDC functions
#include "../DALEC_ALL/mean_pool.c"
#include "../DALEC_ALL/mean_annual_pool.c"
#include "../DALEC_ALL/expdecay2.c"
#include "../../../../math_fun/std.c"
#include "../../../../math_fun/ipow.c"
#include "stdlib.h"
#include "stdio.h"
#include "math.h"  

//***************General inequality EDC******************
//***************DALEC_EDC_PARS_INEQUALITY*********


static int traj_attempt_count[100] = {0};
static int traj_pass_count[100] = {0};
static int fin_zero_count[100] = {0};
static int fin_neg_count[100] = {0};
static int fout_zero_count[100] = {0};
static int fout_neg_count[100] = {0};  
  

//Doing all pools
typedef struct {
    int no_pools_to_check;
    int *pool_indices;
    double *pool_eqf;
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

     //Looping through all pools

/*deriving mean pools here!*/
int s,n,m,i,p;
PEDC=0;

double * FLUXES = DATA->M_FLUXES;
double * POOLS = DATA->M_POOLS;

double *FT;
FT=calloc(nofluxes,sizeof(double));

int f=0;
for (f=0;f<nofluxes;f++){
  FT[f]=0;
  for (n=0;n<N_timesteps;n++){
    FT[f]+=FLUXES[n*nofluxes+f];
    }
  }


for (s=0;s<E.no_pools_to_check;s++){
  
  double EQF=E.pool_eqf[s];
  p = E.pool_indices[s];
  traj_attempt_count[p] += 1;

  double MPOOLSjan=0;
  double MPOOLS=mean_pool(DATA->M_POOLS,p,N_timesteps+1,nopools);
  
  /*deriving mean January pools*/
  /*Assuming COMPLETE years*/
  
  /*pool interval*/
  int dint=(int)floor(N_timesteps/(TIME_INDEX[N_timesteps-1]-TIME_INDEX[0])*365.25);
  /*declaring mean pool array*/
  
  /*deriving mean jan pools*/
  /*based on all jan pools except initial conditions*/
  
    
  
  for (m=0;m<(N_timesteps/dint+1);m++){
  MPOOLSjan=MPOOLSjan+POOLS[nopools*(m*dint)+p]/(N_timesteps/dint+1);}


  //Next step:
  //Loop through all fluxes
  //For each pool create "Fin" and "Fout", and add these to fluxe
  double Fin=0, Fout=0;
  for (i=0;i<DALECmodel->SIOMATRIX[p].N_STATE_INPUT_FLUXES;i++){Fin += FT[DALECmodel->SIOMATRIX[p].STATE_INPUT_FLUXES[i]];}
  for (i=0;i<DALECmodel->SIOMATRIX[p].N_STATE_OUTPUT_FLUXES;i++){Fout += FT[DALECmodel->SIOMATRIX[p].STATE_OUTPUT_FLUXES[i]];}

  // 
  // double Fin[10];
  // double Fout[10];
  double Pstart;
  double Pend;
  /*temporary print switch*/
  int psw=0;
  /*exponential decay tolerance*/
  double etol=.1;
  
  /*Inlcuding H2O pool*/
  /*EDCs 7-13 - inputs, outputs and exponential tolerance*/
  
  /*mean input/output ratio and start ratio*/
  double Rm, Rs;

  /*start and end pools*/
  Pstart=POOLS[p];
  Pend=POOLS[nopools*N_timesteps+p];
  /*mean input/output*/
  Rm=Fin/Fout;
  /*Theoretical starting input/output*/
  Rs=Rm*MPOOLSjan/Pstart;
  /*EB test version*/
  // Rs=MPOOLSjan/Pstart;

/*if (((EDC==1 & DIAG==0) || DIAG==1 || (EDC==1 & DIAG==2 & EDCD->SWITCH[7-1+n]==1))
& ((fabs(log(Rs))>log(EQF)) || (fabs(Rs-Rm)>etol)))
{EDC=ipow(0,EDCD->SWITCH[7-1+n]);EDCD->PASSFAIL[7-1+n]=0;}*/

// PEDC+=-0.5*pow(log(Rs)/log(EQF),2) - 0.5 *pow((Rs-Rm)/etol,2);
 /*EB test version*/
        // 1. Calculate this pool's penalty
        double pool_pedc = -0.5*pow(log(Rs)/log(EQF),2) - 0.5 *pow(log(Rs/Rm)/log(1+etol),2);
        
        // 2. Add it to the total parameter set penalty
        PEDC += pool_pedc;

        // 3. Check if the math survived (no division by zero or log of negatives)
        // 3. Check if the math survived
        if (isfinite(pool_pedc)) {
            traj_pass_count[p] += 1;
        } else {
            // Aggregate the exact reason for the failure
            if (Fin == 0.0) fin_zero_count[p] += 1;
            else if (Fin < 0.0) fin_neg_count[p] += 1;

            if (Fout == 0.0) fout_zero_count[p] += 1;
            else if (Fout < 0.0) fout_neg_count[p] += 1;
        }
    }

    free(FT);
    
    // 4. Print the stats! 
    // Added a static counter so your terminal doesn't crash from printing millions of lines
  static int traj_print_throttle = 0;
    traj_print_throttle++;
    
    // Set to print every 100 passes for testing (adjust as needed)
    if (traj_print_throttle % 50 == 0) { 
        printf("\n--- Trajectory (EDC 8) Stats at %i attempts ---\n", traj_print_throttle);
        for (int j = 0; j < DATA->nopools; j++) {
            if (traj_attempt_count[j] > 0) {
                double percent = (100.0 * (double)traj_pass_count[j] / (double)traj_attempt_count[j]);
                int total_fails = traj_attempt_count[j] - traj_pass_count[j];
                
                printf("pool %2i; Att = %6i, Pass = %6i (%5.2f%%) | Fails: %6i [Fin=0: %6i, Fin<0: %6i | Fout=0: %6i, Fout<0: %6i]\n", 
                        j, traj_attempt_count[j], traj_pass_count[j], percent, 
                        total_fails, fin_zero_count[j], fin_neg_count[j], fout_zero_count[j], fout_neg_count[j]);
            }
        }
        fflush(stdout);
    }
     // printf("PEDC = %2.2f\n",PEDC);
    return PEDC;
}