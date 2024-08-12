#pragma once
#include <math.h>
#include "../../../math_fun/randn.c"

double STEP_ADEMCMC(double *PARS, double *pars_new, PARAMETER_INFO PI, int C,int NC, double *gratio){
/*DE_STEP takes a differential evolution style step*/

/*FIXEDPARS*/
/*ones and zeros depending on whether parameters are kept fixed*/
/*CONTINUE FROM HERE*/
int n,fp,C1=-1,c2=-1;
double rn;


/*STep 1. Find random sample other than current chain*/

C1=C;
while (C1==C){C1=ceil((double)random()*NC/(RAND_MAX))-1;};

/*Step 2. FInd current chain, reference chain, and step between them*/
/*allocating memory for three normalized parameter vectors*/
double *cpar=calloc(PI.npars,sizeof(double));
double *rpar=calloc(PI.npars,sizeof(double));
double *npar=calloc(PI.npars,sizeof(double));
double *step=calloc(PI.npars,sizeof(double));
double *gamma=calloc(PI.npars,sizeof(double));
/*using "*pars" to store sequential outputs*/
for (n=0;n<PI.npars;n++){
cpar[n]=par2nor(PARS[C*PI.npars+n],PI.parmin[n],PI.parmax[n]);
rpar[n]=par2nor(PARS[C1*PI.npars+n],PI.parmin[n],PI.parmax[n]);
step[n] = cpar[n] - rpar[n];}


//printf("C = %i, C1 = %i\n",C,C1);


//printf("cpar\n");
//for (n=0;n<PI.npars;n++){printf("%4.4f ",cpar[n]);};printf("\n");
//printf("rpar\n");
//for (n=0;n<PI.npars;n++){printf("%4.4f ",rpar[n]);};printf("\n");
//printf("step\n");
//for (n=0;n<PI.npars;n++){printf("%4.4f ",step[n]);};printf("\n");




/*Step 3. Create step*/
double Dccoff=0.0001;
double Dsig=1;
double k = (double) PI.npars;
//Math for step variance covariance
double a=Dsig;
double b=(1-Dccoff)*Dsig;
double Dimo_a = 	-b/( a*a  - (k-1)* b*b  + (k-2)*b *a  ) ;
double Dimd_a =  (1- (k-1)*b*Dimo_a )/a ;


double varline=sqrt((1-Dccoff)*Dsig)*randn();
double varrand=sqrt(Dccoff*Dsig);

for (n=0;n<PI.npars;n++){gamma[n] = varline+randn()*varrand;}


//printf("varline = %4.4f, varrand = %4.4f\n",varline, varrand);
//printf("gamma\n");
//for (n=0;n<PI.npars;n++){printf("%8.8f ",gamma[n]);}
//printf("\n");





int withinlim=1;

for (n=0;n<PI.npars;n++){
npar[n] = rpar[n] + step[n]*gamma[n];
 if (npar[n]<0 | npar[n]>1){withinlim=0;}
}

gratio[0]=0;
/*
printf("a = %4.4f, b =%4.4f, Dsig = %4.4f, Dccoff = %4.4f",a,b,Dsig,Dccoff);
printf("\n");
printf("Dimo_a = %4.4f, Dimd_a = %4.4f\n",Dimo_a,Dimd_a);
printf("varline = %4.4f, varrand = %4.4f\n",varline, varrand);
*/


/*Step 4. Transform to parameter space*/
if (withinlim==1){

for (n=0;n<PI.npars;n++){pars_new[n]=nor2par(npar[n],PI.parmin[n],PI.parmax[n]);}



/*step 5. Probability of current2new*/
double gc2n,gn2c,igamma;
double sumgamma=0,isumgamma=0;
double sumgamma2=0,isumgamma2=0;
double prodgamma=1;

for (n=0;n<PI.npars;n++){
sumgamma+= gamma[n];
sumgamma2+= pow(gamma[n],2);
prodgamma*=gamma[n];
igamma = 1/gamma[n];
isumgamma+=igamma;
isumgamma2+=pow(igamma,2);
}

/*
printf(" k = %2.4f\n",k);
printf("sumgamma = %4.4f\n",sumgamma);
printf("sumgamma2 = %4.4f\n",sumgamma2);
printf("isumgamma = %4.4f\n",isumgamma);
printf("isumgamma2 = %4.4f\n",isumgamma2);
printf("prodgamma =%4.4f\n",prodgamma);
*/


gc2n=-0.5*(sumgamma*sumgamma*Dimo_a + sumgamma2*(Dimd_a-Dimo_a));

gn2c=-0.5*log(prodgamma*prodgamma) - 0.5*(isumgamma*isumgamma*Dimo_a + isumgamma2*(Dimd_a-Dimo_a));


gratio[0] = gn2c - gc2n;

/*
printf("gc2n = %4.4f\n",gc2n);
printf("gn2c = %4.4f\n",gn2c);
*/
//printf("gratio = %4.4f\n",gratio[0]);




/*Step 6. Probability of new2current*/

}

free(rpar);
free(cpar);
free(npar);
free(step);
free(gamma);
return withinlim;

}


