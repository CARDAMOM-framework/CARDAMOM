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
int stday=floor(365.25*year/deltat);
int enday=floor(365.25*(year+1)/deltat);

fprintf(stderr, "DEBUG3: deltat = %f\n", deltat);
fprintf(stderr, "DEBUG3: year = %d\n", year);
fprintf(stderr, "DEBUG3: pool = %d\n", pool);
fprintf(stderr, "DEBUG3: nopools = %d\n", nopools);
fprintf(stderr, "DEBUG3: stday = %d\n", stday);
fprintf(stderr, "DEBUG3: endday = %d\n", enday);
fflush(stderr);
fflush(stdout);

for (c=stday;c<enday;c++){
meanpool=meanpool+POOLS[c*nopools+pool]/(enday-stday);}
/*returing meanpool value*/
return meanpool;}
