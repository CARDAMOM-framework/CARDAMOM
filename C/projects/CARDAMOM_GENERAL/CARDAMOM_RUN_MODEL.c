#include "../../auxi_fun/okcheck.c"
#include "math.h"
#include <stdio.h>
#include "CARDAMOM_READ_BINARY_DATA.c"
#include "CARDAMOM_READ_NETCDF_DATA.c"

/*syntax CARDAMOM_READ_BINARY_CARDADATA(char *filename,CARDADATA *CARDADATA)*/

//Helper method, primarily for use identifying file extensions. Adapted from https://stackoverflow.com/questions/744766/how-to-compare-ends-of-strings-in-c for ease and edge case management
int StringEndsWith(const char *str, const char *suffix)
{
    if (!str || !suffix)
        return 0;
    size_t lenstr = strlen(str);
    size_t lensuffix = strlen(suffix);
    if (lensuffix >  lenstr)
        return 0;
    return strncmp(str + lenstr - lensuffix, suffix, lensuffix) == 0;
}




/* the gateway function */
/*NOTE: this function can be used with any valid model ID (stored in CARDADATA.ID)*/
int main(int argc, char *files[])
{

/*declaring loop variable n*/
int n, nn;

/*storing command line inputs as 2 files*/
char metfile[1000];strcpy(metfile,files[1]);
char parfile[1000];strcpy(parfile,files[2]);

//This determines if we are dealing with netCDF files, or traditional cardamom files. By default, we assume the older binary format
int metIsCDF =0;
if (StringEndsWith(metfile, ".nc.cbf")) {
  metIsCDF=1;
}


/*declaring data structure*/
DATA CARDADATA;

/*if (&CARDADATA==0){printf("CARDADATA not assigned, expect error!\n");}
*//*Initialize data structure*/
int OK;
OK=INITIALIZE_DATA_STRUCT(&CARDADATA);
okcheck(OK,"CHECK: DATA structure successfully initialized");

/*step 1 - read the met file & initialize model fields here*/
/*NOTE: metfile contains information on which CARDAMOM model is being run*/
/*therefore this function is transferable to any model type*/
/*NOTE: parameter minimum and maximum values also loaded as a function of CARDADATA.ID by
the CARDAMOM_MODEL_LIBRARY.c function*/

if (metIsCDF){
  //Data file is of the newer NetCDF format
  CARDAMOM_READ_NETCDF_DATA(metfile, &(CARDADATA.ncdf_data));
}else{
  //parse the data file as the older binary format
  CARDAMOM_READ_BINARY_DATA(metfile,&CARDADATA);
}


/*step 2 - read (first time) parameter file here*/

FILE *fd;
/*opening file*/
fd=fopen(parfile,"rb");
/*checking that this has been done successfully*/
filediag(fd,parfile);

/*identifying number of elements in file*/
fseek(fd,0,SEEK_END);
/*number of parameter vectors to run*/
int N=ftell(fd)/(sizeof(double)*(CARDADATA.nopars));

/*comments for display*/
int verbal=1;
if (verbal==1){printf("number of parvecs to run = %d\n",N);}

/*resetting file reader to start*/
fseek(fd,0,SEEK_SET);
/*parameter file will be closed later - in the meantime parameters will simply be read N times*/

/*declaring pars file*/
double *pars=calloc(CARDADATA.nopars,sizeof(double));





/*STEP 3 - declaring flux, pool and edc files*/
/*note - these should be input at command line as variables*/


char fluxfile[1000];
char poolfile[1000];
char edcdfile[1000];
char probfile[1000];

if (argc-1>2){
strcpy(fluxfile,files[3]);
strcpy(poolfile,files[4]);
strcpy(edcdfile,files[5]);
strcpy(probfile,files[6]);}
else{
strcpy(fluxfile,"tempcardafluxfile.bin");
strcpy(poolfile,"tempcardapoolfile.bin");
strcpy(edcdfile,"tempcardaedcdfile.bin");
strcpy(probfile,"tempcardaprobfile.bin");}

FILE *fdf=fopen(fluxfile,"wb");
filediag(fdf,fluxfile);
FILE *fdp=fopen(poolfile,"wb");
filediag(fdp,poolfile);
FILE *fde=fopen(edcdfile,"wb");
filediag(fde,edcdfile);
FILE *fdpro=fopen(probfile,"wb");
filediag(fdpro,probfile);


/*STEP 4 - RUNNING CARDADATA.MLF N TIMES*/
for (n=0;n<N;n++){

/*step 4.1 - reading parameter vector from file*/
fread(pars,sizeof(double),CARDADATA.nopars,fd);


/*step 4.2 - running MODEL_LIKELIHOOD_FUNCTION (e.g. DALEC_CDEA_MLF) with parameter vector from file*/
/*This is stored in CARDADATA.MLF*/
/*Setting EDCDIAG = 1 to ensure full model run*/
CARDADATA.EDCDIAG=1;


CARDADATA.MLF(CARDADATA,pars);

/*step 4.3 - writing DALEC fluxes and pools to file*/
fwrite(CARDADATA.M_FLUXES,sizeof(double),CARDADATA.nodays*CARDADATA.nofluxes,fdf);
fwrite(CARDADATA.M_POOLS,sizeof(double),(CARDADATA.nodays+1)*CARDADATA.nopools,fdp);
fwrite(CARDADATA.M_EDCD,sizeof(int),100,fde);
fwrite(CARDADATA.M_P,sizeof(double),1,fdpro);
/*writing as double - platform issues related to "int"*/
/*double MEDCD[100];for (nn=0;nn<100;nn++){MEDCD[nn]=(double)CARDADATA.M_EDCD[nn];};fwrite(MEDCD,sizeof(double),100,fde);
*/

/*STEP 4 completed*/
}


/*STEP 5 - close all files*/
fclose(fdf);
fclose(fdp);
fclose(fde);
fclose(fdpro);
fclose(fd);

/*Step 6: Free memory*/
/*exhaustive list of all malloc/calloc used fields*/
free(pars);
FREE_DATA_STRUCT(CARDADATA);





return 0;

}
