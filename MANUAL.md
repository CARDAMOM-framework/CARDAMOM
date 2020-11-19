

***CARDAMOM community collaborative manual***


Anthony Bloom, Gregory R Quetin, Victoria Meyer, Paul Levine, Shuang Ma, and others [If you’re making any edits, add your name here!]


“Installing” CARDAMOM    2
Get code from Github    2
Github with Matlab I/O    2
Running CARDAMOM    3
Code summary and available platforms    3
The CBF (CARDAMOM binary file) structure    3
CARDAMOM_RUN_MDF    4
Gelman-Rubin convergence criterion    4
For matlab users    4
CARDAMOM C user guide    5
Make a new CBF file (Matlab)    5
CARDAMOM C developer guide    5l

Intro tips    5
Make a new model    6
Add more parameters to the model    6
Add more pools to the model    7
Define prior range for parameters and why log transformed prior range is used    7
Add a new dataset to CARDAMOM DATA structure    8
Make a new cost function    9
Switches for EDCs    9
CARDAMOM model library    10
“Frequently asked questions” and “frequently encountered issues & solutions”    11
Frequently asked questions (FAQs)    11
Frequently encountered issues & solutions (FEIs…?)    12




***“Installing” CARDAMOM

***Get code from Github***
Get invite from CARDAMOM team to join https://github.com/CARDAMOM-framework
Go to https://github.com/CARDAMOM-framework/CARDAMOM_2.1.6c
Click on green “Code” button, and either (a) git clone with ssh (recommended), or (b) use alternative method (e.g. download zip). 

***Github with Matlab I/O***

Follow these steps *once*. Once you’ve successfully made it to step 6, you can then start matlab and you’re ready to use all CARDAMOM routines without any further action.
Note: PC implementation not supported (but do add notes if you’ve managed to do this!).


SOON TO BE REQUIRED: 
Install homebrew (if you don’t already have it) (https://brew.sh/)
[DETAILS AS NEEDED]
Install netcdf library (if you don’t already have it)
type “brew install netcdf” in terminal window (Mac), see step (1) for installing brew. 


Anthony, Alex: setenv('CARDAMOM_NC_CONFIG_PATH','/usr/local/bin/nc-config')
For PC: no supported solution yet… (Paul: add potential windows solution)
Git clone repository OR unzip “CARDAMOM-master.zip” file.
Start matlab
Set CARDAMOM environmental variables in matlab
To do this:
Create or edit existing matlab startup.m file: e.g. type “edit startup.m” in matlab command window to edit new or existing startup file.
Copy lines from CARDAMOM/MATLAB/startup_template.m into startup.m file, and adapt as instructed in startup_template.m comments.
Save startup.m
Make a directory called “DUMPFILES” by typing “mat” in matlab command window
Tips: 
Type “pwd” right after starting matlab to find out current working directory
paths in “startup.m” should either be absolute paths, or relative to current working directory
to avoid github issues, either (a) the current working directory needs to be outside the “CARDAMOM” folder, or equivalent github cloned folder (recommended), or (b) make a “.gitignore” file and ensure github ignores all user-made files within the “CARDAMOM” folder. 
Quit & restart matlab 
Type “which startup” in matlab command window 
check matlab is pointing at correct startup.m file
In case you encounter issues, more info here (https://www.mathworks.com/help/matlab/ref/startup.html)
Run CARDAMOM_DEMO.
This is a test script to ensure all works well. If you get to the end of the demo script, then you’ve got full CARDAMOM functionality! 
Type “CARDAMOM_DEMO” in matlab command window

    
What happens when you run CARDAMOM_DEMO: CARDAMOM_DEMO.m will locally compile the CARDAMOM C code (using “gcc” C compiler). CARDAMOM (called from within matlab) will then show some attempts to find a starting point with probability > 0, and will then start accepting/rejecting parameter samples (note: this is a shorter-than-usual run only for demo purposes). The CARDAMOM_DEMO script will then make a figure with some of the retrieved parameter results. Completion of the CARDAMOM script means all components of CARDAMOM are operational.

Notes:
This works great on macs and (theoretically) on linux. I have no idea what happens on Windows.
Please report questions and issues encountered in FAQ & FEI section below
Works with Matlab 2017b and later versions. Compatibility with earlier versions is likely straightforward.


***Running CARDAMOM***
***Code summary and available platforms***

Currently CARDAMOM wrapper scripts are generally written in matlab and python. Code availability and description across platforms is on a case-by-case basis. We assume (and maybe eventually strive for?) all function names are preserved across matlab/python/other platforms. 


***CARDAMOM_RUN_MDF***

CBR = CARDAMOM_RUN_MDF(CBF,MCO) 

Recommended MCMC configurations 
Sample 10^5 iterations (MCO.niterations=1e5;) for code testing purposes (e.g. CARDAMOM_DEMO)
Sample 10^7 iterations (MCO.niterations=1e7;) for exploratory runs
Sample 10^8 iterations (MCO.niterations=1e8;) for final (e.g. publication) runs.


Gelman-Rubin convergence criterion
Convergence across all parameters is considered adequate for a Gelman-Rubin convergence criterion of <1.2 or <1.1 across all parameter histograms.

For matlab users

Here’s an example matlab code for (a) running multiple chains, and (b) testing convergence between chains.

MCO.niterations=1e7;
for n=1:3;CBR(n)=CARDAMOM_RUN_MDF(CBF,MCO);end
CBR=cardamomfun_combine_parameter_chains(CBR);
[gr]=cardamomfun_convergence_tests(CBF,CBR,[1,1,1])


***Saving output at runtime***

To have CARDAMOM_RUN_MODEL routine save full output at runtime (to avoid having to run model again for post processing):

Opt. 1 (recommendeds): To save output corresponding to PXI project,

Use CARDAMOM_PROJECT_OUTPUTS_beta.m
This function stores output by default

Opt. 2 (prob not worth it for a handful of cbf files): To save output corresponding to cbf file, use


[CBR,CBF]=CARDAMOM_RUN_MODEL(cbffilename,PARS,OPT);
Set OPT.STORE=1 (see “CARDAMOM_RUN_MODEL.m”) for instruction
Make sure to pass string or cell for cbffilename.



***The CBF (CARDAMOM binary file) structure (and eventually)***
 
We will soon transition to nectdf




***CARDAMOM C user guide***
Make a new CBF file (Matlab)
Step 1. Copy an existing CBF structure (CBF=CBFtemplate), OR load an existing file e.g. CBF=CARDAMOM_READ_BINARY_FILEFORMAT(‘cbffile.cbf’));

Step 2. clear observations CBF=cardamomfun_clear_cbf_obs(CBF);

Step 3. Replace all CBF.MET with new MET datasets
Add zeros if there are no fires
Step 4. Set “CBF.LAT” to equal local latitude (in degrees)

Step 5. Add observations from new locations
Use -9999 for any missing observations in CBF.OBS.* fields.


***CARDAMOM C developer guide***


Intro tips
Before doing any of the following, either git branch and/or backup your C code (!!!)
Regularly & frequently compile (e.g. CARDAMOM_COMPILE) when making any changes.


Make a new model

Making a new model ID in CARDAMOM (e.g. ID=830), based on original model (e.g. ID=811). To do this:

Step 1. Open C/projects/CARDAMOM_GENERAL/CARDAMOM_MODEL_LIBRARY.c and create new model identification information (e.g. ID = 830).

Step 2. make folder in projects/CARDAMOM_MODELS/DALEC/DALEC_830 (if copied, open all files in folder and rename all instances of e.g. ”811" to “830”).

Tips for step 2
copy every instance of DALEC_811 and name them DALEC_830 
You can use “CARDAMOM_COMPILE” in matlab, to see if the code compiles OK.
 if the above works without issue, then you should be able to change a CBF.ID value to CBF.ID=830 and the model will run (e.g. with CARDAMOM_RUN_MODEL) without issue!
 Once you’ve successfully replicated CBF.ID=811 to CBF.ID=830, you can then make model structure changes in DALEC_830.c
 Keep using “CARDAMOM_COMPILE” every so often (in matlab, and equivalent function elsewhere) to see if your new code compiles OK.

For matlab users
Open CARDAMOM_RUN_MODEL.m and add the new model ID to the appropriate “if” statement (e.g. if CBF.ID==1000 || CBF.ID==1001;)



Add more parameters to the model

Step 1. In the folder titled C/projects/CARDAMOM_MODELS/DALEC/DALEC_*** (where *** is the ID for your new model), open MODEL_INFO_***.c, and change “DALECmodel.nopars” (e.g. from “33” to “35”)

Step 2. Open PARS_INFO_***c and add two extra entries at the bottom of the code (with minimum and maximum values). Note: CARDAMOM only supports positive-definite values, if a -ve to +ve range is required, use “exp()” function for -ve to +ve value ranges, and use “log()” to transform these back within DALEC_***.c model. 

Step 3. **FOR MATLAB USERS**. Run this line in the matlab command window after making any changes to the number of parameters or pools in MODEL_INFO_***.c

“MD=CARDAMOM_MODEL_LIBRARY(***,[],1);”

(where *** is the ID for your new model). This will ensure new model parameter/pool info is registered in matlab.

Add more pools to the model

Step 1. In the folder titled C/projects/CARDAMOM_MODELS/DALEC/DALEC_*** (where *** is the ID for your new model), open MODEL_INFO_***.c, and change “DALECmodel.nopools” (e.g. from “8” to “9”)

Step 2. To optimize the pool initial conditions along with other parameters, add an extra parameter to the model. Follow instructions in “Add more parameters to the model” section to do this.

Step 3. **FOR MATLAB USERS**. Run this line in the matlab command window after making any changes to the number of parameters or pools in MODEL_INFO_***.c:

“MD=CARDAMOM_MODEL_LIBRARY(***,[],1);”

(where *** is the ID for your new model). This will ensure new model parameter/pool info is registered in matlab.

Step 4. Adapt EDC2_***.c to either (a) run EDC checks on new pools or (b) limit EDC checks to previously existing pools only (check for instances where “nopools” variable is used in loops). This is (unfortunately) a less-than-elegant approach, and we’re working on a comprehensive solution in the long run.
Define prior range for parameters and why log transformed prior range is used 
Avoid using zero as either the minimum or maximum parameter values, as log transformation is used for creating the new parameter values so that there is equal chance being selected within the same magnitude. Log transformation is essential for parameters spanning several magnitudes, like Soil Organic Carbon turnover rate, while doesn’t make a big difference for parameters like Q10; 

Two examples make it easier to understand:
Soil organic C turnover rate (1e-7 to 1e-3 gC yr-1)
Temperature sensitivity (Q10, 1.2 to 1.6)
For soil organic c turnover rate, we want equal possibilities for values spanning 1e-7 to 1e-6, 1e-6 to 1e-5,1e-5 to 1e-4, and 1e-4 to 1e-3, instead of equal chances for values spanning 10e-7 to 0.1*1e-3, 0.1*1e-3 to 0.2*1e-3, 0.2*1e-3 to 0.3*1e-3, … 0.9*1e-3 to 1* 1e-3.


Left figures are showing the uniform space distributions; Middle figures are showing the log uniform space distributions, which is used in CARDAMOM MCMC; Right figures are showing the actual uniform space distributions in order to get the middle distribution;



***Add a new dataset to CARDAMOM DATA structure***

Example: NBE uncertainty, CH4, etc.

Files that are Modified:
CARDAMOM_Reader.c (?)
CARDAMOM read/writer Matlab/ Python (e.g. add to list of obsnames in “CBFOBS=compile_cbf_obs_fields(CBF)” CARDAMOM_WRITE_BINARY_FILEFORMAT.m)
CARDAMOM_DATA_STRUCTURE.c

Step 1. Add observation variable name to CARDAMOM_DATA_STRUCTURE.c (e.g. follow “GPP” entry as example)
Add number of indices and number of none zero obs (again, follow GPP example throughout code)
Step 2. Add to CARDAMOM_READ_BINARY_DATA.c
Initialize data structure, top of file
~L:150; initialize field: e.g. if (DATA->NEEunc==0){DATA->NEEunc=calloc(DATA->nodays,sizeof(double));}
~L:210; add line by line reading of observations
Located in the binary file by the order of observations
~L:266; if (DATA->noobs>11){c=0;for (n=0;n<DATA->nodays;n++){if (DATA->NEEunc[n]>-9998){DATA->neeuncpts[c]=n;c=c+1;}}}
Free the point of the structure ~L:350
~L:360, free the structure (avoid segmentation fault)

Step 3. Add to CARDAMOM_WRITE_BINARY_FILEFORMAT.m

Step 4. Data is available in the data structure for use in Likelihood etc.
Optional: add new cost function module (e.g. DATA_LIKELIHOOD_CH4.c) to use 
To do this, add cost function module call in CARDAMOM/C/projects/DALEC_CODE/MODEL_LIKELIHOOD_FUNCTIONS/DALEC_ALL_LIKELIHOOD.c
If observation field can only be used by subset of models (e.g. DATA.CH4 can only be used by DATA.ID==1010), then add “IF” statement to only run “DATA_LIKELIHOOD_CH4.c” if DATA.ID==1010.



Make a new cost function

Step 1: Create or copy existing DALEC like function (e.g. copy GPP_LIKELIHOOD.c)

E.g. cp DALEC_LIKELIHOOD_GPP.c DALEC_LIKELIHOOD_NEE.c

Step 2: Add ~L:10 include new cost function module in the overhead of the master file (DALEC_ALL_LIKELIHOOD.c). A call new module in code

E.g. #include "DALEC_LIKELIHOOD_NEE.c"
P=P+DALEC_LIKELIHOOD_NEE(D);

Step 3: Write new cost function (e.g. in DALEC_LIKELIHOOD_NEE.c)



Switches for EDCs
Switches CBF.OTHERPRIORS (Anthony provide more detail)



CARDAMOM model library

List and brief description of currently supported models

Can use the COMPLEX effort to document all the models here, including some examples of the figures.


Group
Model ID
Parent ID(s) if relevant
Description
Details (POC)
Status 
Low complexity
101



3-pool DALEC


Tested
DALEC C only
400





Bloom et al., 2016
Published












DALEC C + plant-available water.
803








804








805








806








807








808








809








810


Non-binary EDCs


Exploratory
811
809




Published (Quetin et al., 2020)
812
813
813 with uWUE




813
811
811 with Shuang Ma’s temp fix


Published (Bloom et al., 2020)
820
811
Climate-sensitive mortality 




821


Ball-berry scheme




830








831








840








DALEC + PAW + PUW
1000






In prep. 
1001
1000
1000 with max infiltration
Paul Levine


1002
1000
1000 with uWUE
Paul Levine


1003
1000
1000 with surface runoff proportional to P
Paul Levine


1004
1000
Testing new modular parameter definitions
Anthony Bloom
In prep.
1005
1000
Boese et al, 2017 (uWUE + rad)
Paul Levine, Yan Yang


1006
1002
1002 with max infiltration
Paul Levine


1007
1005
1005 with max infiltration
Paul Levine


1008
1002
1002 with surface runoff proportional to P
Paul Levine


1009
1005
1005 with surface runoff proportional to P
Paul Levine


1010


CH4 module
Ma
In prep.
1020




Norton
In prep.
1021




Norton
In prep.
1040
1005
Nutrient model
Anthony Bloom
In prep.












DALEC + FF
1200






Exploratory



“Frequently asked questions” and “frequently encountered issues & solutions”


Frequently asked questions (FAQs)

What’s the difference between log-uniform and uniform distributions?
See here https://en.wikipedia.org/wiki/Reciprocal_distribution, and example for 0.01-100 below:

Sampling 0.01-100 range with uniform distribution
~25% probability for a value between 0-25
~25% probability for a value between 25-50
~25% probability for a value between 50-75
~25% probability for a value between 75-100
 
Sampling 0.01-100 range with log-uniform distribution
25% probability for a value between 0.01 – 0.1
25% probability for a value between 0.1-1
25% probability for a value between 1-10
25% probability for a value between 10-100


Frequently encountered issues & solutions (FEIs…?)

CARDAMOM_RUN_MODEL.c
Error type:

CARDAMOM chain gets stuck on “inf” values or “nan” values
Check for zero values in observations with log-normal cost functions [ADD DETAILS HERE]


If CARDAMOM_RUN_MODEL gives you 
ERROR! Execution of CARDAMOM_RUN_MODEL.exe failed, entering keyboard mode
Then it is possible that the cbf and/or cbr file names have too many characters (current limit is 1000 character maximum, submit issue on github if you need it longer for some reason)

####################
####################
# Submission command line functions
#### adapted from Jupyter Notebook tutorial: cardamom_tutorial2

### Command line settings
### From command line:
/.../CARDAMOM/C/projects/CARDAMOM_MDF/CARDAMOM_MDF.exe /.../cbf/filename.cbf /.../cbr/filename.cbr 100000 0 200 0.001 119 1000

number_iterations = "100000" # Small numbers for test, 100000000 standard run, will take a long time.

printrate = "0" #

samplerate = "200" # Number of output parameters = number_iterations/samplerate, generally use between 500 and 1000 output parameters

minimum_step_size = ".001" # Relates to MCMC, standard is currently .001

mcmcid = "119" # Current default MCMC method is 119

nadapt = "1000" # MCMC setting, standard is 1000



# List of Drivers

## Inputs (Model ID: 811, 821)
|CARDAMOM Inputs|Units|Longname|
|:-------------|:-------------|:-------------|
|'Time | [Days since Jan 01, 2001]'|
|'min temp | [C]'|
|'max temp | [C]'|
|'Solar irradiance | [MJ/m2/day]'|
|'CO2 | [ppm]'|
|'Day of year'|day|
|'Burned area | [m2/m2]'|
|'VPD | [hPa]'|
|'Precip. | [mm/day]'|


## List of Ouputs (Model ID: 811, 821)

### Pools (gC/m2, kgH2O/m2, instantaneous)
|CARDAMOM Pools|Longname|
|:--------|:-------------|
|'c_labile'|
|'c_foliar'|
|'c_root'|
|'c_wood'|
|'c_finelitter'|
|'c_som'|
|'h2o_forplant'|

### Fluxes (gC/m2/day, kgH2O/m2/day, mean)
|CARDAMOM Fluxes|Longname|
|:--------|:-------------|
|'gppflux'|
|'decf_tempr'|
|'gpp_to_autoresp'|
|'gpp_to_leaf'|
|'gpp_to_labile'|
|'gpp_to_root'|
|'gpp_to_wood'|
|'labile_to_foliar'|
|'leaf_fall'|
|'leaf_to_litter'|
|'wood_to_soilc'|
|'root_to_litter'|
|'hetresp_litter'|
|'hetresp_som'|
|'litter_to_som'|
|'leaf_onset'|
|'fire_em_total'|
|'fire_em_labile'|
|'fire_em_foliar'|
|'fire_em_root'|
|'fire_em_wood'|
|'fire_em_litter'|
|'fire_em_som'|
|'fire1_to_littersom'|
|'fire2_to_littersom'|
|'fire3_to_littersom'|
|'fire4_to_littersom'|
|'fire5_to_littersom'|
|'et'|
|'runoff'|

# Summary of Models

|Model ID|Model Name|Description|
|:--------|:-------------|:---------|
|DALEC_1000|||
|DALEC_1003|||
|DALEC_1004|||
|DALEC_1004_obs|||
|DALEC_101|||
|DALEC_1010|||
|DALEC_1200|||
|DALEC_400|||
|DALEC_410|||
|DALEC_803|||
|DALEC_804|||
|DALEC_805|||
|DALEC_806|||
|DALEC_807|||
|DALEC_808|||
|DALEC_809|||
|DALEC_810|||
|DALEC_811|||
|DALEC_812|||
|DALEC_813|||
|DALEC_820|||
|DALEC_821|||
|DALEC_830|||
|DALEC_831|||
|DALEC_840|||
