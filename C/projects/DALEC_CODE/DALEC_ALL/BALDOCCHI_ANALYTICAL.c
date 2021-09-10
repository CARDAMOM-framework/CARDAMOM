#pragma once
#include "./ANALYTICALSOLVER_BALDOCCHI.c"

double* BALDOCCHI_ANALYTICAL(double const *met_list, double const *var_list)
{

	/**************** OUTPUTS *****************/
	/* GPP based on a polynomial solution of Ball-Berry-Farquhar */
	/* ET based on the stomatal conductance and VPD */
	
	
    /****************** INPUTS *****************/
    /* met_list[0] = mintemp [C] */
    /* met_list[1] = maxtemp [C] */
    /* met_list[2] = co2 [ppm] */
    /* met_list[3] = rad [MJ/day/m^2]*/
    /* met_list[4] = VPD [hPa] */
    /* met_list[5] = DOY [day] */
    

    /* var_list[0] = m  [unitless] */
    /* var_list[1] = b  [mol m^-2 s^-1] */
    /* var_list[2] = gb, CO2 [mol m^-2 s^-1] */
    /* var_list[3] = Jmax25 [umol m^-2 s^-1] */
    /* var_list[4] = Vmax25 [umol m^-2 s^-1] */
    /* var_list[5] = LAI [m^2/m^2] */
    /* var_list[6] = k [unitless] */
    /* var_list[7] = c1 [unused] */
    /* var_list[8] = c2 [unused] */
    /* var_list[9] = LAT [degree] */
    /******************************************/
    
	/* Note: Instabilities present if 'm' is <5 or Jmax25/Vmax25 < 0.8 (and vis versa)*/
	/* Note: Additional instabilities for very high values of VPD not generally seen on Earth*/    
    
    /* Calculate day length (dayl) in hours */
    double mult,dayl,dec,pi_here,doy,lat;
    doy = (double)met_list[5];
    lat = (double)var_list[9];
    pi_here = (double)3.14;
    dec=(double)-23.4*cos((360.*(doy+10.)/365.)*pi_here/180.)*pi_here/180.;
    mult=(double)tan(lat*pi_here/180)*tan(dec);
    if (mult>=1){
        dayl=24.;}
    else if(mult<=-1)
        dayl=0.001;
    else{
        dayl=(double)24.*acos(-mult) / pi_here;}



    /*initialize intermediate variables*/
    double Oi, psfc, T, RH, tau, gamma, Kc, Ko;
    double cold_inhibition, heat_inhibition, Vm;
    double PAR, APAR, Vmax, Jmax, a, b, c, J1, J2, J, Rd;
    double GPP, GPP_root1, GPP_root2, GPP_root3;
    double aj, dj, ej, bj, ac, dc, ec, bc;
    double par_frac, Wm2_umol, second_per_day, parts_ppm, gram_per_mol_C, gram_per_mol_H20,gram_per_kg, diffusionH2OtoCO2;
    double tset_cold, tset_hot;


    /* conversion constants */
    par_frac = (double)0.45; /*fraction of insolation that is PAR - Mavi & Tupper (2004)*/
    Wm2_umol = (double)4.56; /*conversion of solar radiation (W/m2) to umol/s/m2 photosynthetically active radiation*/
    second_per_day = (double)dayl*3600.; /* seconds per day length of day in hours and seconds per hour */
    parts_ppm=(double)1e6; /* parts to ppm*/
    gram_per_mol_C = (double)12.0107; /* grams per mol Carbon */
    gram_per_mol_H20 = (double)18.01528; /* grams per mol H2O */
    gram_per_kg = (double)1000.; /* grams per kilogram */
    diffusionH2OtoCO2 = (double)1.6; /* ratio of diffusion H2O to CO2 */
	
    /*calculate pressure terms in parts per rather than pascal*/
    /*set up so that all values are in ppm to match Vmax, Jmax, and APAR*/
    psfc = (double)101325; /* [Pa] */
    
    Oi=(double)0.209*parts_ppm; /*oxygen at surface umol/mol to match Vmax, Jmax*/
    T=(double)met_list[1];/*max temperature*/ /*replaced: (double)0.5*(met_list[1]+met_list[0]);*/ /* mean temperature */
    RH=(double)1.-met_list[4]/(6.1094*exp( (17.625*T) / (T+243.04) )); /*relative humidity, <1*/

    tau=(double)2600.*pow(0.57,((T-25.)/10.));
    gamma=(double)Oi/(2.*tau); /*co2 compensation point in ppm, umol/mol to match Vmax, Jmax*/

	
    Kc=(double)30.*pow(2.1,((T-25.)/10.))/psfc*parts_ppm; /*michaelis constant for carboxylation in ppm, umol/mol to match Vmax, Jmax*/
    Ko=(double)30000.*pow(1.2,((T-25.)/10.))/psfc*parts_ppm; /*michaelis constant for oxidation in ppm, umol/mol to match Vmax, Jmax*/

    /*compute temperature-adjusted carboxylation rate, apply temperature limitations to Vmax25, Jmax25*/
    tset_cold = (double)10.; /*temperature for cold limitation [C], could be optimized*/
    tset_hot = (double)40.; /*temperature for hot limitation [C], could be optimized*/
    cold_inhibition=(double)1+exp(0.25*(tset_cold-T));
    heat_inhibition=(double)1+exp(0.4*(T-tset_hot));
    Vmax=(double)var_list[4]*pow(2.1,((T-25.)/10.))/(cold_inhibition*heat_inhibition);
    Jmax=(double)var_list[3]*pow(2.1,((T-25.)/10.))/(cold_inhibition*heat_inhibition);

    /*convert insolation (full shortwave from meteorology) to PAR absorbed by the canopy*/
    PAR = (double)met_list[3]*1e6/second_per_day*Wm2_umol*par_frac; /* convert insolation from MJ m^-2 d^-1 to umol m^-2 s^-1 of PAR, units for analytical setup*/
    
    /* Controlled by LAI and k, k = 0.6 picked from literature, could be optimized */
    APAR = (double)PAR*(1-exp(-var_list[6]*var_list[5])); /* calculate absorbed PAR as a exponential absorption in the canopy */
    
    /*solve quadratic to get J value*/
    a=(double)0.7;
    b=(double)-(Jmax+0.385*APAR);
    c=(double)0.385*Jmax*APAR;
    J1=(double)(-b+pow((pow(b,2.)-4*a*c),0.5)) / (2*a);
    J2=(double)(-b-pow((pow(b,2.)-4*a*c),0.5)) / (2*a);
    J = (J1<=J2) ? J1 : J2;

    /*compute temperature-adjusted leaf respiration*/
    Rd=(double)0.015*Vmax*pow(2.4,((T-25.)/10.)) / (1.+exp((1.3*(T-55.))));

    /*set the a,d,e,b parameters for wj*/
    aj=(double)J;
    dj=(double)gamma;
    ej=(double)4.5;
    bj=(double)10.5*gamma;

    /*set the a,d,e,b parameters for wc*/
    ac=(double)Vmax;
    dc=(double)gamma;
    ec=(double)1.;
    bc=(double)Kc*(1.+Oi/Ko);

	
    double wj_adeb_list[] = {aj,dj,ej,bj};
    double wc_adeb_list[] = {ac,dc,ec,bc};
	
    /**************************/
    /* calculate GPP in polynomial solver */
    double ROOT_CALC, GPP_leaf_now;
    ROOT_CALC = 1.; /* primary root solution */
	
    GPP_leaf_now = ANALYTICALSOLVER_BALDOCCHI(met_list, var_list, wc_adeb_list, wj_adeb_list, Rd, RH, ROOT_CALC);
	
    /**************************/
    /* check dGPP/dCa */
	
    double GPP_Ca_high, dGPPdCa;
	
    /* For screening: calculate GPP with higher value of Ca, + 60 ppm */
    double met_list_high[] = {met_list[0],met_list[1],met_list[2]+60.,met_list[3],met_list[4]}; /* Switch in high value atmospheric CO2 */
    GPP_Ca_high = ANALYTICALSOLVER_BALDOCCHI(met_list_high, var_list, wc_adeb_list, wj_adeb_list, Rd, RH, ROOT_CALC);
	
    /* Response of GPP solution to CO2 */
    dGPPdCa = GPP_Ca_high - GPP_leaf_now;
	


    /***************************************/
    /* Screen for bad behavior of GPP, negative values or negative sensitivity to CO2 slope*/
	
    if ((GPP_leaf_now > 0.0001) & (dGPPdCa > 0.)){
    GPP = GPP_leaf_now;
    /*printf("Root1: %f\n",GPP);*/
    } else {
	
	/* Do not allow negative values of GPP */
	GPP = (double)0.0001;
	
	}
		
	/*printf("Good after GPP leaf level");*/

	/************************************************/
	/* Calculate leaf level stomatal conductance for water */
	double Cs, gs_H20;
    
    /* Calculate the CO2 at the leaf surface */
    Cs=(double)met_list[2] - GPP/var_list[2]; /*Cs is in umol mol^-1*/
    
    /* as a function of m, GPP, RH, and CO2 at leaf + intercept, calc stomatal conductance H20*/
    gs_H20 = (double)diffusionH2OtoCO2 * var_list[0]*GPP*RH/Cs+var_list[1]; /*gs is in mol m^-2 s^-1 of water*/


    /***********************************************/
    /*SCALE TO CANOPY LEVEL*/

    double GPP_canopy, GPP_canopy_gC, lai_scaling;
    
    /* scaling based on big leaf theory and LAI */
    lai_scaling = (double)(1-exp(-var_list[6]*var_list[5]))/var_list[6]; /* unitless or m2/m2 */
    
    /* Scale leaf level GPP to canopy level using a big leaf exponential approximation*/
    GPP_canopy=(double)GPP * lai_scaling; /* umol m^-2 s^-1 */

	/* convert umol of GPP at canopy scale to gC/day/m2 */
	/* Note: could apply the day length here in place of seconds per day*/
	GPP_canopy_gC = (double)GPP_canopy*gram_per_mol_C*second_per_day/1e6; /*1.03775, convert from umol m^-2 s^-1 to gC m^-2 d^-1*/

   
    /***********************************************/
    /*CALCULATE Canopy level stomatal conductance for water*/
	double Gs_canopy_H20, Gs_canopy_kgH20;
    
    /* Canopy scaling */
    Gs_canopy_H20 = (double)gs_H20 * lai_scaling;
    
    /* unit conversion mol H2O to kgH2O */
    /* Note: could apply the day length here in place of seconds per day*/
    Gs_canopy_kgH20 = (double)Gs_canopy_H20*gram_per_mol_H20*second_per_day/gram_per_kg; //units of kg H2O m^-2 day^-1
    
    /* Calculate canopy evapotranspiration */
	
    double ET_canopy_kgH20, psfc_hpa;
    psfc_hpa = psfc/100.; /* surface pressure in hPa to match VPD units */
	
    ET_canopy_kgH20 = (double)(met_list[4]/psfc_hpa)*Gs_canopy_kgH20;


    /***********************************************/
    /*RETURN AN ARRAY*/

    

    //iWUE=(double)GPP_scaled/(Gs_canopy*18.01528*86400./1000.); /*iWUE is in gC / kgH2O

    static double return_arr[2]; /*need static variable to be able to return array?*/
    return_arr[0] = GPP_canopy_gC;
    //return_arr[1]=iWUE;
    return_arr[1] = ET_canopy_kgH20;

  return return_arr;
}
