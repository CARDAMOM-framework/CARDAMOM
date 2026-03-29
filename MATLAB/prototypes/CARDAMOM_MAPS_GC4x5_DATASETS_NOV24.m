function CARDAMOM_MAPS_GC4x5_DATASETS_NOV24




INFO.version=date;
INFO.versionname=['v2.1 (',date,')'];


INFO.DATAPATH='CARDAMOM-MAPS-DATASETS/CARDAMOM-MAPS_GC4x5_V2/';

%naming convention: "4x5deg"
INFO.START_YEAR=2001;
INFO.END_YEAR=2023;



%Other auxiliary stuff
[LSM,LSFRAC]=loadlandseamask('4x5');
[LSM05,LSFRAC05]=loadlandseamask(0.5);
 [x,y,A]=loadworldmesh('4x5');


INFO.x=x;
INFO.y=y;
INFO.A=A;
INFO.LSM=LSM;
INFO.LSFRAC=LSFRAC;



%Data structure 
 DATA0.x=x(1,:);
 DATA0.y=y(:,1);
 DATA0.t=1:12;
 DATA0.info.name='data';
 DATA0.timeunits = 'months since Dec of previous year';
 DATA0.Attributes.description='CARDAMOM-MAPS dataset';
 DATA0.Attributes.details='CARDAMOM-MAPS datasets are CARDAMOM-compliant data';
 DATA0.data=repmat(zeros(size(A)),[1,1,12])*NaN;
%  keyboard


INFO.DATA0=DATA0;




%***********All met data and burned area dealt with here***************
DATASCRIPT_PROCESS_ERA5_MONTHLY_CARDAMOM_DATASETS_NOV23(INFO);





% **********All auxiliary datasets dealt with here******************
 DATA=DATA0;
 DATA.filename=[INFO.DATAPATH,'CARDAMOM-MAPS_GC4x5_LAND_SEA_MASK.nc'];
 DATA.data=double(LSM);
DATA.Attribute.variable_info='MODIS-based land-sea mask classification for CARDAMOM runs';
DATA.info.units='land=1, sea=0';
 CARDAMOM_MAPS_WRITE_2D_DATASET(DATA);

     
     
DATA=DATA0;
DATA.filename=[INFO.DATAPATH,'CARDAMOM-MAPS_GC4x5_LAND_SEA_FRAC.nc'];
DATA.data=LSFRAC;
DATA.Attribute.variable_info='MODIS-based land-sea mask classification for CARDAMOM runs';
DATA.info.units='Value= 1: land=100%, sea=0%;Value= 0: land=0%, sea=100%;';
CARDAMOM_MAPS_WRITE_2D_DATASET(DATA);
% 







 %******************LOADING AND WRITING OBSERVATIONS******************



 %FLUXSAT GPP
 %Source: CARDAMOM-MAPS folder

 %Step 1. Load FLUXSAT GPP




%GPP
v=1;
%GPP
variables{1}='FLUXSAT_GPP';
var_category{1}='FLUXSAT_GPP';
var_info{1}='FLUXSAT GPP regridded as 4x5 degree dataset: monthly average GPP "PER UNIT LAND AREA"';
var_units{1}='gC/m2/day';
mkdir([INFO.DATAPATH,'/',variables{v}]);



 for yr=INFO.START_YEAR:INFO.END_YEAR

     %Read FLUXSAT data layer
     fname=sprintf('CARDAMOM-MAPS-DATASETS/ObsDrivers/%s/CARDAMOM-MAPS_05deg_%s_%i.nc',var_category{v},variables{v},yr);
     if isfile(fname)
         disp('Reading data')
          data_fineres=permute(ncread(fname,'data'),[2,1,3]);
     %Setting all empty land pixels to zero (rather than nan)
          data_fineres(~isfinite(     data_fineres) & repmat(LSM05,[1,1,12])==1)=0;
         disp('Declaring GC data')

    DATA=DATA0;
     DATA.filename=sprintf('%s/%s/CARDAMOM-MAPS_GC4x5_%s_%i.nc',INFO.DATAPATH,variables{v},variables{v},yr);
     %Converting from m/day to mm/day
     disp('Regridding data')
     DATA.data=GEOSChem_regular_grid_to_GC(     data_fineres);
     DATA.Attribute.variable_info=var_info{v};
     DATA.info.units=var_units{v};
     disp('Writing data')
      CARDAMOM_MAPS_WRITE_3D_DATASET(DATA);
     else
         disp('Following file does not exist')
         disp(fname)
     end
 end






%GPP
v=2;

variables{2}='Xu_ABGB';
var_category{2}='ABGB';
var_info{2}='ABGB regridded as 4x5 degree dataset: monthly average ABGB "PER UNIT LAND AREA"';
var_units{2}='gC/m2';
mkdir([INFO.DATAPATH,'/',variables{v}]);


 for yr=INFO.START_YEAR:INFO.END_YEAR

     %Read FLUXSAT data layer
     fname=sprintf('CARDAMOM-MAPS-DATASETS/ObsDrivers/%s/CARDAMOM-MAPS_05deg_%s_%i.nc',var_category{v},variables{v},yr);
     if isfile(fname)
         disp('Reading data')
          data_fineres=ncread(fname,'data');
     %Setting all empty land pixels to zero (rather than nan)
          data_fineres(~isfinite(     data_fineres) & repmat(LSM05,[1,1,12])==1)=0;
         disp('Declaring GC data')

    DATA=DATA0;
     DATA.filename=sprintf('%s/%s/CARDAMOM-MAPS_GC4x5_%s_%i.nc',INFO.DATAPATH,variables{v},variables{v},yr);
     %Converting from m/day to mm/day
     disp('Regridding data')
     DATA.data=GEOSChem_regular_grid_to_GC(     data_fineres);
     DATA.Attribute.variable_info=var_info{v};
     DATA.info.units=var_units{v};
     disp('Writing data')
      CARDAMOM_MAPS_WRITE_3D_DATASET(DATA);
     else
         disp('Following file does not exist')
         disp(fname)
     end
 end


%ABGB



v=3;
variables{3}='GRACE_EWT';
var_category{3}='GRACE';
var_info{3}='EWT regridded as 4x5 degree dataset: monthly average relative water storage anomaly';
var_units{3}='mm H2O';
mkdir([INFO.DATAPATH,'/',variables{v}]);


 for yr=INFO.START_YEAR:INFO.END_YEAR

     %Read FLUXSAT data layer
     fname=sprintf('CARDAMOM-MAPS-DATASETS/ObsDrivers/%s/CARDAMOM-MAPS_05deg_%s_%i.nc',var_category{v},variables{v},yr);
     if isfile(fname)
         disp('Reading data')
     data_fineres=ncread(fname,'data');
     %Setting all empty land pixels to zero (rather than nan)
          data_fineres(~isfinite(     data_fineres) & repmat(LSM05,[1,1,12])==1)=0;
         disp('Declaring GC data')

    DATA=DATA0;
     DATA.filename=sprintf('%s/%s/CARDAMOM-MAPS_GC4x5_%s_%i.nc',INFO.DATAPATH,variables{v},variables{v},yr);
     %Converting from m/day to mm/day
     disp('Regridding data')
     DATA.data=GEOSChem_regular_grid_to_GC(     data_fineres);
     DATA.Attribute.variable_info=var_info{v};
     DATA.info.units=var_units{v};
     disp('Writing data')
      CARDAMOM_MAPS_WRITE_3D_DATASET(DATA);
     else
         disp('Following file does not exist')
         disp(fname)
     end
 end



%LAI
v=4;
variables{4}='MODIS_LAI';
var_category{4}='LAI';
var_info{4}='LAI regridded as 4x5 degree dataset: monthly average leaf area index';
var_units{4}='m2/m2';
mkdir([INFO.DATAPATH,'/',variables{v}]);


 for yr=INFO.START_YEAR:INFO.END_YEAR

     %Read FLUXSAT data layer
     fname=sprintf('CARDAMOM-MAPS-DATASETS/ObsDrivers/%s/CARDAMOM-MAPS_05deg_%s_%i.nc',var_category{v},variables{v},yr);
     if isfile(fname)
         disp('Reading data')
     data_fineres=ncread(fname,'data');
     %Setting all empty land pixels to zero (rather than nan)
          data_fineres(~isfinite(     data_fineres) & repmat(LSM05,[1,1,12])==1)=0;
         disp('Declaring GC data')

    DATA=DATA0;
     DATA.filename=sprintf('%s/%s/CARDAMOM-MAPS_GC4x5_%s_%i.nc',INFO.DATAPATH,variables{v},variables{v},yr);
     %Converting from m/day to mm/day
     disp('Regridding data')
     DATA.data=GEOSChem_regular_grid_to_GC(     data_fineres);
     DATA.Attribute.variable_info=var_info{v};
     DATA.info.units=var_units{v};
     disp('Writing data')
      CARDAMOM_MAPS_WRITE_3D_DATASET(DATA);
     else
         disp('Following file does not exist')
         disp(fname)
     end
 end


%Now import NEE from FLUXCOM 


FLUXCOM=DATASCRIPT_READ_FLUXCOM_DATASETS_APR23;

v=5;
variables{5}='FLUXCOM_NEE';
var_category{5}='FLUXCOM_NEE';
var_info{5}='NEE regridded as 4x5 degree dataset: monthly NEE';
var_units{5}='gC/m2/d';
mkdir([INFO.DATAPATH,'/',variables{v}]);


 for yr=INFO.START_YEAR:INFO.END_YEAR

     %Read FLUXSAT data layer
    
         disp('Reading data')
     data_fineres=FLUXCOM.NEE.data(:,:,FLUXCOM.NEE.year==yr);
     if ~isempty(data_fineres);
     %Setting all empty land pixels to zero (rather than nan)
          data_fineres(~isfinite(     data_fineres) & repmat(LSM05,[1,1,12])==1)=0;
         disp('Declaring GC data')

    DATA=DATA0;
     DATA.filename=sprintf('%s/%s/CARDAMOM-MAPS_GC4x5_%s_%i.nc',INFO.DATAPATH,variables{v},variables{v},yr);
     %Converting from m/day to mm/day
     disp('Regridding data')
     DATA.data=GEOSChem_regular_grid_to_GC(     data_fineres);
     DATA.Attribute.variable_info=var_info{v};
     DATA.info.units=var_units{v};
     disp('Writing data')
      CARDAMOM_MAPS_WRITE_3D_DATASET(DATA);
     else
         disp('Year does not exist');
     end
 end



 
%Now do same for static datasets


v=6;
variables{6}='HWSD_PEQ_iniSOM';
var_category{6}='PEQ_iniSOM';
var_info{6}='HWSD soil carbon database (top 1m)';
var_units{6}='gC/m2';
mkdir([INFO.DATAPATH,'/',variables{v}]);

     %Read FLUXSAT data layer
     fname='CARDAMOM-MAPS-DATASETS/ObsDrivers/PEQ_iniSOM/CARDAMOM-MAPS_05deg_HWSD_PEQ_iniSOM.nc';
     disp('Reading data')
     data_fineres=ncread(fname,'data');
     %Setting all empty land pixels to zero (rather than nan)
     data_fineres(~isfinite(     data_fineres) & LSM05==1)=0;
     disp('Declaring GC data')

    DATA=DATA0;
     DATA.filename=sprintf('%s/%s/CARDAMOM-MAPS_GC4x5_%s.nc',INFO.DATAPATH,'HWSD_PEQ_iniSOM','HWSD_PEQ_iniSOM');
     %Converting from m/day to mm/day
     disp('Regridding data')
     DATA.data=GEOSChem_regular_grid_to_GC(data_fineres);
     DATA.Attribute.variable_info=var_info{v};
     DATA.info.units=var_units{v};
     disp('Writing data')
      CARDAMOM_MAPS_WRITE_2D_DATASET(DATA);

     





 %GFED fires 
 %Source: GFED
%Load directly using
%"GFED=quickload('CARDAMOM_MAPS_READ_GFED_APR23(''GC4x5'')');"





%GFED fires
v=7;
%GPP
variables{v}='GFED_FIRE_C';
var_category{v}='FIRE_C';
var_info{v}='GFED_FIRE_C regridded as 4x5 degree dataset: monthly average GPP "PER UNIT LAND AREA"';
var_units{v}='gC/m2/day';
mkdir([INFO.DATAPATH,'/',variables{v}]);

GFED=quickload('CARDAMOM_MAPS_READ_GFED_NOV24(''GC4x5'')');

 for yr=INFO.START_YEAR:INFO.END_YEAR

     % %Read FLUXSAT data layer
     % fname=sprintf('CARDAMOM-MAPS-DATASETS/ObsDrivers/%s/CARDAMOM-MAPS_05deg_%s_%i.nc',var_category{v},variables{v},yr);
     % if isfile(fname)
     %     disp('Reading data')
     %      data_fineres=permute(ncread(fname,'data'),[2,1,3]);
     % %Setting all empty land pixels to zero (rather than nan)
     %      data_fineres(~isfinite(     data_fineres) & repmat(LSM05,[1,1,12])==1)=0;
     %     disp('Declaring GC data')

    DATA=DATA0;
     DATA.filename=sprintf('%s/%s/CARDAMOM-MAPS_GC4x5_%s_%i.nc',INFO.DATAPATH,variables{v},variables{v},yr);
     %Converting from m/day to mm/day
     disp('Regridding data')
     if ~isempty(find(GFED.year==yr))
     DATA.data=GFED.FireC(:,:,GFED.year==yr);
     DATA.Attribute.variable_info=var_info{v};
     DATA.info.units=var_units{v};
     disp('Writing data')
      CARDAMOM_MAPS_WRITE_3D_DATASET(DATA);
     else
         disp('Following file does not exist')
         disp(fname)
     end
     
 end
 
 
end







function CARDAMOM_MAPS_WRITE_3D_TEMPLATE(DATA);

DATA.data=nan(46,72,12);



DATA.filename='DATA/CARDAMOM-MAPS_GC4x5_MET/CARDAMOM-MAPS_GC4x5_3D_TEMPLATE.nc';

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

DATA.data=nan(46,72)



DATA.filename='DATA/CARDAMOM-MAPS_GC4x5_MET/CARDAMOM-MAPS_GC4x5_2D_TEMPLATE.nc';
         
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



function DATASCRIPT_PROCESS_ERA5_MONTHLY_CARDAMOM_DATASETS_NOV23(INFO)



%Step 0. Read "STATUS" array which includes all ERA datasets
%available/downloaded
ERA5_DATA_STATUS=DATASCRIPT_DOWNLOAD_ERA5_MONTHLY_CARDAMOM_DATASETS_NOV23(0);




INFO.START_YEAR;
INFO.END_YEAR;
version=INFO.version;
versionname=INFO.versionname;



mkdir(INFO.DATAPATH);
YEARS=INFO.START_YEAR:INFO.END_YEAR;


 [x,y,A]=loadworldmesh('4x5');

% 
x=INFO.x;
y=INFO.y;
A=INFO.A;
DATA0=INFO.DATA0;



%  keyboard


LSFRAC=INFO.LSFRAC;
LSM=INFO.LSM;




% 
%  DATA=DATA0;
%  DATA.filename=[DATAPATH,'CARDAMOM-MAPS_GC4x5_LAND_SEA_MASK.nc'];
%  DATA.data=double(LSM);
%      DATA.Attribute.variable_info='MODIS-based land-sea mask classification for CARDAMOM runs';
%      DATA.info.units='land=1, sea=0';
%                          CARDAMOM_MAPS_WRITE_2D_DATASET(DATA);
% 
%      
%      
%       DATA=DATA0;
%  DATA.filename=[DATAPATH,'CARDAMOM-MAPS_GC4x5_LAND_SEA_FRAC.nc'];
%  DATA.data=LSFRAC;
%      DATA.Attribute.variable_info='MODIS-based land-sea mask classification for CARDAMOM runs';
%      DATA.info.units='Value= 1: land=100%, sea=0%;Value= 0: land=0%, sea=100%;';
%                     CARDAMOM_MAPS_WRITE_2D_DATASET(DATA);
% % 



%SW down
%Should be SSRD

    %Read solar radiation from file


    DRIVER.CONVERSION_FAC=1e-6;
    DRIVER.NAME='ssrd';

    DRIVER.PATH=[INFO.DATAPATH,'/SSRD'];
    mkdir( DRIVER.PATH);

 for YR=INFO.START_YEAR:INFO.END_YEAR;
    DATA=DATA0;
         DATA.filename=sprintf('%s/CARDAMOM-MAPS_GC4x5_ERA5_SSRD_%i.nc',    DRIVER.PATH,YR);
     DATA.Attribute.variable_info='ERA5 4x5 degree dataset: monthly average daily shortwave downward radiation';
     DATA.info.units='MJ/m2/day';
     %Populate with existing fields
    for M=1:12
         F=read_era_field_month_year(YR,M,DRIVER.NAME,ERA5_DATA_STATUS);

         %QC and/or QC corrections
         %1. Capping bottom values at zero
         F(F<0)=0;

        if isempty(F)==0

         %Converting from m/day to mm/day
          DATA.data(:,:,M)=GEOSChem_regular_grid_to_GC(F)*    DRIVER.CONVERSION_FAC;

    
        end
     
    end

CARDAMOM_MAPS_WRITE_3D_DATASET(DATA)

    %Writing yearly dataset
end






    %****************2M TEMPERATURE AND VPD*******************
    DRIVER.CONVERSION_OFFSET=-273.15;


    ERA5_T2M.NAME='t2m';
    ERA5_D2M.NAME='d2m';
    

    %EVPD.data=(SCIFUN_H2O_SATURATION_PRESSURE(ET2M.datamax) - SCIFUN_H2O_SATURATION_PRESSURE(ED2M.datamax))*10;


     T2M_MIN_PATH=[INFO.DATAPATH,'/T2M_MIN'];
     T2M_MAX_PATH=[INFO.DATAPATH,'/T2M_MAX'];
     VPD_MAX_PATH=[INFO.DATAPATH,'/VPD_MAX'];
    mkdir( T2M_MIN_PATH);
    mkdir( T2M_MAX_PATH);
    mkdir( VPD_MAX_PATH);

 for YR=INFO.START_YEAR:INFO.END_YEAR;
        DATA_T2M_MIN=DATA0;
        DATA_T2M_MAX=DATA0;
        DATA_VPD_MAX=DATA0;
         DATA_T2M_MIN.filename=sprintf('%s/CARDAMOM-MAPS_GC4x5_ERA5_T2M_MIN_%i.nc', T2M_MIN_PATH,YR);
         DATA_T2M_MAX.filename=sprintf('%s/CARDAMOM-MAPS_GC4x5_ERA5_T2M_MAX_%i.nc', T2M_MAX_PATH,YR);
         DATA_VPD_MAX.filename=sprintf('%s/CARDAMOM-MAPS_GC4x5_ERA5_VPD_MAX_%i.nc', VPD_MAX_PATH,YR);


         DATA_T2M_MIN.info.units='K';
         DATA_T2M_MAX.info.units='K';
         DATA_VPD_MAX.info.units='K';

         DATA_T2M_MIN.Attribute.variable_info='ERA5 4x5 degree dataset: monthly average daily minimum temperature';
         DATA_T2M_MAX.Attribute.variable_info='ERA5 4x5 degree dataset: monthly average daily maximum temperature';
         DATA_VPD_MAX.Attribute.variable_info='ERA5 4x5 degree dataset derived maximum VPD';


     
     %Populate with existing fields
    for M=1:12
         T2M=read_era_field_month_year(YR,M,    ERA5_T2M.NAME,ERA5_DATA_STATUS);
         D2M=read_era_field_month_year(YR,M,    ERA5_D2M.NAME,ERA5_DATA_STATUS);
        if isempty(T2M)==0 & isempty(D2M)==0

        %QC and/or QC corrections and/or postprocessing deriations
         %1. Capping bottom values at zero
        T2M_MIN=min(T2M,[],3);
        T2M_MAX=max(T2M,[],3);
        %Update to deriving (1) VPD first, and then (2) VPD max
        %Step 1. derive VPD
        
       VPD=(SCIFUN_H2O_SATURATION_PRESSURE(T2M+ DRIVER.CONVERSION_OFFSET) - SCIFUN_H2O_SATURATION_PRESSURE(D2M+ DRIVER.CONVERSION_OFFSET))*10;
       VPD_MAX=max(VPD,[],3);
       VPD_MAX(VPD_MAX<0)=0;

      

         %Converting from m/day to mm/day
                 DATA_T2M_MIN.data(:,:,M)=GEOSChem_regular_grid_to_GC(T2M_MIN) + DRIVER.CONVERSION_OFFSET;
                 DATA_T2M_MAX.data(:,:,M)=GEOSChem_regular_grid_to_GC(T2M_MAX) + DRIVER.CONVERSION_OFFSET;
                 DATA_VPD_MAX.data(:,:,M)=GEOSChem_regular_grid_to_GC(VPD_MAX);

    
        end
     
    end

%Annually 

CARDAMOM_MAPS_WRITE_3D_DATASET(DATA_T2M_MIN)
CARDAMOM_MAPS_WRITE_3D_DATASET(DATA_T2M_MAX)
CARDAMOM_MAPS_WRITE_3D_DATASET(DATA_VPD_MAX)

    %Writing yearly dataset
 end



 %**********************Precipitation ******************


    DRIVER.CONVERSION_FAC=1e3;
    DRIVER.NAME='tp';

    DRIVER.PATH=[INFO.DATAPATH,'/PREC'];
    mkdir( DRIVER.PATH);

 for YR=INFO.START_YEAR:INFO.END_YEAR;
    DATA=DATA0;
         DATA.filename=sprintf('%s/CARDAMOM-MAPS_GC4x5_ERA5_PREC_%i.nc',    DRIVER.PATH,YR);
     DATA.Attribute.variable_info='ERA5 4x5 degree dataset: monthly average daily precipitation';
     DATA.info.units='mm/day';
     %Populate with existing fields
    for M=1:12
         F=read_era_field_month_year(YR,M,DRIVER.NAME,ERA5_DATA_STATUS);

         %QC and/or QC corrections
         %1. Capping bottom values at zero
         F(F<0)=0;

        if isempty(F)==0

         %Converting from m/day to mm/day
          DATA.data(:,:,M)=GEOSChem_regular_grid_to_GC(F)*    DRIVER.CONVERSION_FAC;

    
        end
     
    end
    %Writing yearly dataset

CARDAMOM_MAPS_WRITE_3D_DATASET(DATA)

end




%*******************SNOWFALL***********************





    DRIVER.CONVERSION_FAC=1e3;
    DRIVER.NAME='sf';

    DRIVER.PATH=[INFO.DATAPATH,'/SNOWFALL'];
    mkdir( DRIVER.PATH);

 for YR=INFO.START_YEAR:INFO.END_YEAR;
    DATA=DATA0;
         DATA.filename=sprintf('%s/CARDAMOM-MAPS_GC4x5_ERA5_SNOWFALL_%i.nc',    DRIVER.PATH,YR);
     DATA.Attribute.variable_info='ERA5 4x5 degree dataset: monthly average daily snowfall';
     DATA.info.units='mm/day';
     %Populate with existing fields
    for M=1:12
         F=read_era_field_month_year(YR,M,DRIVER.NAME,ERA5_DATA_STATUS);

         %QC and/or QC corrections
         %1. Capping bottom values at zero
         F(F<0)=0;

        if isempty(F)==0

         %Converting from m/day to mm/day
          DATA.data(:,:,M)=GEOSChem_regular_grid_to_GC(F)*    DRIVER.CONVERSION_FAC;

    
        end
     
    end
    %Writing yearly dataset

CARDAMOM_MAPS_WRITE_3D_DATASET(DATA)

end



%*******************Land surface temp (SKT)************************






 DRIVER.CONVERSION_OFFSET = 273.15;
    DRIVER.NAME='skt';

    DRIVER.PATH=[INFO.DATAPATH,'/SKT'];
    mkdir( DRIVER.PATH);

 for YR=INFO.START_YEAR:INFO.END_YEAR;
    DATA=DATA0;
         DATA.filename=sprintf('%s/CARDAMOM-MAPS_GC4x5_ERA5_SKT_%i.nc',    DRIVER.PATH,YR);
     DATA.Attribute.variable_info='ERA5 4x5 degree dataset: monthly average surface skin temp';
     DATA.info.units='deg C';
     %Populate with existing fields
    for M=1:12
         F=read_era_field_month_year(YR,M,DRIVER.NAME,ERA5_DATA_STATUS);

         %QC and/or QC corrections
         %1. Capping bottom values at zero
         F(F<0)=0;

        if isempty(F)==0

         %Converting from m/day to mm/day
          DATA.data(:,:,M)=GEOSChem_regular_grid_to_GC(F) +  DRIVER.CONVERSION_OFFSET;


    
        end
     
    end
    %Writing yearly dataset

CARDAMOM_MAPS_WRITE_3D_DATASET(DATA)

end




%**************************GFED BURNED AREA*************

%Note not low latency as others, need to address
     %Note: GFED already gridded to appropriate resolution

 GFED=quickload('CARDAMOM_MAPS_READ_GFED_JAN24(''GC4x5'')');







    DRIVER.PATH=[INFO.DATAPATH,'/BURNED_AREA'];
    mkdir( DRIVER.PATH);

 for YR=INFO.START_YEAR:INFO.END_YEAR;
    DATA=DATA0;
         DATA.filename=sprintf('%s/CARDAMOM-MAPS_GC4x5_ERA5_BURNED_AREA_%i.nc',    DRIVER.PATH,YR);
     DATA.Attribute.variable_info='GFED4.1s 4x5 degree dataset: monthly total burned area';
     DATA.info.units='m2/m2';
     %Populate with existing fields
     %Note: GFED already gridded to appropriate resolution
    for M=1:12
        gfedidx=find(GFED.month==M & GFED.year==YR);
        if isempty(gfedidx)==0
         F=GFED.BA(:,:,gfedidx);

         %QC and/or QC corrections
         %1. Capping bottom values at zero
         F(F<0)=0;

        

         %Converting from m/day to mm/day
          DATA.data(:,:,M)=F;


    
        end
     
    end
    %Writing yearly dataset

CARDAMOM_MAPS_WRITE_3D_DATASET(DATA)

end










    %Temperature min and max


%Max temp
%  for yr=startyear:endyear
%     DATA=DATA0;
%      DATA.filename=sprintf('%s/T2M_MAX/CARDAMOM-MAPS_GC4x5_ERA5_T2M_MAX_%i.nc',DATAPATH,yr);
%      DATA.data=GEOSChem_regular_grid_to_GC(ET2M.datamax(:,:,ET2M.year==yr));
%      DATA.Attribute.variable_info='ERA5 4x5 degree dataset: monthly average daily maximum temperature';
%      DATA.info.units='deg C';
%  
%       CARDAMOM_MAPS_WRITE_3D_DATASET(DATA)
%       
%           DATA=DATA0;
%      DATA.filename=sprintf('%s/T2M_MIN/CARDAMOM-MAPS_GC4x5_ERA5_T2M_MIN_%i.nc',DATAPATH,yr);
%      DATA.data=GEOSChem_regular_grid_to_GC(ET2M.datamin(:,:,ET2M.year==yr));
%      DATA.Attribute.variable_info='ERA5 4x5 degree dataset: monthly average daily minumum temperature';
%      DATA.info.units='deg C';
%  
%       CARDAMOM_MAPS_WRITE_3D_DATASET(DATA)
%  end
















 end



function F=read_era_field_month_year(YR,M,FNAME,STATUS)


yridx=YR-2000;

    efn=FNAME;
    efidx=find(strcmp(STATUS.fields,efn));

        if STATUS.FNAMES_PRESENT{yridx,M,efidx}>0
        filename=STATUS.FNAMES{yridx,M,efidx};

        %Step 1. read radiation file
        F=ncread(filename, efn);
        %Transposing

        F=permute(F,[2,1,3]);

        %flipping upside down
        F=flipud(F);
        else
            F=[];

        end




end






