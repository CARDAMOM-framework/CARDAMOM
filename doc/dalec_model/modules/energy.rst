.. _soil_energy_exchanges:

Soil Energy Exchanges
=====================

The bulk soil energy inputs consist of (i) ground heat flux, (ii) inter-layer thermal exchanges, (iii) geological heat flux, and (iv) water-flux mediated energy gains and losses. The ground heat flux is modeled using the following formulation, where:

.. math::

   F_{ground\_heat} [PAUL REVIEW] = \kappa_{LY1} \frac{LY1 - LST}{0.5 d_{LY1}} 

Internal soil heat exchanges are simulated as

.. math::

   F_{exchange\{n,n+1\}} [PAUL REVIEW] = \sqrt{K_{LYn}K_{LYn+1}} \frac{T_{LYn+1} - T_{LYn}}{0.5(d_{LYn}+d_{LYn+1})} 

where :math:`K_{LYn}` represents the :math:`n^{th}` layer thermal conductivity.

Geological heat flux is assumed to be a constant input of 105 mW/m². Water-flux mediated energy exchanges are calculated based on the product of H₂O flux and temperature-based calculation of energy per unit volume H₂O. Infiltration-based inputs are assumed to be at 0°C temperature for snowmelt and max(T₂m, 0°C) for liquid precipitation inputs; subsurface runoff losses from layer :math:`n` are assumed to be at the corresponding layer temperature, :math:`T_{LYn}`.

