
function CARDAMOM_MAPS_WRITE_2D_DATASET(DATA)



  delete(DATA.filename)

                
nccreate(DATA.filename,'longitude','Dimensions',{'longitude',size(DATA.data,2)})
ncwrite(DATA.filename,'longitude',DATA.x);
ncwriteatt(DATA.filename,'longitude','units',  'degrees')


nccreate(DATA.filename,'latitude','Dimensions',{'latitude',size(DATA.data,1)})
ncwrite(DATA.filename,'latitude',DATA.y);
ncwriteatt(DATA.filename,'latitude','units',  'degrees')


nccreate(DATA.filename,'data','Dimensions',{'latitude',size(DATA.data,1),'longitude',size(DATA.data,2)})
ncwrite(DATA.filename,'data',DATA.data);
ncwriteatt(DATA.filename,'data','units',DATA.info.units);
ncwriteatt(DATA.filename,'data','variable_info',  DATA.Attribute.variable_info)

disp(sprintf('Done with %s',DATA.filename));




end
