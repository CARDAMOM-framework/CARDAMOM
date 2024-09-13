Knorr phenology module
======================

.. toctree::
   :maxdepth: 2
   :caption: Contents:

Description and equations for Knorr module

Overview
--------

The leaf area index (LAI) phenology model is based on the scheme developed by :ref:`Knorr et al (2010) <Knorr2010>` with updates by :ref:`Norton et al (2023) <Norton2023>`. The :ref:`Knorr et al (2010) <Knorr2010>` model brings together two concepts. First, the transition of LAI between dormant, active, and senescent states is controlled by three potentially limiting environmental constraints: Temperature, photoperiod, and water availability. Second, spatial variability in growth triggers within a population of plants results in smooth, differentiable functions that describe the transition between dormant, active, and senescent states. This provides a more realistic representation of LAI dynamics, as opposed to unrealistic and impractical step functions. :ref:`Norton et al (2023) <Norton2023>` described the coupling of this LAI phenology model to the carbon and water balance in DALEC and implemented it in CARDAMOM, demonstrating its skill in capturing LAI and carbon cycle dynamics across diverse ecosystem types. We note that in :ref:`Norton et al (2023) <Norton2023>` the LAI phenology sub-model used evapotranspiration as an input to compute the water-limited LAI, considering evaporation and transpiration were not modeled separately. Here, evaporation and transpiration are modeled separately, therefore transpiration is used as the input which is consistent with the original :ref:`Knorr et al (2010) <Knorr2010>` formulation and adds realism to this version of CARDAMOM. 

Background
----------

The LAI phenology model was originally developed by describing a generic differential equation in time for the LAI of individual plants, which is then integrated in space to represent LAI dynamics of a population of plants. The time evolution of LAI for an individual plants is commonly described using triggers for growth and senescence (e.g. a growing degree day threshold), which are represented by conditional (step) functions. The :ref:`Knorr et al (2010) <Knorr2010>` model makes the useful development by assuming the triggers for a population of plants can be represented by a Gaussian probability density function. As described below, this enables the time evolution of LAI for the population to be smooth and differentiable. 

In the original formulation, which is also implemented into DALEC, there are two triggers considered: temperature (:math:`T`) and day length (:math:`t_d`). For an individual plant (represented by the tilde), the rate of change in LAI over time can be represented by:

.. math::
   :label: eq_dLAIdt_individual

   \frac{d\tilde{LAI}(t)}{dt} = 
   \begin{cases}
   f_1, & \text{if } T \geq \tilde{T_{\phi}} \text{ and } t_d \geq \tilde{t_c}, \\
   f_2, & \text{else}
   \end{cases}

Where :math:`f_1` represents an arbitrary function to describe LAI when the thresholds are exceeded (during the growing season) and :math:`f_2` represents an abritrary function to describe LAI when the thresholds have not been exceeded (during senescence), :math:`\tilde{T_{\phi}}` is the threshold for temperature and :math:`\tilde{t_c}` is the threshold for day length. For a population of plants, the rate of change in LAI over time can be represented by:

.. math::
   :label: eq_dLAIdt_pop

   \frac{dLAI(t)}{dt} = f_1 \int_{-\infty}^{T} \int_{-\infty}^{t_d}  p(\tilde{T_{\phi}}) q(\tilde{t_c}) \, d\tilde{T_{\phi}} \, d\tilde{t_c} + f_2 \left( 1 - \int_{-\infty}^{T} \int_{-\infty}^{t_d}  p(\tilde{T_{\phi}}) q(\tilde{t_c}) \, d\tilde{T_{\phi}} \, d\tilde{t_c} \right)

Where :math:`p` and :math:`q` are the spatial probability density functions for the two threshold variables, :math:`\tilde{T_{\phi}}` and :math:`\tilde{t_c}`, respectively. The probability density functions for the threshold variables are described by their respective means, :math:`T_{\phi}` and :math:`t_c`, and standard deviations,  :math:`T_r` and :math:`t_r`. Thus, :math:`T_r` and :math:`t_r` represent the variability of the threshold variables temperature and day length within the population. Because the warmest period of the year is almost always after the period of maximum day length, the dual condition above typically results in leaf onset being triggered by temperature and leaf senescence by day length (ignoring water and carbon-supply limitations). 

The above equation can be simplified by representing each integral with a cumulative normal distribution, :math:`\Phi`, as follows:

.. math::
   :label: eq_f_cdf

   f = \int_{-\infty}^{T} p(\tilde{T_{\phi}}) \, d\tilde{T_{\phi}} \, \int_{-\infty}^{t_d} q(\tilde{t_c}) \, d\tilde{t_c} = \Phi \left( \frac{T - T_{\phi}}{T_r} \right) \Phi \left( \frac{t_d - t_c}{t_r} \right)

Where :math:`T` is a phenology-determining temperature, described below. With the above equation, :math:`f` represents the fraction of plants within the population that are actively growing or maintaining leaves. Thus, equation :eq:`eq_dLAIdt_pop` simplifies to:

.. math::
   :label: eq_dLAIdt_pop2

   \frac{dLAI(t)}{dt} = f_1 f + f_2 (1 - f)

Growth and Senescence
---------------------

The LAI growth function, :math:`f_1`, and the LAI senescence function, :math:`f_2` must be defined. Following :ref:`Knorr et al (2010) <Knorr2010>`, LAI growth is described linearly as a function of the difference between the current LAI and a maximum potential LAI:

.. math::
   :label: eq_f1

   f_1 = \zeta (LAI_{max}(t) - LAI(t))

Where :math:`\zeta` is the linear growth rate constant. Note that :math:`LAI_{max}` is also a function of time as it evolves as a function of water availability (described below). Following :ref:`Knorr et al (2010) <Knorr2010>`, senescence is described by a function of leaf longevity:

.. math::
   :label: eq_f2

   f_2 = LAI(t)/\tau_L

Where :math:`\tau_L` is the mean leaf longevity of the population. 

Water Limitation 
----------------

To incorporate water availability constraints on LAI dynamics, a water-limited LAI is defined using the balance of water availability (soil moisture) and water demand (loss via evaporation/transpiration) as follows:

.. math::
   :label: eq_LAI_W

   LAI_W(t) = \frac{W \cdot LAI(t)}{E \cdot \tau_W}

Where :math:`W` is the plant-available water pool (NOTE: Paul to confirm which hydrology pool this is defined as), :math:`E` is the leaf-area specific evapotranspiration rate (NOTE: Paul to confirm whether 1100 uses ET or just T), and :math:`tau_W` is a parameter describing the expected length of water deficit periods that are tolerated before leaf shedding :ref:`Knorr et al (2010) <Knorr2010>`. 

Maximum Potential Leaf Area Index
---------------------------------

The :math:`LAI_{max}` defines the maximum potential LAI and is calculated as the minimum of :math:`LAI_W` and a parameter that defines the structural maximum LAI, :math:`\hat{\Lambda}`:

.. math::
   :label: eq_LAI_max

   \tilde{LAI}_{max}(t) = min(\hat{\Lambda}, LAI_W)

In practice, the minimum is calculated using a quadratically smoothed minimum function. Additionally, in order for water limitation to respond not only to instantaneous changes in :math:`W` and :math:`E`, the actual :math:`LAI_{max}` is calculated as a weighted time integration with exponentially declining weights going into the past. In continuous form this is given by:

.. math::
   :label: eq_LAI_max_weighted

   LAI_{max}(t) = \frac{\int_{-\infty}^{0} \tilde{LAI_{max}}(t + \tilde{t}) e^{\tilde{t}/\tau_s} \, d\tilde{t}}{\int_{-\infty}^{0} e^{\tilde{t}/\tau_s} \, d\tilde{t}} = \frac{1}{\tau_s} \int_{-\infty}^{0} \tilde{LAI_{max}}(t + \tilde{t}) e^{\tilde{t}/\tau_s} \, d\tilde{t} = \frac{1}{\tau_s} e^{-t/\tau_s} \int_{-\infty}^{t} \tilde{LAI_{max}}(t') e^{t' / \tau_s} \, dt'

Where :math:`\tau_s` is the averaging period for :math:`\tilde{LAI}_{max}`, typically assumed constant to avoid correlations with the parameter :math:`\tau_W`. Larger values of :math:`\tau_s` mean the more historical :math:`LAI_{max}` values are used, while smaller values emphasize more recent :math:`\tilde{LAI}_{max}` values.  Note: In the above, :math:`\tilde{t}` represents a relative time shift (essentially, how far back in time). The variable :math:`t+\tilde{t}` means "past times relative to t" (i.e., :math:`\tilde{t} \lt 0` gives past times, and :math:`\tilde{t}`=0 refers to the present). In addition, :math:`t'` represents absolute time rather than relative time. For the discrete form of equation :ref:`eq_LAI_max_weighted`, see equation 27 in :ref:`Knorr et al (2010) <Knorr2010>`. 

Phenology-Determining Temperature
---------------------------------

Typically, the phenology-determining temperature follows the growing-degree days concept. However, for reasons outlines in :ref:`Knorr et al (2010) <Knorr2010>`, the phenology-determining temperature, :math:`T`, that used in equation :ref:`eq_f_cdf` is described with a different approach to be consistent with the LAI model here. As with the weighted time integration of :math:`LAI_{max}` above, the phenology-determining temperature is calculated as the normalized integral of air temperature with exponentially declining weights going into the past:

.. math::
   :label: eq_T_weighted

   T(t) = \frac{\int_{-\infty}^{0} T_{air}(t + \tilde{t}) e^{\tilde{t}/\tau_m} \, d\tilde{t}}{\int_{-\infty}^{0} e^{\tilde{t}/\tau_m} \, d\tilde{t}} = \frac{1}{\tau_m} \int_{-\infty}^{0} T_{air}(t + \tilde{t}) e^{\tilde{t}/\tau_m} \, d\tilde{t} = \frac{1}{\tau_m} e^{-t/\tau_m} \int_{-\infty}^{t} T_{air}(t') e^{t' / \tau_m} \, dt'

Where :math:`\tau_m` is the averaging period for :math:`T_{air}`. For the discrete form of equation :ref:`eq_T_weighted`, see equation 19 in :ref:`Knorr et al (2010) <Knorr2010>`. 

Coupling to the Carbon Balance
------------------------------

The equations described above govern how environmental conditions directly constrain the LAI dynamics over time, including temperature, day length and water availability. This must be coupled to carbon balance, whereby a third potentially limiting factor can control LAI dynamics: plant carbon supply. 

**Note:** I think this section may actually be better placed into the carbon allocation and autotrophic respiration section. Makes more sense to me. 





.. rubric:: References

.. _Knorr2010:

Knorr, W., T. Kaminski, M. Scholze, N. Gobron, B. Pinty, R. Giering, and P.-P. Mathieu, 2010, Carbon cycle data assimilation with a generic phenology model, J. Geophys. Res., 115, G04017, https://doi.org/10.1029/2009JG001119

.. _Norton2023:

Norton, A. J., Bloom, A. A., Parazoo, N. C., Levine, P. A., Ma, S., Braghiere, R. K., and Smallman, T. L., 2023, Improved process representation of leaf phenology significantly shifts climate sensitivity of ecosystem carbon balance, Biogeosciences, 20, 2455–2484, https://doi.org/10.5194/bg-20-2455-2023


         .. KNORR.IN.deltat=deltat;
         .. KNORR.IN.n=0;
         .. KNORR.IN.latitude=DATA.ncdf_data.LAT;
         .. KNORR.IN.T_phi=pars[P.T_phi];
         .. KNORR.IN.T_r=pars[P.T_range];
         .. KNORR.IN.plgr=pars[P.plgr];
         .. KNORR.IN.k_L=pars[P.k_leaf];
         .. KNORR.IN.tau_W=pars[P.tau_W];//0.00000001;//
         .. KNORR.IN.t_c=pars[P.time_c];
         .. KNORR.IN.t_r=pars[P.time_r];;
         .. KNORR.IN.lambda_max=pars[P.lambda_max];

         .. KNORR.IN.lambda_max_memory=  POOLS[p+S.M_LAI_MAX];
         .. KNORR.IN.T_memory=POOLS[p+S.M_LAI_TEMP];
         .. KNORR.IN.temp=air_temp_k;
         .. KNORR.IN.n=n;
         .. KNORR.IN.DOY=DOY[n];
         .. KNORR.IN.lambda=LAI;
         .. KNORR.IN.pasm=(POOLS[p+S.H2O_LY1]+POOLS[nxp+S.H2O_LY1]+POOLS[p+S.H2O_LY2]+POOLS[nxp+S.H2O_LY2])*0.5;//Note: soil moisture also available here
         .. KNORR.IN.transp= transp;
