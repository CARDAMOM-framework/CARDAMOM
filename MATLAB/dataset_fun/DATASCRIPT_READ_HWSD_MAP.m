function HWSD=DATASCRIPT_READ_HWSD_MAP(res)

%NOTE: resulting map is at 1x1 degree (180x360)
%
defval('res',1)
%ERA interim grid
[x,y]=loadworldmesh(res);
%NOTE: All values are density per non-zero values (i.e. land only)
[Hall,Htop,Hsub]=hwsd_read_map(x,y);
HWSD.data=double(Hall);
HWSD.top=double(Htop);
HWSD.sub=double(Hsub);

HWSD.x=x;
HWSD.y=y;
HWSD.units='gC m-2';
HWSDframe=HWSD.data;HWSDframe(isnan(HWSDframe))=0;
HWSD.total=sprintf('%4.2f Pg C',total(HWSDframe.*(111111.111*res)^2.*cos(y*pi/180))*1e-15);
HWSD.note=sprintf('Gridded HWSD soil C at %ix%i grid',size(x,1),size(x,2));

end