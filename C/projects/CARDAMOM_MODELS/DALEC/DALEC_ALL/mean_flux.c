#pragma once


/*mean matrix from double pointer routine*/
double mean_flux(double *FA,int f,int nc, int nofluxes){
/*declarations*/
int c; 
double meanflux=0;
/*deriving mean of flux p*/
for (c=0;c<nc;c++){
meanflux=meanflux+FA[c*nofluxes+f]/(double)nc;
}
/*returing meanflux value*/
return meanflux;}






