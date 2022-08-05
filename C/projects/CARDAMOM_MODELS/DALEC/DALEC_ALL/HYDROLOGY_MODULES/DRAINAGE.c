#pragma once
#include "math.h"
#include "./CONVERTERS/HYDROFUN_PSI2MOI.c"
#include "./CONVERTERS/HYDROFUN_MOI2PSI.c"


/*
Calculate drainage for soil layer
Inputs:
sm - soil moisture (m3/m3)
Qexcess - Excess runoff factor
psi_field - field capacity potential (MPa)
psi_porosity - porosity potential (MPa)
b - retention parameter
*/

double DRAINAGE(double sm, double Qexcess, double psi_field, double psi_porosity, double b){

// soil moisture at field capacity
double sm_field = HYDROFUN_PSI2MOI(psi_field,psi_porosity,b);

// check for excessive soil moisture
double excess_drainage = 0;
if (sm > 1) {
  excess_drainage = sm - 1;
  sm = 1;}

// change in soil moisture (zero if soil moisture is less than field capacity)
double delta_sm = fmax(sm - sm_field,0);

// potential of layer
double psi = HYDROFUN_MOI2PSI(sm,psi_porosity,b);
  
  //Matlab line Massoud et al.  D= frac_Qexcess * (1- (PARS.psiporosity-min(max(STATES.PSI(t,:),PARS.psifield),PARS.psiporosity))./(PARS.psiporosity-PARS.psifield)).*(SMdelta);
// calculate drainage (equals zero if change in soil moisture is zero)
double drainage = excess_drainage + delta_sm * Qexcess * (1 - (psi_porosity - fmin(fmax(psi,psi_field),psi_porosity))/(psi_porosity-psi_field));

return drainage;

}



