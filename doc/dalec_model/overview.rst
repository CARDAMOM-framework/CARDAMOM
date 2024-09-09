Overview of DALEC model
=======================

.. toctree::
   :maxdepth: 2

Overview of DALEC model




The DALEC model (Williams et al., 2005) is an intermediate complexity terrestrial land surface and biosphere model designed primarily for observation integration within model-data integration frameworks; DALEC models have been augmented since Williams et al., (2005) to include a number of processes, such as water storage (Spadavecchia et al., 2011; Bloom et al., 2020), aerobic and anaerobic respiration processes (Ma et al., 2023), climate-sensitive phenology (Norton et al., 2023), amongst other innovations. The DALEC ID=1100 model cosnists of 14 prognostic states, with the following abbreviated differential equations

***Live C states***
dNSC/dt = GPP - Rauto - Alloc_total
dFol/dt = Alloc_fol - Mortality_fol - Disturbance_fol
dRoo/dt = Alloc_roo - Mortality_roo - Disturbance_roo
dWoo/dt = Alloc_woo - Mortality_woo - Disturbance_woo
***Dead organic C states***
dLit/dt = Mortality_fol + Mortality_roo - Decomp_lit - Rhet_lit - Disturbance_lit
dCWD/dt = Mortality_woo - Decomp_cwd - Rhet_cwd- Disturbance_cwd
dSoil/dt = Decomp_cwd + Decomp_lit - Rhet_som - Disturbance_Som
***Water states***
dSnow/dt = Snowfall - sublimation - melt
dSoilLY1/dt = Infiltration - transpiration_LY1 - evaporation - LY1_LY2_exchange - runoff_LY1
dSoilLY2/dt = transpiration_LY2 + LY1_LY2_exchange - LY2_LY3_exchange - runoff_LY2
dSoilLY3/dt = LY2_LY3_exchange - runoff_LY3
***Energy states***
[TO DO]

All right-hand-side terms, and all associated subcomponents and process formulations, are reported in the individual tech note entries.





