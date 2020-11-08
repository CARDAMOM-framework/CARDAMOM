#include <assert.h>
#include "../../auxi_fun/oksofar.c"
#include "../../auxi_fun/okcheck.c"
#include "../../auxi_fun/seedrandomnumber.c"

/*defines all the structures, i.e. DATA, MCOPT, PI*/

#include "../../mcmc_fun/MHMCMC/MCMC_FUN/MCMCOPT.c"
#include "MCMC_SETUP/MCMC_MODULES.c"


#include "../../mcmc_fun/MHMCMC/MCMC_FUN/MHMCMC.c"
#include "../../mcmc_fun/MHMCMC/MCMC_FUN/MHMCMC_NOBOUNCE.c"
/*Temporarily de-activating to write EDC sampler*/
#include "../../mcmc_fun/MHMCMC/MCMC_FUN/DEMCMC.c"
#include "../../mcmc_fun/MHMCMC/MCMC_FUN/MHMCMC_NODIMADAPT.c"
#include "../../mcmc_fun/MHMCMC/MCMC_FUN/MHMCMC_SLOWADAPT.c"
#include "../../mcmc_fun/MHMCMC/MCMC_FUN/MHMCMC_103.c"
#include "../../mcmc_fun/MHMCMC/MCMC_FUN/MHMCMC_104.c"
#include "../../mcmc_fun/MHMCMC/MCMC_FUN/MHMCMC_105.c"
#include "../../mcmc_fun/MHMCMC/MCMC_FUN/MHMCMC_106.c"
#include "../../mcmc_fun/MHMCMC/MCMC_FUN/MHMCMC_107.c"
#include "../../mcmc_fun/MHMCMC/MCMC_FUN/MHMCMC_108.c"
#include "../../mcmc_fun/MHMCMC/MCMC_FUN/MHMCMC_109.c"
#include "../../mcmc_fun/MHMCMC/MCMC_FUN/MHMCMC_110.c"
#include "../../mcmc_fun/MHMCMC/MCMC_FUN/MHMCMC_111.c"
#include "../../mcmc_fun/MHMCMC/MCMC_FUN/MHMCMC_112.c"
#include "../../mcmc_fun/MHMCMC/MCMC_FUN/MHMCMC_113.c"
#include "../../mcmc_fun/MHMCMC/MCMC_FUN/MHMCMC_114.c"
#include "../../mcmc_fun/MHMCMC/MCMC_FUN/MHMCMC_115.c"
#include "../../mcmc_fun/MHMCMC/MCMC_FUN/MHMCMC_116.c"
#include "../../mcmc_fun/MHMCMC/MCMC_FUN/MHMCMC_117.c"
#include "../../mcmc_fun/MHMCMC/MCMC_FUN/MHMCMC_118.c"
#include "../../mcmc_fun/MHMCMC/MCMC_FUN/MHMCMC_119.c"


int main(int argc,char *CLA[]){
/*To correctly set-up the MHMCMC*/

/*inputs*/
/*1. met file in*/
/*2. results file out*/
/*3. number of MCMC solutions requested*/
/*4. print-to-screen frequency*/
/*5. write-to-file frequency*/


/*OK is output flag from all functions*/
int OK;


/*SETTING number of command line inputs as char in CLA[0]*/
sprintf(CLA[0],"%d",argc-1);
/*declaring CARDAMOM Binary Format (.cbf) file*/
char CBFfile[1000];
/*setting default filename*/
/*this mode can be routinely used for testing*/
if (argc-1<1){strcpy(CBFfile,"MCMC_SETUP/TEST_BINARY_DATASET.cbf");}
/*otherwise first argument is filename*/
else {strcpy(CBFfile,CLA[1]);}





/*defining MCMC_OPTIONS structure*/
MCMC_OPTIONS MCOPT;

/*ID=1 adaptive MHMCMC*/
/*ID=2 DE-MCMC*/
/*Hard-coding number of chains for now (for DEMCMC)*/
OK=READ_MCOPT(&MCOPT,CLA);
if (MCOPT.mcmcid==1){MCOPT.nchains=1;}
if (MCOPT.mcmcid==101){MCOPT.nchains=1;}
if (MCOPT.mcmcid==102){MCOPT.nchains=1;}
if (MCOPT.mcmcid==103){MCOPT.nchains=1;}
if (MCOPT.mcmcid==104){MCOPT.nchains=1;}
if (MCOPT.mcmcid==105){MCOPT.nchains=1;}
if (MCOPT.mcmcid==106){MCOPT.nchains=1;}
if (MCOPT.mcmcid==107){MCOPT.nchains=1;}
if (MCOPT.mcmcid==108){MCOPT.nchains=1;}
if (MCOPT.mcmcid==109){MCOPT.nchains=1;}
if (MCOPT.mcmcid==110){MCOPT.nchains=1;}
if (MCOPT.mcmcid==111){MCOPT.nchains=1;}
if (MCOPT.mcmcid==112){MCOPT.nchains=1;}
if (MCOPT.mcmcid==113){MCOPT.nchains=1;}
if (MCOPT.mcmcid==114){MCOPT.nchains=1;}
if (MCOPT.mcmcid==115){MCOPT.nchains=1;}
if (MCOPT.mcmcid==116){MCOPT.nchains=1;}
if (MCOPT.mcmcid==117){MCOPT.nchains=1;}
if (MCOPT.mcmcid==118){MCOPT.nchains=1;}
if (MCOPT.mcmcid==119){MCOPT.nchains=1;}
if (MCOPT.mcmcid==3){MCOPT.nchains=1;}
else if (MCOPT.mcmcid==2){MCOPT.nchains=40;}


okcheck(OK,"MDF options structure read successfully");


/*defining the MCMC output structure*/
MCMC_OUTPUT MCOUT;

/*These lines guarantee high frequency random generator seeding*/
if (argc-1<2){seedrandomnumber(CBFfile);}else{seedrandomnumber(CLA[2]);}

/*Defining all MCMC components*/
/*USER DEFINED: SETUP MCMC - templates provides*/
/*NOTE  : READ_PARI_DATA function is stored in DALEC_CDEA_TEMPLATE/MCMC_SETUP/MCMC_MODULES.c*/
/*TO DO : (a) read DATA first - note that this includes model specific fields, such as nomet, nopars, etc.
        these are all loaded via the CARDAMOM_MODEL_LIBRARY(DATA) function*/
/*      : (b) read PI based on DATA*/




/***********CARDATA STRUCTURE*************/

/*defining data structure*/
DATA DATA;
/*Initialize data structure - this function is found in CARDAMOM_READ_BINARY_DATA*/
OK=INITIALIZE_DATA_STRUCT(&DATA);
okcheck(OK,"Main data structure initialized");

/*read cardamom data from file*/
/*Function also performs and displays basic checks*/
OK=CARDAMOM_READ_BINARY_DATA(CBFfile,&DATA);
okcheck(OK,"Main data structure read successfully");

printf("CARDAMOM MODEL ID = %i\n",DATA.ID);
printf("MCMC ID = %i\n",MCOPT.mcmcid);


/***************PI STRUCTURE AND MLF*****************/


/*defining parameter structure*/
/*this structure is defined as part of the MCMC
Full function can be found in mcmc_fun/MHMCMC/MCMC_FUN/MCMC_OPT.c*/
PARAMETER_INFO PI;

/*initializing structure with correct PI fields (as required by MHMCMC)*/
/*Function is in MCMC_MODULES.c*/
OK=INITIALIZE_PI_STRUCT(&PI,&DATA,&MCOPT);
okcheck(OK,"Parameter info structure initialized");


/*choose model likelihood here*/
/*the MLF function determines the probability of any model output based on data and/or other constraints*/
/*In future versions: this should be done inside CARDAMOM_READ_BINARY_DATA*/



/*READ_PARI_DATA and READ_MCOPT should now be generic for all model types*/
/*CONTAINS "FIND_EDC_INITIAL_VALUES(*DATA,PI);"*/
OK=READ_PARI_DATA(&PI, &DATA, &MCOUT, &MCOPT,CLA);
okcheck(OK,"READ_PARI_DATA successfully executed");


/*calling the MHMCMC here*/

printf("about to start MCMC\n");
printf("Prescribed option = %i\n",MCOPT.mcmcid);
switch (MCOPT.mcmcid){
case 1:
printf("about to start MHMCMC\n");
MHMCMC(DATA.MLF,DATA,PI,MCOPT,&MCOUT);
printf("completed MHMCMC\n");
break;
case 101:
printf("about to start MHMCMC (no bounce at parameter walls)\n");
MHMCMC_NOBOUNCE(DATA.MLF,DATA,PI,MCOPT,&MCOUT);
printf("completed MHMCMC\n");
break;
case 102:
printf("about to start MHMCMC (slow adaptation)\n");
MHMCMC_SLOWADAPT(DATA.MLF,DATA,PI,MCOPT,&MCOUT);
printf("completed MHMCMC\n");
break;
case 103:
printf("about to start MHMCMC (slow adaptation)\n");
MHMCMC_103(DATA.MLF,DATA,PI,MCOPT,&MCOUT);
printf("completed MHMCMC\n");
break;
case 104:
printf("about to start MHMCMC (slow adaptation)\n");
MHMCMC_104(DATA.MLF,DATA,PI,MCOPT,&MCOUT);
printf("completed MHMCMC\n");
break;
case 105:
printf("about to start MHMCMC (id = 105)\n");
MHMCMC_105(DATA.MLF,DATA,PI,MCOPT,&MCOUT);
printf("completed MHMCMC 105\n");
break;
case 106:
printf("about to start MHMCMC (id = 106)\n");
MHMCMC_106(DATA.MLF,DATA,PI,MCOPT,&MCOUT);
printf("completed MHMCMC 106\n");
break;
case 107:
printf("about to start MHMCMC (id = 107)\n");
MHMCMC_107(DATA.MLF,DATA,PI,MCOPT,&MCOUT);
printf("completed MHMCMC 107\n");
break;

case 108:
printf("about to start MHMCMC (id = 108)\n");
MHMCMC_108(DATA.MLF,DATA,PI,MCOPT,&MCOUT);
printf("completed MHMCMC 108\n");
break;

case 109:
printf("about to start MHMCMC (id = 109)\n");
MHMCMC_109(DATA.MLF,DATA,PI,MCOPT,&MCOUT);
printf("completed MHMCMC 109\n");
break;

case 110:
printf("about to start MHMCMC (id = 110)\n");
MHMCMC_110(DATA.MLF,DATA,PI,MCOPT,&MCOUT);
printf("completed MHMCMC 110\n");
break;
case 111:
printf("about to start MHMCMC (id = 111)\n");
MHMCMC_111(DATA.MLF,DATA,PI,MCOPT,&MCOUT);
printf("completed MHMCMC 111\n");
break;

case 112:
printf("about to start MHMCMC (id = 112)\n");
MHMCMC_112(DATA.MLF,DATA,PI,MCOPT,&MCOUT);
printf("completed MHMCMC 112\n");
break;

case 113:
printf("about to start MHMCMC (id = 113)\n");
MHMCMC_113(DATA.MLF,DATA,PI,MCOPT,&MCOUT);
printf("completed MHMCMC 113\n");
break;

case 114:
printf("about to start MHMCMC (id = 114)\n");
MHMCMC_114(DATA.MLF,DATA,PI,MCOPT,&MCOUT);
printf("completed MHMCMC 114\n");
break;

case 115:
printf("about to start MHMCMC (id = 115)\n");
MHMCMC_115(DATA.MLF,DATA,PI,MCOPT,&MCOUT);
printf("completed MHMCMC 115\n");
break;

case 116:
printf("about to start MHMCMC (id = 116)\n");
MHMCMC_116(DATA.MLF,DATA,PI,MCOPT,&MCOUT);
printf("completed MHMCMC 116\n");
break;

case 117:
printf("about to start MHMCMC (id = 117)\n");
MHMCMC_117(DATA.MLF,DATA,PI,MCOPT,&MCOUT);
printf("completed MHMCMC 117\n");
break;

case 118:
printf("about to start MHMCMC (id = 118)\n");
MHMCMC_118(DATA.MLF,DATA,PI,MCOPT,&MCOUT);
printf("completed MHMCMC 118\n");
break;

case 119:
printf("about to start MHMCMC (id = 119)\n");
MHMCMC_119(DATA.MLF,DATA,PI,MCOPT,&MCOUT);
printf("completed MHMCMC 119\n");
break;

case 3:
printf("about to start MHMCMC (no dim adapt)\n");
MHMCMC_NODIMADAPT(DATA.MLF,DATA,PI,MCOPT,&MCOUT);
printf("completed MHMCMC\n");
break;
case 2:
printf("about to start DEMCMC\n");
DEMCMC(DATA.MLF,DATA,PI,MCOPT,&MCOUT);

/*printf("DEMCMC temporarily disconnected, need to de-bug, correct and re-introduce");
printf("completed DEMCMC\n");
break;*/
break;
default:
printf("Error: no valid mcmcid value prescribed...\n");

}
printf("MCMC complete\n");
/*???????*/
/*User Defined function needed to clean up memory*/
MEMORY_CLEANUP(DATA,PI,MCOPT,MCOUT);


return 0;

}





