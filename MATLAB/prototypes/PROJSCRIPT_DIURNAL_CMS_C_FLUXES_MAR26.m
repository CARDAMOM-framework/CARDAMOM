function PROJSCRIPT_DIURNAL_CMS_C_FLUXES_MAR26

%Notes from JULY 2025
%
%Data is here: DATA/DATA_FROM_EREN/CMS_CONUS_JUL25/
%Spans 2001-2021
keyboard

AUX.cardamom_monthly_fluxfile_mean{1}='../DATA/DATA_FROM_EREN/CMS_CONUS/Outputmean_exp1redo5_MAR26.nc';
AUX.cardamom_monthly_fluxfile_unc{1}='../DATA/DATA_FROM_EREN/CMS_CONUS/Outputstd_exp1redo5_MAR26.nc';
AUX.destination_path{1}='../DUMPFILES/CARDAMOM_CONUS_DIURNAL_FLUXES_MAR26_V2_exp1redo5/';
AUX.x=ncread(AUX.cardamom_monthly_fluxfile_mean{1},'longitude');
AUX.y=ncread(AUX.cardamom_monthly_fluxfile_mean{1},'latitude');
AUX.y_era=ncread('../DATA/ERA5_CUSTOM/CONUS_2015_2020_DIURNAL/ECMWF_CARDAMOM_HOURLY_DRIVER_SKT_012015.nc','latitude');
AUX.x_era=ncread('../DATA/ERA5_CUSTOM/CONUS_2015_2020_DIURNAL/ECMWF_CARDAMOM_HOURLY_DRIVER_SKT_012015.nc','longitude');
AUX.lon_range=[ -124.7500  -65.2500];
AUX.lat_range=[24.7500   60.2500];



for e=1
%Step 1. load fluxes for each month
FLUXES=load_erens_cms_monthly_fluxes(AUX,e);
 

%Step 2. Need to figure out fires here:


%Step 3. loop through months to make diurnal files


YEARS=2015:2024;
 %Step 2. Write monthly GEOS-Chem fluxes
 for yr=YEARS
    for m=1:12
            disp(['Month, Year'])
            disp([m,yr])%
                midx=m+(yr-2001)*12;

        %Monthly fluxes


        

 
        %Write monthy fluxes and uncertainties here
        write_monthlyflux_to_geoschem_format(FLUXES.GPP(:,:,midx)*1e3/24/3600,FLUXES.GPPunc(:,:,midx)*1e3/24/3600,yr,m,AUX,'GPP',e);
        write_monthlyflux_to_geoschem_format(FLUXES.NBE(:,:,midx)*1e3/24/3600,FLUXES.NBEunc(:,:,midx)*1e3/24/3600,yr,m,AUX,'NBE',e);
        write_monthlyflux_to_geoschem_format(FLUXES.NEE(:,:,midx)*1e3/24/3600,FLUXES.NEEunc(:,:,midx)*1e3/24/3600,yr,m,AUX,'NEE',e);
        write_monthlyflux_to_geoschem_format(FLUXES.REC(:,:,midx)*1e3/24/3600,FLUXES.RECunc(:,:,midx)*1e3/24/3600,yr,m,AUX,'REC',e);
        write_monthlyflux_to_geoschem_format(FLUXES.FIR(:,:,midx)*1e3/24/3600,FLUXES.FIRunc(:,:,midx)*1e3/24/3600,yr,m,AUX,'FIR',e);

 
         %Step 3.1 Load GFED for that month
         %AUX uses data to get range
         CO2diurnal=load_gfed_diurnal_fields_05deg(m,yr,AUX);

        %Step 3.2 Load met drivers for that month
       [SSRD,SKT]=load_era5_diurnal_fields_new(m,yr);

       %Step 4. derive diurnal fields
        %Kg C/Km^2/sec convert from gC/m2/d
        %q10=1.4;
        GPPdiurnal=SSRD.*repmat(FLUXES.GPP(:,:,midx)./mean(SSRD,3),[1,1,size(SSRD,3)])*1e3/24/3600;
        %reco=q10.^(SKT-repmat(mean(SKT,3),[1,1,248]));
        RECdiurnal=SKT.*repmat(FLUXES.REC(:,:,midx)./mean(SKT,3),[1,1,size(SSRD,3)])*1e3/24/3600;
        %Fires
    %Diurnal fluxes

   

    %Diurnal fluxes
            FIRdiurnal3h=nan2zero(CO2diurnal.*repmat(FLUXES.FIR(:,:,midx)./mean(CO2diurnal,3),[1,1,size(CO2diurnal,3)])*1e3/24/3600);
            FIRdiurnal=repmat(FIRdiurnal3h,[1,1,3]);
            for n=1:3
                FIRdiurnal(:,:,n:3:end)=FIRdiurnal3h;
            end
%Fire is every three hours, need to truplicate


        NBEdiurnal=RECdiurnal-GPPdiurnal+FIRdiurnal;
        NEEdiurnal=RECdiurnal-GPPdiurnal;



        write_hourly_flux_to_geoschem_format(GPPdiurnal,yr,m,AUX,'GPP',e);
        write_hourly_flux_to_geoschem_format(RECdiurnal,yr,m,AUX,'REC',e);
        write_hourly_flux_to_geoschem_format(NBEdiurnal,yr,m,AUX,'NBE',e);
        write_hourly_flux_to_geoschem_format(NEEdiurnal,yr,m,AUX,'NEE',e);
        write_hourly_flux_to_geoschem_format(FIRdiurnal,yr,m,AUX,'FIR',e);
    
 

    end
 end

 
end


%years=2001:2022;

keyboard






 



end


%******new stuff*******


function check_monthly_bilir_values(AUX)


FLUXES=load_erens_cms_monthly_fluxes(AUX,1);

figure(1);clf;
loadaudreycolors;
zerowhitemap;
subplot(2,2,1);
plotglobal(FLUXES.NBEunc(:,:,7+23*12),AUX.x,AUX.y);caxis([-16,16]);title('NBE [July 2024]')
subplot(2,2,2);
plotglobal(FLUXES.GPP(:,:,7+23*12),AUX.x,AUX.y);caxis([-0.5,16]);title('GPP [July 2024]')
subplot(2,2,3);
plotglobal(FLUXES.REC(:,:,7+23*12),AUX.x,AUX.y);caxis([-0.5,16]);title('REC [July 2024]')
subplot(2,2,4);
plotglobal(FLUXES.FIR(:,:,7+23*12),AUX.x,AUX.y);caxis([-0.5,16]);title('FIR [July 2024]')

for s=1:4;
    subplot(2,2,s);
    ch=colorbar;
    ylabel(ch,'gC/m2/d')
    tidyupmap
end

%Check ucnertainties too

figure(2);clf;
loadaudreycolors;
zerowhitemap;
subplot(2,4,1);
plotglobal(FLUXES.NBEunc(:,:,7+23*12),AUX.x,AUX.y);caxis([-0.5,16]);title('NBE uncertainty [July 2024]')
subplot(2,4,2);
plotglobal(FLUXES.GPPunc(:,:,7+23*12),AUX.x,AUX.y);caxis([-0.5,16]);title('GPP uncertainty [July 2024]')
subplot(2,4,3);
plotglobal(FLUXES.RECunc(:,:,7+23*12),AUX.x,AUX.y);caxis([-0.5,16]);title('REC uncertainty [July 2024]')
subplot(2,4,4);
plotglobal(FLUXES.FIRunc(:,:,7+23*12),AUX.x,AUX.y);caxis([-0.5,16]);title('FIR uncertainty[July 2024]')

subplot(2,4,5);
plotglobal(FLUXES.NBEunc(:,:,7+23*12)<0,AUX.x,AUX.y);caxis([0,1]);title('NBE uncertainty <0 [July 2024]')
subplot(2,4,6);
plotglobal(FLUXES.GPPunc(:,:,7+23*12)<0,AUX.x,AUX.y);caxis([0,1]);title('GPP uncertainty<0 [July 2024]')
subplot(2,4,7);
plotglobal(FLUXES.RECunc(:,:,7+23*12)<0,AUX.x,AUX.y);caxis([0,1]);title('REC uncertainty<0 [July 2024]')
subplot(2,4,8);
plotglobal(FLUXES.FIRunc(:,:,7+23*12)<0,AUX.x,AUX.y);caxis([0,1]);title('FIR uncertainty<0 [July 2024]')



total(FLUXES.NBEunc<0)
total(FLUXES.GPPunc<0)
total(FLUXES.RECunc<0)
total(FLUXES.FIRunc<0)

for s=1:8;
    subplot(2,4,s);
    ch=colorbar;
    if s<=4
    ylabel(ch,'gC/m2/d')
    else
         ylabel(ch,'yes/no')
    end
    tidyupmap
end



end

function output_checks_cms_monthly_diurnal


m=9;
yr=2024;
%Read Sep fluxes
k=1;
for d=1:30
       ffile=sprintf('%s/DIURNAL/FIR/CARDAMOM/2024/09/%02i.nc',AUX.destination_path{1},d);
       gfile=sprintf('%s/DIURNAL/GPP/CARDAMOM/2024/09/%02i.nc',AUX.destination_path{1},d);
       nfile=sprintf('%s/DIURNAL/NBE/CARDAMOM/2024/09/%02i.nc',AUX.destination_path{1},d);
       neefile=sprintf('%s/DIURNAL/NEE/CARDAMOM/2024/09/%02i.nc',AUX.destination_path{1},d);
       rfile=sprintf('%s/DIURNAL/REC/CARDAMOM/2024/09/%02i.nc',AUX.destination_path{1},d);
        disp(gfile)
       
       FIR(:,:,(d-1)*24+[1:24])=ncread(ffile,'CO2_Flux');
       GPP(:,:,(d-1)*24+[1:24])=ncread(gfile,'CO2_Flux');
       REC(:,:,(d-1)*24+[1:24])=ncread(rfile,'CO2_Flux');
       NBE(:,:,(d-1)*24+[1:24])=ncread(nfile,'CO2_Flux');
       NEE(:,:,(d-1)*24+[1:24])=ncread(neefile,'CO2_Flux');


end


%Make plot for fires

c = 59; r = 25;


figure(1);clf
subplot(2,3,1);plot(squeeze(GPP(c,r,:)));title('GPP [Sep 2024]')
subplot(2,3,2);plot(squeeze(NEE(c,r,:)));title('NEE [Sep 2024]')
subplot(2,3,3);plot(squeeze(REC(c,r,:)));title('REC [Sep 2024]')
subplot(2,3,4);plot(squeeze(FIR(c,r,:)));title('FIR [Sep 2024]')
subplot(2,3,5);plot(squeeze(NBE(c,r,:)));title('NBE [Sep 2024]')
for s=1:5;
    subplot(2,3,s)
    ylabel('[gC/m2/d]');
end



%Now show all monthlky fluxes and check these are reasonable

figure(1);clf
for m=1:12
     nbe_monthly_file=sprintf('%s/MONTHLY/NBE/CARDAMOM/2024/%02i.nc',AUX.destination_path{1},m);
    %NBE=ncread(nbe_monthly_file,'CO2_flux');
    NBEUNC=ncread(nbe_monthly_file,'Uncertainty');
    lon=ncread(nbe_monthly_file,'longitude');
    lat=ncread(nbe_monthly_file,'latitude');
    caxis([-10,10])

    subplot(3,4,m);

    plotglobal(NBEUNC',lon,lat);
    colorbar

end






k=1;
for d=1:30
       % gfile=sprintf('%s/DIURNAL/NBE/CARDAMOM/2024/09/%02i.nc',AUX.destination_path{1},d);
       % NBE0(:,:,(d-1)*24+[1:24])=ncread(gfile,'CO2_Flux');
      gfile=sprintf('%s/DIURNAL/NBE/CARDAMOM/2024/09/%02i.nc',AUX.destination_path{1},d);
       NBE(:,:,(d-1)*24+[1:24])=ncread(gfile,'CO2_Flux');
end



%Check monthly values to make sure those are OK too
%CONTINUE FROM HERE

%Read monthly fluxes and double0







end


function [SSRD,SKT]=load_era5_diurnal_fields_new(m,yr)



%Assumes "DATA/ERA5/" regardless
sktfname=sprintf('../DATA/ERA5_CUSTOM/CONUS_2015_2020_DIURNAL/ECMWF_CARDAMOM_HOURLY_DRIVER_SKT_%02i%04i.nc',m,yr);
ssrdfname=sprintf('../DATA/ERA5_CUSTOM/CONUS_2015_2020_DIURNAL/ECMWF_CARDAMOM_HOURLY_DRIVER_SSRD_%02i%04i.nc',m,yr);


%Step 1. Download all data 

%DO this from inside matlab or in terminal window, works either way
inmat=0
if inmat==1
cd ../DATA/ERA5_CUSTOM/CONUS_2015_2020_DIURNAL/
python3 CARDAMOM/PYTHON/ECMWF/CUSTOM/DIURNAL_FIELDS_FOR_2015_2024_CONUS_RUN_JUL25.py
cd
end




%Reading that month's SKT and SSRD
SKT=ncread(sktfname,'skt');%K
SSRD=ncread(ssrdfname,'ssrd');%J/m2

%Re-orientating
SKT=flipud(permute(SKT,[2,1,3]));
SSRD=flipud(permute(SSRD,[2,1,3]));




% 
% 
% %pad with NaNs at top and bottom
% SRADraw(2:45,:,:)=ERA5ssrd.data;
% SRADraw(46,:,:)=NaN;
% SRADraw(1,:,:)=NaN;



end




function FLUXES_PATCHED=load_erens_cms_monthly_fluxes(AUX,e)

monthlyflux_file_mean=AUX.cardamom_monthly_fluxfile_mean{e};
monthlyflux_file_unc=AUX.cardamom_monthly_fluxfile_unc{e};

%Load CMS monthly fluxes prodced here
%DATASCRIPT_WRITE_CMS_CARDAMOM_OUTPUTS_JUN18
%ni=ncinfo(fname);
FLUXES.GPP=permute(ncread(monthlyflux_file_mean,'GPP'),[2,1,3]);
FLUXES.REC=permute(ncread(monthlyflux_file_mean,'Resp_eco'),[2,1,3]);
FLUXES.FIR=permute(ncread(monthlyflux_file_mean,'Fire'),[2,1,3]);
FLUXES.NEE=permute(ncread(monthlyflux_file_mean,'NEE'),[2,1,3]);
FLUXES.NBE=permute(ncread(monthlyflux_file_mean,'NBE'),[2,1,3]);

FLUXES.NEEunc=permute(ncread(monthlyflux_file_unc,'NEE'),[2,1,3]);
FLUXES.FIRunc=permute(ncread(monthlyflux_file_unc,'Fire'),[2,1,3]);
FLUXES.NBEunc=permute(ncread(monthlyflux_file_unc,'NBE'),[2,1,3]);
FLUXES.GPPunc=permute(ncread(monthlyflux_file_unc,'GPP'),[2,1,3]);
FLUXES.RECunc=permute(ncread(monthlyflux_file_unc,'Resp_eco'),[2,1,3]);
%NBE=permute(ncread(monthlyflux_file,'NBE'),[2,1,3]);
%Bias corrected NBEbc
% NBE=permute(ncread(monthlyflux_file,'NBE'),[2,1,3]);
% NEE=permute(ncread(monthlyflux_file,'NEE'),[2,1,3]);




LSM=loadlandseamask(0.5);
[x,y]=loadworldmesh(0.5);

%Load landseamask

cpts=x(1,:)>=min(AUX.x) & x(1,:)<=max(AUX.x);
rpts=y(:,1)>=min(AUX.y) & y(:,1)<=max(AUX.y);
LSMconus=LSM(rpts,cpts);
xconus=x(rpts,cpts);
yconus=y(rpts,cpts);

%Wr F = scatteredInterpolant(x,y,v)

IFM=mean(FLUXES.GPP+FLUXES.REC + FLUXES.FIR + FLUXES.NEE + FLUXES.NBE,3);

pts=isfinite(IFM) & LSMconus>0 ;

ptspatch=(~isfinite(IFM)) & LSMconus>0 ;

fluxnames=fieldnames(FLUXES);

N=size(FLUXES.GPP,3);
for f=1:numel(fluxnames)
    fn=fluxnames{f};

    
    for n=1:N
        FLUX_PATCHED=FLUXES.(fn)(:,:,n);
         F = scatteredInterpolant(xconus(pts),yconus(pts),FLUX_PATCHED(pts),'linear','nearest');
         FLUX_PATCHED(ptspatch)=F(xconus(ptspatch),yconus(ptspatch));
     
        FLUXES_PATCHED.(fn)(:,:,n)=FLUX_PATCHED;

    end


end


 %    GPPjul2016=GPP(:,:,7+15*12);
 % 
 % 
 % GPPjul2016(ptspatch)=F(xconus(ptspatch),yconus(ptspatch));
 % 

 %Test plot
% figure(1);clf
% subplot(2,2,1);
% plotglobal(GPP(:,:,7+15*12),AUX.x,AUX.y);
% title('July 2016 GPP with gaps')
% caxis([-2,15])
% subplot(2,2,2);
% plotglobal(GPPjul2016,AUX.x,AUX.y);
% caxis([-2,15]);title('July 2016 GPP patched')

%Next step patch up all fields all times




%address zeros here

    FLUXES_PATCHED.NBEunc(FLUXES_PATCHED.NBEunc<0)=0;
    FLUXES_PATCHED.GPPunc(FLUXES_PATCHED.GPPunc<0)=0;
    FLUXES_PATCHED.RECunc(FLUXES_PATCHED.RECunc<0)=0;
    FLUXES_PATCHED.FIRunc(FLUXES_PATCHED.FIRunc<0)=0;
    FLUXES_PATCHED.NEEunc(FLUXES_PATCHED.NEEunc<0)=0;

    FLUXES_PATCHED.GPP(FLUXES_PATCHED.GPP<0)=0;
    FLUXES_PATCHED.REC(FLUXES_PATCHED.REC<0)=0;
    FLUXES_PATCHED.FIR(FLUXES_PATCHED.FIR<0)=0;



end




function  write_hourly_flux_to_geoschem_format(FLUX,yr,m,AUX,fluxname,e)
%Auxiliary data

dpath=[AUX.destination_path{e},'DIURNAL/'];
if isempty(dir(dpath));mkdir(dpath);end
dpath=[dpath,fluxname,'/'];
if isempty(dir(dpath));mkdir(dpath);;end
dpath=[dpath,'CARDAMOM/'];
if isempty(dir(dpath));mkdir(dpath);;end

%
ypath=sprintf('%s%04i/',dpath,yr);
if isempty(dir(ypath));mkdir(ypath);end
mpath=sprintf('%s%02i/',ypath,m);
if isempty(dir(mpath));mkdir(mpath);end

%TO DO HERE: include full file name and directory

% Step 1. how many days in month
for d=1:eomday(yr,m)
    %indices
    idx=[1:24]+(d-1)*24;
    %make netcdf file here
    %DATASCRIPT_WRITE_NETCDF_DATA
    %dataset)
    DATA.data=nan2zero(FLUX(:,:,idx));
    DATA.x=AUX.x;
    DATA.y=AUX.y;
    DATA.filename=sprintf('%s%02i.nc',mpath,d);
    DATA.t=0.5:23.5;
    DATA.timeunits='hour';
    DATA.info.name='CO2_Flux';
    DATA.info.units='Kg C/Km^2/sec';
    DATA.Attributes.Title=sprintf('3-hourly %s',fluxname);
    DATA.Attributes.Creation_date=date;
    DATA.Attributes.Version='CARDAMOM diurnal fluxes version 4 (CONUS)';
    DATA.Attributes.Description='CARDAMOM mean monthly RECO and GPP downscaled estimates';
    DATA.Attributes.Details='Missing fluxes patched with linear interpolation';
    DATA.Attributes.Contact='Anthony Bloom (abloom at jpl.nasa.gov)';
    OPT.precision='NC_FLOAT';
    DATASCRIPT_WRITE_NETCDF_DATA(DATA,OPT);
    
    %Fdiurnal=append3d(Fdiurnal,ncread(fname,'CO2_Flux'));
end


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


end

function  write_monthlyflux_to_geoschem_format(FLUX,UNC,yr,m,AUX,fluxname,e)




dpath=[AUX.destination_path{e},'MONTHLY/'];
if isempty(dir(dpath));mkdir(dpath);end
dpath=[dpath,fluxname,'/'];
if isempty(dir(dpath));mkdir(dpath);end
dpath=[dpath,'CARDAMOM/'];
if isempty(dir(dpath));mkdir(dpath);end

%
ypath=sprintf('%s%04i/',dpath,yr);
if isempty(dir(ypath));mkdir(ypath);end


%TO DO HERE: include full file name and directory

    fullfilename=sprintf('%s%02i.nc',ypath,m);
    if ~isfile(fullfilename)
    % Step 1. how many days in month
    %indices
    %make netcdf file here
    %DATASCRIPT_WRITE_NETCDF_DATA
    %dataset)
    uncfac=nan2zero(UNC./abs(FLUX));
    uncfac(FLUX==0)=0;
    DATA.data(:,:,:,1)=nan2zero(FLUX);
    DATA.data(:,:,:,2)=uncfac;
    DATA.x=AUX.x;
    DATA.y=AUX.y;
    DATA.filename=fullfilename;
    DATA.t=1;
    DATA.timeunits='N/A';
    DATA.info(1).name='CO2_Flux';
    DATA.info(2).name='Uncertainty';
    DATA.info(1).units='Kg C/Km^2/sec';
    DATA.info(2).units='factor';
    DATA.Attributes.Title=sprintf('Mean monthly %s',fluxname);
    DATA.Attributes.Creation_date=date;
    DATA.Attributes.Version='CARDAMOM diurnal fluxes version 3.1';
    DATA.Attributes.Description='CARDAMOM mean monthly RECO and GPP downscaled estimates';
    DATA.Attributes.NBE_NEE_scaling='Annual NBE was detrended and scaled to roughly equate to GCP land sink; additional statistical perturbations were added to NEE and NBE values to ensure uncertainty factors <=20 (CMS-Flux framework requirement).. As a result, NBE and NEE are approximately (but not always exactly) equal to the sum of constituent fluxes';
    DATA.Attributes.Contact='Anthony Bloom (abloom at jpl.nasa.gov)';
    OPT.precision='NC_FLOAT';
    DATASCRIPT_WRITE_NETCDF_DATA(DATA,OPT);
    end
    %Fdiurnal=append3d(Fdiurnal,ncread(fname,'CO2_Flux'));


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


end




%Based on DATASCRIPT_READ_GFED4_DATA_MAY16
function CO2diurnal=load_gfed_diurnal_fields_05deg(m,yr, AUX)

%Auxiliarty
[x025,y025,~,A025x025]=loadworldmesh(0.25);
[x05,y05]=loadworldmesh(0.5);
ptsx=find(x05(1,:)>=AUX.lon_range(1) & x05(1,:)<=AUX.lon_range(2));
ptsy=find(y05(:,1)>=AUX.lat_range(1) & y05(:,1)<=AUX.lat_range(2));




%Same as DATASCRIPT_READ_GFED4_DATA_MAR16
%But reading 1-year at a time *(YEAR)
%DDF: Flag for requesting daily and diurnal fractions
%Also provides "DAILY" option
%2010Nov-May2011
%http://www.falw.vu/~gwerf/GFED/GFED4/
%
EF=emission_factors;
%GFED4.1s_2005.hdf5

gfedfilepath='../DATA/GFED4/';


% # SAVA: Savanna, grassland, and shrubland fires	     
% # BORF: Boreal forest fires                          
% # TEMF: Temperate forest fires                       
% # DEFO: Tropical deforestation & degradation         
% # PEAT: Peat fires	                             
% # AGRI: Agricultural waste burning  

 GF4.types={'SAVA' ,    'BORF'  ,   'TEMF'   ,  'DEFO' ,   'PEAT'  ,   'AGRI'};
 
 if yr<2017;beta='';else beta='_beta';end
     gfedfnformat='GFED4.1s_';
 
    filename=sprintf('%s%s%4i%s.hdf5',gfedfilepath,gfedfnformat,yr,beta);
    hdf5info(filename);
    disp(yr)
    
%Looping through each month   
%Start/End date
stdat=datenum(sprintf('%02i/01/%4i',m,yr));
endat=stdat+eomday(yr,m)-1;
        
        
%Declare arrays

GF4.DMF=zeros(        720,      1440,6);
%All gridded to GC grid
GF4.ES=zeros(   360    , 720  , (endat-stdat+1) *8,4);

       
            %DM field name
            
             %Dry matter (mass)
            
            %**************daily and diurnal fraction*************
                for n=1:(endat-stdat+1);GF4.daily_frac(:,:,n)=flipud(hdf5read(filename,sprintf('/emissions/%02i/daily_fraction/day_%i',m,n))');end
                %Daily and diurnal fraction
                for n=1:8;GF4.diurnal_frac(:,:,n)=flipud(hdf5read(filename,sprintf('/emissions/%02i/diurnal_cycle/UTC_%i-%ih',m,(n-1)*3,n*3))');end
                %Daily fraction 

            
        for t=1:6    
        %dry matter combusted per field
        field=sprintf('/emissions/%02i/partitioning/DM_%4s',m,GF4.types{t});
        %Store dry matter combusted
        GF4.DMF(:,:,t)=flipud(hdf5read(filename,field)');

        %GF4.DM1x1(:,:,idx,t)=grid_to_1x1(GF4.DM);
        %emission for each species
%        for s=1:3;        GF4.ES(:,:,idx,t,s)=GF4.DM(:,:,idx,t)*EF.SP(s,t);end
        %CONTINUE FROM HERE
%     GF4.year=yr;
%     GF4.month=m;

        end
        
    GF4.units_emissions='g species m-2 month-1';
    GF4.units_DM='kg DM m-2 month';
        

    


%GF4.efac=EF.SP;
%GF4.efac_unc=EF.SPU;

GF4.species={'CO2','CO','CH4','C'};


%CARDAMOM = total C
%GFED = dry matter fraction per fire type
%Only thing to do is normalize by total C and multiply  byCARDAMOM total C 

%Grid to 4x5
GF4.DMF=mapsfun_coarsen_resolution(GF4.DMF,A025x025,2);
GF4.daily_frac=mapsfun_coarsen_resolution(GF4.daily_frac,A025x025,2);
GF4.diurnal_frac=mapsfun_coarsen_resolution(GF4.diurnal_frac,A025x025,2);
%Normalize to 1
GF4.DMF=GF4.DMF./repmat(sum(GF4.DMF,3),[1,1,6]);
GF4.daily_frac=GF4.daily_frac./repmat(sum(GF4.daily_frac,3),[1,1,size(GF4.daily_frac,3)]);
GF4.diurnal_frac=GF4.diurnal_frac./repmat(sum(GF4.diurnal_frac,3),[1,1,size(GF4.diurnal_frac,3)]);

%export summary emissions ONLY
for d=1:(endat-stdat+1)
    for h=1:8
         for s=1:4;        
             for t=1:6; 
                 %disp([d,h,s,t]);
                 idx=h+(d-1)*8;
                 %Emissions per unit C
                 GF4.ES(:,:,idx,s)=GF4.ES(:,:,idx,s)+GF4.DMF(:,:,t)*EF.SP(s,t).*GF4.daily_frac(:,:,d).*GF4.diurnal_frac(:,:,h)./EF.SP(4,t);
             end;
         end
    end
end

%Export CO2 diurnal only
%normalize externally
CO2diurnal=GF4.ES(ptsy,ptsx,:,1);

%keyboard


%Normalize each pixel by total C and multiply by FIR
%Maybe do externally
% for s=1:4;
%     GF4.ES(:,:,:,s)=GF4.ES(:,:,:,s).*repmat(FIR(:,:,sum(GF4.ES(:,:,:,4),3)
% 

%Next:
%(a) convert all DM to C



end




%***************end of new scrpts*******








%Based on DATASCRIPT_READ_GFED4_DATA_MAY16
function CO2diurnal=load_gfed_diurnal_fields(m,yr)
%Same as DATASCRIPT_READ_GFED4_DATA_MAR16
%But reading 1-year at a time *(YEAR)
%DDF: Flag for requesting daily and diurnal fractions
%Also provides "DAILY" option
%2010Nov-May2011
%http://www.falw.vu/~gwerf/GFED/GFED4/
%
EF=emission_factors;
%GFED4.1s_2005.hdf5

gfedfilepath='../DATA/GFED4/';


% # SAVA: Savanna, grassland, and shrubland fires	     
% # BORF: Boreal forest fires                          
% # TEMF: Temperate forest fires                       
% # DEFO: Tropical deforestation & degradation         
% # PEAT: Peat fires	                             
% # AGRI: Agricultural waste burning  

 GF4.types={'SAVA' ,    'BORF'  ,   'TEMF'   ,  'DEFO' ,   'PEAT'  ,   'AGRI'};
 
 if yr<2017;beta='';else beta='_beta';end
     gfedfnformat='GFED4.1s_';
 
    filename=sprintf('%s%s%4i%s.hdf5',gfedfilepath,gfedfnformat,yr,beta);
    hdf5info(filename);
    disp(yr)
    
%Looping through each month   
%Start/End date
stdat=datenum(sprintf('%02i/01/%4i',m,yr));
endat=stdat+eomday(yr,m)-1;
        
        
%Declare arrays

GF4.DMF=zeros(        720,      1440,6);
%All gridded to GC grid
GF4.ES=zeros(   46    , 72  , (endat-stdat+1) *8,4);

       
            %DM field name
            
             %Dry matter (mass)
            
            %**************daily and diurnal fraction*************
                for n=1:(endat-stdat+1);GF4.daily_frac(:,:,n)=flipud(hdf5read(filename,sprintf('/emissions/%02i/daily_fraction/day_%i',m,n))');end
                %Daily and diurnal fraction
                for n=1:8;GF4.diurnal_frac(:,:,n)=flipud(hdf5read(filename,sprintf('/emissions/%02i/diurnal_cycle/UTC_%i-%ih',m,(n-1)*3,n*3))');end
                %Daily fraction 

            
        for t=1:6    
        %dry matter combusted per field
        field=sprintf('/emissions/%02i/partitioning/DM_%4s',m,GF4.types{t});
        %Store dry matter combusted
        GF4.DMF(:,:,t)=flipud(hdf5read(filename,field)');

        %GF4.DM1x1(:,:,idx,t)=grid_to_1x1(GF4.DM);
        %emission for each species
%        for s=1:3;        GF4.ES(:,:,idx,t,s)=GF4.DM(:,:,idx,t)*EF.SP(s,t);end
        %CONTINUE FROM HERE
%     GF4.year=yr;
%     GF4.month=m;

        end
        
    GF4.units_emissions='g species m-2 month-1';
    GF4.units_DM='kg DM m-2 month';
        

    


%GF4.efac=EF.SP;
%GF4.efac_unc=EF.SPU;

GF4.species={'CO2','CO','CH4','C'};


%CARDAMOM = total C
%GFED = dry matter fraction per fire type
%Only thing to do is normalize by total C and multiply  byCARDAMOM total C 

%Grid to 4x5
GF4.DMF=GEOSChem_regular_grid_to_GC(GF4.DMF);
GF4.daily_frac=GEOSChem_regular_grid_to_GC(GF4.daily_frac);
GF4.diurnal_frac=GEOSChem_regular_grid_to_GC(GF4.diurnal_frac);
%Normalize to 1
GF4.DMF=GF4.DMF./repmat(sum(GF4.DMF,3),[1,1,6]);
GF4.daily_frac=GF4.daily_frac./repmat(sum(GF4.daily_frac,3),[1,1,size(GF4.daily_frac,3)]);
GF4.diurnal_frac=GF4.diurnal_frac./repmat(sum(GF4.diurnal_frac,3),[1,1,size(GF4.diurnal_frac,3)]);

%export summary emissions ONLY
for d=1:(endat-stdat+1)
    for h=1:8
         for s=1:4;        
             for t=1:6; 
                 %disp([d,h,s,t]);
                 idx=h+(d-1)*8;
                 %Emissions per unit C
                 GF4.ES(:,:,idx,s)=GF4.ES(:,:,idx,s)+GF4.DMF(:,:,t)*EF.SP(s,t).*GF4.daily_frac(:,:,d).*GF4.diurnal_frac(:,:,h)./EF.SP(4,t);
             end;
         end
    end
end

%Export CO2 diurnal only
%normalize externally
CO2diurnal=GF4.ES(:,:,:,1);

%keyboard


%Normalize each pixel by total C and multiply by FIR
%Maybe do externally
% for s=1:4;
%     GF4.ES(:,:,:,s)=GF4.ES(:,:,:,s).*repmat(FIR(:,:,sum(GF4.ES(:,:,:,4),3)
% 

%Next:
%(a) convert all DM to C



end








function  [SRAD,SKT]=load_erai_diurnal_fields(yr,m,source_path)
%ERA-interim met downloaded using ...


%Step 1. define file names
srad_fc_filename=sprintf('%sERAI_05x05_srad_fc_%4i_%02i.nc',source_path,yr,m);
skt_fc_filename=sprintf('%sERAI_05x05_skt_fc_%4i_%02i.nc',source_path,yr,m);
skt_an_filename=sprintf('%sERAI_05x05_skt_%4i_%02i.nc',source_path,yr,m);

%Step 2. load filenames
SRAD=ncread(srad_fc_filename,'ssrd');
SKTfc=ncread(skt_fc_filename,'skt');
SKTan=ncread(skt_an_filename,'skt');

%Step 3. Arrange datasets int standard GEOS-Chem grid
ntsteps=eomday(yr,m)*8;
SKT(:,:,1:2:ntsteps)=SKTan;
SKT(:,:,2:2:ntsteps)=SKTfc;
%SRAD2(1:4:end)=SRAD(1:4:end);
for t=4:-1:2;    SRAD(:,:,t:4:end)=SRAD(:,:,t:4:end)-SRAD(:,:,t-1:4:end);end

SRAD=GEOSChem_regular_grid_to_GC(GRIDSCRIPT_ERAI_HALFRES_REGRID(SRAD));
SKT=GEOSChem_regular_grid_to_GC(GRIDSCRIPT_ERAI_HALFRES_REGRID(SKT));

%Step 4. 






end






function [SRAD,SKT]=load_era5_diurnal_fields_old(yr,m,era5_sourcepath)

    %Assumes "DATA/ERA5/" regardless
OPT.storepath=era5_sourcepath;


%For testing only
ERA5ssrd=DATASCRIPT_READ_ERA5_DIURNAL_3H_DEC20('ssrd',[0.5,0.5],yr,m,OPT);
ERA5skt=DATASCRIPT_READ_ERA5_DIURNAL_3H_DEC20('skt',[0.5,0.5],yr,m,OPT);


%Setting 
SRADraw=GEOSChem_regular_grid_to_GC(ERA5ssrd.data);
SKTraw=GEOSChem_regular_grid_to_GC(ERA5skt.data);
% 
% 
% %pad with NaNs at top and bottom
% SRADraw(2:45,:,:)=ERA5ssrd.data;
% SRADraw(46,:,:)=NaN;
% SRADraw(1,:,:)=NaN;

SKT=SKTraw(:,:,2:end)/2+SKTraw(:,:,1:end-1)/2;

SRADraw(:,:,1)=0;
SRAD=SRADraw(:,:,2:end);
for t=2:8;SRAD(:,:,t:8:end)=SRADraw(:,:,t+1:8:end)-SRADraw(:,:,t:8:end-1);end
%convert to J/m2/d
SRAD=SRAD*1e6;
%convert to kelvin
SKT=SKT+273.15;

end




function [SRAD,SKT]=load_era5_diurnal_fields(yr,m,merra2_sourcepath)





end





function [SRAD3h,SKT3h]=load_merra2_diurnal_fields(yr,m,merra2_sourcepath)

    %Assumes "DATA/ERA5/" regardless



%Full path name and file list

metfilepath=sprintf('%s%4i/%02i/', merra2_sourcepath,yr,m);

files=dir([metfilepath,'*A1*.nc4']);
N=numel(files);


%Next step, read all files and compile SRAD and SKT

SKT1h=zeros(46,72,24*N);
SRAD1h=zeros(46,72,24*N);

for n=1:N
    idx=[1:24]+(n-1)*24;
    SKT1h(:,:,idx)=permute(ncread([files(n).folder,'/',files(n).name],   'TS'),[2,1,3]);
    SRAD1h(:,:,idx)=permute(ncread([files(n).folder,'/',files(n).name],   'SWGDN'),[2,1,3]);
end



% 
% 
% %pad with NaNs at top and bottom
% SRADraw(2:45,:,:)=ERA5ssrd.data;
% SRADraw(46,:,:)=NaN;
% SRADraw(1,:,:)=NaN;


   %units          = 'W m-2'
%convert to J/m2/d
SRAD1h=SRAD1h*24*3600;

%Three hourly for now
SRAD3h=SRAD1h(:,:,2:3:end);
SKT3h=SKT1h(:,:,2:3:end);


end



function  write_nee_fires_to_geoschem_format(NEEmean,NEEvar,FIRES, yr,m,destination_path)
%Auxiliary data
GC=GEOSChem_xygrids;


%
if isempty(dir(destination_path));mkdir(destination_path);;end
ypath=sprintf('%s%04i/',destination_path,yr);
if isempty(dir(ypath));mkdir(ypath);end
mpath=sprintf('%s%02i/',ypath,m);
if isempty(dir(mpath));mkdir(mpath);end

% Step 1. how many days in month
for d=1:eomday(yr,m)
    %indices
    idx=[1:8]+(d-1)*8;
    %make netcdf file here
    %DATASCRIPT_WRITE_NETCDF_DATA
    %dataset)
    DATA.data(:,:,:,1)=nan2zero(NEEmean(:,:,idx));
    DATA.data(:,:,:,2)=nan2zero(NEEvar(:,:,idx));
    DATA.data(:,:,:,3)=nan2zero(FIRES(:,:,idx));
    DATA.x=GC.x(1,:);
    DATA.y=GC.y(:,1);
    DATA.filename=sprintf('%s%02i.nc',mpath,d);
    DATA.t=1.5:3:24;
    DATA.timeunits='hour';
    DATA.info(1).name='CO2_Flux_NEEmean';
    DATA.info(2).name='CO2_Flux_NEEanomaly';
    DATA.info(3).name='CO2_Flux_FIRES';
    DATA.info(1).units='Kg C/Km^2/sec';
    DATA.info(2).units='Kg C/Km^2/sec';
    DATA.info(3).units='Kg C/Km^2/sec';
    DATA.Attributes.Title='3 hourly NEE and fires';
    DATA.Attributes.Creation_date=date;
    DATA.Attributes.Version='CARDAMOM diurnal fluxes version 1.3.2';
    DATA.Attributes.Description='CARDAMOM mean monthly RECO and GPP downscaled estimates';
    DATA.Attributes.Flux_info='For timestep t, NBE(t)  = CO2_Flux_NEEmean  + CO2_Flux_NEEanomaly(t) + CO2_Flux_FIRES(t), where "NEEmean" is the mean monthly NEE flux';
    DATA.Attributes.Contact='Anthony Bloom (abloom at jpl.nasa.gov)';
    OPT.precision='NC_FLOAT';
    DATASCRIPT_WRITE_NETCDF_DATA(DATA,OPT);
    
    %Fdiurnal=append3d(Fdiurnal,ncread(fname,'CO2_Flux'));
end


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


end




function [FIR,FIRunc,GPP,GPPunc,NBE,NBEunc,NEE,NEEunc,REC,RECunc]=load_cms_monthly_fluxes(monthlyflux_file)

%Load CMS monthly fluxes prodced here
%DATASCRIPT_WRITE_CMS_CARDAMOM_OUTPUTS_JUN18
%ni=ncinfo(fname);
GPP=permute(ncread(monthlyflux_file,'GPP'),[2,1,3]);
REC=permute(ncread(monthlyflux_file,'Resp_eco'),[2,1,3]);
FIR=permute(ncread(monthlyflux_file,'FIRE'),[2,1,3]);
NEEunc=permute(ncread(monthlyflux_file,'NEE_uncertainty'),[2,1,3]);
FIRunc=permute(ncread(monthlyflux_file,'FIRE_uncertainty'),[2,1,3]);
NBEunc=permute(ncread(monthlyflux_file,'NBE_uncertainty'),[2,1,3]);
GPPunc=permute(ncread(monthlyflux_file,'GPP_uncertainty'),[2,1,3]);
RECunc=permute(ncread(monthlyflux_file,'Reco_uncertainty'),[2,1,3]);
%NBE=permute(ncread(monthlyflux_file,'NBE'),[2,1,3]);
%Bias corrected NBEbc
NBE=permute(ncread(monthlyflux_file,'NBEbc'),[2,1,3]);
NEE=permute(ncread(monthlyflux_file,'NEE'),[2,1,3]);

end



function EF=emission_factors
    
    
    %Emission factors (recommended for GFEDv4)
    %http://www.falw.vu/~gwerf/GFED/GFED4/ancill/. 
% DM                  1000     1000     1000     1000     1000     1000 
% C                488.273  464.989  489.416  491.751  570.055  480.352 
EF.type={'SAVA','BORF ','TEMF','DEFO','PEAT','AGRI'};
%Species emission factors (based on EXCEL spreadsheet, NOT online text
%file).
%1703

EF.SP(1,:)=[1686,	1489,	1647,	1643,	1703,	1585];
EF.SP(2,:)=[63	,127	,88	93	,210	102];
EF.SP(3,:)=[1.94,	5.96	,3.36	,5.07	,20.8,	5.82];
EF.SP(4,:)=[           488.273  464.989  489.416  491.751  570.055  480.352 ];


%Uncertainties:
EF.SPU(1,:)=[38,	121,	37,	58,	58	,100];
EF.SPU(2,:)=[17	,45,	19,	27,	68,	33];
EF.SPU(3,:)=[0.85,	3.14	,0.91,	1.98,	11.42,	3.56];
EF.SPU(4,:)=[NaN NaN NaN NaN NaN NaN];






EF.species={'CO2','CO','CH4','C'};
EF.units='g species kg-1 dry matter';



end

