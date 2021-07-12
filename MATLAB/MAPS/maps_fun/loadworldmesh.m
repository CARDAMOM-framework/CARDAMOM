function [x,y,A]=loadworldmesh(res,xylims)

defval('xylims',[-180,180,-90,90]);
%Note that GC does not support xylims at this point

%Resx (& Res y) options: 
%1. Numeric 
%2. String
if ischar(res)
    if strcmp(res,'4x5');
        GC=GEOSChem_xygrids(res);x=GC.x;y=GC.y;A=GC.area;
    %For regularity, first and last row of GEOS-Chem latitude are -90 and
    %90
    y(1,:)=-90;y(end,:)=90;
    end
    
else
    

if numel(res)==1; res=repmat(res,[1,2]);end

%Loads commonly used world meshgrids
hafresx=res(1)/2;
hafresy=res(2)/2;

[x,y]=meshgrid(xylims(1)+hafresx:res(1):xylims(2)-hafresx,xylims(3)+hafresy:res(2):xylims(4)-hafresy);

%Area
A=111111.111^2*(res(1)*res(2))*cos(y*pi/180);

end

 
end