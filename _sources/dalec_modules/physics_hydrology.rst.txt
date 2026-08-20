Soil physics and hydrology modules
==================================

The soil physics module consists of four prognostic states, comprised of three soil layers and a snow layer. The soil layers (:math:`W_{ly1}`, :math:`W_{ly2}`, and :math:`W_{ly3}`), are defined with depths z1, z2 and z3. We characterize the three layers as functionally distinct entities in which :math:`W_{ly1}` represents near-surface moisture for bare soil evaporation and plant water uptake, and energy states to determine the temperature of heterotrophic respiration, :math:`W_{ly2}` represent an additional plant water uptake reservoir, and :math:`W_{ly3}` represents water and energy states innaccessible to plant water. Functionally, infiltration--including snowmelt--enter :math:`W_{ly1}`. Moisture and energy exchanges between :math:`W_{ly1}`, :math:`W_{ly2}` and :math:`W_{ly3}` are derived through thermal and hydraulic gradients. For each layer, we define the water balance differential equation as :math:`d(W_{n})/dt`, where :math:`W_n` represents the moisture state of layer :math:`n` (in mm H\ :sub:`2`\ O), and the energy balance as :math:`d(E_{n})/dt`, where :math:`E_n` represents the bulk energy content per unit area (in J m\ :sup:`-2`). 

.. toctree::
   :maxdepth: 2
   :caption: Hydrology:

   physics_hydrology/soil_hydrology
   physics_hydrology/snow

.. toctree::
   :maxdepth: 2
   :caption: Energy balance:

   physics_hydrology/energy
