Evapotranspiration
===========================================

.. toctree::
   :maxdepth: 2
   :caption: Contents:


Transpiration and Surface Evaporation are calculated based off a modified penomon monteith equation.

Transpiration
-----------------------------

.. math::

    Tr_{ly1} + Tr_{ly2} = \frac{\Delta \cdot (R_{nl}) + \rho_a \cdot c_p \cdot VPD \cdot g_a}{\Delta + \gamma \cdot \left(1 + g_a \cdot \left(\frac{1}{g_a} + \frac{1}{g_s}\right)\right)} \cdot \frac{24}{\lambda \cdot 60 \cdot 60}

* :math:`\Delta` is the rate of change of saturated vapor pressure with air temperature:
    :math:`\Delta = 0.04145 \cdot e^{0.06088 \cdot T_C}`
        * :math:`T_C` is the air temperature (°C)

* :math:`R_{nl}` is the net fraction of radiation absorbed by leaves
    :math:`R_{nl} = SRAD \cdot (1 - 0.5 \cdot (\rho_{PAR} + \rho_{NIR})) \cdot \left(1 - e^{-K \cdot LAI \cdot \Omega}\right)`
        * :math:`SRAD` is the incident shortwave radiation (W m⁻²)
        * :math:`\rho_{PAR}` parameter describing reflectance for photosynthetically active radiation
        * :math:`\rho_{NIR}` parameter describing reflectance for near infrared radiation
        * :math:`K` is the vegetation extinction coefficient 
        * :math:`LAI` is the leaf area index
        * :math:`\Omega` is the clumping index 

* :math:`g_s` is the stomatal conductance calculated using the Medlyn stomatal conducatance model 
    :math:`g_s = 1.6\cdot 0.02405 \cdot LAI \cdot \frac{A_n}{c_a-c_i}`
        * :math:`LAI` is the leaf area index (:math:`m^2/m^2`) 
        * :math:`c_{a}` is the atmospheric CO\ :sub:`2` concentration
        * :math:`c_i` is the intercellular CO\ :sub:`2` concentration
            :math:`c_i = c_{a} \cdot \left(1 - \frac{1}{1 + \frac{g_1}{\sqrt{VPD}}}\right)`
                * :math:`g_1` is the stomatal slope parameter
                * :math:`VPD` is the vapor pressure deficit 


* :math:`g_a` parameter describing aerodynamic conductance (m s\ :sup:`-1`)
* :math:`VPD` is the vapor pressure deficit 
* :math:`\rho_a` is the air density: 1.225 (:math:`kg/m^3`)
* :math:`c_p` is the specific heat capacity of air: 1000 (J/kg/K)
* :math:`\lambda` is the latent heat of vaporization: :math:`2.26 \cdot 10^6` (J/kg) 
* :math:`\gamma` is the psychrometric constant :math:`\frac{100*1005}{\lambda*0.622}`




Evaporation
-----------------------------
.. math::

    Ev = \min\left(\frac{P_{tot}}{Ev_{max}}, 1.\right) \cdot \frac{1.26 \cdot \left(1 - 0.5 \cdot (\rho_{PAR} + \rho_{NIR})\right) \cdot SRAD \cdot e^{-K \cdot LAI \cdot \Omega}}{\lambda + \gamma} \cdot \frac{24}{\lambda \cdot 60 \cdot 60}

* :math:`P_{tot}` is the total precipitation 
* :math:`Ev_{max}` parmater for maximum pan evaporation
* :math:`SRAD` is the incident shortwave radiation (W m⁻²)
* :math:`\rho_{PAR}` parameter describing reflectance for photosynthetically active radiation
* :math:`\rho_{NIR}` parameter describing reflectance for near infrared radiation
* :math:`K` is the vegetation extinction coefficient 
* :math:`LAI` is the leaf area index
* :math:`\Omega` is the clumping index 
* :math:`\lambda` is the latent heat of vaporization: :math:`2.26 \cdot 10^6` (J/kg) 
* :math:`\gamma` is the psychrometric constant :math:`\frac{100*1005}{\lambda*0.622}`
