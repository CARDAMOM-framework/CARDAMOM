#pragma once

/*
Convert potential (MPa) to volumetric soil moisture (m3/m3)
Inputs:
psi - potential (MPa)
psi_porosity - porosity potential (MPa)
b - retention parameter
*/

double HYDROFUN_PSI2MOI(double psi, double psi_porosity, double b){

double moi = pow(psi/psi_porosity,(-1/b));
 
return moi;
}