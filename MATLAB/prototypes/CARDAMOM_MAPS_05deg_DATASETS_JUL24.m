function CARDAMOM_MAPS_05deg_DATASETS_JUL24
%This function is used to generate CARDAMOM-compliant met drivers at 0.5
%degrees for a global domain.
keyboard
date_time_str=datestr(datetime);
YEARS=2001:2023;
%Step 1. Load 0.5 degree datasets
%
%TO DO:netcdf4
[x,y]=loadworldmesh(0.5);

DATA0.x=x(1,:);
DATA0.y=y(:,1);
DATA0.t=1:12;
DATA0.info.name='data';
DATA0.timeunits = 'months since Dec of previous year';
DATA0.Attributes.description='CARDAMOM-MAPS dataset';
DATA0.Attributes.details='CARDAMOM-MAPS datasets are CARDAMOM-compliant data';

%*******Land sea mask *******

[LSM,LSF]=loadlandseamask(0.5);

DATA=DATA0;
DATA.filename='DATA/CARDAMOM-MAPS_05deg_MET/CARDAMOM-MAPS_05deg_LAND_SEA_MASK.nc';
DATA.data=double(LSM);
    DATA.Attribute.variable_info='MODIS-based land-sea mask classification for CARDAMOM runs';
    DATA.info.units='land=1, sea=0';
    
                   CARDAMOM_MAPS_WRITE_2D_DATASET(DATA);

    
                   DATA=DATA0;
DATA.filename='DATA/CARDAMOM-MAPS_05deg_MET/CARDAMOM-MAPS_05deg_LAND_SEA_FRAC.nc';
DATA.data=LSF;
    DATA.Attribute.variable_info='MODIS-based land-sea mask classification for CARDAMOM runs';
    DATA.info.units='Value= 1: land=100%, sea=0%;Value= 0: land=0%, sea=100%;';
    
                   CARDAMOM_MAPS_WRITE_2D_DATASET(DATA);





ET2M=quickload('DATASCRIPT_READ_ERA5_MONTHLY_AUG20(''t2m'',0.5,2001:2023)');
ED2M=quickload('DATASCRIPT_READ_ERA5_MONTHLY_AUG20(''d2m'',0.5,2001:2023)');
ESSRD=quickload('DATASCRIPT_READ_ERA5_MONTHLY_AUG20(''ssrd'',0.5,2001:2023)');
EPREC=quickload('DATASCRIPT_READ_ERA5_MONTHLY_AUG20(''tp'',0.5,2001:2023)');
ESNOW=quickload('DATASCRIPT_READ_ERA5_MONTHLY_AUG20(''sf'',0.5,2001:2023)');
ESTRD=quickload('DATASCRIPT_READ_ERA5_MONTHLY_AUG20(''strd'',0.5,2001:2023)');
ESKT=quickload('DATASCRIPT_READ_ERA5_MONTHLY_AUG20(''skt'',0.5,2001:2023)');
%GFED=quickload('CARDAMOM_MAPS_READ_GFED_MAR22(''05deg'')');
 GFED=quickload('CARDAMOM_MAPS_READ_GFED_JAN24(''05deg'')');


%CO2 
NOAACO2=DATASCRIPT_READ_NOAA_GLOBAL_CO2_MAR24;


%ESTRD=quickload('DATASCRIPT_READ_ERA5_MONTHLY_AUG20(''strd'',0.5,2001:2021)');

% 
% %Extensions
% 
% ET2M=quickload('DATASCRIPT_READ_ERA5_MONTHLY_AUG20(''t2m'',0.5,2020)');
% ED2M=quickload('DATASCRIPT_READ_ERA5_MONTHLY_AUG20(''d2m'',0.5,2020)');
% ESSRD=quickload('DATASCRIPT_READ_ERA5_MONTHLY_AUG20(''ssrd'',0.5,2020)');
% EPREC=quickload('DATASCRIPT_READ_ERA5_MONTHLY_AUG20(''tp'',0.5,2020)');
% ESNOW=quickload('DATASCRIPT_READ_ERA5_MONTHLY_AUG20(''sf'',0.5,2020)');



%DATA should contain
%DATA.filename (filename used to write the data).
%DATA.x (vector with longitudes)
%DATA.y (vector with latitudes)
%DATA.t (vector with times)
%(Optional)
%DATA.lonunits (string with longitude units)
%DATA.latunits (string with longitude units)
%DATA.timeunits (string with time units)
%DATA.data (4D)
%DATA.Attributes (if any)
%DATA.info(n).name (dataset name(s))
%DATA.info(n).units (dataset unit(s))





%Min 2m temp
for yr=YEARS;
    DATA=DATA0;
    DATA.filename=sprintf('DATA/CARDAMOM-MAPS_05deg_MET/CARDAMOM-MAPS_05deg_ERA5_T2M_MIN_%i.nc',yr);
    
    DATA.data=ET2M.datamin(:,:,ET2M.year==yr);
        DATA.Attribute.variable_info='ERA5 0.5 degree dataset: monthly average daily minimum temperature';
                DATA.info.units='deg C';
                %Make file
               CARDAMOM_MAPS_WRITE_3D_DATASET(DATA);
                
end




%Max 2m temp
for yr=YEARS
   DATA=DATA0;
    DATA.filename=sprintf('DATA/CARDAMOM-MAPS_05deg_MET/CARDAMOM-MAPS_05deg_ERA5_T2M_MAX_%i.nc',yr);
    DATA.data=ET2M.datamax(:,:,ET2M.year==yr);
    DATA.Attribute.variable_info='ERA5 0.5 degree dataset: monthly average daily maximum temperature';
    DATA.info.units='deg C';

     CARDAMOM_MAPS_WRITE_3D_DATASET(DATA)
    
end


%Skin temp
for yr=YEARS
   DATA=DATA0;
    DATA.filename=sprintf('DATA/CARDAMOM-MAPS_05deg_MET/CARDAMOM-MAPS_05deg_ERA5_SKT_%i.nc',yr);
    DATA.data=ESKT.data(:,:,ESKT.year==yr);
    DATA.Attribute.variable_info='ERA5 0.5 degree dataset: monthly average surface skin temp';
    DATA.info.units=ESKT.units;

     CARDAMOM_MAPS_WRITE_3D_DATASET(DATA)
    
end




%Srad
for yr=YEARS
   DATA=DATA0;
    DATA.filename=sprintf('DATA/CARDAMOM-MAPS_05deg_MET/CARDAMOM-MAPS_05deg_ERA5_SSRD_%i.nc',yr);
    DATA.data=ESSRD.data(:,:,ESSRD.year==yr);
    DATA.Attribute.variable_info='ERA5 0.5 degree dataset: monthly average shortwave  downward radiation';
    DATA.info.units=ESSRD.units;

     CARDAMOM_MAPS_WRITE_3D_DATASET(DATA)
    
end



%Sthermal radiation
for yr=YEARS
   DATA=DATA0;
    DATA.filename=sprintf('DATA/CARDAMOM-MAPS_05deg_MET/CARDAMOM-MAPS_05deg_ERA5_STRD_%i.nc',yr);
    DATA.data=ESTRD.data(:,:,ESTRD.year==yr);
    DATA.Attribute.variable_info='ERA5 0.5 degree dataset: monthly average thermal downward radiation';
    DATA.info.units=ESTRD.units;

     CARDAMOM_MAPS_WRITE_3D_DATASET(DATA)
    
end


%CO2 ppm

for yr=YEARS

   DATA=DATA0;
    DATA.filename=sprintf('DATA/CARDAMOM-MAPS_05deg_MET/CARDAMOM-MAPS_05deg_NOAA_CO2_%i.nc',yr);
    DATA.data=repmat(permute(NOAACO2.data(NOAACO2.year==yr),[3,2,1]),[size(ESSRD.data,1),size(ESSRD.data,2),1]);
    disp(size(DATA.data))
    DATA.Attribute.variable_info='NOAA global mean surface atmospheric CO2 (not spatially resolved, mean replicated everywhere)';
    DATA.Attribute.source='ftp://aftp.cmdl.noaa.gov/products/trends/co2/co2_mm_gl.txt';
    DATA.Attribute.last_access='Wed Aug  5 09:08:40 2020';
    DATA.Attribute.version_date=date_time_str;
    DATA.info.units='CO2 [ppm]';
     CARDAMOM_MAPS_WRITE_3D_DATASET(DATA)
end



%Burned area

for yr=YEARS
    
   DATA=DATA0;
    DATA.filename=sprintf('DATA/CARDAMOM-MAPS_05deg_MET/CARDAMOM-MAPS_05deg_GFED4.1s_BURNED_AREA_%i.nc',yr);
    DATA.data=GFED.BA(:,:,GFED.year==yr);
    DATA.Attribute.variable_info='GFEDv4.1s burned area at 0.5 degrees';
    DATA.Attribute.source='https://www.globalfiredata.org/data.html';
    DATA.Attribute.description='GFEDv4.1s burned area for 2001-2016, 2017-onward BA approximated based on climatology of total fire C and burned area for each month';
    DATA.info.units='Burned area [m2/m2]';
    DATA.Attribute.version_date=date_time_str;

    
     CARDAMOM_MAPS_WRITE_3D_DATASET(DATA)
end


%VPD



VPD=(SCIFUN_H2O_SATURATION_PRESSURE(ET2M.datamax) - SCIFUN_H2O_SATURATION_PRESSURE(ED2M.datamax))*10;

%VPD
for yr=YEARS
   DATA=DATA0;
    DATA.filename=sprintf('DATA/CARDAMOM-MAPS_05deg_MET/CARDAMOM-MAPS_05deg_ERA5_VPD_%i.nc',yr);
    DATA.data=VPD(:,:,ET2M.year==yr);
    DATA.Attribute.variable_info='ERA5 0.5 degree dataset: monthly average shortwave solar downward radiation';
    DATA.info.units='VPD [hPa]';
        DATA.Attribute.version_date=date_time_str;


     CARDAMOM_MAPS_WRITE_3D_DATASET(DATA)
    
end


%PREC
for yr=YEARS
   DATA=DATA0;
    DATA.filename=sprintf('DATA/CARDAMOM-MAPS_05deg_MET/CARDAMOM-MAPS_05deg_ERA5_TOTAL_PREC_%i.nc',yr);
    DATA.data=EPREC.data(:,:,EPREC.year==yr)*1e3;
    DATA.Attribute.variable_info='ERA5 0.5 degree dataset: monthly average shortwave solar downward radiation';
    DATA.info.units='mm/day';
        DATA.Attribute.version_date=date_time_str;


     CARDAMOM_MAPS_WRITE_3D_DATASET(DATA)
end



%SNOW
for yr=YEARS
   DATA=DATA0;
    DATA.filename=sprintf('DATA/CARDAMOM-MAPS_05deg_MET/CARDAMOM-MAPS_05deg_ERA5_SNOWFALL_%i.nc',yr);
    DATA.data=ESNOW.data(:,:,ESNOW.year==yr)*1e3;
    DATA.Attribute.variable_info='ERA5 0.5 degree dataset: monthly total snow fall';
    DATA.info.units='mm/day';
        DATA.Attribute.version_date=date_time_str;


     CARDAMOM_MAPS_WRITE_3D_DATASET(DATA)
end




end


function CARDAMOM_MAPS_TEMPLATES



end

function CARDAMOM_MAPS_WRITE_3D_TEMPLATE(DATA);

DATA.data=nan(360,720,12);



DATA.filename='DATA/CARDAMOM-MAPS_05deg_MET/CARDAMOM-MAPS_05deg_3D_TEMPLATE.nc';

if isfile(DATA.filename);delete(DATA.filename);end

nccreate(DATA.filename,'longitude','Dimensions',{'longitude',size(DATA.data,2)})
ncwrite(DATA.filename,'longitude',DATA.x);
ncwriteatt(DATA.filename,'longitude','units',  'degrees')


nccreate(DATA.filename,'latitude','Dimensions',{'latitude',size(DATA.data,1)})
ncwrite(DATA.filename,'latitude',DATA.y);
ncwriteatt(DATA.filename,'latitude','units',  'degrees')
%*****CONTINUE FROM HERE*****

nccreate(DATA.filename,'time','Dimensions',{'time',size(DATA.data,3)})
ncwrite(DATA.filename,'time',1:12);
ncwriteatt(DATA.filename,'time','units', 'months since Dec of previous year')

DATA.info.units='unitless ';
DATA.Attribute.variable_info='TEMPLATE DATASET';
nccreate(DATA.filename,'data','Dimensions',{'latitude',size(DATA.data,1),'longitude',size(DATA.data,2),'time',size(DATA.data,3)})
ncwrite(DATA.filename,'data',DATA.data);
ncwriteatt(DATA.filename,'data','units',DATA.info.units);
ncwriteatt(DATA.filename,'data','variable_info',  DATA.Attribute.variable_info)

disp(sprintf('Done with %s',DATA.filename));




end

function CARDAMOM_MAPS_WRITE_2D_TEMPLATE(DATA);

DATA.data=nan(360,720);



DATA.filename='DATA/CARDAMOM-MAPS_05deg_MET/CARDAMOM-MAPS_05deg_2D_TEMPLATE.nc';
         
if isfile(DATA.filename);delete(DATA.filename);end

nccreate(DATA.filename,'longitude','Dimensions',{'longitude',size(DATA.data,2)})
ncwrite(DATA.filename,'longitude',DATA.x);
ncwriteatt(DATA.filename,'longitude','units',  'degrees')


nccreate(DATA.filename,'latitude','Dimensions',{'latitude',size(DATA.data,1)})
ncwrite(DATA.filename,'latitude',DATA.y);
ncwriteatt(DATA.filename,'latitude','units',  'degrees')


DATA.info.units='unitless ';
DATA.Attribute.variable_info='TEMPLATE DATASET';
nccreate(DATA.filename,'data','Dimensions',{'latitude',size(DATA.data,1),'longitude',size(DATA.data,2)})
ncwrite(DATA.filename,'data',DATA.data);
ncwriteatt(DATA.filename,'data','units',DATA.info.units);
ncwriteatt(DATA.filename,'data','variable_info',  DATA.Attribute.variable_info)

disp(sprintf('Done with %s',DATA.filename));




end



%Step 2. Write to file