.. _sub:heterotrophic_respiration:

Heterotrophic Respiration
=========================

In the DALEC heterotrophic respiration module, CO\ :sub:`2` is respired both anaerobically and aerobically, while CH\ :sub:`4` is only respired anaerobically. For each necromass pool (\ :math:`SClit`, \ :math:`SCcwd`, and \ :math:`SCsom`), aerobic (\ :math:`FCrhaelit`, \ :math:`FCrhaecwd`, \ :math:`FCrhaesom`) and anaerobic respiration (\ :math:`FCrhanlit`, \ :math:`FCrhancwd`, \ :math:`FCrhansom`) are calculated as:

.. math::

    FCrhaelit(t) = SClit(t) Plit f_{V_{ae}}(t)f_{T}(t)f_{W}(t)(1- Plitxsom)

.. math::

    FCrhaecwd(t) = SClit(t) Pcwd f_{V_{ae}}(t)f_{T}(t)f_{W}(t)(1- Pcwdxsom)

.. math::

    FCrhaesom(t) = SCsom(t) Psom f_{V_{ae}}(t)f_{T}(t)f_{W}(t)

.. math::

    FCrhanlit(t) = SClit(t) Plit (1 - f_{V_{ae}}(t))f_{T}(t)Pfwc (1- Plitxsom)

.. math::

    FCrhancwd(t) = SClit(t) Pcwd (1 - f_{V_{ae}}(t))f_{T}(t)Pfwc (1- Pcwdxsom)

.. math::

    FCrhansom(t) = SCsom(t) Psom (1 - f_{V_{ae}}(t))f_{T}(t)Pfwc

where \ :math:`Plit`, \ :math:`Pcwd`, and \ :math:`Psom` are the basal turnover rates of \ :math:`SClit`, \ :math:`SCcwd`, and \ :math:`SCsom` respectively.

:math:`f_{V_{ae}}` is the aerobic fraction of respiration, calculated as:

.. math::

    f_{V_{ae}} = (1 - \SMa)^{\PSfv}

where \ :math:`\SMa` is the fractional soil moisture in layer 1, and \ :math:`\PSfv` is an optimized regression scalar that determines the shape of the soil moisture response curve.

:math:`f_{Wc}` is the soil moisture scalar when the soil is saturated; The anaerobic soil is saturated and thus :math:`f_{Wc}` is given a constant value of 1 in equation (2); :math:`f_{V_{ae}}` is the aerobic fraction of the vertical soil column; :math:`1 - f_{V_{ae}}` is the anaerobic fraction of the soil. Equations (4) to (6) describe how :math:`f_{W}` and :math:`f_{V_{ae}}` are calculated.

DALEC resolves site-specific data-constrained parameters to characterize the shape of the soil moisture-respiration curve, where the soil moisture scalar (:math:`f_{W}`) for aerobic respiration is based on the moisture of the aerobic soil (:math:`\theta_{ae}`). By definition, the fractional soil moisture in layer 1 (:math:`\SMa`) equals:

.. math::

    \SMa = \theta_{ae}f_{V_{ae}} + \theta_{an}(1 - f_{V_{ae}})

where \ :math:`\theta_{an}` is the moisture of the anaerobic soil, which always equals 1; the volumetric fraction of anaerobic soil is (:math:`1 - f_{V_{ae}}`); :math:`\theta_{ae}` is then derived using the last two equations:

.. math::

    \theta_{ae} = ((\SMa-1)/f_{V_{ae}} +1)

According to Exbrayat et al. (2013), we use a segmented function to allow :math:`f_{W}` to reach 1 at optimum soil moisture (:math:`\theta_{s_{opt}}`), and then decrease to :math:`f_{W_{c}}` (:math:`f_{W_{c}} \leq 1`) to represent the high soil moisture suppression on aerobic respiration. Both :math:`\theta_{s_{opt}}` and :math:`f_{W_{c}}` are set to be optimized by the data.

.. math::

    \text{(placeholder for segmented function)}

:math:`f_T` is a temperature scaling factor, defined as:

.. math::

    f_T(t) = \PQtenrhcotwo^{\frac{\STa(t) - 25^\circ\mathrm{C}}{10}}

where \ :math:`\PQtenrhcotwo` is the factor by which respiration rate increases with a 10°C increase in temperature (relative to a reference temperature of 25°C) and \ :math:`\STa` is the temperature of soil layer 1.

The heterotrophic respiration terms in the form of \({CO}_{2}\) (\({Rh}_{CO_{2}}\)) and \({CH}_{4}\) (\({Rh}_{CH_{4}}\)) are then calculated as:

.. math::

    {Rh}_{CO_{2}} = {Rh}_{ae}*1 + {Rh}_{an}*\left( 1 - f_{CH_{4}} \right)

.. math::

    {Rh}_{CH_{4}} = {Rh}_{ae}*0 + {Rh}_{an}*f_{CH_{4}}

where :math:`f_{CH_{4}}` is the fraction of \({CH}_{4}\) in anaerobic respiration:

.. math::

    f_{CH_{4}} = r_{CH_{4}}*Q_{10_{CH_{4}}}^{\frac{T - T_{mean}}{10}}

where :math:`r_{CH_{4}}` is the potential ratio of anaerobically mineralized C released as CH\ :sub:`4`; :math:`Q_{10_{CH_{4}}}` is the factor by which \({CH}_{4}\) production rate increases with a 10°C increase in temperature, on top of the temperature sensitivity encountered in equations 1 and 2 (:math:`f_T`). The reason we put a :math:`Q_{10_{CH_{4}}}` on top of the general respiration temperature sensitivity term here is that studies have found higher temperature sensitivity in methane production than CO\ :sub:`2` respiration across microbial to ecosystem scales (Yvon-Durocher et al. 2014). T is the mean air temperature of the current time step, :math:`T_{mean}` is the multi-year mean air temperature at the region.

