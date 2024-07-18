Knorr phenology module
======================

.. toctree::
   :maxdepth: 2
   :caption: Contents:

Description and equations for Knorr module

Leaf area index phenology
--------------------------

The leaf area index (LAI) phenology model is based on the scheme developed by Knorr et al. (2010) with updates by Norton et al. (2023). The Knorr et al. (2010) model brings together two concepts. First, the transition of LAI between dormant, active, and senescent states is controlled by three potentially limiting environmental constraints: Temperature, photoperiod, and water availability. Second, spatial variability in growth triggers within a population of plants results in smooth, differentiable functions that describe the transition between dormant, active, and senescent states. This provides a more realistic representation of LAI dynamics, as opposed to unrealistic and impractical step functions. Norton et al. (2023) described the coupling of this LAI phenology model to the carbon and water balance in DALEC and implemented it in CARDAMOM, demonstrating its skill in capturing LAI and carbon cycle dynamics across diverse ecosystem types. We note that in Norton et al. (2023) the LAI phenology sub-model used evapotranspiration as an input to compute the water-limited LAI, considering evaporation and transpiration were not modeled separately. Here, evaporation and transpiration are modeled separately, therefore transpiration is used as the input which is consistent with the original Knorr et al. (2010) formulation and adds realism to this version of CARDAMOM. For a full description of the model, we refer readers to Knorr et al. (2010) and Norton et al. (2023).

Knorr, W., T. Kaminski, M. Scholze, N. Gobron, B. Pinty, R. Giering, and P.-P. Mathieu (2010), Carbon cycle data assimilation with a generic phenology model, J. Geophys. Res., 115, G04017, doi:10.1029/2009JG001119.

Norton, A. J., Bloom, A. A., Parazoo, N. C., Levine, P. A., Ma, S., Braghiere, R. K., and Smallman, T. L.: Improved process representation of leaf phenology significantly shifts climate sensitivity of ecosystem carbon balance, Biogeosciences, 20, 2455–2484, https://doi.org/10.5194/bg-20-2455-2023, 2023.
