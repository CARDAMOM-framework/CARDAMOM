
/*Script needs to include*/
/*(1) MOUNT script: defines all data-optimizable parameters*/
/*(2) PROB script: defines all probability calculations (EDCs etc)*/
/*(3) MODULE script: contains all model calculations*/



int MOUNT_INFILTRATION_MODULE(DATA *DATA){

/*Step 1. Index module*/
DATA->MODULE_IDX.HYDROLOGY.infiltration_imax=DATA->nopars;

/*PARNAME: Imax [mm/day]*/
ADD_PARAMETER_TO_STACK(DATA,"Imax",1,1000);/*pars[0]*/

return 0;

}






