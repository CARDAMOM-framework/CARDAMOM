Creating a New DALEC Model ID
==============================

This guide describes the step-by-step process for creating a new DALEC model ID based on an existing model as a template. This process is useful when you want to create a variant of an existing model (e.g., to test modifications or create a new model configuration).

Overview
--------

Creating a new DALEC model involves:

1. Copying and renaming model files
2. Updating internal references throughout the code
3. Registering the model in the CARDAMOM framework
4. Updating model field requirements
5. Compiling and testing

This guide uses the example of creating DALEC_1140 and DALEC_1141 from DALEC_1100 as a template.

Prerequisites
-------------

Before starting, ensure you have:

- A working CARDAMOM installation with successful compilation
- Access to the source model directory (e.g., ``C/projects/CARDAMOM_MODELS/DALEC/DALEC_1100/``)
- Python 3 with netCDF4 library installed
- Basic familiarity with C programming and shell commands

Step 1: Copy the Template Model Directory
------------------------------------------

Navigate to the DALEC models directory and copy the template model:

.. code-block:: bash

   cd /path/to/CARDAMOM/C/projects/CARDAMOM_MODELS/DALEC
   cp -r DALEC_1100 DALEC_1140
   cp -r DALEC_1100 DALEC_1141

This creates new directories with all necessary files.

Step 2: Rename Files in New Directories
----------------------------------------

Rename all files containing the old model ID to use the new ID:

.. code-block:: bash

   cd DALEC_1140
   for file in *1100*; do
       mv "$file" "${file//1100/1140}"
   done

   cd ../DALEC_1141
   for file in *1100*; do
       mv "$file" "${file//1100/1141}"
   done

Step 3: Update Internal Code References
----------------------------------------

Replace all internal references from the old model ID to the new one:

.. code-block:: bash

   # For DALEC_1140
   cd DALEC_1140
   find . -type f \( -name "*.c" -o -name "*.h" \) -exec sed -i '' 's/1100/1140/g' {} \;

   # For DALEC_1141
   cd ../DALEC_1141
   find . -type f \( -name "*.c" -o -name "*.h" \) -exec sed -i '' 's/1100/1141/g' {} \;

**Important:** On Linux systems, use ``sed -i`` instead of ``sed -i ''``.

Step 4: Register Models in CARDAMOM_MODEL_LIBRARY.c
----------------------------------------------------

Add the new models to the CARDAMOM model library:

1. Open ``C/projects/CARDAMOM_GENERAL/CARDAMOM_MODEL_LIBRARY.c``

2. Add include statements (around line 62-65):

.. code-block:: c

   #include "../CARDAMOM_MODELS/DALEC/DALEC_1100/MODEL_INFO_1100.c"
   #include "../CARDAMOM_MODELS/DALEC/DALEC_1140/MODEL_INFO_1140.c"
   #include "../CARDAMOM_MODELS/DALEC/DALEC_1141/MODEL_INFO_1141.c"

3. Add model ID checks (around line 140-145):

.. code-block:: c

   else if (ID==1100  ){MODEL_INFO_1100(DATA);}
   else if (ID==1140  ){MODEL_INFO_1140(DATA);}
   else if (ID==1141  ){MODEL_INFO_1141(DATA);}

Step 5: Update DALEC_MODEL_FIELD_REQUIREMENTS.txt
--------------------------------------------------

Update the model field requirements file to include the new model IDs:

1. Open ``PYTHON/check_fun/DALEC_MODEL_FIELD_REQUIREMENTS.txt``

2. Find lines with model-specific requirements (e.g., DISTURBANCE_FLUX, YIELD)

3. Update from:

.. code-block:: text

   DISTURBANCE_FLUX:[0,INF]:1100:1100:ALL
   YIELD:[0,INF]:1100:1100:ALL

To:

.. code-block:: text

   DISTURBANCE_FLUX:[0,INF]:[1100,1140,1141]:[1100,1140,1141]:ALL
   YIELD:[0,INF]:[1100,1140,1141]:[1100,1140,1141]:ALL

This ensures the stress testing framework recognizes the new models.

Step 6: Compile CARDAMOM
-------------------------

Compile the entire CARDAMOM framework:

.. code-block:: bash

   cd /path/to/CARDAMOM
   ./BASH/CARDAMOM_COMPILE.sh

Check for compilation errors. If successful, you should see:

.. code-block:: text

   COMPILER=gcc
   compile: CARDAMOM_C compiled successfully

**Common Issues:**

- **Missing includes:** Verify all files are properly included in ``CARDAMOM_MODEL_LIBRARY.c``
- **Syntax errors:** Check that sed replacements didn't break code structure

Step 7: Create Test Input Files
--------------------------------

Create demo CBF NetCDF files with the correct model IDs:

.. code-block:: bash

   # Copy the base demo file
   cp CARDAMOM_DEMO_INPUT_FILE.cbf.nc CARDAMOM_DEMO_INPUT_FILE_1140.cbf.nc
   cp CARDAMOM_DEMO_INPUT_FILE.cbf.nc CARDAMOM_DEMO_INPUT_FILE_1141.cbf.nc

Update the ID values using Python:

.. code-block:: python

   from netCDF4 import Dataset

   for model_id in (1140, 1141):
       with Dataset(f'CARDAMOM_DEMO_INPUT_FILE_{model_id}.cbf.nc', 'r+') as nc:
           nc.variables['ID'][:] = model_id
           print(f"{model_id}: ID = {float(nc.variables['ID'][:])}")

Verify the changes:

.. code-block:: bash

   ncdump -v ID CARDAMOM_DEMO_INPUT_FILE_1140.cbf.nc | tail -3
   # Should show: ID = 1140 ;

Step 8: Run Stress Tests
--------------------------

Test the new models using the CARDAMOM stress test:

.. code-block:: bash

   # Test DALEC_1140
   python3 PYTHON/check_fun/CARDAMOM_CBF_NC_FILE_STRESS_TEST.py \
       CARDAMOM_DEMO_INPUT_FILE_1140.cbf.nc

   # Test DALEC_1141
   python3 PYTHON/check_fun/CARDAMOM_CBF_NC_FILE_STRESS_TEST.py \
       CARDAMOM_DEMO_INPUT_FILE_1141.cbf.nc

**Expected Output:**

Both tests should pass with results similar to:

.. code-block:: text

   ✅ TEST 1 SUCCESSFUL: short run with no OBS and EDC=0 completed in 1.4 seconds
   ✅ TEST 2b SUCCESSFUL: short run with OBS and EDC=0 completed in 3.0 seconds

If TEST 1 or TEST 2b fails, review the previous steps carefully.

Verification Checklist
----------------------

Before considering the new model complete, verify:

- ☐ All files renamed correctly (no references to old model ID in filenames)
- ☐ All code references updated (grep for old ID returns only comments/documentation)
- ☐ Model registered in ``CARDAMOM_MODEL_LIBRARY.c``
- ☐ Field requirements updated in ``DALEC_MODEL_FIELD_REQUIREMENTS.txt``
- ☐ CARDAMOM compiles without errors or warnings
- ☐ Demo CBF files created with correct IDs
- ☐ Stress tests pass (TEST 1 and TEST 2b)
- ☐ Template model (e.g., 1100) still works correctly

Troubleshooting
---------------

Compilation Errors
~~~~~~~~~~~~~~~~~~

**Error:** "MODEL_INFO_1140 not defined"

**Solution:** Check that the include statement is added to ``CARDAMOM_MODEL_LIBRARY.c`` and the path is correct.

Test Failures
~~~~~~~~~~~~~

**Error:** TEST 1 fails with segmentation fault

**Solution:**

1. Verify ``DALEC_MODEL_FIELD_REQUIREMENTS.txt`` includes the new model IDs
2. Check that all required fields (DISTURBANCE_FLUX, YIELD) list the new models
3. Ensure the template model passes tests before creating new models

**Error:** "NetCDF: Unknown file format" when running ncdump

**Solution:** The CBF file may be corrupted. Recreate it by copying the original demo file and updating IDs again.

Model Doesn't Run
~~~~~~~~~~~~~~~~~

**Error:** Model ID not recognized by CARDAMOM_MDF

**Solution:**

1. Recompile CARDAMOM
2. Verify the model ID check is added to ``CARDAMOM_MODEL_LIBRARY.c``
3. Check that ``nc.variables['ID'][:]`` equals the new model ID in the CBF file (``ncdump -v ID FILE``)

For AI Assistants
-----------------

When helping users create new DALEC models:

1. **Always verify template model works first** - Run stress tests on the template model before creating copies
2. **Check all file paths** - Use ``find`` and ``grep`` to verify files exist and references are correct
3. **Compile frequently** - Compile after each major step to catch errors early
4. **Update both code and configuration** - Models need registration in both C code and Python configuration files
5. **Test thoroughly** - Both TEST 1 (no observations) and TEST 2b (with observations) must pass
6. **Preserve portability** - No platform-specific code or library dependencies
7. **Follow CARDAMOM coding conventions** - Brief code, no macros unless necessary

The key files that must be modified:

- ``CARDAMOM_MODEL_LIBRARY.c`` (include and ID check)
- ``DALEC_MODEL_FIELD_REQUIREMENTS.txt`` (field requirements)
- All model-specific C files (function names and references)
- Demo CBF files (for testing)

Common pitfalls:

- Not updating ``DALEC_MODEL_FIELD_REQUIREMENTS.txt``
- Incomplete sed replacements (always use ``find ... -exec sed``)

Summary
-------

Creating a new DALEC model ID involves systematic copying, renaming, and registration steps. The process is straightforward but requires attention to detail. Always:

1. Work from a tested template
2. Update all references (files, code, configuration)
3. Compile and test thoroughly
5. Document any model-specific changes

Following this guide ensures new models integrate seamlessly into the CARDAMOM framework and maintain code quality and portability standards.

Additional Resources
--------------------

- :doc:`stress_testing` - Detailed information on CARDAMOM stress testing
- :doc:`compilation` - CARDAMOM compilation guide
- ``markdown_manuals/MODEL_IDs`` - Legacy documentation on model IDs
- ``markdown_manuals/CARDAMOM_C_developer_guide`` - General C development guide

.. note::
   This process creates an exact copy of the template model with a new ID. To modify model behavior, additional changes to the model equations, parameters, or EDC constraints will be required after following this guide.
