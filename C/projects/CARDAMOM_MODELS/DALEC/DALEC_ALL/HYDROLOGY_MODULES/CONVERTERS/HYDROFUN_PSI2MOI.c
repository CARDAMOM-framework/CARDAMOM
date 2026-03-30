#pragma once

/*
Convert potential (MPa) to volumetric soil moisture (m3/m3)
Inputs:
psi_eff - effective water potential (MPa) of the liquid-saturated state (i.e. corrected for ice volume)
psi_porosity - porosity potential (MPa)
b - retention parameter
*/

double HYDROFUN_PSI2MOI(double psi, double psi_porosity, double b){

double moi = pow(psi/psi_porosity,(-1.0/b));
 
return moi;
}