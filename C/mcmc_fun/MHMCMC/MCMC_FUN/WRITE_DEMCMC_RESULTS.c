#pragma once
#include <netcdf.h>

//TODO: break this into it's own shared file! See cardamom_run_model.c
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
int paramDimID,; //Dim ID numbers, must be populated each run
if (!inited){  
        inited=1;
        FAILONERROR(nc_create(MCO.outfile,NC_CLOBBER, &ncid ));

        FAILONERROR(nc_def_dim(ncid,"Parameter",PI.npars,&paramDimID));
        FAILONERROR(nc_def_dim(ncid,"Sample",NC_UNLIMITED,&paramDimID));

}else{
        FAILONERROR(nc_open(MCO.outfile,NC_WRITE, &ncid ));
         
        //TODO: populate DIM IDs
}







//MCO.nchains is the slab 







int n;

FILE *fileout=fopen(MCO.outfile,"ab");
for (n=0;n<PI.npars*MCO.nchains;n++){
        fwrite(&PARS[n],1,sizeof(double),fileout);}
    
/*writing likelyhood*/
/*NOTE: As of July 11th 2014, probability no longer written to file*/
/*Probability is a "re-derivable" quantity, therefore if needed, it can
either (a) be re-derived using the MODEL_LIKELIHOOD function, or (b) 
written to a separate file.*/
/*fwrite(&PROB,1,sizeof(double),fileout);*/

fclose(fileout);


return 0;


}
