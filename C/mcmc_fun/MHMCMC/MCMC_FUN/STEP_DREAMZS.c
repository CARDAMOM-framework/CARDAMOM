#pragma once
#include <math.h>
#include <stdlib.h>
#include "../../../math_fun/randn.c"

/*DREAM(ZS)-lite parallel direction update: draw a differential-evolution
 *direction from the archive, but only apply it to a crossover-selected subset
 *of parameters. This is the main DREAM-style difference from DEMCMCZS, which
 *updates every parameter on each parallel proposal.*/
int STEP_DREAMZS_PARALLEL(double *xi, double *Z, int M, double *pars_new, PARAMETER_INFO PI, double CR, int *nupdate_out){

int n,r1=-1,r2=-1;
double rn;

if (CR<1.0/(double)PI.npars){CR=1.0/(double)PI.npars;}
if (CR>1.0){CR=1.0;}

double *npar=calloc(PI.npars,sizeof(double));
double *nz1=calloc(PI.npars,sizeof(double));
double *nz2=calloc(PI.npars,sizeof(double));
int *update=calloc(PI.npars,sizeof(int));

for (n=0;n<PI.npars;n++){npar[n]=par2nor(xi[n],PI.parmin[n],PI.parmax[n]);}

/*picking two distinct rows of the archive*/
while (r1==r2){
r1=ceil((double)random()*M/((double)RAND_MAX))-1;
r2=ceil((double)random()*M/((double)RAND_MAX))-1;}

/*Force at least one updated dimension so low CR values cannot produce a null move.*/
int force_dim=(int)(((double)random()/((double)RAND_MAX))*PI.npars);
if (force_dim>=PI.npars){force_dim=PI.npars-1;}

int nupdate=0;
for (n=0;n<PI.npars;n++){
if (n==force_dim || (double)random()/(double)RAND_MAX<CR){
update[n]=1;
nupdate=nupdate+1;
}}

/*DREAM uses the number of updated dimensions for the DE scale.*/
double gamma_de=2.38/sqrt(2.0*(double)nupdate);
if ((double)random()/(double)RAND_MAX<0.1){gamma_de=1;}

int withinlim=1;
for (n=0;n<PI.npars;n++){
rn=randn();
nz1[n]=par2nor(Z[r1*PI.npars+n],PI.parmin[n],PI.parmax[n]);
nz2[n]=par2nor(Z[r2*PI.npars+n],PI.parmin[n],PI.parmax[n]);
if (update[n]==1){
npar[n]=npar[n]+gamma_de*(nz1[n]-nz2[n])+0.01*rn;
}
if (npar[n]<0 || npar[n]>1){withinlim=0;}
}

if (withinlim==1){
for (n=0;n<PI.npars;n++){pars_new[n]=nor2par(npar[n],PI.parmin[n],PI.parmax[n]);}
}

if (nupdate_out!=NULL){nupdate_out[0]=nupdate;}

free(npar);
free(nz1);
free(nz2);
free(update);
return withinlim;
}
