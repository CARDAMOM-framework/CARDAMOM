#include <stdio.h>
//#include "INTERNAL_ENERGY_PER_H2O_UNIT_MASS.c"  

#define DGCM_PI 3.1415927 //pi 
#define DGCM_T3 273.16 //Triple point temp, K
#define DGCM_TK0C 273.15 //Kelvin at 0C
// Specific heat of ice 
#define DGCM_SPECIFIC_HEAT_ICE 2093.// 2093 J kg?1 K
#define DGCM_SPECIFIC_HEAT_WATER 4186.// 4186 J kg?1 K
//Latent heat of vaporization
#define DGCM_LATENT_HEAT_VAPORIZATION 2.501e6; // J kg-1 
#define DGCM_LATENT_HEAT_FUSION_3 3.34e5
#define DGCM_T_LIQUID_H2O_ZERO_ENERGY 56.79//Zero-energy temperature of super-cooled liquid water 56.79 K
#define DGCM_SEC_DAY (24*60*60)//Seconds per day

int main(){

//Minimnum temp
//Energy for 1m dry soil at -50C

//1mm H2O at -50C
    double LF, TEMP, U;

    LF = 0;
    TEMP=-50+273.15;
        U=(1 - LF) * DGCM_SPECIFIC_HEAT_ICE* TEMP + LF * DGCM_SPECIFIC_HEAT_WATER* (TEMP - DGCM_T_LIQUID_H2O_ZERO_ENERGY);
//1mm H2O at + 50C
printf("1mm H2O LF = 0 @ -50C = %2.2e\n",U); 



    LF = 0;
    TEMP=0+273.15;
        U=(1 - LF) * DGCM_SPECIFIC_HEAT_ICE* TEMP + LF * DGCM_SPECIFIC_HEAT_WATER* (TEMP - DGCM_T_LIQUID_H2O_ZERO_ENERGY);

printf("1mm H2O LF = 0 @ +0C = %2.2e\n",U);



    LF = 1;
    TEMP=0+273.15;
        U=(1 - LF) * DGCM_SPECIFIC_HEAT_ICE* TEMP + LF * DGCM_SPECIFIC_HEAT_WATER* (TEMP - DGCM_T_LIQUID_H2O_ZERO_ENERGY);

printf("1mm H2O LF = 1 @ +0C = %2.2e\n",U);



    LF = 1;
    TEMP=50+273.15;
        U=(1 - LF) * DGCM_SPECIFIC_HEAT_ICE* TEMP + LF * DGCM_SPECIFIC_HEAT_WATER* (TEMP - DGCM_T_LIQUID_H2O_ZERO_ENERGY);

printf("1mm H2O LF = 1 @ +50C = %2.2e\n",U); 



//Max IE for 1m soil =





printf("hello\n");

return 0;
}
