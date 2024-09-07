Soil hydrology
==============

.. toctree::
   :maxdepth: 2
   :caption: Contents:

Overview
--------

The soil hydrology module consists of four prognostic states, comprised of three soil layers and a snow layer. The snow layer is described in ``reference snow section``. The soil layers (:math:`W_{ly1}`, :math:`W_{ly2}`, and :math:`W_{ly3}`) are characterized functionally distinct entities:

* :math:`W_{ly1}` represents near-surface moisture for bare soil evaporation and plant water uptake. The energy state of this layer (described in ``reference energy balance section``) is used to determine the temperature of heterotrophic respiration (see section ``reference heterotrophic section``).
* :math:`W_{ly2}` represents an additional reservoir accessible by plant roots for water uptake, but sits below :math:`W_{ly1}` and therefore does not experience direct evaporation.
* :math:`W_{ly3}` represents water that is below :math:`W_{ly2}` and is innaccessible to plant roots. 

Functionally, water enters :math:`W_{ly1}` through infiltration, water is exchanged between layers as a function of hydraulic gradients, and is removed via (i) bare soil evaporation from :math:`W_{ly1}`, (ii) plant transpiration from :math:`W_{ly1}` and :math:`W_{ly2}`, and (iii) subsurface runoff each soil layer.

Prognostic and diagnostic water states
--------------------------------------

Each of the :math:`n` soil water layers (:math:`W_{lyn}`) consists of a prognistic water mass in units of kgH\ :sub:`2`\ O m\ :sup:`-2`, which is assumed to have a density of 1000 kg m\ :sup:`-3` and therefore simplified as being in units of mmH\ :sub:`2`\ O. At each time step :math:`t`, a diagnostic soil moisture state (:math:`\theta_{lyn}`, in units of m\ :sup:`3` m\ :sup:`-3`) can be derived as 

.. math::
   \theta_{lyn}(t) = W_{lyn}(t)(10^3p_{lyn}z_{lyn})^{-1}

where :math:`z_{lyn}` is the depth of layer :math:`n` in m, and :math:`p_{lyn}` is the porosity of layer :math:`n` in m\ :sup:`3` m\ :sup:`-3`.

Each layer has a diagnostic matric water potential (:math:`\Psi_{lyn}`, in MPa) that is derived as 

.. math::
   \psi_{lyn}(t) = \psi_{p}\theta_{lyn}(t)^{-b}

where :math:`\psi_{p}` is set to -1.17 x 10\ :sup:`-3` MPa (see [Massoud:2022]_ ``although we don't use the exact same value``) and :math:`b` is a unitless retention curve parameter.

Each layer also has a diagnostic temperature (:math:`T_{lyn}`, in K) and liquid fraction (:math:`T_{lyn}`, in kg kg\ :sup:`-1`), the derivation of which is described in section ``reference energy balance section``.


.. Soil water exchanges
.. --------------------

.. Soil water exchanges consist of (i) infiltration into :math:`W_{ly1}` (Qx\ :sub:`inf`), (ii) evaporation from  :math:`W_{ly1}` (:math:`Ev`), (iii) transpiration from :math:`W_{ly1}` (:math:`Tr1`) and :math:`W_{ly2}` (:math:`Tr2`), (iv) exchanges between :math:`W_{ly1}` and :math:`W_{ly2}` (:math:`Qx_{ly12}`) and between :math:`W_{ly2}` and :math:`W_{ly3}` (:math:`Qx_{ly23}`), and (v) subsurface runoff (:math:`Q_{ly1}`, :math:`Q_{ly2}`, and :math:`Q_{ly3}`). When soil water liquid fraction (:math:`LF_{ly1}`, :math:`LF_{ly2}`, and :math:`LF_{ly3}`) is zero, the flux calculations are scaled to zero; when soil water liquid fraction of layer :math:`n` (:math:`LF_{lyn}`) is greater than zero, the fluxes are linearly scaled by :math:`LF_{lyn}`. 

Infiltration into surface
-------------------------

Infiltration of liquid precipitation and snowmelt into the topmost soil layer (:math:`Qx_{inf}`, in mm d\ :sup:`-1`) is calculated for each timestep :math:`t` as

.. math::
   Qx_{inf}(t) = I_{max}(1 - \exp(-\frac{P_{rain}(t) + Melt(t)}{I_{max}}))

where :math:`I_{max}` is the maximum infiltration parameter, :math:`P_{rain}` is the liquid precipitation (:math:`P_{tot} - P_{snow}`), and :math:`Melt` is snowmelt (defined in equation ``reference snowmelt equation``), all in mm d\ :sup:`-1`. This formulation ensures :math:`\approx 100\%` infiltration when :math:`P_{rain} + Melt << I_{max}`, and inflitration is :math:`\approx I_{max}` :math:`P_{rain} + Melt >> I_{max}`. Non-infilrated surface water (i.e., :math:`P_{rain} + Melt - Qx_{inf}`) is removed as surface runoff (:math:`Q_{surf}`).

Inter-layer water transfers
---------------------------

Interlayer water transfers between layers :math:`n` and : :math:`n+1` (:math:`Qx_{lyn,n+1}`) are derived based on the difference in soil water potential (:math:`\Psi_{lyn}` and :math:`\Psi_{lyn+1}`). The equation used to derive the exchange in mm d:sup:`-1` at each time step :math:`t` is:

.. math::
   Qx_{lyn,n+1}(t) = \rho_l \sqrt{k_{lyn}(t)k_{lyn+1}(t)} \left[\frac{10^{-6}(\Psi_{lyn}(t) - \Psi_{lyn+1}(t))}{\rho_l g 0.5(z_{lyn}+z_{lyn+1})} + 1 \right]

where :math:`\rho_l` is the density of liquid water (1000 kg m\ :sup:`-3`), :math:`g` is the gravitational acceleration on the surface of Earth (9.8 m s\ :sup:`-2`), and  :math:`k_{lyn}` is the hydraulic conductivity of layer :math:`n` in m s\ :sup:`-1` and is derived as

.. math::
   k_{lyn}(t) = k_0\theta_{lyn}(t)^{2b+3}

where :math:`k_0` is the saturated hydraulic conductivity parameter (in m s\ :sup:`-1`).

``Implementation notes?? We actually calculate potential transfer and check that against the available water in the source layer and available space in the destination layer``

Subsurface runoff
------------------

Subsurface runoff of layer :math:`n` (:math:`Q_{lyn}`, in mm d\ :sup:`-1`) is derived using 

.. \textbf{Subsurface runoff:} subsurface runoff is modelled using the formulation described in \citet{Massoud:2021vz}.  We pick an arbitrarily small value to represent $\Psi_{porosity}$ (-1.17 x 10$^{-3}$ MPa), to avoid the degenerate model behavior for $\Psi_{porosity} = 0$. Excess moisture between \Pfieldcap and $\Psi_{porosity}$ is removed as described in \citet{Massoud:2021vz}.


.. \textbf{Evapotranspiration losses:}
.. Soil evaporation and transpiration fluxes are calculated as part of the soil-plant-atmosphere biophysics module (see section 3.3). Evaporation is only removed from LY1. Transpiration is removed from LY1 and LY2; we assume  an exponential root density profile to represent relative transpiratory losses from LY1 and LY2 (see eq. XX in supplement); the \Prootfrac parameter effectively defines the relative weight of transpiration dependence on LY1 and LY2 states; in brief, parameter \Prootfrac = 1 represents a depth-wise homogeneous root density profile, and \Prootfrac << 1 represents a rapidly exponentially decaying root profile with effectively zero influence of LY2 soil moisture on transpiration. The relationship between soil moisture, soil water potential and transpiration losses from LY1 and LY2 is further described in section \ref{subsub:water_stress}


.. [Massoud:2022] Massoud, E. C., Bloom, A. A., Longo, M., Reager, J. T., Levine, P. A., and Worden, J. R., 2022. Information content of soil hydrology in a west Amazon watershed as informed by GRACE. Hydrology and Earth System Sciences, 26(5), pp.1407–1423. https://doi.org/10.5194/hess-26-1407-2022
