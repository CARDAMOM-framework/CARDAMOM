#pragma once
#include "./LIU_AN_ET_REFACTOR.c"

typedef struct {
    struct{        
        double SRAD;
        double VPD;
        double TEMP;
        double vcmax25;
        double co2;
        double beta_factor;
        double g1;
        double LAI;
        double ga;
        double VegK;
        double Tupp;
        double Tdown;
        double C3_frac;
        double clumping;
        double leaf_refl_par;
        double leaf_refl_nir;
        double maxPevap;
        double precip;        
    }IN;
    struct {
        double An;
        double transp;
        double evap;
    }OUT;
    
}LIU_AN_ET_STRUCT;

//double* LIU_AN_ET_REFACTOR(double const *met_list, double const *var_list)
//{
    
int LIU_AN_ET(LIU_AN_ET_STRUCT * A)
{


double SRAD=A->IN.SRAD;
double VPD=A->IN.VPD;
double TEMP=A->IN.TEMP; // Temperature (K)
double vcmax25=A->IN.vcmax25;
double co2=A->IN.co2;
double beta_factor=A->IN.beta_factor;
double g1=A->IN.g1;
double LAI=A->IN.LAI;
double ga=A->IN.ga;
double VegK=A->IN.VegK;
double Tupp=A->IN.Tupp; // Units are K
double Tdown=A->IN.Tdown; // Units are K
double C3_frac=A->IN.C3_frac;
double clumping=A->IN.clumping;
double leaf_refl_par=A->IN.leaf_refl_par;
double leaf_refl_nir=A->IN.leaf_refl_nir;
double maxPevap=A->IN.maxPevap;
double precip=A->IN.precip;

//CONSTS
double Ephoton = 2.0e-25/500.0e-9;// Planck constant times speed of light (J.s*m.s-1) divided by light wavelength (m)
double NA = 6.02e23;// Avogadro's constant /mol
double R = 8.31e-3;//Gas constant, kJ/mol/K
double lambda0 = 2.26e6;
double gammaV = 100*1005/(lambda0*0.622);


//DATA contains all met forcings

double PAR; //incident PAR (?) review
double T_C; // air temperature in C
double Kc; //Michaelis-Menten parameter temperature dependent
double Ko; //Michaelis-Menten parameter temperature dependent
double cp; //specific heat capacity of air

double Vcmax; //the maximum rate of carboxylation of Rubisco
double Jmax; //the maximum rate of electron transport 
double J;
double medlyn_term;
double ci;
//double C3_frac = 1.; //Fraction of C3 per gridcell
double a1;
double a2;

double An_C3;
double An_C4;

double An;

//Array of results with 3 positions for An, E, and Transpiration;
//static double r[3];


PAR = SRAD/(2*Ephoton*NA)*1e6;

//absorbed PAR assuming black canopy. 
//PAR = PAR*(1. - exp(-LAI*VegK));

PAR *= (1. - leaf_refl_par)*(1. - exp(-VegK*LAI*clumping));


T_C = TEMP - DGCM_TK0C;  // Convert temperature to degrees C


Kc = 300.*exp(0.074*(T_C - 25.));
Ko = 300.*exp(0.015*(T_C - 25.));
cp = 36.9 + 1.18*(T_C - 25.) + 0.36*pow((T_C - 25.), 2.);


//Vcmax = vcmax25*exp(50.*(TEMP - 298.)/(298.*R*TEMP));
double q_10 = 2.0;

Vcmax = vcmax25*pow(q_10,0.1*(T_C-25.))/((1 + exp(0.3*(T_C-(Tupp-DGCM_TK0C))))*(1 +exp(0.3*((Tdown-DGCM_TK0C)-T_C))));

Jmax = Vcmax*exp(1.);

J = (0.3*PAR + Jmax - sqrt(pow(0.3*PAR + Jmax,2) - 4.*0.9*0.3*PAR*Jmax))/2/0.9;


medlyn_term = 1. + g1/sqrt(VPD);

ci = co2*(1. - 1./medlyn_term);


//Two terms for C3 photosythesis
a1 = Vcmax*(ci - cp)/(ci + Kc*(1.+209./Ko));
a2 = J*(ci-cp)/(4.*(ci + 2.*cp));



An_C3 = fmax(0., fmin(a1*beta_factor,a2) - 0.015*Vcmax*beta_factor);

//Two terms for C4 photosynthesis
a1 = Vcmax;
a2 = J;

An_C4 = fmax(0., fmin(a1*beta_factor,a2) - 0.015*Vcmax*beta_factor);


//Total photosynthesis 
An = C3_frac*(An_C3) + (1. - C3_frac)*(An_C4);

//To scale from leaf to canopy, comment out the following line and uncomment the one after
//double canopy_scale = 1.;
double canopy_scale = (1. - exp(-VegK*LAI*clumping))/(VegK); 

//r[0] = An*canopy_scale*(12.e-6)*(24.*60.*60.); //from umolCO2m-2s-1 to gCm-2day-1
A->OUT.An = An*canopy_scale*(12.e-6)*(24.*60.*60.);
//##################Transpiration#################

double SRADg; 
double sV; // sV is the rate of change of saturated vapor pressure with air temperature in degC. 
double petVnum;
double petVnumB;
double gs; //stomatal conductance
double transp; // transpiration
double evap; // evaporation
double Psurf = 100.0; //Surface pressure in kPa
double VPD_kPa = VPD;//*Psurf; //100.0 kPa = 1000.0 hPa => Surface pressure
double evap_scale_factpr;

sV = 0.04145*exp(0.06088*T_C); 

SRADg = (1. - 0.5*(leaf_refl_par+leaf_refl_nir))*SRAD*exp(-VegK*LAI*clumping);

SRAD = (1. - 0.5*(leaf_refl_par+leaf_refl_nir))*SRAD;

petVnum = (sV*(SRAD-SRADg)+1.225*1000*VPD_kPa*ga)/lambda0*60*60;
petVnumB = 1.26*(sV*SRADg)/(sV+gammaV)/lambda0*60*60; //from mm.hr-1 
if(beta_factor > 0 && SRAD >0){

gs = 1.6*An/(co2-ci)*LAI*0.02405; 

//transp = petVnum/(sV+gammaV*(1+ga*(1/ga+1/gs)));
transp = petVnum/(sV+gammaV*(1+ga*(1/ga+1/gs)));

}
else{
transp = 0.0;
}

//r[1] = transp*24; //from mm.hr-1 to mm.day-1

A->OUT.transp = transp*24;



evap_scale_factpr = fmin(precip/maxPevap, 1.);

evap = petVnumB*evap_scale_factpr;

//evap = petVnumB;

//r[2] = evap*24; //from mm.hr-1 to mm.day-1

A->OUT.evap = evap*24;



//printf("SRADg = %f, VegK = %f, LAI = %f\n", (SRADg/SRAD), VegK, LAI);
//printf("transp = %f, evap = %f\n", r[1], r[2] );

return 0;
}
