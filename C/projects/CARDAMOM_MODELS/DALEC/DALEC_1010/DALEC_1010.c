#pragma once
#include "../../../DALEC_CODE/DALEC_ALL/ACM.c"
#include "../../../DALEC_CODE/DALEC_ALL/offset.c"
#include "../../../DALEC_CODE/DALEC_ALL/DALEC_MODULE.c"

/*Code used by Bloom et al., 2016
See also Bloom & Williams 2015,  Fox et al., 2009; Williams et al., 1997*/


int DALEC_1010(DATA DATA, double const *pars)
{

double gpppars[11],pi;
double PAW_fs,N,thetas_opt,fwc,r_me,Q10ch4; /*jc*/
double thetas,fT,fW,fV,theta_ae,fW1,fW2,fCH4,fT_ch4; /*jc*/
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
  POOLS[0]=pars[17];    /*shuang Labile C pool*/
  POOLS[1]=pars[18];
  POOLS[2]=pars[19];
  POOLS[3]=pars[20];
  POOLS[4]=pars[21];
  POOLS[5]=pars[22];
  /*water pool*/

POOLS[6]=pars[26];
POOLS[7]=pars[35];


/* NOTES FOR POOLS AND FLUXES
DATA.MET[:,0]: projday
DATA.MET[:,1]: mintemp
DATA.MET[:,2]: maxtemp
DATA.MET[:,3]: rad
DATA.MET[:,4]: co2
DATA.MET[:,5]: yearday
DATA.MET[:,6]: burned area
DATA.MET[:,7]: VPD
DATA.MET[:,8]: precipitation


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
  30. PAW -> PUW transfer
	31. PUW runoff
  32. Aerobic Rh from litter
  33. aerobic Rh from SOM
  34. anaerobic Rh from litter
  35. anaerobic Rh from SOM
  36. Rh_CO2
  37. Rh_CH4
  38. fV Volumetric fraction of aerobic Rh
  39. fT Temperature scaler
  40. fW Water scaler
  41. fCH4 CH4 fraction
  42. thetas=PAW/PAW_fs
*/




/*constants for exponents of leaffall and labrelease factors*/
/*width*/
double wf=pars[15]*sqrt(2)/2;
double wl=pars[13]*sqrt(2)/2;


/*factor*/
double ff=(log(pars[4])-log(pars[4]-1))/2;
/*double fl=(log(1.001)-log(0.001))/2;*/
double fl=(log(pars[31])-log(pars[31]-1))/2;




/*additional offset*/
double osf=offset(pars[4],wf);
double osl=offset(pars[31],wl);


/*scaling to biyearly sine curve*/
double sf=365.25/pi;

/*Combustion factors*/
double CF[6];
CF[0]=pars[28];
CF[1]=pars[27];
CF[2]=pars[28];
CF[3]=pars[28];
CF[4]=pars[27]/2+pars[28]/2;
CF[5]=pars[29];


/*resilience factor*/


/*number of MET drivers*/
int nomet=((DALEC *)DATA.MODEL)->nomet;

/*number of DALEC pools*/
int nopools=((DALEC *)DATA.MODEL)->nopools;

/*number of DALEC fluxes to store*/
int nofluxes=((DALEC *)DATA.MODEL)->nofluxes;


/*repeating loop for each timestep*/
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

/*-----------------------------------------------------------------------*/
/*jc get env. scalers ready*/
    /*Plant available water when the soil is fully saturated*/
    PAW_fs=pars[36];
    /*Apprximate for soil mointure percentage = PAW/PAW_fs*/
    thetas = POOLS[p+6]/PAW_fs;
    /*tunable scaler describing the shape of the curve: PAW/PAW_fs against fractional volume of aerobic respiratio*/
    N=pars[37];
    /*optimum water scaler (fW), where the maximum respiration is achieved without limitation from water
    it is the highest point in the function of PAW/PAW_fs against water scaler (fW)*/
    thetas_opt=pars[38];
    /*the lowest point of water scaler when the soil is fully saturated,
     in the function of PAW/PAW_fs against water scaler (fW)*/
    fwc=pars[39];
    /*potential ratio of CO2 converted to CH4*/
    r_me=pars[40];
    /*Q10 of methane production*/
    Q10ch4=pars[41];

    // if (thetas>=0 && thetas<=1){
    //  // printf("thetas is between 0-1\n");}
    //     printf("\n");}
    //   else{
    //     printf("ERROR 1: thetas is not between 0-1\n");
    //     printf("Value of POOLS[p+6] = %f\n", POOLS[p+6]);
    //     printf("Value of PAW_fs = %f\n", PAW_fs);
    //   }
    /* fT Temperature factor on respiration */
    // fT = fmin(1,exp(pars[9]*(0.5*(DATA.MET[m+2]+DATA.MET[m+1])-DATA.meantemp)));
    //arrhenius
    //fT = exp(pars[9]*(0.5*(DATA.MET[m+2]+DATA.MET[m+1])-DATA.meantemp)); 
    //Q10
    fT = pow(pars[9],(0.5*(DATA.MET[m+2]+DATA.MET[m+1])-DATA.meantemp)/10); 
    //fT = pow(pars[9],(0.5*(DATA.MET[m+2]+DATA.MET[m+1])-DATA.meantemp)/10);
    /* fV Volumetric factor seperating aerobic and anaerobic respiration */
    /* statistically fitting the fV curves (S1,S2,S3 schemes) with total soil moisture (PAW/PAW_fs) */
    fV = fmax(0,(1-pow(thetas,N)));
    /* fW soil moisture scaler is a function of theta_ae*/
    theta_ae = ((thetas-1)/fV +1);
    fW1 = 1/thetas_opt*theta_ae;
    fW2 = ((1-fwc)/(thetas_opt-1)*theta_ae + (fwc-(1-fwc)/(thetas_opt-1)));
    if (theta_ae>=0 && theta_ae < thetas_opt){
      fW = fW1;}
    else if(theta_ae>=thetas_opt && theta_ae<=1){
      fW = fW2;}
    else{
      fW = 0;
    //   printf("ERROR 2: theta_ae is larger than 1 or smaller than 0\n");
    //   // printf("Value of theta_ae = %f\n", theta_ae);
    //   // printf("Value of thetas = %f\n", thetas);
    //   // printf("Value of fV = %f\n", fV);
    //   // printf("Value of POOLS[p+6] = %f\n", POOLS[p+6]);
    //   // printf("Value of PAW_fs = %f\n", PAW_fs);
    }
    /* fCH4*/
    // fT_ch4 = exp(Q10ch4*(0.5*(DATA.MET[m+2]+DATA.MET[m+1])-DATA.meantemp));
    fT_ch4 = pow(Q10ch4,(0.5*(DATA.MET[m+2]+DATA.MET[m+1])-DATA.meantemp)/10);
    fCH4 = fmin(r_me * fT_ch4,1);
    // fCH4 = r_me * fT_ch4;
    /* fmin() won't be needed once CH4 data is assimilated, as CH4 portion of CO2 should be very small,
    manually put the upper limit 1 here mainly based on the mass balance, but notice that DIC is not
    considered to contribute when the current timestep Rh is depeleted by CH4. I think it's good for
    now. Can think about DIC pool meanwhile */
/*-----------------------------------------------------------------------*/

/*GPP*/
FLUXES[f+0]=ACM(gpppars,constants)*fmin(POOLS[p+6]/pars[25],1);
/*Evapotranspiration (VPD = DATA.MET[m+7])*/
FLUXES[f+28]=FLUXES[f+0]*DATA.MET[m+7]/pars[23];
/*temprate - now comparable to Q10 - factor at 0C is 1*/
/* x (1 + a* P/P0)/(1+a)*/
FLUXES[f+1]=pow(pars[9],(0.5*(DATA.MET[m+2]+DATA.MET[m+1])-DATA.meantemp)/10)*((DATA.MET[m+8]/DATA.meanprec-1)*pars[32]+1);
/*respiration auto*/
FLUXES[f+2]=pars[1]*FLUXES[f+0];
/*leaf production*/
FLUXES[f+3]=(FLUXES[f+0]-FLUXES[f+2])*pars[2];
/*labile production*/
FLUXES[f+4] = (FLUXES[f+0]-FLUXES[f+2]-FLUXES[f+3])*pars[13-1];
/*root production*/
FLUXES[f+5] = (FLUXES[f+0]-FLUXES[f+2]-FLUXES[f+3]-FLUXES[f+4])*pars[4-1];
/*wood production*/
FLUXES[f+6] = FLUXES[f+0]-FLUXES[f+2]-FLUXES[f+3]-FLUXES[f+5]-FLUXES[f+4];
/*leaf fall factor*/
FLUXES[f+8] = (2/sqrt(pi))*(ff/wf)*exp(-pow(sin((DATA.MET[m+0]-pars[14]+osf)/sf)*sf/wf,2));
/*Labrelease factor*/
FLUXES[f+15]=(2/sqrt(pi))*(fl/wl)*exp(-pow(sin((DATA.MET[m+0]-pars[11]+osl)/sf)*sf/wl,2));
/*labile release - re-arrange order in next versions*/
FLUXES[f+7] = POOLS[p+0]*(1-pow(1-FLUXES[f+15],deltat))/deltat;
/*leaf litter production*/
FLUXES[f+9] = POOLS[p+1]*(1-pow(1-FLUXES[f+8],deltat))/deltat;
/*wood litter production*/
FLUXES[f+10] = POOLS[p+3]*(1-pow(1-pars[6-1],deltat))/deltat;
/*root litter production*/
FLUXES[f+11] = POOLS[p+2]*(1-pow(1-pars[7-1],deltat))/deltat;
/*respiration heterotrophic litter*/
FLUXES[f+12] = POOLS[p+4]*(1-pow(1-FLUXES[f+1]*pars[8-1],deltat))/deltat;
/*respiration heterotrophic SOM*/
FLUXES[f+13] = POOLS[p+5]*(1-pow(1-FLUXES[f+1]*pars[9-1],deltat))/deltat;
/*-----------------------------------------------------------------------*/
/*jc calculate aerobic and anaerobic respirations*/
/*32 aerobic Rh from litter*/
FLUXES[f+32] = POOLS[p+4]*(1-pow(1-fW*fT*fV*pars[8-1],deltat))/deltat;
/*33 aerobic Rh from SOM*/
FLUXES[f+33] = POOLS[p+5]*(1-pow(1-fW*fT*fV*pars[9-1],deltat))/deltat;
/*34 anaerobic Rh from litter*/
//FLUXES[f+34] = POOLS[p+4]*(1-pow(1-1*fT*(1-fV)*pars[8-1],deltat))/deltat;
FLUXES[f+34] = POOLS[p+4]*(1-pow(1-1*fT*(1-fV)*fwc*pars[8-1],deltat))/deltat;
/*35 anaerobic Rh from SOM*/
//FLUXES[f+35] = POOLS[p+5]*(1-pow(1-1*fT*(1-fV)*pars[9-1],deltat))/deltat;
FLUXES[f+35] = POOLS[p+5]*(1-pow(1-1*fT*(1-fV)*fwc*pars[9-1],deltat))/deltat;
/*36 Rh_CO2*/
FLUXES[f+36] = (FLUXES[f+32]+FLUXES[f+33])*1+(FLUXES[f+34]+FLUXES[f+35])*(1-fCH4);
/*37 Rh_CH4*/
FLUXES[f+37] = (FLUXES[f+32]+FLUXES[f+33])*0+(FLUXES[f+34]+FLUXES[f+35])*fCH4;
/*38 fV Volumetric fraction of aerobic Rh*/
FLUXES[f+38] = fV;
/*39 fT Temperature scaler*/
FLUXES[f+39] = fT;
/*40 fW Water scaler*/
FLUXES[f+40] = fW;
/*41 fCH4 CH4 fraction*/
FLUXES[f+41] = fCH4;
/*42 PAW/PAW_fs thetas*/
FLUXES[f+42] = thetas;
/* CH4 production=TEMP*RH*WEXT*Q10 */
/*FLUXES[f+32] = ch4pars[0]*(FLUXES[f+12]+FLUXES[f+13])*pow(ch4pars[1],(0.5*(DATA.MET[m+2]+DATA.MET[m+1])-15)/10)*ch4pars[2];*/
/*-----------------------------------------------------------------------*/
/*litter to SOM*/
FLUXES[f+14] = POOLS[p+4]*(1-pow(1-pars[1-1]*FLUXES[f+1],deltat))/deltat;


/*total pool transfers (no fires yet)*/

        POOLS[nxp+0] = POOLS[p+0] + (FLUXES[f+4]-FLUXES[f+7])*deltat;
        POOLS[nxp+1] = POOLS[p+1] + (FLUXES[f+3] - FLUXES[f+9] + FLUXES[f+7])*deltat;
        POOLS[nxp+2] = POOLS[p+2] + (FLUXES[f+5] - FLUXES[f+11])*deltat;
        POOLS[nxp+3] = POOLS[p+3] +  (FLUXES[f+6] - FLUXES[f+10])*deltat;
/*-----------------------------------------------------------------------*/
        /*jc*/
        /* POOLS[nxp+4] = POOLS[p+4] + (FLUXES[f+9] + FLUXES[f+11] - FLUXES[f+12] - FLUXES[f+14])*deltat;*/
        POOLS[nxp+4]=POOLS[p+4]+(FLUXES[f+9]+FLUXES[f+11]-FLUXES[f+32]-FLUXES[f+34]-FLUXES[f+14])*deltat;
        /*POOLS[nxp+5]= POOLS[p+5]+ (FLUXES[f+14] - FLUXES[f+13]+FLUXES[f+10])*deltat;*/
        POOLS[nxp+5]=POOLS[p+5]+(FLUXES[f+14]-FLUXES[f+33]-FLUXES[f+35]+FLUXES[f+10])*deltat;
/*-----------------------------------------------------------------------*/
/*Water pool = Water pool - runoff + prec (mm/day) - ET*/
	/*printf("%2.1f\n",POOLS[p+6]);*/
	/*PAW total runoff*/

	FLUXES[f+29]=pow(POOLS[p+6],2)/pars[24]/deltat*(1-pars[33]);
  /*PAW -> PUW transfer*/
	FLUXES[f+30]=FLUXES[f+29]*pars[33]/(1-pars[33]);
	/*PUW runoff*/
	FLUXES[f+31]=pow(POOLS[p+7],2)/pars[34]/deltat;
	/*Maximum water loss at W = pars[24]/2;*/
	if (POOLS[p+6]>pars[24]/2){FLUXES[f+29]=(POOLS[p+6]-pars[24]/4)/deltat*(1-pars[33]);
        FLUXES[f+30]=(POOLS[p+6]-pars[24]/4)/deltat*pars[33]/(1-pars[33]);}
	if (POOLS[p+7]>pars[34]/2){FLUXES[f+31]=(POOLS[p+7]-pars[34]/4)/deltat;}

	POOLS[nxp+6]=POOLS[p+6] + (-FLUXES[f+29] - FLUXES[f+30] + DATA.MET[m+8] - FLUXES[f+28])*deltat;
	/*Plant-unavailable water budget*/

        POOLS[nxp+7]=POOLS[p+7] + (FLUXES[f+30] - FLUXES[f+31])*deltat;



	/*total pool transfers - WITH FIRES*/
	/*first fluxes*/

	/*CFF = Combusted C fire flux
	NCFF = Non-combusted C fire flux*/

	/*Calculating all fire transfers (1. combustion, and 2. litter transfer)*/
	/*note: all fluxes are in gC m-2 day-1*/
	for (nn=0;nn<6;nn++){FLUXES[f+17+nn] = POOLS[nxp+nn]*DATA.MET[m+6]*CF[nn]/deltat;}
	for (nn=0;nn<4;nn++){FLUXES[f+23+nn] = POOLS[nxp+nn]*DATA.MET[m+6]*(1-CF[nn])*(1-pars[30])/deltat;}

	/*Adding all fire pool transfers here*/
	/*live C pools*/
	for (nn=0;nn<4;nn++){POOLS[nxp+nn]=POOLS[nxp+nn]-(FLUXES[f+17+nn]+FLUXES[f+23+nn])*deltat;}
	/*dead C pools*/
	/*litter*/
	POOLS[nxp+4]=POOLS[nxp+4]+(FLUXES[f+23]+FLUXES[f+23+1]+FLUXES[f+23+2]-FLUXES[f+17+4]-FLUXES[f+23+4])*deltat;
	/*som*/
	POOLS[nxp+5]=POOLS[nxp+5]+(FLUXES[f+23+3]+FLUXES[f+23+4]-FLUXES[f+17+5])*deltat;

	/*fires - total flux in gC m-2 day-1*/
	/*this term is now (essentially) obsolete*/
	/*replace in next version of DALEC_FIRES*/
	FLUXES[f+16]=0;for (nn=0;nn<6;nn++){FLUXES[f+16]+=FLUXES[f+17+nn];}

	/*Net ecosystem exchange + Fires*/
/*jc*/
/*	NEE[n]=-FLUXES[f+0]+FLUXES[f+2]+FLUXES[f+12]+FLUXES[f+13]+FLUXES[f+16];*/
/* did not count for CH4 for a direct mapping to the observation NEE*/
NEE[n]=-FLUXES[f+0]+FLUXES[f+2]+FLUXES[f+36]+FLUXES[f+16];


}

return 0;
}
