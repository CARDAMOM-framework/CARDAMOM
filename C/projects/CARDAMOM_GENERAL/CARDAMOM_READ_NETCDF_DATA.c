//This file was moddified from CARDAMOM_READ_BINARY_DATA.c
//it attepts to read in a netcdf file and store it
//Note that a loit of this is lifted directly from https://www.unidata.ucar.edu/software/netcdf/docs/simple_nc4_rd_8c-example.html
//TODO: Work on integrating this with the rest of CARDAMOM using CARDAMOM_RUN_MODEL.m



/*INCLUDING THE PARAMETER_INFO structure*/
/*Note: it remains unclear as to where this structure should be defined*/

#pragma once
#include "../../auxi_fun/filediag.c"
#include "../../auxi_fun/oksofar.c"
#include "stdlib.h"
#include "stdio.h"
#include "memory.h"
#include <math.h>
#include "CARDAMOM_NETCDF_DATA_STRUCTURE.c"

#include <netcdf.h>


//#include "CARDAMOM_MODEL_LIBRARY.c"




/* Handle errors by printing an error message and exiting with a
 * non-zero status. */
#define ERRCODE 2
#define ERR(e) {printf("Error in %s at %d: %s\n", __FILE__, __LINE__, nc_strerror(e)); exit(ERRCODE);}





/*
 * Function:  ncdf_read_int_attr
 * --------------------
 * Attempts to read a single int that is stored under a particular name in the netCDF file's attributes
 *
 *  ncid: netCDF file ID to pull the data from. This is the id given by nc_open after the netCDF file is opened
 *  context: this is the context in which the attribute was stored. Can be a variable name, or '/' if it is a global attribute
 *  attrName: This is the name of the attibute to read
 *  defaultVal: the default value to use if the requested attribute does not exsist
 *
 *  returns: the int attribute read, or defaultVal if the requested attribute does not exsist.
 *   if there is an error, the program exits after displaying a message.
 */
int ncdf_read_int_attr(int ncid, const char* context, const char * attrName, int defaultVal ){
	int retval =0; //Return value variable for NCDF calls.
	int attrResult;
	int varID=NC_GLOBAL;
	if ((context != NULL) && (context[0] != '/')){
		//Attempt to locate the id of the approprate variable
		if ((retval = nc_inq_varid(ncid, context, &varID))){
			//The failure was in locating the context, not the arttr
			ERR(retval);
		}
	}
	if ((retval = nc_get_att_int(ncid, varID, attrName, &attrResult))){
		if (retval ==NC_ENOTATT ){
			return defaultVal;
		}
		ERR(retval);
	}
	return attrResult;
}
/*
 * Function:  ncdf_read_double_attr
 * --------------------
 * Attempts to read a single double that is stored under a particular name in the netCDF file's attributes
 *
 *  ncid: netCDF file ID to pull the data from. This is the id given by nc_open after the netCDF file is opened
 *  context: this is the context in which the attribute was stored. Can be a variable name, or '/' if it is a global attribute
 *  attrName: This is the name of the attibute to read
 *  defaultVal: the default value to use if the requested attribute does not exsist
 *
 *  returns: the double attribute read. if there is an error, the program exits after displaying a message.
 */
double ncdf_read_double_attr(int ncid, const char* context, const char * attrName, double defaultVal ){
  int retval =0; //Return value variable for NCDF calls.
	double attrResult;
	int varID=NC_GLOBAL;
	if ((context != NULL) && (context[0] != '/')){
		//Attempt to locate the id of the approprate variable
		if ((retval = nc_inq_varid(ncid, context, &varID))){
			//The failure was in locating the context, not the arttr
			ERR(retval);
		}
	}
	if ((retval = nc_get_att_double(ncid, varID, attrName, &attrResult))){
		if (retval ==NC_ENOTATT ){
			return defaultVal;
		}
		ERR(retval);
	}
	return attrResult;
}





/*
 * Function:  ncfd_get_var_info
 * --------------------
 *  Helper method that Attempts to read the length and varID of a 1 or 0 dimensional variable that is stored under a particular name in the netCDF file's variables
 *
 *  ncid: netCDF file ID to pull the data from. This is the id given by nc_open after the netCDF file is opened
 *  varName: This is the name of the variable to read
 *  arrayLen: pointer where the total length in number of elements of the resulting array will be written (NOT MEMORY SIZE)
 *  varID: Pointer to where the varID of the requested var will be written
 *
 *  returns: None
 *   if there is an error, the program exits after displaying a message.
 */


void ncfd_get_var_info(int ncid, const char * varName, size_t * arrayLen, int * varID) {
  int retval =0; //Return value variable for NCDF calls.
	int numberOfDims;
	int dimensionID;
	//Attempt to locate the id of the approprate variable
	if ((retval = nc_inq_varid(ncid, varName, varID))){
		ERR(retval);
	}
	//check to make sure the variable has only one dim (Critial to assure well-defined behavior for all other ncdf function calls!!!)
	if ((retval = nc_inq_varndims(ncid, *varID, &numberOfDims))){
		ERR(retval);
	}
	if (numberOfDims>1 || numberOfDims<0){
		printf("Error in %s at %d while trying to read var %s: CARDAMOM ncfd_get_var_info only supports 1 and 0 dimensional variables, but var has %d dimensions.\n", __FILE__, __LINE__, varName,numberOfDims);
		exit(1);
	}
	//begin work in preperation for allocating the array where the variable data will be stored.
	if (numberOfDims==0){
		//special case, var is a scalar, so we know the length of the array is 1
		*arrayLen=1;
	}
	if ((retval = nc_inq_vardimid(ncid, *varID, &dimensionID))){
		ERR(retval);
	}
	if ((retval = nc_inq_dimlen(ncid, dimensionID, arrayLen))){
		ERR(retval);
	}
}



/*
 * Function:  ncdf_read_int_var
 * --------------------
 * Attempts to read a 1 dimensional int variable that is stored under a particular name in the netCDF file's variables
 *
 *  ncid: netCDF file ID to pull the data from. This is the id given by nc_open after the netCDF file is opened
 *  varName: This is the name of the variable to read
 *  arrayLen: pointer where the total length in number of elements of the resulting array will be written (NOT MEMORY SIZE)
 *
 *  returns: an array of all the values of the variable,
 *   if there is an error, the program exits after displaying a message.
 */
int * ncdf_read_int_var(int ncid, const char * varName, size_t * arrayLen ){
	int retval =0; //Return value variable for NCDF calls.
	int varID;
	ncfd_get_var_info(ncid, varName, arrayLen, &varID);
	//allocate the actual array we will be returning
	int * resultArray= (int*)calloc(*arrayLen, sizeof(int));

	if ((retval = nc_get_var_int(ncid, varID, resultArray))){
		ERR(retval);
	}
	return resultArray;
}

/*
 * Function:  ncdf_read_double_var
 * --------------------
 * Attempts to read a 1 dimensional double variable that is stored under a particular name in the netCDF file's variables
 *
 *  ncid: netCDF file ID to pull the data from. This is the id given by nc_open after the netCDF file is opened
 *  varName: This is the name of the variable to read
 *  arrayLen: pointer where the total length in number of elements of the resulting array will be written (NOT MEMORY SIZE)
 *
 *  returns: an array of all the values of the variable,
 *   if there is an error, the program exits after displaying a message.
 */
double * ncdf_read_double_var(int ncid, const char * varName, size_t * arrayLen ){
	int retval =0; //Return value variable for NCDF calls.
	int varID;
	ncfd_get_var_info(ncid, varName, arrayLen, &varID);
	//allocate the actual array we will be returning
	double * resultArray= (double*)calloc(*arrayLen, sizeof(double));

	if ((retval = nc_get_var_double(ncid, varID, resultArray))){
		ERR(retval);
	}
	return resultArray;
}




/*
 * Function:  ncdf_read_double_2D
 * --------------------
 * Attempts to read a 2 dimensional double variable that is stored under a particular name in the netCDF file's variables
 *
 *  ncid: netCDF file ID to pull the data from. This is the id given by nc_open after the netCDF file is opened
 *  varName: This is the name of the variable to read
 *  dimLen: pointer to a 2 element array where the total length in number of elements of the resulting arrays will be written (NOT MEMORY SIZE)
 							Note: this memory MUST be allocated allready, and should be of type size_t[2].
 *
 *  returns: a 2D array of all the values of the variable, represented by an array of pointers to arrays. (IE, a double**, or double[][])
 *   if there is an error, the program exits after displaying a message.
 */
double ** ncdf_read_double_2D(int ncid, const char * varName, size_t * dimLen ){
	int retval =0; //Return value variable for NCDF calls.
	int varID;
	int numberOfDims;
	int dimensionIDs[2] = {-1,-1}; //array where we store the dimensionIDs. default to -1
	//Attempt to locate the id of the approprate variable
	if ((retval = nc_inq_varid(ncid, varName, &varID))){
		ERR(retval);
	}
	//check to make sure the variable has only one dim (Critial to assure well-defined behavior for all other ncdf function calls!!!)
	if ((retval = nc_inq_varndims(ncid, varID, &numberOfDims))){
		ERR(retval);
	}
	if (numberOfDims!=2){
		printf("Error in %s at %d while trying to read var %s: CARDAMOM netCDF was trying to read 2D var, but var has %d dimensions.\n", __FILE__, __LINE__, varName,numberOfDims);
		exit(1);
	}
	//Now store all the dimensionIDs (There should be 2)
	if ((retval = nc_inq_vardimid(ncid, varID, dimensionIDs))){
		ERR(retval);
	}
	//Dim 1
	if ((retval = nc_inq_dimlen(ncid, dimensionIDs[0], dimLen))){
		ERR(retval);
	}
	//Dim 2
	if ((retval = nc_inq_dimlen(ncid, dimensionIDs[1], dimLen+1))){ //Beware, dimLen+1 is pointer arithmatic, NOT normal addition
		ERR(retval);
	}

	size_t startIndexes[2] = {0,0}; //This is just the indexes we want to start at, which we pass to nc_get_vara. We will begin at 0,0, and increment only the first dim
	size_t readCount[2] = {1,-1}; //This is just the number of elements we want to read at a time, which we pass to nc_get_vara.
	readCount[1]= dimLen[1]; //We want to read all the elements that are there, so we set it to the len of the second dim
	//allocate the array we will be returning
	double ** resultArray= (double**)calloc(dimLen[0], sizeof(double*));
	for(int idx=0; idx<dimLen[0]; idx++){
		//Allocate and populate the child arrays that hold the data
		resultArray[idx] = (double*)calloc(dimLen[1], sizeof(double));
		if ((retval = nc_get_vara(ncid, varID, startIndexes, readCount, resultArray[idx] ))){
					ERR(retval);
		}
	}
	return resultArray;
}





/*
 * Function:  ncdf_read_single_int_var
 * --------------------
 * Attempts to read a 1 dimensional int variable that is stored under a particular name in the netCDF file's variables
 *
 *  ncid: netCDF file ID to pull the data from. This is the id given by nc_open after the netCDF file is opened
 *  varName: This is the name of the variable to read
 *  defaultVal: default value to return if the requested variable does not exsist
 *
 *  returns: the value of the variable,
 *   if there is an error, the program exits after displaying a message.
 */
int ncdf_read_single_int_var(int ncid, const char * varName, int defaultVal){
	int retval =0; //Return value variable for NCDF calls.
	int varID;
	int result;
	if ((retval = nc_inq_varid(ncid, varName, &varID))){
		if (retval ==NC_ENOTVAR ){
			return defaultVal;
		}
	}
	size_t arrayLen;
	ncfd_get_var_info(ncid, varName, &arrayLen, &varID);
	//No need to allocate an array, just check the size and die if it is wrong
	if (arrayLen>1){
		printf("Error in %s at %d while trying to read var %s: ncdf_read_single_int_var only supports one value, but var has %lu values.\n", __FILE__, __LINE__, varName,arrayLen);
		exit(1);
	}

	if ((retval = nc_get_var_int(ncid, varID, &result))){
		ERR(retval);
	}
	return result;
}


/*
 * Function:  ncdf_read_single_double_var
 * --------------------
 * Attempts to read a scalar double variable that is stored under a particular name in the netCDF file's variables
 *
 *  ncid: netCDF file ID to pull the data from. This is the id given by nc_open after the netCDF file is opened
 *  varName: This is the name of the variable to read
 *  defaultVal: default value to return if the requested variable does not exsist
 *
 *  returns: the value of the variable,
 *   if there is an error, the program exits after displaying a message.
 */
double ncdf_read_single_double_var(int ncid, const char * varName, double defaultVal ){
	int retval =0; //Return value variable for NCDF calls.
	int varID;
	double result;
	if ((retval = nc_inq_varid(ncid, varName, &varID))){
		if (retval ==NC_ENOTVAR ){
			return defaultVal;
		}
	}
	size_t arrayLen;
	ncfd_get_var_info(ncid, varName, &arrayLen, &varID);
	//No need to allocate an array, just check the size and die if it is wrong
	if (arrayLen>1){
		printf("Error in %s at %d while trying to read var %s: ncdf_read_single_double_var only supports one value, but var has %lu values.\n", __FILE__, __LINE__, varName,arrayLen);
		exit(1);
	}
	if ((retval = nc_get_var_double(ncid, varID, &result))){
		ERR(retval);
	}
	return result;
}




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

	DATA->ABGB=ncdf_read_single_double_var(ncid, "ABGB", -9999.0);
	DATA->CH4=ncdf_read_single_double_var(ncid, "CH4", -9999.0);
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


	return 0;
}
