.. CARDAMOM documentation master file, created by
   sphinx-quickstart on Fri May  3 15:49:27 2024.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

Welcome to the CARDAMOM documentation
=====================================

**Technical documentation of the CARbon DAta-MOdel fraMework C implemtation (CARDAMOM-C)**

**Authors:** Paul A. Levine, A. Anthony Bloom, Matthew A. Worden, T. Eren Bilir, Gregory R. Quetin, Shuang Ma, Alexander J. Norton

This technical documentation is under construction. In due course, incremental versions will be designated with a digital object identifier (DOI). In the meantime, to make reference to this documentation, please use the following:

**How to cite:** Levine, Paul A., Bloom, A. Anthony, Worden, Matthew A., Bilir, T. Eren, Quetin, Gregory R., Ma, Shuang, Norton, Alexander J., **Technical documentation of the CARbon DAta-MOdel fraMework C implemtation (CARDAMOM-C)**, *in prep.* https://cardamom-framework.github.io/CARDAMOM (last accessed *insert date accessed*)

The CARDAMOM documentation presented here is written specifically for the `JPL, Stanford, UCSB and Caltech CARDAMOM framework <https://github.com/CARDAMOM-framework/CARDAMOM>`_ implementation, publicly available under a `GPL-v3 license <https://www.gnu.org/licenses/gpl-3.0.en.html>`_. :ref:`copyright-statement` © 2024, California Institute of Technology (“Caltech”) and University of Washington. U.S. Government sponsorship acknowledged.

.. toctree::
   :maxdepth: 3
   :caption: CARDAMOM Overview:

   cardamom_overview/overview
   cardamom_overview/forward_model
   cardamom_overview/model_data_fusion

.. toctree::
   :maxdepth: 2
   :hidden:
   :caption: DALEC Model:

   dalec_versions

DALEC Model
===========

.. raw:: html

   <!-- Dropdown to select DALEC version -->
   <select id="toctree-selector" onchange="switchToctree()">
       <option value="select_version">Select DALEC version</option>
       <option value="dalec_1100">DALEC 1100</option>
       <option value="dalec_1101">DALEC 1101</option>
   </select>

   <!-- DALEC 1100 content (initially hidden, controlled by JavaScript) -->
   <div id="select_version" class="toctree-content">
       <ul>
           <li><a href="dalec_versions.html">DALEC versions</a></li>
       </ul>
   </div>

   <!-- DALEC 1100 content (initially hidden, controlled by JavaScript) -->
   <div id="dalec_1100" class="toctree-content" style="display:none;">
       <ul>
           <li><a href="dalec_model/1100.html">DALEC 1100</a></li>
           <div style="padding-left: 20px;"> <!-- Wrapper for indented items -->
               <li><a href="dalec_model/1100/overview.html">Overview of DALEC 1100</a></li>
               <li><a href="dalec_model/1100/modules.html">Modules used by DALEC 1100</a></li>
               <li><a href="dalec_model/1100/table.html">Variables in DALEC 1100</a></li>
           </div>

       </ul>
   </div>

   <!-- DALEC 1101 content (initially hidden) -->
   <div id="dalec_1101" class="toctree-content" style="display:none;">
       <ul>
           <li><a href="dalec_model/1101.html">DALEC 1101</a></li>
           <div style="padding-left: 20px;"> <!-- Wrapper for indented items -->
               <li><a href="dalec_model/1101/overview.html">Overview of DALEC 1101</a></li>
               <li><a href="dalec_model/1101/modules.html">Modules used by DALEC 1101</a></li>
               <li><a href="dalec_model/1101/table.html">Variables in DALEC 1101</a></li>
           </div>

       </ul>
   </div>

.. raw:: html

   <script type="text/javascript">
       function switchToctree() {
           // Get the selected value from the dropdown
           var selector = document.getElementById('toctree-selector');
           var selectedValue = selector.value;

           // Hide all the div sections corresponding to different models
           var sections = document.getElementsByClassName('toctree-content');
           for (var i = 0; i < sections.length; i++) {
               sections[i].style.display = 'none';
           }

           // Display the selected model
           document.getElementById(selectedValue).style.display = 'block';
       }
   </script>

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
