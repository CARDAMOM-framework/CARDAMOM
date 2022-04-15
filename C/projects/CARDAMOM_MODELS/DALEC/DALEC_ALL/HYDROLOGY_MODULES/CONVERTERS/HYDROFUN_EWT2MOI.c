#pragma once

/* 
Convert equivalent water thickness (mm or kg/m2) to volumetric soil moisture (m3/m3)
Inputs:
ewt - equivalent water thickness (mm)
p - layer porosity (1)
z - layer thickness (m)
*/

double HYDROFUN_EWT2MOI(double ewt, double p, double z){

double moi=ewt/(1000*p*z);

return moi;
}