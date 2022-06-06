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
+ *Install homebrew (if you don’t already have it: https://brew.sh)*
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

 [Link to PYTHON DEMO](PYTHON_DEMO.md)
