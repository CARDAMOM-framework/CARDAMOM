#pragma once
#include "../auxi_fun/cardamom_random.h"
double randn(){

double pi=3.141592653589793;
double r1=(double)cardarand()/(double)CARDAMOM_RAND_MAX;
double r2=(double)cardarand()/(double)CARDAMOM_RAND_MAX;


double rn=sqrt(-2*log(r1)) * cos(2*pi*r2);

return rn;

}

