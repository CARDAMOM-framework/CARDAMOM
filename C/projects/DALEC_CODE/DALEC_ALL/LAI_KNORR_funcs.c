#pragma once
/* Functions to support the Leaf Phenology Model of Knorr et al. (2010, doi: 10.1029/2009JG001119)*/
//
// Minimum function with quadratic transition (smoothed) between two values. Example:
// x  = (double)0.1;
// y  = (double)1e-6;
// x0 = 5e-3
// z = MinimumQuadraticSmooth(x, y, x0)
/* x  = first value */
/* y  = second value */
/* eta = degree of smoothing */
double MinQuadraticSmooth(double const x, double const y, double const eta)
{
  double z, mins;

  z    = pow(x+y,2) - 4.0 * eta * x * y;
  z    = fmax(z,1e-18);
  mins = (x + y - sqrt(z)) / (2.0*eta);
  return mins;
}


// Maximum function with exponential transition (smoothed) between two values. Example:
/* x  = first value */
/* y  = second value */
/* x0 = degree of smoothing */
double MaxExponentialSmooth(double const x, double const y, double const x0)
{
  double maxx;

  if (x >= (y-x0)) {
    maxx = x + x0*exp(-(x-y)/x0 - 1.0);  // statements to be executed if the test expression is true
  }
  else {
    maxx = y;  // statements to be executed if the test expression is false
  }
  return maxx;
}

// Day length function - hours of sunlight per day //
double ComputeDaylightHours(double const *pars)
{
  /*double mult=0,dayl=0,dec=0;*/
  double mult,dayl,dec;
/*Notes: DALEC code passes the following *pars pointers:
constant pars terms
pars[0]=latitude;
pars[1]=day of year;
pars[2]=pi;
*/


  /*pars= &pars;  
 *   consts= &consts;
 *   */
  dec=(double)-23.4*cos((360.*(pars[1]+10.)/365.)*pars[2]/180.)*pars[2]/180.;
  mult=(double)tan(pars[0]*pars[2]/180)*tan(dec);
  if (mult>=1){ 
   dayl=24.;}  
  else if(mult<=-1)
  dayl=0.;
  else{
  dayl=(double)24.*acos(-mult) / pars[2];}

  return dayl;
}

