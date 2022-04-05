## Perturbation experiments using CARDAMOM outputs

(1) Create a .cbr file using your cbf.nc file and CARDAMOM_MDF (e.g. actual.cbf.nc and parameters.cbr), as you’d normally do.

(2) Copy original cbf file to make a new cbf file (e.g. perturbed.cbf.nc), and add a perturbation, e.g. on precipitation, (deltaPREC) on the perturbed.cbf.nc met drivers

### ###Warning: ensure "reference_mean" attribute is either (a) the same value, and explicitly defined, in perturbed.cbf.nc and actual.cbf.nc, or (b) is definied in perturbed.cbf.nc as the time averaged values from actual.cbf.nc###

(3) Run two CARDAMOM_RUN_MODEL runs with perturbed.cbf.nc and actual.cbf.nc (both with parameters.cbr)

(4) For each parameter sample (e.g. 2000 samples), and for a given variable, e.g. NBE, take NBE difference.
For normalized gradients e.g. NBEperturbed – NBEactual., and divide each by deltaPREC, to calculate 2000 “deltaNBE/deltaPREC” values.

(5) Calculate stats of deltaNBE/deltaPREC (e.g. means, stdevs, percentiles etc).
