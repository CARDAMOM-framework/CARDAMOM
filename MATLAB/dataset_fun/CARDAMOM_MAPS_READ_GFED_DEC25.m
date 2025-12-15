
function GFED=CARDAMOM_MAPS_READ_GFED_DEC25(RES)

%This function reads 2001-latest burned area, and works around continuity
%issues and land-sea mask issues,
%also reads emissions, as/when these are needed
%gridding in commonly used resolutions done internally

YEARS=2001:2024;

[~,lsfrac025]=loadlandseamask(0.25);



GBA=[];
GCE=[];
for yr=YEARS
    GF4=DATASCRIPT_READ_GFED4_DATA_MAY16(yr);
    GBA(:,:,(yr-2001)*12+[1:12])=GF4.BA;
    GCE(:,:,(yr-2001)*12+[1:12])=GF4.ES(:,:,:,4);
end


 M4D=repmat(lsfrac025>0.5,[1,1,size(GBA,3),size(GBA,4)]);
 GBA(M4D==0)=NaN;GBA(isnan(GBA) & M4D)=0;
 GCE(M4D==0)=NaN;GCE(isnan(GCE) & M4D)=0;
 %Ensures land fire-free regions = 0 but sea regions are = NaN
 
 switch RES
     case '05deg'
 
         GFED.BA=GBA(2:2:end,2:2:end,:)*0;
          GFED.FireC=GCE(2:2:end,2:2:end,:)*0;
          Fcount=GBA(2:2:end,2:2:end,:)*0;

         for r=1:2
             for c=1:2
          GFED.BA=GFED.BA + nan2zero(GBA(2:2:end,2:2:end,:));
          GFED.FireC=GFED.FireC + nan2zero(GCE(2:2:end,2:2:end,:));
          Fcount=Fcount + double( isnan(GBA(2:2:end,2:2:end,:))==0);
                 
             end
         end
         GFED.BA=GFED.BA./Fcount;
         GFED.FireC=GFED.FireC*12/365.25./Fcount;
         
         
     case 'GC4x5'
 
          
 GFED.BA=GEOSChem_regular_grid_to_GC(GBA);
 GFED.FireC=GEOSChem_regular_grid_to_GC(GCE)*12/365.25;%g species/m2/month
 
     case '0.25deg'
GFED.BA=GBA;
 GFED.FireC=GCE*12/365.25;%g species/m2/month
         
 end
 %nantotal(nanmean(GCDRIOBS.FireC,3).*GC.area)*365.25/1e15

 disp('GFEDv4s beta does not provide 2017-21 burned area');
 disp('Using GFEDv4s 2001-2016 BA/Emissions ratio')
 idxdec16=16*12;

 %Using climatology of emissions and fluxes
 for m=1:(YEARS(end)-2016)*12
    BAextra(:,:,m)=sum(GFED.BA(:,:,m:12:idxdec16),3)./sum(GFED.FireC(:,:,m:12:idxdec16),3).*GFED.FireC(:,:,idxdec16+m);
 end
 %storing and replacing NaNs with zeros (Nans occur when 2010-2016 fires
 %are zero)
GFED.BA(:,:,idxdec16+1:end)= BAextra;
GFED.BA=nan2zero(GFED.BA);


yrdates=YEARS(1)+1/24:1/12:YEARS(end)+1;
GFED.year=floor(yrdates);
GFED.month=round(mod(yrdates*12-0.5,12)+1);



end





