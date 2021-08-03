#pragma once
//Note: DALEC_OBSERVATION_OPERATORS.c included in DALEC_MODULE.
#include "../../../DALEC_CODE/DALEC_ALL/DALEC_MODULE.c"
#include "../../../DALEC_CODE/DALEC_ALL/HYDROLOGY_MODULES/DRAINAGE.c"
#include "../../../DALEC_CODE/DALEC_ALL/HYDROLOGY_MODULES/CONVERTERS/HYDROFUN_EWT2MOI.c"
#include "../../../DALEC_CODE/DALEC_ALL/HYDROLOGY_MODULES/CONVERTERS/HYDROFUN_MOI2EWT.c"
#include "../../../DALEC_CODE/DALEC_ALL/HYDROLOGY_MODULES/CONVERTERS/HYDROFUN_MOI2CON.c"
#include "../../../DALEC_CODE/DALEC_ALL/HYDROLOGY_MODULES/CONVERTERS/HYDROFUN_MOI2PSI.c"
#include "../../../DALEC_CODE/DALEC_ALL/LIU_An_et.c"
#include "../../../DALEC_CODE/DALEC_ALL/LAI_KNORR.c"

/*Code used by Bloom et al., 2016
See also Bloom & Williams 2015,  Fox et al., 2009; Williams et al., 1997*/

int DALEC_1100_MODCONFIG(DALEC * DALECmodel){

DALECmodel->nopools=8;
DALECmodel->nomet=9;/*This should be compatible with CBF file, if not then disp error*/
DALECmodel->nopars=63;
DALECmodel->nofluxes=46;

//declaring observation operator structure, and filling with DALEC configurations
static OBSOPE OBSOPE;
//Initialize all SUPPORT OBS values (default value = false).
INITIALIZE_OBSOPE_SUPPORT(&OBSOPE);

//Set SUPPORT_OBS values to true if model supports observation operation.
printf("DALEC_1100_MODCONFIG, Line 22...\n");
OBSOPE.SUPPORT_GPP_OBS=true;
OBSOPE.SUPPORT_LAI_OBS=true;
OBSOPE.SUPPORT_ET_OBS=true;
OBSOPE.SUPPORT_NBE_OBS=true;
OBSOPE.SUPPORT_ABGB_OBS=true;
OBSOPE.SUPPORT_SOM_OBS=true;
OBSOPE.SUPPORT_GRACE_EWT_OBS=true;
OBSOPE.SUPPORT_FIR_OBS=true;

//Provide values required by each OBS operator
//Note: each OBS operator requirements are unique, see individual observation operator functions to see what's required 
//Note: no values required for any SUPPORT_*_OBS quantity set to false.

//GPP-specific variables
OBSOPE.GPP_flux=0;
//LAI-specific variables
OBSOPE.LAI_foliar_pool=1;
OBSOPE.LAI_LCMA=16;
//ET variabiles
OBSOPE.ET_flux=28;
//NBE-specific variables
static int NBE_fluxes[]={0,2,12,13,16};
OBSOPE.NBE_fluxes=NBE_fluxes;
static double NBE_flux_signs[]={-1.,1.,1.,1.,1.};
OBSOPE.NBE_flux_signs=NBE_flux_signs;
OBSOPE.NBE_n_fluxes=5;

//ABGB-specific variables
static int ABGB_pools[]={0,1,2,3}; 
OBSOPE.ABGB_pools=ABGB_pools;
OBSOPE.ABGB_n_pools=4;

//SOM-specific variables
static int SOM_pools[]={4,5}; 
OBSOPE.SOM_pools=SOM_pools;
OBSOPE.SOM_n_pools=2;
//H2O-specific variables
static int GRACE_EWT_h2o_pools[]={6,7};
OBSOPE.GRACE_EWT_h2o_pools=GRACE_EWT_h2o_pools;
OBSOPE.GRACE_EWT_n_h2o_pools=2;
//Fire-specific variables
OBSOPE.FIR_flux=16;

DALECmodel->OBSOPE=OBSOPE;

return 0;}



int DALEC_1100(DATA DATA, double const *pars)
{

/*C-pools, fluxes, meteorology indices*/
int p=0,f,m,nxp, i;
int n=0,nn=0;
double pi=3.1415927;
double lai_met_list[1],lai_var_list[20];

double deltat=DATA.ncdf_data.TIME_INDEX.values[1] - DATA.ncdf_data.TIME_INDEX.values[0];
int N_timesteps=DATA.ncdf_data.TIME_INDEX.length;


 double constants[10]={pars[10],0.0156935,4.22273,208.868,0.0453194,0.37836,7.19298, 0.011136,2.1001,0.789798};

/*Pointer transfer - all data stored in fluxes and pools will be passed to DATA*/
double *FLUXES=DATA.M_FLUXES;
double *POOLS=DATA.M_POOLS;
double *LAI=DATA.M_LAI;
// double *NEE=DATA.M_NEE;

  /*assigning values to pools*/
  /*L,F,R,W,Lit,SOM*/
  POOLS[0]=pars[17];
  POOLS[1]=pars[53]*pars[16];
  POOLS[2]=pars[19];
  POOLS[3]=pars[20];
  POOLS[4]=pars[21];
  POOLS[5]=pars[22];
  /*water pools*/
  POOLS[6]=pars[26];
  POOLS[7]=pars[35];

double *SSRD=DATA.ncdf_data.SSRD.values;
double *T2M_MIN=DATA.ncdf_data.T2M_MIN.values;
double *T2M_MAX=DATA.ncdf_data.T2M_MAX.values;
double *CO2=DATA.ncdf_data.CO2.values;
double *DOY=DATA.ncdf_data.DOY.values;
double *PREC=DATA.ncdf_data.TOTAL_PREC.values;
double *VPD=DATA.ncdf_data.VPD.values;
double *BURNED_AREA=DATA.ncdf_data.BURNED_AREA.values;
double *TIME_INDEX=DATA.ncdf_data.TIME_INDEX.values;

double meantemp = (DATA.ncdf_data.T2M_MAX.reference_mean + DATA.ncdf_data.T2M_MIN.reference_mean)/2;
double meanrad = DATA.ncdf_data.SSRD.reference_mean;
double meanprec = DATA.ncdf_data.TOTAL_PREC.reference_mean;

/* NOTES FOR POOLS AND FLUXES

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
  29. PAW runoff
  30. PUW->PAW transfer
  31. PUW runoff
  32. Surface runoff
  33. Transpiration
  34. Evaporation
  35. labile C  request LAI
  36. foliar  to litter
  37. LAI
  38. T_memory
  39. Lambda_max_memory
  40. dLambdadt
  41. labile C  availability flag
  42. LAI  fire loss
  43. foliar C  fire loss
  44. f (T)
  45. f (td)
*/


/*constants for exponents of leaffall and labrelease factors*/
/*width*/
double wf=pars[15]*sqrt(2)/2;
double wl=pars[13]*sqrt(2)/2;


/*factor*/
double ff=(log(pars[4])-log(pars[4]-1))/2;
/*double fl=(log(1.001)-log(0.001))/2;*/
double fl=(log(pars[31])-log(pars[31]-1))/2;


// Porosity scaling factor (see line 124 of HESS paper)
double psi_porosity = -0.117*9.8/1000;

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
double Fcfolavailable;

/*resilience factor*/


/*number of MET drivers*/
// int nomet=((DALEC *)DATA.MODEL)->nomet;

/*number of DALEC pools*/
int nopools=((DALEC *)DATA.MODEL)->nopools;

/*number of DALEC fluxes to store*/
int nofluxes=((DALEC *)DATA.MODEL)->nofluxes;


/*repeating loop for each timestep*/
for (n=0; n < N_timesteps; n++){
/*ppol index*/
p=nopools*n;
/*next pool index*/
nxp=nopools*(n+1);
/*met index*/
// m=nomet*n;
/*flux array index*/
f=nofluxes*n;



/*LAI*/
LAI[n]=POOLS[p+1]/pars[16]; 


/*Calculate light extinction coefficient*/
double B = (DOY[n]-81)*2*pi/365.;
double ET1 = 9.87*sin(2*B)-7.53*cos(B)-1.5*sin(B);
double DA = 23.45*sin((284+DOY[n])*2*pi/365); //Deviation angle
double LST = (int) (DOY[n]*24*60) % (24*60);
double AST = LST+ET1;
double h = (AST-12*60)/4; //hour angle
double alpha = asin((sin(pi/180*DATA.ncdf_data.LAT)*sin(pi/180*DA)+cos(pi/180*DATA.ncdf_data.LAT)*cos(pi/180.*DA)*cos(pi/180*h)))*180/pi; //solar altitude
double zenith_angle = 90-alpha;
double LAD = 1.0; //leaf angle distribution
double VegK = sqrt(pow(LAD,2)+ pow(tan(zenith_angle/180*pi),2))/(LAD+1.774*pow((1+1.182),-0.733)); //Campbell and Norman 1998


/*Temp scaling factor*/
double g;
int Tminmin = pars[47] - 273.15; 
int Tminmax = pars[48] - 273.15;
if( DATA.MET[m+1] < Tminmin ) {
    g=0;
}
else if (DATA.MET[m+1] > Tminmax) {
    g=1;
}
else {
    g=(DATA.MET[m+1] - Tminmin)/(Tminmax - Tminmin);
}

double vcmax25 = pars[46]; 
double g1 = pars[45]; 
double beta = fmin(POOLS[p+6]/pars[25],1);
double ga = pars[49]; 
double Tupp = pars[50]; 
double Tdown = pars[51]; 
double C3_frac = pars[52]; 

// GPP, T, and E from LIU_An_et
// Annual radiation, VPD in kPa, mean T in K
double *LIU_An_et_out = LIU_An_et(SSRD[n]*1e6/(24*3600), VPD[n]/10, 273.15+0.5*(T2M_MIN[n]+T2M_MAX[n]), vcmax25, CO2[n], beta, g1, LAI[n], ga, VegK, Tupp, Tdown, C3_frac)
// GPP
FLUXES[f+0] = LIU_An_et_out[0]*g;
//transpiration//
FLUXES[f+33] = LIU_An_et_out[1];
//evaporation//
FLUXES[f+34] = LIU_An_et_out[2];
// Evapotranspiration
FLUXES[f+28]=FLUXES[f+34]+FLUXES[f+33];

// Infiltration (mm/day)
double infil = pars[34]*(1 - exp(-PREC[n]/pars[34]));

// Surface runoff (mm/day)
FLUXES[f+32] = PREC[n] - infil;

// Update pools, including infiltration
POOLS[nxp+6] = POOLS[p+6] + deltat*infil;
POOLS[nxp+7] = POOLS[p+7];

// Volumetric soil moisture from water pools
double sm_PAW = HYDROFUN_EWT2MOI(POOLS[nxp+6],pars[39],pars[42]);
double sm_PUW = HYDROFUN_EWT2MOI(POOLS[nxp+7],pars[40],pars[43]);

// Update PAW SM with infiltration
//sm_PAW += HYDROFUN_EWT2MOI(infil*deltat,pars[39],pars[42]);

// Calculate drainage
double drain_PAW = DRAINAGE(sm_PAW,pars[44],-pars[41],psi_porosity,pars[24]);
double drain_PUW = DRAINAGE(sm_PUW,pars[44],-pars[41],psi_porosity,pars[24]);

// Drainage becomes runoff from pools
FLUXES[f+29] = HYDROFUN_MOI2EWT(drain_PAW,pars[39],pars[42])/deltat;
FLUXES[f+31] = HYDROFUN_MOI2EWT(drain_PUW,pars[40],pars[43])/deltat;

// Remove drainage from layers
sm_PAW -= drain_PAW;
sm_PUW -= drain_PUW;

// Convert to conductivity
double k_PAW = HYDROFUN_MOI2CON(sm_PAW,pars[33],pars[24]);
double k_PUW = HYDROFUN_MOI2CON(sm_PUW,pars[33],pars[24]);

// Convert to potential
double psi_PAW = HYDROFUN_MOI2PSI(sm_PAW,psi_porosity,pars[24]);
double psi_PUW = HYDROFUN_MOI2PSI(sm_PUW,psi_porosity,pars[24]);

// Calculate inter-pool transfer in m/s (positive is PUW to PAW)
double xfer = -1000 * sqrt(k_PAW*k_PUW) * (1000*(psi_PAW-psi_PUW)/(9.8*0.5*(pars[42]+pars[43])) + 1);

// Transfer flux in mm/day
FLUXES[f+30] = xfer*1000*3600*24;

// Update pools, including ET from PAW
POOLS[nxp+6] += (FLUXES[f+30] - FLUXES[f+29] - FLUXES[f+28])*deltat;
POOLS[nxp+7] += (-FLUXES[f+30] - FLUXES[f+31])*deltat;


//KNORR LAI
if (n==0){
  /*Initialize phenology memory of air-temperature */
  lai_var_list[5]=pars[37]+3*pars[38];
  /*Initialize phenology memory of water/structural limitation */
  lai_var_list[11]=pars[42];
}
lai_met_list[0]=(DATA.MET[m+2] + DATA.MET[m+1])/2.0; /* meantemp, deg C*/
lai_var_list[0]=n; /*current timestep index of model run*/
lai_var_list[19]=deltat; /*time increment of model run*/
lai_var_list[1]=LAI[n]; /*initial LAI parameter*/
lai_var_list[2]=LAI[n];
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
/*Put the plant-available soil water into the LAI_KNORR input list*/
lai_var_list[17]=(POOLS[p+6]+POOLS[nxp+6])/2.0;
/*Put the evapotranspiration flux into the LAI_KNORR input list*/
lai_var_list[18]=FLUXES[f+28];
// Run KNORR LAI module
// - this computes a potential LAI
double *LAI_KNORR_out = LAI_KNORR(lai_met_list, lai_var_list)
FLUXES[f+37] = LAI_KNORR_out[0];  // LAI (environmental target)
FLUXES[f+38] = LAI_KNORR_out[1];  // T_memory
FLUXES[f+39] = LAI_KNORR_out[2];  // lambda_max_memory
FLUXES[f+40] = LAI_KNORR_out[3]/deltat;  // dlambda/dt (units: LAI per day)
FLUXES[f+44] = LAI_KNORR_out[4];  // fraction of plants above temperature threshold
FLUXES[f+45] = LAI_KNORR_out[5];  // fraction of plants above day length threshold
/*Update environmental memory variables for next iteration*/
/*temperature memory*/
lai_var_list[5]=FLUXES[f+38];//POOLS[nxp+10];
/*water/structural memory*/
lai_var_list[11]=FLUXES[f+39];//POOLS[nxp+11];


/*temprate - now comparable to Q10 - factor at 0C is 1*/
/* x (1 + a* P/P0)/(1+a)*/
FLUXES[f+1]=exp(pars[9]*0.5*(T2M_MIN[n]+T2M_MAX[n]-meantemp))*((PREC[n]/meanprec-1)*pars[32]+1);
/*respiration auto*/
FLUXES[f+2]=pars[1]*FLUXES[f+0];
/*leaf production*/
FLUXES[f+3]=0;
/*labile production*/
/* - now the combination of two fractional allocation parameters*/
FLUXES[f+4] = (FLUXES[f+0]-FLUXES[f+2])*(pars[12]+pars[2]);
Fcfolavailable=FLUXES[f+4] + POOLS[p+0]/deltat;
if (FLUXES[f+40]*pars[16] > Fcfolavailable){
  /* flag for carbon availability limitation (0=canopy in senescence, 1=labile C does not limit growth, 2=labile C limits LAI growth) */
  FLUXES[f+41]=2.0;
  /* flux from labile pool to foliar pool */
  FLUXES[f+35]=Fcfolavailable;
  /* flux from foliar pool to litter pool */
  FLUXES[f+36]=0;
}
else if (FLUXES[f+40]*pars[16] < Fcfolavailable && FLUXES[f+40]*pars[16] > 0){
  FLUXES[f+41]=1.0;
  /* flux from labile pool to foliar pool */
  FLUXES[f+35]=FLUXES[f+40]*pars[16];
  /* flux from foliar pool to litter pool */
  FLUXES[f+36]=0;
}
else {
  FLUXES[f+41]=0.0;
  /* flux from labile pool to foliar pool */
  FLUXES[f+35]=0;
  /* flux from foliar pool to litter pool */
  FLUXES[f+36]=-FLUXES[f+37]*pars[16];
}

/*root production*/        
FLUXES[f+5] = (FLUXES[f+0]-FLUXES[f+2]-FLUXES[f+3]-FLUXES[f+4])*pars[4-1];            
/*wood production*/       
FLUXES[f+6] = FLUXES[f+0]-FLUXES[f+2]-FLUXES[f+3]-FLUXES[f+5]-FLUXES[f+4]; 
/*leaf fall factor*/
FLUXES[f+8] = (2/sqrt(pi))*(ff/wf)*exp(-pow(sin((TIME_INDEX[n]-pars[14]+osf)/sf)*sf/wf,2));
/*Labrelease factor*/
FLUXES[f+15]=(2/sqrt(pi))*(fl/wl)*exp(-pow(sin((TIME_INDEX[n]-pars[11]+osl)/sf)*sf/wl,2));
/*labile release - re-arrange order in next versions*/
FLUXES[f+7] = FLUXES[f+35];
/*leaf litter production*/
FLUXES[f+9] = FLUXES[f+36];
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
        POOLS[nxp+1] = POOLS[p+1] + (FLUXES[f+7] - FLUXES[f+9])*deltat;
        POOLS[nxp+2] = POOLS[p+2] + (FLUXES[f+5] - FLUXES[f+11])*deltat;
        POOLS[nxp+3] = POOLS[p+3] +  (FLUXES[f+6] - FLUXES[f+10])*deltat;
        POOLS[nxp+4] = POOLS[p+4] + (FLUXES[f+9] + FLUXES[f+11] - FLUXES[f+12] - FLUXES[f+14])*deltat; 
        POOLS[nxp+5]= POOLS[p+5]+ (FLUXES[f+14] - FLUXES[f+13]+FLUXES[f+10])*deltat;                    


	/*total pool transfers - WITH FIRES*/
	/*first fluxes*/

	/*CFF = Combusted C fire flux
	NCFF = Non-combusted C fire flux*/

	/*Calculating all fire transfers (1. combustion, and 2. litter transfer)*/
	/*note: all fluxes are in gC m-2 day-1*/
	for (nn=0;nn<6;nn++){FLUXES[f+17+nn] = POOLS[nxp+nn]*BURNED_AREA[n]*CF[nn]/deltat;}
	for (nn=0;nn<4;nn++){FLUXES[f+23+nn] = POOLS[nxp+nn]*BURNED_AREA[n]*(1-CF[nn])*(1-pars[30])/deltat;}

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


   /*Fraction of C-foliar lost due to fires*/
    FLUXES[f+43] = BURNED_AREA[n]*(CF[1] + (1-CF[1])*(1-pars[30]));
   /*Calculate LAI (lambda) lost due to fire
     - we lose the same fraction of LAI as we do C-foliar 
     - FE_\Lambda^{(t+1)} = \Lambda^{(t+1)'} * BA ( k_{factor(i)} + (1 - k_{factor(i)}) r )*/
   FLUXES[f+42] = FLUXES[f+37]*BURNED_AREA[n]*(CF[1] + (1-CF[1])*(1-pars[30]));
   /*Subtract fire loss LAI from current LAI*/
   FLUXES[f+37] = FLUXES[f+37] - FLUXES[f+42];
  
}

/*LAI is a CARDAMOM-wide state variable, ensuring available at first/last timestep in general (LAI) form, rather than only as "Cfol/LCMA"*/
LAI[n+1]=POOLS[nxp+1]/pars[16];

return 0;
}






