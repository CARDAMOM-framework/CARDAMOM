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
        double Q10mr; // parameter: Q10 parameter for maintenance respiration (unitless)
        double ALLOC_FOL_POT; // potential allocation flux to foliar pool (gC/m2/d)
        


        double k_growth;
        double W_o;
        double s_ar;
        double s_wood;
        double s_root;
        double e_ar;
        double e_wood;
        double e_root;
        double Paw_norm;
        
    } IN;
    struct {

        

        double AUTO_RESP_MAINTENANCE;// autotrophic maintenance respiration flux (gC/m2/d)
        double AUTO_RESP_GROWTH;// autotrophic growth respiration flux (gC/m2/d)
        double ALLOC_FOL_ACTUAL; // actual allocation flux to root pool (gC/m2/d)
        double ALLOC_WOO_ACTUAL; // actual allocation flux to root pool (gC/m2/d)
        double ALLOC_ROO_ACTUAL; // actual allocation flux to root pool (gC/m2/d)

        double growth_carbon; //Labile C production
        double resep_alloc;
        double fol_alloc;
        double growth_yield_nofoliar;
        double wood_alloc;
        double root_alloc;
        double nsc_alloc;
        double ALLOC_NSC_ACTUAL;
        
        double AUTO_RESP_TOTAL; // autotrophic respiration (gC/m2/d)
        double NPP; // net primary productivity, GPP - Rauto (gC/m2/d)
        double CUE; // plant carbon use efficiency, NPP/GPP (gC/m2/d)
        double NONLEAF_MORTALITY_FACTOR; //Proportional removal from all three pools.
    }OUT;
  }NLM_AND_ALLOC_STRUCT;


double sigmoid(double start,double k,double x,double x_o,double end){
    if(end>start){
        return start + (end-start)/(1+exp(k*(x-x_o)));
    }
    else{
        return end + (start-end)/(1+exp(-k*(x-x_o)));
    }
}     

//Main function 
int NLM_AND_ALLOC(NLM_AND_ALLOC_STRUCT * S){

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

    

    double NSC_PLUS_GPP_RATE = S->IN.NSC/S->IN.deltat  + (S->IN.GPP  - S->IN.Rd);

    S->OUT.NONLEAF_MORTALITY_FACTOR=0;

    printf("Rd: %f\n", S->IN.Rd);


    //IF maintenance 
    //Spend all NSC on maintenance
    S->OUT.NONLEAF_MORTALITY_FACTOR=1/exp(NSC_PLUS_GPP_RATE/POTENTIAL_AUTO_RESP_MAINTENANCE);






    S->OUT.AUTO_RESP_MAINTENANCE = POTENTIAL_AUTO_RESP_MAINTENANCE * (1- S->OUT.NONLEAF_MORTALITY_FACTOR);

    //Calculate leftover NSCs
    //Only use available NSC after maintenance resp accounted for
    double LEFTOVER_NSC_RATE = NSC_PLUS_GPP_RATE - S->OUT.AUTO_RESP_MAINTENANCE  ;

    printf("LEFTOVER_NSC_RATE: %f\n", LEFTOVER_NSC_RATE);
    S->OUT.growth_carbon = LEFTOVER_NSC_RATE;

    
    S->OUT.resep_alloc = sigmoid(S->IN.s_ar,S->IN.k_growth,S->IN.Paw_norm,S->IN.W_o,S->IN.e_ar);
    S->OUT.AUTO_RESP_GROWTH = S->OUT.resep_alloc  * LEFTOVER_NSC_RATE;

    
    double growth_yield =  LEFTOVER_NSC_RATE - S->OUT.AUTO_RESP_GROWTH;

   
    S->OUT.ALLOC_FOL_ACTUAL = fmin(growth_yield, fmax(0, S->IN.ALLOC_FOL_POT ) );
    S->OUT.fol_alloc = S->OUT.ALLOC_FOL_ACTUAL/S->OUT.growth_carbon;


    double growth_yield_nofoliar = growth_yield - S->OUT.ALLOC_FOL_ACTUAL;
    

    S->OUT.wood_alloc = sigmoid(S->IN.s_wood,S->IN.k_growth,S->IN.Paw_norm,S->IN.W_o,S->IN.e_wood);
    S->OUT.ALLOC_WOO_ACTUAL = S->OUT.wood_alloc * growth_yield_nofoliar;
    

    S->OUT.root_alloc = sigmoid(S->IN.s_root,S->IN.k_growth,S->IN.Paw_norm,S->IN.W_o,S->IN.e_root);
    S->OUT.ALLOC_ROO_ACTUAL = S->OUT.root_alloc * growth_yield_nofoliar;

    S->OUT.nsc_alloc = 1- S->OUT.wood_alloc  -  S->OUT.root_alloc;
    S->OUT.ALLOC_NSC_ACTUAL = growth_yield_nofoliar - S->OUT.ALLOC_WOO_ACTUAL - S->OUT.ALLOC_ROO_ACTUAL ;

    //  printf("F_LABREL_ACTUAL: %f\n", S->OUT.F_LABREL_ACTUAL);
    // printf("ALLOC_WOO_ACTUAL: %f\n", S->OUT.ALLOC_WOO_ACTUAL);
    // printf("ALLOC_ROO_ACTUAL: %f\n", S->OUT.ALLOC_ROO_ACTUAL);


    printf("\n %f %f %f %f %f %f \n", LEFTOVER_NSC_RATE, S->OUT.AUTO_RESP_GROWTH,S->OUT.ALLOC_FOL_ACTUAL,S->OUT.ALLOC_WOO_ACTUAL,S->OUT.ALLOC_ROO_ACTUAL,S->OUT.ALLOC_NSC_ACTUAL);
    
    //Diagnostic variables
    S->OUT.AUTO_RESP_TOTAL = S->OUT.AUTO_RESP_MAINTENANCE + S->OUT.AUTO_RESP_GROWTH;
    S->OUT.NPP = S->IN.GPP - S->OUT.AUTO_RESP_TOTAL;
    S->OUT.CUE = S->OUT.NPP/S->IN.GPP;






    return 0;
}
    
    
    
    