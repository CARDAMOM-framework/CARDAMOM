#pragma once
#include "../../../DALEC_CODE/DALEC_ALL/ACM.c"
#include "../../../DALEC_CODE/DALEC_ALL/offset.c"
#include "../../../DALEC_CODE/DALEC_ALL/DALEC_MODULE.c"

/*Code used by Bloom et al., 2016
See also Bloom & Williams 2015,  Fox et al., 2009; Williams et al., 1997*/


int DALEC_900(DATA DATA, double const *pars)
{

double gpppars[11],pi;
/*C-pools, fluxes, meteorology indices*/
int p,f,m,nxp, i;
int n=0,nn=0;
pi=3.1415927;


/*constant gpppars terms*/
gpppars[3]=1;
gpppars[6]=DATA.LAT;
gpppars[8]=-2.0;
gpppars[9]=1.0;
gpppars[10]=pi;

double deltat=DATA.deltat;
int nr=DATA.nodays;


double constants[10]={pars[10],0.0156935,4.22273,208.868,0.0453194,0.37836,7.19298, 0.011136,2.1001,0.789798};

/*Pointer transfer - all data stored in fluxes and pools will be passed to DATA*/
double *FLUXES=DATA.M_FLUXES;
double *POOLS=DATA.M_POOLS;
double *LAI=DATA.M_LAI;
double *NEE=DATA.M_NEE;

  /*assigning values to pools*/
  /*L,F,R,W,Lit,SOM*/
  POOLS[0]=pars[17];
  POOLS[1]=pars[18];
  POOLS[2]=pars[19];
  POOLS[3]=pars[20];
  POOLS[4]=pars[21];
  POOLS[5]=pars[22];
  /*water pool*/

/* NOTES FOR POOLS AND FLUXES
DATA.MET[:,0]: projday
DATA.MET[:,1]: mintemp
DATA.MET[:,2]: maxtemp
DATA.MET[:,3]: rad
DATA.MET[:,4]: co2
DATA.MET[:,5]: DoY
DATA.MET[:,6]: removal by fire (i.e. leading to emissions)
DATA.MET[:,7]: VPD


  POOLS[0,0]=pars(8);Lab
  POOLS[0,1]=pars(5);Fol
  POOLS[0,2]=pars(6);Roo
  POOLS[0,3]=pars(3);Woo
  POOLS[0,4]=pars(2);Litter
  POOLS[0,5]=pars(2);Som


        %fluxes - other*********
        0.GPP
        1.temprate
        2.respiration_auto
        3.leaf_production
        4.labile_production
        5.root_production
        6.wood_production
        7.labile_release
        8.leaffall_factor
        9.leaflitter_production
        10.woodlitter_production
        11.rootlitter_production
     	12.respiration_het_litter
  	13.respiration_het_som
  	14.litter2som
  	15.labrelease_factor
	16. Fires (total)
	17-22. Fires (C pools to atmosphere)
	23-27. Fires (C pool transfers)
	28. ET
	29. Runoff
*/

/*scaling to biyearly sine curve*/
double sf=365.25/pi;

/*Combustion factors*/
double CF[6];
CF[0]=pars[27];
CF[1]=pars[26];
CF[2]=pars[27];
CF[3]=pars[27];
CF[4]=pars[26]/2+pars[27]/2;
CF[5]=pars[28];


/*resilience factor*/


/*number of MET drivers*/
int nomet=((DALEC *)DATA.MODEL)->nomet;

/*number of DALEC pools*/
int nopools=((DALEC *)DATA.MODEL)->nopools;

/*number of DALEC fluxes to store*/
int nofluxes=((DALEC *)DATA.MODEL)->nofluxes;

// resilience factor
double rfac=0.5; // fraction of the none-burnt pool that survives

/*repeating loop for each deltat*/
for (n=0; n < nr; n++){
/*ppol index*/
p=nopools*n;
/*next pool index*/
nxp=nopools*(n+1);
/*met index*/
m=nomet*n;
/*flux array index*/
f=nofluxes*n;



/*LAI*/
LAI[n]=POOLS[p+1]/pars[16];
/*GPP*/
gpppars[0]=LAI[n];
gpppars[1]=DATA.MET[m+2];
gpppars[2]=DATA.MET[m+1];
gpppars[4]=DATA.MET[m+4];
gpppars[5]=DATA.MET[m+5];
gpppars[7]=DATA.MET[m+3];



/*GPP*/
FLUXES[f+0]=ACM(gpppars,constants);

// temprate - now comparable to Q10 - factor at 0C is 1
FLUXES[f+1]=exp(pars[9]*0.5*(DATA.MET[m+2]+DATA.MET[m+1]));
// respiration auto
FLUXES[f+2]=pars[1]*FLUXES[f+0];
// allocation to leaves
FLUXES[f+3]=(FLUXES[f+0]-FLUXES[f+2])*pars[2];
// allocation to labile pool
FLUXES[f+4] = (FLUXES[f+0]-FLUXES[f+2]-FLUXES[f+3])*pars[12];
// allocation to roots
FLUXES[f+5] = (FLUXES[f+0]-FLUXES[f+2]-FLUXES[f+3]-FLUXES[f+4])*pars[3];
// allocation to wood
FLUXES[f+6] = FLUXES[f+0]-FLUXES[f+2]-FLUXES[f+3]-FLUXES[f+5]-FLUXES[f+4];

/* 25/09/14 - JFE
Here we calculate the Growing Season Index based on
- Jolly et al. A generalized, bioclimatic index to predict foliar phenology in response to climate Global Change Biology, Volume 11, page 619-632 - 2005 (doi: 10.1111/j.1365-2486.2005.00930.x)
- Stoeckli, R., T. Rutishauser, I. Baker, M. A. Liniger, and A. S. Denning (2011), A global reanalysis of vegetation phenology, J. Geophys. Res., 116, G03020, doi:10.1029/2010JG001545.

It is the product of 3 limiting factors for temperature, photoperiod and vapour pressure deficit
that grow linearly from 0 to 1 between a calibrated min and max value.
Temperature, photoperiod and VPD are direct input
*/

// calculate the temperature based limiting factor
double Tfac;
double p14c = pars[14];
double p13c = pars[13];
if ((DATA.MET[m+1]+273.15)>p14c) // Tmin larger than parameter Tmmax
{
   Tfac = 1.;
}
else if ((DATA.MET[m+1]+273.15)<p13c) // Tmin lower than parameter Tmmin
{
   Tfac = 0.;
}
else
{
   Tfac = (DATA.MET[m+1]+273.15-(p13c))/(p14c-(p13c));
}

double dec = -23.4*cos((360.*(gpppars[5]+10.)/365.)*gpppars[10]/180.)*gpppars[10]/180.;
double mult = tan(gpppars[6]*gpppars[10]/180)*tan(dec);
double dayl;
if (mult>=1){
dayl=24.*60*60;} /* in seconds */
else if(mult<=-1)
dayl=0.*60*60;
else{
dayl=24.*60*60*acos(-mult) / gpppars[10];}

// calculate the photoperiod limiting factor
double PHOTOfac;
if (dayl>pars[23]) // photoperiod larger than parameter PHOTOmax
{
   PHOTOfac = 1.;
}
else if (dayl<pars[15]) // photoperiod lower than parameter PHOTOmin
{
   PHOTOfac = 0.;
}
else
{
   PHOTOfac = (dayl-pars[15])/(pars[23]-pars[15]);
}

// calculate the VPD based limiting factor - high VPD is limiting
double VPDfac;
if (DATA.MET[m+7]>pars[25]) // VPD larger than parameter VPDmax
{
   VPDfac = 0.;
}
else if (DATA.MET[m+7]<pars[24]) // VPD lower than parameter VPDmin
{
   VPDfac = 1.;
}
else
{
   VPDfac = 1-(DATA.MET[m+7]-pars[24])/(pars[25]-pars[24]);
}

// calculate the GSI and store it in the FLUXES array with ID 17
FLUXES[f+28] = Tfac*PHOTOfac*VPDfac;

FLUXES[f+8]=(1-FLUXES[f+28])*pars[4]; // leaf fall
FLUXES[f+15]=FLUXES[f+28]*pars[11]; // lab release

// Average biogenic fluxes over the time step

// total labile release into foliar
FLUXES[f+7] = POOLS[p+0]*(1-pow(1-FLUXES[f+15],deltat))/deltat;

// leaf to litter
FLUXES[f+9] = POOLS[p+1]*(1-pow(1-FLUXES[f+8],deltat))/deltat;

// wood to SOM
FLUXES[f+10] = POOLS[p+3]*(1-pow(1-pars[5],deltat))/deltat;

// root to litter
FLUXES[f+11] = POOLS[p+2]*(1-pow(1-pars[6],deltat))/deltat;

// respiration heterotrophic from litter
FLUXES[f+12] = POOLS[p+4]*(1-pow(1-FLUXES[f+1]*pars[7],deltat))/deltat;

// respiration heterotrophic from SOM
FLUXES[f+13] = POOLS[p+5]*(1-pow(1-FLUXES[f+1]*pars[8],deltat))/deltat;

// decomposition of litter into SOM
FLUXES[f+14] = POOLS[p+4]*(1-pow(1-pars[0]*FLUXES[f+1],deltat))/deltat;


// Update pools

// labile pool: +allocation of NPP -leaf production
POOLS[nxp+0] = POOLS[p+0] + (FLUXES[f+4]-FLUXES[f+7])*deltat;

// foliar pool: +allocation of NPP -leaf fall + production from labile
POOLS[nxp+1] =  POOLS[p+1] + (FLUXES[f+3] - FLUXES[f+9] + FLUXES[f+7])*deltat;

// wood pool: +allocation of NPP -wood transfer to SOM
POOLS[nxp+3] = POOLS[p+3] +  (FLUXES[f+6] - FLUXES[f+10])*deltat;

// root pool: +allocation of NPP -transfer to litter
POOLS[nxp+2] = POOLS[p+2] + (FLUXES[f+5] - FLUXES[f+11])*deltat;

// litter pool: +leaf fall +root to litter -rh_litter -decomposition
POOLS[nxp+4] = POOLS[p+4] + (FLUXES[f+9] + FLUXES[f+11] - FLUXES[f+12] - FLUXES[f+14])*deltat;

// SOM pool: +decomposition of litter -rh_som +wood_transfer
POOLS[nxp+5]= POOLS[p+5]+ (FLUXES[f+14] - FLUXES[f+13] + FLUXES[f+10])*deltat;

NEE[n]=-FLUXES[f+0]+FLUXES[f+2]+FLUXES[f+12]+FLUXES[f+13]; // NEE only contains biogenic fluxes

// Reco[n] = FLUXES[f+2]+FLUXES[f+12]+FLUXES[f+13];

// perform the deforestation removal of labile, leaf and wood if require
/*if (DATA.MET[m+6] > 0.)
{
   POOLS[nxp+0] = POOLS[nxp+0]*(1-DATA.MET[m+6]);
   POOLS[nxp+1] = POOLS[nxp+1]*(1-DATA.MET[m+6]);
   POOLS[nxp+3] = POOLS[nxp+3]*(1-DATA.MET[m+6]);
} // end removal*/

FLUXES[f+16]=0.; // fire emissions
// perform the fire part and save all fluxes
if (DATA.MET[m+6] > 0.)
{
 // Calculate all fire transfers (1. combustion, and 2. litter transfer) note: all fluxes are in gC m-2 day-1
 for (nn=0;nn<6;nn++){FLUXES[f+17+nn] = POOLS[nxp+nn]*DATA.MET[m+6]*CF[nn]/deltat;} // combustion
 for (nn=0;nn<5;nn++){FLUXES[f+23+nn] = POOLS[nxp+nn]*DATA.MET[m+6]*(1-CF[nn])*(1-rfac)/deltat;} // litter transfer

 // Update pools
 // Live C pools
 for (nn=0;nn<4;nn++) {POOLS[nxp+nn]=POOLS[nxp+nn]-(FLUXES[f+17+nn]+FLUXES[f+23+nn])*deltat;}
 // Dead C pools Litter and SOM
 POOLS[nxp+4]=POOLS[nxp+4]+(FLUXES[f+23]+FLUXES[f+23+1]+FLUXES[f+23+2]-FLUXES[f+17+4]-FLUXES[f+23+4])*deltat;
 POOLS[nxp+5]=POOLS[nxp+5]+(FLUXES[f+23+3]+FLUXES[f+23+4]-FLUXES[f+17+5])*deltat;

 // save the sum of all fluxes
   for (nn=0;nn<6;nn++){FLUXES[f+16]+=FLUXES[f+17+nn];}

}
else // be sure fluxes are 0
{
   for (nn=0;nn<6;nn++) {FLUXES[f+17+nn] = 0;}
   for (nn=0;nn<5;nn++) {FLUXES[f+23+nn] = 0;}
}// end fire

} // end time steploop

return 0;
} // end CARBON_MDEOl
