#pragma once
/*mean matrix from double pointer routine*/
double mean_annual_pool(double *POOLS, int year, int pool, int nopools, double deltat){
/*inputs
 * POOLS: Pools double pointer, as output from DALEC
 * year: year for which to average (first year = 0)
 * pool: the specific pool 
 * nc
declarations*/
int c=0;
double meanpool=0;

/*deriving mean of pool p*/
int stday=floor(365.25*year/deltat + 0.5); // adding 0.5 ensures we are far from the exact edge and machine error can't change this
int enday=floor(365.25*(year+1)/deltat + 0.5);

for (c=stday;c<enday;c++){
    meanpool=meanpool+POOLS[c*nopools+pool]/(enday-stday);}
    /*returing meanpool value*/
return meanpool;}
