#pragma once
#include "../../../DALEC_CODE/DALEC_ALL/ACM.c"
#include "../../../DALEC_CODE/DALEC_ALL/offset.c"
#include "../../../DALEC_CODE/DALEC_ALL/DALEC_MODULE.c"
#include "../../../DALEC_CODE/DALEC_ALL/LAI_KNORR.c"

/*Code used by Bloom et al., 2016
See also Bloom & Williams 2015,  Fox et al., 2009; Williams et al., 1997*/

int DALEC_1021(DATA DATA, double const *pars)
{

double gpppars[11],pi,lai_met_list[1],lai_var_list[20];
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
  // POOLS[1]=pars[18];
  POOLS[1]=pars[36]*pars[16];
  POOLS[2]=pars[19];
  POOLS[3]=pars[20];
  POOLS[4]=pars[21];
  POOLS[5]=pars[22];
  /*water pool*/
  POOLS[6]=pars[26];
  POOLS[7]=pars[35];
  /*LAI module variables*/
  POOLS[8]=pars[36];  /* LAI */


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

/*foliar carbon transfer intermediate variables*/
double netCeffect, nominalClosses;
double LAIClimdiff, FCdiff, LAIdiff;

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
/* for the first iteration we compute an initial value of
  evapotranspiration and soil water for use in the LAI_KNORR module */
if (n==0){
  printf("deltat = %2.7f\n",deltat);
  LAI[n]=POOLS[p+1]/pars[16];
  lai_var_list[2]=pars[36];
  /*GPP*/
  gpppars[0]=pars[36];
  gpppars[1]=DATA.MET[m+2];
  gpppars[2]=DATA.MET[m+1];
  gpppars[4]=DATA.MET[m+4];
  gpppars[5]=DATA.MET[m+5];
  gpppars[7]=DATA.MET[m+3];
  /*GPP*/
  FLUXES[f+0]=ACM(gpppars,constants)*fmin(POOLS[p+6]/pars[25],1);
  /*Evapotranspiration (VPD = DATA.MET[m+7])*/
  FLUXES[f+28]=FLUXES[f+0]*DATA.MET[m+7]/pars[23];
  /*Put this evapotranspiration flux into the LAI_KNORR input list*/
  lai_var_list[18]=FLUXES[f+28];
  /*Plant-available water*/
  POOLS[p+6]=pars[26];
  /*Put this plant-available soil water into the LAI_KNORR input list*/
  lai_var_list[17]=POOLS[p+6];
  /*Initialize phenology memory of air-temperature */
  lai_var_list[5]=pars[37]+3*pars[38];
  /*Initialize phenology memory of water/structural limitation */
  lai_var_list[11]=pars[42];
}

lai_met_list[0]=(DATA.MET[m+2] + DATA.MET[m+1])/2.0; /* meantemp, deg C*/
lai_var_list[0]=n; /*current timestep index of model run*/
lai_var_list[19]=deltat; /*time increment of model run*/
lai_var_list[1]=pars[36]; /*initial LAI parameter*/
lai_var_list[3]=pars[37]; /*T_phi*/
lai_var_list[4]=pars[38]; /*T_r*/
lai_var_list[6]=pars[39]; /*tau_m*/
lai_var_list[7]=pars[40]; /*plgr*/
lai_var_list[8]=pars[41]; /*k_L*/
lai_var_list[9]=pars[42]; /*lambda_max*/
lai_var_list[10]=pars[43]; /*tau_W*/
lai_var_list[12]=DATA.LAT; /*latitude*/
lai_var_list[13]=DATA.MET[m+5]; /*day of year*/
lai_var_list[14]=pi; /*pi*/
lai_var_list[15]=pars[44]; /*t_c*/
lai_var_list[16]=pars[45]; /*t_r*/
// Run KNORR LAI module
LAI[n]=LAI_KNORR(lai_met_list, lai_var_list)[0];
FLUXES[f+34] = LAI_KNORR(lai_met_list, lai_var_list)[0];  // LAI (environmental target)
FLUXES[f+35] = LAI_KNORR(lai_met_list, lai_var_list)[1];  // T_memory
FLUXES[f+36] = LAI_KNORR(lai_met_list, lai_var_list)[2];  // lambda_max_memory
FLUXES[f+37] = LAI_KNORR(lai_met_list, lai_var_list)[3]/deltat;  // dlambda/dt (units: LAI per day)

/*Calculate the initial foliar carbon pool*/
// if (n==0){
  // POOLS[p+1]=FLUXES[f+34]*pars[16];
// }

/* if target LAI change (in units of carbon flux) exceeds the available carbon from labile pool, we down-scale the 
LAI increment (dlambda/dt) and the KNORR LAI */
// if (FLUXES[f+37]*FLUXES[f+40] + POOLS[p+1]/pars[46] > POOLS[p+0]/deltat){
if (FLUXES[f+37]*pars[16] > POOLS[p+0]/deltat){
  /* flag for carbon availability limitation (1 = labile C limits LAI growth) */
  FLUXES[f+38]=1.0;
  /* down-scale LAI by the difference between what is requested and what is available */
  /* - we compute the difference between the KNORR LAI rate of change (in C-flux units) and 
       what is available from C-labile. This is in g C m-2 d-1.
     - then we need to compute how much LAI should be down-scaled by, so we compute the LAI 
       per model timestep */
  FCdiff = (FLUXES[f+37]*pars[16] + POOLS[p+1]/pars[46]) - POOLS[p+0]/deltat;
  LAIdiff = (FCdiff*deltat)/pars[16];
  // FLUXES[f+34] = FLUXES[f+34] - LAIdiff;
  /* set dlambdadt to be available carbon in C-labile, divided by LMCA to get it back into LAI units*/
  // FLUXES[f+37]=(POOLS[p+0]/deltat)/FLUXES[f+40];
}
else {
	/* flag for carbon availability limitation (1 = labile C limits LAI growth) */
	FLUXES[f+38]=0.0;
}

LAI[n]=POOLS[p+1]/pars[16]; 

/*GPP*/
  gpppars[0]=FLUXES[f+34];
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

/*requested carbon from labile to foliar (governed by LAI_KNORR)*/
// FLUXES[f+32]=fmax(fmin(FLUXES[f+37]*FLUXES[f+40] + POOLS[p+1]/pars[46], POOLS[p+0]/deltat), 0);
FLUXES[f+32]=fmax(FLUXES[f+37]*pars[16], 0);
/*foliar to litter carbon flux(governed by LAI_KNORR)*/
// FLUXES[f+33]=-fmin(fmin(FLUXES[f+37]*FLUXES[f+40] + POOLS[p+1]/pars[46], POOLS[p+0]/deltat), 0) + POOLS[p+1]/pars[46];
FLUXES[f+33]=-fmin(FLUXES[f+37]*pars[16], 0);
/*leaf production*/
FLUXES[f+3] = FLUXES[f+32];
/*labile production*/
FLUXES[f+4] = (FLUXES[f+0]-FLUXES[f+2])*pars[13-1];
/*root production*/        
FLUXES[f+5] = (FLUXES[f+0]-FLUXES[f+2]-FLUXES[f+3]-FLUXES[f+4])*pars[4-1];            
/*wood production*/       
FLUXES[f+6] = FLUXES[f+0]-FLUXES[f+2]-FLUXES[f+3]-FLUXES[f+5]-FLUXES[f+4]; 
/*leaf fall factor*/
FLUXES[f+8] = (2/sqrt(pi))*(ff/wf)*exp(-pow(sin((DATA.MET[m+0]-pars[14]+osf)/sf)*sf/wf,2));
/*Labrelease factor*/
FLUXES[f+15]=(2/sqrt(pi))*(fl/wl)*exp(-pow(sin((DATA.MET[m+0]-pars[11]+osl)/sf)*sf/wl,2));
/*labile release - re-arrange order in next versions*/
// FLUXES[f+7] = POOLS[p+0]*(1-pow(1-FLUXES[f+15],deltat))/deltat;
FLUXES[f+7] = FLUXES[f+32];
/*leaf litter production*/       
// FLUXES[f+9] = POOLS[p+1]*(1-pow(1-FLUXES[f+8],deltat))/deltat;
FLUXES[f+9] = FLUXES[f+33];
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
        POOLS[nxp+1] = POOLS[p+1] + (FLUXES[f+3] - FLUXES[f+9])*deltat;
        // POOLS[nxp+1] = FLUXES[f+34]*pars[16];
        POOLS[nxp+2] = POOLS[p+2] + (FLUXES[f+5] - FLUXES[f+11])*deltat;
        POOLS[nxp+3] = POOLS[p+3] +  (FLUXES[f+6] - FLUXES[f+10])*deltat;
        POOLS[nxp+4] = POOLS[p+4] + (FLUXES[f+9] + FLUXES[f+11] - FLUXES[f+12] - FLUXES[f+14])*deltat; 
        POOLS[nxp+5]= POOLS[p+5]+ (FLUXES[f+14] - FLUXES[f+13]+FLUXES[f+10])*deltat;     
        POOLS[nxp+8] = FLUXES[f+34];

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
	/*Plant-available water ODE*/
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

	/*Fraction of C-foliar lost due to fires*/
    FLUXES[f+41] = DATA.MET[m+6]*(CF[1] + (1-CF[1])*(1-pars[30]));
	/*Calculate LAI (lambda) lost due to fire
	  - we lose the same fraction of LAI as we do C-foliar 
	  - FE_\Lambda^{(t+1)} = \Lambda^{(t+1)'} * BA ( k_{factor(i)} + (1 - k_{factor(i)}) r )*/
	FLUXES[f+39] = FLUXES[f+34]*DATA.MET[m+6]*(CF[1] + (1-CF[1])*(1-pars[30]));
	/*Subtract fire loss LAI from current LAI*/
	FLUXES[f+34] = FLUXES[f+34] - FLUXES[f+39];
  /*Update variables for LAI_KNORR to be used in next iteration*/
  /*plant-available water*/
  lai_var_list[17]=POOLS[nxp+6]; /*pasm = PAW*/
  /*evapotranspiration variables*/
  lai_var_list[18]=FLUXES[f+28]; /*ET*/
  /*current LAI*/
  lai_var_list[2]=FLUXES[f+34];//POOLS[nxp+9];
  /*temperature memory*/
  lai_var_list[5]=FLUXES[f+35];//POOLS[nxp+10];
  /*water/structural memory*/
  lai_var_list[11]=FLUXES[f+36];//POOLS[nxp+11];

}

return 0;
}







