function CBF=CARDAMOM_READ_NC_CBF_FILE(filename)




if nargin==0;filename='CARDAMOM/DATA/MODEL_ID_1000_EXAMPLE.cbf.nc';end


%Step 1. ncinfo
ni=ncinfo(filename);

%Step 1. Read all variables
for n=1:numel(ni.Variables);
        
    CBF.(ni.Variables(n).Name).values=ncread(filename,ni.Variables(n).Name);
    
   
    for a=1: numel(ni.Variables(n).Attributes)
        if strcmp(ni.Variables(n).Attributes(a).Name,  '_FillValue')==0
    CBF.(ni.Variables(n).Name).(ni.Variables(n).Attributes(a).Name)=ni.Variables(n).Attributes(a).Value;
        end
    end
    
end





