#pragma once
int ADAPT_STEP_SIZE_106(double *PARSALL, PARAMETER_INFO PI, COUNTERS N, MCMC_OPTIONS MCO){



/*first adjusting all step sizes*/

int n;
double fac=(1-(double)N.ACC/(double)MCO.nOUT/2);
double  adapfac=fac*(MCO.nADAPT/100)/100+1;
/*
double adapfac=1.5;*/
/*double minstepsize=100./(double)N.ITER;
if (minstepsize>0.01){minstepsize=0.01;}
if (minstepsize<MCO.minstepsize){minstepsize=MCO.minstepsize;}
*/
double minstepsize=MCO.minstepsize;
N.ACCRATE=(double)N.ACCLOC/(double)MCO.nADAPT;

/*N.ACCRATE=(double)N.ACC/(double)N.ITER;*/

/*MAKE THIS COMPATIBLE WITH STRUCTS AND CODE*/


double sdsf, sdsfcp=1;
/*Dimension specific adjustments*/
if (N.ACCLOC>3){

	/*need to remove this!!*/
	double norparstd;
	double *norparvec=calloc(N.ACCLOC,sizeof(double));
	double parrange;
	int p, i;
	/*std of each parameter (for all recently N.ACCLOCepted parameters)*/
	for (p=0;p<PI.npars;p++){
		for (n=0;n<N.ACCLOC;n++){
			/*transforming parameters and storing in pointer*/
			norparvec[n]=par2nor(PARSALL[PI.npars*n+p],PI.parmin[p],PI.parmax[p]);}
			
		/*calculating variability*/
		norparstd=std(norparvec,N.ACCLOC);
		/*reducing if the stepsize is too small*/
		/*disabled step-dim-adapt*/
		sdsf=pow(norparstd/PI.stepsize[p],fac);
		/*Ensure scaling does not reduce size < minstepsize*/
		if (PI.stepsize[p]*sdsf<minstepsize){PI.stepsize[p]=minstepsize;
		sdsf=minstepsize/PI.stepsize[p];}
		PI.stepsize[p]=PI.stepsize[p]*sdsf;
		sdsfcp=sdsfcp*sdsf;

		}

	free(norparvec);

	}


if (N.ACCRATE<0.23){adapfac=1./adapfac;}
/*scaling by cumulative product of single-dim scalings*/
adapfac=adapfac/pow(sdsfcp,1./(double)PI.npars);
/*printf("adapfac = %2.2f\n",adapfac);
printf("sdsfcp = %2.2f\n",sdsfcp);
printf("adapfac*sdsfcp^1/N = %2.2f\n",adapfac*pow(sdsfcp,1./(double)PI.npars));
printf("N.ACCRATE = %2.2f\n",N.ACCRATE);
*/
for (n=0;n<PI.npars;n++){PI.stepsize[n]=PI.stepsize[n]*adapfac;}


/*OVERALL CHECK*/
for (n=0;n<PI.npars;n++){
/*printf("STEPSIZE = %8.6f (minstepsize = %8.6f, Niterations = %10i)\n",PI.stepsize[n],minstepsize,N.ITER);*/
}






return 0;

}





