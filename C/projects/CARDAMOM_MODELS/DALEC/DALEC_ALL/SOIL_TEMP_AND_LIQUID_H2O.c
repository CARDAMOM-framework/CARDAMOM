typedef struct {    
    struct inputs{
    dry_soil_heat_capacity;//J/m3/K
    depth;//m 
    soil_water;//mm (or kg/m2)
    internal_energy;//Joules
    }
    struct outputs{
           soil_temp;//deg C
   liquid_h2o_frac;//0-1
    }
 }SOIL_TEMP_AND_LIQUID_H2O_STRUCT;

    

//Main function 
int SOIL_TEMP_AND_LIQUID_H2O(SOIL_TEMP_AND_LIQUID_H2O_STRUCT * S);


//Triple point temp:
T3 = 273.16;
// Specific heat of ice 2093 J kg?1 K?1
double ci=  2093;
//Specific heat of water 4186 J kg?1 K
double cw=  4186;

//Internal energy at triple pointUi3
//Volumetric heat capacity of soils per m3 [J/m3/K]
S->dry_soil_heat_capacity; 


//Ui3 = (Cs z + Ws Ci) T3
// 
// lil3 Specific latent heat of fusion of water at the triple point 3.34 · 105 J kg?1
// llv3 Specific latent heat of vaporization of water at the triple point 2.5 · 10?6 J kg?1
// liv3 Specific latent heat of sublimation of ice at the triple point li`3 + l`v3 = 2.834 · 10?6 J kg?1

double lhfusion3=3.34e5;

double Ui3 = S->dry_soil_heat_capacity * S->depth + S->soil_water *ci ; 
double Ul3 = Ui3 + S->soil_water*lhfusion3; 

        
S->soil_temp = 0;  



if (S->internal_energy<Ui3){
    
    
    DGCM_T3
    
}
        
        
        
else if (S->internal_energy<Ui3){
    
}
        


//Step 0. Constants



//Step 1. calculate soil mass 
double M = density*depth;


    
//Calculate outputs based on inputs
S.out.soil_temp = 
  
    
    

}
    
    
    
    