#pragma once
#include "./LAI_KNORR_funcs.c"
/* Leaf Phenology Model of Knorr et al. (2010, doi: 10.1029/2009JG001119)*/
double* LAI_GSI(double const *met_list, double const *var_list)
{
/*Notes: DALEC code passes the following *consts and *pars pointers:
 double constants[10]={pars[10],0.0156935,4.22273,208.868,0.0453194,0.37836,7.19298, 0.011136,2.1001,0.789798};

Forcing inputs:
  latitude
  day of year
  pi
  Temperature
  VPD

Parameter inputs:
  Tmnmin            # parameter: minimum temperature threshold for GSI
  Tmnmax            # parameter: maximum temperature threshold for GSI
  Photoperiodmin    # parameter: minimum photoperiod threshold for GSI
  Photoperiodmax    # parameter: maximum photoperiod threshold for GSI
  VPDmin            # parameter: minimum VPD threshold for GSI
  VPDmax            # parameter: maximum VPD threshold for GSI

*/

  /*initialize intermediate variables*/
  double daylengthpars[3];
  double daylength;
  double Tmn, VPD;
  double Tmnmin, Tmnmax;
  double Photoperiodmin, Photoperiodmax;
  double VPDmin, VPDmax;
  double Tfac, PHOTOfac, VPDfac;
  double GSI;

  // Forcings/drivers
  Tmn=(double)met_list[0]; // Original DALEC model, Tmn is the 21 day average Tmn, MET[m+9]
  VPD=(double)met_list[1]; // Original DALEC model, this is the 21 day average VPD, MET[m+11]

  // Parameters
  Tmnmin=(double)var_list[3]; // Original DALEC model, pars[13];
  Tmnmax=(double)var_list[4]; // Original DALEC model, pars[14];
  Photoperiodmin=(double)var_list[5]; // Original DALEC model, pars[15];
  Photoperiodmax=(double)var_list[6]; // Original DALEC model, pars[23];
  VPDmin=(double)var_list[7]; // Original DALEC model, pars[24];
  VPDmax=(double)var_list[8]; // Original DALEC model, pars[25];

  /* compute daylight hours (hours of sunlight per day) */
  daylengthpars[0]=var_list[0];  /* latitude */
  daylengthpars[1]=var_list[1];  /* day of year */
  daylengthpars[2]=var_list[2];  /* pi */
  daylength = ComputeDaylightHours(daylengthpars);  // Original DALEC model, this is the 21 day average photoperiod, MET[m+10]

  /* 25/09/14 - JFE
  Here we calculate the Growing Season Index based on 
   Jolly et al. A generalized, bioclimatic index to predict foliar phenology in response to climate Global Change Biology, Volume 11, page 619-632 - 2005 (doi: 10.1111/j.1365-2486.2005.00930.x) 
  - Stoeckli, R., T. Rutishauser, I. Baker, M. A. Liniger, and A. S. Denning (2011), A global reanalysis of vegetation phenology, J. Geophys. Res., 116, G03020, doi:10.1029/2010JG001545.
  
  It is the product of 3 limiting factors for temperature, photoperiod and vapour pressure deficit 
  that grow linearly from 0 to 1 between a calibrated min and max value.
  Temperature, photoperiod and VPD are direct input
  */    


  // calculate the temperature based limiting factor
  if (Tmn > Tmnmax) // Tmin larger than parameter Tmmax
  {
      Tfac = 1.;
  }
  else if (Tmn < (Tmnmin)) // Tmin lower than parameter Tmmin
  {
      Tfac = 0.;         
  }
  else
  {
      Tfac = (Tmn-(Tmnmin))/(Tmnmax-(Tmnmin));
  }

  // calculate the photoperiod limiting factor
  if (daylength > Photoperiodmax) // photoperiod larger than parameter PHOTOmax
  {
      PHOTOfac = 1.;
  }
  else if (daylength < Photoperiodmin) // photoperiod lower than parameter PHOTOmin
  {
      PHOTOfac = 0.;         
  }
  else
  {
      PHOTOfac = (daylength-Photoperiodmin)/(Photoperiodmax-Photoperiodmin);
  }

  // calculate the VPD based limiting factor - high VPD is limiting
  if (VPD>VPDmax) // VPD larger than parameter VPDmax
  {
      VPDfac = 0.;
  }
  else if (VPD<VPDmin) // VPD lower than parameter VPDmin
  {
      VPDfac = 1.;         
  }
  else
  {
      VPDfac = 1-(VPD-VPDmin)/(VPDmax-VPDmin);
  }

  // calculate the GSI and store it in the FLUXES array with ID 17
  GSI = Tfac*PHOTOfac*VPDfac;


  static double return_arr[4];
  return_arr[0] = GSI;
  return_arr[1] = Tfac;
  return_arr[2] = PHOTOfac;
  return_arr[3] = VPDfac;
  return return_arr;
}

