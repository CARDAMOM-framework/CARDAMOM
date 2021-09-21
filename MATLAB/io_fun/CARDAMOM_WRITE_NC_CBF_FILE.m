function CARDAMOM_WRITE_NC_CBF_FILE(CBF,fname)



%Step 0. Check file OK
    if isempty(dir(fname))==0
        fprintf('%s\nFile exists...',fname);
        g=input('    ...Overwrite(y/n)','s');
        if g=='y';delete(fname);end
    end

notimesteps=numel(CBF.time.values);


    
%Adjust number as needed. Not urgent for field to dynamically vary
fieldnames=fields(CBF);


for f=1:numel(fieldnames)
    

       if numel(CBF.(fieldnames{f}).values)==notimesteps
       
       nccreate(fname,fieldnames{f},'Dimensions',{'time',notimesteps},'FillValue',-9999); 
       ncwrite(fname,fieldnames{f},CBF.(fieldnames{f}).values);
       
      
       
       elseif numel(CBF.(fieldnames{f}).values)==1;
           
        nccreate(fname,fieldnames{f}); 
       ncwrite(fname,fieldnames{f},CBF.(fieldnames{f}).values);

           
       
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

