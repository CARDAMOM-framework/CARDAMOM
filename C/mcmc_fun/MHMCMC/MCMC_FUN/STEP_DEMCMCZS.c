#pragma once
#include <math.h>
#include <stdlib.h>
#include "../../../math_fun/randn.c"

/*DE-MCZ parallel direction update (ter Braak & Vrugt 2008, eq. 2): jump along
 *the difference of two states drawn from the archive Z (rather than from the
 *other live chains, as in standard DE-MC/STEP_DEMCMC.c)*/
int STEP_DEMCMCZ_PARALLEL(double *xi, double *Z, int M, double *pars_new, PARAMETER_INFO PI){

int n,r1=-1,r2=-1;
double rn;

double *npar=calloc(PI.npars,sizeof(double));
double *nz1=calloc(PI.npars,sizeof(double));
double *nz2=calloc(PI.npars,sizeof(double));

for (n=0;n<PI.npars;n++){npar[n]=par2nor(xi[n],PI.parmin[n],PI.parmax[n]);}

/*picking two distinct rows of the archive*/
while (r1==r2){
r1=ceil((double)random()*M/((double)RAND_MAX))-1;
r2=ceil((double)random()*M/((double)RAND_MAX))-1;}

/*gamma: standard DE-MC scaling, with an occasional large jump (10% of updates)*/
double gamma_de=2.38/sqrt(2.0*(double)PI.npars);
if ((double)random()/(double)RAND_MAX<0.1){gamma_de=1;}

int withinlim=1;
for (n=0;n<PI.npars;n++){
rn=randn();
nz1[n]=par2nor(Z[r1*PI.npars+n],PI.parmin[n],PI.parmax[n]);
nz2[n]=par2nor(Z[r2*PI.npars+n],PI.parmin[n],PI.parmax[n]);
npar[n]=npar[n]+gamma_de*(nz1[n]-nz2[n])+0.01*rn;
if (npar[n]<0 || npar[n]>1){withinlim=0;}
}

if (withinlim==1){
for (n=0;n<PI.npars;n++){pars_new[n]=nor2par(npar[n],PI.parmin[n],PI.parmax[n]);}
}

free(npar);free(nz1);free(nz2);
return withinlim;
}


/*DE-MC snooker update (ter Braak & Vrugt 2008, eqs. 3-4): propose a step
 *along the line through the current chain and a reference state z, both
 *drawn from the archive Z. Requires the (npars-1)*log(.) Metropolis-Hastings
 *correction returned via gratio, since the proposal is not symmetric.*/
int STEP_DEMCMCZ_SNOOKER(double *xi, double *Z, int M, double *pars_new, PARAMETER_INFO PI, double *gratio){

int n,rz=-1,r1=-1,r2=-1;

double *npar=calloc(PI.npars,sizeof(double));
double *nz=calloc(PI.npars,sizeof(double));
double *d=calloc(PI.npars,sizeof(double));

for (n=0;n<PI.npars;n++){npar[n]=par2nor(xi[n],PI.parmin[n],PI.parmax[n]);}

/*picking three distinct rows: z (the snooker reference line partner) and r1,r2 (for the projection)*/
while (rz==r1 || rz==r2 || r1==r2){
rz=ceil((double)random()*M/((double)RAND_MAX))-1;
r1=ceil((double)random()*M/((double)RAND_MAX))-1;
r2=ceil((double)random()*M/((double)RAND_MAX))-1;}

double dnorm2=0;
for (n=0;n<PI.npars;n++){
nz[n]=par2nor(Z[rz*PI.npars+n],PI.parmin[n],PI.parmax[n]);
d[n]=npar[n]-nz[n];
dnorm2=dnorm2+d[n]*d[n];}

gratio[0]=0;

/*Guard against a degenerate (zero-length) snooker line: cannot define a direction, so reject*/
if (dnorm2<1e-12){
free(npar);free(nz);free(d);
return 0;}

double dnorm=sqrt(dnorm2);

/*projecting zR1 and zR2 orthogonally onto the line xi-z*/
double s1=0,s2=0;
for (n=0;n<PI.npars;n++){
s1=s1+(par2nor(Z[r1*PI.npars+n],PI.parmin[n],PI.parmax[n])-nz[n])*d[n]/dnorm;
s2=s2+(par2nor(Z[r2*PI.npars+n],PI.parmin[n],PI.parmax[n])-nz[n])*d[n]/dnorm;}

/*gamma_s ~ U[1.2,2.2], the randomised snooker step size ter Braak & Vrugt (2008) use as default*/
double gammas=1.2+((double)random()/(double)RAND_MAX)*1.0;

int withinlim=1;
double xstarnorm2=0;
for (n=0;n<PI.npars;n++){
npar[n]=npar[n]+gammas*(s1-s2)*d[n]/dnorm;
xstarnorm2=xstarnorm2+(npar[n]-nz[n])*(npar[n]-nz[n]);
if (npar[n]<0 || npar[n]>1){withinlim=0;}}

if (withinlim==1){
for (n=0;n<PI.npars;n++){pars_new[n]=nor2par(npar[n],PI.parmin[n],PI.parmax[n]);}
/*Metropolis-Hastings correction for the snooker geometry (eq. 4)*/
gratio[0]=((double)PI.npars-1.0)*log(sqrt(xstarnorm2)/dnorm);}

free(npar);free(nz);free(d);
return withinlim;
}
