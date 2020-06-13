#pragma once
#include <math.h>
#include "../../../math_fun/randn.c"

int STEP(double *pars0, double *pars, PARAMETER_INFO PI){

/*FIXEDPARS*/
/*ones and zeros depending on whether parameters are kept fixed*/

int n,fp;
double npar=0,rn=0;

/*SAMPLING PARAMETERS*/
for (n=0;n<PI.npars;n++){
	fp=0;
	while (fp==0){
	/*normalizing parameter*/
		rn=randn();
		npar=par2nor(pars0[n],PI.parmin[n],PI.parmax[n])+rn*PI.stepsize[n]*(1-PI.parfix[n]);
		
		/*constraining parameter within 0-1 limits*/
		if (npar>0 & npar<1){fp=1;pars[n]=nor2par(npar,PI.parmin[n],PI.parmax[n]);}}

}

return 0;

}


