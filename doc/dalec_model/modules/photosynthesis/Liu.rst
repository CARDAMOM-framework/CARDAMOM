Modified Farquhar-von Caemmerer-Berry model
===========================================

.. toctree::
   :maxdepth: 2
   :caption: Contents:


To model photosynthesis and hence calculate GPP we apply a vairant of the Farquhar-von Caemmerer-Berry model adapted from [Liu:2021]_.

1. The absorbed photosynthetically active radiation (APAR) driving photosynthesis is calculated using the Beer-Lambert law [Campbell:1998]_ :

.. math::

    APAR = PAR \cdot (1 - \rho_\text{leaf}) \cdot (1 - e^{(-K \cdot LAI \cdot \Omega)})

* :math:`PAR` is the photosythnetically active radiaion
    :math:`PAR = \frac{SRAD}{2 \cdot E_{photon} \cdot N_A} \cdot 10^6`
        * :math:`PAR` is the photosynthetically active radiation (μmol photons m⁻² s⁻¹)
        * :math:`SRAD` is the incident shortwave radiation (W m⁻²)
        * :math:`E_{photon} = 2.0 \times 10^{-25} / (500.0 \times 10^{-9})` J (Planck constant times speed of light divided by light wavelength)
        * :math:`N_A = 6.02 \times 10^{23}` mol⁻¹ (Avogadro's constant)
* :math:`K` is the vegetation extinction coefficient [Campbell:1998]_
* :math:`LAI` is the leaf area index
* :math:`\Omega` is the clumping index [Braghiere:2019]_
* :math:`\rho_\text{leaf}` parameter describing the reflected portion of photosynthetically active radiation due to canopy reflectance


2. Net carbon assimilation is calculated based on C3 and C4 photosynthesis biochemistry to determine potential leaf-level photosynthesis (unstressed by water availability). This is expressed in terms of two potentially limiting rates:

a. Rubisco-limited rate (:math:`a_1`):

   .. math::

       a_1 = \begin{cases}
           V_\text{cmax} \cdot \frac{c_i - c_p}{c_i + K_c \cdot (1 + 209/K_o)} & \text{for C3} \\
           V_\text{cmax} & \text{for C4}
       \end{cases}

* :math:`V_\text{cmax}` (mol CO\ :sub:`2` m\ :sup:`-2` s\ :sup:`-1`) is the maximum rate of carboxylation.
    :math:`V_{cmax} = \frac{V_{cmax25} \cdot q_{10}^{0.1(T_C - 25)}}{\left(1 + e^{0.3(T_C - (T_{upp} - T_0))}\right) \left(1 + e^{0.3((T_{down} - T_0) - T_C)}\right)}`
        * :math:`V_{cmax25}` is the maximum rate of carboxylation at 25°C
        * :math:`q_{10}` parameter describing temperature sensitivity
        * :math:`T_C` is the air temperature in °C
        * :math:`T_{upp}` parameter describing the upper temperature limit for photosynthesis (in K)
        * :math:`T_{down}` parameter describing the lower temperature limit for photosynthesis (in K)
        * :math:`T_0` is the freezing point of water in Kelvin (273.15 K)

* :math:`c_i` is the intercellular CO\ :sub:`2` concentration
    :math:`c_i = c_{a} \cdot \left(1 - \frac{1}{1 + \frac{g_1}{\sqrt{VPD}}}\right)`
        * :math:`c_{a}` is the atmospheric CO\ :sub:`2` concentration
        * :math:`g_1` is the stomatal slope parameter
        * :math:`VPD` is the vapor pressure deficit

* :math:`c_p` is the CO\ :sub:`2` compensation point (the CO\ :sub:`2` concentration at which photosynthesis equals respiration)
    :math:`c_p = 36.9 + 1.18(T_C - 25) + 0.36(T_C - 25)^2`
        * :math:`T_C` is the air temperature in °C

* :math:`K_c` is the Michaelis-Menten concentration for CO\ :sub:`2`
    :math:`K_c = 300 \cdot e^{0.074(T_C - 25)}`
        * :math:`T_C` is the air temperature in °C

* :math:`K_o` is the Michaelis-Menten concentration for O\ :sub:`2`
    :math:`K_o = 300 \cdot e^{0.015(T_C - 25)}`
        * :math:`T_C` is the air temperature in °C


b. Light-limited rate (:math:`a_2`):

   .. math::

       a_2 = \begin{cases}
           J \cdot \frac{c_i - c_p}{4(c_i + 2c_p)} & \text{for C3} \\
           J & \text{for C4}
       \end{cases}

* :math:`J` is the rate of electron transport.
    :math:`J = \frac{0.3 \cdot PAR + V_{cmax} \cdot e - \sqrt{(0.3 \cdot PAR + V_{cmax} \cdot e)^2 - 1.08 \cdot PAR \cdot V_{cmax} \cdot e}}{1.8}`
        * :math:`PAR` is the photosynthetically active radiation
        * :math:`V_{cmax}` is the maximum rate of carboxylation
        * :math:`e` is the mathematical constant (approximately 2.71828)

3. The total net carbon assimilation (:math:`A_n`) is given by the weighted sum of C3 and C4 net carbon assimilation fractions:

.. math::

    A_n =  \min(a_1 \cdot \beta, a_2) - R_d)

* :math:`\beta` is the minimum of the moisture stress factor related to the mean soil moisture concentration in the root zone, and the temperature stress factor
* :math:`R_d` is the leaf dark respiration.

4. GPP, representing the total canopy photosynthesis, is calculated by integrating leaf-level photosynthesis over the entire canopy leaf area index:

.. math::

    GPP = A_n \cdot \frac{1 - \exp(-K \cdot LAI \cdot \Omega)}{K}
    
    
    
.. [Braghiere:2019] Braghiere, R.K., Quaife, T., Black, E., He, L. and Chen, J.M., 2019. Underestimation of global photosynthesis in Earth system models due to representation of vegetation structure. Global Biogeochemical Cycles, 33(11), pp.1358-1369. https://doi.org/10.1029/2018GB006135

.. [Campbell:1998] Campbell, G.S. and Norman, J.M., 2000. An introduction to environmental biophysics. Springer Science & Business Media.

.. [Farquhar:1980] Farquhar, G.D., von Caemmerer, S.V. and Berry, J.A., 1980. A biochemical model of photosynthetic CO 2 assimilation in leaves of C 3 species. planta, 149, pp.78-90. https://doi.org/10.1007/BF00386231

.. [Liu:2021] Liu, Y., Holtzman, N.M. and Konings, A.G., 2021. Global ecosystem-scale plant hydraulic traits retrieved using model–data fusion. Hydrology and Earth System Sciences, 25(5), pp.2399-2417. https://doi.org/10.5194/hess-25-2399-2021


