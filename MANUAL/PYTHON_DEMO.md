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
