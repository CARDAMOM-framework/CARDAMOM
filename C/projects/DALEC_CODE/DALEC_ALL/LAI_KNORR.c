#pragma once
/* Leaf Phenology Model of Knorr et al. (2010, doi: 10.1029/2009JG001119)*/
double* LAI_KNORR()
{
  /*double gc=0,pp=0,qq=0,ci=0,e0=0,mult=0,dayl=0,cps=0,dec=0;*/
  //double gc,pp,qq,ci,e0,mult,dayl,cps,dec,GPP;
/*Notes: DALEC code passes the following *consts and *pars pointers:
 double constants[10]={pars[10],0.0156935,4.22273,208.868,0.0453194,0.37836,7.19298, 0.011136,2.1001,0.789798};
pars terms:
pars[0]=LAI[n]
pars[1]=(DATA.MET[m+1] + DATA.MET[m+2]/2 ; mean air temperature
pars[2]=

constants terms:
consts[0] = T_Phi = 12.5        # parameter: temperature at leaf onset (degrees C)
consts[1] = T_r   = 2.0         # parameter: spatial range (1 sigma) of T_Phi (degrees C)
consts[2] = t_c   = 10.0        # parameter: day length at leaf shedding (hours)
consts[3] = t_r   = 0.5         # parameter: spatial range (1 sigma) of t_c (hours)
consts[4] = xi  = 0.5           # parameter: initial linear leaf growth (1/days)
consts[5] = k_L   = 0.1         # parameter: inverse of leaf longevity from start of senscence (1/days)
consts[6] = lambda_max = 6.0.   # parameter: maximum potential leaf area index (m2/m2)


constant gpppars terms
gpppars[3]=1;
gpppars[6]=DATA.LAT;
gpppars[8]=-2.0;
gpppars[9]=1.0;
gpppars[10]=pi;
gpppars[0]=LAI[n];
gpppars[1]=DATA.MET[m+2];
gpppars[2]=DATA.MET[m+1];
gpppars[4]=DATA.MET[m+4];
gpppars[5]=DATA.MET[m+5];
gpppars[7]=DATA.MET[m+3];
*/


  /*pars= &pars;  
 *   consts= &consts;
 *   */
  /* only run this on the first time step! */
  //T_init=T_phi+3*T_r
  /* cumulative normal distribution function (derived from the c-function erfc) */
  //T_deviation=(T-T_phi)/T_r
  //f_T=0.5*erfc(-T_deviation*sqrt(0.5))


  //gc=(double)pow(fabs(pars[8]),consts[9])/(consts[5] * pars[9] + 0.5 * ( pars[1]- pars[2]));
  //pp=(double)pars[0]*pars[3]/gc*consts[0]*exp(consts[7]*pars[1]);
  //qq=(double)consts[2]-consts[3];
  //ci=(double)0.5*(pars[4]+qq-pp+pow(pow(pars[4]+qq-pp,2)-4*(pars[4]*qq-pp*consts[2]),0.5));
  //e0=(double)consts[6]*pow(pars[0],2)/(pow(pars[0],2)+consts[8]);
  //dec=(double)-23.4*cos((360.*(pars[5]+10.)/365.)*pars[10]/180.)*pars[10]/180.;
  //mult=(double)tan(pars[6]*pars[10]/180)*tan(dec);
  //if (mult>=1){ 
  // dayl=24.;}  
  //else if(mult<=-1)
  //dayl=0.;
  //else{
  //dayl=(double)24.*acos(-mult) / pars[10];}


  //cps=(double)e0*pars[7]*gc*(pars[4]-ci)/(e0*pars[7]+gc*(pars[4]-ci));
  //GPP=cps*(consts[1]*dayl+consts[4]);
  static double return_arr[2];
  return_arr[0] = 5.0;
  return_arr[1] = 10.0;
  return return_arr;
}

