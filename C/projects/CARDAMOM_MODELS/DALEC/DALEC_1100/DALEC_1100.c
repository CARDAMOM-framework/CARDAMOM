#pragma once
//Note: DALEC_OBSERVATION_OPERATORS.c included in DALEC_MODULE.
#include "DALEC_1100_INDICES.c"
#include "PARS_INFO_1100.c"
#include "../DALEC_ALL/DALEC_MODULE.c"//Includes all calls to DALEC-compatible EDC functions
#include "../DALEC_ALL/HYDROLOGY_MODULES/DRAINAGE.c"
#include "../DALEC_ALL/HYDROLOGY_MODULES/CONVERTERS/HYDROFUN_EWT2MOI.c"
#include "../DALEC_ALL/HYDROLOGY_MODULES/CONVERTERS/HYDROFUN_MOI2EWT.c"
#include "../DALEC_ALL/HYDROLOGY_MODULES/CONVERTERS/HYDROFUN_MOI2CON.c"
#include "../DALEC_ALL/HYDROLOGY_MODULES/CONVERTERS/HYDROFUN_MOI2PSI.c"
#include "../DALEC_ALL/LIU_AN_ET_REFACTOR.c"
#include "../DALEC_ALL/CH4_MODULES/HET_RESP_RATES_JCR.c"
#include "../DALEC_ALL/KNORR_ALLOCATION.c"
#include "../DALEC_ALL/SOIL_TEMP_AND_LIQUID_FRAC.c"
#include "../DALEC_ALL/H2O_TEMP_AND_LIQUID_FRAC.c"
#include "../DALEC_ALL/INITIALIZE_INTERNAL_SOIL_ENERGY.c"
#include "../DALEC_ALL/INTERNAL_ENERGY_PER_LIQUID_H2O_UNIT_MASS.c"
#include "../DALEC_ALL/INTERNAL_ENERGY_PER_H2O_UNIT_MASS.c"
#include "../DALEC_ALL/ALLOC_AND_AUTO_RESP_FLUXES.c"


//Returns structure with sources and sinks, matches number of fluxes
int DALEC_1100_FLUX_SOURCES_SINKS(DALEC * DALECmodel){
    

    struct DALEC_1100_FLUXES F=DALEC_1100_FLUXES;
    struct DALEC_1100_POOLS S=DALEC_1100_POOLS;

    // /Step 1. Declare & initialize 
 DALEC_FLUX_SOURCE_SINK_MATRIX   FIOMATRIX;
    // external source or pool sink, or not conserved quantity
    //Default = -1 
   
    FIOMATRIX.SOURCE=calloc(DALECmodel->nofluxes, sizeof(int));
    FIOMATRIX.SINK=calloc(DALECmodel->nofluxes, sizeof(int));
     
    
    int n;
    for (n=0;n<DALECmodel->nofluxes; n++){FIOMATRIX.SOURCE[n]=-1;FIOMATRIX.SINK[n]=-1;}
    
    
    //Step 2. Define
//     
// 
//         // C_lab
//         FIOMATRIX.SINK[F.gpp]=S.C_lab;
//         FIOMATRIX.SOURCE[F.resp_auto_maint]=S.C_lab;
//         FIOMATRIX.SOURCE[F.foliar_prod]=S.C_lab;
//         FIOMATRIX.SOURCE[F.root_prod]=S.C_lab;
//         FIOMATRIX.SOURCE[F.wood_prod]=S.C_lab;
//         FIOMATRIX.SOURCE[F.resp_auto_growth]=S.C_lab;
//         FIOMATRIX.SOURCE[F.f_lab]=S.C_lab;  
//         FIOMATRIX.SOURCE[F.fx_lab2lit]=S.C_lab;
// 
// 
//         // C_fol
//         FIOMATRIX.SINK[F.foliar_prod]=S.C_fol;
//         FIOMATRIX.SOURCE[F.fol2lit]=S.C_fol;
//         FIOMATRIX.SOURCE[F.f_fol]=S.C_fol;
//         FIOMATRIX.SOURCE[F.fx_fol2lit]=S.C_fol;
//         
//         // C_roo
//         FIOMATRIX.SINK[F.root_prod]=S.C_roo;
//         FIOMATRIX.SOURCE[F.roo2lit]=S.C_roo;
//         FIOMATRIX.SOURCE[F.f_roo]=S.C_roo;
//         FIOMATRIX.SOURCE[F.fx_roo2lit]=S.C_roo;
//         
//         // C_woo
//         FIOMATRIX.SINK[F.wood_prod]=S.C_woo;
//         FIOMATRIX.SOURCE[F.woo2cwd]=S.C_woo;
//         FIOMATRIX.SOURCE[F.f_woo]=S.C_woo;
//         FIOMATRIX.SOURCE[F.fx_woo2cwd]=S.C_woo;
// 
//         
//         // C_lit
//         FIOMATRIX.SINK[F.fx_lab2lit]=S.C_lit;
//         FIOMATRIX.SINK[F.fol2lit]=S.C_lit;
//         FIOMATRIX.SINK[F.fx_fol2lit]=S.C_lit;
//         FIOMATRIX.SINK[F.roo2lit]=S.C_lit;
//         FIOMATRIX.SINK[F.fx_roo2lit]=S.C_lit;
//         FIOMATRIX.SOURCE[F.ae_rh_lit]=S.C_lit;
//         FIOMATRIX.SOURCE[F.an_rh_lit]=S.C_lit;
//         FIOMATRIX.SOURCE[F.f_lit]=S.C_lit;
//         FIOMATRIX.SOURCE[F.lit2som]=S.C_lit;
//         FIOMATRIX.SOURCE[F.fx_lit2som]=S.C_lit;
// 
//         // C_cwd
//         FIOMATRIX.SINK[F.woo2cwd]=S.C_cwd;
//         FIOMATRIX.SINK[F.fx_woo2cwd]=S.C_cwd;
//         FIOMATRIX.SOURCE[F.ae_rh_cwd]=S.C_cwd;
//         FIOMATRIX.SOURCE[F.an_rh_cwd]=S.C_cwd;
//         FIOMATRIX.SOURCE[F.f_cwd]=S.C_cwd;
//         FIOMATRIX.SOURCE[F.cwd2som]=S.C_cwd;
//         FIOMATRIX.SOURCE[F.fx_cwd2som]=S.C_cwd;
// 
//         // C_som
//         FIOMATRIX.SINK[F.cwd2som]=S.C_som;
//         FIOMATRIX.SINK[F.fx_cwd2som]=S.C_som;        
//         FIOMATRIX.SINK[F.lit2som]=S.C_som;
//         FIOMATRIX.SINK[F.fx_lit2som]=S.C_som;
//         FIOMATRIX.SOURCE[F.ae_rh_som]=S.C_som;
//         FIOMATRIX.SOURCE[F.an_rh_som]=S.C_som;
//         FIOMATRIX.SOURCE[F.f_som]=S.C_som;

        // H2O_SWE
        FIOMATRIX.SOURCE[F.melt]=S.H2O_SWE;
        FIOMATRIX.SOURCE[F.sublimation]=S.H2O_SWE;
        FIOMATRIX.SINK[F.snowfall]=S.H2O_SWE;


        // E_SWE
        FIOMATRIX.SOURCE[F.e_melt]=S.E_SWE;
        FIOMATRIX.SOURCE[F.e_sublimation]=S.E_SWE;
        FIOMATRIX.SINK[F.snow_heat]=S.E_SWE;
        FIOMATRIX.SINK[F.e_snowfall]=S.E_SWE;
// 
//         // H2O_PAW
//         FIOMATRIX.SINK[F.infil]=S.H2O_PAW;
//         FIOMATRIX.SOURCE[F.et]=S.H2O_PAW;
//         FIOMATRIX.SOURCE[F.paw2puw]=S.H2O_PAW;
//         FIOMATRIX.SOURCE[F.q_paw]=S.H2O_PAW;
// 
//         // H2O_PUW        
//         FIOMATRIX.SOURCE[F.q_puw]=S.H2O_PUW;
//         FIOMATRIX.SINK[F.paw2puw]=S.H2O_PUW;
// 
//         // E_PAW
//         FIOMATRIX.SINK[F.gh_in]=S.E_PAW;
//         FIOMATRIX.SINK[F.infil_e]=S.E_PAW;
//         FIOMATRIX.SOURCE[F.et_e]=S.E_PAW;
//         FIOMATRIX.SOURCE[F.q_paw_e]=S.E_PAW;
//         FIOMATRIX.SOURCE[F.paw2puw_e]=S.E_PAW;
//         FIOMATRIX.SOURCE[F.paw2puw_th_e]=S.E_PAW;
// 
//         // E_PUW
//         FIOMATRIX.SINK[F.paw2puw_e]=S.E_PUW;
//         FIOMATRIX.SINK[F.geological]=S.E_PUW;
//         FIOMATRIX.SINK[F.paw2puw_th_e]=S.E_PUW;
//         FIOMATRIX.SOURCE[F.q_puw_e]=S.E_PUW;


        
DALECmodel->FIOMATRIX = FIOMATRIX;
            

//DALEC flux indices    

//Calculate State source sink matrix
DALEC_STATE_SOURCE_SINK_MATRIX_CONFIG(DALECmodel);
return 0;

}



    
    
    



int DALEC_1100(DATA DATA, double const *pars){

    



struct DALEC_1100_PARAMETERS P=DALEC_1100_PARAMETERS;
struct DALEC_1100_FLUXES F=DALEC_1100_FLUXES;
struct DALEC_1100_POOLS S=DALEC_1100_POOLS;


    
double *SSRD=DATA.ncdf_data.SSRD.values;
double *T2M_MIN=DATA.ncdf_data.T2M_MIN.values;
double *T2M_MAX=DATA.ncdf_data.T2M_MAX.values;
double *CO2=DATA.ncdf_data.CO2.values;
double *DOY=DATA.ncdf_data.DOY.values;
double *PREC=DATA.ncdf_data.TOTAL_PREC.values;
double *VPD=DATA.ncdf_data.VPD.values;
double *BURNED_AREA=DATA.ncdf_data.BURNED_AREA.values;
double *TIME_INDEX=DATA.ncdf_data.TIME_INDEX.values;
double *SNOWFALL=DATA.ncdf_data.SNOWFALL.values;
double *SKT=DATA.ncdf_data.SKT.values;
double *STRD=DATA.ncdf_data.STRD.values;

/*C-pools, fluxes, meteorology indices*/
int p=0,f,m,nxp, i;
int n=0,nn=0;
//double pi=3.1415927;
double pi=DGCM_PI;

//PREDERIVED TERMS 

    double PREDERIVED_GEO_FLUX=0.105*3600*24;


double deltat=DATA.ncdf_data.TIME_INDEX.values[1] - DATA.ncdf_data.TIME_INDEX.values[0];
double one_over_deltat=1/deltat;
 
int N_timesteps=DATA.ncdf_data.TIME_INDEX.length;


/*Pointer transfer - all data stored in fluxes and pools will be passed to DATA*/
double *FLUXES=DATA.M_FLUXES;
double *POOLS=DATA.M_POOLS;

 


  POOLS[S.H2O_SWE]=pars[P.i_SWE_H2O];
  POOLS[S.E_SWE]=pars[P.i_SWE_H2O]*pars[P.i_SWE_E];
    


    
    

    //POOLS[S.D_SCF]=POOLS[S.H2O_SWE]/(POOLS[S.H2O_SWE]+pars[P.scf_scalar]);} //snow cover fraction gen1
      double smr1=fmin(1.0,POOLS[S.H2O_SWE]/pars[P.SWEmax]); //___scf_gen3
      POOLS[S.D_SCF]=1-pow((1/pi*acos(2*smr1-1)),pars[P.Nmelt]); //___scf_gen3


// 
//         //Diagnostic time-invariant quantities
//         // Porosity scaling factor (see line 124 of HESS paper)
//         double psi_porosity = -0.117/100;
//         double PAWmax=pars[P.PAW_por]*pars[P.PAW_z]*1000; //PAW capacity in mm
//         double PUWmax=pars[P.PUW_por]*pars[P.PUW_z]*1000; //PUW capacity in mm
//         
//         

// 
// 
//         
//     //Declare stryct
     H2O_TEMP_AND_LIQUID_FRAC_STRUCT SWETEMP ,  SWETEMP2,  SWETEMP3,  SWETEMP4;
//   //Populate with run-specific constrants
//     //PAW
    SWETEMP.IN.h2o=POOLS[S.H2O_SWE]; //mm
     SWETEMP.IN.internal_energy = POOLS[S.E_SWE];//m 
//     PAWSOILTEMP.IN.soil_water = POOLS[S.H2O_PAW];//mm (or kg/m2)
//     PAWSOILTEMP.IN.internal_energy = POOLS[S.E_PAW];//Joules
//     //Pass pointer to function 
     H2O_TEMP_AND_LIQUID_FRAC(&SWETEMP);  //Outputs are in K
// 
//         //Store outputs 
     POOLS[S.D_TEMP_SWE]=SWETEMP.OUT.TEMP;  //In K  
//     
//     

// /*number of DALEC pools*/
int nopools=((DALEC *)DATA.MODEL)->nopools;
// 
// /*number of DALEC fluxes to store*/
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
// 



    //Initializing everything

    FLUXES[f+F.snow_heat]=0;
        FLUXES[f+F.snowfall]=0;
        FLUXES[f+F.e_snowfall]=0;
        FLUXES[f+F.melt]=0;
        FLUXES[f+F.e_melt]=0;
        FLUXES[f+F.sublimation]=0;
        FLUXES[f+F.e_sublimation]=0;
        
    int nosnow=0;

    
    double air_temp_k = DGCM_TK0C+0.5*(T2M_MIN[n]+T2M_MAX[n]);


    double tskin_k = SKT[n]+DGCM_TK0C;



////STEP 1.  ADD ALL INPUTS AND UPDATE STATES/////
/*Snow water equivalent*/
FLUXES[f+F.snowfall] = SNOWFALL[n];
FLUXES[f+F.e_snowfall]=FLUXES[f+F.snowfall]*INTERNAL_ENERGY_PER_H2O_UNIT_MASS(fmin(air_temp_k,DGCM_TK0C), 0);

    double RAIN_IN_SNOW=(PREC[n]-SNOWFALL[n])*POOLS[p+S.D_SCF];
    double E_RAIN_IN_SNOW=INTERNAL_ENERGY_PER_H2O_UNIT_MASS(fmax(air_temp_k,DGCM_TK0C), 1)*RAIN_IN_SNOW;

//Appending snow with these variables
    FLUXES[f+F.snowfall] += RAIN_IN_SNOW;
    FLUXES[f+F.e_snowfall] += E_RAIN_IN_SNOW;

double H2O_SWE_PLUS_INPUTS=POOLS[p+S.H2O_SWE]+FLUXES[f+F.snowfall]*deltat; /*first step snowfall to SWE*/
double E_SWE_PLUS_INPUTS=POOLS[p+S.E_SWE]+(FLUXES[f+F.e_snowfall])*deltat;


if (H2O_SWE_PLUS_INPUTS>0){
     SWETEMP.IN.h2o=H2O_SWE_PLUS_INPUTS; ;//mm
     SWETEMP.IN.internal_energy =E_SWE_PLUS_INPUTS;//m 
    H2O_TEMP_AND_LIQUID_FRAC(&SWETEMP);  //Outputs are in K
}
else 
{SWETEMP.OUT.TEMP=DGCM_TK0C;
        SWETEMP.OUT.LF=0;}



    double PREC_INDUCED_MELT=0;
    double E_PREC_INDUCED_MELT=0;

    if (SWETEMP.OUT.LF==1){
    PREC_INDUCED_MELT =H2O_SWE_PLUS_INPUTS/deltat;
    E_PREC_INDUCED_MELT =E_SWE_PLUS_INPUTS/deltat;
    nosnow=1;
        H2O_SWE_PLUS_INPUTS=0;
        E_SWE_PLUS_INPUTS=0;
        
 
    
    }

    else if (SWETEMP.OUT.LF>0){
    PREC_INDUCED_MELT=H2O_SWE_PLUS_INPUTS*SWETEMP.OUT.LF/deltat;
    H2O_SWE_PLUS_INPUTS=H2O_SWE_PLUS_INPUTS*(1-SWETEMP.OUT.LF);
    E_PREC_INDUCED_MELT=INTERNAL_ENERGY_PER_H2O_UNIT_MASS(DGCM_TK0C, 1)*PREC_INDUCED_MELT;
    E_SWE_PLUS_INPUTS= E_SWE_PLUS_INPUTS- E_PREC_INDUCED_MELT*deltat;
    }




    /*first step snowfall to SWE*/
//transient_SCF
    //double SCFtemp = POOLS[nxp+S.H2O_SWE]/(POOLS[nxp+S.H2O_SWE]+pars[P.scf_scalar]);
double smr2=fmin(1.0,H2O_SWE_PLUS_INPUTS/pars[P.SWEmax]); //___scf_gen3
double SCF_PLUS_INPUTS = 1-pow((1/pi*acos(2*smr2-1)),pars[P.Nmelt]); //___scf_gen3






    //Step 2. Calculate heat fluxes 
    
    if (H2O_SWE_PLUS_INPUTS>0){
        
        //Calculate new SWE temp



        //Calculate energy flux
        FLUXES[f+F.snow_heat] = SCF_PLUS_INPUTS*(pars[P.thermal_cond_swe]* (tskin_k - SWETEMP.OUT.TEMP)/(H2O_SWE_PLUS_INPUTS*1e-3*0.5))*DGCM_SEC_DAY;


        //Max energy allowed to induce complete melt
        double MAX_ENERGY= INTERNAL_ENERGY_PER_H2O_UNIT_MASS(DGCM_TK0C, 1)*H2O_SWE_PLUS_INPUTS;


        //Min energy required to equilibrate with LST
        double MIN_ENERGY= INTERNAL_ENERGY_PER_H2O_UNIT_MASS(tskin_k, 0)*H2O_SWE_PLUS_INPUTS;

// 
// 
//         printf("SCF_PLUS_INPUTS = %2.2f\n", SCF_PLUS_INPUTS);
// printf(" FLUXES[f+F.snow_heat] = %2.2f\n", FLUXES[f+F.snow_heat]);
//         printf("MAX_ENERGY = %2.2f\n",MAX_ENERGY);
//         printf("SWETEMP.OUT.TEMP = %2.2f\n",SWETEMP.OUT.TEMP);

        if ((E_SWE_PLUS_INPUTS+FLUXES[f+F.snow_heat] *deltat)>MAX_ENERGY){


        //Cap energy to SWETEMP = 0C @ LF = 1 to ensure energy budget closed.


        //POOLS[nxp+S.E_SWE]=MAX_ENERGY ;
     FLUXES[f+F.snow_heat]= (MAX_ENERGY-E_SWE_PLUS_INPUTS)/deltat;

        SWETEMP.OUT.TEMP=DGCM_TK0C;
        SWETEMP.OUT.LF=1;
            nosnow=1;
//            printf("nosnow0\n");
        
        
        }
        else if ((E_SWE_PLUS_INPUTS+FLUXES[f+F.snow_heat] *deltat)<MIN_ENERGY & tskin_k<DGCM_TK0C){



        //POOLS[nxp+S.E_SWE]=MAX_ENERGY ;
     FLUXES[f+F.snow_heat]= (MIN_ENERGY-E_SWE_PLUS_INPUTS)/deltat;

        SWETEMP.OUT.TEMP=tskin_k;
        SWETEMP.OUT.LF=0;
}

       

            else

        {
        //Recalculate SWE temp
             SWETEMP.IN.h2o=H2O_SWE_PLUS_INPUTS; ;//mm
     SWETEMP.IN.internal_energy = E_SWE_PLUS_INPUTS+FLUXES[f+F.snow_heat]*deltat;//m 
    H2O_TEMP_AND_LIQUID_FRAC(&SWETEMP);  //Outputs are in K


        }



    //Only taking following steps if LF>0
    if (SWETEMP.OUT.LF>0){
        
        //Snowmelt H2O
     FLUXES[f+F.melt] = H2O_SWE_PLUS_INPUTS*SWETEMP.OUT.LF/deltat;
    //Snowmelt energy
    FLUXES[f+F.e_melt] = FLUXES[f+F.melt] * INTERNAL_ENERGY_PER_H2O_UNIT_MASS(DGCM_TK0C, 1.0);}
    else
    { FLUXES[f+F.melt] =0;
    FLUXES[f+F.e_melt] =0;}




        
          
//Only calculate sublimation if snow hasn't 100% melted
    if (nosnow==0 & SWETEMP.OUT.TEMP<DGCM_TK0C){



//Remove sublimation next
    //double SUBLIMATIONpotential =  pars[P.sublimation_rate]*SSRD[n]*SCF_PLUS_INPUTS*exp((tskin_k - DGCM_TK0C)* pars[P.snow_sub_temp_exp])*(1-SWETEMP.OUT.LF);

    double SUBLIMATIONpotential =  pars[P.sublimation_rate]*SSRD[n]*SCF_PLUS_INPUTS*(1-SWETEMP.OUT.LF);



        //Calculate sublimation as residual
    if ((H2O_SWE_PLUS_INPUTS - deltat*(   FLUXES[f+F.melt] + SUBLIMATIONpotential))<0.0){
        FLUXES[f+F.sublimation]=H2O_SWE_PLUS_INPUTS/deltat -    FLUXES[f+F.melt];
    nosnow=1;
        //printf("nosnow1\n");
    }
        else{
        FLUXES[f+F.sublimation]=SUBLIMATIONpotential;}

//Sublimation energy, assumes at temp of snow
            //printf("Sublimation temp, LF = %2.2f, %2.2f\n",SWETEMP.OUT.TEMP,SWETEMP.OUT.LF);


// printf("SWETEMP, LF before  melt= %2.2e, %2.2e\n",SWETEMP.OUT.TEMP,SWETEMP.OUT.LF);
//         SWETEMP.IN.h2o=H2O_SWE_PLUS_INPUTS-FLUXES[f+F.melt] *deltat; ;//mm
//      SWETEMP.IN.internal_energy =E_SWE_PLUS_INPUTS+FLUXES[f+F.snow_heat]*deltat-FLUXES[f+F.e_melt] *deltat ;//m  ;//m 
//     H2O_TEMP_AND_LIQUID_FRAC(&SWETEMP);  //Outputs are in K
//         printf("SWETEMP after melt = %2.2e %2.2e\n",SWETEMP.OUT.TEMP,SWETEMP.OUT.LF);


//         double E1 = INTERNAL_ENERGY_PER_H2O_UNIT_MASS( SWETEMP.OUT.TEMP,  SWETEMP.OUT.LF)*(FLUXES[f+F.sublimation]+FLUXES[f+F.melt])*deltat;
//         double E2 = INTERNAL_ENERGY_PER_H2O_UNIT_MASS( SWETEMP.OUT.TEMP, 1)*FLUXES[f+F.melt]*deltat;
//         double E3 = INTERNAL_ENERGY_PER_H2O_UNIT_MASS( SWETEMP.OUT.TEMP, 0)*FLUXES[f+F.sublimation]*deltat;

//         if (n==5){
//         printf("E1= %2.10e\n",E1);
//         printf("E2= %2.10e\n",E2);
//         printf("E3 = %2.10e\n",E3);
//         printf("DGCM_TK0C -  SWETEMP.OUT.TEMP = %2.10f\n", DGCM_TK0C - SWETEMP.OUT.TEMP);}


//Add up melt + sublimation fluxes




FLUXES[f+F.e_sublimation] = FLUXES[f+F.sublimation] * INTERNAL_ENERGY_PER_H2O_UNIT_MASS( SWETEMP.OUT.TEMP, 0);

   


//capping sublimation energy loss to minimize numerical rounding errors




    }

        else
        {
            FLUXES[f+F.sublimation]=0;
            FLUXES[f+F.e_sublimation]=0;}



        //Update states here
FLUXES[f+F.melt] +=    PREC_INDUCED_MELT;
FLUXES[f+F.e_melt] +=   E_PREC_INDUCED_MELT;
        

        if (nosnow==1){
       POOLS[nxp+S.H2O_SWE] =0;
            POOLS[nxp+S.E_SWE] = 0;}
        else
        {
       POOLS[nxp+S.H2O_SWE] =POOLS[p+S.H2O_SWE]+ (FLUXES[f+F.snowfall] -  FLUXES[f+F.melt] -  FLUXES[f+F.sublimation])*deltat;
       POOLS[nxp+S.E_SWE] =  POOLS[p+S.E_SWE] +   (FLUXES[f+F.e_snowfall] + FLUXES[f+F.snow_heat] -  FLUXES[f+F.e_melt] -  FLUXES[f+F.e_sublimation])*deltat;
        
    if       ( POOLS[nxp+S.H2O_SWE] <0){
            POOLS[nxp+S.H2O_SWE] =0;
            POOLS[nxp+S.E_SWE]=0;
            nosnow=1;
        //printf("nosnow2\n");

        }
        
        }
//          if(n<2){
//          printf("*****%i-energy******\n",n);
// 
//                  printf("SWE  E t=0= %2.10e\n",POOLS[p+S.E_SWE]);
//          printf("SWE E t+1= %2.10e\n",POOLS[nxp+S.E_SWE]);
//      printf("Melt E t= %2.10e\n",FLUXES[f+F.e_melt]*deltat);
//      printf("Sublimation E t= %2.10e\n",FLUXES[f+F.e_sublimation]*deltat);
//     printf("Snowfall E t= %2.10e\n",FLUXES[f+F.e_snowfall]*deltat);
//     printf("Heat flux E t= %2.10e\n",FLUXES[f+F.snow_heat]*deltat);}

        //Ensuring all pools remain numerically zero or positive.




    }
    else
    {
        nosnow=1;
        //printf("nosnow3\n");
        FLUXES[f+F.e_sublimation] = 0;
        FLUXES[f+F.sublimation] = 0;
        FLUXES[f+F.e_melt] = 0;
        FLUXES[f+F.melt] = 0;
        FLUXES[f+F.snow_heat] = 0;
        POOLS[nxp+S.E_SWE]=0;
        POOLS[nxp+S.H2O_SWE]=0;
    
    }


//     /****************************RECORD t+1 DIAGNOSTIC STATES*************************/
//     POOLS[nxp+S.D_LAI]=POOLS[nxp+S.C_fol]/pars[P.LCMA]; //LAI
//    POOLS[nxp+S.D_SCF]=POOLS[nxp+S.H2O_SWE]/(POOLS[nxp+S.H2O_SWE]+pars[P.scf_scalar]); //snow cover fraction
    
       double smr3=fmin(1.0,POOLS[nxp+S.H2O_SWE]/pars[P.SWEmax]);
    POOLS[nxp+S.D_SCF]=1-pow((1/pi*acos(2*smr3-1)),pars[P.Nmelt]); //snow cover fraction gen3
        
    if (    nosnow==0){

//___scf_gen3 start
//    POOLS[nxp+S.D_SCF]=POOLS[nxp+S.H2O_SWE]/(POOLS[nxp+S.H2O_SWE]+pars[P.scf_scalar]); //snow cover fraction gen1

//___scf_gen3 end  

    SWETEMP.IN.h2o = POOLS[nxp+S.H2O_SWE];//mm (or kg/m2)
    SWETEMP.IN.internal_energy = POOLS[nxp+S.E_SWE];//Joules
   H2O_TEMP_AND_LIQUID_FRAC(&SWETEMP);
   POOLS[nxp+S.D_TEMP_SWE]=SWETEMP.OUT.TEMP;}//In K
    else
    {   POOLS[nxp+S.D_TEMP_SWE] =DGCM_TK0C;}

// //         
//       if(n>1 & n<8){
//          printf("*****%i-final******\n",n);
//              if (      POOLS[nxp+S.H2O_SWE]>0){printf("***POOLS[nxp+S.H2O_SWE]>0***\n");}
//          printf("SWE t=0= %2.10e\n",POOLS[p+S.H2O_SWE]);
//          printf("SWE t+1= %2.10e\n",POOLS[nxp+S.H2O_SWE]);
//      printf("Snowfall t= %2.10e\n",FLUXES[f+F.snowfall]*deltat);
//      printf("SNow temp final, lf= %2.2e\n",POOLS[nxp+S.D_TEMP_SWE]);
//      printf("Melt t= %2.10e\n",FLUXES[f+F.melt]*deltat);
//      printf("Sublimation t= %2.10e\n",FLUXES[f+F.sublimation]*deltat);
//     printf("SWE  E t=0= %2.10e\n",POOLS[p+S.E_SWE]);
//     printf("SWE E t+1= %2.10e\n",POOLS[nxp+S.E_SWE]);
//           printf("dSWE  E t=0= %2.10e\n",POOLS[nxp+S.E_SWE]-POOLS[p+S.E_SWE]);
//                     printf("dSWE E flux sum = %2.10e\n", (FLUXES[f+F.snow_heat] + FLUXES[f+F.e_snowfall] - FLUXES[f+F.e_sublimation] - FLUXES[f+F.e_melt])*deltat);
// 
//              printf("dSWE  diff= %2.10e\n",POOLS[nxp+S.E_SWE]-POOLS[p+S.E_SWE] - (FLUXES[f+F.snow_heat] + FLUXES[f+F.e_snowfall] - FLUXES[f+F.e_sublimation] - FLUXES[f+F.e_melt])*deltat);
// 
//     printf("Melt E t= %2.10e\n",FLUXES[f+F.e_melt]*deltat);
//      printf("Sublimation E t= %2.10e\n",FLUXES[f+F.e_sublimation]*deltat);
//     printf("Snowfall E t= %2.10e\n",FLUXES[f+F.e_snowfall]*deltat);
//     printf("Heat flux E t= %2.10e\n",FLUXES[f+F.snow_heat]*deltat);
// 
//     printf("Nosnow = %i\n",nosnow);
//       printf("H2O_SWE_PLUS_INPUTS = %2.2f\n",H2O_SWE_PLUS_INPUTS);}



// 





    
}


return 0;
}


int DALEC_1100_MODCONFIG(DALEC * DALECmodel, DATA * DATA){


struct DALEC_1100_PARAMETERS P=DALEC_1100_PARAMETERS;
struct DALEC_1100_FLUXES F=DALEC_1100_FLUXES;
struct DALEC_1100_POOLS S=DALEC_1100_POOLS;
struct DALEC_1100_EDCs E=DALEC_1100_EDCs;




DALECmodel->dalec=DALEC_1100;
DALECmodel->nopools=4;
DALECmodel->nopars=7;
DALECmodel->nofluxes=7;
DALECmodel->noedcs=2;


DALEC_1100_FLUX_SOURCES_SINKS(DALECmodel);




//Define PARS_INFO here (ranges, and eventually names, etc)
PARS_INFO_1100(DALECmodel);

//EDC operator
//Make sure has same number as number of EDCs
//This is generic EDCs structure defined in ../DALEC_EDCs/DALEC_EDC_FUNCTIONS.c
//Has three args, data (void), function (in "DATA", and "void *", and "double" out), and "boolean" prerun.



// 
// 
 DALECmodel->EDCs=calloc(DALECmodel->noedcs,sizeof(EDCs));
// //Abbrieviate
EDCs * EDCs=DALECmodel->EDCs;
// 
// //Som lit turnover rate
// 
// //EDC: litter tor > cwd tor
// //List all inequality calls here
// static DALEC_EDC_PARAMETER_INEQUALITY_STRUCT EDC_litcwdtor; 
// static DALEC_EDC_PARAMETER_INEQUALITY_STRUCT EDC_cwdsomtor;
// static DALEC_EDC_PARAMETER_INEQUALITY_STRUCT EDC_mr_rates;
// 
// EDC_litcwdtor.big_par_index=P.t_lit;
// EDC_litcwdtor.small_par_index=P.t_cwd;
// EDCs[E.litcwdtor].data=&EDC_litcwdtor;
// EDCs[E.litcwdtor].function=&DALEC_EDC_PARAMETER_INEQUALITY;
// EDCs[E.litcwdtor].prerun=true;
// 
// //EDC: cwd tor > som tor
// EDC_cwdsomtor.big_par_index=P.t_cwd;
// EDC_cwdsomtor.small_par_index=P.t_som;
// EDCs[E.cwdsomtor].data=&EDC_cwdsomtor;
// EDCs[E.cwdsomtor].function=&DALEC_EDC_PARAMETER_INEQUALITY;
// EDCs[E.cwdsomtor].prerun=true;
// 
// //EDC: foliar and root mr > wood mr
// EDC_mr_rates.big_par_index=P.rauto_mr_r;
// EDC_mr_rates.small_par_index=P.rauto_mr_w;
// EDCs[E.mr_rates].data=&EDC_mr_rates;
// EDCs[E.mr_rates].function=&DALEC_EDC_PARAMETER_INEQUALITY;
// EDCs[E.mr_rates].prerun=true;
// 
// 


//State ranges
   static DALEC_EDC_STATE_RANGES_STRUCT EDC_sr;

   //Create array (1x number of pools) for minimum and max values 
   //Index exactly corresponds to "POOLS" indices

   EDC_sr.min_val=calloc(DALECmodel->nopools,sizeof(double));

   EDC_sr.max_val=calloc(DALECmodel->nopools,sizeof(double));
   
   int n;
   for (n=0;n<DALECmodel->nopools;n++){
   EDC_sr.min_val[n]=-INFINITY;
              EDC_sr.max_val[n]=INFINITY;}

//    //  
    EDC_sr.min_val[S.H2O_SWE]=0;
    EDC_sr.max_val[S.H2O_SWE]=DALECmodel->PARS_INFO.parmax[P.i_SWE_H2O];


    EDC_sr.min_val[S.E_SWE]=0;

        EDC_sr.min_val[S.D_TEMP_SWE]=DGCM_TK0C-100;
    EDC_sr.max_val[S.D_TEMP_SWE]=DGCM_TK0C;



    //EDC_sr.min_val[S.E_SWE]=0;



//Adding EDC to the EDCs list


    EDCs[E.state_ranges].data=&EDC_sr;
    EDCs[E.state_ranges].function=&DALEC_EDC_STATE_RANGES;
    EDCs[E.state_ranges].prerun=false;







 static DALEC_EDC_TRAJECTORY_STRUCT EDC_st;
 
 static int edc_pool_indices[1];
    static double edc_pool_eqf[1];
EDC_st.pool_indices=edc_pool_indices;
EDC_st.pool_eqf=edc_pool_eqf;
EDC_st.no_pools_to_check=1;

EDC_st.pool_indices[0]=S.H2O_SWE;




for (n=0;n<EDC_st.no_pools_to_check;n++){EDC_st.pool_eqf[n]=DATA->ncdf_data.EDC_EQF;printf("DATA->ncdf_data.EDC_EQF = %2.2f\n",DATA->ncdf_data.EDC_EQF);}

    
EDCs[E.state_trajectories].data=&EDC_st;
EDCs[E.state_trajectories].function=&DALEC_EDC_TRAJECTORY;
EDCs[E.state_trajectories].prerun=false;



//declaring observation operator structure, and filling with DALEC configurations
static OBSOPE OBSOPE;
//Initialize all SUPPORT OBS values (default value = false).
INITIALIZE_OBSOPE_SUPPORT(&OBSOPE);

OBSOPE.SUPPORT_SCF_OBS=true;

OBSOPE.SCF_pool=S.D_SCF;


DALECmodel->OBSOPE=OBSOPE;



return 0;}







