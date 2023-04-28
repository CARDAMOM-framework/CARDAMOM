#pragma once

int INITIALIZE_PARAMETER_FIELDS(DATA *DATA){
/*initializing parmin and parmax fields*/
DATA->parmin=NULL;
DATA->parmax=NULL;
DATA->parname=NULL;
DATA->parmin=calloc(DATA->nopars,sizeof(double));
DATA->parmax=calloc(DATA->nopars,sizeof(double));
DATA->parname=calloc(DATA->nopars,sizeof(char *));
return 0;}

//#include "ADD_PARAMETER_TO_STACK.c"
//#include "WRITE_MODEL_STACKS_TO_FILE.c"
/*Including all model PARS_INFO_MMMM (model) files*/
//#include "../CARDAMOM_MODELS/DALEC/DALEC_101/MODEL_INFO_101.c"
//#include "../CARDAMOM_MODELS/DALEC/DALEC_400/MODEL_INFO_400.c"
//#include "../CARDAMOM_MODELS/DALEC/DALEC_803/MODEL_INFO_803.c"
//#include "../CARDAMOM_MODELS/DALEC/DALEC_804/MODEL_INFO_804.c"
//#include "../CARDAMOM_MODELS/DALEC/DALEC_805/MODEL_INFO_805.c"
//#include "../CARDAMOM_MODELS/DALEC/DALEC_806/MODEL_INFO_806.c"
//#include "../CARDAMOM_MODELS/DALEC/DALEC_807/MODEL_INFO_807.c"
//#include "../CARDAMOM_MODELS/DALEC/DALEC_808/MODEL_INFO_808.c"

//#include "../CARDAMOM_MODELS/DALEC/DALEC_809/MODEL_INFO_809.c"
//#include "../CARDAMOM_MODELS/DALEC/DALEC_811/MODEL_INFO_811.c"
//#include "../CARDAMOM_MODELS/DALEC/DALEC_812/MODEL_INFO_812.c"
//#include "../CARDAMOM_MODELS/DALEC/DALEC_813/MODEL_INFO_813.c"
//#include "../CARDAMOM_MODELS/DALEC/DALEC_810/MODEL_INFO_810.c"
//#include "../CARDAMOM_MODELS/DALEC/DALEC_820/MODEL_INFO_820.c"
    //#include "../CARDAMOM_MODELS/DALEC/DALEC_821/MODEL_INFO_821.c"
//#include "../CARDAMOM_MODELS/DALEC/DALEC_830/MODEL_INFO_830.c"
//#include "../CARDAMOM_MODELS/DALEC/DALEC_831/MODEL_INFO_831.c"
//#include "../CARDAMOM_MODELS/DALEC/DALEC_840/MODEL_INFO_840.c"
//#include "../CARDAMOM_MODELS/DALEC/DALEC_1200/MODEL_INFO_1200.c"

//#include "../CARDAMOM_MODELS/DALEC/DALEC_1000/MODEL_INFO_1000.c"
//#include "../CARDAMOM_MODELS/DALEC/DALEC_1001/MODEL_INFO_1001.c"
//#include "../CARDAMOM_MODELS/DALEC/DALEC_1002/MODEL_INFO_1002.c"
//#include "../CARDAMOM_MODELS/DALEC/DALEC_1003/MODEL_INFO_1003.c"
//#include "../CARDAMOM_MODELS/DALEC/DALEC_1004/MODEL_INFO_1004.c"
//#include "../CARDAMOM_MODELS/DALEC/DALEC_1005/MODEL_INFO_1005.c"
                #include "../CARDAMOM_MODELS/DALEC/DALEC_1006/MODEL_INFO_1006.c"
//#include "../CARDAMOM_MODELS/DALEC/DALEC_1009/MODEL_INFO_1009.c"
//#include "../CARDAMOM_MODELS/DALEC/DALEC_1010/MODEL_INFO_1010.c"
//#include "../CARDAMOM_MODELS/DALEC/DALEC_1011/MODEL_INFO_1011.c"
   //            #include "../CARDAMOM_MODELS/DALEC/DALEC_1012/MODEL_INFO_1012.c"
    //#include "../CARDAMOM_MODELS/DALEC/DALEC_1015/MODEL_INFO_1015.c"
    //#include "../CARDAMOM_MODELS/DALEC/DALEC_1016/MODEL_INFO_1016.c"
    //#include "../CARDAMOM_MODELS/DALEC/DALEC_1017/MODEL_INFO_1017.c"
               #include "../CARDAMOM_MODELS/DALEC/DALEC_1025/MODEL_INFO_1025.c"
//#include "../CARDAMOM_MODELS/DALEC/DALEC_1030/MODEL_INFO_1030.c"
//#include "../CARDAMOM_MODELS/DALEC/DALEC_1031/MODEL_INFO_1031.c"
      //          #include "../CARDAMOM_MODELS/DALEC/DALEC_1032/MODEL_INFO_1032.c"
//#include "../CARDAMOM_MODELS/DALEC/DALEC_1040/MODEL_INFO_1040.c"
    //#include "../CARDAMOM_MODELS/DALEC/DALEC_1060/MODEL_INFO_1060.c"
    //#include "../CARDAMOM_MODELS/DALEC/DALEC_1080/MODEL_INFO_1080.c"
    //#include "../CARDAMOM_MODELS/DALEC/DALEC_1100/MODEL_INFO_1100.c"
    //#include "../CARDAMOM_MODELS/DALEC/DALEC_1101/MODEL_INFO_1101.c"
    //#include "../CARDAMOM_MODELS/DALEC/DALEC_1102/MODEL_INFO_1102.c"
    //#include "../CARDAMOM_MODELS/DALEC/DALEC_1103/MODEL_INFO_1103.c"
    //#include "../CARDAMOM_MODELS/DALEC/DALEC_1105/MODEL_INFO_1105.c"
    //#include "../CARDAMOM_MODELS/DALEC/DALEC_1106/MODEL_INFO_1106.c"

/*This function attributes model specific variables based
 * on the ID number stored in ID*/
/*General note: The code IDxMA (where x is the model ID) is kept above the model attribute
line. The reason for this is to ensure that Matlab can easily access this code*/



int CARDAMOM_MODEL_LIBRARY(DATA *DATA){


int status=0;
int ID = DATA->ncdf_data.ID;
if (ID==0){printf("No model prescribed, expect error!");}
//if (ID==101  ){MODEL_INFO_101(DATA);}
//else if (ID==400  ){MODEL_INFO_400(DATA);}
//else if (ID==803  ){MODEL_INFO_803(DATA);}
//else if (ID==804  ){MODEL_INFO_804(DATA);}
//else if (ID==805  ){MODEL_INFO_805(DATA);}
//else if (ID==806  ){MODEL_INFO_806(DATA);}
//else if (ID==807  ){MODEL_INFO_807(DATA);}
//else if (ID==808  ){MODEL_INFO_808(DATA);}
//else if (ID==809  ){MODEL_INFO_809(DATA);}
//else if (ID==810  ){MODEL_INFO_810(DATA);}
//else if (ID==811  ){MODEL_INFO_811(DATA);}
//else if (ID==812  ){MODEL_INFO_812(DATA);}
//else if (ID==813  ){MODEL_INFO_813(DATA);}
//else if (ID==820  ){MODEL_INFO_820(DATA);}
    //else if (ID==821  ){MODEL_INFO_821(DATA);}
//else if (ID==830  ){MODEL_INFO_830(DATA);}
//else if (ID==831  ){MODEL_INFO_831(DATA);}
//else if (ID==840  ){MODEL_INFO_840(DATA);}
//else if (ID==1000  ){MODEL_INFO_1000(DATA);}
//else if (ID==1001  ){MODEL_INFO_1001(DATA);}
//else if (ID==1002  ){MODEL_INFO_1002(DATA);}
//else if (ID==1003  ){MODEL_INFO_1003(DATA);}
//else if (ID==1004  ){MODEL_INFO_1004(DATA);}
      //else if (ID==1005  ){MODEL_INFO_1005(DATA);}
       else if (ID==1006  ){MODEL_INFO_1006(DATA);}
//else if (ID==1009  ){MODEL_INFO_1009(DATA);}
//      else if (ID==1012  ){MODEL_INFO_1012(DATA);}
//else if (ID==1015  ){MODEL_INFO_1015(DATA);}
//else if (ID==1016  ){MODEL_INFO_1016(DATA);}
//else if (ID==1017  ){MODEL_INFO_1017(DATA);}
       else if (ID==1025  ){MODEL_INFO_1025(DATA);}
//else if (ID==1030  ){MODEL_INFO_1030(DATA);}
//else if (ID==1031  ){MODEL_INFO_1031(DATA);}
   //   else if (ID==1032  ){MODEL_INFO_1032(DATA);}
//else if (ID==1040  ){MODEL_INFO_1040(DATA);}
//else if (ID==1060  ){MODEL_INFO_1060(DATA);}
//else if (ID==1080  ){MODEL_INFO_1080(DATA);}
//else if (ID==1010  ){MODEL_INFO_1010(DATA);}
//else if (ID==1011  ){MODEL_INFO_1011(DATA);}
//else if (ID==1200  ){MODEL_INFO_1200(DATA);}
        //else if (ID==1100  ){MODEL_INFO_1100(DATA);}
    //else if (ID==1101  ){MODEL_INFO_1101(DATA);}
    //else if (ID==1102  ){MODEL_INFO_1102(DATA);}
    //else if (ID==1103  ){MODEL_INFO_1103(DATA);}
    //else if (ID==1105  ){MODEL_INFO_1105(DATA);}
        //else if (ID==1106  ){MODEL_INFO_1106(DATA);}
        else {status=1;
        printf("*********************************************\n");
        printf("No loaded in CARDAMOM_MODEL_LIBRARY.c, expect error!\n");
        printf("*********************************************\n");
        }
printf("*******CARDAMOM MODEL LIBRARY DONE****************\n");
printf("Model ID = %i loaded & configured...", ID);
printf("****************************************************\n");

return status;}


