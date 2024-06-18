.. CARDAMOM documentation master file, created by
   sphinx-quickstart on Fri May  3 15:49:27 2024.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

Welcome to CARDAMOM's documentation!
====================================

The CARDAMOM documentation presented here is written specifically for the `JPL, Stanford, UCSB and Caltech CARDAMOM framework <https://github.com/CARDAMOM-framework/CARDAMOM>`_ implementation, publicly available under a `GPL-v3 license <https://www.gnu.org/licenses/gpl-3.0.en.html>`_. :ref:`copyright-statement` © 2024, California Institute of Technology (“Caltech”) and University of Washington. U.S. Government sponsorship acknowledged.

[The CARDAMOM tech note is under construction]


.. toctree::
   :maxdepth: 3
   :caption: CARDAMOM Overview:

   cardamom_overview/overview
   cardamom_overview/forward_model
   cardamom_overview/model_data_fusion


.. toctree::
   :maxdepth: 3
   :caption: DALEC model:
   
   dalec_model/overview
   dalec_model/modules
   dalec_model/versions

.. toctree::
   :maxdepth: 2
   :caption: Model-data fusion framework:
   
   framework/cost_function
   framework/observation_operators
   framework/mcmc


.. toctree::
   :maxdepth: 2
   :caption: Using CARDAMOM:

   using_cardamom/file_io
   using_cardamom/compilation
   using_cardamom/technote


.. toctree::
   :maxdepth: 2
   :caption: Old Markdown Manual:
   
   markdown_manuals/ADDING_NEW_NETCF_VARIABLES_AND_ATTRIBUTES
   markdown_manuals/C_MEMORY_LEAKS
   markdown_manuals/CARDAMOM_C_developer_guide
   markdown_manuals/CARDAMOM_NETCDF_INPUT_FILE
   markdown_manuals/COST_FUNCTION
   markdown_manuals/DEBUGGING_TIPS
   markdown_manuals/GETTING_STARTED
   markdown_manuals/GITHUB_BASICS
   markdown_manuals/MATLAB_DEMO
   markdown_manuals/MCMC_IN_CARDAMOM_OVERVIEW
   markdown_manuals/MODEL_IDs
   markdown_manuals/OUTPUT_DATA_ANALYSIS
   markdown_manuals/PARALLELIZATION
   markdown_manuals/PERTURBATION_EXPERIMENTS
   markdown_manuals/PYTHON_DEMO

All of our old stuff is here. We can delete things once they are put into the RST manual, and eventually delete this entire section once it's empty.

.. toctree::
   :maxdepth: 2
   :caption: Miscellaneous


   copyright

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`
