function GG=GEOSChem_xygrids(gridname)

%http://acmg.seas.harvard.edu/geos/doc/man/appendix_2.html

defval('gridname','4x5')
if strcmp(gridname,'2x2.5');
    resx=2.5;resy=2;
    GG.lon=[-180:resx:177.5]';
    GG.lat=[-89,-88:resy:88,89]';
elseif strcmp(gridname,'4x5');
    resx=5;resy=4;
    GG.lon=[-180:resx:175]';
    GG.lat=[-89,-86:resy:86,89]';
end

    [x,y]=meshgrid(GG.lon,GG.lat);
    GG.x=x;
    GG.y=y;
    GG.area=111111.111^2*(resx*resy)*cos(GG.y*pi/180);


    % Calculates the area of each grid cell on a sphere.
    % Formula: R^2 * (lon2 - lon1) * (sin(lat2) - sin(lat1))
    % Lat/Lon edges should be in degrees. Output area in m^2.
    %GC.area accurate
    EARTH_RADIUS_METERS = 6.371e6;
    ydiff=y*0+2;
    ydiff([1,end],:)=1;
    GG.area_accurate=EARTH_RADIUS_METERS.^2 *(5*pi/180)*(sin((y+ydiff)*pi/180) - sin((y-ydiff)*pi/180));
    
    
end