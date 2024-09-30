.. _sub:allocation_autotrophic_respiration:

Allocation and autotrophic respiration
======================================

We develop a model for plant growth, allocation, and autotrophic respiration that is generic and globally applicable. This scheme is designed to better represent the hierarchy of controls that govern these processes than previous DALEC model versions. We first describe the principles of this new scheme and its assumptions, then present the equations and their coupling to other components of DALEC.

Photosynthesis supplies the plant with new carbon substrate. The fate of this assimilated carbon depends on a number of other processes that can be broadly partitioned to autotrophic respiration, growth, and reserves. We do not explicitly consider the formation of secondary metabolites (e.g., for defense or reproduction) or carbon loss processes such as root exudates or emission of volatile organic compounds, generally considered to be minor components of the plant carbon balance.

Plants store carbon as labile non-structural carbohydrate (NSC) reserves (e.g., soluble sugars, starch) and as structural tissue (e.g., wood, leaf, fine root). Carbon storage as NSCs plays a critical role in buffering against differences in the uptake and demand of carbon (Hartmann and Trumbore, 2016). There are two main forms of NSCs: soluble sugars and starch. Soluble sugars are readily available and metabolically active. Starch is immobile and metabolically inert but can be remobilized for maintenance and growth. We do not explicitly distinguish between these two forms of NSCs, such that our approach assumes that the NSC pool is equivalent to sucrose plus starch and that mortality occurs when the NSC pool goes to zero (discussed further below). Furthermore, NSC pool dynamics are represented as a passive process, governed by the balance between sources and sinks, and we do not currently consider active regulation of NSC storage (:ref:`Dietze et al 2014 <Dietze2014>`, :ref:`MartinezVilalta et al 2016 <MartinezVilalta2016>`), given current uncertainties around the governing processes (:ref:`ElOmari et al 2022 <ElOmari2022>`).

To represent autotrophic respiration, we follow the growth and maintenance paradigm (:ref:`Mccree et al 1970 <Mccree1970>`), which stipulates that all respiratory metabolic processes fall under either the growth or maintenance terms (:ref:`Amthor et al 2000 <Amthor2000>`). Growth respiration is associated with the growth rate of structural biomass, and maintenance respiration is associated with the mass of live biomass. This differs from many other DALEC versions that assume autotrophic respiration as a fixed fraction of GPP, which follows the hypothesis of a fixed ratio of NPP:GPP or "carbon-use efficiency" (CUE=NPP/GPP), regardless of biomass, age, or environmental conditions (:ref:`Waring et al 1998 <Waring1998>`,:ref:`Gifford et al 2003 <Gifford2003>`). The fixed CUE approach is favorable in its simplicity and has been used widely in models (:ref:`Friend et al 2019 <Friend2019>`, :ref:`Sierra et al 2022 <Sierra2022>`), but evidence shows that CUE is highly variable (:ref:`Collalti et al 2019 <Collalti2019>`, :ref:`Collalti et al 2020 <Collalti2020>`). The fixed CUE approach also incorrectly assumes that autotrophic respiration occurs instantaneously and as a direct consequence of GPP, which is invalid, especially on sub-annual timescales (:ref:`MartinezVilalta et al 2016 <MartinezVilalta2016>`) and conflicts with observations showing that respired CO2 is a mix of old and newly assimilated carbon (:ref:`Sierra et al 2022 <Sierra2022>`). The formulation presented here allows for dynamic plant carbon respiration and allocation, the result of which is that CUE is a time-varying diagnostic variable rather than a prescribed parameter.

The rate of change in total plant carbon (``C_plant = C_fol + C_roo + C_woo + C_lab``) is represented by

.. math::
   :label: \frac{dC_{plant}(t)}{dt}

   \frac{dC_{plant}(t)}{dt} = GPP(t) - R_{a,m}(t) - R_{a,g}(t) - S(t)

where ``R_{a,m}`` is the plant maintenance respiration flux, ``R_{a,g}`` is the plant growth respiration flux, and ``S(t)`` (*Comment from Alex: maybe we should change this symbol to something like ``F_{sinks}``, as it conflicts with ``S`` meaning a state*) is an integrated sink term that includes turnover and senescence processes that transfer carbon from the foliar, root, and woody pools to the litter and coarse woody debris pools (*Why aren't we naming these fluxes specifically, e.g., ``F_{fol_to_litter}``?*).

The rate of change in the NSC pool is represented by

.. math::
   :label: \frac{dC_{lab}(t)}{dt}

   \frac{dC_{lab}(t)}{dt} = GPP(t) - R_{a,m}(t) - R_{a,g}(t) - G(t)

where ``G(t)`` is the growth rate of structural biomass (foliar, root, wood); in other words, ``G(t)`` is the sum of the allocation fluxes to the foliar, root, and wood pools. Thus, GPP provides the carbon input to the NSC pool, and the NSCs provide the source of carbon for all autotrophic respiration losses and growth. This structure allows for the representation of the widely observed temporal decoupling of photosynthesis from ``G`` and ``R_a`` (:ref:`Fatichi et al 2014 <Fatichi2014>`).

Maintenance respiration is tissue-dependent and temperature-sensitive. We assume there is no maintenance respiration cost for the NSC pool. Foliar maintenance respiration is linked to the temperature-corrected leaf mitochondrial respiration rate (dark respiration), computed in the photosynthesis scheme and integrated over canopy leaf area index as follows:

.. math::
   :label: R_{a,m}^{fol}

   R_{a,m}^{fol}(t) = f_T(t) \cdot R_d(t) (1 - \exp(-K(t) \cdot LAI(t) \cdot P_{clumping}))/K

and ``LAI(t) = C_{fol}(t)/P_{LCMA}``, and ``f_T(t)`` is a ``Q_{10}`` temperature scaling function with a reference temperature of 25 degrees Celsius (see Equations :eq:`f_t_eq` and :eq:`f_t_eq_dark`). The wood maintenance respiration rate is calculated as

.. math::
   :label: R_{a,m}^{wood}

   R_{a,m}^{wood}(t) = f_T(t) \cdot m_{R,w} \cdot C_{woo}
  
and root respiration rate is calculated as

.. math::
   :label: R_{a,m}^{root}

   R_{a,m}^{root}(t) = f_T(t) \cdot m_{R,r} \cdot C_{roo}

where ``m_{R,w}`` and ``m_{R,r}`` are the maintenance respiration coefficients for the wood carbon pool (``C_{woo}``) and for the root carbon pool (``C_{roo}``), respectively. The ``Q_{10}`` temperature scaling functions are calculated as

.. math::
   :label: f_t_eq

   f_T(t)_{maintresp} = P_{q10}^{\left( \frac{T_{max}(t)-25}{10} \right)}


.. math::
   :label: f_t_eq_dark

   f_T(t)_{darkresp} = P_{q10, dark}^{\left( \frac{T_{max}(t)-25}{10} \right)}


*Comment from Alex: I believe ``T_{max}`` should be ``T_{mean}`` in the above equations, as the code takes mean air temperature as the input. Can you double check this?* ``T_{max}`` is air temperature in degrees Celsius, ``P_{q10}`` is the shared ``Q_{10}`` parameter for root and wood maintenance respiration, and ``P_{q10, dark}`` is the ``Q_{10}`` parameter for leaf dark respiration rate.

Maintenance respiration can only occur when there is an adequate supply of labile carbon. The available labile carbon (``NSC_a``) is given by the current NSCs plus ``GPP`` minus ``R_{a,m}^{fol}``. This assumes the net export of labile carbon from the canopy equals ``GPP`` minus ``R_{a,m}^{fol}`` and that foliar maintenance is prioritized before the maintenance of wood or root. Total maintenance respiration, ``R_{a,m}``, is therefore described by

.. math::
   :label: R_{a,m}

   R_{a,m} =
   \begin{cases}
      NSC_a & \text{if } (R_{a,m}^{wood} + R_{a,m}^{root}) \geq NSC_a \\
      R_{a,m}^{fol} + R_{a,m}^{wood} + R_{a,m}^{root} & \text{else}
   \end{cases}       

If ``NSC_a`` is not sufficient to fully maintain the existing tissues, we assume there is a fractional loss of live biomass that is proportional to the deficiency in ``NSC_a`` via a mortality factor, ``M_f``, as follows:

.. math::
   :label: M_f

   M_f =
   \begin{cases}
      1 - \frac{NSC_a}{R_{a,m}} & \text{if } R_{a,m} \geq NSC_a \\
      1 & \text{else}
   \end{cases}       

where ``M_f`` is later multiplied by each of the live biomass pools as an additional transfer flux to the litter and coarse woody debris pools. If all of ``NSC_a`` is consumed by ``R_{a,m}``, then both ``G`` and ``R_{a,g}`` are zero, meaning this formulation assumes that the maintenance of existing plant tissues is prioritized before the growth of new tissue. This provides the pathway for plant carbon starvation to occur (:ref:`McDowell et al 2008 <McDowell2008>`).

Growth respiration, ``R_{a,g}``, is related to ``G`` via the growth yield parameter (``Y_g``), which represents the units of C appearing in new biomass per unit of glucose C used for growth, or the amount of structural biomass formed per unit of photosynthates. The parameter ``Y_g`` typically has a value between 0.7-0.85, equivalent to 1.2-1.4 g glucose (g dry matter)−1 (:ref:`Cannell et al 2000 <Cannell2000>`):

.. math::
   :label: R_{a,g}

   R_{a,g}(t) = \frac{(1 - Y_g)}{Y_g} \cdot G(t)

where ``(1-Y_g)/Y_g`` is the growth coefficient, which represents the units of C respired per C unit of new biomass synthesized from a glucose substrate, with a range of approximately 0.2 to 0.4 g C (g C dry matter)−1.

The question then becomes how one should model ``G``. A wide range of approaches have been developed for terrestrial biosphere models (:ref:`Franklin et al 2012 <Franklin2012>`, :ref:`Merganicova et al 2019 <Merganicova2019>`). Here, we implement an empirical, source-sink approach to model ``G`` and its component fluxes, i.e., allocation to foliar, root, and wood pools (:ref:`Merganicova et al 2019 <Merganicova2019>`), where ``G`` depends on both labile substrate and potential growth rate functions.

First, the potential carbon supply for ``G`` (``G_{supply}``) is computed by multiplying the growth yield by the available NSCs minus ``R_{a,m}`` (*The equation below is problematic. It is the same as the model code, but it double counts the foliar maintenance respiration as ``R_{a,m}^{fol}`` appears in both the ``NSC_a`` and ``R_{a,m}`` terms*):

.. math::
   :label: G_{supply}

   G_{supply} = Y_g \cdot (NSC_a - R_{a,m})

Then, we compute the demand for carbon by the potential growth rate of foliar, root, and wood as follows:

.. math::
   :label: G_{demand}

   \begin{split}
   G_{demand,fol} & = max \left( 0, \frac{LAI(t) \cdot P_{LCMA} - C_{fol}(t)}{\delta t} \right) \\
   G_{demand,roo} & = \frac{ \phi_{RL} \cdot (LAI(t) \cdot P_{LCMA})}{\delta t} \\ 
   G_{demand,woo} & = \frac{\phi_{WL} \cdot (LAI(t) \cdot P_{LCMA})}{\delta t}
   \end{split}

Where ``LAI`` equals the target LAI at time ``t`` computed from the leaf phenology model, ``P_{LCMA}`` is the leaf mass of carbon per area, and ``\phi_{RL}`` and ``\phi_{WL}`` are empirical parameters for the potential growth rate of root and wood per unit of target foliar carbon mass. The formulation for foliar growth demand is positive when the canopy is actively growing leaves (e.g., spring onset) and scales with the rate of change in LAI, and zero when the canopy is in senescence. The root and wood growth demands are only dependent on the target LAI, which allows for canopy phenology to mediate root and wood growth. This is an approximation of the pipe model theory, which requires the ratio between conducting sapwood area and the mass of foliage to remain constant over long timescales (Shinozaki et al., 1964). This formulation is useful as it allows the model to capture the commonly observed temporal offset between foliar growth and growth of root and wood (:ref:`Klein et al 2015 <Klein2015>`). The total demand for growth, ``G_{demand}``, equals the sum of ``G_{demand,fol}``, ``G_{demand,roo}``, and ``G_{demand,woo}``. Alternative hypotheses for the potential growth rate of foliar, root, and wood can be easily implemented in the future by modifying the equations.

The actual growth flux is then computed as the minimum of the supply and demand:

.. math::
   :label: G(t)

   G(t) = min \left( G_{supply}, G_{demand} \right)

If ``G_{demand}`` is greater than ``G_{supply}``, then the growth fluxes are linearly scaled down by an equal proportion to balance actual growth with supply. This assumes that the growth of foliar, root, and wood are prioritized equally.

Overall, this formulation for plant carbon allocation and respiration makes a substantial advance in DALEC. It provides a relatively simple yet comprehensive structure that is consistent with current understanding of the processes, with limited free parameters, and opens up opportunities for new insights into the temporal complexities of plant carbon dynamics.


.. rubric:: References

.. _Amthor2000:

Insert reference info for Amthor2000

.. _Cannell2000:

Insert reference info for Cannell2000

.. _Collalti2019:

Insert reference info for Collalti2019

.. _Collalti2020:

Insert reference info for Collalti2020

.. _Dietze2014:

Insert reference info for Dietze2014, https://doi.org/10.1146/annurev-arplant-050213-040054

.. _ElOmari2022:

Insert reference info for ElOmari2022

.. _Fatichi2014:

Insert reference info for Fatichi2014

.. _Franklin2012:

Insert reference info for Franklin2012

.. _Friend2019:

Insert reference info for Friend2019, https://doi.org/10.1007/s13595-019-0819-x

.. _Gifford2003:

Insert reference info for Gifford2003, https://doi.org/10.1071/FP02083

.. _Klein2015:

Insert reference info for Klein2015

.. _MartinezVilalta2016:

Insert reference info for MartinezVilalta2016

.. _Mccree1970:

Insert reference info for Mccree1970

.. _McDowell2008:

Insert reference info for McDowell2008

.. _Merganicova2019:

Insert reference info for Merganicova2019

.. _Sierra2022:

Insert reference info for Sierra2022

.. _Waring1998:

Insert reference info for Waring1998

