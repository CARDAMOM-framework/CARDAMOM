function DATA=DATASCRIPT_READ_RAPID_OUTPUTS_MAY24
%Script reads RAPID inverse runoff estimation results and projects them
%onto the CARDAMOM 0.5 degree grid.

%Ancillary datasets read here
%DATA.ANC=read_ancillary_datasets;

Ntimesteps=360;
[x,y,A]=loadworldmesh(0.5);
%Doubling columns 
Qtemp=repmat(A*0,[1,2,Ntimesteps]);
Qpriortemp=repmat(A*0,[1,2,Ntimesteps]);
A2=repmat(A,[1,2]);
AQ=repmat(A*0,[1,2]);
%ROund the globe twice (to overcome issues, solved later on)
xedges=-360:0.5:360;
yedges=-90:0.5:90;
xcenters=-360+0.25:0.5:360;
ycenters=-90+0.25:0.5:90;




AIDX=11:91;

for a=AIDX;

rapidfname{a}=sprintf('DATA/RAPID_MAY24/m3_riv_pfaf_ii_GLDAS_COR_M_1980-01_2009-12_utc/m3_riv_pfaf_%02i_GLDAS_COR_M_1980-01_2009-12_utc.nc4',a);
rapidfname_prior{a}=sprintf('DATA/RAPID_MAY24/m3_riv_pfaf_ii_GLDAS_ENS_M_1980-01_2009-12_utc/m3_riv_pfaf_%02i_GLDAS_ENS_M_1980-01_2009-12_utc.nc4',a);
shapefilename{a}=sprintf('DATA/RAPID_MAY24/catchments/cat_pfaf_%02i_MERIT_Hydro_v07_Basins_v01.shp',a);


if isfile(rapidfname{a});
disp(rapidfname{a});
disp(rapidfname_prior{a});


%Read catchment areas for 12

SH=shaperead(shapefilename{a});


LAT=ncread(rapidfname{a},'lat');

LON=ncread(rapidfname{a},'lon');


%reading and re-ordering
RIVM3=ncread(rapidfname{a},'m3_riv');
rivm3id=ncread(rapidfname{a},'rivid');
idx=rivm3id*0;
for n=1:numel(LAT);idx(n)=find(rivm3id==SH(n).COMID);end
RIVM3=RIVM3(idx,:);


RIVM3prior=ncread(rapidfname_prior{a},'m3_riv');
rivm3id=ncread(rapidfname_prior{a},'rivid');
for n=1:numel(LAT);idx(n)=find(rivm3id==SH(n).COMID);end
RIVM3prior=RIVM3prior(idx,:);


%Shape file downloaded from here:

%"https://drive.google.com/drive/folders/1PxHZB-vO5O7EUQJSxYS3ptV-tuI39uKD"


%Next step: find % of polygon on 0.5 degree area



% testpolygonx=[179,179,180,181,181,180];
% testpolygony=[65,66,67,66,65,64];

% clear SH;
% SH(1).X=testpolygonx;
% SH(1).Y=testpolygony;

for n=1:numel(SH);

    if mod(n,100)==0;disp(n);end
    %Step 1. grid this into 0.5 deg grid

                %[sbmask,R]=vec2mtx(SH(n).Y,SH(n).X,1/0.5,[-90,90],[-180,180],'filled');
                

                %Step 2. Grid this into ~1km within limits 
                MMX(1)=xedges(xedges==max(xedges(xedges<min(SH(n).X))));
                MMX(2)=xedges(xedges==min(xedges(xedges>max(SH(n).X))));

                                %Step 2. Grid this into ~1km within limits 
                MMY(1)=yedges(yedges==max(yedges(yedges<min(SH(n).Y))));
                MMY(2)=yedges(yedges==min(yedges(yedges>max(SH(n).Y))));

        
                 [sbmask,R]=vec2mtx(SH(n).Y,SH(n).X,1/0.01,MMY,MMX,'filled');
                 %setting 2s to 0s, 0s and 1s to 1
                 sbmask(sbmask==0)=1;
                 sbmask(sbmask==2)=0;
                    
                  %now coarsen grid
                  sbmaskcoarse=0;
                 for xx=1:50
                     for yy=1:50
                            sbmaskcoarse=sbmaskcoarse+sbmask(yy:50:end,xx:50:end);
                     end
                 end

                %Now add runoff to 0.5 deg pixels, with relative weigths
                %Note: runoff is in absolute terms, so no conversions
                %needed. Runoff will be converted into per-area values at
                %the end
                %FInd index

                qweights=sbmaskcoarse/total(sbmaskcoarse);

        ridx=find(ycenters == MMY(1)+0.25):find(ycenters == MMY(2)-0.25);
        cidx=find(xcenters == MMX(1)+0.25):find(xcenters == MMX(2)-0.25);
         for t=1:Ntimesteps
            Qtemp(ridx,cidx,t)=Qtemp(ridx,cidx,t)+qweights.*RIVM3(n,t);
            Qpriortemp(ridx,cidx,t)=Qpriortemp(ridx,cidx,t)+qweights.*RIVM3prior(n,t);
         end
         AQ(ridx,cidx)=AQ(ridx,cidx)+sbmaskcoarse.*A2(ridx,cidx)/(50*50);
%         AQprior(ridx,cidx)=AQprior(ridx,cidx)+sbmaskcoarse.*A2(ridx,cidx)/(50*50);



                %Step 3. calcularing 

    %Step 2. Calculate area (will need this later) 

end

    end


    %Fold Q to ensure Qfinal spans -180  - 180
    Qfinal=Qtemp(:,end/4+1:end*3/4,:);
    Qfinal(:,1:end/2,:)=Qfinal(:,1:end/2,:)+Qtemp(:,end*3/4+1:end,:);
    Qfinal(:,end/2+1:end,:)=Qfinal(:,end/2+1:end,:)+Qtemp(:,1:end/4,:);

    Qpriorfinal=Qpriortemp(:,end/4+1:end*3/4,:);
    Qpriorfinal(:,1:end/2,:)=Qpriorfinal(:,1:end/2,:)+Qpriortemp(:,end*3/4+1:end,:);
    Qpriorfinal(:,end/2+1:end,:)=Qpriorfinal(:,end/2+1:end,:)+Qpriortemp(:,1:end/4,:);

    AQfinal=AQ(:,end/4+1:end*3/4);
    AQfinal(:,1:end/2)=AQfinal(:,1:end/2)+AQ(:,end*3/4+1:end);
    AQfinal(:,end/2+1:end)=AQfinal(:,end/2+1:end)+AQ(:,1:end/4);
    
%save a copy of Q
disp('Saving backup file')
save RAPID2024regrid_dump_interim_Q.mat Qfinal AQfinal Qpriorfinal
end


DATA.Q=Qfinal;
DATA.A=AQfinal;
DATA.Qprior=Qpriorfinal;

DATA.units='m3 per pixel';
end




