#pragma once

/*
Convert volumetric soil moisture (m3/m3) to potential (MPa)
Inputs:
sm - Soil moisture (m3/m3)
psi_porosity - porosity potential (MPa)
b - retention parameter
*/

double HYDROFUN_MOI2PSI(double sm, double psi_porosity, double b){

double psi=psi_porosity*(pow((1/sm),b));
 
return psi;
}