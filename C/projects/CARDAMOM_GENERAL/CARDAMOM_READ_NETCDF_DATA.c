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


#define DEFAULT_DOUBLE_VAL -9999.0
#define DEFAULT_INT_VAL -9999


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



//Function for reading these
/*OBS_STRUCT READ_NETCDF_OBS_FIELDS(int ncid, char * OBSNAME){
OBS_STRUCT OBS;
printf("Struct declared OK\n");

printf("OBSNAME = %s\n",OBSNAME);


char uncsf[50],OBSunc[50];
strcpy(OBSunc,OBSNAME);
strcpy(uncsf,"unc");
strcat(OBSunc,uncsf);



size_t unclength;
OBS.unc = ncdf_read_double_var(ncid, OBSunc , &unclength);




OBS.values = ncdf_read_double_var(ncid, OBSNAME , &(OBS.length));
OBS.opt_log_transform=ncdf_read_int_attr(ncid, OBSNAME,"opt_log_transform");
OBS.opt_normalization=ncdf_read_int_attr(ncid, OBSNAME,"opt_normalization");
OBS.opt_mean_only=ncdf_read_int_attr(ncid, OBSNAME,"opt_mean_only");
OBS.opt_structural_error=ncdf_read_int_attr(ncid, OBSNAME,"opt_structural_error");
OBS.min_threshold_value=ncdf_read_double_attr(ncid, OBSNAME,"min_threshold_value");
OBS.single_monthly_unc=ncdf_read_double_attr(ncid, OBSNAME,"single_monthly_unc");
OBS.single_annual_unc=ncdf_read_double_attr(ncid, OBSNAME,"single_annual_unc");
OBS.structural_unc=ncdf_read_double_attr(ncid, OBSNAME,"structural_unc");
return OBS;
};
*/



int CARDAMOM_READ_NETCDF_DATA(char *filename,NETCDF_DATA *DATA)
{
	int retval =0; //Return value variable for NCDF calls.
	int ncid;
	if ((retval = nc_open(filename, NC_NOWRITE, &ncid))){
 		ERR(retval);
 	}

	DATA->ABGB.values=ncdf_read_double_var(ncid, "ABGB", &(DATA->ABGB.length));



	DATA->CH4.values=ncdf_read_double_var(ncid, "CH4", &(DATA->CH4.length));
		DATA->CH4.Uncertainty=ncdf_read_double_attr(ncid, "CH4","Uncertainty");
		DATA->CH4.Annual_Uncertainty=ncdf_read_double_attr(ncid, "CH4","Annual_Uncertainty");
		DATA->CH4.Uncertainty_Threshold=ncdf_read_double_attr(ncid, "CH4","Uncertainty_Threshold");



	DATA->EDC=ncdf_read_single_double_var(ncid, "EDC");

	DATA->EDCDIAG=ncdf_read_single_double_var(ncid, "EDCDIAG");


	

//Read data
DATA->ET=READ_NETCDF_OBS_FIELDS(ncid, "ET");






printf("DATA->ET.values[0] = %2.2f\n",DATA->ET.values[1]);
printf("DATA->ET.unc[0] = %2.2f\n",DATA->ET.unc[1]);
printf("DATA->ET.length[0] = %i\n",(int)DATA->ET.length);
printf("**********************\n");




	//DATA->ET.Uncertainty=ncdf_read_double_attr(ncid, "ET","Uncertainty");
//	DATA->ET.Annual_Uncertainty=ncdf_read_double_attr(ncid, "ET","Annual_Uncertainty");
//	DATA->ET.Uncertainty_Threshold=ncdf_read_double_attr(ncid, "ET","Uncertainty_Threshold");








	DATA->EWT.values=ncdf_read_double_var(ncid, "EWT", &(DATA->EWT.length));
		DATA->EWT.Uncertainty=ncdf_read_double_attr(ncid, "EWT","Uncertainty");
		DATA->EWT.Annual_Uncertainty=ncdf_read_double_attr(ncid, "EWT","Annual_Uncertainty");

	DATA->GPP.values=ncdf_read_double_var(ncid, "GPP", &(DATA->GPP.length));
		DATA->GPP.Uncertainty=ncdf_read_double_attr(ncid, "GPP","Uncertainty");
		DATA->GPP.Annual_Uncertainty=ncdf_read_double_attr(ncid, "GPP","Annual_Uncertainty");
		DATA->GPP.gppabs=ncdf_read_double_attr(ncid, "GPP","gppabs");
		DATA->GPP.Uncertainty_Threshold=ncdf_read_double_attr(ncid, "GPP","Uncertainty_Threshold");

	DATA->ID=ncdf_read_single_double_var(ncid, "ID" );

	DATA->LAI.values=ncdf_read_double_var(ncid, "LAI", &(DATA->LAI.length));

	DATA->LAT=ncdf_read_single_double_var(ncid, "LAT" );

	DATA->Mean_Biomass.values=ncdf_read_double_var(ncid, "Mean_Biomass", &(DATA->Mean_Biomass.length));
		DATA->Mean_Biomass.Uncertainty=ncdf_read_double_attr(ncid, "Mean_Biomass","Uncertainty");

	DATA->Mean_Fire.values=ncdf_read_double_var(ncid, "Mean_Fire", &(DATA->Mean_Fire.length));
		DATA->Mean_Fire.Uncertainty=ncdf_read_double_attr(ncid, "Mean_Fire","Uncertainty");

	DATA->Mean_GPP.values=ncdf_read_double_var(ncid, "Mean_GPP", &(DATA->Mean_GPP.length));
		DATA->Mean_GPP.Uncertainty=ncdf_read_double_attr(ncid, "Mean_GPP","Uncertainty");

	DATA->Mean_LAI.values=ncdf_read_double_var(ncid, "Mean_LAI", &(DATA->Mean_LAI.length));
		DATA->Mean_LAI.Uncertainty=ncdf_read_double_attr(ncid, "Mean_LAI","Uncertainty");

	DATA->CH4.values=ncdf_read_double_var(ncid, "CH4", &(DATA->CH4.length));
		DATA->CH4.Uncertainty=ncdf_read_double_attr(ncid, "CH4","Uncertainty");
		DATA->CH4.Annual_Uncertainty=ncdf_read_double_attr(ncid, "CH4","Annual_Uncertainty");

	DATA->SOM.values=ncdf_read_double_var(ncid, "SOM", &(DATA->SOM.length));
		DATA->SOM.Uncertainty=ncdf_read_double_attr(ncid, "SOM","Uncertainty");

	DATA->NBE.values=ncdf_read_double_var(ncid, "NBE", &(DATA->NBE.length));
		DATA->NBE.Uncertainty=ncdf_read_double_attr(ncid, "NBE","Uncertainty");
		DATA->NBE.Annual_Uncertainty=ncdf_read_double_attr(ncid, "NBE","Annual_Uncertainty");

	DATA->PARPRIORS.values=ncdf_read_double_var(ncid, "PARPRIORS", &(DATA->PARPRIORS.length));
	DATA->PARPRIORUNC.values=ncdf_read_double_var(ncid, "PARPRIORUNC", &(DATA->PARPRIORUNC.length));
	DATA->OTHERPRIORS.values=ncdf_read_double_var(ncid, "OTHERPRIORS", &(DATA->OTHERPRIORS.length));
	DATA->OTHERPRIORSUNC.values=ncdf_read_double_var(ncid, "OTHERPRIORSUNC", &(DATA->OTHERPRIORSUNC.length));

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

/*
	DATA->EDC=ncdf_read_single_double_var(ncid, "EDC", -9999.0);
	DATA->EDCDIAG=ncdf_read_single_double_var(ncid, "EDCDIAG", -9999.0);
	DATA->ET=ncdf_read_single_double_var(ncid, "ET", -9999.0);
	DATA->EWT=ncdf_read_single_double_var(ncid, "EWT", -9999.0);
	DATA->GPP.values=ncdf_read_double_var(ncid, "GPP", &(DATA->GPP.length));
	DATA->GPP.Uncertainty=ncdf_read_double_attr(ncid, "GPP","Uncertainty", -9999.0 );
	DATA->GPP.Annual_Uncertainty=ncdf_read_double_attr(ncid, "GPP","Annual_Uncertainty", -9999.0 );
	DATA->GPP.gppabs=ncdf_read_double_attr(ncid, "GPP","gppabs", -9999.00 );
	DATA->GPP.obs_unc_threshold=ncdf_read_double_attr(ncid, "GPP","obs_unc_threshold", -9999.0 );


	DATA->ID=ncdf_read_single_double_var(ncid, "ID", -9999.0);
	DATA->LAI.values=ncdf_read_double_var(ncid, "LAI", &(DATA->LAI.length));
*/

	return 0;
}
