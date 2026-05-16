#pragma once
#include "math.h"
#include "./CONVERTERS/HYDROFUN_PSI2LIQ.c"
#include "./CONVERTERS/HYDROFUN_MOI2PSI.c"


/*
Calculate drainage for soil layer
Inputs:
sm - soil moisture (m3/m3)
Qexcess - Excess runoff factor
psi_field - field capacity potential (MPa)
psi_porosity - porosity potential (MPa)
b - retention parameter
lf - liquid fraction of H2O content 
*/

double DRAINAGE(double sm, double Qexcess, double psi_field, double psi_porosity, double b, double lf){

  // 1. check for excessive soil moisture: drain out sm>1 so that all sm values are <=1, and won't break psi_eff. 
 // (note that sm>1 only occurs transiently in between time steps and must be in a liquid/drainable state)
      double excess_drainage = 0.0;
      if (sm > 1.0) {
        excess_drainage = sm - 1.0;
        sm = 1.0;}

  // 2. Calculate current effective potential of the liquid-saturated state
    double psi_eff = HYDROFUN_MOI2PSI(sm, psi_porosity, b, lf);

  // 3. Check for excessive soil moisture: 
  if (psi_eff <= psi_field) {
        return excess_drainage; // Soil is at or drier than field capacity; only excess drainage can leave.
    }

  // 4. Calculate drainable liquid volume: 
      // Calculate effective porosity (fraction of pore space that is ice-free)
    double ice_sat_tv = sm * (1.0 - lf);
    double effective_porosity = 1.0 - ice_sat_tv;
      // Current effective liquid saturation
    double sm_eff = HYDROFUN_PSI2LIQ(psi_eff, psi_porosity, b);
      // liquid saturation at field capacity
    double sm_field = HYDROFUN_PSI2LIQ(psi_field, psi_porosity, b);   

      // drainable water in units of effective saturation (zero if soil liquid is less than field capacity)
    double delta_sm = fmax(sm_eff - sm_field,0.0);
      // drainable water as volume
    double delta_sm_liq = delta_sm * effective_porosity;
  
  //Matlab line Massoud et al.  D= frac_Qexcess * (1- (PARS.psiporosity-min(max(STATES.PSI(t,:),PARS.psifield),PARS.psiporosity))./(PARS.psiporosity-PARS.psifield)).*(SMdelta);
// calculate drainage (equals zero if change in soil moisture is zero)
double drainage = excess_drainage + delta_sm_liq * Qexcess * (1.0 - (psi_porosity - fmin(fmax(psi_eff,psi_field),psi_porosity))/(psi_porosity-psi_field));

return drainage;

}



