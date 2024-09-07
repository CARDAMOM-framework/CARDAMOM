Soil hydrology
==============

.. toctree::
   :maxdepth: 2
   :caption: Contents:

Overview
--------

The soil physics module consists of four prognostic states, comprised of three soil layers and a snow layer. The soil layers (:math:`W_{ly1}`, :math:`W_{ly2}`, and :math:`W_{ly3}`), are defined with depths z1, z2 and z3. We characterize the three layers as functionally distinct entities in which :math:`W_{ly1}` represents near-surface moisture for bare soil evaporation and plant water uptake, and energy states to determine the temperature of heterotrophic respiration, :math:`W_{ly2}` represent an additional plant water uptake reservoir, and :math:`W_{ly3}` represents water and energy states innaccessible to plant water. Functionally, infiltration--including snowmelt--enter :math:`W_{ly1}`. Moisture and energy exchanges between :math:`W_{ly1}`, :math:`W_{ly2}` and :math:`W_{ly3}` are derived through thermal and hydraulic gradients. For each layer, we define the water balance differential equation as :math:`d(W_{n})/dt`, where :math:`W_n` represents the moisture state of layer :math:`n` (in mm H\ :sub:`2`\ O), and the energy balance as :math:`d(E_{n})/dt`, where :math:`E_n` represents the bulk energy content per unit area (in J m\ :sup:`-2`). 

Soil water exchanges
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

Interlayer water transfers between layers :math:`n` and : :math:`n+1` (:math:`Qx_{lyn,n+1}`) are derived based on the difference in soil water potential (:math:`\Psi_{lyn}` and :math:`\Psi_{lyn+1}`) between :math:`W_{lyn}` and : :math:`W_{lyn+1}`. The equation used to derive the exchange in mm d:sup:`-1` at each time step :math:`t` is:

.. math::
   Qx_{lyn,n+1}(t) = \rho_l \sqrt{k_{lyn}(t)k_{lyn+1}(t)} \left[\frac{10^{-6}(\Psi_{lyn}(t) - \Psi_{lyn+1}(t))}{\rho_l g 0.5(z_{lyn}+z_{lyn+1})} + 1 \right]

where :math:`\rho_l` is the density of liquid water (1000 kg m\ :sup:`-3`), :math:`k_{lyn}` is the hydraulic conductivity of layer :math:`n` in m s\ :sup:`-1` (see below for derivation), :math:`\Psi_{lyn}` is the matric potential of layer :math:`n` in MPa, :math:`g` is the gravitational acceleration on the surface of Earth (9.8 m s\ :sup:`-2`), and :math:`z_{lyn}` is the depth of layer :math:`n` in m.

:math:`k_{lyn}` is derived based on the volumetric soil moisture of layer :math:`n` (:math:`\theta_{lyn}`) as :math:`k_{lyn}(t) = k_0\theta_{lyn}(t)^{2b+3}`, where :math:`k_0` is the saturated hydraulic conductivity parameter (in m s\ :sup:`-1`) and :math:`b` is the retention parameter, while :math:`\theta_{lyn}` is derived as :math:`\theta_{lyn}(t) = W_{lyn}(t)(1000p_{lyn}z_{lyn})^{-1}` where :math:p_{lyn} is the porosity of layer :math:`n`.

``Implementation notes?? We actually calculate potential transfer and check that against the available water in the source layer and available space in the destination layer``

Subsurface runoff
------------------

Subsurface runoff of layer :math:`n` (:math:`Q_{lyn}`, in mm d\ :sup:`-1`) is derived using 

.. \textbf{Subsurface runoff:} subsurface runoff is modelled using the formulation described in \citet{Massoud:2021vz}.  We pick an arbitrarily small value to represent $\Psi_{porosity}$ (-1.17 x 10$^{-3}$ MPa), to avoid the degenerate model behavior for $\Psi_{porosity} = 0$. Excess moisture between \Pfieldcap and $\Psi_{porosity}$ is removed as described in \citet{Massoud:2021vz}.


.. \textbf{Evapotranspiration losses:}
.. Soil evaporation and transpiration fluxes are calculated as part of the soil-plant-atmosphere biophysics module (see section 3.3). Evaporation is only removed from LY1. Transpiration is removed from LY1 and LY2; we assume  an exponential root density profile to represent relative transpiratory losses from LY1 and LY2 (see eq. XX in supplement); the \Prootfrac parameter effectively defines the relative weight of transpiration dependence on LY1 and LY2 states; in brief, parameter \Prootfrac = 1 represents a depth-wise homogeneous root density profile, and \Prootfrac << 1 represents a rapidly exponentially decaying root profile with effectively zero influence of LY2 soil moisture on transpiration. The relationship between soil moisture, soil water potential and transpiration losses from LY1 and LY2 is further described in section \ref{subsub:water_stress}

