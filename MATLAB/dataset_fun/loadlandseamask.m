function [LSM,LSfrac]=loadlandseamask(res,frac)
%Using MODIS landsea mask 
%already saved? opt = 0/1
defval('res',3)
defval('frac',0.5);


LSfrac=DATASCRIPT_READ_MODIS_LANDSEAMASK_MAY17(res);
LSM=LSfrac>frac;

%replace LSfrac NaNs by zero for completeness
LSfrac(isnan(LSfrac))=0;


end





% function lsmask_coarse=loadlandseamask(resx,resy);
% %Saved as obsolete
% %Updated May 2017 with ERA-interim 0.125x0.125 land-sea mask
% %already saved? opt = 0/1
% defval('resx',3)
% defval('resy',resx)
% 
% lsmask=load('phdmatlab/lsmaskascii');
% [xf,yf]=meshgrid(-180:0.5:180,-90:0.5:90);
% %lsmask=lsmask(1:360,1:720);
% [x,y]=loadworldmesh([resx,resy]);
% lsmask_coarse=round(interp2(xf,yf,lsmask,x,y,'cubic'));
% %lsmask_coarse=floor(interp2(xf,yf,lsmask,x,y,'cubic'));
% 
% 
% end

