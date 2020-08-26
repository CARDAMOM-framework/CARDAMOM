% Step 1. Read CBF file
CBF=CARDAMOM_READ_BINARY_FILEFORMAT('CARDAMOM/DATA/CARDAMOM_DATA_DRIVERS_EXAMPLE.cbf');
%Dimensions
%nopars
%nodays
%Number of timesteps
nodays=size(CBF.MET,1);
%Number of met fields
nomet=size(CBF.MET,2);


%Step 2. write as netcdf file


fname='CARDAMOM/DATA/CARDAMOM_DATA_DRIVERS_EXAMPLE_beta.nc.cbf';delete(fname)

%*******Write CBF structure fields to .nc file**********

%Setting/testing convention here: pass global "attributes" as variables,
%unless completely self-explanatory...

%Global attributes
nccreate(fname,'ID'); ncwrite(fname,'ID',CBF.ID)
nccreate(fname,'LAT'); ncwrite(fname,'LAT',CBF.LAT)
nccreate(fname,'EDC'); ncwrite(fname,'EDC',CBF.EDC)
% nccreate(fname,'MCMC_ID'); ncwrite(fname,'MCMC_ID',int(119)) %Undefined function 'int' for input arguments of type 'double'.
nccreate(fname,'nodays'); ncwrite(fname,'nodays',nodays)
%nccreate(fname,'nomet'); ncwrite(fname,'nomet',CBF.nomet)

%*********write CBF met***********
nccreate(fname,'MET','Dimensions',{'nodays', nodays,'nomet',nomet}); ncwrite(fname,'MET',CBF.MET);




nccreate(fname,'EDCDIAG'); ncwrite(fname,'EDCDIAG',CBF.EDCDIAG)
nccreate(fname,'rc_random_search'); ncwrite(fname,'rc_random_search',double(CBF.rc_random_search))

nccreate(fname,'PARPRIORS','Dimensions',{'nopars', nodays}); 
ncwrite(fname,'PARPRIORS',CBF.PARPRIORS)
nccreate(fname,'PARPRIORUNC','Dimensions',{'nopars',numel(CBF.PARPRIORUNC)});
ncwrite(fname,'PARPRIORUNC',CBF.PARPRIORUNC)

% not sure to keep MET one variable or divide into multiple ones?
% nccreate(fname,'MET','Dimensions',{}); 
% ncwrite(fname,'MET',CBF.MET)

%Caveat = for some reason, matlab wants variable created before making
%global attributes
nccreate(fname,'GPP','Dimensions',{'time',numel(CBF.OBS.GPP)});
%Writing variable 
ncwrite(fname,'GPP',CBF.OBS.GPP);
%Variable attributes
ncwriteatt(fname,'GPP','Description','Gross Primary Productivity')
ncwriteatt(fname,'GPP','Uncertainty',CBF.OBSUNC.GPP.unc)
ncwriteatt(fname,'GPP','Annual Uncertainty',CBF.OBSUNC.GPP.annual_unc)
ncwriteatt(fname,'GPP','gppabs',CBF.OBSUNC.GPP.gppabs)
ncwriteatt(fname,'GPP','obs_unc_threshold',CBF.OBSUNC.GPP.obs_unc_threshold)
ncwriteatt(fname,'GPP','gppabs_info',CBF.OBSUNC.GPP.gppabs_info)
ncwriteatt(fname,'GPP','info',CBF.OBSUNC.GPP.info)
ncwriteatt(fname,'GPP','uncertainty factors',CBF.OBSinfo.uncertainty_factors)

%global attributes
nccreate(fname,'CH4','Dimensions',{'time',numel(CBF.OBS.CH4)});
%Writing variable 
%----- ncwrite(fname,'CH4',CBF.OBS.CH4); %Variable data must not be empty.
%Variable attributes
ncwriteatt(fname,'CH4','Description','Net Methane Emission')
ncwriteatt(fname,'CH4','Uncertainty',CBF.OBSUNC.CH4.unc)
ncwriteatt(fname,'CH4','Annual Uncertainty',CBF.OBSUNC.CH4.annual_unc)
ncwriteatt(fname,'CH4','info',CBF.OBSUNC.CH4.info)
ncwriteatt(fname,'CH4','obs_unc_threshold',CBF.OBSUNC.CH4.obs_unc_threshold)

%**********LAI******************
%global attributes
nccreate(fname,'LAI','Dimensions',{'time',numel(CBF.OBS.LAI)});
%Writing variable 
ncwrite(fname,'LAI',CBF.OBS.LAI);
%Variable attributes
ncwriteatt(fname,'LAI','Description','Leaf Area Index')
ncwriteatt(fname,'LAI','LAI data requirement',CBF.OBSinfo.LAI)
ncwriteatt(fname,'LAI','uncertainty factors',CBF.OBSinfo.uncertainty_factors)
% maybe needed these three fields later
% ncwriteatt(fname,'LAI','Uncertainty',CBF.OBSUNC.LAI.unc) 
% ncwriteatt(fname,'LAI','Annual Uncertainty',CBF.OBSUNC.LAI.annual_unc)
% ncwriteatt(fname,'LAI','obs_unc_threshold',CBF.OBSUNC.LAI.obs_unc_threshold)

%...
%...
%...

%********






