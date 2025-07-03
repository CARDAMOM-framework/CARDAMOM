

%Step 1. Random field
rng(0);
DATA05x05=10+rand(360,720);
AreaGC=getfield(GEOSChem_xygrids,'area');
[~,~,Area05x05]=loadworldmesh(0.5);

%STep 2. Write it out
fname="Random_field_regridding_test.nc";delete(fname)
nccreate(fname,'DATA05x05','Dimensions',{'lat' 360 'lon' 720});
nccreate(fname,'Area05x05','Dimensions',{'lat' 360 'lon' 720});
nccreate(fname,'DATAGC','Dimensions',{'lat_gc' 46 'lon_gc' 72});
nccreate(fname,'AreaGC','Dimensions',{'lat_gc' 46 'lon_gc' 72});

%Step 3. Random field regridded to GC
DATAGC=GEOSChem_regular_grid_to_GC(DATA05x05);

%Step 4.
ncwrite(fname,'DATA05x05',DATA05x05);
ncwrite(fname,'Area05x05',Area05x05);
ncwrite(fname,'DATAGC',DATAGC);
ncwrite(fname,'AreaGC',AreaGC);

%Step 5. Check totals
fprintf('Total data @ 05deg = %2.2f\n',total(Area05.*DATA05x05 ))
fprintf('Total data @ GC grid = %2.2f\n',total(AreaGC.*DATAGC))

