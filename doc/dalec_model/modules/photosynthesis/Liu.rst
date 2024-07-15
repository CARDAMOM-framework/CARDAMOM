Modified Farquhar-von Caemmerer-Berry model
===========================================

.. toctree::
   :maxdepth: 2
   :caption: Contents:


To model photosynthesis and hence calculate GPP we apply a vairant of the Farquhar-von Caemmerer-Berry model adapted from [Liu:2021]_.

1. The absorbed photosynthetically active radiation (APAR) driving photosynthesis is calculated using the Beer-Lambert law:

.. math::

    APAR = SW \cdot (1 - \rho_\text{leaf}) \cdot (1 - \exp(-K \cdot LAI \cdot \Omega))

where 

* :math:`SW` is the incident shortwave radiation
* :math:`\rho_\text{leaf}` is a parameter describing the reflected portion of photosynthetically active radiation due to canopy reflectance
* :math:`K` is the vegetation extinction coefficient [Campbell:1998]_
* :math:`LAI` is the leaf area index
* :math:`\Omega` is the clumping index [Braghiere:2019]_

2. Net carbon assimilation is calculated based on C3 and C4 photosynthesis biochemistry to determine potential leaf-level photosynthesis (unstressed by water availability). This is expressed in terms of two potentially limiting rates:

a. Rubisco-limited rate (:math:`a_1`):

   .. math::

       a_1 = \begin{cases}
           V_\text{cmax} \cdot \frac{c_i - c_p}{c_i + K_c \cdot (1 + 209/K_o)} & \text{for C3} \\
           V_\text{cmax} & \text{for C4}
       \end{cases}

   where 
* :math:`V_\text{cmax}` (mol CO\ :sub:`2` m\ :sup:`-2` s\ :sup:`-1`) is the maximum rate of carboxylation.

b. Light-limited rate (:math:`a_2`):

   .. math::

       a_2 = \begin{cases}
           J \cdot \frac{c_i - c_p}{4(c_i + 2c_p)} & \text{for C3} \\
           J & \text{for C4}
       \end{cases}

   where :math:`J` is the rate of electron transport.

The total net carbon assimilation (:math:`A_n`) is given by the weighted sum of C3 and C4 net carbon assimilation fractions:

.. math::

    A_n = \max(0, \min(a_1 \cdot \beta, a_2) - R_d)

where :math:`\beta` is the moisture stress factor related to the mean soil moisture concentration in the root zone, and :math:`R_d` is the leaf dark respiration, calculated as :math:`0.015 \cdot V_\text{cmax} \cdot \beta`.

3. GPP, representing the total canopy photosynthesis, is calculated by integrating leaf-level photosynthesis over the entire canopy leaf area index:

.. math::

    GPP = A_n \cdot \frac{1 - \exp(-K \cdot LAI \cdot \Omega)}{K}

.. [Liu:2021] Liu, Y., Holtzman, N.M. and Konings, A.G., 2021. Global ecosystem-scale plant hydraulic traits retrieved using model–data fusion. Hydrology and Earth System Sciences, 25(5), pp.2399-2417. https://doi.org/10.5194/hess-25-2399-2021
.. [Campbell:1998] Campbell, G.S. and Norman, J.M., 2000. An introduction to environmental biophysics. Springer Science & Business Media.
.. [Braghiere:2019] Braghiere, R.K., Quaife, T., Black, E., He, L. and Chen, J.M., 2019. Underestimation of global photosynthesis in Earth system models due to representation of vegetation structure. Global Biogeochemical Cycles, 33(11), pp.1358-1369. https://doi.org/10.1029/2018GB006135
