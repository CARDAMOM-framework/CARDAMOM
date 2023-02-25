Adding new netcdf variables and attributes


Easy steps for either "TIMESERIES_OBS" or "SINGLE_OBS"

1. Declare variables in 
CARDAMOM/C/projects/CARDAMOM_GENERAL/CARDAMOM_NETCDF_DATA_STRUCTURE.c

2. Define (i.e. read from netcdf) 
CARDAMOM/C/projects/CARDAMOM_GENERAL/CARDAMOM_READ_NETCDF_DATA.c

3. Declare model-corresponding quantity in 
CARDAMOM/C/projects/CARDAMOM_GENERAL/CARDAMOM_DATA_STRUCTURE.c

4. Write observation operator in 
CARDAMOM/C/projects/CARDAMOM_MODELS/DALEC/DALEC_OBSERVATION_OPERATORS/DALEC_OBSERVATION_OPERATORS.c
This could be easy if there's an obvious apples:apples comparsion between model and observed quantities, or could be more complex

5. Go in any/all relevant DALEC model codes (DALEC_XXXX/DALEX_XXXX.c) and set observation operator to "true" (see similar variable observation operator statements)

6. Go to CARDAMOM/C/projects/COST_FUNCTION/MODEL_LIKELIHOOD_FUNCTIONS/DALEC_ALL_LIKELIHOOD.c and do the following:
  6a. add new variable index in "struct LIKELIHOOD_INDICES"
  6b. Add CARDAMOM_*_OBS_LIKELIHOOD statement along with other statements later in the code.
  
  
  
  
Note: if you have unique new attributes,  different steps involved (consult with CARDAMOM tech team people to figure out best way forward).

