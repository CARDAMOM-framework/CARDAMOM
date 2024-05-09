#pragma once
#include <netcdf.h>

////TODO: break this into it's own shared file! See cardamom_run_model.c
/* Handle netCDF library errors by printing an error message and exiting with a
 * non-zero status.*/
#define ERREXITCODE 2
#define NCDFERR(e) {printf("Error in %s at %d: %s\n", __FILE__, __LINE__, nc_strerror(e));}
#define FILE_NAME_MAX_LEN 1000

//This is a tiny macro used to do error handeling for netCDF methods with the standard format of returning a result code.
//It helps make the code look less insane by allowing one-line calls
//This as a macro because we want the __FILE__ and __LINE__ macros to work
#define FAILONERROR(ncretval) if (ncretval != NC_NOERR) {NCDFERR(ncretval); exit(ERREXITCODE);}
//This variant will not die, but will still thow a message at the user about the problem.
#define WARNONERROR(ncretval) if (ncretval != NC_NOERR) {NCDFERR(ncretval);}


//This scans the string and removes all instances of the string toFind, and replaces them with the single char toReplace.
void str_inplace_replace(char * str, const char * toFind, const char toReplace){
  //Yeah this implementation is N^2... but we have a small fixed max N, so don't @ me.
  char* substring=str;
  while ((substring = strstr(substring, toFind))){
    memmove(substring, substring+1,strlen(substring)); //Memmove needed due to overlapping strings. Move the whole string, with null term
    substring[0]=toReplace; //put in the filler char
  }

}





int WRITE_DEMCMC_RESULTS(double *PARS,PARAMETER_INFO PI,MCMC_OPTIONS MCO){

  int ncid = 0; //This is the netcdf id num
  int ncretval = 0; //This is a reused variable for the return value of ncdf methods.
  static int inited = 0;
  int paramDimID,sampleDimID; //Dim ID numbers, must be populated each invocation

  int parsVarID; // Variable ID numbers, also must be repopulated each invocation

  if (!inited){  
          inited=1;
          FAILONERROR(nc_create(MCO.outfile,NC_CLOBBER, &ncid ));
          FAILONERROR(nc_def_dim(ncid,"Parameter",PI.npars,&paramDimID));
          FAILONERROR(nc_def_dim(ncid,"Sample",NC_UNLIMITED,&sampleDimID));
          FAILONERROR(nc_def_var(ncid,"Parameters",NC_DOUBLE,2,(const int[]){sampleDimID,paramDimID},&parsVarID));

          //End NetCDF definition phase, in order to allow for writting
          nc_enddef(ncid);

  }else{
          //dims and data already exist, so we only need to reacquire the IDs
          FAILONERROR(nc_open(MCO.outfile,NC_WRITE, &ncid ));
          FAILONERROR(nc_inq_dimid(ncid,"Parameter",&paramDimID));
          FAILONERROR(nc_inq_dimid(ncid,"Sample", &sampleDimID));
          FAILONERROR(nc_inq_varid(ncid,"Parameters",&parsVarID));

  }
  //Decide on where to put our write based on the current length of the unlimited dimension Sample
  size_t currentSamples;
  FAILONERROR(nc_inq_dimlen(ncid,sampleDimID, &currentSamples));
  //Do the write. Remember to start at currentSamples
  FAILONERROR(nc_put_vara_double(ncid,parsVarID,(const size_t[]){currentSamples,0}, (const size_t[]){MCO.nchains,PI.npars},PARS ));

  FAILONERROR(nc_close(ncid));

  return 0;


}


	//Is identical function to WRITE_DEMCMC_RESULTS(PARS,PI,MCO);}
	//Except it writes one instance of "PARS" and overwrites previous file
	//Include also attribute with N.ITER
	//If possible, include attribute with random seed (this is optional).
int WRITE_DEMCMC_RESTART(double *PARS,PARAMETER_INFO PI,MCMC_OPTIONS MCO){

  int ncid = 0; //This is the netcdf id num
  int ncretval = 0; //This is a reused variable for the return value of ncdf methods.
  int paramDimID; //Dim ID number, must be populated each invocation

  int parsVarID; // Variable ID numbers, also must be repopulated each invocation

  FAILONERROR(nc_create(MCO.startfile,NC_CLOBBER, &ncid ));
  FAILONERROR(nc_def_dim(ncid,"Parameter",PI.npars,&paramDimID));
  FAILONERROR(nc_def_var(ncid,"Parameters",NC_DOUBLE,1,(const int[]){paramDimID},&parsVarID));

  //End NetCDF definition phase, in order to allow for writting
  nc_enddef(ncid);

  //Do the write.
  FAILONERROR(nc_put_vara_double(ncid,parsVarID,(const size_t[]){0}, (const size_t[]){PI.npars},PARS ));

  FAILONERROR(nc_close(ncid));

  return 0;


}