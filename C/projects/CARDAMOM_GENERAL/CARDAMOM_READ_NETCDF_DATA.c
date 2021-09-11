//This file was moddified from CARDAMOM_READ_BINARY_DATA.c
//it attepts to read in a netcdf file and store it
//Note that a lot of this is derived from https://www.unidata.ucar.edu/software/netcdf/docs/simple_nc4_rd_8c-example.html

#pragma once
#include "../../auxi_fun/filediag.c"
#include "../../auxi_fun/oksofar.c"
#include "stdlib.h"
#include "stdio.h"
#include "memory.h"
#include <math.h>
#include "CARDAMOM_NETCDF_DATA_STRUCTURE.c"
#include "../COST_FUNCTION/CARDAMOM_LIKELIHOOD_FUNCTION.c"
#include "NETCDF_AUXILLIARY_FUNCTIONS.c"


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
DATA->NBE=READ_NETCDF_TIMESERIES_OBS_FIELDS(ncid, "ABGB");
DATA->CH4=READ_NETCDF_TIMESERIES_OBS_FIELDS(ncid, "CH4");
DATA->ET=READ_NETCDF_TIMESERIES_OBS_FIELDS(ncid, "ET");
DATA->EWT=READ_NETCDF_TIMESERIES_OBS_FIELDS(ncid, "EWT");
DATA->GPP=READ_NETCDF_TIMESERIES_OBS_FIELDS(ncid, "GPP");
DATA->LAI=READ_NETCDF_TIMESERIES_OBS_FIELDS(ncid, "LAI");
DATA->NBE=READ_NETCDF_TIMESERIES_OBS_FIELDS(ncid, "NBE");
DATA->DOM=READ_NETCDF_TIMESERIES_OBS_FIELDS(ncid, "DOM");

DATA->Mean_ABGB=READ_NETCDF_SINGLE_OBS_FIELDS(ncid, "Mean_ABGB");
DATA->Mean_GPP      =READ_NETCDF_SINGLE_OBS_FIELDS(ncid, "Mean_GPP");
DATA->Mean_LAI       =READ_NETCDF_SINGLE_OBS_FIELDS(ncid, "Mean_LAI");
DATA->Mean_FIR     =READ_NETCDF_SINGLE_OBS_FIELDS(ncid, "Mean_FIR");



//Global defaults: these are set in pre-process if not defined below
// default_int_value(&OBS->opt_unc_type,0);
// default_int_value(&OBS->opt_normalization,0);
// default_int_value(&OBS->opt_filter,0);
// default_double_value(&OBS->opt_min_threshold,log(0));//minus infinity
// default_double_value(&OBS->structural_unc,0);



// Default CH4 options

default_int_value(&DATA->CH4.opt_unc_type,1);
default_double_value(&DATA->CH4.single_unc,2);
default_double_value(&DATA->CH4.min_threshold,10);//gC/m2

// Default CH4 options

default_int_value(&DATA->CH4.opt_unc_type,1);
default_double_value(&DATA->CH4.single_unc,2);
default_double_value(&DATA->CH4.min_threshold,1e-5);//mgCH4/m2/d

//Default ET options

default_int_value(&DATA->ET.opt_unc_type,1);
default_double_value(&DATA->ET.single_unc,2);
default_double_value(&DATA->ET.min_threshold,0.1);

//Default EWT options;
default_double_value(&DATA->EWT.single_unc,2);
default_int_value(&DATA->EWT.opt_normalization,1);

//Default GPP options
default_int_value(&DATA->GPP.opt_unc_type,1);
default_double_value(&DATA->GPP.single_unc,2);
default_double_value(&DATA->GPP.min_threshold,0.1);//gC/m2/d

//Default LAI options
default_int_value(&DATA->LAI.opt_unc_type,1);
default_double_value(&DATA->LAI.single_unc,2);
default_double_value(&DATA->LAI.min_threshold,0.1);//m2/m2


//Default NBE options;
default_double_value(&DATA->NBE.single_unc,1);//gC/m2/d

//Default DOM options
default_int_value(&DATA->DOM.opt_unc_type,1);
default_double_value(&DATA->DOM.single_unc,2);
default_double_value(&DATA->DOM.min_threshold,10);//gC/m2



//pre-process obs to save time
//Only required for timeseries obs
TIMESERIES_OBS_STRUCT_PREPROCESS(&DATA->ABGB);
TIMESERIES_OBS_STRUCT_PREPROCESS(&DATA->CH4);
TIMESERIES_OBS_STRUCT_PREPROCESS(&DATA->ET);
TIMESERIES_OBS_STRUCT_PREPROCESS(&DATA->EWT);
TIMESERIES_OBS_STRUCT_PREPROCESS(&DATA->GPP);
TIMESERIES_OBS_STRUCT_PREPROCESS(&DATA->LAI);
TIMESERIES_OBS_STRUCT_PREPROCESS(&DATA->NBE);
TIMESERIES_OBS_STRUCT_PREPROCESS(&DATA->DOM);





	DATA->EDC=ncdf_read_single_double_var(ncid, "EDC");
	DATA->EDC_EQF=ncdf_read_single_double_var(ncid, "EDC_EQF");
	DATA->EDCDIAG=ncdf_read_single_double_var(ncid, "EDCDIAG");
	DATA->ID=ncdf_read_single_double_var(ncid, "ID" );
	DATA->LAT=ncdf_read_single_double_var(ncid, "LAT" );


       
        
      
	DATA->SSRD.values=ncdf_read_double_var(ncid, "SSRD", &(DATA->SSRD.length));
		DATA->SSRD.reference_mean=ncdf_read_double_attr(ncid, "SSRD","reference_mean");

	DATA->T2M_MAX.values=ncdf_read_double_var(ncid, "T2M_MAX", &(DATA->T2M_MAX.length));
		DATA->T2M_MAX.reference_mean=ncdf_read_double_attr(ncid, "T2M_MAX","reference_mean");

	DATA->T2M_MIN.values=ncdf_read_double_var(ncid, "T2M_MIN", &(DATA->T2M_MIN.length));
		DATA->T2M_MIN.reference_mean=ncdf_read_double_attr(ncid, "T2M_MIN","reference_mean");

	DATA->TIME_INDEX.values=ncdf_read_double_var(ncid, "TIME_INDEX", &(DATA->TIME_INDEX.length));
		DATA->TIME_INDEX.reference_mean=ncdf_read_double_attr(ncid, "TIME_INDEX","reference_mean");

	DATA->TOTAL_PREC.values=ncdf_read_double_var(ncid, "TOTAL_PREC", &(DATA->TOTAL_PREC.length));
		DATA->TOTAL_PREC.reference_mean=ncdf_read_double_attr(ncid, "TOTAL_PREC","reference_mean");

	DATA->VPD.values=ncdf_read_double_var(ncid, "VPD", &(DATA->VPD.length));
		DATA->VPD.reference_mean=ncdf_read_double_attr(ncid, "VPD","reference_mean");

	DATA->BURNED_AREA.values=ncdf_read_double_var(ncid, "BURNED_AREA", &(DATA->BURNED_AREA.length));
                DATA->BURNED_AREA.reference_mean=ncdf_read_double_attr(ncid, "BURNED_AREA","reference_mean");

	DATA->CO2.values=ncdf_read_double_var(ncid, "CO2", &(DATA->CO2.length));
		DATA->CO2.reference_mean=ncdf_read_double_attr(ncid, "CO2","reference_mean");

	DATA->DOY.values=ncdf_read_double_var(ncid, "DOY", &(DATA->DOY.length));
		DATA->DOY.reference_mean=ncdf_read_double_attr(ncid, "DOY","reference_mean");

	return 0;
}
