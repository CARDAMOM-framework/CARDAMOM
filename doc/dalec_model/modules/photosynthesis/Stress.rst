GPP Stress (Beta Functions)
===========================================
.. toctree::
   :maxdepth: 2
   :caption: Contents:

Plant Water Potential GPP limitations
-----------------------------

We represent water stress constraints on gross primary productivity and stomatal conductance based on water potential along the soil-plant continuum. For simplicity of representation, we assume water potential is consistent between soil and leaf, as opposed to quantifying a distinct leaf water potential. We compute the water stress factor using a sigmoid function across each soil layer accessible to the plant, subsequently normalized by the depth and rooting fraction of the two soil layers:

.. math::

    \beta_1(\Psi_{ly1}) = \frac{1}{1 + e^{-\beta_{plgr}\left(\frac{\Psi_{ly1}}{\Psi_{50}} + 1\right)}}

.. math::

    \beta_2(\Psi_{ly2}) = \frac{1}{1 + e^{-\beta_{plgr}\left(\frac{\Psi_{ly2}}{\Psi_{50}} + 1\right)}}

.. math::

    \beta = \frac{\beta_1 \cdot z_{ly1} + \beta_2 \cdot z_{ly2} \cdot f_{root}}{z_{ly1} + z_{ly2} \cdot f_{root}}

* :math:`\Psi_{ly1}` water potential (MPa) of the soil-plant continuum from the first soil layer 
* :math:`\Psi_{ly2}` water potential (MPa) of the soil-plant continuum from the second soil layer 
* :math:`\beta_{plgr}` parameter representing the growth rate of the sigmoid function
* :math:`\Psi_{50}` parameter represnting the water potential (MPa) at which the soil-plant continuum exhibits 50% hydraulic conductivity.
* :math:`z_{ly1}` parameter represnting the depth of the layer one soil water pool (m)
* :math:`z_{ly2}` parameter represnting the depths of layer two soil water pool (m)
* :math:`f_{root}` is the ratio of root density in soil layer two to that of root density in soil layer one.

Sigmoidal functions have been used in other land surface models to represent water stress. For example, the Community Land Model version 5 (CLM5), as discussed by Kennedy et al. (2019), uses a sigmoidal function to characterize water stress as a function of leaf water potential. Additionally, Anderegg et al. (2017) highlighted that sigmoidal-like functions, along with Weibull and negative exponential functions, effectively depict the partial dependence of stomatal conductance on leaf water potential. Using plant water potential further better represents hydraulic restrictions on plant transpiration (Manzoni et al., 2013).


Cold weather GPP limitations
-----------------------------

In order to account for cold-weather limitations to gross primary production (GPP), we adopted the formulation of Stettz et al. (2022), in which :math:`g(t)`, a scaling factor on GPP is calculated as followig:

.. math::

    g(t) = 
    \begin{cases}
    0, & \text{if}\ T2M_{MIN} > T_{min_{min}}\\
    \frac{T2M_{MIN} - T_{min_{min}}}{T_{min_{max}} - T_{min_{min}}}, & \text{if}\ T_{min_{min}} < T2M_{MIN} < T_{min_{max}} \\
    1, & \text{if}\ T2M_{MIN} > T_{min_{max}}
    \end{cases}

* :math:`T2M_{MIN}` mean of daily minimum 2-meter air temperature over time step (K)   
* :math:`T_{min_{min}}` parameter represnting minimum cold temperature scaling factor (K)
* :math:`T_{min_{max}}` parameter represnting maximum cold temperature scaling factor (K)


