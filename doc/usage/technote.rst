Contributing to this technical manual
=====================================

.. toctree::
   :maxdepth: 2

**How to contribute to the CARDAMOM technical manual:**

Making edits/modifications locally
----------------------------------

*Note: This is the preferred method for any additions or modifications that involve more than one document, as it insures that all commits on the repository will build without errors.*

* Steps 1--3 only need to be done once on your local machine.

1. Install `Sphinx <https://www.sphinx-doc.org/>`_ on your machine. There are `several methods <https://www.sphinx-doc.org/en/master/usage/installation.html>`_ available for doing this. This step only needs to be done once, while the following steps should be done each time you are making any changes.

2. Clone the `GitHub CARDAMOM repository`_ to create a local repository on your machine, if you don't have one already.

3. Navigate to the ``doc`` directory inside your local repository, and create two new directories inside; one called ``_build`` and the other called ``_static``.

* Steps 4--8 need to be done each time you want to make and additions or changes.

4. ``git pull`` the latest commit from the `GitHub CARDAMOM repository <https://github.com/CARDAMOM-framework/CARDAMOM>`_ to make sure your local repository is up to date. If you do not already have  

5. Make any additions or changes to your local repository. The preferred format for new files is the `reStructuredText (reST) <https://www.sphinx-doc.org/en/master/usage/restructuredtext/basics.html>`_ format, but it is also possible to use the `Markdown <https://www.sphinx-doc.org/en/master/usage/markdown.html>`_ format.

6. Run the command ``make html`` from within the `doc <https://github.com/CARDAMOM-framework/CARDAMOM/tree/main/doc>`_ directory of your local repository. If there are any errors, fix them and repeat this step until there are none.

7. The above step will create a local version of the html documentation, which can be viewed by opening ``doc/_build/html/index.html``.

8. If everything looks good, ``git commit`` your changes and ``git push`` to the `GitHub CARDAMOM repository`_. This will automatically trigger the build process to take place on GitHub. Progress can be observed on the `actions <https://github.com/CARDAMOM-framework/CARDAMOM/actions>`_ page.

Making edits/modifications on the GitHub web interface
------------------------------------------------------

*Note: this method is fine for making modifications to a single file at a time, but it will create a new commit every time a single file is modified. Therefore, if more than one file needs to be modified (i.e., creating a new file that must be added to a higher-level index), this method is not preferred, as it creates commits that do not represent complete builds.*
