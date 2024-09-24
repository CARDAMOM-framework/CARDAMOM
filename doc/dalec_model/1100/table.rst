Variables in DALEC 1100
=======================

.. toctree::
   :maxdepth: 2

Variables used in DALEC model

.. list-table:: Input variables
   :widths: 1 1 1 1 1
   :header-rows: 1

   * - Abbreviation
     - Description
     - Units
     - Code string
     - Notes
   * - :math:`SSRD`
     - Downwelling shortwave radiation
     - J m\ :sup:`-2` d\ :sup:`-1`
     - ``SSRD``
     - 
   * - :math:`STRD`
     - Downwelling longwave radiation
     - J m\ :sup:`-2` d\ :sup:`-1`
     - ``STRD``
     - 
   * - :math:`T_{min}`
     - Minimum temperature
     - K
     - ``T2M_MIN``
     - Mean of daily minimum temperature over time step
   * - :math:`T_{max}`
     - Maximum temperature
     - K
     - ``T2M_MAX``
     - Mean of daily maximum temperature over time step
   * - :math:`T_{skin}`
     - Skin temperature
     - K
     - ``SKT``
     - 
   * - :math:`P_{tot}`
     - Total precipitation
     - mmH\ :sub:`2`\ O d\ :sup:`-1`
     - ``PREC``
     - Includes rain and snowfall
   * - :math:`P_{snow}`
     - Snowfall
     - mmH\ :sub:`2`\ O d\ :sup:`-1`
     - ``SNOWFALL``
     - Must be less than or equal to :math:`P_{tot}`
   * - :math:`VPD`
     - Vapor pressure deficit
     - mPa
     - ``VPD``
     - 
   * - :math:`CO2`
     - Atmospheric CO\ :sub:`2` concentration
     - ppm
     - ``CO2``
     - 
   * - :math:`BA`
     - Burned area
     - m\ :sup:`2` m\ :sup:`-2`
     - ``BURNED_AREA``
     - 
   * - :math:`DIST`
     - Non-fire distirbance
     - gC m\ :sup:`-2` d\ :sup:`-1`
     - ``DIST``
     - 
   * - :math:`YIELD`
     - Yield
     - ???
     - ``YIELD``
     - 

.. list-table:: Prognostic variables (pools)
   :widths: 1 1 1 1 1 1 1
   :header-rows: 1

   * - Abbreviation
     - Description
     - Units
     - Fluxes/forcing in
     - Fluxes out
     - Code string
     - Notes
   * - :math:`C_{lab}`
     - Labile carbon (non-structural carbohydrate)
     - gC m\ :sup:`-2`
     - :math:`GPP`
     - :math:`Ra_{dark}`, :math:`Ra_{maint}`, :math:`Ra_{gr}`, :math:`PRD_{fol}`, :math:`PRD_{roo}`, :math:`PRD_{woo}`, :math:`DEC_{lab}`, :math:`DIS_{lab}`, :math:`FIR_{lab}`, :math:`FIRx_{lab}`
     - ``C_lab``
     - 
   * - :math:`C_{fol}`
     - Foliar carbon
     - gC m\ :sup:`-2`
     - :math:`PRD_{fol}`
     - :math:`DEC_{fol}`, :math:`DECph_{fol}`, :math:`FIR_{fol}`, :math:`FIRx_{fol}`, :math:`DIS_{fol}`
     - ``C_fol``
     - 
   * - :math:`C_{woo}`
     - Wood/ligneous carbon
     - gC m\ :sup:`-2`
     - :math:`PRD_{woo}`
     - :math:`DEC_{woo}`, :math:`FIR_{woo}`, :math:`FIRx_{woo}`, :math:`DIS_{woo}`
     - ``C_woo``
     - Includes woody root biomass
   * - :math:`C_{roo}`
     - Fine root carbon
     - gC m\ :sup:`-2`
     - :math:`PRD_{roo}`
     - :math:`DEC_{roo}`, :math:`FIR_{roo}`, :math:`FIRx_{roo}`, :math:`DIS_{roo}`
     - ``C_roo``
     - Does not include woody root biomass
   * - :math:`C_{lit}`
     - Litter carbon
     - gC m\ :sup:`-2`
     - **add yield** :math:`DEC_{lab}`, :math:`DEC_{fol}`, :math:`DECph_{fol}`, :math:`DEC_{roo}`, :math:`FIRx_{lab}`, :math:`FIRx_{fol}`, :math:`FIRx_{roo}`
     - **add yield** :math:`RhAe_{lit}`, :math:`RhAn_{lit}`, :math:`DEC_{lit}`, :math:`FIR_{lit}`, :math:`FIRx_{lit}`
     - ``C_lit``
     - 
   * - :math:`C_{cwd}`
     - Coarse woody debris carbon
     - gC m\ :sup:`-2`
     - **add yield** :math:`DEC_{woo}`, :math:`FIR_{woo}`
     - :math:`RhAe_{cwd}`, :math:`RhAn_{cwd}`, :math:`DEC_{cwd}`, :math:`FIR_{cwd}`, :math:`FIRx_{cwd}`:math:`RhAe_{cwd}`, :math:`RhAn_{cwd}`, :math:`DEC_{cwd}`, :math:`FIR_{cwd}`, :math:`FIRx_{cwd}`
     - ``C_cwd``
     - 
   * - :math:`C_{som}`
     - Soil organic matter carbon
     - gC m\ :sup:`-2`
     - :math:`DEC_{lit}`, :math:`FIRx_{lit}`, :math:`DEC_{cwd}`, :math:`FIRx_{cwd}`,
     - :math:`RhAe_{som}`, :math:`RhAn_{som}`, :math:`FIR_{som}`
     - ``C_som``
     - 
   * - :math:`W_{ly1}`
     - Layer 1 water
     - mmH\ :sub:`2`\ O
     - :math:`Qx_{inf}`
     - :math:`Ev`, :math:`Tr_{ly1}`, :math:`Q_{ly1}`, :math:`Qx_{ly1,2}`
     - ``H2O_LY1``
     - 
   * - :math:`W_{ly2}`
     - Layer 2 water
     - mmH\ :sub:`2`\ O
     - :math:`Qx_{ly1,2}`
     - :math:`Tr_{ly2}`, :math:`Q_{ly2}`, :math:`Qx_{ly2,3}`
     - ``H2O_LY2``
     - 
   * - :math:`W_{ly3}`
     - Layer 3 water
     - mmH\ :sub:`2`\ O
     - :math:`Qx_{ly2,3}`
     - :math:`Q_{ly3}`
     - ``H2O_LY3``
     - 
   * - :math:`W_{swe}`
     - Snow water equivalent
     - mmH\ :sub:`2`\ O
     - :math:`P_{snow}`
     - :math:`Melt`, :math:`Subl`
     - ``H2O_SWE``
     - 
   * - :math:`E_{ly1}`
     - Energy content of :math:`W_{ly1}`
     - J m\ :sup:`-2`
     - :math:`E_{gh}`, :math:`EQx_{inf}`
     - :math:`EEv`, :math:`ETr_{ly1}`, :math:`EQ_{ly1}`, :math:`EQx_{ly1,2}`, :math:`EQthx_{ly1,2}`
     - ``E_LY1``
     - 
   * - :math:`E_{ly2}`
     - Energy content of :math:`W_{ly2}`
     - J m\ :sup:`-2`
     - :math:`EQx_{ly1,2}`, :math:`EQthx_{ly1,2}`
     - :math:`ETr_{ly2}`, :math:`EQ_{ly2}`, :math:`EQx_{ly2,3}`, :math:`EQthx_{ly2,3}`
     - ``E_LY2``
     - 
   * - :math:`E_{ly3}`
     - Energy content of :math:`W_{ly3}`
     - J m\ :sup:`-2`
     - :math:`E_{geo}`, :math:`EQx_{ly2,3}`, :math:`EQthx_{ly2,3}`
     - :math:`EQ_{ly3}`
     - ``E_LY3``
     - 




.. list-table:: Diagnostic variables
   :widths: 1 1 1 1 1
   :header-rows: 1

   * - :math:Abbreviation
     - Description
     - Units
     - Code string
     - Notes
   * - :math:`LAI`
     - Leaf area index
     - m\ :sup:`2` m\ :sup:`-2`
     - ``D_LAI``
     - 
   * - :math:`SCF`
     - Snow covered fraction
     - 1
     - ``D_SCF``
     - Fraction of non-vegetation-covered land surface (gap fraction) that is covered by a layer of snow. 
   * - :math:`T_{ly1}`
     - :math:`W_{ly1}` temperature
     - K
     - ``D_TEMP_LY1``
     - 
   * - :math:`T_{ly2}`
     - :math:`W_{ly2}` temperature
     - K
     - ``D_TEMP_LY2``
     - 
   * - :math:`T_{ly3}`
     - :math:`W_{ly3}` temperature
     - K
     - ``D_TEMP_LY3``
     - 
   * - :math:`LF_{ly1}`
     - :math:`W_{ly1}` liquid fraction
     - 1
     - ``D_LF_LY1``
     - Ratio of liquid water to total water in layer 1
   * - :math:`LF_{ly2}`
     - :math:`W_{ly2}` liquid fraction
     - 1
     - ``D_LF_LY2``
     - Ratio of liquid water to total water in layer 2
   * - :math:`LF_{ly3}`
     - :math:`W_{ly3}` liquid fraction
     - 1
     - ``D_LF_LY3``
     - Ratio of liquid water to total water in layer 3
   * - :math:`\theta_{ly1}`
     - :math:`W_{ly1}` soil moisture
     - 1
     - ``D_SM_LY1``
     - Ratio of total water to pore space in layer 1
   * - :math:`\theta_{ly2}`
     - :math:`W_{ly2}` soil moisture
     - 1
     - ``D_SM_LY2``
     - Ratio of total water to pore space in layer 2
   * - :math:`\theta_{ly3}`
     - :math:`W_{ly3}` soil moisture
     - 1
     - ``D_SM_LY3``
     - Ratio of total water to pore space in layer 3
   * - :math:`\Psi_{ly1}`
     - :math:`W_{ly1}` water potential
     - MPa
     - ``D_PSI_LY1``
     - 
   * - :math:`\Psi_{ly2}`
     - :math:`W_{ly2}` water potential
     - MPa
     - ``D_PSI_LY2``
     - 
   * - :math:`\Psi_{ly3}`
     - :math:`W_{ly3}` water potential
     - MPa
     - ``D_PSI_LY3``
     - 


.. POOLS_INFO.NAME[S.M_LAI_MAX]='Past Maximum Leaf Area Index in vegetation memory';
.. POOLS_INFO.ABBREVIATION[S.M_LAI_MAX]='M_LAI_MAX';
.. POOLS_INFO.UNITS[S.M_LAI_MAX]='m2/m2';
.. POOLS_INFO.DESCRIPTION[S.M_LAI_MAX]='Exponentially declining term encoding vegetation \'memory\' of past water/structural limitations for LAI, to capture lagged effects of past stress.';

.. POOLS_INFO.NAME[S.M_LAI_TEMP]='Past Leaf temperature in vegetation memory';
.. POOLS_INFO.ABBREVIATION[S.M_LAI_TEMP]='M_LAI_TEMP';
.. POOLS_INFO.UNITS[S.M_LAI_TEMP]='K';
.. POOLS_INFO.DESCRIPTION[S.M_LAI_TEMP]='Exponentially declining term encoding vegetation \'memory\' of past temperature limitations for LAI, to capture lagged effects of past stress.';




.. list-table:: Fluxes
   :widths: 1 1 1 1 1
   :header-rows: 1

   * - Abbreviation
     - Description
     - Units
     - Code string
     - Notes
   * - :math:`GPP`
     - Gross primary production
     - gC m\ :sup:`-2` d\ :sup:`-1`
     - ``gpp``
     - Gross C assimilation, without leaf maintenance respiration removed
   * - :math:`GPP_{net}`
     - Net gross primary production
     - gC m\ :sup:`-2` d\ :sup:`-1`
     - ``gppnet``
     - Net :math:`GPP` after removing leaf maintenance respiration, same as labile production
   * - :math:`PRD_{lab}`
     - Labile production
     - gC m\ :sup:`-2` d\ :sup:`-1`
     - ``lab_prod``
     - Same as :math:`GPP_{net}` (flagged for removal)
   * - :math:`PRD_{fol}`
     - Foliar production
     - gC m\ :sup:`-2` d\ :sup:`-1`
     - ``foliar_prod``
     - 
   * - :math:`PRD_{roo}`
     - Fine root production
     - gC m\ :sup:`-2` d\ :sup:`-1`
     - ``root_prod``
     - 
   * - :math:`PRD_{woo}`
     - Wood and coarse root production
     - gC m\ :sup:`-2` d\ :sup:`-1`
     - ``wood_prod``
     - 
   * - :math:`Ra_{gr}`
     - Autotrophic growth respiration
     - gC m\ :sup:`-2` d\ :sup:`-1`
     - ``resp_auto_growth``
     - 
   * - :math:`Ra_{dark}`
     - Autotrophic maintenance dark respiration
     - gC m\ :sup:`-2` d\ :sup:`-1`
     - ``resp_auto_maint_dark``
     - Mitochondrial respiration from :math:`C_{fol}`
   * - :math:`Ra_{maint}`
     - Autotrophic maintenance respiration
     - gC m\ :sup:`-2` d\ :sup:`-1`
     - ``resp_auto_maint``
     - Includes :math:`Ra_{dark}`
   * - :math:`RhAe_{cwd}`
     - Aerobic heterotrophic respiration from :math:`C_{cwd}` 
     - gC m\ :sup:`-2` d\ :sup:`-1`
     - ``ae_rh_cwd``
     - 
   * - :math:`DEC_{fol}`
     - Foliar decomposition
     - gC m\ :sup:`-2` d\ :sup:`-1`
     - ``fol2lit``
     - :math:`C_{fol}` transferred to :math:`C_{lit}` due to mortality
   * - :math:`DECph_{fol}`
     - Phenological foliar decomposition
     - gC m\ :sup:`-2` d\ :sup:`-1`
     - ``ph_fol2lit``
     - :math:`C_{fol}` transferred to :math:`C_{lit}` due to phenological senescence
   * - :math:`DEC_{woo}`
     - Wood and coarse root decomposition
     - gC m\ :sup:`-2` d\ :sup:`-1`
     - ``woo2cwd``
     - :math:`C_{woo}` transferred to :math:`C_{cwd}`
   * - :math:`DEC_{roo}`
     - Fine root decomposition
     - gC m\ :sup:`-2` d\ :sup:`-1`
     - ``roo2lit``
     - :math:`C_{roo}` transferred to :math:`C_{lit}`
   * - :math:`DEC_{lab}`
     - Labile decomposition
     - gC m\ :sup:`-2` d\ :sup:`-1`
     - ``lab2lit``
     - :math:`C_{lab}` transferred to :math:`C_{lit}`
   * - :math:`DEC_{cwd}`
     - Coarse woody debris decomposition
     - gC m\ :sup:`-2` d\ :sup:`-1`
     - ``cwd2som``
     - :math:`C_{cwd}` transferred to :math:`C_{som}`
   * - :math:`DEC_{lit}`
     - Litter decomposition
     - gC m\ :sup:`-2` d\ :sup:`-1`
     - ``lit2som``
     - :math:`C_{lit}` transferred to :math:`C_{som}`
   * - :math:`DIS_{lab}`
     - :math:`C_{lab}` disturbance loss
     - gC m\ :sup:`-2` d\ :sup:`-1`
     - ``dist_lab``
     - 
   * - :math:`DIS_{fol}`
     - :math:`C_{fol}` disturbance loss
     - gC m\ :sup:`-2` d\ :sup:`-1`
     - ``dist_fol``
     - 
   * - :math:`DIS_{woo}`
     - :math:`C_{woo}` disturbance loss
     - gC m\ :sup:`-2` d\ :sup:`-1`
     - ``dist_woo``
     - 
   * - :math:`DIS_{roo}`
     - :math:`C_{roo}` disturbance loss
     - gC m\ :sup:`-2` d\ :sup:`-1`
     - ``dist_roo``
     - 
   * - :math:`FIR_{lab}`
     - :math:`C_{lab}` fire loss
     - gC m\ :sup:`-2` d\ :sup:`-1`
     - ``f_lab``
     - Combustion loss to atmosphere
   * - :math:`FIR_{fol}`
     - :math:`C_{fol}` fire loss
     - gC m\ :sup:`-2` d\ :sup:`-1`
     - ``f_fol``
     - Combustion loss to atmosphere
   * - :math:`FIR_{woo}`
     - :math:`C_{woo}` fire loss
     - gC m\ :sup:`-2` d\ :sup:`-1`
     - ``f_woo``
     - Combustion loss to atmosphere
   * - :math:`FIR_{roo}`
     - :math:`C_{roo}` fire loss
     - gC m\ :sup:`-2` d\ :sup:`-1`
     - ``f_roo``
     - Combustion loss to atmosphere
   * - :math:`FIR_{lit}`
     - :math:`C_{lit}` fire loss
     - gC m\ :sup:`-2` d\ :sup:`-1`
     - ``f_lit``
     - Combustion loss to atmosphere
   * - :math:`FIR_{cwd}`
     - :math:`C_{cwd}` fire loss
     - gC m\ :sup:`-2` d\ :sup:`-1`
     - ``f_cwd``
     - Combustion loss to atmosphere
   * - :math:`FIR_{som}`
     - :math:`C_{som}` fire loss
     - gC m\ :sup:`-2` d\ :sup:`-1`
     - ``f_som``
     - Combustion loss to atmosphere
   * - :math:`FIRx_{lab}`
     - :math:`C_{lab}` fire mortality 
     - gC m\ :sup:`-2` d\ :sup:`-1`
     - ``fx_lab2lit``
     - Transfer of :math:`C_{lab}` to :math:`C_{lit}` due to fire
   * - :math:`FIRx_{fol}`
     - :math:`C_{fol}` fire mortality 
     - gC m\ :sup:`-2` d\ :sup:`-1`
     - ``fx_fol2lit``
     - Transfer of :math:`C_{fol}` to :math:`C_{lit}` due to fire
   * - :math:`FIRx_{roo}`
     - :math:`C_{roo}` fire mortality 
     - gC m\ :sup:`-2` d\ :sup:`-1`
     - ``fx_roo2lit``
     - Transfer of :math:`C_{roo}` to :math:`C_{lit}` due to fire
   * - :math:`FIRx_{woo}`
     - :math:`C_{woo}` fire mortality 
     - gC m\ :sup:`-2` d\ :sup:`-1`
     - ``fx_woo2cwd``
     - Transfer of :math:`C_{woo}` to :math:`C_{cwd}` due to fire
   * - :math:`FIRx_{cwd}`
     - :math:`C_{cwd}` fire mortality 
     - gC m\ :sup:`-2` d\ :sup:`-1`
     - ``fx_cwd2som``
     - Transfer of :math:`C_{cwd}` to :math:`C_{som}` due to fire
   * - :math:`FIRx_{lit}`
     - :math:`C_{lit}` fire mortality 
     - gC m\ :sup:`-2` d\ :sup:`-1`
     - ``fx_lit2som``
     - Transfer of :math:`C_{lit}` to :math:`C_{som}` due to fire
   * - :math:`RhAe_{lit}`
     - Aerobic heterotrophic respiration from :math:`C_{lit}` 
     - gC m\ :sup:`-2` d\ :sup:`-1`
     - ``ae_rh_lit``
     - 
   * - :math:`RhAe_{som}`
     - Aerobic heterotrophic respiration from :math:`C_{som}` 
     - gC m\ :sup:`-2` d\ :sup:`-1`
     - ``ae_rh_som``
     - 
   * - :math:`RhAe_{tot}`
     - Total heterotrophic respiration CO\ :sub:`2` flux
     - gC m\ :sup:`-2` d\ :sup:`-1`
     - ``rh_co2``
     - Sum of :math:`RhAe_{cwd}`, :math:`RhAe_{lit}`, and :math:`RhAe_{som}`
   * - :math:`RhAn_{cwd}`
     - Anaerobic heterotrophic respiration from :math:`C_{cwd}` 
     - gC m\ :sup:`-2` d\ :sup:`-1`
     - ``an_rh_cwd``
     - 
   * - :math:`RhAn_{lit}`
     - Anaerobic heterotrophic respiration from :math:`C_{lit}` 
     - gC m\ :sup:`-2` d\ :sup:`-1`
     - ``an_rh_lit``
     - 
   * - :math:`RhAn_{som}`
     - Anaerobic heterotrophic respiration from :math:`C_{som}` 
     - gC m\ :sup:`-2` d\ :sup:`-1`
     - ``an_rh_som``
     - 
   * - :math:`RhAn_{tot}`
     - Total heterotrophic respiration CH\ :sub:`4` flux
     - gC m\ :sup:`-2` d\ :sup:`-1`
     - ``rh_ch4``
     - Sum of :math:`RhAn_{cwd}`, :math:`RhAn_{lit}`, and :math:`RhAn_{som}`
   * - :math:`Q_{surf}`
     - Surface H\ :sub:`2`\ O runoff 
     - mmH\ :sub:`2`\ O d\ :sup:`-1`
     - ``q_surf``
     - 
   * - :math:`Q_{ly1}`
     - Runoff from :math:`W_{ly1}`
     - mmH\ :sub:`2`\ O d\ :sup:`-1`
     - ``q_ly1``
     - 
   * - :math:`Q_{ly2}`
     - Runoff from :math:`W_{ly2}`
     - mmH\ :sub:`2`\ O d\ :sup:`-1`
     - ``q_ly2``
     - 
   * - :math:`Q_{ly3}`
     - Runoff from :math:`W_{ly3}`
     - mmH\ :sub:`2`\ O d\ :sup:`-1`
     - ``q_ly3``
     - 
   * - :math:`Qx_{inf}`
     - Surface H\ :sub:`2`\ O infiltration to :math:`W_{ly1}`
     - mmH\ :sub:`2`\ O d\ :sup:`-1`
     - ``infil``
     - 
   * - :math:`Qx_{ly1,2}`
     - Transfer from :math:`W_{ly1}` to :math:`W_{ly2}`
     - mmH\ :sub:`2`\ O d\ :sup:`-1`
     - ``ly1xly2``
     - Positive values indicate downward transfer
   * - :math:`Qx_{ly2,3}`
     - Transfer from :math:`W_{ly2}` to :math:`W_{ly3}`
     - mmH\ :sub:`2`\ O d\ :sup:`-1`
     - ``ly2xly3``
     - Positive values indicate downward transfer
   * - :math:`EQ_{ly1}`
     - Internal energy of :math:`Q_{ly1}`
     - J m\ :sup:`-2` d\ :sup:`-1`
     - ``q_ly1_e``
     - Based on temperature of :math:`W_{ly1}`
   * - :math:`EQ_{ly2}`
     - Internal energy of :math:`Q_{ly2}`
     - J m\ :sup:`-2` d\ :sup:`-1`
     - ``q_ly2_e``
     - Based on temperature of :math:`W_{ly2}`
   * - :math:`EQ_{ly3}`
     - Internal energy of :math:`Q_{ly3}`
     - J m\ :sup:`-2` d\ :sup:`-1`
     - ``q_ly2_e``
     - Based on temperature of :math:`W_{ly2}`
   * - :math:`EQx_{inf}`
     - Internal energy of :math:`Qx_{inf}`
     - J m\ :sup:`-2` d\ :sup:`-1`
     - ``infil_e``
     - Based on weighted average temperature of snow melt (273.15K) and rainfall (air temperature)';
   * - :math:`EQx_{ly1,2}`
     - Internal energy of :math:`Qx_{ly1,2}`
     - J m\ :sup:`-2` d\ :sup:`-1`
     - ``ly1xly2_e``
     - Based on temperature of donor pool (:math:`W_{ly1}` when :math:`Qx_{ly1,2}` is positive, :math:`W_{ly2}` when :math:`Qx_{ly1,2}` is negative)
   * - :math:`EQx_{ly2,3}`
     - Internal energy of :math:`Qx_{ly2,3}`
     - J m\ :sup:`-2` d\ :sup:`-1`
     - ``ly2xly3_e``
     - Based on temperature of donor pool (:math:`W_{ly2}` when :math:`Qx_{ly2,3}` is positive, :math:`W_{ly3}` when :math:`Qx_{ly2,3}` is negative)
   * - :math:`EQthx_{ly1,2}`
     - Thermal condictivity between :math:`E_{ly1}` and :math:`E_{ly2}`
     - J m\ :sup:`-2` d\ :sup:`-1`
     - ``ly1xly2_th_e``
     - 
   * - :math:`EQthx_{ly2,3}`
     - Thermal condictivity between :math:`E_{ly2}` and :math:`E_{ly3}`
     - J m\ :sup:`-2` d\ :sup:`-1`
     - ``ly2xly3_th_e``
     - 
   * - :math:`E_{gh}`
     - Ground heat heat flux
     - J m\ :sup:`-2` d\ :sup:`-1`
     - ``gh_in``
     - 
   * - :math:`E_{geo}`
     - Geological heat flux
     - J m\ :sup:`-2` d\ :sup:`-1`
     - ``geological``
     - 
   * - :math:`Snow`
     - Snowfall 
     - mmH\ :sub:`2`\ O d\ :sup:`-1`
     - ``snowfall``
     - Identical to :math:`P_{snow}` forcing
   * - :math:`Melt`
     - Snowmelt 
     - mmH\ :sub:`2`\ O d\ :sup:`-1`
     - ``melt``
     - 
   * - :math:`Subl`
     - Sublimation 
     - mmH\ :sub:`2`\ O d\ :sup:`-1`
     - ``sublimation``
     - 
   * - :math:`Ev`
     - Evaporation 
     - mmH\ :sub:`2`\ O d\ :sup:`-1`
     - ``evap``
     - 
   * - :math:`Tr_{ly1}`
     - Transpiration from :math:`W_{ly1}`
     - mmH\ :sub:`2`\ O d\ :sup:`-1`
     - ``transp1``
     - 
   * - :math:`Tr_{ly2}`
     - Transpiration from :math:`W_{ly2}`
     - mmH\ :sub:`2`\ O d\ :sup:`-1`
     - ``transp2``
     - 
   * - :math:`EEv`
     - Internal energy of :math:`Ev`
     - J m\ :sup:`-2` d\ :sup:`-1`
     - ``evap_e``
     - 
   * - :math:`ETr_{ly1}`
     - Internal energy of :math:`Tr_{ly1}`
     - J m\ :sup:`-2` d\ :sup:`-1`
     - ``transp1_e``
     - 
   * - :math:`ETr_{ly2}`
     - Internal energy of :math:`Tr_{ly2}`
     - J m\ :sup:`-2` d\ :sup:`-1`
     - ``transp2_e``
     - 
   * - :math:`SW_{in}`
     - Incoming shortwave radiation
     - W m\ :sup:`-2`
     - ``SWin``
     - :math:`SSRD` forcing converted to W m\ :sup:`-2`
   * - :math:`LW_{in}`
     - Incoming longtwave radiation
     - W m\ :sup:`-2`
     - ``LWin``
     - :math:`STRD` forcing converted to W m\ :sup:`-2`
   * - :math:`SW_{out}`
     - Outgoing shortwave radiation
     - W m\ :sup:`-2`
     - ``SWout``
     - 
   * - :math:`LW_{out}`
     - Outgoing longtwave radiation
     - W m\ :sup:`-2`
     - ``LWout``
     - 
   * - :math:`R_{net}`
     - Net radiation
     - W m\ :sup:`-2`
     - ``net_radiation``
     - Defined as :math:`SW_{in} + LW_{in} - SW_{out} - LW_{out}`
   * - :math:`H`
     - Sensible heat flux
     - W m\ :sup:`-2`
     - ``sensible_heat``
     - 
   * - :math:`LE`
     - Latent heat flux
     - W m\ :sup:`-2`
     - ``latent_heat``
     - 
   * - :math:`G`
     - Ground heat flux
     - W m\ :sup:`-2`
     - ``ground_heat``
     - 



.. * - :math:`Ra_{tot}`
..   - Total autotrophic respiration
..   - gC m\ :sup:`-2` d\ :sup:`-1`
..   - `resp_auto`
.. - Includes maintenance and growth respiration

.. FLUX_INFO.NAME[F.gh_in]='Ground heat flux in converted units';
.. FLUX_INFO.ABBREVIATION[F.gh_in]='gh_in';
.. FLUX_INFO.UNITS[F.gh_in]='J/m2/day';
.. FLUX_INFO.DESCRIPTION[F.gh_in]='Ground heat flux in converted units';


.. FLUX_INFO.NAME[F.aetr]='Aerobic turnover scalar';
.. FLUX_INFO.ABBREVIATION[F.aetr]='aetr';
.. FLUX_INFO.UNITS[F.aetr]='scalar';
.. FLUX_INFO.DESCRIPTION[F.aetr]='Aerobic turnover scalar';

.. FLUX_INFO.NAME[F.antr]='Anaerobic turnover scalar';
.. FLUX_INFO.ABBREVIATION[F.antr]='antr';
.. FLUX_INFO.UNITS[F.antr]='scalar';
.. FLUX_INFO.DESCRIPTION[F.antr]='Anaerobic turnover scalar';

.. FLUX_INFO.NAME[F.an_co2_c_ratio]='Anaerobic CO2 C ratio';
.. FLUX_INFO.ABBREVIATION[F.an_co2_c_ratio]='an_co2_c_ratio';
.. FLUX_INFO.UNITS[F.an_co2_c_ratio]='fraction';
.. FLUX_INFO.DESCRIPTION[F.an_co2_c_ratio]='CO2 fraction in anaerobic C decomposition';

.. FLUX_INFO.NAME[F.an_ch4_c_ratio]='Anaerobic CH4 C ratio';
.. FLUX_INFO.ABBREVIATION[F.an_ch4_c_ratio]='an_ch4_c_ratio';
.. FLUX_INFO.UNITS[F.an_ch4_c_ratio]='fraction';
.. FLUX_INFO.DESCRIPTION[F.an_ch4_c_ratio]='CH4 fraction in anaerobic C decomposition';

.. FLUX_INFO.NAME[F.target_LAI]='Target leaf area index';
.. FLUX_INFO.ABBREVIATION[F.target_LAI]='target_LAI';
.. FLUX_INFO.UNITS[F.target_LAI]='m2/m2';
.. FLUX_INFO.DESCRIPTION[F.target_LAI]='Target LAI calculated based on KNORR phenology scheme influenced by water, light, and temperature. Coded in KNORR_ALLOCATION.c';

.. FLUX_INFO.NAME[F.dlambda_dt]='Delta leaf area index';
.. FLUX_INFO.ABBREVIATION[F.dlambda_dt]='dlambda_dt';
.. FLUX_INFO.UNITS[F.dlambda_dt]='m2/m2';
.. FLUX_INFO.DESCRIPTION[F.dlambda_dt]='Difference between Target leaf area index (target_LAI) and current leaf area index. Coded in KNORR_ALLOCATION.c ';

.. FLUX_INFO.NAME[F.f_temp_thresh]='Temperature threshold';
.. FLUX_INFO.ABBREVIATION[F.f_temp_thresh]='f_temp_thresh';
.. FLUX_INFO.UNITS[F.f_temp_thresh]='fraction';
.. FLUX_INFO.DESCRIPTION[F.f_temp_thresh]='Fraction of plants within pixel able to grow foliage on temperature threshold. Coded in KNORR_ALLOCATION.c';

.. FLUX_INFO.NAME[F.f_dayl_thresh]='Daylight threshold';
.. FLUX_INFO.ABBREVIATION[F.f_dayl_thresh]='f_dayl_thresh';
.. FLUX_INFO.UNITS[F.f_dayl_thresh]='fraction';
.. FLUX_INFO.DESCRIPTION[F.f_dayl_thresh]='Fraction of plants within pixel able to grow foliage based on daylight threshold. Coded in KNORR_ALLOCATION.c';

.. FLUX_INFO.NAME[F.lai_fire]='Leaf area index fire loss';
.. FLUX_INFO.ABBREVIATION[F.lai_fire]='lai_fire';
.. FLUX_INFO.UNITS[F.lai_fire]='m2/m2';
.. FLUX_INFO.DESCRIPTION[F.lai_fire]='Leaf area index fire lost to fire';

.. FLUX_INFO.NAME[F.foliar_fire_frac]='C_fol fire loss fraction';
.. FLUX_INFO.ABBREVIATION[F.foliar_fire_frac]='foliar_fire_frac';
.. FLUX_INFO.UNITS[F.foliar_fire_frac]='fraction';
.. FLUX_INFO.DESCRIPTION[F.foliar_fire_frac]='Foliage pool lost due to fire';


.. FLUX_INFO.NAME[F.ets]='Total land-atmosphere water';
.. FLUX_INFO.ABBREVIATION[F.ets]='ets';
.. FLUX_INFO.UNITS[F.ets]='kgH2O/m2/day';
.. FLUX_INFO.DESCRIPTION[F.ets]='Total land-atmosphere water flux (evaporation + transpiration + sublimation)';

.. FLUX_INFO.NAME[F.beta_factor]='Stress scaling factor';
.. FLUX_INFO.ABBREVIATION[F.beta_factor]='beta_factor';
.. FLUX_INFO.UNITS[F.beta_factor]='fraction';
.. FLUX_INFO.DESCRIPTION[F.beta_factor]='Stress due to cold weather and water availability';

.. FLUX_INFO.NAME[F.soil_beta_factor]='H2O stress scaling factor';
.. FLUX_INFO.ABBREVIATION[F.soil_beta_factor]='soil_beta_factor';
.. FLUX_INFO.UNITS[F.soil_beta_factor]='fraction';
.. FLUX_INFO.DESCRIPTION[F.soil_beta_factor]='Stress due to soil water availability';



.. list-table:: Parameters
   :widths: 1 1 1 1 1 1
   :header-rows: 1

   * - Abbreviation
     - Description
     - Units
     - Code string
     - Prior range
     - Notes
   * - :math:`I_{max}`
     - Maximum infiltration capacity
     - mmH\ :sub:`2`\ O d\ :sup:`-1`
     - ``max_infil``
     - :math:`[1,100]`
     - 
   * - :math:`\kappa_0`
     - Saturated hydraulic conductivity
     - m s\ :sup:`-1`
     - ``hydr_cond``
     - :math:`[10^{-9},10^{-4}]`
     - 
   * - :math:`b`
     - Retention parameter
     - None
     - ``retention``
     - :math:`[1.5,10]`
     - 
   * - :math:`z_{ly1}`
     - :math:`W_{ly1}` depth
     - m
     - ``LY1_z``
     - :math:`[0.01,1]`
     - 
   * - :math:`z_{ly2}`
     - :math:`W_{ly2}` depth
     - m
     - ``LY2_z``
     - :math:`[0.01,20]`
     - 
   * - :math:`z_{ly3}`
     - :math:`W_{ly3}` depth
     - m
     - ``LY3_z``
     - :math:`[0.01,100]`
     - 
   * - :math:`p_{ly1}`
     - :math:`W_{ly1}` porosity
     - m\ :sup:`3` m\ :sup:`-3`
     - ``LY1_por``
     - :math:`[0.2,0.8]`
     - 
   * - :math:`p_{ly2}`
     - :math:`W_{ly2}` porosity
     - m\ :sup:`3` m\ :sup:`-3`
     - ``LY2_por``
     - :math:`[0.2,0.8]`
     - 
   * - :math:`p_{ly3}`
     - :math:`W_{ly3}` porosity
     - m\ :sup:`3` m\ :sup:`-3`
     - ``LY3_por``
     - :math:`[0.2,0.8]`
     - 
   * - :math:`\psi_{fc}`
     - Water potential at field capacity
     - -MPa
     - ``field_cap``
     - :math:`[0.01,0.1]`
     - 
   * - :math:`DF_{lit}`
     - Fraction of decomposed :math:`C_{lit}` transferred to :math:`C_{som}`
     - 1
     - ``tr_lit2som``
     - :math:`[0.01,0.99]`
     - 
   * - :math:`DF_{cwd}`
     - Fraction of decomposed :math:`C_{cwd}` transferred to :math:`C_{som}`
     - 1
     - ``tr_cwd2som``
     - :math:`[0.01,0.99]`
     -  
   * - :math:`K_{roo}`
     - Autotrophic maintenance respiration coefficient for roots at 25\ :math:`^{\circ}`\ C 
     - d\ :sup:`-1`
     - ``rauto_mr_r``
     - :math:`[10^{-4},10^{-2}]`
     - 
   * - :math:`K_{woo}`
     - Autotrophic maintenance respiration coefficient for wood at 25\ :math:`^{\circ}`\ C 
     - d\ :sup:`-1`
     - ``rauto_mr_w``
     - :math:`[10^{-6},5 \times 10^{-5}]`
     - 
   * - :math:`Q_{10}^{Ra}`
     - Autotrophic maintenance respiration Q\ :sub:`10` coefficient
     - 1
     - ``rauto_mr_q10``
     - :math:`[1,5]`
     - 
   * - :math:`Q_{10}^{dark}`
     - Dark respiration Q\ :sub:`10` coefficient
     - 1
     - ``rauto_mr_q10``
     - :math:`[1,5]`
     - 
   * - :math:`\Gamma`
     - Growth yield
     - 1
     - ``rauto_gr``
     - :math:`[0.6,0.95]`
     - gC in new biomass per gC used for growth, see ``Cannell and Thornley 2000``
   * - :math:`k_{woo}`
     - Background turnover rate of :math:`C_{woo}`
     - d\ :sup:`-1`
     - ``t_wood``
     - :math:`[2.5\times 10^{-5},10^{-2}]`
     - 
   * - :math:`k_{lab}`
     - Background turnover rate of :math:`C_{lab}`
     - d\ :sup:`-1`
     - ``t_lab``
     - :math:`[10^{-4},10^{-2}]`
     - 
   * - :math:`k_{roo}`
     - Background turnover rate of :math:`C_{roo}`
     - d\ :sup:`-1`
     - ``t_roo``
     - :math:`[10^{-4},10^{-2}]`
     - 
   * - :math:`k_{fol}`
     - Background turnover rate of :math:`C_{fol}`
     - d\ :sup:`-1`
     - ``t_fol``
     - :math:`[6.8\times 10^{-4},3.34\times 10^{-2}]`
     - 
   * - :math:`k_{lit}`
     - Background turnover rate of :math:`C_{lit}`
     - d\ :sup:`-1`
     - ``t_lit``
     - :math:`[10^{-4},10^{-1}]`
     - ``check units???``
   * - :math:`k_{cwd}`
     - Background turnover rate of :math:`C_{cwd}`
     - d\ :sup:`-1`
     - ``t_cwd``
     - :math:`[5\times 10^{-5},5\times 10^{-2}]`
     - ``check units???``
   * - :math:`k_{som}`
     - Background turnover rate of :math:`C_{som}`
     - d\ :sup:`-1`
     - ``t_som``
     - :math:`[10^{-7},10^{-1}]`
     - ``check units???``
   * - :math:`Q_{10}^{aer}`
     - Q\ :sub:`10` coefficient of aerobic heterotrphic respiration
     - 1
     - ``Q10rhco2``
     - :math:`[1,5]`
     - 
   * - :math:`Q_{10}^{ana}`
     - Q\ :sub:`10` coefficient of anaerobic heterotrphic respiration
     - 1
     - ``Q10ch4``
     - :math:`[1,5]`
     - 
   * - :math:`f_{CH_{4}}`
     - CH\ :sub:`4` to CO\ :sub:`2` conversion ratio
     - 1
     - ``r_ch4``
     - :math:`[0.001,0.9]`
     - 
   * - :math:`LCMA`
     - Leaf carbon mass per area
     - gC m\ :sup:`-2`
     - ``LCMA``
     - :math:`[5,200]`
     - 
   * - :math:`i_{C_{lab}}`
     - Initial value of :math:`C_{lab}`
     - gC m\ :sup:`-2`
     - ``i_labile``
     - :math:`[1,10^{5}]`
     - 
   * - :math:`i_{C_{fol}}`
     - Initial value of :math:`C_{fol}`
     - gC m\ :sup:`-2`
     - ``i_foliar``
     - :math:`[1,2\times 10^{3}]`
     - 
   * - :math:`i_{C_{roo}}`
     - Initial value of :math:`C_{roo}`
     - gC m\ :sup:`-2`
     - ``i_root``
     - :math:`[1,2\times 10^{3}]`
     - 
   * - :math:`i_{C_{woo}}`
     - Initial value of :math:`C_{woo}`
     - gC m\ :sup:`-2`
     - ``i_wood``
     - :math:`[1,10^{5}]`
     - 
   * - :math:`i_{C_{cwd}}`
     - Initial value of :math:`C_{cwd}`
     - gC m\ :sup:`-2`
     - ``i_cwd``
     - :math:`[1,10^{5}]`
     - 
   * - :math:`i_{C_{lit}}`
     - Initial value of :math:`C_{lit}`
     - gC m\ :sup:`-2`
     - ``i_lit``
     - :math:`[1,2\times 10^{3}]`
     - 
   * - :math:`i_{C_{som}}`
     - Initial value of :math:`C_{som}`
     - gC m\ :sup:`-2`
     - ``i_som``
     - :math:`[1,2\times 10^{5}]`
     - 
   * - :math:`i_{\theta_{ly1}}`
     - Initial value of :math:`\theta_{ly1}`
     - 1
     - ``i_LY1_SM``
     - :math:`[0.01,1]`
     - 
   * - :math:`i_{\theta_{ly2}}`
     - Initial value of :math:`\theta_{ly2}`
     - 1
     - ``i_LY2_SM``
     - :math:`[0.01,1]`
     - 
   * - :math:`i_{\theta_{ly3}}`
     - Initial value of :math:`\theta_{ly3}`
     - 1
     - ``i_LY3_SM``
     - :math:`[0.01,1]`
     - 
   * - :math:`i_{SWE}`
     - Initial value of :math:`\W_{swe}`
     - mm H\ :sub:`2`\ O
     - ``i_SWE``
     - :math:`[10^{-6},10^{4}]`
     - 
   * - :math:`i_{E_{ly1}}`
     - Initial value of :math:`E_{ly1}`
     - J mmH\ :sub:`2`\ O\ :sup:`-1`
     - ``i_LY1_E``
     - :math:`[4.67\times 10^{5},1.11\times 10^{6}]`
     - ``Energy in 1mm H2O at -50C to 1mm H2O at +50C ?????``
   * - :math:`i_{E_{ly2}}`
     - Initial value of :math:`E_{ly2}`
     - J mmH\ :sub:`2`\ O\ :sup:`-1`
     - ``i_LY2_E``
     - :math:`[4.67\times 10^{5},1.11\times 10^{6}]`
     - ``Energy in 1mm H2O at -50C to 1mm H2O at +50C ?????``
   * - :math:`i_{E_{ly3}}`
     - Initial value of :math:`E_{ly3}`
     - J mmH\ :sub:`2`\ O\ :sup:`-1`
     - ``i_LY3_E``
     - :math:`[4.67\times 10^{5},1.11\times 10^{6}]`
     - ``Energy in 1mm H2O at -50C to 1mm H2O at +50C ?????``
   * - :math:`VHC_{E_{ly1}}`
     - Volumetric heat capacity of :math:`E_{ly1}`
     - J K m\ :sup:`-3`
     - ``LY1_vhc``
     - :math:`[1.3\times 10^{6},3\times 10^{6}]`
     - 
   * - :math:`VHC_{E_{ly2}}`
     - Volumetric heat capacity of :math:`E_{ly2}`
     - J K m\ :sup:`-3`
     - ``LY2_vhc``
     - :math:`[1.3\times 10^{6},3\times 10^{6}]`
     - 
   * - :math:`VHC_{E_{ly3}}`
     - Volumetric heat capacity of :math:`E_{ly3}`
     - J K m\ :sup:`-3`
     - ``LY3_vhc``
     - :math:`[1.3\times 10^{6},3\times 10^{6}]`
     - 
   * - :math:`CF_{fol}`
     - Foliar combustion factor
     - 1
     - ``cf_foliar``
     - :math:`[0.01,1]`
     - 
   * - :math:`CF_{lig}`
     - Ligneous combustion factor
     - 1
     - ``cf_ligneous``
     - :math:`[0.01,1]`
     - 
   * - :math:`CF_{som}`
     - Soil combustion factor
     - 1
     - ``cf_DOM``
     - :math:`[0.01,1]`
     - 
   * - :math:`r`
     - Resilience factor
     - 1
     - ``resilience``
     - :math:`[0.01,1]`
     - 
   * - :math:`Q_{ex}`
     - Excess runoff factor
     - 1
     - ``Q_excess``
     - :math:`[0.01,1]`
     - 
   * - :math:`g_{1}`
     - Medlyn :math:`g_{1}` parameter ``cite paper``
     - 1
     - ``Med_g1``
     - :math:`[1.79,5.79]`
     - 
   * - :math:`V_{cmax25}`
     - Maximum rate of RuBisCO carboxylization at 25\ :math:`^{\circ}`\ C 
     - :math:`\mu`\ molCO\ :sub:`2` m\ :sup:`-2` s\ :sup:`-1`
     - ``Vcmax25``
     - :math:`[1,150]`
     - 
   * - :math:`Tmin_{min}`
     - Minimum cold temperature scaling factor 
     - K
     - ``Tminmin``
     - :math:`[258.15,273.15]`
     - ``cite Stetz paper``
   * - :math:`Tmin_{max}`
     - Maximum cold temperature scaling factor 
     - K
     - ``Tminmax``
     - :math:`[273.15,288.15]`
     - ``cite Stetz paper``
   * - :math:`T_{upp}`
     - Upper temperature limit for photosynthesis 
     - K
     - ``Tupp``
     - :math:`[249.15,318.15]`
     - 
   * - :math:`T_{down}`
     - Lower temperature limit for photosynthesis 
     - K
     - ``Tdown``
     - :math:`[213.15,286.15]`
     - 
   * - :math:`g_{a}`
     - Aerodynamic conductance
     - m s\ :sup:`-1`
     - ``ga``
     - :math:`[10^{-3},10]`
     - 
   * - :math:`\Omega`
     - Leaf clumping index
     - 1
     - ``clumping``
     - :math:`[0.35,1]`
     -      
   * - :math:`\rho_{PAR}`
     - Reflectance for photosynthetically active radiation
     - 1
     - ``leaf_refl_par``
     - :math:`[0.01,0.5]`
     -      
   * - :math:`\rho_{NIR}`
     - Reflectance for near infrared radiation
     - 1
     - ``leaf_refl_nir``
     - :math:`[0.3,0.7]`
     -      
   * - :math:`sn_{1}`
     - Minimum temperature threshold for snow melt
     - K
     - ``min_melt``
     - :math:`[263.15,283.15]`
     -      
   * - :math:`sn_{2}`
     - Snowmelt slope
     - ``????``
     - ``melt_slope``
     - :math:`[10^{-5},1]`
     -      
   * - :math:`sn_{3}`
     - Snow cover fraction scalar
     - ``????``
     - ``scf_scalar``
     - :math:`[10,10^{3}]`
     -         
   * - :math:`S_{fv}`
     - Scalar for aerobic volumetric fraction
     - ``????``
     - ``S_fv``
     - :math:`[1,100]`
     -      
   * - :math:`\theta_{opt}`
     - Optimum soil moisture for water scalar in heterotrophic respiration
     - 1
     - ``thetas_opt``
     - :math:`[0.2,1]`
     -      
   * - :math:`F_{wc}`
     - water scaler fW value at the end point C
     - ``????``
     - ``fwc``
     - :math:`[0.01,1]`
     -      
   * - :math:`Ev_{max}`
     - Maximum pan evaporation
     - mm d\ :sup:`-1`
     - ``maxPevap``
     - :math:`[0.01,100]`
     -      
   * - :math:`T_{\phi}`
     - Maximum temperature at leaf onset
     - K
     - ``T_phi``
     - :math:`[268.15,323.15]`
     -  
   * - :math:`T_{range}`
     - Spatial range of mean temperature at leaf onset
     - K
     - ``T_range``
     - :math:`[0.1,10]`
     -  
   * - :math:`\Lambda_{g}`
     - LAI linear growth constant
     - Per time-step
     - ``plgr``
     - :math:`[0.001,0.5]`
     -  
   * - :math:`K_{leaf}`
     - Inverse of leaf longevity during senescence period
     - Per time-step
     - ``k_leaf``
     - :math:`[0.001,0.5]`
     -  
   * - :math:`\Lambda_{max}`
     - Intrinsic maximum LAI
     - m\ :sup:`2` m\ :sup:`-2`
     - ``plgr``
     - :math:`[0.1,10]`
     -  
   * - :math:`\tau_{LAI}`
     - Target survival time for LAI under water-deficit conditions
     - d ``????``
     - ``tau_W``
     - :math:`[0.1,300]`
     -  
   * - :math:`t_{c}`
     - Mean daylength at leaf shedding
     - hours of sunlight d\ :sup:`-1`
     - ``time_c``
     - :math:`[2,22]`
     -  
   * - :math:`t_{r}`
     - Spatial range of :math:`t_{c}`
     - hours of sunlight d\ :sup:`-1`
     - ``time_r``
     - :math:`[0.1,6]`
     -  
   * - :math:`T_{mem}`
     - Initial value of LAI temperature memory
     - K
     - ``init_T_mem``
     - :math:`[268.14,323.15]`
     -  
   * - :math:`\theta_{mem}`
     - Initial value of LAI water/structural memory
     - 1
     - ``init_LAIW_mem``
     - :math:`[0.1,1]`
     -  
   * - :math:`\psi_{50}`
     - 50% stomatal closure water potential
     - -MPa
     - ``psi_50``
     - :math:`[0.1,30]`
     - Water potential when soil-plant continuum is at 50% hydraulic conductivity due to stomatal closure
   * - :math:`\psi_{hmf}`
     - 50% mortality water potential
     - -MPa
     - ``psi_50``
     - :math:`[0.1,30]`
     - Water potential triggering 50% biomass mortality due to cavitation
   * - :math:`\Beta_{plgr}`
     - Photosynthetic water stress logistic growth rate
     - None
     - ``beta_lgr``
     - :math:`[4.1,50]`
     - 
   * - :math:`\Beta_{mlgr}`
     - Mortality water stress logistic growth rate
     - None
     - ``beta_lgrHMF``
     - :math:`[4.1,50]`
     - 
   * - :math:`\phi_{rl}`
     - Root-to-leaf allocation ratio
     - 1
     - ``phi_RL``
     - :math:`[10^{-4},5]`
     - Ratio of carbon allocation to root per target foliar pool size
   * - :math:`\phi_{wl}`
     - Wood-to-leaf allocation ratio
     - 1
     - ``phi_WL``
     - :math:`[10^{-4},5]`
     - Ratio of carbon allocation to wood per target foliar pool size
   * - :math:`\kappa_{deep}`
     - Deep soil thermal conductivity
     - W m\ :sup:`-1` K\ :sup:`-1`
     - ``thermal_cond``
     - :math:`[0.3,2]`
     - 
   * - :math:`\kappa_{surf}`
     - Surface soil thermal conductivity
     - W m\ :sup:`-1` K\ :sup:`-1`
     - ``thermal_cond_surf``
     - :math:`[0.03,2]`
     - 
   * - :math:`Q_{10}^{gpp}`
     - Q\ :sub:`10` coefficient for :math:`GPP`
     - 1
     - ``q10canopy``
     - :math:`[1,5]`
     - 
   * - :math:`RDSF`
     - ``Something about dark respiration??``
     - ``???``
     - ``canopyRdsf``
     - :math:`[0.005,0.025]`
     - 
   * - :math:`Es`
     - Sublimation rate
     - ``mm/day/SCF/kPha/(MJ/m2/d) ???``
     - ``sublimation_rate``
     - :math:`[0.001,10]`
     - 
   * - :math:`f_{root}`
     - Root fraction of :math:`W_{ly2}` to :math:`W_{ly1}`
     - ``mm/day/SCF/kPha/(MJ/m2/d) ???``
     - ``sublimation_rate``
     - :math:`[0.001,1]`
     - 