

## CARDAMOM cost function <a name="cardamom-cost-function"/>
 
Observation timeseries summary:

“Values”: 
time-resolved observation values for any time-varying CARDAMOM observations; 
nectdf dataset with one dimension = number of model timesteps. 
Attributes described below
“-9999” or “Nan” values accepted as fill values for missing observations. 
Any other values are assumed to be “valid observations” 

“Unc”:
 time-resolved observation value uncertainty; 
netcdf dataset (with one dimension = number of model timesteps). 
No attributes. 
If no “unc” values are provided, then “single_unc” value used to populate “unc”.
If “unc” values are missing for any valid observations, “single_unc” value is used.


“Opt_normalization” 
Option 0: does nothing, same as -9999 (unless default values are hard-coded, see set of default values).
Option 1: removes mean from both data and model values prior to model-data residual calculation; so far, only used for assimilation of GRACE data, but can be used for anything (e.g. NBE values, if mean NBE is inaccurate, for example). Units of “option 1” transformation are “unitless”, i.e. relative anomalies from the mean values are inter-comparable. 

Option 2: divides both data and model by their respective mean values. Used for “linear assimilation” of SIF data. Units of “option 2” transformation are in native observation units”, i.e. relative anomalies to zero are inter-comparable, or, in other words the units of the model variable and observed data are irrelevant. 


Attributes for “values”.


 *Opt filter*
 Description: provides different options for pre-aggregation of data prior to model-data difference calculation. For each option, only subset of fields are required, others will be ignored
 
** Option 0**
 - Description: "no filter" no operation on data & model prior to least squares model-data residual calculation.
 - Requires either (a) time-resolved uncertainty ("unc" field), at the temporal resolution or (b) a single uncertainty value (“single_unc” field); see “unc” description for details.


**Option 1**
Description: “mean only” filter calculates mean model and data values at all valid observation timesteps; the mean model and data values are then used in the cost function
Requires “single_unc”.

** Option 2**
Description: constrains annual means and monthly anomalies (Quetin et al., 2020, Bloom et al., 2020)
- Requires “single_monthly_unc” and “single_annual_unc” values
- “single _unc” and/or “unc” are ignored.
- Only use with "opt_unc_type = 0"
 
 
** opt_unc_type
 - Determines uncertainty distribution characteristics, where
 - Opt = 0 assumes gaussian uncertainty distribution in units of variable
 - Opt = 1 assumes log-gaussian uncertainty distribution (a.k.a. "uncertainty factor")
 - Opt = 2 assumes gaussian uncertainty distribution as fraction of variable value.
   Currently can only be used with "Opt filter" = 0
 
 
 //(0 = none, 1 = remove mean, 2 = divide by mean)
 
 
 <img width="782" alt="image" src="https://user-images.githubusercontent.com/23563444/160677023-99e0b654-e69d-49ae-96d2-caaa42d95ccc.png">
 
 
 <img width="810" alt="image" src="https://user-images.githubusercontent.com/23563444/170361246-82ad29e5-fb9f-4efb-85a5-959576a64d27.png">

 
## Editing LIKELIHOOD field in the output file (cbr)

If you find the existing LIKELIHOOD field is missing a variable (eg. PEQ_iniSOM) that you used to constrain model (prerequisites are 1)there of course need to be a field named PEQ_iniSOM in your input nc file, 2)the cost function has a PEQ_iniSOM operator). Consult the CARDAMOM developer team. You can add it by editing three places in the DALEC_ALL_LIKELIHOOD file:

1) add "int PEQ_iniSOM;" after line 21
2) add a number coundting after the last number in line ~40: LIKELIHOOD_INDICES={
     0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    10,11,12,13,14,15,16,17,18(this is the number you added)};
3) change LI->NOLIELIHOODS=18 to LI->NOLIELIHOODS=19 (add a number accordingly)
