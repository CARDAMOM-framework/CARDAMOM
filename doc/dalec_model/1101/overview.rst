Overview of DALEC 1101
======================

.. toctree::
   :maxdepth: 2

**Overview of DALEC model**

(Including new allocation)

The DALEC model (Williams et al., 2005) is an intermediate complexity terrestrial land surface and biosphere model designed primarily for observation integration within model-data integration frameworks; DALEC models have been augmented since Williams et al., (2005) to include a number of processes, such as water storage (Spadavecchia et al., 2011; Bloom et al., 2020), aerobic and anaerobic respiration processes (Ma et al., 2023), climate-sensitive phenology (Norton et al., 2023), amongst other innovations. 

**Overview of DALEC 1101 model**


The DALEC ID=1101 model cosnists of 14 prognostic states, with the following abbreviated differential equations

***Live C states***

:math:`\frac{dC_{lab}}{dt} = GPP - Ra_{dark} - Ra_{maint} - Ra_{gr} - PRD_{fol} - PRD_{roo} - PRD_{woo} - DEC_{lab} - DIS_{lab} - FIR_{lab} - FIRx_{lab}`

:math:`\frac{dC_{fol}}{dt} = PRD_{fol} - DEC_{fol} - DECph_{fol} - FIR_{fol} - FIRx_{fol} - DIS_{fol}`

:math:`\frac{dC_{roo}}{dt} = Alloc_roo - Mortality_roo - Disturbance_roo`

:math:`\frac{dC_{woo}}{dt} = Alloc_woo - Mortality_woo - Disturbance_woo`

***Dead organic C states***

:math:`\frac{dC_{lit}}{dt} = Mortality_fol + Mortality_roo - Decomp_lit - Rhet_lit - Disturbance_lit`

:math:`\frac{dC_{cwd}}{dt} = Mortality_woo - Decomp_cwd - Rhet_cwd- Disturbance_cwd`

:math:`\frac{dC_{som}}{dt} = Decomp_cwd + Decomp_lit - Rhet_som - Disturbance_Som`

***Water states***

:math:`\frac{dW_{SWE}}{dt} = Snowfall - sublimation - melt`

:math:`\frac{dW_{ly1}}{dt} = Infiltration - transpiration_LY1 - evaporation - LY1_LY2_exchange - runoff_LY1`

:math:`\frac{dW_{ly2}}{dt} = transpiration_LY2 + LY1_LY2_exchange - LY2_LY3_exchange - runoff_LY2`

:math:`\frac{dW_{ly3}}{dt} = LY2_LY3_exchange - runoff_LY3`

***Energy states***
[TO DO]

All right-hand-side terms, and all associated subcomponents and process formulations, are reported in the individual tech note entries.

.. **Forcing data**

.. The DALEC ID=1101 model forcing data consists of 
.. - Meteorological forcing
..       - Total downward radiation (W/m2)
..       - Total precipitation (mm/day)
..       - Snowfall (mm/day)
..       - Vapor Pressure Deficit (VPD) (hPa)
..       - 2m air temperature (deg C)
..       - Land-surface (skin) temperature (deg C)
.. - Disturbance forcing
..       - Burned area (m2/m2)
..       - C removals from non-fire deforestation (gC/m2/day)
..       - C removals from non-fire degradation (gC/m2/day)
.. - Atmospheric CO2 concentration (ppm CO2).













