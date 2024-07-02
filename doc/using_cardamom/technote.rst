Contributing to this technical documentation
============================================

.. toctree::
   :maxdepth: 2

**How to contribute to the CARDAMOM technical documentation:**

Structure of the DALEC technical documentation
----------------------------------------------

The DALEC modules should be self-contained, in that all abbreviations (math symbols, acronyms, etc.) should be defined within the page for that module. Each model version will have its own documentation that includes implementation, and should contain tables of all pools, fluxes, and parameters. Consistency between tables and module documentation is recommended, but not required. For example, the hydrology module documentation may contain generic equations describing transfer of water between soil layers that can apply to any abritrary number of soil layers. The implementation of each specific model version will describe, e.g., the number of soil layers; the table will contain the abbreviations foe each soil layer, and those may not be the same as the generic abbreviations used in the module documentation.


Making edits/modifications locally
----------------------------------

*Note: This is the preferred method for any additions or modifications that involve more than one document, as it insures that all commits on the repository will build without errors.*

* Steps 1--4 only need to be done once on your local machine.

1. Install `Sphinx <https://www.sphinx-doc.org/>`_ on your machine. There are `several methods <https://www.sphinx-doc.org/en/master/usage/installation.html>`_ available for doing this.

2. Install the `myst-parser <https://myst-parser.readthedocs.io/en/latest/intro.html>`_ and the `sphinx_rtd_theme <https://sphinx-rtd-theme.readthedocs.io/en/stable/installing.html>`_. These are both python libraries, so should be installed using `pip <https://pypi.org/project/pip/>`_ or `conda <https://conda.io/projects/conda/en/latest/user-guide/getting-started.html>`_.

3. Clone the `GitHub CARDAMOM repository <https://github.com/CARDAMOM-framework/CARDAMOM>`_ to create a local repository on your machine, if you don't have one already. See `Interacting with our GitHub repository`_ for details on how to clone the repository. 

4. Navigate to the ``doc`` directory inside your local repository, and create two new directories inside; one called ``_build`` and the other called ``_static``.

* Steps 5--9 need to be done each time you want to make and additions or changes.

5. ``git pull`` the latest commit from the `GitHub CARDAMOM repository`_ to make sure your local repository is up to date. See `Interacting with our GitHub repository`_ for details on how to pull changes from GitHub.

6. Make any additions or changes to your local repository. The preferred format for new files is the `reStructuredText (reST) <https://www.sphinx-doc.org/en/master/usage/restructuredtext/basics.html>`_ format, but it is also possible to use the `Markdown <https://www.sphinx-doc.org/en/master/usage/markdown.html>`_ format.

7. Run the command ``make html`` from within the `doc <https://github.com/CARDAMOM-framework/CARDAMOM/tree/main/doc>`_ directory of your local repository. If there are any errors, fix them and repeat this step until there are none.

8. The above step will create a local version of the html documentation, which can be viewed by opening ``doc/_build/html/index.html``.

9. If the build process completes without errors and the html content looks the way you want it to, ``git add`` any new and modified files, ``git commit`` your changes, and ``git push`` to the `GitHub CARDAMOM repository`_. This will automatically trigger the build process to take place on GitHub. Progress can be observed on the `actions <https://github.com/CARDAMOM-framework/CARDAMOM/actions>`_ page. See `Interacting with our GitHub repository`_ for details on how to commit and push changes to GitHub.

Making edits/modifications on the GitHub web interface
------------------------------------------------------

*Note: this method is fine for making modifications to a single file at a time, but it will create a new commit every time a single file is modified. Therefore, if more than one file needs to be modified (i.e., creating a new file that must be added to a higher-level index), this method is not preferred, as it creates commits that do not represent complete builds.*


Interacting with our GitHub repository
--------------------------------------

* To clone the `GitHub CARDAMOM repository`_, use ``git clone https://github.com/CARDAMOM-framework/CARDAMOM.git`` from the command line, or use a GUI based solution such as `GitHub Desktop <https://desktop.github.com>`_.
