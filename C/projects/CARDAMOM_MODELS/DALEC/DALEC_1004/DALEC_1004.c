#pragma once
#include "../../../DALEC_CODE/DALEC_ALL/ACM.c"
#include "../../../DALEC_CODE/DALEC_ALL/offset.c"
#include "../../../DALEC_CODE/DALEC_ALL/DALEC_MODULE.c"
#include "EDC1_1004.c"
#include "EDC2_1004.c"

/*Code used by Bloom et al., 2016
See also Bloom & Williams 2015,  Fox et al., 2009; Williams et al., 1997*/



int MOUNT_DALEC_1004_PARS(DATA * DATA){

/*Step 1. Index MODULE*/
DATA->MODULE_IDX.MODELS.dalec_1004=DATA->nopars;

/*Populate with module parameters*/
double parmin[36], parmax[36];
char *parname[36];

parname[0]="Decomposition rate";
parmin[0]=0.00001;
parmax[0]=0.01;

parname[1]="Fraction of GPP repsired";
parmin[1]=0.2;
parmax[1]=0.8;

parname[2]="Fraction of (1-fgpp) to foliage";
parmin[2]=0.01;
parmax[2]=0.5;

parname[3]="Fraction to roots";
parmin[3]=0.01;
parmax[3]=1;

parname[4]="Leaf Lifespan";
parmin[4]=1.001;
parmax[4]=8;

parname[5]="TOR wood [yr-1]";
parmin[5]=0.000025;
parmax[5]=0.001;

parname[6]="TOR roots";
parmin[6]=0.0001;
parmax[6]=0.01;

parname[7]="TOR litter";
parmin[7]=0.0001;
parmax[7]=0.01;

parname[8]="TOR SOM";
parmin[8]=0.0000001;
parmax[8]=0.001;

parname[9]="Temp factor (Q10)";
parmin[9]=0.018;
parmax[9]=0.08;

parname[10]="ACM canopy efficiency";
parmin[10]=5;
parmax[10]=50;

parname[11]="Leaf onset day";
parmin[11]=365.25;
parmax[11]=365.25*4;

parname[12]="Fraction to Clab";
parmin[12]=0.01;
parmax[12]=0.5;

parname[13]="Clab Release period";
parmin[13]=365.25/12;
parmax[13]=100;


parname[14]="Leaf fall day";
parmin[14]=365.25;
parmax[14]=365.25*4;

parname[15]="Leaf fall period";
parmin[15]=365.25/12;
parmax[15]=150;

/*LMCA*/
/*Kattge et al. 2011*/
/*Kattge et al., provide a range of 10 400 g m-2, i.e. 5 200 gC m-2*/
parname[16]="LCMA [gC/m2]";
parmin[16]=5;
parmax[16]=200;

/*C labile*/
parname[17]="C labile @ t=0";
parmin[17]=1.0;
parmax[17]=2000.0;

/*C foliar*/
parname[18]="C foliar @ t=0";
parmin[18]=1.0;
parmax[18]=2000.0;

/*C roots*/
parname[19]="C roots @ t=0";
parmin[19]=1.0;
parmax[19]=2000.0;

/*C_wood*/
parname[20]="C wood @ t=0";
parmin[20]=1.0;
parmax[20]=100000.0;

/*C litter*/
parname[21]="C litter @ t=0 [gC]";
parmin[21]=1.0;
parmax[21]=2000.0;

/*C_som*/
parname[22]="C som @ t=0 [gC]";
parmin[22]=1.0;
parmax[22]=200000.0;

/*IWUE: GPP*VPD/ET: gC/kgH2o *hPa*/
parname[23]="iWUE";
parmin[23]=10;
parmax[23]=50;

/*Runoff focal point (~maximum soil storage capacity x 4)*/
parname[24]="Q focal point [mm]";
parmin[24]=1;
parmax[24]=100000;

/*"Wilting point"*/
parname[25]="Wilting point [mm]";
parmin[25]=1;
parmax[25]=10000;

/*"Bucket at t0"*/
parname[26]="PAW @ t=0 [mm]";
parmin[26]=1;
parmax[26]=10000;

parname[27]="Foliar biomass CF";
parmin[27]=0.01;
parmax[27]=1;

/*"Ligneous" biomass CF".*/
parname[28]="Stem biomass CF";
parmin[28]=0.01;
parmax[28]=1;

parname[29]="DOM CF";
parmin[29]=0.01;
parmax[29]=1;

parname[30]="Fire resilience factor";
parmin[30]=0.01;
parmax[30]=1;

parname[31]="Lab pool lifespan (yrs)";
parmin[31]=1.001;
parmax[31]=8;

parname[32]="Moisture factor";
parmin[32]=0.01;
parmax[32]=1;

/*PAW->PUW runoff fraction*/
parname[33]="PAW->PUW Q fraction";
parmin[33]=0.01;
parmax[33]=1;

/*PUW Runoff focal point (~maximum soil storage capacity x 4)*/
parname[34]="Runoff focal point [mm]";
parmin[34]=1;
parmax[34]=100000;

parname[35]="PUW pool";
parmin[35]=1;
parmax[35]=10000;

int n;
for (n=0;n<36;n++){
ADD_PARAMETER_TO_STACK(DATA,parname[n],parmin[n],parmax[n]);}


return 0;
}



int DALEC_1004(DATA DATA, double const *pars)
{


/*Step 1. Calculate EDC1, chec
*/
DALEC *MODEL=(DALEC *)DATA.MODEL;

/*Copy default structure*/
/*EDCD=*((DALEC *)DATA.MODEL)->EDCD;*/
int k;
double P=0;
int EDC=1;
EDC=ipow(EDC1_1004(pars,DATA, MODEL->EDCD),DATA.EDC);
P=P+log((double)EDC);


if (EDC==1 | MODEL->EDCD->DIAG>0){
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



/*GPP*/
FLUXES[f+0]=ACM(gpppars,constants)*fmin(POOLS[p+6]/pars[25],1);
/*Evapotranspiration (VPD = DATA.MET[m+7])*/
FLUXES[f+28]=FLUXES[f+0]*DATA.MET[m+7]/pars[23];
/*temprate - now comparable to Q10 - factor at 0C is 1*/
/* x (1 + a* P/P0)/(1+a)*/
FLUXES[f+1]=exp(pars[9]*0.5*(DATA.MET[m+2]+DATA.MET[m+1]-DATA.meantemp))*((DATA.MET[m+8]/DATA.meanprec-1)*pars[32]+1);
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
	NEE[n]=-FLUXES[f+0]+FLUXES[f+2]+FLUXES[f+12]+FLUXES[f+13]+FLUXES[f+16];


}


EDC=ipow(EDC2_1004(pars,DATA, MODEL->EDCD),DATA.EDC);
P=P+log((double)EDC);
for (k=0;k<100;k++){printf("%i ",DATA.M_EDCD[k]);}
printf("\n");

}
printf("***");

return P;
}







