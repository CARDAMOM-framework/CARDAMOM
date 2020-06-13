#pragma once
#include "../../../math_fun/declare_matrix.c"

int ADAPT_STEP_SIZE(double *PARSALL, PARAMETER_INFO PI, COUNTERS N, MCMC_OPTIONS MCO){



/*first adjusting all step sizes*/

int n;
double fac=2;
double adapfac=1.5;
double minstepsize=1000./(double)N.ITER;
if (minstepsize>0.1){minstepsize=0.1;}
if (minstepsize<MCO.minstepsize){minstepsize=MCO.minstepsize;}
N.ACCRATE=(double)N.ACCLOC/(double)MCO.nADAPT;


/*MAKE THIS COMPATIBLE WITH STRUCTS AND CODE*/

if (N.ACCRATE<0.23){
for (n=0;n<PI.npars;n++){PI.stepsize[n]=PI.stepsize[n]/adapfac;}}
else if(N.ACCRATE>0.44){
for (n=0;n<PI.npars;n++){PI.stepsize[n]=PI.stepsize[n]*adapfac;}}


/*Dimension specific adjustments*/
/*For PCArotation case, need to recast samples in PCA space*/

int dimadapt=1;
if (N.ACCLOC>3 && dimadapt==1){
	/*need to remove this!!*/
	double norparstd;
	double *norparvec=calloc(PI.npars,sizeof(double));
	double **norparmat;
	declare_matrix(&norparmat,PI.npars,N.ACCLOC);
	double parrange;
	int p, i;
	
	/*Step 1. normalize and rotate parameter vectors*/
	/*std of each parameter (for all recently N.ACCLOCepted parameters)*/
	for (n=0;n<N.ACCLOC;n++){
			/*Step 1. transforming parameters and storing in pointer*/
	for (p=0;p<PI.npars;p++){norparvec[p]=par2nor(PARSALL[PI.npars*n+p],PI.parmin[p],PI.parmax[p]);}
		/*Step 2. rotating and storing in matrix*/
	for (p=0;p<PI.npars;p++){for (i=0;i<PI.npars;i++){norparmat[p][n]+=norparvec[i]*PI.PCArotation[i][p];}}}
	/*Step 3. adapt*/
		for (p=0;p<PI.npars;p++){
		/*calculating variability*/
		norparstd=std(norparmat[p],N.ACCLOC);
		/*reducing if the stepsize is too small*/
 		/*if (PI.stepsize[p]<norparstd/fac & N.ACCRATE<0.23)*/
		/*disabled step-dim-adapt*/
		/*{PI.stepsize[p]=PI.stepsize[p]*sqrt(adapfac);}*/
 		if (PI.stepsize[p]<norparstd/2){PI.stepsize[p]=PI.stepsize[p]*sqrt(adapfac);}
 		if (PI.stepsize[p]>norparstd*2){PI.stepsize[p]=PI.stepsize[p]/sqrt(adapfac);}
}
	free(*norparmat);free(norparmat);free(norparvec);

}


/*OVERALL CHECK*/
for (n=0;n<PI.npars;n++){
if (PI.stepsize[n]>1){PI.stepsize[n]=PI.stepsize[n]/adapfac;}

if (PI.stepsize[n]<minstepsize){PI.stepsize[n]=PI.stepsize[n]*adapfac;}

if (PI.stepsize[n]<minstepsize){PI.stepsize[n]=minstepsize;}

/*printf("STEPSIZE = %8.6f (minstepsize = %8.6f, Niterations = %10i)\n",PI.stepsize[n],minstepsize,N.ITER);*/
}






return 0;

}





