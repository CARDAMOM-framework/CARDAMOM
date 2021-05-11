#pragma once
#include "../../../DALEC_CODE/DALEC_ALL/BALDOCCHI_ANALYTICAL.c"
#include "../../../DALEC_CODE/DALEC_ALL/offset.c"
#include "../../../DALEC_CODE/DALEC_ALL/DALEC_MODULE.c"

/*Code used by Bloom et al., 2016
See also Bloom & Williams 2015,  Fox et al., 2009; Williams et al., 1997*/


int DALEC_821(DATA DATA, double const *pars)
{

double met_list[5],var_list[9],pi;
/*C-pools, fluxes, meteorology indices*/
int p,f,m,nxp, i;
int n=0,nn=0;
pi=3.1415927;


double deltat=DATA.deltat;
int nr=DATA.nodays;


/*Pointer transfer - all data stored in fluxes and pools will be passed to DATA*/
double *FLUXES=DATA.M_FLUXES;
double *POOLS=DATA.M_POOLS;
double *LAI=DATA.M_LAI;
double *NEE=DATA.M_NEE;

  /*assigning values to pools*/
  /*L,F,R,W,Lit,SOM*/
  POOLS[0]=pars[16];
  POOLS[1]=pars[17];
  POOLS[2]=pars[18];
  POOLS[3]=pars[19];
  POOLS[4]=pars[20];
  POOLS[5]=pars[21];
  /*water pool*/

POOLS[6]=pars[24];


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
*/

var_list[0]=pars[31]; /*m*/
var_list[1]=pars[32]; /*b*/
var_list[2]=pars[33]; /*gb*/
var_list[3]=pars[34]; /*Jmax*/
var_list[4]=pars[35]; /*Vmax*/
var_list[7]=7.19298; /*c1*/
var_list[8]=2.1001; /*c2*/

/*constants for exponents of leaffall and labrelease factors*/
/*width*/
double wf=pars[14]*sqrt(2)/2;
double wl=pars[12]*sqrt(2)/2;


/*factor*/
double ff=(log(pars[4])-log(pars[4]-1))/2;
/*double fl=(log(1.001)-log(0.001))/2;*/
double fl=(log(pars[29])-log(pars[29]-1))/2;




/*additional offset*/
double osf=offset(pars[4],wf);
double osl=offset(pars[29],wl);


/*scaling to biyearly sine curve*/
double sf=365.25/pi;

/*Combustion factors*/
double CF[6];
CF[0]=pars[26];
CF[1]=pars[25];
CF[2]=pars[26];
CF[3]=pars[26];
CF[4]=pars[25]/2+pars[26]/2;
CF[5]=pars[27];


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
LAI[n]=POOLS[p+1]/pars[15];

var_list[5]=LAI[n];
var_list[6]=0.6;//1.0//208.868; /* k */

met_list[0]=DATA.MET[m+1]; /*mintemp, deg C*/
met_list[1]=DATA.MET[m+2]; /*maxtemp, deg C*/
met_list[2]=DATA.MET[m+4]; /*co2 in ppm*/
met_list[3]=DATA.MET[m+3]; /*rad (swdown)*/
met_list[4]=DATA.MET[m+7]; /*VPD*/

double psfc=1013.25; /*hPa*/


double *BALDOCCHI_ANALYTICAL_OUTPUT = BALDOCCHI_ANALYTICAL(met_list,var_list);

/*GPP*/
/* GPP * soil water limitation */
FLUXES[f+0]=BALDOCCHI_ANALYTICAL_OUTPUT[0]*fmin(POOLS[p+6]/pars[23],1);
/*Evapotranspiration (VPD = DATA.MET[m+7])*/
/* ET * soil water limitation */
FLUXES[f+28]=BALDOCCHI_ANALYTICAL_OUTPUT[1]*fmin(POOLS[p+6]/pars[23],1);

/*calculate water use efficiency*/
double WUE=FLUXES[f+0]/FLUXES[f+28];

/*temprate - now comparable to Q10 - factor at 0C is 1*/
/* x (1 + a* P/P0)/(1+a)*/
FLUXES[f+1]=exp(pars[9]*(0.5*(DATA.MET[m+2]+DATA.MET[m+1])-DATA.meantemp))*((DATA.MET[m+8]/DATA.meanprec-1)*pars[30]+1);
/*respiration auto*/
FLUXES[f+2]=pars[1]*FLUXES[f+0];
/*leaf production*/
FLUXES[f+3]=(FLUXES[f+0]-FLUXES[f+2])*pars[2];
/*labile production*/
FLUXES[f+4] = (FLUXES[f+0]-FLUXES[f+2]-FLUXES[f+3])*pars[12-1];
/*root production*/
FLUXES[f+5] = (FLUXES[f+0]-FLUXES[f+2]-FLUXES[f+3]-FLUXES[f+4])*pars[4-1];
/*wood production*/
FLUXES[f+6] = FLUXES[f+0]-FLUXES[f+2]-FLUXES[f+3]-FLUXES[f+5]-FLUXES[f+4];
/*leaf fall factor*/
FLUXES[f+8] = (2/sqrt(pi))*(ff/wf)*exp(-pow(sin((DATA.MET[m+0]-pars[13]+osf)/sf)*sf/wf,2));
/*Labrelease factor*/
FLUXES[f+15]=(2/sqrt(pi))*(fl/wl)*exp(-pow(sin((DATA.MET[m+0]-pars[10]+osl)/sf)*sf/wl,2));
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
/*litter to SOM*/
FLUXES[f+14] = POOLS[p+4]*(1-pow(1-pars[1-1]*FLUXES[f+1],deltat))/deltat;

/*total pool transfers (no fires yet)*/

        POOLS[nxp+0] = POOLS[p+0] + (FLUXES[f+4]-FLUXES[f+7])*deltat;
        POOLS[nxp+1] = POOLS[p+1] + (FLUXES[f+3] - FLUXES[f+9] + FLUXES[f+7])*deltat;
        POOLS[nxp+2] = POOLS[p+2] + (FLUXES[f+5] - FLUXES[f+11])*deltat;
        POOLS[nxp+3] = POOLS[p+3] +  (FLUXES[f+6] - FLUXES[f+10])*deltat;
        POOLS[nxp+4] = POOLS[p+4] + (FLUXES[f+9] + FLUXES[f+11] - FLUXES[f+12] - FLUXES[f+14])*deltat;
        POOLS[nxp+5]= POOLS[p+5]+ (FLUXES[f+14] - FLUXES[f+13]+FLUXES[f+10])*deltat;
/*Water pool = Water pool - runoff + prec (mm/day) - ET*/
	/*printf("%2.1f\n",POOLS[p+6]);*/
	/*runoff*/
	FLUXES[f+29]=pow(POOLS[p+6],2)/pars[22]/deltat;
	/*Maximum water loss at W = pars[22]/2;*/
	if (POOLS[p+6]>pars[22]/2){FLUXES[f+29]=(POOLS[p+6]-pars[22]/4)/deltat;}

	POOLS[nxp+6]=POOLS[p+6]-FLUXES[f+29]*deltat + DATA.MET[m+8]*deltat - FLUXES[f+28]*deltat;



	/*total pool transfers - WITH FIRES*/
	/*first fluxes*/

	/*CFF = Combusted C fire flux
	NCFF = Non-combusted C fire flux*/

	/*Calculating all fire transfers (1. combustion, and 2. litter transfer)*/
	/*note: all fluxes are in gC m-2 day-1*/
	for (nn=0;nn<6;nn++){FLUXES[f+17+nn] = POOLS[nxp+nn]*DATA.MET[m+6]*CF[nn]/deltat;}
	for (nn=0;nn<4;nn++){FLUXES[f+23+nn] = POOLS[nxp+nn]*DATA.MET[m+6]*(1-CF[nn])*(1-pars[28])/deltat;}

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
	NEE[n]=-FLUXES[f+0]+FLUXES[f+2]+FLUXES[f+12]+FLUXES[f+13]+FLUXES[f+16];


}

return 0;
}
