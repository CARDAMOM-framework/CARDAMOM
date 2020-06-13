#pragma once
#include <math.h>
#include "../../../math_fun/randn.c"
#include "mcmc_boundary_bounce.c"

int STEP_WITH_BOUNCE_NN(double *pars0, double *pars, PARAMETER_INFO PI){

/*FIXEDPARS*/
/*ones and zeros depending on whether parameters are kept fixed*/

int n,fp;
double npar,rn,step, npar_new;
double step_aug=1;

/*SAMPLING PARAMETERS*/
for (n=0;n<PI.npars;n++){
	/*sampling for each parameter*/

	/*normalizing parameter*/
		rn=randn();
		step = rn*PI.stepsize[n]*(1-PI.parfix[n])*step_aug;
		npar=par2nor(pars0[n],PI.parmin[n],PI.parmax[n]);
		npar_new=mcmc_boundary_bounce(npar,step); 
		/*constraining parameter within 0-1 limits*/
		pars[n]=nor2par(npar_new,PI.parmin[n],PI.parmax[n]);}

return 0;

}


