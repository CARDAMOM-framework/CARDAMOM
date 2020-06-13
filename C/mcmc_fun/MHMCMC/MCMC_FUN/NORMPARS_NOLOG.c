#pragma once
#include <math.h>
/*CONVERTING PARAMETERS TO 0-1 range*/
/*IN THIS ROUTINE THE RANGE IS NOT LOG-TRANSFORMED*/

double par2nor(double p, double mn, double mx){
return (p-mn)/(mx-mn);}


/*AND VISE VERSA*/


double nor2par(double p, double mn, double mx){
return mn+(mx-mn)*p;}
