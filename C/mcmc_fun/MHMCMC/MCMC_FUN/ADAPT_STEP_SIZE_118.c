#pragma once
#include "../../../math_fun/cholesky.c"
int ADAPT_STEP_SIZE_118(double *PARSALL, PARAMETER_INFO PI, COUNTERS * N, MCMC_OPTIONS MCO){



/*first adjusting all step sizes*/

int n;
double fac=1, scfac=(1-(double)N->ACC/(double)MCO.nOUT/2);
double adapfac=scfac*0.001+1;
/*ratio loc vs total*/
/*
double adapfac=1.5;*/


/*MAKE THIS COMPATIBLE WITH STRUCTS AND CODE*/



/*Dimension specific adjustments*/
if (N->ACCLOC>PI.npars*10){

/*Growth in all dims if ACCRATE>0.23*/
if (N->ACCRATE<0.23){N->amp=N->amp/adapfac;}
else {N->amp=N->amp*adapfac;}


	double norparstd,norparmean;
	double **norparvec;
	declare_matrix(&norparvec,PI.npars, N->ACCLOC);
	double parrange;
	int p, i, os;
	/*Offset (gaining 2, losing 1)*/
	os=floor((double)N->ACCLOC/2);
	/*std of each parameter (for all recently N->ACCLOCepted parameters)*/
	for (p=0;p<PI.npars;p++){
	for (n=0;n<(N->ACCLOC-os);n++){
	/*transforming parameters and storing in pointer*/
	norparvec[p][n]=par2nor(PARSALL[PI.npars*(n+os)+p],PI.parmin[p],PI.parmax[p]);}}


/*Calculare covariance of norparvec AND store*/
double **CM,**CCM;
declare_matrix(&CM,PI.npars,PI.npars);
declare_matrix(&CCM,PI.npars,PI.npars);
int pp;
for (p=0;p<PI.npars;p++){
for (pp=0;pp<PI.npars;pp++){
CM[p][pp]=covariance(norparvec[p],norparvec[pp],N->ACCLOC-os);}}


/*Now adding to existing covariance matrix*/
/*Assuming unbiased pooled covariance @ inf*/
for (p=0;p<PI.npars;p++){
for (pp=0;pp<PI.npars;pp++){
N->parcovariance[p][pp]=CM[p][pp]*pow(2.38,2)/(double)PI.npars;}}

/*Now calculating cholesky and storing*/



int choldiag=cholesky(N->parcovariance,PI.npars,CCM);

if (choldiag==1){
for (p=0;p<PI.npars;p++){
for (pp=0;pp<PI.npars;pp++){
N->parcholesky[p][pp]=CCM[p][pp];}}}


		/*calculating variability*/
		
		
		/*Storing in N->parstd*/




	free(*(norparvec));
	free(norparvec);
	free(*(CM));free(CM);
	free(*(CCM));free(CCM);
	}


/*OVERALL CHECK*/
/*printf("STEPSIZE = %8.6f (minstepsize = %8.6f, Niterations = %10i)\n",PI.stepsize[n],minstepsize,N->ITER);*/







return 0;

}





