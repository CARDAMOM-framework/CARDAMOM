#pragma once
#include <math.h>
#include <stdlib.h>
#include "../../../math_fun/randn.c"

/*HYBRID_AIDE proposal: compose a CARDAMOM DEMCMC translation with an affine
 *stretch move. The DE translation uses the local CARDAMOM gamma convention:
 *90% small steps (0.1 * textbook gamma) and 10% full difference-vector jumps.*/
int STEP_HYBRID_AIDE(double *PARS, double *pars_new, PARAMETER_INFO PI, int C, int NC, double *gratio){

int n,j1=C,j2=C,j3=C;
double rn;

while (j1==C){j1=ceil((double)random()*NC/((double)RAND_MAX))-1;}
while (j2==C || j2==j1){j2=ceil((double)random()*NC/((double)RAND_MAX))-1;}
while (j3==C || j3==j1 || j3==j2){j3=ceil((double)random()*NC/((double)RAND_MAX))-1;}

double *xcur=calloc(PI.npars,sizeof(double));
double *x1=calloc(PI.npars,sizeof(double));
double *x2=calloc(PI.npars,sizeof(double));
double *x3=calloc(PI.npars,sizeof(double));
double *xint=calloc(PI.npars,sizeof(double));
double *xnew=calloc(PI.npars,sizeof(double));

for (n=0;n<PI.npars;n++){
xcur[n]=par2nor(PARS[C*PI.npars+n],PI.parmin[n],PI.parmax[n]);
x1[n]=par2nor(PARS[j1*PI.npars+n],PI.parmin[n],PI.parmax[n]);
x2[n]=par2nor(PARS[j2*PI.npars+n],PI.parmin[n],PI.parmax[n]);
x3[n]=par2nor(PARS[j3*PI.npars+n],PI.parmin[n],PI.parmax[n]);
}

double gamma_de=1 - (1 - 2.38 / sqrt(2.0 * PI.npars) * 0.1) *
	(double)(((double)random() / (double)RAND_MAX) < 0.9);

double a_gw=1.0+exp(randn());
double u_z=(double)random()/(double)RAND_MAX;
double z=pow((u_z*(sqrt(a_gw)-1.0/sqrt(a_gw))+1.0/sqrt(a_gw)),2.0);

int de_first=((double)random()/(double)RAND_MAX>0.5);
int withinlim=1;

if (de_first){
/*DEMCMC translation -> affine stretch*/
for (n=0;n<PI.npars;n++){
rn=randn();
xint[n]=xcur[n]+gamma_de*(x2[n]-x1[n])+0.000001*rn;
xnew[n]=x3[n]+z*(xint[n]-x3[n]);
}}
else{
/*affine stretch -> DEMCMC translation*/
for (n=0;n<PI.npars;n++){
rn=randn();
xint[n]=x3[n]+z*(xcur[n]-x3[n]);
xnew[n]=xint[n]+gamma_de*(x2[n]-x1[n])+0.000001*rn;
}}

for (n=0;n<PI.npars;n++){
if (xnew[n]<0 || xnew[n]>1){withinlim=0;}
}

gratio[0]=0;
if (withinlim==1){
for (n=0;n<PI.npars;n++){pars_new[n]=nor2par(xnew[n],PI.parmin[n],PI.parmax[n]);}
gratio[0]=((double)PI.npars-1.0)*log(z);
}

free(xcur);
free(x1);
free(x2);
free(x3);
free(xint);
free(xnew);

return withinlim;
}
