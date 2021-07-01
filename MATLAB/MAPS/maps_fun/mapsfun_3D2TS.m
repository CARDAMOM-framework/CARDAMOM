function TS=mapsfun_3D2TS(M3,AW,normts)
%TS=mapsfun_3D2TS(M3,AW,normts)
%
%M3 = 3D array with layers = timestepss, and each layer is a global map.
% Areea weights 
%normts = 1 IF you want area mean, 
%normts = 0 IF youu want area total
%
defval('normts',1);

if size(M3,1)==46 & size(M3,2)==72 & (nargin==1 | isempty(AW));GC=GEOSChem_xygrids;AW=GC.area;end

%Extracts time setries from A3 based  on area weights A
for m=1:size(M3,3)
    TS(m)=total(M3(:,:,m).*AW)/(total(AW)^normts);
end



end