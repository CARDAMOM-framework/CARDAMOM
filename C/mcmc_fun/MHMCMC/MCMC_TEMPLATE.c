#include "../../auxi_fun/oksofar.c"

/*defines all the structures, i.e. DATA, MCO, PI*/

#include "MCMC_FUN/MCMCOPT.c"
#include "MCMC_SETUP/MCMC_MODULES.c"


#include "MCMC_FUN/MHMCMC.c"

/*MCMC subroutine*/
#include "MCMC_SETUP/MODEL_LIKELYHOOD.c"


int main(int argc,char *CLA[]){
/*To correctly set-up the MHMCMC*/

/*defining data structure*/
DATA DATA;
/*defining parameter structure*/
/*full parameter structure defined in MCMC_FUN/MCMCOPT.c*/
PARAMETER_INFO PI;
/*defining MCMC_OPTIONS structure*/
MCMC_OPTIONS MCOPT;
/*defining the MODEL_LIKELYHOOD*/
/*MODEL_LIKELYHOOD MODEL;*/

/*STORING ANY MCMC OUTPUT OF IMMEDIATE USE HERE
 * examples include best parameter combination
 * run stats,*/
MCMC_OUTPUT MCOUT;





/*Defining all MCMC components*/
/*USER DEFINED: SETUP MCMC - templates provides*/
READ_PARI_DATA(&PI, &DATA, &MCOUT, CLA);
READ_MCOPT(&MCOPT,CLA);

/*the function MODEL should be defined in the subroutines*/


printf("About to run the code\n");
/*MHMCMC*/


printf("PI.npars = %d\n",PI.npars);
printf("PI.parmax[2] = %f\n",PI.parmax[2]);
printf("DATA.dri[0] = %f\n",DATA.dri[0]);
printf("MCOPT.nOUT = %d\n",MCOPT.nOUT);
printf("MODELPTR = %d\n",(int)&MODEL_LIKELYHOOD);
printf("MCOPT.outfile %s\n",MCOPT.outfile);
printf("MCOPT.stepfile %s\n",MCOPT.stepfile);


/*double *BESTPARS;
BESTPARS=MHMCMC(&MODEL_LIKELYHOOD,DATA,PI,MCO);
int n;for (n=0;n<PI.npars;n++){printf("%f\n",BESTPARS[n]);}
free(BESTPARS);*/


MHMCMC(MODEL_LIKELYHOOD,DATA,PI,MCOPT,&MCOUT);

/*displaying some MCMC stats*/
printf("MCMC.complete = %d\n", MCOUT.complete);
/*best parameters*/
printf("BEST PARAMETER FIT\n");
int n;
for (n=0;n<PI.npars;n++){printf(" %f ", MCOUT.best_pars[n]);}
printf("\n");


/*???????*/
/*User Defined function needed to clean up memory*/
MEMORY_CLEANUP(DATA,PI,MCOPT,MCOUT);



return 0;

}





