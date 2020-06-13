#pragma once
/* GPP RESPONSE CURVE HERE*/
double ACM(double const *pars, double const *consts)
{
  /*double gc=0,pp=0,qq=0,ci=0,e0=0,mult=0,dayl=0,cps=0,dec=0;*/
  double gc,pp,qq,ci,e0,mult,dayl,cps,dec,GPP;
/*Notes: DALEC code passes the following *consts and *pars pointers:
 double constants[10]={pars[10],0.0156935,4.22273,208.868,0.0453194,0.37836,7.19298, 0.011136,2.1001,0.789798};
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
  gc=(double)pow(fabs(pars[8]),consts[9])/(consts[5] * pars[9] + 0.5 * ( pars[1]- pars[2]));
  pp=(double)pars[0]*pars[3]/gc*consts[0]*exp(consts[7]*pars[1]);
  qq=(double)consts[2]-consts[3];
  ci=(double)0.5*(pars[4]+qq-pp+pow(pow(pars[4]+qq-pp,2)-4*(pars[4]*qq-pp*consts[2]),0.5));
  e0=(double)consts[6]*pow(pars[0],2)/(pow(pars[0],2)+consts[8]);
  dec=(double)-23.4*cos((360.*(pars[5]+10.)/365.)*pars[10]/180.)*pars[10]/180.;
  mult=(double)tan(pars[6]*pars[10]/180)*tan(dec);
  if (mult>=1){ 
   dayl=24.;}  
  else if(mult<=-1)
  dayl=0.;
  else{
  dayl=(double)24.*acos(-mult) / pars[10];}


  cps=(double)e0*pars[7]*gc*(pars[4]-ci)/(e0*pars[7]+gc*(pars[4]-ci));
  GPP=cps*(consts[1]*dayl+consts[4]);
  return GPP;
}

