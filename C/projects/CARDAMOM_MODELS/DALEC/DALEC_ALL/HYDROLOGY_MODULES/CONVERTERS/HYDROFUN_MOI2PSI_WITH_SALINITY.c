#pragma once
/* This innovation is undertaken and led by the University of Riverside team, including Holly P, Medha Yasa, Anuradha Garg and Saverio Perri,
/* with participation of A. Bloom, Eren Bilir and Matthew Worden in testing steps. 
/* [UCR please add any request for acknowledgment for anyone who uses this function, this would be nice until we can reference a publication or product with DOI]
/* Estimate osmotic potential as a function of soil moisture

double HYDROFUN_MOI2PSI_WITH_SALINITY(double moi, double psi_porosity, double b, double temp, double m_NaCl, double n){
/*depth parameter can be added and changed if necessary (calculations are currently set for 1 m of soil profile) */
/*Inputs:
Matric Potential
moi - Soil moisture (units: m3/m3)
psi_porosity - porosity potential (units: MPa)
b - retention parameter
temp - soil water temperature (units: K)

Osmotic potential
psi_pi - Osmotic potential (units: MPa)
temp - Soil water temperature (units: K)
v_hoff - Vant Hoff coefficient
g_const - gas constant (units: J/mol·K)
m_NaCl - mass of sodium chloride per unit area (units: g/m2)
Mm_NaCl - molar mass of sodium chloride (units: g/mol)
n - soil porosity (units: m3/m3)
*/
 
double psi=psi_porosity*(pow((1/moi),b));
double v_hoff = 2.0;
double g_const = 8.314;
double Mm_NaCl = 58.44;
/* osmotic potential equation */
double psi_pi_interm = -(v_hoff * g_const * temp * m_NaCl)/(Mm_NaCl * moi * n * 1000000);
double psi_pi = fmax(psi_pi_interm, -30.12);
/* setting the minimum osmotic potential so it doesn’t drop below -30.12 MPa. This value corresponds to the osmotic potential at the maximum dissolvable NaCl concentration */ 
/* We divided the psi_pi formulation by 10^6 to convert osmotic potential unit from Pa to MPa */

/*summing two psi terms*/
double psi_total = psi + psi_pi;

return psi_total;
}

