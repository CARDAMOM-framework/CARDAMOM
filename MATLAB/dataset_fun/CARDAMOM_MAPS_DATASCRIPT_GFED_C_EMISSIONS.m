%For comparison: for yr=2001:2016; GF4=DATASCRIPT_READ_GFED4_DATA_MAY16(yr);GFEDCannual025x025(:,:,yr-2000)=mean(GF4.ES(:,:,:,4),3);end
% [~,~,A025]=loadworldmesh(0.25);
% GFEDCannual05x05=mapsfun_coarsen_resolution(GFEDCannual025x025,A025,2);
%save DUMPFILES/GFED_2001_2016_05x05_NOCODE.mat GFEDCannual05x05
%Units are gC/m2/month
load DUMPFILES/GFED_2001_2016_05x05_NOCODE.mat GFEDCannual05x05



%Step 2. Write out raw time-averaged mean GFED data
%Note: PLEVINE will ensure no non-zero GFED fluxes prescribed with zero
%GFED burned area values... (if that makes any sense).

MFIRES.data=mean(GFEDCannual05x05,3)*12/365.25;
MFIRES.units='gC/m2/d';
MFIRES.global_total_check = total(MFIRES.data.*A)*365.25/1e15;
MFIRES.global_total_check_units = 'PgC/yr';

save DUMPFILES/CARDAMOM_MAPS_DATASCRIPT_GFED_C_EMISSIONS.mat  MFIRES
