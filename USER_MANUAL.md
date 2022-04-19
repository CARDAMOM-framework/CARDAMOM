


# SUMMARY
### CARDAMOM community collaborative manual
***Anthony Bloom, Gregory R Quetin, Victoria Meyer, Paul Levine, Shuang Ma, Renato Braghiere and others***  
[If you’re making any edits, add your name here!]

Start here! This is the summary of the CARDAMOM community collaborative manual. Each section has a brief summary of a part of the manual and a link to a more detailed source. This manual covers everything from getting started runing CARDAMOM to example analysis and model development. See table of contexts and section curators below. Curators are in charge of (i) starting a new markdown page and moving content there, and (2) getting other people to contribute content where necessary. Use this manual collaboratively with the CARDAMOM team.

## Table of Contents

- [README file](README.md)
- [GITHUB Basics](#-github-basics)
  * [LINK](MANUAL/GITHUB_BASICS.md)
  
- [Getting Started With CARDAMOM](#-getting-started)
  * Curator: Jessie Au
  * [Link](MANUAL/GETTING_STARTED.md)
  * Notes: probably need to make new pages for DEMO
  * [“Installing” CARDAMOM](#-installing--cardamom)
  * [Get code from Github](#get-code-from-github)
  * [CARDAMOM Github User Must Read](#cardamom-git-must-read)
  * [CARDAMOM Matlab Demo and Setup Test](#cardamom-with-matlab)
  * [CARDAMOM Python Demo and Setup](#cardamom-with-python)

- [Running CARDAMOM](#running-cardamom)
  * Curator: Paul Levine
  * Notes: main section, and command line example; expect subsections for matlab and python
  * Step 1: CARDAMOM_MDF
  * Step 2: CARDAMOM_RUN_MODEL
  * Saving output at runtime (Matlab)

- [Output Data and Analysis]
  * Curator: Greg Quetin
  * [Standard Input, Outputs, Parameters](#cardamom-input-output-parameters)

- [CARDAMOM cost function]
  * Curator: Anthony Bloom
  * [LINK](MANUAL/COST_FUNCTION.md)
  * Notes: with as much as possible review from others
  * Options

- [The ".cbf.nc" File (CARDAMOM netcdf input files)](#cardamom-cbffile)
  * Curator: Matthew Worden
  * [LINK](CARDAMOM_NETCDF_INPUT_FILE.md)
  * cbf.nc Fields
  * [Make a new cbf.nc file](#cardamom-make-cbffile)

- [PERTURBATION EXPERIMENTS](#perturbation-exp)
  * [LINK](MANUAL/PERTURBATION_EXPERIMENTS.md)

- [CARDAMOM C developer guide](#cardamom-c-developer-guid)
  * Curator: Shuang Ma
  * [LINK](MANUAL/CARDAMOM C developer guide.md)
  * [Intro tips]
  * [Make a new model]
  * [Add more parameters to the model]
  * [Add more pools to the model]
  * [Define prior range for parameters and why log transformed prior range is used]
  * [Add a new dataset to CARDAMOM DATA structure]
  * [Make a new cost function]
  * [Switches for EDCs]

- [Running CARDAMOM in parallel](#submission-command-line)
  * Curator: Sarah Worden
  * Notes: scope = examples or resources associated to parallel computing option

- [CARDAMOM-MAPS]
  * Notes: (point to CARDAMOM-MAPS manual)
  * [CARDAMOM-MAPS Repository](https://github.com/CARDAMOM-framework/CARDAMOM-MAPS)

- [“Frequently asked questions” and “frequently encountered issues & solutions”](#faq)
  * Curator: Anthony Bloom
  * [Frequently asked questions (FAQs)]
  * [Frequently encountered issues & solutions (FEIs…?)]

- [Troubleshooting CARDAMOM runs: "my CARDAMOM run doesn't work”](#troubleshoot)

- [Appendix](#appendix)
  * [CARDAMOM model library](#cardamom-model-library)
  * [Recommended Settings](#-recommended-settings)
  * [Helper Scripts](#-helper-scripts)

- [References](#-references)


- [Manual development Notes](#-dev-notes)
  * Notes for this manual development


  




## Getting Started with CARDAMOM <a name="-getting-started"/>

This section covers everything to get you up and running, including "Installing" CARDAMOM, a must read for using the GitHub repository, and demos in both Matlab and Python for running CARDAMOM.



## Running CARDAMOM <a name="running-cardamom"/>

This section outlines setting up CARDAMOM runs now that it is downloaded and tested (see above). The first example will be CARDAMOM run from the command line, followed by descriptions of CARDAMOM wrappers - written in both *Matlab* and *Python*. Code availability and description across platforms is on a case-by-case basis. The basic steps of running CARDAMOM consists of an 'assimilation run', where CARDAMOM is fed drivers and observations through the CBF. This run produces a set of parameters that are constrained by the observations. Finally, a CARDAMOM forward run uses these parameters to predict the states and fluxes of the carbon cycle through time (Figure).

![cardamom_run](/images/CARDAMOM_RUN.png)
*Figure: Diagram of the basic CARDAMOM run modes.*



## CARDAMOM Output Data and Analysis

[NEEDS SUMMARY PARAGRAPH AND LINK]




## The "cbf.nc" file (CARDAMOM binary input file)<a name="cardamom-cbffile"/>

The CBF file contains information on the model's driving meterology and the observations for constraining the model. In addition, the CBF file contains additional information on what model ID to run, uncertainties of observations, parameter priors and uncertainties, and which EDCs to have on. The driving meteorology is often taken from site level measurements or global reanalysis. This section includes the data included in the CBF file and how to create a CBF file to run CARDAMOM



## CARDAMOM C developer guide <a name="cardamom-c-developer-guid"/>

This section covers the addition of models, processes, observations, and cost functions to the CARDAMOM C-code base. Before doing any of the following, either git branch and/or backup your C code. Regularly & frequently compile (e.g. CARDAMOM_COMPILE) when making any changes. The section covers making a new model, adding addtional parameters, pools and fluxes, as well as new datasets. Includes examples.

Before getting started, here are some [Github Basics](MANUAL/GITHUB_BASICS.md) to consider.
 
Find the complete section [HERE](MANUAL/CARDAMOM C developer guide.md).



## Running CARDAMOM in parallel <a name="submission-command-line"/>

It is common to run CARDAMOM in parallel on remote servers. Generally this is done by running multiple points across multiple CPUs. This section covers some helpful tips and examples for creating multiple point runs and submitting them to servers. Examples include limited information for servers commonly used with CARDAMOM as well as example SLURM submission scripts. See also, [CARDAMOM-MAPS](https://github.com/CARDAMOM-framework/CARDAMOM-MAPS).


## “Frequently asked questions” and “frequently encountered issues & solutions” <a name="faq"/>

The frequently asked questions section is a collection of common issues and standard solutions to the eccentric inputs and outputs of CARDAMOM.
 



## Appendix <a name="appendix"/>

### CARDAMOM model library <a name="cardamom-model-library"/>

List and brief description of currently supported models\

Can use the COMPLEX effort to document all the models here, including some examples of the figures.\

#### TABLE A1. CARDAMOM model IDs.

| Group                            | Model ID   | Parent ID(s) if relevant | Description                                         | Details (POC)                   | Status                          |
|----------------------------------|----------  |--------------------------|-----------------------------------------------------|---------------------------------|---------------------------------|
| Low complexity                   | 101        |                          | 3-pool DALEC                                        |                                 | Published (Famiglietti et al., 2021)                   |
| DALEC C only                     | 400        |                          |                                                     | Bloom et al., 2016              | Published                       |
| DALEC C + plant-available water. | 803        |                          |                                                     |                                 |                                 |
|                                  | 804        |                          |                                                     |                                 |                                 |
|                                  | 805        |                          |                                                     |                                 |                                 |
|                                  | 806        |                          |                                                     |                                 |                                 |
|                                  | 807        |                          |                                                     |                                 |                                 |
|                                  | 808        |                          |                                                     |                                 |                                 |
|                                  | 809        |                          |                                                     |                                 |      Published (Yin et al., 2021)     |
|                                  | 810        |                          | Non-binary EDCs                                     |                                 | Exploratory                     |
|                                  | 811        | 809                      |                                                     |                                 | Published (Quetin et al., 2020) |
|                                  | 812        | 813                      | 813 with uWUE                                       |                                 |                                 |
|                                  | 813        | 811                      | 811 with Shuang Ma’s temp fix                       |                                 | Published (Bloom et al., 2020)  |
|                                  | 820        | 811                      | Climate-sensitive mortality                         |                                 |                                 |
|                                  | 821        |                          | Ball-berry scheme                                   |                                 |    In prep.                     |
|                                  | 830        |     809                  |     Stephanie Stettz old weather GPP test           |                                 |                                 |
|                                  | 831        |       809                |    Stettz et al. cold weather GPP                   |                                 |     Stettz et al., 2021, in prep.      |
|                                  | 840        |         No idea          |         No idea                                     |                                 |                                 |
| DALEC + PAW + PUW                | 1000       |      811                 |                                                     |                                 |  Published (Famiglietti et al., 2021)   |
|                                  | 1001       | 1000                     | 1000 with max infiltration                          | Paul Levine                     |                                 |
|                                  | 1002       | 1000                     | 1000 with uWUE                                      | Paul Levine                     |                                 |
|                                  | 1003       | 1000                     | 1000 with surface runoff proportional to P          | Paul Levine                     | Published (Famiglietti et al., 2021)  |
|                                  | 1004       | 1000                     | Testing new modular parameter definitions           | Anthony Bloom                   | In prep.                        |
|                                  | 1005       | 1000                     | Boese et al, 2017 (uWUE + rad)                      | Paul Levine, Yan Yang           |    Yang et al., in prep.        |
|                                  | 1006       | 1005                     | 1005 with continuous EDCs                           | Paul Levine                     | Exploratory                     |
|                                  | 1007       | 1005                     | 1005 with max infiltration                          | Paul Levine                      | Exploratory                     |
|                                  | 1008       | 1002                     | 1002 with surface runoff proportional to P          | Paul Levine                      | Exploratory                     |
|                                  | 1009       | 1005                     | 1005 with surface runoff proportional to P          | Paul Levine                       | Exploratory                     |
|                                  | 1010-1014       |                          | CH4 module                                          | Ma                              | In prep.                        |
|                                  | 1015-1019  |                          | snow module                                          | Ma                              | In prep.                        |
|                                  | 1020       |                          |                                                     | Norton                          | In prep.                        |
|                                  | 1021       |                          |                                                     | Norton                          | In prep.                        |
|                                  | 1030--1039 | 1000                     | VPD-GPP sensitivity                                 | Paul Levine                     |     In prep.                  |
|                                  | 1040       | 1005                     | Nutrient model                                      | Anthony Bloom                   | In prep.                        |
|                                  | 1050       | 1000                     | Nitrogen model                                      | Renato Braghiere                | In prep.                        |
|                                  | 1060--1069 | 1000                     | Hydrology from Massoud et al, 2021                  | Paul Levine                     |                                 |
| Flagship testing                 | 1090--1099 |101x,105x,106x,110x       | Testing of flagship model                           |                                 | For testing only                |
| Flagship                         | 1100--1110 |101x,105x,106x            | "Kitchen sink" model                                |                                 | Exploratory                     |
| DALEC + FF                       | 1200       |                          |                                                     |                                 | Exploratory                     |



### Recommended Settings <a name="-recommended-settings"/>

These are some of the recommended "standard" settings.

```
number_iterations = "100000" # Small numbers for test, 100000000 standard run, will take a long time.
printrate = "0" #
samplerate = "200" # Number of output parameters = number_iterations/samplerate, generally use between 500 and 1000 output parameters
minimum_step_size = ".001" # Relates to MCMC, standard is currently .001
mcmcid = "119" # Current default MCMC method is 119
nadapt = "1000" # MCMC setting, standard is 1000
```



***end of actual "rules"***




"









## References <a name="-references"/>

Williams, M., Schwarz, P. A., Law, B. E., Irvine, J., & Kurpius, M. R. (2005). An improved analysis of forest carbon dynamics using data assimilation. Global Change Biology, 11(1), 89–105.

Bloom, A., & Williams, M. (2015). Constraining ecosystem carbon dynamics in a data-limited world: integrating ecological" common sense" in a model-data fusion framework. Biogeosciences, 12(5), 1299.

Bloom, A. A., Exbrayat, J.-F., van der Velde, I. R., Feng, L., & Williams, M. (2016). The decadal state of the terrestrial carbon cycle: Global retrievals of terrestrial carbon allocation, pools, and residence times. Proceedings of the National Academy of Sciences, 113(5), 1285–1290. https://doi.org/10.1073/pnas.1515160113

Quetin, G. R., Bloom, A. A., Bowman, K. W., & Konings, A. G. (2020). Carbon Flux Variability from a Relatively Simple Ecosystem Model with Assimilated Data is Consistent with Terrestrial Biosphere Model Estimates. Journal of Advances in Modeling Earth Systems. https://doi.org/10.1029/2019MS001889

Famiglietti, C.A., Smallman, T.L., Levine, P.A., Flack-Prain, S., Quetin, G.R., Meyer, V., Parazoo, N.C., Stettz, S.G., Yang, Y., Bonal, D. and Bloom, A.A., 2021. Optimal model complexity for terrestrial carbon cycle prediction. Biogeosciences, 18(8), pp.2727-2754.

Bloom, A. A., Bowman, K. W., Liu, J., Konings, A. G., Worden, J. R., Parazoo, N. C., et al. (2020). Lagged effects dominate the inter-annual variability of the 2010-2015 tropical carbon balance (preprint). Biogeochemistry: Land. https://doi.org/10.5194/bg-2019-459
 
Yin, Y., Bloom, A.A., Worden, J., Saatchi, S., Yang, Y., Williams, M., Liu, J., Jiang, Z., Worden, H., Bowman, K. and Frankenberg, C., 2020. Fire decline in dry tropical ecosystems enhances decadal land carbon sink. Nature communications, 11(1), pp.1-7.


## Manual Development Notes <a name="-getting-started"/>
## Ad-hoc material to "file" away in manual contents
### Fill value conventions: Nans and -9999
### DOM = all dead organic C states, SOM = only DALEC soil 

Also: link at end of each section to get to next section (e.g. previous, next, and "back to summary", definitely include that in all files).
