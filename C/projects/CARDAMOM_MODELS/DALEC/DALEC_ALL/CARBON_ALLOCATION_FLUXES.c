

typedef struct {    
    struct {
    double   TEMP;//deg C
    double   SRAD;//MJ m2 d
    double   NSC;//Clab
    double   PAW_SM;//m3/m3
    double   parameter1;//replace with any name, no constraints on naming convention
    double   parameter2;//replace with any name, no constraints on naming convention
        //Add any extra inputs here
    } IN;
    struct {
    double *    AUTO_RESP_MAINTENANCE;
      double *       AUTO_RESP_GROWTH;
      double *       ALLOC_FOL;
      double *       ALLOC_WOO;
     double *        ALLOC_ROO;}OUT;
        //Add any extra outputs here
  }CARBON_ALLOCATION_FLUXES_STRUCT;

    

//Main function 
int CARBON_ALLOCATION_FLUXES(CARBON_ALLOCATION_FLUXES_STRUCT * S){
//Add functionality here
    //Use any field in S->IN for calculations
    //example
   double A = S->IN. parameter1 + S->IN. parameter2;
   double B = S->IN. parameter1*S->IN.TEMP;
   double C = A-B;
   //...
//...
   //..
   //Once done with calculations, populate output variables
    
*S->OUT.AUTO_RESP_MAINTENANCE=0;
*S->OUT.AUTO_RESP_MAINTENANCE=0;
*S->OUT.ALLOC_FOL=0;
*S->OUT.ALLOC_WOO=0;
*S->OUT.ALLOC_ROO=0;

    
    
    

return 0;
}
    
    
    
    