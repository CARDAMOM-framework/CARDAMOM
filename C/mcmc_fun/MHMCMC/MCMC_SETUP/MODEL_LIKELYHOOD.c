#include <math.h>

double MODEL_LIKELYHOOD(DATA DATA, double *PARS){
/*This function is to be set by the user*/
/*Accepts parameter info, if unused then ignore*/


double P=0,D;
double unc=0.1;
double M;
int n;

/*this is my test model*/
for (n=0;n<DATA.nd;n++){
D=DATA.dri[n];
M=D*PARS[0]+sqrt(D)*PARS[1]+pow(D,2)*PARS[2]+PARS[3];
/*IMPORTANT - theis function should return the LOG likelihood*/
P+=-0.5*pow(M-DATA.obs[n],2)/(pow(unc,2));
}





return P;

}




