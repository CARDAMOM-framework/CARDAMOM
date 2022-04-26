

typedef struct {    
    struct {
    double   TEMP;//deg C
    double  C_LIVE;// Live C
    double GPP;
    double mr;
    double   SRAD;//MJ m2 d
    double   NSC;//Clab
    double   PAW_SM;//m3/m3
    double   parameter1;//replace with any name, no constraints on naming convention
    double   parameter2;//replace with any name, no constraints on naming convention
        //Add any extra inputs here
    } IN;
    struct {
        double Flabprod;//Labile C production
    double     AUTO_RESP_MAINTENANCE;//gC/m2/d
      double       AUTO_RESP_GROWTH;//gC/m2/d
      double       ALLOC_FOL;//gC/m2/d
      double       ALLOC_WOO;//gC/m2/d
     double         ALLOC_ROO;//gC/m2/d
             //Add any extra outputs here
    }OUT;
  }ALLOC_AND_AUTO_RESP_FLUXES_STRUCT;

    

//Main function 
int ALLOC_AND_AUTO_RESP_FLUXES(ALLOC_AND_AUTO_RESP_FLUXES_STRUCT * S){

    
    //Maintenance respiration
S->OUT.AUTO_RESP_MAINTENANCE = S->IN.mr*S->IN.TEMP*S->IN.C_LIVE;//FOr example



//...
   //..
   //Once done with calculations, populate output variables
    
S->OUT.AUTO_RESP_MAINTENANCE=0;
S->OUT.AUTO_RESP_MAINTENANCE=0;
S->OUT.ALLOC_FOL=0;
S->OUT.ALLOC_WOO=0;
S->OUT.ALLOC_ROO=0;

    
    
    

return 0;
}
    
    
    
    