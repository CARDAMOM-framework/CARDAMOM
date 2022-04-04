# CARDAMOM Netcdf input file

## Summary

The CARDAMOM netcdf input file is a "one-stop shop" for all inputs required to configure CARDAMOM including:
- Model structure choice (Model ID [add link here])
- Forcing data required by model structure (met data, burned area, latitude)
- Observations (datasets, time varying uncertainties, and other cost function options)
- MCMC options
- Minimal ancillary non-functional data (these are optional: includes longitude, notes, etc.)



## FAQ
###Q: If I’m not assimilating an observation, can I just delete the corresponding OBS field my cbf.nc file, or do I have to keep it and put NaN in the corresponding subsets?
Answer: you can delete it


 
