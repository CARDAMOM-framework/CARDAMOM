#pragma once
int ADAPT_DEMCMC_STEP_SIZE(PARAMETER_INFO PI, COUNTERS N){


/*first adjusting all step sizes*/

int n;
double adapfac=1.1;
double minstepsize=0.000001;
/*if (minstepsize<MCO.minstepsize){minstepsize=MCO.minstepsize;}*/
double ACCRATE=(double)N.ACC/(double)N.ITER;

/*CONTINUE FROM HERE*/
/*MAKE THIS COMPATIBLE WITH STRUCTS AND CODE*/
/*printf("PI.stepsize = %2.2e, N.ACC = %i, N.ITER = %i, ACCRATE = %2.2f\n",PI.stepsize[0],N.ACC,N.ITER,ACCRATE);
*/

n=0;
if (ACCRATE<0.23){PI.stepsize[n]=PI.stepsize[n]/adapfac;}
else if(ACCRATE>0.44){PI.stepsize[n]=PI.stepsize[n]*adapfac;}



/*OVERALL CHECK*/
if (PI.stepsize[n]>1){PI.stepsize[n]=PI.stepsize[n]/adapfac;}
if (PI.stepsize[n]<minstepsize){PI.stepsize[n]=PI.stepsize[n]*adapfac;}
if (PI.stepsize[n]<minstepsize){PI.stepsize[n]=minstepsize;}



return 0;

}





