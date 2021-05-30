#pragma once

/*
Convert volumetric soil moisture (m3/m3) to conductivity (m/s)
Inputs:
sm - Soil moisture (m3/m3)
k0 - saturated hydraulic conductivity (m/s)
b - retention parameter
*/

double HYDROFUN_MOI2CON(double sm, double k0, double b){

double moi=k0*pow(sm,2*b+3);
 
return moi;
}