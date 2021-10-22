#pragma once
/* This join-co2-ch4 respiration model, originally described in
Ma et al 2021 in prep
*/

//ch4pars[7]={pars[P.PAW_fs],pars[P.S_fv],pars[P.thetas_opt],pars[P.fwc],pars[P.r_ch4],pars[P.Q10ch4],pars[P.q10rhco2]};

//in the brackets are input fed from main module, use pointer to a double if output is an array
double *JCR(double const *ch4pars, double T2M_MIN, double T2M_MAX, double PAW)
{
	/* input array already defined above,
  define names for ch4pars elements to be convenient in the JCR module*/
  double PAW_fs,S_fv,thetas_opt,fwc,r_ch4,Q10ch4,Q10rhco2,meantemp; /*jc*/
	// output array and corresponding elements
  static double jcr_o[5];
  double thetas,fT,fV,fW,fCH4; /*jc*/
  // the rest in JCR
  double fW1,fW2,fT_ch4,theta_ae;

	/*-----------------------------------------------------------------------*/
	/*jc get env. scalers ready*/
    /*Plant available water when the soil is fully saturated*/
    PAW_fs=ch4pars[1];
    /*Apprximate for soil mointure percentage = PAW/PAW_fs*/
    thetas = PAW/PAW_fs;
    jcr_o[0] = thetas;
    /*tunable scaler describing the shape of the curve: PAW/PAW_fs against fractional volume of aerobic respiratio*/
    S_fv=ch4pars[2];
    /*optimum water scaler (fW), where the maximum respiration is achieved without limitation from water
    it is the highest point in the function of PAW/PAW_fs against water scaler (fW)*/
    thetas_opt=ch4pars[3];
    /*the lowest point of water scaler when the soil is fully saturated,
     in the function of PAW/PAW_fs against water scaler (fW)*/
    fwc=ch4pars[4];
    /*potential ratio of CO2 converted to CH4*/
    r_ch4=ch4pars[5];
    /*Q10 of methane production*/
    Q10ch4=ch4pars[6];
    Q10rhco2=ch4pars[7];
    meantemp=ch4pars[8];
    /* fT Temperature factor on respiration */
    // fT = fmin(1,exp(pars[9]*(0.5*(DATA.MET[m+2]+DATA.MET[m+1])-meantemp)));
    //arrhenius method
    //fT = exp(pars[9]*(0.5*(DATA.MET[m+2]+DATA.MET[m+1])-meantemp)); 
    //Q10 method
    fT = pow(Q10rhco2,(0.5*(T2M_MIN+T2M_MAX)-meantemp)/10); 
    jcr_o[1] = fT;
    //fT = pow(pars[9],(0.5*(DATA.MET[m+2]+DATA.MET[m+1])-meantemp)/10);
    /* fV Volumetric factor seperating aerobic and anaerobic respiration */
    /* statistically fitting the fV curves (S1,S2,S3 schemes) with total soil moisture (PAW/PAW_fs) */
    fV = fmax(0,(1-pow(thetas,S_fv)));
    jcr_o[2] = fV;
    /* fW soil moisture scaler is a function of theta_ae*/
    theta_ae = ((thetas-1)/fV +1);
    fW1 = 1/thetas_opt*theta_ae;
    fW2 = ((1-fwc)/(thetas_opt-1)*theta_ae + (fwc-(1-fwc)/(thetas_opt-1)));
    if (theta_ae>=0 && theta_ae < thetas_opt){
      fW = fW1;}
    else if(theta_ae>=thetas_opt && theta_ae<=1){
      fW = fW2;}
    else{
      fW = 0;}
	  jcr_o[3] = fW;
	/* fCH4*/
    // fT_ch4 = exp(Q10ch4*(0.5*(DATA.MET[m+2]+DATA.MET[m+1])-meantemp));
    fT_ch4 = pow(Q10ch4,(0.5*(T2M_MIN+T2M_MAX)-meantemp)/10);
    fCH4 = fmin(r_ch4 * fT_ch4,1);
    // fCH4 = r_ch4 * fT_ch4;
    /* fmin() won't be needed once CH4 data is assimilated, as CH4 portion of CO2 should be very small,
    manually put the upper limit 1 here mainly based on the mass balance, but notice that DIC is not
    considered to contribute when the current timestep Rh is depeleted by CH4. I think it's good for
    now. Can think about DIC pool meanwhile */
    jcr_o[4] = fCH4;
/*-----------------------------------------------------------------------*/


return jcr_o;
}

