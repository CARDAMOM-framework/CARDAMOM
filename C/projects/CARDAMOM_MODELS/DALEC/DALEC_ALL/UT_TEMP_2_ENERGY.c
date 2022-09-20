#include <stdio.h>
//#include "INTERNAL_ENERGY_PER_H2O_UNIT_MASS.c"  
int main(){

//Minimnum temp
//Energy for 1m dry soil at -50C


//double IE=INTERNAL_ENERGY_PER_H2O_UNIT_MASS(273.15-50, 0);
//Volumetric heat capacity
double vhc=1.3e6;

printf("1m soil, por = 0.8, SM=0, @ -50C = %2.2e",vhc*(273.15-50)); 

//Max IE for 1m soil =


//energy per kg h2o
double U = 4186*(273.15+50 - 56.79);

//energy per m 
printf("1m soil, por = 0.8, SM=1, @ +50C = %2.2e",0.8*1000*U + vhc*(273.15+50));


printf("hello\n");

return 0;
}
