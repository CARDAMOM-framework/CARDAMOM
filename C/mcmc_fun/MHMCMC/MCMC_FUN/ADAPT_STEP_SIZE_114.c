#pragma once
#include "../../../math_fun/cholesky.c"
int ADAPT_STEP_SIZE_114(double *PARSALL, PARAMETER_INFO PI, COUNTERS * N, MCMC_OPTIONS MCO){



/*first adjusting all step sizes*/

int n;
double fac=1, scfac=(1-(double)N->ACC/(double)MCO.nOUT/2);
double adapfac=scfac*(MCO.nADAPT/100)/1000+1;
/*ratio loc vs total*/
double acc2tot=(double)N->ACCLOC/(double)N->ACC;
/*
double adapfac=1.5;*/
/*double minstepsize=100./(double)N->ITER;
if (minstepsize>0.01){minstepsize=0.01;}
if (minstepsize<MCO.minstepsize){minstepsize=MCO.minstepsize;}
*/
double minstepsize=MCO.minstepsize;


/*MAKE THIS COMPATIBLE WITH STRUCTS AND CODE*/


if (N->ACCRATE<0.23){N->amp=N->amp/adapfac;}
else {N->amp=N->amp*adapfac;}
/*Growth in all dims if ACCRATE>0.23*/

/*Dimension specific adjustments*/
if (N->ACCLOC>3){

	double norparstd,norparmean;
	double **norparvec;
	declare_matrix(&norparvec,PI.npars, N->ACCLOC);
	double parrange;
	int p, i;
	/*std of each parameter (for all recently N->ACCLOCepted parameters)*/
	for (p=0;p<PI.npars;p++){
		for (n=0;n<N->ACCLOC;n++){
			/*transforming parameters and storing in pointer*/
			norparvec[p][n]=par2nor(PARSALL[PI.npars*n+p],PI.parmin[p],PI.parmax[p]);}
	
}

/*Calculare covariance of norparvec AND store*/
double **CM,**CCM;
declare_matrix(&CM,PI.npars,PI.npars);
declare_matrix(&CCM,PI.npars,PI.npars);
int pp;
for (p=0;p<PI.npars;p++){
for (pp=0;pp<PI.npars;pp++){
CM[p][pp]=covariance(norparvec[p],norparvec[pp],N->ACCLOC);}}


/*Now adding to existing covariance matrix*/
/*Assuming unbiased pooled covariance @ inf*/
for (p=0;p<PI.npars;p++){
for (pp=0;pp<PI.npars;pp++){
N->parcovariance[p][pp]=CM[p][pp]*(acc2tot)*pow(2.38,2)/(double)PI.npars + N->parcovariance[p][pp]*(1-acc2tot);}}

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





