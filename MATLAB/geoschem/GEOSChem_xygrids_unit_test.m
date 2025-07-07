

%Step 1. Random field


rng(0);
DATA05x05=10+rand(360,720);
DATA05x05([1:10,351:360],:)=0;
AreaGC=getfield(GEOSChem_xygrids,'area_accurate');
[x,y,~,Area05x05]=loadworldmesh(0.5);
latitude=y(:,1);
longitude=x(1,:);





%STep 2. Write it out
fname="Random_field_regridding_test.nc";delete(fname)
nccreate(fname,'DATA05x05','Dimensions',{'latitude' 360 'longitude' 720});
nccreate(fname,'Area05x05','Dimensions',{'latitude' 360 'longitude' 720});
nccreate(fname,'latitude','Dimensions',{'latitude' 360});
nccreate(fname,'longitude','Dimensions',{'longitude' 720});
nccreate(fname,'DATAGC','Dimensions',{'lat_gc' 46 'lon_gc' 72});
nccreate(fname,'AreaGC','Dimensions',{'lat_gc' 46 'lon_gc' 72});

%Step 3. Random field regridded to GC
DATAGC=GEOSChem_regular_grid_to_GC4x5_accurate(DATA05x05);

%Step 4.
ncwrite(fname,'latitude',latitude);
ncwrite(fname,'longitude',longitude);
ncwrite(fname,'DATA05x05',DATA05x05);
ncwrite(fname,'Area05x05',Area05x05);
ncwrite(fname,'DATAGC',DATAGC);
ncwrite(fname,'AreaGC',AreaGC);

%Step 5. reread
fname="Random_field_regridding_test.nc";
DATA05x05=ncread(fname,'DATA05x05');
Area05x05=ncread(fname,'Area05x05');
DATAGC=ncread(fname,'DATAGC');
AreaGC=ncread(fname,'AreaGC');

%Step 6. Rederived
DATAGCnew=GEOSChem_regular_grid_to_GC4x5_accurate(DATA05x05);
    
%Step 7. Check totals
fprintf('Total data @ 05deg = %2.2f\n',total(Area05x05.*DATA05x05 ))
fprintf('Total data @ GC grid = %2.2f\n',total(AreaGC.*DATAGC))
fprintf('Total data @ GC grid = %2.2f\n',total(AreaGC.*DATAGCnew))

