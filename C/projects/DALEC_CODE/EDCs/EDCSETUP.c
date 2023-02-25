#pragma once
int EDCSETUP(DATA DATA, struct EDCDIAGNOSTIC ** EDCD){

/*TO DO HERE: set EDCD.EDCSWITCHES to DATA.OTHERPRIORS fields 30 to 50*/
/*DON'T FORGET TO DO THE SAME IN CDEA files*/
/*see if it is possible to initiate both using the same function*/
/*E.g. INITIATE_EDC_SWITCHES*/

static struct EDCDIAGNOSTIC EDCDmem;
EDCDmem.DIAG=DATA.ncdf_data.EDCDIAG;
int n;
/*EDC switches are stored in DATA->parpriors (positions 31-50);*/
/*switch all on if EDCDIAG<2 , otherwise, switch on/off according to EDCSWITCH*/
/*EDCSETUP function will be included in DALEC_ALL_LIKELIHOOD.c*/

/*
printf("*****")
printf("*****Note on status of DALEC EDC settings*******\n");
printf("*****")
printf("*****")
printf("*****")
printf("******Only default values used for now, introduce EDC settings via netcdf system when needed\n");
printf("*****")
*/


for (n=0;n<100;n++){EDCDmem.SWITCH[n]=1;}
// if (EDCDmem.DIAG==2){for (n=0;n<20;n++){EDCDmem.SWITCH[n]=DATA.otherpriors[n+30];}}

//if (EDCDmem.DIAG==2){for (n=0;n<20;n++){EDCDmem.SWITCH[n]=0;}}


/*EQF is stored in the "DATA.otherpriorunc" fields associated with EDCs 7-12*/
/*default value is 2*/
//EDCDmem.EQF=DATA.ncdf_data.EDC_EQF;

//printf("EDCD->EQF* = %2.2f\n",EDCDmem.EQF);


/*Default structure has all EDC=1, and 100 EDCs*/
EDCDmem.nedc=100;
for (n=0;n<EDCDmem.nedc;n++){EDCDmem.PASSFAIL[n]=1;}


/*Double pointer = contents of EDCD reassigned to static struct*/
*EDCD=&EDCDmem;


return 0;
}
