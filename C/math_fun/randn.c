#pragma once
#include "../auxi_fun/cardamom_random.h"
double randn(){

double pi=3.141592653589793;
double r1=cardaurand();
double r2=cardaurand();


double rn=sqrt(-2*log(r1)) * cos(2*pi*r2);

return rn;

}

