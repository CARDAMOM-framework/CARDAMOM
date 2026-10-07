Stress Testing your CARDAMOM input (.nc) file
==============================================

Overview
--------

The ``CARDAMOM_CBF_NC_FILE_STRESS_TEST.py`` script validates CARDAMOM NetCDF input files (``.cbf.nc``) by running a series of automated tests to ensure the file is properly formatted and can be successfully processed by CARDAMOM executables.

Location
--------

The stress test script is located at::

    CARDAMOM/PYTHON/check_fun/CARDAMOM_CBF_NC_FILE_STRESS_TEST.py

Usage
-----

Run the stress test from the CARDAMOM root directory::

    python3 PYTHON/check_fun/CARDAMOM_CBF_NC_FILE_STRESS_TEST.py <input_file.cbf.nc>

Example::

    python3 PYTHON/check_fun/CARDAMOM_CBF_NC_FILE_STRESS_TEST.py CARDAMOM_DEMO_INPUT_FILE.cbf.nc


What the Script Tests
---------------------

**Pre-Test Validation**

Before running CARDAMOM executables, the script checks:

* **Time-varying length consistency**: All time-varying variables (drivers and observational constraints) must have the same length along the time dimension
* **Field validation**: All required fields for the specified DALEC model are present and within valid ranges (as specified in ``DALEC_MODEL_FIELD_REQUIREMENTS.txt``)

**TEST 1: Minimal run without observations (EDC=0)**

* **Purpose**: Verify CARDAMOM_MDF.exe can run (and find a solution) with minimal computational requirements
* **Configuration**:
    * Removes non-required variables from the input.nc file
    * Sets EDC (Ecological & Dynamical Constraints) to 0
    * Sets MCMC sampler to minimal settings (nITERATIONS=1, nSAMPLES=1, nPRINT=1)
* **Expected outcome**: Completes successfully within ~1-2 seconds
(Is given 120 seconds to run).
***Output files**: ``TEST1.cbf.nc``, ``TEST1.cbr.nc``

**Unit Verification (after TEST 1)**

* **Purpose**: Verify that with no observations, the likelihood is zero
* **Method**: Runs ``CARDAMOM_RUN_MODEL.exe`` (forward model) with TEST1 parameters
* **Expected outcome**: Likelihood = 0
* **Output file**: ``TEST1.output.nc``

**TEST 2a: Forward model observation insensitivity check**

* **Purpose**: Verify observations do not affect forward model dynamics (only likelihood calculations)
* **Configuration**: Creates TEST2b input file (with all observations), then runs forward model with TEST1 parameters (from no-observation run)
* **Analysis**: Reports finite value counts for each of the 31 likelihood types and identifies any with zero finite values
* **Output files**: ``TEST2b.cbf.nc``, ``TEST2a.output.nc``

**Likelihood Types Analyzed** (from ``DALEC_ALL_LIKELIHOOD.c``): The list currently includes

Time-series observations:
    * ABGB, CH4, DOM, ET, LE, H, EWT, GPP, SIF, LAI, NBE, ROFF, SCF, FIR, SWE

Mean observations:
    * Mean_ABGB, Mean_FIR, Mean_GPP, Mean_LAI

Parameter/emergent quantity constraints (PEQ):
    * PEQ_Cefficiency, PEQ_CUE, PEQ_NBEmrg, PEQ_iniSnow, PEQ_iniSOM, PEQ_C3frac, PEQ_Vcmax25, PEQ_LCMA, PEQ_clumping, PEQ_r_ch4, PEQ_S_fv, PEQ_rhch4_rhco2


Future versions of CARDAMOM_CBF_NC_FILE_STRESS_TEST.py, th

**TEST 2b: Short run with observations (EDC=0)**

* **Purpose**: Verify CARDAMOM can run successfully with observations present
* **Configuration**:
    * Keeps all observation data from input file
    * Sets EDC to 0
    * Sets MCMC to minimal settings
* **Expected outcome**: Completes successfully within 1-2 seconds
* **Output file**: ``TEST2b.cbr.nc``

**TEST 3a (optional): EDC search initiation with no observations**

* **Purpose**: Verify EDC (Ecological/Demographic Constraints) search can initiate without crashing
* **Configuration**:
    * No observations
    * EDC=1 (constraints enabled)
    * 10 second timeout (initiation check only)
* **Note**: This test is skipped if the original input file has EDC=0
* **Expected outcome**: Process runs for 10 seconds without crashing

**TEST 3b (optional): EDC search with shortened time series**

* **Purpose**: Verify EDC search works with reduced data (faster test)
* **Configuration**:
    * Shortened time series (24 timesteps)
    * No observations
    * EDC=1
    * 10 second timeout
* **Expected outcome**: Process runs for 10 seconds without crashing

Why These Tests Matter
-----------------------

* **TEST 1** ensures the basic CARDAMOM MCMC engine works with minimal data
* **Unit Verification** confirms that without observations, there is no data constraint (likelihood = 0)
* **TEST 2a** validates the critical separation between forward model dynamics (which depend only on parameters and drivers) and likelihood calculations (which depend on observations)
* **TEST 2b** ensures CARDAMOM can utilize observation data for parameter optimization
* **TEST 3a/3b** verify that EDC constraints (physical/biological constraints on parameters) can be applied successfully

File Naming Convention
-----------------------

All test output files use simple sequential naming:

* ``TEST1.cbf.nc``, ``TEST1.cbr.nc``, ``TEST1.output.nc``
* ``TEST2b.cbf.nc``, ``TEST2b.cbr.nc``
* ``TEST2a.output.nc``
* ``TEST3a.cbf.nc`` (if run)
* ``TEST3b.cbf.nc`` (if run)

Interpreting Results
---------------------

**Success indicators:**

* All tests complete without errors
* Unit verification shows likelihood = 0
* TEST2a shows reasonable finite value counts for likelihood types (warnings for 0 finite values indicate missing observations)
* Temporary files (``*.ncSTART``) are automatically cleaned up

**Failure indicators:**

* Timeouts (process hangs)
* Crashes with error messages
* Inconsistent time-varying lengths
* Missing required fields
* Values outside valid ranges

Related Files
-------------

* ``DALEC_MODEL_FIELD_REQUIREMENTS.txt``: Specifies which fields are required by which DALEC models
* ``DALEC_ALL_LIKELIHOOD.c``: Defines the 31 likelihood types and their order
