# Output Data and Analysis

This manual section will cover what outputs occur with running CARDAMOM and common forms of analysis. We will cover both the outputs from assimilation runs (the parameter set) and the outputs from forward runs after assimilation (fluxes and states of the ecosystem). For reference, some of the outputs common to many models are included here as tables (Table 1: Pools, Table 2: Fluxes, and Table 3: Parameters). For a specific models names parameters, pools and fluxes check the following scripts in the specific model:

'''
Example location
'''

The content of this section will include:

- [Intro to reading output data](#-getting-started)
    * [Output data format for parameters]
    * [Output data format for states and fluxes]
    * [Reading data]
    
- [Intro to analyzing data]
    * [CARDAMOM Ensemble and Summary Statistics]
    * [Basic data plots]
    * [Aggregating CARDAMOM output]

- [Appendix: Output Tables]
    * [Parameters]
    * [States]
    * [Fluxes]
    * [Cost Function]


## CARDAMOM state and flux conventions 

- Initial conditions correspond to pools (states) at first timestep t = 0
- Model Meteorological forcing at timestep t is centered between pools (states) t and t+1
- Model fluxes at timestep t also centered between states t and t +1 


<img width="830" alt="image" src="https://user-images.githubusercontent.com/23563444/117489093-086c5b80-af22-11eb-8006-693c716d142f.png">

*In CARDAMOM C code* 
See conventions above

*For Matlab users*
CARDAMOM_RUN_MODEL.m throws out first one, but only because it’s repeat of initial conditions (which are contained in parameter vector).



### Helper Scripts <a name="-helper-scripts"/>

Helper scripts (running and analysing CARDAMOM) are primarily written in Matlab and Python.

|Filepath|Description|
|:--------|:-------------|
|script1||
|script1||
|script1||



## Aggregation and uncertainty propagatation
Description: general rules and guidelines for estimating and propagating uncertainty

"rule no 1." (some funny movie reference)
"Rule no 2.": perform operation on data before calculating uncertainty (on states and fluxes?)

- example: detrending state and flux data. 
  - Step 1. Detrend every CARDAMOM sample 
  - Step 2. Calculate percentile/statistics based on detrended dataset.

Other examples:
- annually average time series (action = average annually for each samples first, then
- monthly anomalies
- normalizing data (GRACE
- For comparison against observation, perform "observation operator" first, then aggregate.


Exception (sort of) to rule = "statistics on statistics".

For example: what is the range of IAV (e.g. as sigma value) among CARDAMOM samples?
Step 1. Calculate IAV per sample.
Step 2. Summarize mean, median, standard deviation, percentiles of IAV among samples.

Two contrasting examples:
"ensemble mean" = mean of N CARDAMOM samples.

(1) What is the IAV of ensemble mean GPP?
Answer: (1) calculate ensemble mean GPP, and (2) calculate its IAV. Effectively no uncertainty estimate here, possible never need to do this for a single pixel (however: this order may be required for multi pixel uncertainty propagation).

(2) What is the IAV of all GPP ensemble members?
Answer: (1) calculate IAV for each sample, and (2) calculate any useful/relevant statistics on IAV differences among samples (e.g. medians, percentiles).

Note on above examples: mean GPP IAV is **not** equal to mean of GPP IAV values corresponding to individual ensemble members (e.g. if distribution is skewed). iav(mean(GPP)) =/= mean(iav(GPP)).


<img width="1113" alt="image" src="https://user-images.githubusercontent.com/23563444/110368127-74534600-7ffd-11eb-9d81-479a39618fe0.png">


***How to aggregate statistics between pixels***
General issue: no information on correlation between samples between pixels. However, some uncertainty estimation generally required

Dilemma. Two pixels, one with mean GPP = 0.2 gC/m2/d (desert), and another with GPP = 10 gC/m2/d (rainforest). How to you aggregate IAVs (if at all)? 




***end of actual "rules"***



#### List of Ouputs (Model ID: 811, 821)

##### Pools (gC/m2, kgH2O/m2, instantaneous)
|CARDAMOM Pools|Longname|
|:--------|:-------------|
|'c_labile'|
|'c_foliar'|
|'c_root'|
|'c_wood'|
|'c_finelitter'|
|'c_som'|
|'h2o_forplant'|

##### Fluxes (gC/m2/day, kgH2O/m2/day, mean)
|CARDAMOM Fluxes|Longname|
|:--------|:-------------|
|'gppflux'|
|'decf_tempr'|
|'gpp_to_autoresp'|
|'gpp_to_leaf'|
|'gpp_to_labile'|
|'gpp_to_root'|
|'gpp_to_wood'|
|'labile_to_foliar'|
|'leaf_fall'|
|'leaf_to_litter'|
|'wood_to_soilc'|
|'root_to_litter'|
|'hetresp_litter'|
|'hetresp_som'|
|'litter_to_som'|
|'leaf_onset'|
|'fire_em_total'|
|'fire_em_labile'|
|'fire_em_foliar'|
|'fire_em_root'|
|'fire_em_wood'|
|'fire_em_litter'|
|'fire_em_som'|
|'fire1_to_littersom'|
|'fire2_to_littersom'|
|'fire3_to_littersom'|
|'fire4_to_littersom'|
|'fire5_to_littersom'|
|'et'|
|'runoff'|

##### Example List of Parameters Optimized in CARDAMOM

| Shortname | Codename                                                                       | Units | Range |
|-----------|--------------------------------------------------------------------------------|-------|-------|
|           | Decomposition rate                                                             |       |0.00001 - 0.01|
|           | Fraction of GPP respired                                                       |       |0.2 - 0.8|
|           | Fraction of (1-fgpp) to foliage                                                |       |0.01 - 0.5|
|           | Fraction of (1-fgpp) to roots                                                  |       |0.01 - 1.0|
|           | Leaf Lifespan                                                                  |       |1.001 - 8.0|
|           | TOR wood* - 1% loss per year value                                             |       |0.000025 - 0.001|
|           | TOR roots                                                                      |       |0.0001 - 0.01|
|           | TOR litter                                                                     |       |0.0001 - 0.01|
|           | TOR SOM                                                                        |       |0.0000001 - 0.001|
|           | Temp factor* = Q10 = 1.2-1.6                                                   |       |0.018 - 0.08|
|           | Canopy Efficiency                                                              |       |5.0 - 50.0|
|           | Bday                                                                           |day of year |365.25 - 4x365.25|
|           | Fraction to Clab                                                               |       |0.01 - 0.5|
|           | Clab Release period                                                            |days |365.25/12 - 100|
|           | Fday                                                                           |day of year |365.25 - 4x365.25|
|           | Leaf fall period                                                               |days |365.25/12 - 150|
|           | LMCA                                                                           |g C/m2 |5.0 - 200.0|
|           | C labile                                                                       |g C/m2 |1.0 - 2000.0|
|           | C foliar                                                                       |g C/m2 |1.0 - 2000.0|
|           | C roots                                                                        |g C/m2 |1.0 - 2000.0|
|           | C_wood                                                                         |g C/m2 |1.0 - 100000.0|
|           | C litter                                                                       |g C/m2 |1.0 - 2000.0|
|           | C_som                                                                          |g C/m2 |1.0 - 200000.0|
|           | IWUE: GPPxVPD/ET: gC/kgH2o -hPa                                                |hPa gC/kgH2O |10.0 - 50.0|
|           | Runoff focal point (~maximum soil storage capacity x 4)                        |       |1.0 - 100000.0|
|           | "Wilting point"                                                                |kgH2O/m2 |1.0 - 10000.0|
|           | "Bucket at t0"                                                                 |kgH2O/m2 |1.0 - 10000.0|
|           | Foliar biomass CF                                                              |       |0.01 - 1.0|
|           | "Ligneous" biomass CF".                                                        |       |0.01 - 1.0|
|           | DOM CF".                                                                       |       |0.01 - 1|
|           | Resilience factor (since transfer to litter is represented as (1-pars[30])) ". |       |0.01 - 1|
|           | Lab pool lifespan                                                              |       |1.001 - 8.0|
|           | Moisture factor                                                                |       |0.01 - 1.0|




