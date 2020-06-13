#pragma once
#include <math.h>
#include "../../../math_fun/randn.c"

int STEP_DEMCMC(double *PARS, double *pars_new, PARAMETER_INFO PI, int C,int NC){
/*DE_STEP takes a differential evolution style step*/

/*FIXEDPARS*/
/*ones and zeros depending on whether parameters are kept fixed*/
/*CONTINUE FROM HERE*/
int n,fp,c1=-1,c2=-1;
double rn;

double frac=0;

/*allocating memory for three normalized parameter vectors*/
double *npar=calloc(PI.npars,sizeof(double));
double *npar1=calloc(PI.npars,sizeof(double));
double *npar2=calloc(PI.npars,sizeof(double));
double *step=calloc(PI.npars,sizeof(double));
/*using "*pars" to store sequential outputs*/
for (n=0;n<PI.npars;n++){npar[n]=par2nor(PARS[C*PI.npars+n],PI.parmin[n],PI.parmax[n]);}

/*step 1. dparvec*/
double dparvec=0,rdev=0,rdev0=0,dir,nparstep;
int irdev;


while (frac<1){
	/*first: pick c1 and c2, make sure that (a) they are between 1 and NC, and (b) they are not C*/
	c1=-1;c2=-1;
	while (c1==-1 | c2==-1){
	c1=ceil((double)random()*NC/(RAND_MAX))-1;if (c1==C){c1=-1;}
	c2=ceil((double)random()*NC/(RAND_MAX))-1;if (c2==C | c2==c1){c2=-1;}}

	/*SAMPLING PARAMETERS*/
	for (n=0;n<PI.npars;n++){
		rn=randn();
		/*normalizing parameter*/
		npar1[n]=par2nor(PARS[c1*PI.npars+n],PI.parmin[n],PI.parmax[n]);
		npar2[n]=par2nor(PARS[c2*PI.npars+n],PI.parmin[n],PI.parmax[n]);
		/*Vector between chains c1 and c2*/
		step[n]=(npar1[n]-npar2[n])*PI.stepsize[0]+PI.stepsize[0]*0.000001*rn;
		nparstep=npar[n]+step[n];
		/*calculating relative deviation from within box*/
		rdev=(nparstep-(double)(step[n]>0))/step[n];
		if (rdev>rdev0){irdev=n;rdev0=rdev;}}

	/*step distance to wall*/
	dir=1-rdev0;rdev=0,rdev0=0;
	
	for (n=0;n<PI.npars;n++){npar[n]=npar[n]+step[n]*dir;}
	/*storing fraction of step covered*/
	frac=frac+dir*(1-frac);
}

for (n=0;n<PI.npars;n++){pars_new[n]=nor2par(npar[n],PI.parmin[n],PI.parmax[n]);}

free(npar);
free(npar1);
free(npar2);
free(step);
return 0;

}


