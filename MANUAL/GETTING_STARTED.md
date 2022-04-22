## Getting Started with CARDAMOM <a name="-getting-started"/>

### “Installing” CARDAMOM <a name="-installing--cardamom"/>

#### Get code from Github
+ Get invite from CARDAMOM team to join https://github.com/CARDAMOM-framework
+ Go to https://github.com/CARDAMOM-framework/CARDAMOM_2.1.6c
+ Click on green “Code” button, and select git clone with ssh

Example

Step 1. type "cd /Users/[yourusername]/", in your mac terminal, for example (or alternatively go to the preffered directory for storing CARDAMOM code).

Step 2. type "git clone https://github.com/CARDAMOM-framework/CARDAMOM_2.1.6c" mac terminal.
- See Git Clone FAQ below for troubleshooting

**Do not use .zip approach** (!) unless you only intend to download code once, and do not anticipate collaborating with team.
 
 
 ### git clone FAQ
 Get fatal error when typing "git clone https://github.com/CARDAMOM-framework/CARDAMOM_2.1.6c", what should I do?
 
 
 
 
 
 
 
 
 
 
(NOTE: make link to CARDAMOM GITHUB.md, Shuang's user guide).


*SOON TO BE REQUIRED:* 
+ *Install homebrew (if you don’t already have it) (https://brew.sh/)[DETAILS AS NEEDED]*
    * *Install netcdf library (if you don’t already have it)*
    * *type “brew install netcdf” in terminal window (Mac), see step (1) for installing brew. *
    * *Anthony, Alex: setenv('CARDAMOM_NC_CONFIG_PATH','/usr/local/bin/nc-config')*
    * *For PC: no supported solution yet… (Paul: add potential windows solution)*

+ Demos below will compile CARDAMOM and run short assimilation runs and forward runs for testing. There are options in both Python and Matlab. Check Appendix for additional tools written in Matlab and Python.

### CARDAMOM Github User Must Read <a name="cardamom-git-must-read">

After you clone the CARDAMOM repository to your local, please take a minute to go through the first section in  [CARDAMOM_GIT_MUST_READ.md](https://github.com/CARDAMOM-framework/CARDAMOM_2.1.6c/blob/master/CARDAMOM_GIT_MUST_READ.md). We recommand all users to follow the instructions in order to effectively maintain the CARDAMOM github environment. 

### CARDAMOM Matlab Demo and Setup Test <a name="cardamom-with-matlab"/>

[Link to MATLAB DEMO](MATLAB_DEMO.md)

### CARDAMOM Python Demo and Setup <a name="cardamom-with-python"/>


#### Compile code from Github
+ Use a C compiler to comple the assimilation (CARDAMOM_MDF.exe) and forward run (CARDAMOM_RUN_MODEL.exe) functions. For example:

```bash
gcc -std=c99 CARDAMOM/C/projects/CARDAMOM_MDF/CARDAMOM_MDF.c -o CARDAMOM/C/projects/CARDAMOM_MDF/CARDAMOM_MDF.exe -lm
gcc -std=c99 CARDAMOM/C/projects/CARDAMOM_GENERAL/CARDAMOM_RUN_MODEL.c -o CARDAMOM/C/projects/CARDAMOM_GENERAL/CARDAMOM_RUN_MODEL.exe -lm

```

#### Tutorials


Summary of tools to interact with CARDAMOM using python. Read and write of binary files. Conversion of outputs (parameters, pools, fluxes) to netcdf. Additional functionality in summarizing point runs into a netcdf map of results.

See tutorials (PYTHON/tutorials)

- [Tutorial 1](https://github.com/CARDAMOM-framework/CARDAMOM_2.1.6c/blob/master/PYTHON/tutorials/cardamom_tutorial1.ipynb):
Description of input file (.cbf) and how to open/view file. Open and view parameter output from assimilation runs (.cbr). Open and view pools/fluxes from forward run (.bin).

- [Tutorial 2](https://github.com/CARDAMOM-framework/CARDAMOM_2.1.6c/blob/master/PYTHON/tutorials/cardamom_tutorial2.ipynb):
This tutorial will show the basic set up to run a CARDAMOM assimilation and forward run from the command line. This is set up for mac/linux.\

    * Basic Steps:
    * Download/git-clone a copy of CARDAMOM
    * Compile the c-code of CARDAMOM (make sure to do this after any changes)
    * Run compiled file for assimilation with a pointer towards the input (.cbf) and parameters (.cbr) locations.
    * With a parameters file, run a compiled file for a forward run to retrieve the outputs of your parameter ensemble. Point required for outputs (.bin).
    * Common practice is to organize input, parameter and output files in: topfolder/cbf/.cbf topfolder/cbr/.cbr topfolder/output/*.bin
