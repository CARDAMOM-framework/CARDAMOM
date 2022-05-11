//This file was moddified from CARDAMOM_READ_BINARY_DATA.c
//it attepts to read in a netcdf file and store it
//Note that a lot of this is derived from https://www.unidata.ucar.edu/software/netcdf/docs/simple_nc4_rd_8c-example.html

#pragma once
#include "../../auxi_fun/filediag.c"
#include "stdlib.h"
#include "stdio.h"
#include "memory.h"
#include <math.h>
#include "CARDAMOM_NETCDF_DATA_STRUCTURE.c"
//Data
#include "../COST_FUNCTION/CARDAMOM_LIKELIHOOD_FUNCTION.c"
#include "NETCDF_AUXILLIARY_FUNCTIONS.c"
//Fusion


#include <netcdf.h>


//NOTE ABOUT THIS MACRO:
//If set to 1, netCDF methods will continue to run and return with default values if they fail to find the requested variable or attribute
//if set to 0, they will instantly die on failing to find any variable or attribute
//This is meant as a debugging tool more than anything else, and can be used with a "fully complete" netcdf cardamom file containing every variable in order to check if there were typeos in the c code.
//In the production enviroment, this should always be set to 1
#define ALLOW_DEFAULTS 1


//#include "CARDAMOM_MODEL_LIBRARY.c"




/* Handle errors by printing an error message and exiting with a
 * non-zero status.*/

#define ERRCODE 2
#define ERR(e) {printf("Error in %s at %d: %s\n", __FILE__, __LINE__, nc_strerror(e)); exit(ERRCODE);}
//Helper for errors in variable-getting methods
#define ERR_VAR(e, var) {printf("Error in %s at %d with variable \"%s\": %s\n", __FILE__, __LINE__, var, nc_strerror(e)); exit(ERRCODE);}
//Helper for errors in attribute-getting methods
#define ERR_ATTR_AND_CONTEXT(e, attr, context, varID) {printf("Error in %s at %d with attribute \"%s\" in the context \"%s\" (%d):  %s\n", __FILE__, __LINE__, attr,context,varID, nc_strerror(e)); exit(ERRCODE);}




/*
 * Function:  CARDAMOM_READ_NETCDF_DATA
 * --------------------
 * Attempts to read in the cardamom netcdf file
 *
 * filename: The path of the file to be read
 * DATA: The DATA struct to read the data into
 *
 *
 *	NOTE: if you intend to modify what is read in when reading a netCDF file,
 *  You MUST edit both this method, and the DATA struct in CARDAMOM/C/projects/CARDAMOM_GENERAL/CARDAMOM_DATA_STRUCTURE.c
 */





	int DEFAULT_REFERENCE_MEAN(TIMESERIES_DRIVER_STRUCT  * DRI){
   if (DRI->reference_mean==DEFAULT_DOUBLE_VAL){
     int n;DRI->reference_mean=0;
     for (n=0;n<DRI->length;n++){
     DRI->reference_mean+=DRI->values[n]/(double)DRI->length;}
     }
     
     return 0;
   }



int CARDAMOM_READ_NETCDF_DATA(char *filename,NETCDF_DATA *DATA)
{
	int retval =0; //Return value variable for NCDF calls.
	int ncid;
	if ((retval = nc_open(filename, NC_NOWRITE, &ncid))){
 		ERR(retval);
 	}


       

    
    
// 
// //Global defaults
// default_int_value(OBS.opt_unc_type,0);
// default_int_value(OBS.opt_normalization,0);//(0 = none, 1 = remove mean, 2 = divide by mean)
// default_int_value(OBS.opt_filter,0);
// default_double_value(OBS.opt_min_threshold,log(0));//minus infinity
// default_double_value(OBS.structural_unc,0);

   
    

//Read data
DATA->ABGB=READ_NETCDF_TIMESERIES_OBS_FIELDS(ncid, "ABGB");
DATA->CH4=READ_NETCDF_TIMESERIES_OBS_FIELDS(ncid, "CH4");
DATA->CWOO=READ_NETCDF_TIMESERIES_OBS_FIELDS(ncid, "CWOO");
DATA->DOM=READ_NETCDF_TIMESERIES_OBS_FIELDS(ncid, "DOM");
DATA->ET=READ_NETCDF_TIMESERIES_OBS_FIELDS(ncid, "ET");
DATA->EWT=READ_NETCDF_TIMESERIES_OBS_FIELDS(ncid, "EWT");
DATA->GPP=READ_NETCDF_TIMESERIES_OBS_FIELDS(ncid, "GPP");
DATA->SIF=READ_NETCDF_TIMESERIES_OBS_FIELDS(ncid, "SIF");
DATA->LAI=READ_NETCDF_TIMESERIES_OBS_FIELDS(ncid, "LAI");
DATA->NBE=READ_NETCDF_TIMESERIES_OBS_FIELDS(ncid, "NBE");
// printf("Just read NBE...\n");
// printf("DATA->NBE.values[0] = %2.2f\n",DATA->NBE.values[0]);
DATA->ROFF=READ_NETCDF_TIMESERIES_OBS_FIELDS(ncid, "ROFF");
DATA->SCF=READ_NETCDF_TIMESERIES_OBS_FIELDS(ncid, "SCF");
//Read time-averaged data

DATA->Mean_ABGB=READ_NETCDF_SINGLE_OBS_FIELDS(ncid, "Mean_ABGB");
DATA->Mean_GPP=READ_NETCDF_SINGLE_OBS_FIELDS(ncid, "Mean_GPP");
DATA->Mean_LAI=READ_NETCDF_SINGLE_OBS_FIELDS(ncid, "Mean_LAI");
DATA->Mean_FIR=READ_NETCDF_SINGLE_OBS_FIELDS(ncid, "Mean_FIR");


//Read parameters and single observations
DATA->PEQ_Cefficiency=READ_NETCDF_SINGLE_OBS_FIELDS(ncid, "PEQ_Cefficiency");
DATA->PEQ_CUE=READ_NETCDF_SINGLE_OBS_FIELDS(ncid, "PEQ_CUE");
DATA->PEQ_C3frac=READ_NETCDF_SINGLE_OBS_FIELDS(ncid, "PEQ_C3frac");
DATA->PEQ_Vcmax25=READ_NETCDF_SINGLE_OBS_FIELDS(ncid, "PEQ_Vcmax25");
DATA->PEQ_iniSOM=READ_NETCDF_SINGLE_OBS_FIELDS(ncid, "PEQ_iniSOM");
DATA->PEQ_iniSnow=READ_NETCDF_SINGLE_OBS_FIELDS(ncid, "PEQ_iniSnow");

//Global defaults: these are set in pre-process if not defined below
// default_int_value(&OBS->opt_unc_type,0);
// default_int_value(&OBS->opt_normalization,0);
// default_int_value(&OBS->opt_filter,0);
// default_double_value(&OBS->opt_min_threshold,log(0));//minus infinity
// default_double_value(&OBS->structural_unc,0);


printf("Warning: No longer assigning default uncertainty values. Please double check your driver file contains uncertainties for all observations used.\n");
printf("Example uncertainty values (used in the past) per variable:\n");
// Default ABGB options
printf("ABGB.opt_unc_type=1; ABGB.single_unc=2; ABGB.min_threshold=10 gC/m2;\n");
default_int_value(&DATA->ABGB.opt_unc_type,1);
default_double_value(&DATA->ABGB.single_unc,2);
default_double_value(&DATA->ABGB.min_threshold,10);//gC/m2

// Default CH4 options
printf("CH4.opt_unc_type=1; CH4.single_unc=2; CH4.min_threshold=1e-5 mgCH4/m2/d;\n");
default_int_value(&DATA->CH4.opt_unc_type,1);
default_double_value(&DATA->CH4.single_unc,2);
default_double_value(&DATA->CH4.min_threshold,1e-5);//mgCH4/m2/d

//Default ET options
printf("ET.opt_unc_type=1; ET.single_unc=2; ET.min_threshold=0.1 mm/d;\n");
default_int_value(&DATA->ET.opt_unc_type,1);
default_double_value(&DATA->ET.single_unc,2);
default_double_value(&DATA->ET.min_threshold,0.1);

//Default ROFF options
printf("ROFF.opt_unc_type=1; ROFF.single_unc=2; ROFF.min_threshold=0.1 mm/d;\n");
default_int_value(&DATA->ROFF.opt_unc_type,1);
default_double_value(&DATA->ROFF.single_unc,2);
default_double_value(&DATA->ROFF.min_threshold,0.1);

//Default EWT options;
printf("EWT.single_unc=2; EWT.opt_normalization=1;\n");
default_double_value(&DATA->EWT.single_unc,50);//mm
default_int_value(&DATA->EWT.opt_normalization,1);

//Default GPP options
printf("GPP.opt_unc_type=1; GPP.single_unc=2; GPP.min_threshold=0.1 gC/m2/d;\n");
default_int_value(&DATA->GPP.opt_unc_type,1);
default_double_value(&DATA->GPP.single_unc,2);
default_double_value(&DATA->GPP.min_threshold,0.1);//gC/m2/d

//Default SIF options - shuang added
printf("SIF.opt_unc_type=1; SIF.single_unc=2; SIF.min_threshold=0.1 gC/m2/d;\n");
default_int_value(&DATA->SIF.opt_unc_type,1);
default_double_value(&DATA->SIF.single_unc,2);
default_double_value(&DATA->SIF.min_threshold,0.1);//gC/m2/d

//Default LAI options
printf("LAI.opt_unc_type=1; LAI.single_unc=2; LAI.min_threshold=0.1 m2/m2;\n");
default_int_value(&DATA->LAI.opt_unc_type,1);
default_double_value(&DATA->LAI.single_unc,2);
default_double_value(&DATA->LAI.min_threshold,0.1);//m2/m2


//Default NBE options;
printf("NBE.single_unc=1 gC/m2/d;\n");
default_double_value(&DATA->NBE.single_unc,1);//gC/m2/d

//Default DOM options
printf("DOM.opt_unc_type=1; DOM.single_unc=2; DOM.min_threshold=10 gC/m2;\n");
default_int_value(&DATA->DOM.opt_unc_type,1);
default_double_value(&DATA->DOM.single_unc,2);
default_double_value(&DATA->DOM.min_threshold,10);//gC/m2


//Default SCF options
printf("SCF.opt_unc_type=0; SCF.single_unc=0.1; SCF.min_threshold=0.1 m2/m2;\n");
default_int_value(&DATA->SCF.opt_unc_type,0);
default_double_value(&DATA->SCF.single_unc,0.1);
default_double_value(&DATA->SCF.min_threshold,0.1);//m2/m2


//pre-process obs to save time
//Only required for timeseries obs
TIMESERIES_OBS_STRUCT_PREPROCESS(&DATA->ABGB);
TIMESERIES_OBS_STRUCT_PREPROCESS(&DATA->CH4);
TIMESERIES_OBS_STRUCT_PREPROCESS(&DATA->CWOO);
TIMESERIES_OBS_STRUCT_PREPROCESS(&DATA->DOM);
TIMESERIES_OBS_STRUCT_PREPROCESS(&DATA->ET);
TIMESERIES_OBS_STRUCT_PREPROCESS(&DATA->EWT);
TIMESERIES_OBS_STRUCT_PREPROCESS(&DATA->GPP);
TIMESERIES_OBS_STRUCT_PREPROCESS(&DATA->SIF);
TIMESERIES_OBS_STRUCT_PREPROCESS(&DATA->LAI);
TIMESERIES_OBS_STRUCT_PREPROCESS(&DATA->NBE);
TIMESERIES_OBS_STRUCT_PREPROCESS(&DATA->ROFF);
TIMESERIES_OBS_STRUCT_PREPROCESS(&DATA->SCF);



printf("Done preprocess");



    
    
printf("Done reading all other edc ");


	DATA->BURNED_AREA.values=ncdf_read_double_var(ncid, "BURNED_AREA", &(DATA->BURNED_AREA.length));
                DATA->BURNED_AREA.reference_mean=ncdf_read_double_attr(ncid, "BURNED_AREA","reference_mean");
                                                DEFAULT_REFERENCE_MEAN(&DATA->BURNED_AREA);
                                                
                                                
	DATA->CO2.values=ncdf_read_double_var(ncid, "CO2", &(DATA->CO2.length));
		DATA->CO2.reference_mean=ncdf_read_double_attr(ncid, "CO2","reference_mean");
                                                DEFAULT_REFERENCE_MEAN(&DATA->CO2);

        
	DATA->DOY.values=ncdf_read_double_var(ncid, "DOY", &(DATA->DOY.length));
		//DATA->DOY.reference_mean=ncdf_read_double_attr(ncid, "DOY","reference_mean");
    
                    	DATA->SKT.values=ncdf_read_double_var(ncid, "SKT", &(DATA->SKT.length));
		DATA->SKT.reference_mean=ncdf_read_double_attr(ncid, "SKT","reference_mean");
                DEFAULT_REFERENCE_MEAN(&DATA->SKT);

	DATA->SNOWFALL.values=ncdf_read_double_var(ncid, "SNOWFALL", &(DATA->SNOWFALL.length));
		DATA->SNOWFALL.reference_mean=ncdf_read_double_attr(ncid, "SNOWFALL","reference_mean");
        DEFAULT_REFERENCE_MEAN(&DATA->SNOWFALL);

	DATA->SSRD.values=ncdf_read_double_var(ncid, "SSRD", &(DATA->SSRD.length));
		DATA->SSRD.reference_mean=ncdf_read_double_attr(ncid, "SSRD","reference_mean");
                DEFAULT_REFERENCE_MEAN(&DATA->SSRD);
                
                	DATA->STRD.values=ncdf_read_double_var(ncid, "STRD", &(DATA->STRD.length));
		DATA->STRD.reference_mean=ncdf_read_double_attr(ncid, "STRD","reference_mean");
                DEFAULT_REFERENCE_MEAN(&DATA->STRD);


	DATA->T2M_MAX.values=ncdf_read_double_var(ncid, "T2M_MAX", &(DATA->T2M_MAX.length));
		DATA->T2M_MAX.reference_mean=ncdf_read_double_attr(ncid, "T2M_MAX","reference_mean");
                        DEFAULT_REFERENCE_MEAN(&DATA->T2M_MAX);


	DATA->T2M_MIN.values=ncdf_read_double_var(ncid, "T2M_MIN", &(DATA->T2M_MIN.length));
		DATA->T2M_MIN.reference_mean=ncdf_read_double_attr(ncid, "T2M_MIN","reference_mean");
                        DEFAULT_REFERENCE_MEAN(&DATA->T2M_MIN);


	DATA->TIME_INDEX.values=ncdf_read_double_var(ncid, "time", &(DATA->TIME_INDEX.length));
		//DATA->TIME_INDEX.reference_mean=ncdf_read_double_attr(ncid, "time","reference_mean");


	DATA->TOTAL_PREC.values=ncdf_read_double_var(ncid, "TOTAL_PREC", &(DATA->TOTAL_PREC.length));
		DATA->TOTAL_PREC.reference_mean=ncdf_read_double_attr(ncid, "TOTAL_PREC","reference_mean");
                                DEFAULT_REFERENCE_MEAN(&DATA->TOTAL_PREC);


	DATA->VPD.values=ncdf_read_double_var(ncid, "VPD", &(DATA->VPD.length));
		DATA->VPD.reference_mean=ncdf_read_double_attr(ncid, "VPD","reference_mean");
                                DEFAULT_REFERENCE_MEAN(&DATA->VPD);



        
        
        
        //Summary & derived variables
// double EDC;
// double EDCDIAG;
// double EDC_EQF;
// double ID;
// double LAT;
// double Ntimesteps;
// double deltat;

	DATA->EDC=ncdf_read_single_double_var(ncid, "EDC");
 
    DATA->EDCDIAG=ncdf_read_single_int_var(ncid, "EDCDIAG");
    default_int_value(&DATA->EDCDIAG,0);

    
	DATA->EDC_EQF=ncdf_read_single_double_var(ncid, "EDC_EQF");
	    default_double_value(&DATA->EDC_EQF,2);

    
    DATA->ID=ncdf_read_single_double_var(ncid, "ID" );
	DATA->LAT=ncdf_read_single_double_var(ncid, "LAT" );
    
    //Pre-processing
    //Ntimesteps
    DATA->Ntimesteps=DATA->TIME_INDEX.length;
    //Delta T
    DATA->deltat=DATA->TIME_INDEX.values[1]-DATA->TIME_INDEX.values[0];
    //Mean temp
    DATA->meantemp=DATA->T2M_MIN.reference_mean*0.5 + DATA->T2M_MAX.reference_mean*0.5;
    //Solar Zenith Angle
    DATA->SZA=(double *)calloc(DATA->Ntimesteps, sizeof(double));
    int n;
    double pi=3.1415927;
    for (n=0;n<DATA->Ntimesteps;n++){
    /*Calculate light extinction coefficient*/
double B = (DATA->DOY.values[n]-81)*2*pi/365.;
double ET1 = 9.87*sin(2*B)-7.53*cos(B)-1.5*sin(B);
double DA = 23.45*sin((284+DATA->DOY.values[n])*2*pi/365); //Deviation angle
//double LST = (int) (DOY[n]*24*60) % (24*60);
double LST=0.5*24*60;
double AST = LST+ET1;
double h = (AST-12*60)/4; //hour angle
double alpha = asin((sin(pi/180*DATA->LAT)*sin(pi/180*DA)+cos(pi/180*DATA->LAT)*cos(pi/180.*DA)*cos(pi/180*h)))*180/pi; //solar altitude
 DATA->SZA[n] = 90-alpha;}
    
   
printf("Done reading all data");




MCMCID_STRUCT MCMCID;

// MCOPT->nOUT=DATA.ncdf_data.MCMCID.nOUT;
// MCOPT->nPRINT=DATA.ncdf_data.MCMCID.nPRINT;
// MCOPT->nWRITE=DATA.ncdf_data.MCMCID.nWRITE;
// MCOPT->minstepsize=DATA.ncdf_data.MCMCID.minstepsize;
// MCOPT->mcmcid=DATA.ncdf_data.MCMCID.value;
// MCOPT->nADAPT=DATA.ncdf_data.MCMCID.nADAPT;



MCMCID.value = ncdf_read_single_double_var(ncid, "MCMCID");
MCMCID.nITERATIONS = ncdf_read_int_attr(ncid, "MCMCID","nITERATIONS");
MCMCID.nPRINT = ncdf_read_int_attr(ncid, "MCMCID","nPRINT");
MCMCID.nSAMPLES= ncdf_read_int_attr(ncid, "MCMCID","nSAMPLES");
MCMCID.nADAPT= ncdf_read_int_attr(ncid, "MCMCID","nADAPT");
MCMCID.fADAPT= ncdf_read_double_attr(ncid, "MCMCID","fADAPT");
MCMCID.minstepsize=ncdf_read_double_attr(ncid, "MCMCID","minstepsize");
MCMCID.seed_number=ncdf_read_double_attr(ncid, "MCMCID","seed_number");

if (isnan(MCMCID.value)){MCMCID.value=DEFAULT_DOUBLE_VAL;}



DATA->MCMCID=MCMCID;

//

printf("Done reading MCMC valuea and attributes");

        
	return 0;
}
