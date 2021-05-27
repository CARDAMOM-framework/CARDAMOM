#pragma once
double LIU_An(double SRAD, double VPD, double TEMP, double vcmax25, double co2, double beta_factor, double g1){
//double LIU_An(double const *gpppars, double const *consts, double const *pars, double const beta){

//double SRAD = 12.*gpppars[7]; //Shortwave downward radiation
//double VPD = gpppars[11]/10.; //VPD
//double TEMP = 273.15 + (gpppars[1]+gpppars[2])/2.; //(Tmin + Tmax)/2
//double co2 = gpppars[4]; //co2
//double vcmax25 = pars[37]; 
//double g1 = pars[36]; 
//double beta_factor = beta;        

//Add any more parameters to function definition

//CONSTS
double t0C = 273.15;//C to kelvin
double Ephoton = 2.0e-25/500.0e-9;// Planck constant times speed of light (J.s*m.s-1) divided by light wavelength (m)
double NA = 6.02e23;// Avogadro's constant /mol
double R = 8.31e-3;//Gas constant, kJ/mol/K


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
double a1;
double a2;

double An;


PAR = SRAD/(Ephoton*NA)*1e6;

T_C = TEMP - t0C;

Kc = 300.*exp(0.074*(T_C - 25.));
Ko = 300.*exp(0.015*(T_C - 25.));
cp = 36.9 + 1.18*(T_C - 25.) + 0.36*pow((T_C - 25.), 2.);

Vcmax = vcmax25*exp(50.*(TEMP - 298.)/(298.*R*TEMP));
Jmax = Vcmax*exp(1.);

J = (0.3*PAR + Jmax - sqrt(pow(0.3*PAR + Jmax,2) - 4.*0.9*0.3*PAR*Jmax))/2/0.9;


medlyn_term = 1. + g1/sqrt(VPD);

ci = co2*(1. - 1./medlyn_term);


a1 = Vcmax*(ci - cp)/(ci + Kc*(1.+209./Ko));
a2 = J*(ci-cp)/(4.*(ci + 2.*cp));


An = max(0., fmin(a1*beta_factor,a2) - 0.015*Vcmax*beta_factor);


return An;
}
