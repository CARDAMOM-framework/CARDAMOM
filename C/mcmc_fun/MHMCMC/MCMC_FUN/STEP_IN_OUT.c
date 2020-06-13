#pragma once
#include <math.h>
#include "../../../math_fun/randn.c"

double STEP_IN_OUT(double *pars0, double *pars, PARAMETER_INFO PI){

/*FIXEDPARS*/
/*ones and zeros depending on whether parameters are kept fixed*/

int n;
double npar=0,rn=0,fp=0;

/*SAMPLING PARAMETERS*/
for (n=0;n<PI.npars;n++){
	/*normalizing parameter*/
		rn=randn();
		npar=par2nor(pars0[n],PI.parmin[n],PI.parmax[n])+rn*PI.stepsize[n]*(1-PI.parfix[n]);
		
		pars[n]=nor2par(npar,PI.parmin[n],PI.parmax[n]);
		/*constraining parameter within 0-1 limits*/
		if (npar<0 | npar>1){fp=log(0);}}



return fp;

}


