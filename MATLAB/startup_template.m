%CARDAMOM/MATLAB/startup_template.m is a mock "startup.m" file
%Step 1. Create or edit existing startup.m file by typing "edit startup.m" in matlab command window 
%Step 2. Add the following lines in your startup file
%Step 3. Adapth the paths to make sure these are consistent with your directory system (e.g. depending on where you placed the "CARDAMOM" folder) 

%*****CARDAMOM C PATH******
setenv('CARDAMOM_C_PATH','CARDAMOM/C');

%****CARDAMOM matlab directory****
addpath(genpath('CARDAMOM/MATLAB/'));savepath


