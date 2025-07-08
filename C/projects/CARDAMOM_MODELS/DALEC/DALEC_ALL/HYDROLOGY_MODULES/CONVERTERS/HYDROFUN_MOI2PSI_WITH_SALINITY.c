#pragma once
/* This innovation is undertaken and led by the University of Riverside team, including Holly P, Medha Yasa, Anuradha Garg and Saverio Perri,
/* with participation of A. Bloom, Eren Bilir and Matthew Worden in testing steps. 
/* [UCR please add any request for acknowledgment for anyone who uses this function, this would be nice until we can reference a publication or product with DOI]
/* Convert volumetric soil moisture (m3/m3) to potential (MPa)
Inputs:
moi - Soil moisture (m3/m3)
psi_porosity - porosity potential (MPa)
b - retention parameter
*/

double HYDROFUN_MOI2PSI_WITH_SALINITY(double moi, double psi_porosity, double b){

double psi=psi_porosity*(pow((1/moi),b));
 
return psi;
}

