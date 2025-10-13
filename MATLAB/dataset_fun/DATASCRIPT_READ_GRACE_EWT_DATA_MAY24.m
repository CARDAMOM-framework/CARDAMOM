function GRAV=DATASCRIPT_READ_GRACE_EWT_DATA_MAY24
%This script grids the GRACE data to comply with CARDAMOM global and
%regional analyses formats and conventions

%Now pointing to latest MASCONS dataset (from Elahe Tajfar)
%To rerun this script on your local machine, you will need to adjust these
%paths
gracefiles='DATA/GRACE/MASCONS/GRCTellus.JPL.200204_202401.GLO.RL06.1M.MSCNv03CRI 1.nc';
%This script does the following:
%   - Reads GRACE data and stores in matlab structure
%   - Interpolates GRACE data to generate an envenly spaced monthly product
%   - Derived the derivative (dEWT/dt) and also stores on same grid




%Scale factor
scalefac=ncread(gracefiles,'scale_factor')';

    GRAV.data=permute(ncread(gracefiles,'lwe_thickness'),[2,1,3]);
    GRAV.data=GRAV.data.*repmat(scalefac,[1,1,size(GRAV.data,3)]);
%eurocentric
    GRAV.data=circshift(GRAV.data,[0,360,0]);
    GRAV.date=ncread(gracefiles,'time')+datenum('01/01/2002');
    GRAV.year=year(GRAV.date);
    GRAV.month=month(GRAV.date);
    GRAV.units='cm';
    GRAV.gfdates=datenum('01/01/2002')+365.25/24:365.25/12:datenum('05/01/2024');
    FINITEMASK=isfinite(GRAV.data);
    GRAV.data(FINITEMASK==0)=0;
    GRAV.gfdata=permute(interp1(GRAV.date,permute(GRAV.data,[3,2,1]),GRAV.gfdates,'spline'),[3,2,1]);

[x,y,A]=loadworldmesh(0.5);

GRAV.lon=x(1,:);
GRAV.lat=y(:,1);


   %Storing delta EWT
GRAV.gfdates2=[GRAV.gfdates-365.25/24,GRAV.gfdates(end)+365.25/24];

%Filtering to dates which are either within 10 days or within 2month gap

%Doing for mod-month dates

    %Nearest 
    GRAV.gfdates_valid=GRAV.gfdates*false;
    for n=1:numel(GRAV.gfdates)
        %Nearest date < date
        closestbefore= max(GRAV.date(GRAV.date<GRAV.gfdates(n))) - GRAV.gfdates(n);
        closestafter= min(GRAV.date(GRAV.date>GRAV.gfdates(n))) - GRAV.gfdates(n);

        if ~isempty(closestbefore) & ~isempty(closestafter) & ((closestafter-closestbefore<365.25*2/12) | (closestbefore>-365.25/24 | closestafter<365.25/24))
            GRAV.gfdates_valid(n)=true;
        end
    end



    %Doing for start-of-month
   %Nearest 
    GRAV.gfdates2_valid=GRAV.gfdates2*false;
    for n=1:numel(GRAV.gfdates2)
        %Nearest date < date
        closestbefore= max(GRAV.date(GRAV.date<GRAV.gfdates2(n))) - GRAV.gfdates2(n);
        closestafter= min(GRAV.date(GRAV.date>GRAV.gfdates2(n))) - GRAV.gfdates2(n);

        if ~isempty(closestbefore) & ~isempty(closestafter) & ((closestafter-closestbefore<365.25*2/12) | (closestbefore>-365.25/24 | closestafter<365.25/24))
            GRAV.gfdates2_valid(n)=true;
        end
    end



  



    %Removing invalid date stamps
    GRAV.gfdata(:,:, ~GRAV.gfdates_valid)=NaN;


    
    %gfdates2mid=datenum('01/01/2002')+365.25/24:365.25/12:datenum('05/01/2024');
    gfdata2=permute(interp1(GRAV.date,permute(GRAV.data,[3,2,1]),GRAV.gfdates2,'spline'),[3,2,1]);
    gfdata2(:,:,~GRAV.gfdates2_valid)=NaN;
    GRAV.dEWT=diff(gfdata2,[],3);
    GRAV.dEWT_units='cm/month';

    
    %FIlling values outside -1000 to 1000 with nans
    [x,y]=loadworldmesh(0.5);

    pts=abs(GRAV.data)>1000 | repmat(y,[1,1,size(GRAV.data,3)])<-60;
    GRAV.data(pts)=NaN;
        pts=abs(GRAV.gfdata)>1000 | repmat(y,[1,1,size(GRAV.gfdata,3)])<-60;
    GRAV.gfdata(pts)=NaN;
            pts=abs(GRAV.dEWT)>1000 | repmat(y,[1,1,size(GRAV.dEWT,3)])<-60;
    GRAV.dEWT(pts)=NaN;
    
    GRAV.gfmonth=month(GRAV.gfdates);
    GRAV.gfyear=year(GRAV.gfdates);


    %Setting non-valid layers to zero


    %Do same for gfdates 2
    


    
    %Step 1. Find nearest point > date
    %Step 2. Find nearest point < date
    %Step 3. Find difference
    %Step 4. Flag if diff > threshold


    % 
    

%data scaling factors loaded here
%ds=netcdf('/Users/abloom/DATA/GRACE/CLM4.SCALE_FACTOR.DS.G300KM.RL05.DSTvSCS1401.nc');
%Gap filling for monthly 2003-2015 product
    %Dates for spline interpolation
    


 


end




% 
% function TEST_GAPFILL_GRACE_DATA
%     %Create consistent monthly product of GRACE data
% 
%     %This Gapfilling approach regresses monthly 2005-2010 mean data against 
%     %gaps (centred at gaps).
%     %
%     %For edge gaps, the 2005-2010 seasonality is brought as close as
%     %possible
% 
% 
% 
% 
% 
% 
% 
%     %Step 1. 
%     %tests
%   tgra=squeeze(GRAV.data(150,240,:));
%   tdat=GRAV.date;
%   idat=datenum('01/01/2003')+365.25/24:365.25/12:datenum('01/01/2016');
%   igra=interp1(tdat,tgra,idat,'spline');
%   ggra=squeeze(GRAV.gfdata(150,240,:));
%   gdat=GRAV.gfdates;
% 
%   %test figure
%   figure(1);clf
%   axes('Position',[0.05,0.4,0.93,0.5]);
% hold on;set(gca,'FontSize',8);
%   plot(tdat,tgra,'b.-','MarkerSize',10,'LineWidth',1);
%   plot(idat,igra,'ro-','MarkerSize',5,'LineWidth',1);
%   plot(gdat,ggra,'ks-','MarkerSize',5,'LineWidth',1);
%  set(gca,'xlim',[datenum('01/01/2003'),datenum('01/01/2016')])
%    plotyearbars([],[],[],[],4);
%    legend('GRACE','GRACE (gapfilled)')
%    ylabel('Equivalent Water Thickness (EWT) [cm]');
%    title(sprintf('Example: 60N; 60E'))
% 
% 
% 
% 
% 
%     end
% 
    
