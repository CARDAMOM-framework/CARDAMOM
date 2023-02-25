
#include "../../auxi_fun/okcheck.c"
#include "math.h"
#include <stdio.h>
#include "CARDAMOM_READ_NETCDF_DATA.c"
/*syntax CARDAMOM_READ_BINARY_CARDADATA(char *filename,CARDADATA *CARDADATA)*/




/* the gateway function */
/*NOTE: this function can be used with any valid model ID (stored in CARDADATA.ID)*/
int main(int argc, char *files[])
{


/*storing command line inputs as 2 files*/
char metfile[200];strcpy(metfile,files[1]);
//printf("reading %s",metfile );


/*declaring data structure*/
NETCDF_DATA CARDADATA;

CARDAMOM_READ_NETCDF_DATA(metfile,&CARDADATA);

printf("CH4_STRUCT has uncer: %f first 3 vals are: %f %f %f",CARDADATA.CH4.Uncertainty, CARDADATA.CH4.values[0],CARDADATA.CH4.values[1],CARDADATA.CH4.values[2] );

return 0;

}
