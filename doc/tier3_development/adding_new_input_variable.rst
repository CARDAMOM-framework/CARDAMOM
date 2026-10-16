.. _adding-new-input-variable:

Adding new input variable type
================================

This guide provides step-by-step instructions for adding a new input variable type to CARDAMOM. This is a Tier 3 development task that involves modifying the data structure, input/output mechanisms, and model field requirements.

Overview
--------

Adding a new input variable to CARDAMOM requires modifications across several components:

1. Data structure definitions (C)
2. NetCDF reading functionality (C)
3. Demo input file generator (Python)
4. Model field requirements specification
5. Sample data files

Step-by-step procedure
-----------------------

1. Create sample data file
^^^^^^^^^^^^^^^^^^^^^^^^^^

Create a text file in ``CARDAMOM/DATA/TEXT_FILE_DRIVERS/`` with your variable name (e.g., ``YOUR_VARIABLE.txt``). This file should contain one value per line, matching the time dimension of other driver files (typically 216 timesteps for the demo file).

.. code-block:: bash

   # Example: Create YOUR_VARIABLE.txt with 216 values
   cd CARDAMOM/DATA/TEXT_FILE_DRIVERS/
   # Populate with appropriate values for your variable

2. Update data structure
^^^^^^^^^^^^^^^^^^^^^^^^

Add the new variable to the NETCDF_DATA structure in ``CARDAMOM/C/projects/CARDAMOM_GENERAL/CARDAMOM_NETCDF_DATA_STRUCTURE.c``:

.. code-block:: c

   // In the NETCDF_DATA structure, add:
   TIMESERIES_DRIVER_STRUCT YOUR_VARIABLE;

Place it alphabetically or logically with similar driver variables.

3. Add NetCDF reading functionality
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Update ``CARDAMOM/C/projects/CARDAMOM_GENERAL/CARDAMOM_READ_NETCDF_DATA.c`` to read the new variable from NetCDF files:

.. code-block:: c

   DATA->YOUR_VARIABLE.values=ncdf_read_double_var(ncid, "YOUR_VARIABLE", &(DATA->YOUR_VARIABLE.length));
   DATA->YOUR_VARIABLE.reference_mean=ncdf_read_double_attr(ncid, "YOUR_VARIABLE","reference_mean");
   DEFAULT_REFERENCE_MEAN(&DATA->YOUR_VARIABLE);

4. Update demo file generator
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Modify ``CARDAMOM/PYTHON/check_fun/DEMO_CBF_NC_GENERATOR.py`` to include the new variable in generated NetCDF files. Add an entry to the ``nc_structure`` dictionary:

.. code-block:: python

   nc_structure = {
       # ... existing variables ...
       'YOUR_VARIABLE': (('time_dim',), -9999., {
           'units': 'your_units_here',
           'variable_info': 'Description of data source',
           'coordinates': 'LAT LON'
       }),
       # ... more variables ...
   }

5. Update field requirements
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Add the variable to ``CARDAMOM/PYTHON/check_fun/DALEC_MODEL_FIELD_REQUIREMENTS.txt``:

.. code-block:: text

   YOUR_VARIABLE:[min_value,max_value]:NONE:NONE:NONE

Where:
   - ``[min_value,max_value]`` specifies the valid physical range
   - The three ``NONE`` fields specify: models requiring it, models requiring finite values, and models responding to minute changes [1]_

6. Generate new demo file
^^^^^^^^^^^^^^^^^^^^^^^^^^

Run the demo file generator to create a new ``CARDAMOM_DEMO_INPUT_FILE.cbf.nc`` with your variable:

.. code-block:: bash

   cd CARDAMOM/PYTHON/check_fun/
   python3 DEMO_CBF_NC_GENERATOR.py

7. Verify integration
^^^^^^^^^^^^^^^^^^^^^^

Compile and run CARDAMOM with the new demo file to verify the variable is read correctly. You can temporarily add print statements in the C code to verify the values are loaded:

.. code-block:: c

   printf("YOUR_VARIABLE successfully read, length = %zu\n", DATA.ncdf_data.YOUR_VARIABLE.length);
   if (DATA.ncdf_data.YOUR_VARIABLE.length > 0) {
       printf("YOUR_VARIABLE values (first 10): ");
       for (int n = 0; n < 10 && n < DATA.ncdf_data.YOUR_VARIABLE.length; n++) {
           printf("%.2f ", DATA.ncdf_data.YOUR_VARIABLE.values[n]);
       }
       printf("\n");
   }

8. Testing
^^^^^^^^^^

Run the stress test to ensure the model compiles and executes correctly:

.. code-block:: bash

   cd CARDAMOM/PYTHON/check_fun/
   python3 CARDAMOM_CBF_NC_STRESS_TEST.py

Validation checklist
--------------------

Before considering the integration complete, verify:

- [ ] Text data file exists with correct dimensions
- [ ] Data structure includes new variable
- [ ] NetCDF reader correctly loads the variable
- [ ] Demo file generator includes the variable
- [ ] Field requirements properly specify valid ranges
- [ ] Generated NetCDF file contains the variable with expected values
- [ ] Model compiles without errors
- [ ] Model runs and reads the variable successfully
- [ ] Stress tests pass

Important notes
---------------

.. [1] Individual DALEC models may treat this variable as either an observation or forcing variable. Incorporating the variable into specific model physics is beyond the scope of this input variable integration task.

.. [2] Once the variable has been fully incorporated into DALEC/CARDAMOM model code and its role is established, update the field requirements from ``NONE:NONE:NONE`` to appropriate model-specific settings as needed. This is also beyond the scope of the initial integration.

Coding conventions
------------------

When modifying C code, follow these CARDAMOM conventions:

- Follow existing patterns and style throughout the codebase
- Avoid macros or library dependencies unless absolutely necessary
- Ensure modifications do not alter portability across platforms
- Do not introduce new compiler dependencies
- Keep code modifications brief and easy to interpret
- Use clear, descriptive variable names matching existing conventions

See also
--------

- :ref:`File formats documentation <file-io>`
- :ref:`Compilation guide <compilation>`
- Model field requirements specification
