#pragma once
/* Functions to support the Leaf Phenology Model of Knorr et al. (2010, doi: 10.1029/2009JG001119)*/
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
