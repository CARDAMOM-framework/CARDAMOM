%Step 1. Load GFAS data

GFAS=DATASCRIPT_READ_GFAS_DATA_JUN25;



%Step 2. Load burned area dataset 
ni=ncinfo('CARDAMOM-MAPS-DATASETS/CARDAMOM-MAPS_GC4x5_V2/BURNED_AREA/CARDAMOM-MAPS_GC4x5_ERA5_BURNED_AREA_2001.nc');

YEARS=2003:2021;
BA=[];
for yr=YEARS

    
    BA(:,:,[1:12]+(yr-2003)*12)=ncread(sprintf('CARDAMOM-MAPS-DATASETS/CARDAMOM-MAPS_GC4x5_V2/BURNED_AREA/CARDAMOM-MAPS_GC4x5_ERA5_BURNED_AREA_%04i.nc',yr),'data');
    
end

%Step 3. Regrid GFAS to GC grid and only keep 2003-2023

pts=GFAS.year>=2003 & GFAS.year<=2021;
GFASBA=GEOSChem_regular_grid_to_GC(GFAS.burned_area(:,:,pts));
GFASCext=GEOSChem_regular_grid_to_GC(GFAS.c_fluxes(:,:,end-35:end));
GFASC=GEOSChem_regular_grid_to_GC(GFAS.c_fluxes(:,:,pts));
GFASCfull=GEOSChem_regular_grid_to_GC(GFAS.c_fluxes);


%Step 4. Evaluate correlation between GFAS and BA

CMBA=correlate3d(GFASBA,BA);
CMC=correlate3d(GFASC,BA);


%Step 4. Calculate gradient zerop intercept
G=mean(BA,3)./mean(GFASC,3);
G(isinf(G))=0;


%Step 5. Extrapolate burned area
BAreconstruction=G.*GFASC;
BAext=G.*GFASCext;
BAfull=G.*GFASCfull;
BAfull(isnan(BAfull))=0;


%Step 6. 

fname='BURNED_AREA_EXTENSION_FOR_EB_JUN25.nc';delete(fname);
nccreate(fname,'BURNED_AREA','Dimensions',{'lat' 46 'lon' 72 'time' 264 })
nccreate(fname,'time','Dimensions',{ 'time' })
nccreate(fname,'lon','Dimensions',{ 'lon' })
nccreate(fname,'lat','Dimensions',{ 'lat' })

ncwrite(fname,'BURNED_AREA',BAfull);
ncwrite(fname,'time',1:264);
ncwriteatt(fname,'time','Units','Months Since and including Jan 2003')
ncwrite(fname,'lon',GC.x(1,:));
ncwrite(fname,'lat',GC.y(:,1));


%Step 7 evaluate


RM=  quickload('loadrivermasks(0.5)');
AMAMASK=RM(:,:,1)>0.5;
AMAGC=GEOSChem_regular_grid_to_GC(AMAMASK)  ;

figure(1);clf
subplot(3,1,1);hold on
BAnc=ncread(fname,'BURNED_AREA');
plot(mapsfun_3D2TS(BA,GC.area),'k')
plot(mapsfun_3D2TS(BAnc,GC.area),'r')
title('Global')
ylabel('Burned area [m2/m2]')

subplot(3,1,2);hold on
BAnc=ncread(fname,'BURNED_AREA');
plot(mapsfun_3D2TS(BA,GC.area.*AMAGC),'k')
plot(mapsfun_3D2TS(BAnc,GC.area.*AMAGC),'r')
title('Amazon')
ylabel('Burned area [m2/m2]')

subplot(3,1,3);hold on
BAnc=ncread(fname,'BURNED_AREA');
plot(mapsfun_3D2TS(BA,GC.area.*(GC.y>50)),'k');
plot(mapsfun_3D2TS(BAnc,GC.area.*(GC.y>50)),'r')
title('Northern High Latitudes');
ylabel('Burned area [m2/m2]')

legend('Burned area (as used by Bilir et al.)','Burned area reconstruction & extension')


for s=1:3
   subplot(3,1,s);
   set(gca,'xtick',6:12:264,'xticklabel',2003:2024)
   legend('Burned area (as used by Bilir et al.)','Burned area reconstruction & extension')

end




