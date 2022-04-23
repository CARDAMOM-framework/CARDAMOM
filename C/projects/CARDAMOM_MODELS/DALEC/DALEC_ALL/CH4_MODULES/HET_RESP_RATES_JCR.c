#pragma once
/* This join-co2-ch4 respiration model, originally described in
Ma et al 2021 in prep
*/

//ch4pars[7]={pars[P.S_fv],pars[P.thetas_opt],pars[P.fwc],pars[P.r_ch4],pars[P.Q10ch4],pars[P.q10rhco2],meantemp};

//in the brackets are input fed from main module, use pointer to a double if output is an array




typedef struct{
    
    struct {
        double TEMP;
        double SM;
         double S_FV;
         double SM_OPT;
         double FWC;
        double  R_CH4;
        double  Q10CH4;
        double  Q10CO2;
    }IN;
            
            struct {    
    double*  aerobic_tr;//Aerobic turnover rate scalar
    double * anaerobic_tr;//Anaerobic turnover rate scalar
        double * anaerobic_co2_c_ratio;//CO2_C_ratio
        double * anaerobic_ch4_c_ratio;//CH4_C_ratio
    }OUT;
    
} HET_RESP_RATES_JCR_STRUCT;




int HET_RESP_RATES_JCR(HET_RESP_RATES_JCR_STRUCT * S)
{
	/* input array already defined above,
  define names for ch4pars elements to be convenient in the JCR module*/
  //double PAW_fs,S_fv,thetas_opt,fwc,r_ch4,Q10ch4,Q10rhco2,meantemp; /*jc*/
  double reftemp=25;
	// output array and corresponding elements
  static double jcr_o[4];
  //
  double fT,fV,fW,fCH4; /*jc*/
  // the rest in JCR
  double fW1,fW2,fT_ch4,theta_ae;
//double ch4pars[7]={pars[P.S_fv],pars[P.thetas_opt],pars[P.fwc],pars[P.r_ch4],pars[P.Q10ch4],pars[P.Q10rhco2],meantemp};

  //Soil moisture
    double thetas=S->IN.SM;
	/*-----------------------------------------------------------------------*/
	/*jc get env. scalers ready*/
    /*tunable scaler describing the shape of the curve: PAW/PAW_fs against fractional volume of aerobic respiratio*/
    double S_fv=S->IN.S_FV;
    /*optimum water scaler (fW), where the maximum respiration is achieved without limitation from water
    it is the highest point in the function of PAW/PAW_fs against water scaler (fW)*/
    double thetas_opt=S->IN.SM_OPT;
    /*the lowest point of water scaler when the soil is fully saturated,
     in the function of PAW/PAW_fs against water scaler (fW)*/
    double fwc=S->IN.FWC;
    /*potential ratio of CO2 converted to CH4*/
    double r_ch4=S->IN.R_CH4;
    /*Q10 of methane production*/
    double Q10ch4=S->IN.Q10CH4;
   double  Q10rhco2=S->IN.Q10CO2;
    /* fT Temperature factor on respiration */
    // fT = fmin(1,exp(pars[9]*(0.5*(DATA.MET[m+2]+DATA.MET[m+1])-meantemp)));
    //arrhenius method
    //fT = exp(pars[9]*(0.5*(DATA.MET[m+2]+DATA.MET[m+1])-meantemp)); 
    //Q10 method
    fT = pow(Q10rhco2,(S->IN.TEMP-reftemp)/10); 
    jcr_o[0] = fT;
    //fT = pow(pars[9],(0.5*(DATA.MET[m+2]+DATA.MET[m+1])-meantemp)/10);
    /* fV Volumetric factor seperating aerobic and anaerobic respiration */
    /* statistically fitting the fV curves (S1,S2,S3 schemes) with total soil moisture (PAW/PAW_fs) */
    fV = fmax(0,(1-pow(thetas,S_fv)));
    jcr_o[1] = fV;
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
	  jcr_o[2] = fW;
	/* fCH4*/
    // fT_ch4 = exp(Q10ch4*(0.5*(DATA.MET[m+2]+DATA.MET[m+1])-meantemp));
    fT_ch4 = pow(Q10ch4,(S->IN.TEMP-reftemp)/10);
    fCH4 = fmin(r_ch4 * fT_ch4,1);
    // fCH4 = r_ch4 * fT_ch4;
    /* fmin() won't be needed once CH4 data is assimilated, as CH4 portion of CO2 should be very small,
    manually put the upper limit 1 here mainly based on the mass balance, but notice that DIC is not
    considered to contribute when the current timestep Rh is depeleted by CH4. I think it's good for
    now. Can think about DIC pool meanwhile */
    jcr_o[3] = fCH4;
/*-------------------------------OUTPUTS----------------------------------------*/

*S->OUT.aerobic_tr=jcr_o[2]*jcr_o[0]*jcr_o[1];
*S->OUT.anaerobic_tr=fwc*jcr_o[0]*(1-jcr_o[1]);
*S->OUT.anaerobic_ch4_c_ratio=jcr_o[3];
*S->OUT.anaerobic_co2_c_ratio=(1-jcr_o[3]);
    
    
    

return 0;
}

