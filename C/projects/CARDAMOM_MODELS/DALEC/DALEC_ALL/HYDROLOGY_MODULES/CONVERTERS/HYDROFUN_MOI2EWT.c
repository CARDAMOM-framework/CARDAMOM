#pragma once

/* 
Convert volumetric soil moisture (m3/m3) to equivalent water thickness (mm or kg/m2)
Inputs:
moi - volumetric soil moisture (m3/m3)
p - layer porosity (1)
z - layer thickness (m)
*/

double HYDROFUN_MOI2EWT(double moi, double p, double z){

double ewt=moi*1000*p*z;

return ewt;
}