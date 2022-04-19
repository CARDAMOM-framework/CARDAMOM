# CARDAMOM Netcdf input file

## Summary

The CARDAMOM netcdf input file is a "one-stop shop" for all inputs required to configure CARDAMOM including:
- Model structure choice (Model ID [add link here])
- Forcing data required by model structure (met data, burned area, latitude)
- Observations (datasets, time varying uncertainties, and other cost function options)
- MCMC options
- Minimal ancillary non-functional data (these are optional: includes longitude, notes, etc.)


## Variable names, definitions and conventions


| DALEC Meteorological Driver | Description                                                                                     | Units                                                           |
| --------------------------- | ----------------------------------------------------------------------------------------------- | --------------------------------------------------------------- |
| BURNED\_AREA                | Burned area                                                                                     | m<sup>2</sup>/m<sup>2 </sup>(meters squared per meters squared) |
| CO2                         | Atmospheric carbon dioxide concentration                                                        | ppm (parts per million)                                         |
| DOY                         | Day of year                                                                                     | Days                                                            |
| Total\_Prec                 | Total precipitation                                                                             | mm/day (millimeters per day)                                    |
| SNOWFALL                    | Total snowfall                                                                                  | mm/day (millimeters per day)                                    |
| SSRD                        | Surface shortwave radiation downwards (Solar irradiance or radiation from sun reaching surface) | MJ/m<sup>2</sup>/day (Mega Joules per meters squared per day)   |
| T2M\_MIN                    | Minimum 2-meter air temperature                                                                 | <sup>o</sup>C (degrees Celsius)                                 |
| T2M\_MAX                    | Maximum 2-meter air temperature                                                                 | <sup>o</sup>C (degrees Celsius)                                 |
| time                        | Time since January 01/01/2000 00:00                                                             | Days                                                            |
| VPD                         | Vapor pressure deficit                                                                          | hPA (hectopascal)                                               |



| Time Varying Observations                                                           | Description                                              | Units                                                                            |
| ----------------------------------------------------------------------------------- | -------------------------------------------------------- | -------------------------------------------------------------------------------- |
| ABGB                                                                                | Above and belowground biomass                            | g C/m<sup>2</sup>/day (grams of carbon per meters squared)                       |
| CH4                                                                                 | Methane                                                  | g CH4/m<sup>2</sup>/day (grams of methane per meters squared per day)            |
| ET                                                                                  | Evapotranspiration                                       | mm/day (millimeters per day)                                                     |
| equivalent to kg H2O/m<sup>2</sup>/day (kilograms water per meters squared per day) |
| EWT                                                                                 | Equivalent Water Thickness                               | cm (centimeters)                                                                 |
| GPP                                                                                 | Gross primary productivity or Solar induced fluorescence |  g CO2/m<sup>2</sup>/day (grams of carbon dioxide per meters squared per day) or |
| LAI                                                                                 | Leaf area index                                          | m<sup>2</sup>/m<sup>2</sup>(meters squared per meters squared)                   |
| NBE                                                                                 | Net Biosphere Exchange                                   | g CO2/m<sup>2</sup>/day (grams of carbon dioxide per meters squared per day)     |
| SCF                                                                                 |                                                          |                                                                                  |
| SOM                                                                                 | Soil Organic Matter                                      | g C/m<sup>2</sup>(grams of carbon per meters)                                    |


| Time Invariant Observations | Description                                                                                     | Units                                                                        |
| --------------------------- | ----------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------------- |
| Mean\_Biomass               | Mean above and belowground biomass                                                              | m<sup>2</sup>/m<sup>2</sup>                                                  |
| Mean\_Fire                  | Mean fire emissions                                                                             | g CO2/m<sup>2</sup>/day (grams of carbon dioxide per meters squared per day) |
| Mean\_GPP                   | Mean gross primary productivity                                                                 | g CO2/m<sup>2</sup>/day (grams of carbon dioxide per meters squared per day) |
| Mean\_LAI                   | Mean leaf area index                                                                            | m<sup>2</sup>/m<sup>2</sup>(meters squared per meters squared)               |
| PEQ\_Cefficiency            | Canopy efficiency – If in tropics assign value as 17.5                                          |                                                                              |
| PEQ\_CUE                    | Autotrophic fraction – If NBE and GPP are used do not assign this otherwise assign value of 0.5 |                                                                              |
| PEQ\_iniSOM                 | Initial soil organic matter                                                                     | g C/m<sup>2</sup>(grams of carbon per meters squared per day)                |

| Important NETCDF Variables | Description                        | Units               |
| -------------------------- | ---------------------------------- | ------------------- |
| MCMCID                     | Monte Carlo Markov chain algorithm | unitless            |
| ID                         | DALEC Model ID                     | unitless            |
| EDC                        | Environmental dependent constraints   | unitless            |
| LAT                        | Latitude                           | <sup>o</sup>degrees |





## FAQ
### Q: If I’m not assimilating an observation, can I just delete the corresponding OBS field my cbf.nc file, or do I have to keep it and put NaN in the corresponding subsets?
Answer: you can delete it


 
