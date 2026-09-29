Getting Started with CARDAMOM
==============================

"Installing" CARDAMOM
---------------------

Get code from Github with ssh
~~~~~~~~~~~~~~~~~~~~

+ Go to https://github.com/CARDAMOM-framework/CARDAMOM
+ Click on green "Code" button, and select git clone with ssh

.. image:: ../images/Green_Code_GH_Button.png
   :width: 500

+ Copy the command




Example
^^^^^^^

**Step 1.** Type ``cd /Users/[yourusername]/`` in your mac terminal, for example (or alternatively go to the preferred directory for storing CARDAMOM code).

**Step 2.** Type ``git clone https://github.com/CARDAMOM-framework/CARDAMOM_2.1.6c`` in mac terminal.

- See Git Clone FAQ below for troubleshooting

.. warning::
   **Do not use .zip approach** (!) unless you only intend to download code once, and do not anticipate collaborating with team.


Compiling CARDAMOM
~~~~~~~~~~~~~~~~~~

First step is to compile the CARDAMOM C code.

Tips
^^^^

If you are working on a new machine, make sure to re-install required packages like python, homebrew, etc.

FAQ
^^^

**Q:** "I get 'Error: could not find any nc-config command.' or something like that, what do I do?"

**A:** Install netcdf using homebrew::

   brew install netcdf

**Q:** "git clone doesn't work, because github needs a key: what do I do now?"

**A:** To get a github key, follow instructions here: https://docs.github.com/en/authentication/connecting-to-github-with-ssh/adding-a-new-ssh-key-to-your-github-account


Get fatal error when typing ``git clone https://github.com/CARDAMOM-framework/CARDAMOM_2.1.6c``, what should I do?

(NOTE: make link to CARDAMOM GITHUB.md, Shuang's user guide).


Required Installation Steps
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. note::
   **SOON TO BE REQUIRED:**

   * Install homebrew (if you don't already have it: https://brew.sh)
   * Install netcdf library (if you don't already have it)

     + Type ``brew install netcdf`` in terminal window (Mac), see step (1) for installing brew.
     + Anthony, Alex: ``setenv('CARDAMOM_NC_CONFIG_PATH','/usr/local/bin/nc-config')``
     + For PC: no supported solution yet… (Paul: add potential windows solution)

   * You may also need:

     + ``brew install python`` (even for matlab users)
     + Install pip on macOS: https://www.geeksforgeeks.org/how-to-install-pip-in-macos/
     + ``pip3 install netcdf4``


+ Demos below will compile CARDAMOM and run short assimilation runs and forward runs for testing. There are options in both Python and Matlab. Check Appendix for additional tools written in Matlab and Python.

CARDAMOM Github User Must Read
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

After you clone the CARDAMOM repository to your local, please take a minute to go through the first section in `CARDAMOM_GIT_MUST_READ.md <https://github.com/CARDAMOM-framework/CARDAMOM_2.1.6c/blob/master/CARDAMOM_GIT_MUST_READ.md>`_. We recommend all users to follow the instructions in order to effectively maintain the CARDAMOM github environment.

CARDAMOM Matlab Demo and Setup Test
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

See :doc:`MATLAB DEMO <../markdown_manuals/MATLAB_DEMO>` for details.

CARDAMOM Python Demo and Setup
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

See :doc:`PYTHON DEMO <../markdown_manuals/PYTHON_DEMO>` for details.
