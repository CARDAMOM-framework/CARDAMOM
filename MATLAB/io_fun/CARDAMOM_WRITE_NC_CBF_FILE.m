function CARDAMOM_WRITE_NC_CBF_FILE(CBF,fname)



%Step 0. Check file OK
    if isempty(dir(fname))==0
        fprintf('%s\nFile exists...',fname);
        g=input('    ...Overwrite(y/n)','s');
        if g=='y';delete(fname);end
    end

    
%Step 1. get dimensions
nopars=numel(CBF.PARPRIORS.values);
notimesteps=numel(CBF.TIME_INDEX.values);


    
%Adjust number as needed. Not urgent for field to dynamically vary
fieldnames=fields(CBF);

parfields={'PARPRIORS','PARPRIORUNC','OTHERPRIORS','OTHERPRIORSUNC'};

for f=1:numel(fieldnames)
    
    
   if sum(strcmp(parfields,fieldnames{f}))

nccreate(fname,fieldnames{f},'Dimensions',{'nopars',nopars},'FillValue',-9999); 
ncwrite(fname,fieldnames{f},CBF.(fieldnames{f}).values)

   else
       if numel(CBF.(fieldnames{f}).values)==notimesteps
       
       nccreate(fname,fieldnames{f},'Dimensions',{'time',notimesteps},'FillValue',-9999); 
       ncwrite(fname,fieldnames{f},CBF.(fieldnames{f}).values);
       
      
       
       elseif numel(CBF.(fieldnames{f}).values)==1;
           
        nccreate(fname,fieldnames{f}); 
       ncwrite(fname,fieldnames{f},CBF.(fieldnames{f}).values);

           
       end
       
   end
   
          disp(fieldnames{f});

   %Write attributes related to quantity
   if isfield(CBF.(fieldnames{f}),'Attributes');
   natt=numel(CBF.(fieldnames{f}).Attributes);
   else natt=0;
   end
   
   if natt>0;
       for a=1:natt
           if strcmp(CBF.(fieldnames{f}).Attributes(a).Name,'_FillValue')==0
       ncwriteatt(fname,fieldnames{f},CBF.(fieldnames{f}).Attributes(a).Name, CBF.(fieldnames{f}).Attributes(a).Value)
           end
       end
       
   end
       
   
   
   
end

   
   







%Use output from 

%CBF=CARDAMOM_READ_NC_CBF_FILE







end

