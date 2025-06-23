function DATA=CARDAMOM_MAPS_DATASCRIPT_NASS_CMS_CONUS_CROP_MAP_JUN25
%Script description
%
%This script details the gridding of the 2015-2020 National Agricultural
%Statistics Service dataset for use within the CARDAMOM framework
%
%CARDAMOM models that are configured to integrate (via forcing or
%assimilation) crop C removals are compatible for use with this dataset
%
%
%Version 2.1


%stuff we need throughout
[x,y,A]=loadworldmesh(0.5);
[x05,y05,A05]=loadworldmesh(0.05);
ptslat=y05(:,1)>20 & y05(:,1)<60;
ptslon=x05(1,:)>-130 & x05(1,:)<-60;
A05conus=A05(ptslat,ptslon);

%CROPMAP=repmat(A05*0,[1,1,3]);
%Six years
CROPMAP=repmat(A05(ptslat ,ptslon)*0,[1,1,6]);

fname='DUMPFILES/PROJSCRIPT_NA_CMS_AGRICULTURE_CROP_MAP_APR25_DUMPDATA_V2.1.mat';

%Figure out why one NAN weight happens at        2418.   4428



%step 1. Load all datasets
CROP=read_crop_C_fluxes_NEW;
COUNTYSHAPES=shaperead('DATA/US_COUNTIES/us-county-boundaries.shp');
%Add name_nospace
for n=1:numel(COUNTYSHAPES); 
    COUNTYSHAPES(n).name_nospace=COUNTYSHAPES(n).name(COUNTYSHAPES(n).name~=' ' & COUNTYSHAPES(n).name~='.' & COUNTYSHAPES(n).name~='''' );    
    COUNTYSHAPES(n).name_nospace(COUNTYSHAPES(n).name_nospace=='ñ')='n';
end
STATESHAPES=shaperead('DATA/US_STATES/cb_2023_us_state_20m.shp');


if isfile(fname)

    load(fname,'CROPMAPGLOB');

else


%Goal: generate gridded agriculture dataset at 0.5 degree


statecountymaskfname='DUMPFILES/PROJSCRIPT_NA_CMS_AGRICULTURE_CROP_MAP_APR25_DUMPDATA_SCMASKS.mat';


if ~isfile(statecountymaskfname)
 COUNTYSHAPES=shaperead('DATA/US_COUNTIES/us-county-boundaries.shp');

save(statecountymaskfname,'COUNTYSHAPES','STATESHAPES')



%Step 0. Initialize data






%Make "other counties" state shape here


%Step 2. Rasterize C fluxes
%Loop through each entry
% 2. 1. Rasterize county contouts for each year
% 2.2. Load corresponding total C fluxes
% 2. 3. Distribute on 0.5 degree grid

%Loop through every state and year

%Step 1. For state gridding, rasterize states, and then normalize edges to
%include no double-counting

% 
% W=0;
% for s=1:52
%         disp(s)
%         %Step 1. Rasterize 
%         MR=vec2mtx(STATESHAPES(s).Y,STATESHAPES(s).X,1/0.05,[-90,90],[-180,180],'filled');
%         M=MR+1;M(M==3)=0;
%         W=W+M;
%         %Store vector 
%         STATEVECS(s).PTS=find(M>0);
% end
% %Now calculate weights for each one
% for s=1:52
%         disp(s)
%         %Step 1. Rasterize 
%         STATEVECS(s).WEIGHTS=1./W(STATEVECS(s).PTS);
% end


%Next, do this for all counties in each state, and then multiply the
%weights by state weight to ensure we don't doub overlapping countries from bordering
%states 


%save(statecountymaskfname,'COUNTYVECS','-append')

%******HAS NOT BEEN TESTED YET (NEED TO TEST FIRST)******
keyboard


%Load landcover frqactions
LCF=DATASCRIPT_READ_MODIS_MCD12C1;
%Croplands = 12
%Water = 1;
CROPMASK=double(LCF.data(ptslat,ptslon,12))/100;
LANDMASK=1-double(LCF.data(ptslat,ptslon,1))/100;


 W=0;Mc=0;Wc=0;
 %MOC=zeros(3600,7200);
    for c=1:numel(COUNTYSHAPES)
%38 15 Hawaii
% 39 01 Alabama
% 40 20 Kansas
% 41 44 Rhode Island
% 42 09 Connecticut
% 43 26 Michigan
% 44 02 Alaska
% 45 10 Delaware
% 46 35 New Mexico
% 47 72 Puerto Rico
%Excluding Alaska, Hawaii, Puerto Rico
        if ~strcmpi(COUNTYSHAPES(c).statefp,{'02','72','15'})

                    M=vec2mtx(COUNTYSHAPES(c).Y,COUNTYSHAPES(c).X,1/0.05,[20,60],[-130,-60],'filled');
                    %Mc=vec2mtx(COUNTYSHAPES(c).Y,COUNTYSHAPES(c).X,1/0.05,[-90,90],[-180,180]);
%Step 1. Store where the county points are
               COUNTYVECS(c).PTS=find(M<2);

               %Step 2. Store map of outer countours 
               %MOC(M==1 & MOC>=0)=MOC(M==1 & MOC>=0)+1;
              % MOC(MOC>1)=-1;
               
        W=W+double(M<2);
        %Wc=Wc+double(Mc==1);
        %end
                disp([c])
                %plotglobal(MOC);drawnow;
            %   if total(~isfinite(W))>0;keyboard;end
        end
    end


    keyboard


    %Now make a "continental contour", and double edge weights
    W0=W>0;
    WC=W0*0;
    for c=-1:1; for r=-1:1
            W1=circshift(W0,c,1);
            W1=circshift(W1,r,2);
            WC=WC+W1;
    end
    end


    CC=double((WC>=4) -(WC>=9))       ;
    
    CC=CC+1;%including all regions inside and outside outer contour
    CC(WC<4 & W==0)=0;%Explicitly excluding all regions outside outer contour where W==0
    W=W.*CC;

    


        

    % %Now add state weights to ensure we normalize borders with other states
    % W(STATEVECS(s).PTS)=W(STATEVECS(s).PTS)./STATEVECS(s).WEIGHTS;

    %Now multiply by 

    for c=1:numel(COUNTYSHAPES)

          %if strcmpi(COUNTYSHAPES(c).statefp,STATESHAPES(s).STATEFP)



        disp(sprintf("Normalizing %i",c))

        %Filitering any zero Weught 
        PTS=COUNTYVECS(c).PTS;


        %Step 1. Rasterize and multiply by sea area
        COUNTYVECS(c).WEIGHTS=(1./W(PTS)).*LANDMASK(PTS);
        %Step 2. Multiply by crop area if crop area>0, otherwise leave
        %weights intact



        if total(CROPMASK(PTS))>0
                    COUNTYVECS(c).WEIGHTS=COUNTYVECS(c).WEIGHTS.*CROPMASK(PTS);
        else
            disp(sprintf('No crop cover recorded in county %i',c))
        end
            
    end

save(statecountymaskfname,'COUNTYVECS','-append')
save(statecountymaskfname,'W','-append')
save(statecountymaskfname,'CC','-append')
    
%end
else

    load(statecountymaskfname,'COUNTYVECS','CC','W')


end

CCW05=x05*0;
CCW05(ptslat,ptslon)=1./(W);
CCW05(isinf(CCW05))=0;




%Approach 


keyboard
%******HAS NOT BEEN TESTED YET******



% %
%     for year=2015:2020
%         for s=1:52
% 
% 
% 
% 
%         %Step 1. Rasterize state
%         M=vec2mtx(STATESHAPES(s).Y,STATESHAPES(s).X,1/0.05,[-90,90],[-180,180],'filled');
% 
% 
%         %Step 2. Find all State counties with data that are NOT OTHER
%         pts=find(strcmpi(CROP.State,STATESHAPES(s).NAME) & CROP.Year==year & strcmp(CROP.County,'OTHER (COMBINED) COUNTIES')==false);
% 
%         %Step 3. Match counties with COUNTYSHAPES
% 
% 
% 
% 
%         x=[];y=[];
%         M=zeros(3600,7200);
%         for n=1:numel(pts)
%             disp(n)
%             [LON,LAT]=DATASCRIPT_READ_US_COUNTY_LONLAT(CROP.County{pts(n)}, STATESHAPES(s).NAME,COUNTYSHAPES);
% 
% 
%              M=M+(2 - vec2mtx(LAT,LON,1/0.05,[-90,90],[-180,180],'filled'));
%             x=[x,LON];
%             y=[y,LAT];
%         end
% 
% 
% 
% 
%         %Step 3. Take difference
% 
% 
% 
% 
%     end
% end
% 
% 





for n=1:CROP.nentries


    
    %Disp loop constant
    disp(n);


    if   strcmp(CROP.State  (n),'HAWAII')==false
    
    %Step 1. Find counties spanned by CROP reported entry
    %One or more county IDs
    CID=COUNTYMAP_IDS(CROP,n,COUNTYSHAPES);

    %Step 2. FInd CR

    %Step 1. Match county from CROP with county from COUNTYSHAPES 
    %COUNTYMAP=load_and_rasterize_county_data(CROP.County{n}, CROP.State{n}, COUNTYSHAPES,CROP);
    %Add fluxes to CROPMAP
    WEIGHTMAP=W*0;
    for m=1:numel(CID)
    WEIGHTMAP(COUNTYVECS(CID(m)).PTS)=WEIGHTMAP(COUNTYVECS(CID(m)).PTS)+COUNTYVECS(CID(m)).WEIGHTS;
    if isnan(sum(COUNTYVECS(CID(m)).WEIGHTS));disp(m);end
    end
    %Normalize WEIGHTS to redistribute, and re-weigh by area
    WEIGHTMAP=WEIGHTMAP.*A05conus/total(WEIGHTMAP.*A05conus);
    if total(isnan(WEIGHTMAP))>0;keyboard;end

    yi=CROP.Year(n)-2014;

    CROPMAP(:,:,yi)=CROPMAP(:,:,yi)+WEIGHTMAP*CROP.yield_C(n);

%        else 
        %Step 1. Find all counties not present in crop county state
%Do this by calling "other counties per state shape"
        %
    end





end



%Fit crop map into global grid
 CROPMAPGLOB=repmat(A05,[1,1,6])*0;
 CROPMAPGLOB(ptslat,ptslon,:)=CROPMAP;
%
save(fname,'CROPMAPGLOB');
end

%Units in tons
%Convert to gC/m2/day
%Also caculate totals for each year

%total CONUS
TOTS=squeeze(sum(sum(CROPMAPGLOB,2),1))*1e6/1e12;

%Turn into gC/m2/yr
CROPMAPGLOBgCm2d=CROPMAPGLOB*1e6./A05;

%Make figures
figure(1);clf;cmp=loadaudreycolors;zerowhitemap;
subplot(3,2,1);plotglobal(CROPMAPGLOBgCm2d(:,:,1));title('2015');ch=colorbar;ylabel(ch,'gC/m2/yr');caxis([0,200]);
subplot(3,2,2);plotglobal(CROPMAPGLOBgCm2d(:,:,2));title('2016');ch=colorbar;ylabel(ch,'gC/m2/yr');caxis([0,200]);
subplot(3,2,3);plotglobal(CROPMAPGLOBgCm2d(:,:,3));title('2017');ch=colorbar;ylabel(ch,'gC/m2/yr');caxis([0,200]);
subplot(3,2,4);plotglobal(CROPMAPGLOBgCm2d(:,:,4));title('2018');ch=colorbar;ylabel(ch,'gC/m2/yr');caxis([0,200]);
subplot(3,2,5);plotglobal(CROPMAPGLOBgCm2d(:,:,5));title('2019');ch=colorbar;ylabel(ch,'gC/m2/yr');caxis([0,200]);
subplot(3,2,6);plotglobal(CROPMAPGLOBgCm2d(:,:,6));title('2020');ch=colorbar;ylabel(ch,'gC/m2/yr');caxis([0,200]);


%FLUXCOM=quickload('DATASCRIPT_READ_FLUXCOM_DATASETS_MAY20');
%subplot(2,3,4);plotglobal(mean(FLUXCOM.GPP.data,3)*365.25);title('FLUXCOM mean GPP');ch=colorbar;ylabel(ch,'gC/m2/yr');caxis([0,2000]);


for s=1:6; subplot(3,2,s); %set(gca,'xtickmode','manual','ytickmode','manual');     
     xlim([-130,-60]);ylim([23,53]);tidyupmap(0,0);plotcountries; 

end




%subplot(2,2,4);plotglobal(CROPMAPGLOB(:,:,3)- CROPMAPGLOB(:,:,2));title('2018');


DATA.data=CROPMAPGLOBgCm2d;
DATA.years=[2015,2016,2017,2018,2019,2020];
DATA.description='Yield carbon removal annual, for 2015, 2016, 2017 and 2018, 2019, 2020';
DATA.units='gC/m2/day';


fname='TEMPDATA/CROP_DATA_V2.1_FOR_EB_APR25.nc';
delete(fname);
nccreate(fname,'CROP_YIELD_C','Dimensions',{'lat' size(DATA.data,1) 'lon' size(DATA.data,2) 'year' size(DATA.data,3)});
ncwrite(fname,'CROP_YIELD_C',DATA.data);
ncwriteatt(fname,'CROP_YIELD_C','description',DATA.description)
ncwriteatt(fname,'CROP_YIELD_C','units',DATA.units)




keyboard





end



function [idx]=FIND_US_COUNTY_ID(COUNTY_NAME,STATE_NAME,COUNTYSHAPES)


if isempty(COUNTYSHAPES)
f2='DATA/US_COUNTIES/us-county-boundaries.shp';
COUNTYSHAPES=shaperead(f2);
end




idx=find(contains({COUNTYSHAPES.name},COUNTY_NAME,'IgnoreCase',true) & contains({COUNTYSHAPES.state_name},STATE_NAME,'IgnoreCase',true));


if isempty(idx);disp('No find, removing spaces and trying again');

COUNTY_NAME_NS=COUNTY_NAME(COUNTY_NAME~=' ' & COUNTY_NAME~='.' & COUNTY_NAME~='''');
idx=find(contains({COUNTYSHAPES.name_nospace},COUNTY_NAME_NS,'IgnoreCase',true) & contains({COUNTYSHAPES.state_name},STATE_NAME,'IgnoreCase',true));


end



if isempty(idx);disp('No find, testing for "St." vs "SAINT"');
    if contains(COUNTY_NAME,'SAINT')

COUNTY_NAME_ST=COUNTY_NAME_NS;
    
COUNTY_NAME_ST=['St',COUNTY_NAME_NS(6:end)];
idx=find(contains({COUNTYSHAPES.name_nospace},COUNTY_NAME_ST,'IgnoreCase',true) & contains({COUNTYSHAPES.state_name},STATE_NAME,'IgnoreCase',true));

    end
end
%pts=( contains({STATESHAPES.state_name},STATE_NAME,'IgnoreCase',true))
%STATESHAPES(pts).name


%If still empty
if isempty(idx);error('county & state do not exist in database');end

% %S=STATESHAPES(idx);
% LON=STATESHAPES(idx).X;
% LAT=STATESHAPES(idx).Y;


end





function CROP=read_crop_C_fluxes_NEW

%Filename
fname='DATA/CMS/CROPS_PER_COUNTY_APR24/county_totals_2015-2020.csv';


%DATA/CMS/CROPS_PER_COUNTY_JAN23/county_totals_2016-2018.csv


RAWDATA=importdata(fname);

h=RAWDATA.textdata(1,:);


CROP.State=RAWDATA.textdata(2:end,1);
CROP.Agdistrict=RAWDATA.textdata(2:end,2);
CROP.County=RAWDATA.textdata(2:end,3);

%Turn CHesapeake city to Chesapeake
pts=find(contains(CROP.County,'Chesa','IgnoreCase',true));
for n=1:numel(pts);CROP.County{pts(n)}='CHESAPEAKE';end

%Turn Suffolk City to Suffolk
pts=find(contains(CROP.County,'Suffolk City','IgnoreCase',true));
for n=1:numel(pts);CROP.County{pts(n)}='Suffolk';end

%Turn Suffolk City to Suffolk
pts=find(contains(CROP.County,'Virginia Beach City','IgnoreCase',true));
for n=1:numel(pts);CROP.County{pts(n)}='Virginia Beach';end


for n=4:10
CROP.(h{n})=RAWDATA.data(:,n-3);
end

CROP.nentries=numel(CROP.County);



CROP.data_origin_id=CROP.yield_C*0+1;

%Loop through all crop entries, and add entries for missing years
%Version 2.1
N=CROP.nentries;
for n=1:N
    pts=strcmp(CROP.County{n}, CROP.County)  &  strcmp(CROP.State{n}, CROP.State);
    Years=(CROP.Year(pts));
    if numel(Years)<6';
        for y=2015:2020
            %Filling in with mean for missing years
            if isempty(find(Years==y))
                CROP.County{CROP.nentries+1}=CROP.County{n};
                CROP.State{CROP.nentries+1}=CROP.State{n};
                CROP.Agdistrict{CROP.nentries+1}=CROP.Agdistrict{n};
                CROP.yield_C(CROP.nentries+1)=mean(CROP.yield_C(pts));
                CROP.data_origin_id(CROP.nentries+1)=2;
                CROP.Year(CROP.nentries+1)=y;
                CROP.nentries=CROP.nentries+1;
            end
        end
    end
    


end



end



function CROP=read_crop_C_fluxes_OLD

%Filename
fname='DATA/CMS/CROPS_PER_COUNTY_JAN23/county_totals_2016-2018.csv';


%DATA/CMS/CROPS_PER_COUNTY_JAN23/county_totals_2016-2018.csv


RAWDATA=importdata(fname);

h=RAWDATA.textdata(1,:);


CROP.State=RAWDATA.textdata(2:end,1);
CROP.Agdistrict=RAWDATA.textdata(2:end,2);
CROP.County=RAWDATA.textdata(2:end,3);

%Turn CHesapeake city to Chesapeake
pts=find(contains(CROP.County,'Chesa','IgnoreCase',true));
for n=1:numel(pts);CROP.County{pts(n)}='CHESAPEAKE';end

%Turn Suffolk City to Suffolk
pts=find(contains(CROP.County,'Suffolk City','IgnoreCase',true));
for n=1:numel(pts);CROP.County{pts(n)}='Suffolk';end

%Turn Suffolk City to Suffolk
pts=find(contains(CROP.County,'Virginia Beach City','IgnoreCase',true));
for n=1:numel(pts);CROP.County{pts(n)}='Virginia Beach';end


for n=4:10
CROP.(h{n})=RAWDATA.data(:,n-3);
end

CROP.nentries=numel(CROP.County);




end



function CID=COUNTYMAP_IDS(CROP,N,COUNTYSHAPES)
%N=135;
COUNTY_NAME=CROP.County{N};
STATE_NAME=CROP.State{N};
YEAR=CROP.Year(N);

%All reporting under "Other (COMBINED) COUnties" or "NA" grouped into one
%catergory
if strcmp(COUNTY_NAME,'OTHER (COMBINED) COUNTIES') | strcmp(COUNTY_NAME,'OTHER COUNTIES') |  strcmp(COUNTY_NAME,'NA')

    %if strcmp(COUNTY_NAME,'NA'); disp(sprinf('COUNTY %i is reported as NA'));end
    %Find all counties not included in CROP
    %Note that this is done at "State" level, rather than "Agroregion"
    %level
% 

%****THIS STEP ASSUMES OTHER (COMBINED) COUNTIES only refers to excluded
%countis

% %Step 1 list of all counties in state
% pts_all_counties=find( contains({COUNTYSHAPES.state_name},STATE_NAME,'IgnoreCase',true));
% %Step 2. Find all counties that reported that year
% pts_crop_counties=find( contains(CROP.State,STATE_NAME,'IgnoreCase',true) & CROP.Year==YEAR);
% %Step 3. Make list of all counties not included in crop_counties
% CID=[];
% for n=1:numel(pts_all_counties);
% 
%     ID=find(contains(CROP.County(pts_crop_counties),COUNTYSHAPES(pts_all_counties(n)).name,'IgnoreCase',true));
%     %If County not listed in CROP reportings, then recording as UNLISTED
%     if    isempty(ID);CID=[CID, pts_all_counties(n)];end
% end
%****END OF STEP****

%***THIS STEPS REDISTRIBUTES "OTHER (COMBINED) COUNTIES" to all counties in
%state***
CID=find( contains({COUNTYSHAPES.state_name},STATE_NAME,'IgnoreCase',true));



else
CID=FIND_US_COUNTY_ID(COUNTY_NAME,STATE_NAME,COUNTYSHAPES);

end

end

function COUNTYMAP=load_and_rasterize_county_data(COUNTY_NAME, STATE_NAME,COUNTYSHAPES,CROP);


%Step 1. Load vectors





if strcmp(COUNTY_NAME,'OTHER (COMBINED) COUNTIES') |  strcmp(COUNTY_NAME,'OTHER COUNTIES') | strcmp(COUNTY_NAME,'NA')

%Step 1. find all other counties in state not listed in CROP
%Step 1.1 list of all counties in state
pts=( contains({COUNTYSHAPES.state_name},STATE_NAME,'IgnoreCase',true));
%All the elements that correspiond to that state
pts=( contains(CROP.State,STATE_NAME,'IgnoreCase',true));
%CROP counties
COUNTIES=CROP.County(pts)


% For 

%Step 2. Concattenate lats and lons
    %Self call

    
keyboard

else

[LON,LAT]=DATASCRIPT_READ_US_COUNTY_LONLAT(COUNTY_NAME, STATE_NAME,COUNTYSHAPES);


end


%Step 2. rasterize
%             pr=y(:,1)>ylims(1) & y(:,1)<ylims(2);
%             pc=x(1,:)>xlims(1) & x(1,:)<xlims(2);
%             ylims=minmax(y(pr,1));
%             xlims=minmax(x(1,pc));
%             xlims=[-80,-40];
%             ylims=[-25,10];
       xlims=[-130,-60];
            ylims=[20,60];
%                    xlims=[-180,180];
%             ylims=[-90,90];

            [sbmask,R]=vec2mtx(LAT,LON,1/0.5,ylims,xlims,'filled');
                        sbmask=2-sbmask;
            sbmask(sbmask>1)=1;    


COUNTYMAP.mask=sbmask./total(sbmask);





end