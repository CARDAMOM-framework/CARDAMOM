#pragma once

/*
Convert volumetric soil moisture (m3/m3) to conductivity (m/s)
Inputs:
moi - Soil moisture (m3/m3)
k0 - saturated hydraulic conductivity (m/s)
b - retention parameter
*/

double HYDROFUN_MOI2CON(double moi, double k0, double b){

double con=k0*pow(moi,2*b+3);
 
return con;
}