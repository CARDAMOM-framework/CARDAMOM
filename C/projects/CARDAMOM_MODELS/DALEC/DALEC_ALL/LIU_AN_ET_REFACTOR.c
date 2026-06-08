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
        double q10canopy;
        double q10canopyRd;
        double canopyRdsf;
        double NSC;
        double deltat;
    }IN;
    struct {
        double An;
        double Ag;
        double Rd;
        double transp;
        double evap;
        double LEAF_MORTALITY_FACTOR;
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

/* --- Physical and thermodynamic constants --- */
double Ephoton = 2.0e-25/500.0e-9; // Energy per photon at 500 nm: h*c/lambda (J); h=6.626e-34 J.s, c=3e8 m/s
double NA      = 6.02e23;           // Avogadro's constant (mol-1)
double R       = 8.31e-3;           // Universal gas constant (kJ mol-1 K-1)
double lambda0 = 2.26e6;            // Latent heat of vaporization of water (J kg-1)
double rho_air = 1.225;             // Dry air density at sea level (kg m-3)
double Cp_air  = 1005.;             // Specific heat of dry air at constant pressure (J kg-1 K-1)
double eps_mol = 0.622;             // Ratio of molar mass of water vapour to dry air: Mw/Md = 18.015/28.97 (-)
/* Psychrometric constant gamma = rho_air * Cp_air / (lambda0 * eps_mol) (Pa K-1);
   here expressed as a dimensionless coefficient for use with kPa and m s-1 units */
double gammaV  = 100.*Cp_air/(lambda0*eps_mol); // ~0.067 kPa K-1; Monteith & Unsworth 2008


/* --- FvCB photosynthesis state variables --- */
double PAR;          // Incident PAR at canopy top (umol photons m-2 s-1)
double T_C;          // Air temperature (deg C)
double Kc;           // Michaelis-Menten constant for CO2 (umol mol-1), temperature-dependent
double Ko;           // Michaelis-Menten constant for O2 (mmol mol-1), temperature-dependent
double cp;           // CO2 compensation point in absence of Rd, Gamma* (umol mol-1)
double Vcmax;        // Maximum rate of Rubisco carboxylation at T_C (umol CO2 m-2 s-1)
double Jmax;         // Maximum rate of electron transport at T_C (umol e- m-2 s-1)
double J;            // Actual electron transport rate (umol e- m-2 s-1)
double medlyn_term;  // Medlyn optimal stomatal model factor: 1 + g1/sqrt(VPD); Medlyn et al. 2011
double ci;           // Leaf intercellular CO2 concentration (umol mol-1)
//double C3_frac = 1.; //Fraction of C3 per gridcell
double a1;           // Rubisco-limited (co-limited) gross assimilation term (umol CO2 m-2 s-1)
double a2;           // Electron-transport-limited gross assimilation term (umol CO2 m-2 s-1)

double Ag_C3;        // Gross assimilation, C3 pathway (leaf-level, umol CO2 m-2 s-1)
double Ag_C4;        // Gross assimilation, C4 pathway (leaf-level, umol CO2 m-2 s-1)
double Ag;           // Blended gross assimilation (leaf-level, umol CO2 m-2 s-1)
double An;           // Net assimilation = Ag - Rd (leaf-level, umol CO2 m-2 s-1)

double Rd_C3;        // Dark respiration, C3 (umol CO2 m-2 s-1)
double Rd_C4;        // Dark respiration, C4 (umol CO2 m-2 s-1)
double Rd;           // Blended dark respiration (umol CO2 m-2 s-1)

/* Convert incoming shortwave to incident PAR.
   SRAD is total shortwave (W m-2); ~50% is PAR (400-700 nm); dividing by Ephoton*NA
   converts W m-2 (= J s-1 m-2) to mol photons m-2 s-1, then *1e6 -> umol photons m-2 s-1.
   The factor of 2 accounts for the ~50% PAR fraction of shortwave; Liu et al. myFun.py. */
PAR = SRAD/(2.*Ephoton*NA)*1e6;

/* --- Canopy radiation interception --- */
/* canopy_scale: integral of Beer's law profile int_0^LAI exp(-k*Omega*l) dl
   = (1 - exp(-k*Omega*LAI)) / (k*Omega);  denominator requires both k AND Omega (clumping);
   de Pury & Farquhar (1997) Plant Cell Environ. 20:537; Bonan (2019) Ch. 14 Eq. 14.6 */
double canopy_scale = (1. - exp(-VegK*LAI*clumping))/(VegK*clumping); // Beer's law integral /(k*Omega), not /(k); de Pury & Farquhar 1997; Bonan Ch. 14 Eq. 14.6

//printf(" \n Initial LAI %f \n ", LAI);

/* PAR_leaf: effective leaf-level absorbed PAR for the big-leaf model.
   Derived as APAR_canopy / canopy_scale = I_0*(1-rho)*k*Omega; this keeps
   all FvCB quantities (a1, a2) at leaf level so canopy_scale is applied only once at output.
   Bonan (2019) Ch. 14; Liu et al. myFun.py f_carbon() */
double PAR_leaf = PAR * (1. - leaf_refl_par) * VegK * clumping; // leaf-level PAR = APAR/canopy_scale = I_0*(1-rho)*k*Omega; Bonan Ch. 14


T_C = TEMP - DGCM_TK0C;  // Convert temperature to degrees C


/* Temperature-dependent Michaelis-Menten kinetics; Q10-type fits to Bernacchi et al. (2001) data.
   Kc ref = 300 umol mol-1, Ko ref = 300 mmol mol-1, both at 25 C. */
Kc = 300.*exp(0.074*(T_C - 25.)); // Michaelis constant for CO2 (umol mol-1)
Ko = 300.*exp(0.015*(T_C - 25.)); // Michaelis constant for O2 (mmol mol-1)
cp = 36.9 + 1.18*(T_C - 25.) + 0.036*pow((T_C - 25.), 2.); // CO2 compensation point Gamma* (umol mol-1); Bernacchi et al. 2001; Liu et al. myFun.py

/* --- Vcmax temperature response (Q10 formulation with upper/lower inhibition) ---
   Vcmax(T) = Vcmax25 * Q10^(0.1*(T-25)) / [(1+exp(0.3*(T-Tupp))) * (1+exp(0.3*(Tdown-T)))]
   Tupp, Tdown: upper/lower thermal inhibition thresholds (K); inverted by MCMC. */
double q_10 = A->IN.q10canopy;
double fT = pow(A->IN.q10canopyRd,(T_C-25.)/10.); // Q10 temperature scaling for Rd; reference T = 25 C

Vcmax = vcmax25*pow(q_10,0.1*(T_C-25.))/((1. + exp(0.3*(T_C-(Tupp-DGCM_TK0C))))*(1. + exp(0.3*((Tdown-DGCM_TK0C)-T_C))));

/* Jmax/Vcmax ~ e ≈ 2.72 at 25 C; Leuning (1997) J. Exp. Bot. 48:345
   (Wullschleger 1993 109-species dataset rescaled to 25 C: slope = 2.68, r2 = 0.87)
   Fixed ratio minimises MCMC parameter space; temperature response of Jmax tracks Vcmax. */
Jmax = Vcmax*exp(1.);

/* Electron transport rate J: non-rectangular hyperbola (Farquhar & Wong 1984).
   kai1 = 0.9 (curvature parameter), kai2 = 0.3 (quantum yield, mol e- per mol photon);
   Bonan (2019) Ch. 11 Eq. 11.33; Liu et al. myFun.py */
J = (0.3*PAR_leaf + Jmax - sqrt(pow(0.3*PAR_leaf + Jmax,2) - 4.*0.9*0.3*PAR_leaf*Jmax))/2./0.9; // PAR_leaf keeps a2 leaf-level (consistent with a1); single canopy_scale at output; Bonan Ch. 11

/* Medlyn et al. (2011) optimal stomatal conductance model: gs/g0 ~ 1 + g1/sqrt(VPD).
   ci derived analytically from gs = An/(co2-ci)*1.6, giving ci = co2*(1 - 1/medlyn_term).
   Medlyn et al. (2011) Glob. Change Biol. 17:2134 */
medlyn_term = 1. + g1/sqrt(VPD);

ci = co2*(1. - 1./medlyn_term);

    if (ci<cp){ci=cp;} // CLM 4.5: ci cannot drop below CO2 compensation point

/* --- C3 photosynthesis: FvCB co-limitation (Farquhar, von Caemmerer & Berry 1980) ---
   a1: Rubisco-limited rate; 209 umol mol-1 = ambient O2 concentration (Coa);
   a2: RuBP-regeneration (electron-transport) limited rate;
   beta_factor applied to a1 only: soil moisture suppresses Rubisco carboxylation
   (stomatal + non-stomatal limitation); light-limited a2 is not gated by stomata;
   Bonan (2019) Ch. 11 Eqs. 11.28-11.29 */
a1 = Vcmax*(ci - cp)/(ci + Kc*(1.+209./Ko)); // Rubisco-limited (umol CO2 m-2 s-1); 209 = O2 conc. in mmol/mol; Bonan Ch. 11 Eq. 11.28
// beta_factor applied to a1 only; a2 (light-limited) unconstrained by soil moisture
a2 = J*(ci-cp)/(4.*(ci + 2.*cp)); // RuBP-limited (umol CO2 m-2 s-1); Bonan Ch. 11 Eq. 11.29

Ag_C3 = fmin(a1*beta_factor, a2); // C3 gross assimilation: co-limited minimum
Rd_C3 = A->IN.canopyRdsf*vcmax25*fT; // C3 dark respiration = canopyRdsf * Vcmax25 * Q10(T)

/* --- C4 photosynthesis: simplified two-term co-limitation ---
   C4 plants have a CO2-concentrating mechanism (CCM), so Rubisco is not CO2-limited;
   a1 ~ Vcmax (enzyme-limited), a2 ~ J (light-limited).
   C3_frac is prescribed per pixel from remote-sensing products, not inverted by MCMC. */
a1 = Vcmax;
a2 = J;

Ag_C4 = fmin(a1*beta_factor, a2); // C4 gross assimilation
Rd_C4 = A->IN.canopyRdsf*vcmax25*fT; // C4 dark respiration

/* Fractional C3/C4 mix (C3_frac prescribed, not MCMC-inverted) */
Ag = C3_frac*(Ag_C3) + (1. - C3_frac)*(Ag_C4); // blended gross assimilation (leaf-level, umol CO2 m-2 s-1)
Rd = C3_frac*(Rd_C3) + (1. - C3_frac)*(Rd_C4); // blended dark respiration (leaf-level, umol CO2 m-2 s-1)

/* Potential canopy Rd (g C m-2 day-1):
   leaf-level Rd * canopy_scale (leaf->canopy) * 12e-6 (g C per umol CO2) * 86400 (s day-1) */
double Rd_daily_potential = Rd*canopy_scale*(12.e-6)*(24.*60.*60.);

/* Leaf mortality from NSC (non-structural carbohydrates) limitation.
   Exponential formulation: LEAF_MORTALITY_FACTOR -> 0 when NSC >> Rd*deltat (no stress),
   -> 1 when NSC -> 0 (full mortality); updated to exponential form 11/1/2023. */
if (Rd_daily_potential==0){
    A->OUT.LEAF_MORTALITY_FACTOR=0;
} else {
    A->OUT.LEAF_MORTALITY_FACTOR=(1./(exp(A->IN.NSC/(Rd_daily_potential * A->IN.deltat))));
}

/* Actual daily canopy Rd, scaled by leaf survival fraction */
A->OUT.Rd = Rd_daily_potential*(1. - A->OUT.LEAF_MORTALITY_FACTOR);

/* Scale leaf-level Rd by survival fraction before computing leaf-level An */
Rd = Rd*(1. - A->OUT.LEAF_MORTALITY_FACTOR);

/* Net assimilation (leaf-level); both Ag and Rd are leaf-level here.
   canopy_scale applied once below when writing to output. */
An = Ag - Rd;

/* Scale leaf -> canopy and convert units: umol CO2 m-2 s-1 -> g C m-2 day-1
   12e-6: molecular mass of C in g per umol CO2 (12 g mol-1 * 1e-6 mol umol-1)
   86400: seconds per day */
//double canopy_scale = 1.; // uncomment to disable canopy scaling (diagnostic)
A->OUT.Ag = Ag*canopy_scale*(12.e-6)*(24.*60.*60.);
A->OUT.An = An*canopy_scale*(12.e-6)*(24.*60.*60.);

/* ===================== Transpiration: Penman-Monteith (Monteith 1965) =====================
   Canopy transpiration E = [Delta*(Rn_veg) + rho_air*Cp*(VPD)*ga] / [lambda*(Delta + gamma*(1 + ga/gs))]
   where Delta = d(esat)/dT (kPa K-1), gamma = psychrometric constant (kPa K-1),
   ga = aerodynamic conductance (m s-1), gs = canopy stomatal conductance (m s-1).
   Reference: Monteith & Unsworth (2008) "Principles of Environmental Physics", Ch. 13;
              Liu et al. myFun.py PenmanMonteith(). */

double SRADg;           // Shortwave radiation reaching the ground surface (W m-2)
double sV;              // Slope of saturation vapour pressure curve, d(esat)/dT (kPa K-1)
double petVnum;         // PM numerator for canopy transpiration (W m-2 equivalent)
double petVnumB;        // PT numerator for ground evaporation (Priestley-Taylor 1972)
double gs;              // Canopy stomatal conductance (m s-1)
double transp;          // Canopy transpiration (mm hr-1)
double evap;            // Ground evaporation (mm hr-1)
double VPD_kPa = VPD;   // VPD already in kPa
double evap_scale_factpr;

/* Slope of saturation vapour pressure curve (kPa K-1); empirical fit to Magnus equation */
sV = 0.04145*exp(0.06088*T_C);

/* Canopy and ground radiation partitioning using Beer's law:
   SRADg = below-canopy shortwave (W m-2); (1-0.5*(rho_PAR+rho_NIR)) = broadband canopy absorptance */
SRADg = (1. - 0.5*(leaf_refl_par+leaf_refl_nir))*SRAD*exp(-VegK*LAI*clumping);
SRAD  = (1. - 0.5*(leaf_refl_par+leaf_refl_nir))*SRAD; // absorbed canopy shortwave (W m-2)

/* PM numerator for canopy transpiration (mm hr-1):
   numerator = [Delta*(Rn_canopy) + rho_air*Cp*VPD*ga] / lambda * 3600
   rho_air = 1.225 kg m-3; Cp_air = 1005 J kg-1 K-1; lambda0 = 2.26e6 J kg-1 */
petVnum = (sV*(SRAD-SRADg) + rho_air*Cp_air*VPD_kPa*ga)/lambda0*60.*60.; // mm hr-1

/* Priestley-Taylor ground evaporation (mm hr-1):
   alpha_PT = 1.26 (empirical coefficient; Priestley & Taylor 1972 Mon. Wea. Rev. 100:81);
   uses below-canopy radiation SRADg; assumes soil evaporation ~ equilibrium */
petVnumB = 1.26*(sV*SRADg)/(sV+gammaV)/lambda0*60.*60.; // mm hr-1; alpha_PT=1.26; Priestley & Taylor 1972

if(beta_factor > 0 && SRAD > 0){

    /* Canopy stomatal conductance to water vapour (m s-1):
       Based on leaf-level gs_CO2 = An/(co2-ci) (mol CO2 m-2 s-1 per umol mol-1);
       1.6: ratio of diffusivity of H2O to CO2 in air (dimensionless; Jones 1992 "Plants and Microclimate");
       LAI: leaf-to-canopy upscaling (m2 leaf m-2 ground);
       0.02405: molar volume of air at ~20 C = RT/P = 8.314*293/101325 = 0.02406 m3 mol-1,
                converts mol CO2 m-2 s-1 -> m s-1. */
    gs = fmax(0., 1.6*An/(co2-ci)*LAI*0.02405);

    /* Penman-Monteith transpiration (mm hr-1).
       Denominator: sV + gamma*(1 + ga/gs) = sV + gamma*ga*(1/ga + 1/gs); Monteith 1965.
       Liu et al. myFun.py PenmanMonteith() */
    transp = petVnum/(sV + gammaV*(ga*(1./ga + 1./gs)));

} else {
    transp = 0.0;
}

A->OUT.transp = transp*24.; // mm hr-1 -> mm day-1

/* Ground evaporation: scale by precipitation availability (evap <= precip/maxPevap) */
evap_scale_factpr = fmin(precip/maxPevap, 1.);
evap = petVnumB*evap_scale_factpr;

A->OUT.evap = evap*24.; // mm hr-1 -> mm day-1

return 0;
}
