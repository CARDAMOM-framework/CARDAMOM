#pragma once
/* This innovation is undertaken and led by the University of Riverside team, including Holly P, Medha Yasa, Anuradha Garg and Saverio Perri,
/* with participation of A. Bloom, Eren Bilir and Matthew Worden in testing steps. 
/* [UCR please add any request for acknowledgment for anyone who uses this function, this would be nice until we can reference a publication or product with DOI]
/* Convert volumetric soil moisture (m3/m3) to potential (MPa)

double HYDROFUN_MOI2PSI_WITH_SALINITY(double moi, double psi_porosity, double b, double temp){
/*Inputs:
moi - Soil moisture (units: m3/m3)
psi_porosity - porosity potential (units: MPa)
b - retention parameter
temp - soil water temperature (units: K)
*/
 
double psi=psi_porosity*(pow((1/moi),b));
/*UCR team: you can propose code in line below; I'm setting this to zero for now  to get the ball rolling*/
/*Also defining some constants which you can populate*/
double salinity = 0.0;/*use this notation to define all your variables*/
double psi_pi = - 0.0;

/*summing two psi terms*/
double psi_total = psi + psi_pi;

return psi;
}

