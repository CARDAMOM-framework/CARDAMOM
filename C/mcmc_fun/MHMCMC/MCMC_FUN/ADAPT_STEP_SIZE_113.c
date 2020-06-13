#pragma once
int ADAPT_STEP_SIZE_113(double *PARSALL, PARAMETER_INFO PI, COUNTERS *N, MCMC_OPTIONS MCO){



/*first adjusting all step sizes*/

int n;
double fac=1, scfac=(1-(double)N->ACC/(double)MCO.nOUT/2);
double adapfac=scfac*(MCO.nADAPT/100)/100+1;
/*ratio loc vs total*/
double acc2tot=(double)N->ACCLOC/(double)N->ACC;
/*
double adapfac=1.5;*/
/*double minstepsize=100./(double)N.ITER;
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

	/*need to remove this!!*/
	double norparstd,norparmean;
	double *norparvec=calloc(N->ACCLOC,sizeof(double));
	double parrange;
	int p, i;
	/*std of each parameter (for all recently N.ACCLOCepted parameters)*/
	for (p=0;p<PI.npars;p++){
		for (n=0;n<N->ACCLOC;n++){
			/*transforming parameters and storing in pointer*/
			norparvec[n]=par2nor(PARSALL[PI.npars*n+p],PI.parmin[p],PI.parmax[p]);}
			
		/*calculating variability*/
		norparstd=std(norparvec,N->ACCLOC);
		norparmean=mean(norparvec,N->ACCLOC);
		
		
		
		/*Storing in N.parstd*/
		N->parstdev[p]=((pow(N->parstdev[p],2)+ pow(N->parmean[p],2))*(1-acc2tot) 
+ (pow(norparstd,2)+ pow(norparmean,2))*acc2tot);		


		N->parmean[p]=N->parmean[p]*(1-acc2tot) + norparmean*acc2tot;

		N->parstdev[p]=sqrt(N->parstdev[p] - pow(N->parmean[p],2));
		


		}
	free(norparvec);

	}


/*OVERALL CHECK*/
/*printf("STEPSIZE = %8.6f (minstepsize = %8.6f, Niterations = %10i)\n",PI.stepsize[n],minstepsize,N.ITER);*/







return 0;

}





