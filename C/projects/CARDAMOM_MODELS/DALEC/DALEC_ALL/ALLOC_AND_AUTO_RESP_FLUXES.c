

typedef struct {    
    struct {
        double deltat;//model timestep
        double TEMP;  //deg C
        double C_LIVE;// Live C
        double NSC;   //labile carbon pool, Clab, non-structural carbohydrates (gC)
        double GPP;
        double mr;    // parameter: maintenance respiration coefficient (gC/gC/d)
        double gr;    // parameter: growth respiration coefficient (gC/gC)
        double Q10mr; // parameter: Q10 parameter for maintenance respiration (unitless)
        double ALLOC_FOL_POT; // potential allocation flux to foliar pool (gC/m2/d)
        double ALLOC_WOO_POT; // potential allocation flux to wood pool (gC/m2/d)
        double ALLOC_ROO_POT; // potential allocation flux to root pool (gC/m2/d)
        //Add any extra inputs here
    } IN;
    struct {
        double F_LABPROD; //Labile C production
        double F_LABREL_ACTUAL; //Labile C production
        double AUTO_RESP_MAINTENANCE;// autotrophic maintenance respiration flux (gC/m2/d)
        double AUTO_RESP_GROWTH;// autotrophic growth respiration flux (gC/m2/d)
        double ALLOC_FOL_ACTUAL; // actual allocation flux to root pool (gC/m2/d)
        double ALLOC_WOO_ACTUAL; // actual allocation flux to root pool (gC/m2/d)
        double ALLOC_ROO_ACTUAL; // actual allocation flux to root pool (gC/m2/d)
        double AUTO_RESP_TOTAL; // autotrophic respiration (gC/m2/d)
        double NPP; // net primary productivity, GPP - Rauto (gC/m2/d)
        double CUE; // plant carbon use efficiency, NPP/GPP (gC/m2/d)
    }OUT;
  }ALLOC_AND_AUTO_RESP_FLUXES_STRUCT;

    

//Main function 
int ALLOC_AND_AUTO_RESP_FLUXES(ALLOC_AND_AUTO_RESP_FLUXES_STRUCT * S){

    double fT; // temperature scaling for maintenance respiration
    double F_LABREL_SUPPLY; //
    double F_LABREL_DEMAND; //
    double TOTAL_GROWTH_POT; //
    double TOTAL_GROWTH_ACTUAL; //
    double SCALE_ALLOC_FLUXES; //

    //Maintenance respiration
    fT = pow(S->IN.Q10mr,(S->IN.TEMP-25)/10); 
    S->OUT.AUTO_RESP_MAINTENANCE = S->IN.mr * fT * S->IN.C_LIVE;
    S->OUT.F_LABPROD = S->IN.GPP - S->OUT.AUTO_RESP_MAINTENANCE;

    //Potential supply of labile carbon for plant growth
    F_LABREL_SUPPLY = fmax(0,  (1 - S->IN.gr) * (S->IN.NSC / S->IN.deltat));
    //Potential demand of labile carbon by plant growth
    TOTAL_GROWTH_POT = S->IN.ALLOC_FOL_POT + S->IN.ALLOC_WOO_POT + S->IN.ALLOC_ROO_POT;
    F_LABREL_DEMAND = fmax(0, TOTAL_GROWTH_POT);
    //Actual release of labile carbon (before growth respiration costs subtracted)
    S->OUT.F_LABREL_ACTUAL = fmin(F_LABREL_SUPPLY, F_LABREL_DEMAND);
    //Growth respiration
    S->OUT.AUTO_RESP_GROWTH = S->IN.gr * S->OUT.F_LABREL_ACTUAL;
    //Actual release of labile carbon i.e. growth flux (after subtracting growth respiration costs)
    TOTAL_GROWTH_ACTUAL = (1 - S->IN.gr) * S->OUT.F_LABREL_ACTUAL;

    //Scaling factor for allocation fluxes, accounts for NSC limitation and growth respiration cost
    // - if actual growth is smaller than potential growth, we down-scale plant allocation fluxes
    SCALE_ALLOC_FLUXES = fmin(1, TOTAL_GROWTH_ACTUAL / TOTAL_GROWTH_POT);
    S->OUT.ALLOC_FOL_ACTUAL = SCALE_ALLOC_FLUXES * S->IN.ALLOC_FOL_POT;
    S->OUT.ALLOC_WOO_ACTUAL = SCALE_ALLOC_FLUXES * S->IN.ALLOC_WOO_POT;
    S->OUT.ALLOC_ROO_ACTUAL = SCALE_ALLOC_FLUXES * S->IN.ALLOC_ROO_POT;

    //Diagnostic variables
    S->OUT.AUTO_RESP_TOTAL = S->OUT.AUTO_RESP_MAINTENANCE + S->OUT.AUTO_RESP_GROWTH;
    S->OUT.NPP = S->IN.GPP - S->OUT.AUTO_RESP_TOTAL;
    S->OUT.CUE = S->OUT.NPP/S->IN.GPP;


return 0;
}
    
    
    
    