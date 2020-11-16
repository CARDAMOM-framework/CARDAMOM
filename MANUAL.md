This is the collaborative manual

Need to copy in google docs manual

# Submission command line functions
#### adapted from Jupyter Notebook tutorial: cardamom_tutorial2

### Command line settings
### From command line:
/.../CARDAMOM/C/projects/CARDAMOM_MDF/CARDAMOM_MDF.exe /.../cbf/filename.cbf /.../cbr/filename.cbr 100000 0 200 0.001 119 1000

number_iterations = "100000" # Small numbers for test, 100000000 standard run, will take a long time.

printrate = "0" #

samplerate = "200" # Number of output parameters = number_iterations/samplerate, generally use between 500 and 1000 output parameters

minimum_step_size = ".001" # Relates to MCMC, standard is currently .001

mcmcid = "119" # Current default MCMC method is 119

nadapt = "1000" # MCMC setting, standard is 1000



# List of Drivers

## Inputs (Model ID: 811, 821)
|CARDAMOM Inputs|Units|Longname|
|:-------------|:-------------|:-------------|
|'Time | [Days since Jan 01, 2001]'|
|'min temp | [C]'|
|'max temp | [C]'|
|'Solar irradiance | [MJ/m2/day]'|
|'CO2 | [ppm]'|
|'Day of year'|day|
|'Burned area | [m2/m2]'|
|'VPD | [hPa]'|
|'Precip. | [mm/day]'|


## List of Ouputs (Model ID: 811, 821)

### Pools (gC/m2, kgH2O/m2, instantaneous)
|CARDAMOM Pools|Longname|
|:--------|:-------------|
|'c_labile'|
|'c_foliar'|
|'c_root'|
|'c_wood'|
|'c_finelitter'|
|'c_som'|
|'h2o_forplant'|

### Fluxes (gC/m2/day, kgH2O/m2/day, mean)
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

# Summary of Models

|Model ID|Model Name|Description|
|:--------|:-------------|:---------|
|DALEC_1000|||
|DALEC_1003|||
|DALEC_1004|||
|DALEC_1004_obs|||
|DALEC_101|||
|DALEC_1010|||
|DALEC_1200|||
|DALEC_400|||
|DALEC_410|||
|DALEC_803|||
|DALEC_804|||
|DALEC_805|||
|DALEC_806|||
|DALEC_807|||
|DALEC_808|||
|DALEC_809|||
|DALEC_810|||
|DALEC_811|||
|DALEC_812|||
|DALEC_813|||
|DALEC_820|||
|DALEC_821|||
|DALEC_830|||
|DALEC_831|||
|DALEC_840|||
