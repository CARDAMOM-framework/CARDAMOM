
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../MCMC_FUN/INITIALIZE_MCMC_OUTPUT.c"
/*USER DEFINED STRUCTURE*/
/*This is the ONLY structure the user should alter, adapt, etc.,
 *  * in order to optimally stored all required model drivers, and measurements
 *   * observations, etc. etc.*/

typedef struct{
double *dri;
double *obs;
int nd;
}DATA;





int READ_PARI_DATA(PARAMETER_INFO *PI, DATA *DATA, MCMC_OUTPUT *MCOUT, char *CLA[]){
/*defining data structure*/
/*note that this also includes
 * allocating any memory required for model output*/
/*DATA: essentially defines all memory required for model
 * input and output*/

DATA->nd=6;
double dri[]={0.01,0.05,0.1,0.5,1,1.5};
double obs[]={1.9534,2.3993,1.9876,3.7763,4.4265,6.8153};
DATA->dri=calloc(6,sizeof(double));
DATA->obs=calloc(6,sizeof(double));
int n;for (n=0;n<6;n++){DATA->dri[n]=dri[n];DATA->obs[n]=obs[n];}


/*defining parameter structure*/
PI->npars=4;
double pn[]={0.05,0.05,0.05,0.05};
double px[]={5,5,5,5};
double pi[]={1.5,1.5,1.5,1.5};
double pf[]={0,0,0,0};
double sz[]={0.01,0.01,0.01,0.01};
PI->parmin=calloc(4,sizeof(double));
PI->parini=calloc(4,sizeof(double));
PI->parmax=calloc(4,sizeof(double));
PI->parfix=calloc(4,sizeof(double));

PI->stepsize=calloc(4,sizeof(double));
for (n=0;n<PI->npars;n++){
PI->parmin[n]=pn[n];
PI->parmax[n]=px[n];
PI->parini[n]=pi[n];
PI->parfix[n]=pf[n];
PI->stepsize[n]=sz[n];}


INITIALIZE_MCMC_OUTPUT(*PI,MCOUT);

return 0;
}







int READ_MCOPT(MCMC_OPTIONS *MCOPT, char *CLA[]){
/*defining MCMC_OPTIONS structure*/
MCOPT->APPEND=0;
MCOPT->nADAPT=50;
MCOPT->fADAPT=0.5;
MCOPT->nOUT=1000;
MCOPT->nPRINT=500;
MCOPT->nWRITE=5;
MCOPT->returnpars=0;
MCOPT->randparini=1;
MCOPT->fixedpars=0;
char outfile[200]="MCMC_OUTPUT/PARS_OUT_";
char stepfile[200]="MCMC_OUTPUT/STEP_OUT_";
strcat(outfile,CLA[1]);
strcat(stepfile,CLA[1]);
/*directory*/
strcpy(MCOPT->outfile,outfile);
strcpy(MCOPT->stepfile,stepfile);

return 0;
}


/*Enter all fields originally defined with CALLOC*/
int MEMORY_CLEANUP(DATA DATA, PARAMETER_INFO PI, MCMC_OPTIONS MCOPT, MCMC_OUTPUT MCOUT){

free(PI.parmin);
free(PI.parmax);
free(PI.parini);
free(PI.parfix);
free(PI.stepsize);
free(DATA.dri);
free(DATA.obs);

/*freeing MCOUT field*/
free(MCOUT.best_pars);


return 0;

}








