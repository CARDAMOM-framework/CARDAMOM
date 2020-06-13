#pragma once
#include <math.h>
#include "../../../math_fun/randn.c"

int STEP_DEMCMC(double *pars0, double *pars, PARAMETER_INFO PI, int C,int NC){
/*DE_STEP takes a differential evolution style step*/


/*FIXEDPARS*/
/*ones and zeros depending on whether parameters are kept fixed*/
/*CONTINUE FROM HERE*/
int n,fp,c1=-1,c2=-1;
double npar=0,npar0=0,npar1=0,npar2=0,rn1=0,rn2=0;


/*first: pick c1 and c2, make sure that (a) they are between 1 and NC, and (b) they are not C*/
while (c1==-1 | c2==-1){
c1=ceil((double)random()*NC/(RAND_MAX))-1;if (c1==C){c1=-1;}
c2=ceil((double)random()*NC/(RAND_MAX))-1;if (c2==C | c2==c1){c2=-1;}}

/*step 1. dparvec*/
double dparvec=0;/*CONTINUE FROM HERE*/

/*SAMPLING PARAMETERS*/
for (n=0;n<PI.npars;n++){
	
	fp=0;
	while (fp==0){
	/*normalizing parameter*/
		rn1=randn();
		rn2=randn();
		npar0=par2nor(pars0[C*PI.npars+n],PI.parmin[n],PI.parmax[n]);
		npar1=par2nor(pars0[c1*PI.npars+n],PI.parmin[n],PI.parmax[n]);
		npar2=par2nor(pars0[c2*PI.npars+n],PI.parmin[n],PI.parmax[n]);
		/*printf("c2 = %i, PI.npars = %i, n = %i, pars0[c2*PI.npars+n] = %2.2e ",c2,PI.npars,n, pars0[c2*PI.npars+n]);*/
	/*Vector between chains c1 and c2: note: PI.stepsize (without rn1 scaling) doesn't work*/
		npar=npar0+(npar1-npar2)*PI.stepsize[0]/10+rn2*PI.stepsize[0]*0.05;
		/*printf("npar0 = %6.4e, npar1 = %6.4e, npar2 = %6.4e, npar = %6.4e\n",npar0,npar1,npar2,npar);*/
		/*constraining parameter within 0-1 limits*/
		if (npar>0 & npar<1){fp=1;pars[C*PI.npars+n]=nor2par(npar,PI.parmin[n],PI.parmax[n]);}}

}

return 0;

}


