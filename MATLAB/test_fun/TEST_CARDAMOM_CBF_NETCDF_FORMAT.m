% Step 1. Read CBF file
CBF=CARDAMOM_READ_BINARY_FILEFORMAT('CARDAMOM/DATA/CARDAMOM_DATA_DRIVERS_EXAMPLE.cbf');


%Step 2. write as netcdf file


fname='CARDAMOM/DATA/CARDAMOM_DATA_DRIVERS_EXAMPLE_beta.nc.cbf';delete(fname)



%Setting/testing convention here: pass global "attributes" as variables,
%unless completely self-explanatory...

%Global attributes
nccreate(fname,'ID'); ncwrite(fname,'ID',CBF.ID)


%Caveat = for some reason, matlab wants variable created before making
%global attributes
nccreate(fname,'GPP','Dimensions',{'time',numel(CBF.OBS.GPP)});
%Writing variable 
ncwrite(fname,'GPP',CBF.OBS.GPP);
%Variable attributes
ncwriteatt(fname,'GPP','Description','Gross Primary Productivity')







