#pragma once

typedef struct {    
    struct {
        double deltat;//model timestep
        double TEMP;  //K
        double C_LIVE_W;// Live C in wood only. Note that this includes heartwood C, and overall maintenance respiration reflects bulk cost of live + dead wood.
        double C_LIVE_R;// Live C in other pools (other live pools, exluding NSCs themselves). 
        double NSC;   //labile carbon pool, Clab, non-structural carbohydrates (gC)
        double GPP;
        double Rd;    // Canopy level dark/day respiration
        double mr_r;    // parameter: maintenance respiration coefficient for root (gC/gC/d) - see Cannell and Thornley (2000, doi: 10.1006/anbo.1999.0996)
        double mr_w;    // parameter: maintenance respiration coefficient for wood (gC/gC/d) - will be smaller than mr_r from EDC. 
        double gr;    // parameter: growth yield parameter (gC/gC) - see "growth yield" (Y_g) in Cannell and Thornley (2000, doi: 10.1006/anbo.1999.0996)
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
        double NONLEAF_MORTALITY_FACTOR; //Proportional removal from all three pools.
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
    fT = pow(S->IN.Q10mr,(S->IN.TEMP-(25+DGCM_TK0C))/10); // reference temperature is 25 degrees C
    //Autotrophic maintenance for wood and non-wood
    double POTENTIAL_AUTO_RESP_MAINTENANCE =   S->IN.mr_w * fT * S->IN.C_LIVE_W + S->IN.mr_r * fT * S->IN.C_LIVE_R;

// Can be negative -- accounting for net Co2 loss in Ag - Rd -- but can't exceed NSC availability
    S->OUT.F_LABPROD = S->IN.GPP - S->IN.Rd;
    //- S->OUT.AUTO_RESP_MAINTENANCE;
    //Calculate actual maintenance respiration, based on available NSCs

    //Full available resources 
    // Most land models
    //We're using GPP for whole timesteo with rest of NSC
    //Basically GPP + NSC available on daily basis...
    //Removes Rd, which is accounted for separately
    double NSC_PLUS_GPP_RATE = S->IN.NSC/S->IN.deltat  + (S->IN.GPP  - S->IN.Rd);

S->OUT.NONLEAF_MORTALITY_FACTOR=1/exp(NSC_PLUS_GPP_RATE/POTENTIAL_AUTO_RESP_MAINTENANCE); //new

//     //IF maintenance 
//                //Spend all NSC on maintenance
//    if (POTENTIAL_AUTO_RESP_MAINTENANCE>NSC_PLUS_GPP_RATE)
//                     {S->OUT.AUTO_RESP_MAINTENANCE =  NSC_PLUS_GPP_RATE + S->IN.Rd;
//                     //Basically a carbon starvation factor
//                      //Insufficient NSCs to maintain tissues.
//                      //Alternative: 
//                      //if NSC_PLUS_GPP_RATE = 0; MORTALITY = 1; all biomass is lost.
//                      //if NSC_PLUS_GPP_RATE = POTENTIAL_AUTO_RESP_MAINTENANCE; MORTALITY = 0; all biomass is lost.

//                      //MORTALITY_FACTOR  = 1 - NSC_PLUS_GPP_RATE/POTENTIAL_AUTO_RESP_MAINTENANCE;
//                      //MORTALITY_FACTOR  = 1 - NSC_PLUS_GPP_RATE/POTENTIAL_AUTO_RESP_MAINTENANCE;
//                      //Current model: if % maintenance resp not available, then lose same % of biomass.
//                      //Alernative model: remobilize foliar and fine root sugars (if at all possible, check literature)
//                      S->OUT.NONLEAF_MORTALITY_FACTOR  = fmin(1 - NSC_PLUS_GPP_RATE/POTENTIAL_AUTO_RESP_MAINTENANCE,1);}

//     else

//Spend full amount on maintenance resp
    //{S->OUT.AUTO_RESP_MAINTENANCE = POTENTIAL_AUTO_RESP_MAINTENANCE + S->IN.Rd;} //new
    S->OUT.AUTO_RESP_MAINTENANCE = POTENTIAL_AUTO_RESP_MAINTENANCE * ( 1 - S->OUT.NONLEAF_MORTALITY_FACTOR) ; //new

     //Calculate leftover NSCs
    //Only use available NSC after maintenance resp accounted for
    
    



    S->OUT.F_LABREL_ACTUAL =0;
        S->OUT.AUTO_RESP_GROWTH = 0;
        S->OUT.ALLOC_FOL_ACTUAL =0;
       S->OUT.ALLOC_WOO_ACTUAL =0;
       S->OUT.ALLOC_ROO_ACTUAL =0;

    // if (S->OUT.NONLEAF_MORTALITY_FACTOR==0){
    

        double LEFTOVER_NSC_RATE = NSC_PLUS_GPP_RATE - S->OUT.AUTO_RESP_MAINTENANCE; //

   
    //Only proceed if AUTO_RESP_MAINTENANCE>=NSC
    //
    //NEW_PSEUDOCODE

    //Potential supply of labile carbon for plant growth
    F_LABREL_SUPPLY = fmax(0,  S->IN.gr * (LEFTOVER_NSC_RATE));
    
    //NEW PSEUDOCODE: F_LABREL_SUPPLY = fmax(0,  (1 - S->IN.gr) * (LEFTOVER_NSC/ S->IN.deltat));

    //Potential demand of labile carbon by plant growth
    TOTAL_GROWTH_POT = S->IN.ALLOC_FOL_POT + S->IN.ALLOC_WOO_POT + S->IN.ALLOC_ROO_POT;
    F_LABREL_DEMAND = fmax(0, TOTAL_GROWTH_POT);

    // Compute exponential growth factor representing amount of growth resources mobilizable 

    double GF = 1/exp(F_LABREL_SUPPLY/F_LABREL_DEMAND);
    //Actual release of labile carbon (before growth respiration costs subtracted)
    //S->OUT.F_LABREL_ACTUAL = fmin(F_LABREL_SUPPLY, F_LABREL_DEMAND);
    S->OUT.F_LABREL_ACTUAL = F_LABREL_DEMAND * (1 -GF); 


    //Scaling factor for allocation fluxes, accounts for NSC limitation and growth respiration cost
    // - if actual growth is smaller than potential growth, we down-scale plant allocation fluxes
    //SCALE_ALLOC_FLUXES = fmin(1, S->OUT.F_LABREL_ACTUAL / TOTAL_GROWTH_POT);//rid
    S->OUT.ALLOC_FOL_ACTUAL = S->IN.ALLOC_FOL_POT * (1 - GF);
    S->OUT.ALLOC_WOO_ACTUAL = S->IN.ALLOC_WOO_POT * (1 - GF);
    S->OUT.ALLOC_ROO_ACTUAL = S->IN.ALLOC_ROO_POT * (1 - GF);

    //Actual release of labile carbon i.e. growth flux (after subtracting growth respiration costs)
    //TOTAL_GROWTH_ACTUAL = TOTAL_GROWTH_POT * SCALE_ALLOC_FLUXES;
    TOTAL_GROWTH_ACTUAL = S->OUT.ALLOC_FOL_ACTUAL + S->OUT.ALLOC_WOO_ACTUAL + S->OUT.ALLOC_ROO_ACTUAL;
    //Growth respiration
    S->OUT.AUTO_RESP_GROWTH = (1-S->IN.gr)/S->IN.gr * TOTAL_GROWTH_ACTUAL;

    //Diagnostic variables
    S->OUT.AUTO_RESP_TOTAL = S->OUT.AUTO_RESP_MAINTENANCE + S->OUT.AUTO_RESP_GROWTH;
    S->OUT.NPP = S->IN.GPP - S->OUT.AUTO_RESP_TOTAL;
    S->OUT.CUE = S->OUT.NPP/S->IN.GPP;



return 0;
}
    
    
    
    