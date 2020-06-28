function CARFIELDS=CARDAMOM_PROJECT_OUTPUTS_beta(PXI,FIELDS)

%Functionality: uses PXI and FIELDS structure
%
%FIELDS structure contains N fields. For a given field "n", example
%
%Required
%FIELDS(n).name='GPP_monthly'
%FIELDS(n).func=@(CBR,cbffile) mean(CBR.GPP); 
%
%
%Optional
%FIELDS(n).overwrite=0; %default = 0
%Functions on n x m array
%FIELDS(n).stats(s).name
%FIELDS(n).stats(s).func; %default = percentiles, mean, stdes


defval('writetofile',0);
for f=1:numel(FIELDS) 
    %Defaults
    if  isstruct(FIELDS) & (isfield(FIELDS,'stats')==0 | isempty(FIELDS(f).stats))
        FIELDS(f).stats(1).func=@(M) percentile(M,5);
        FIELDS(f).stats(2).func=@(M) percentile(M,25);
        FIELDS(f).stats(3).func=@(M) percentile(M,50);
        FIELDS(f).stats(4).func=@(M) percentile(M,75);
        FIELDS(f).stats(5).func=@(M) percentile(M,95);
        FIELDS(f).stats(6).func=@(M) mean(M);
        FIELDS(f).stats(7).func=@(M) std(M);
        
        FIELDS(f).stats(1).name='perc05';
        FIELDS(f).stats(2).name='perc25';
        FIELDS(f).stats(3).name='perc50';
        FIELDS(f).stats(4).name='perc75';
        FIELDS(f).stats(5).name='perc95';
        FIELDS(f).stats(6).name='mean';
        FIELDS(f).stats(7).name='stdev';
    end
    
end
%
%
%OUTPUTS

if isstr(FIELDS);
    fname=FIELDS;clear FIELDS;
    FIELDS(1).name=fname;
end

%Step 0. Global definitions
px=find(PXI.lat>=-90 & PXI.lat<=90);

output_path=sprintf('CARDAMOM_DISK/OUTPUTS/%s',PXI.run_name);
dirmake(output_path);

if isempty(FIELDS);
    disp('List of available fields')
    disp(ls([output_path,'/*mat']))
else


crmopt.MODEL.ID=PXI.ID;
crmopt.MODEL.MA=CARDAMOM_MODEL_LIBRARY(PXI.ID);
crmopt.STORE=1;%flag to store outputs
crmopt.compile=0;

    %Step 1. Check if files exist or need overwriting 
for f=1:numel(FIELDS);

    
    filenames{f}=[output_path,'/',FIELDS(f).name,'.mat'];
        
    if isfield(FIELDS,'overwrite') & FIELDS(f).overwrite==1 ;fstatus(f)=0;else;    fstatus(f)=isfile(filenames{f});end
    
    
         F{f}=matfile(filenames{f},'Writable',true);

   
end





%Step 2. Populate missing files
    % if writetofile==1
   %  end
    %Step 2.1 loop through all pixels & populate array(s)
    
    
if total(fstatus==0)>0;
    
    
for p=px;%1:numel(PXI.lon);
            %Step 2.1.1 load CBF CBR files
            cbffile=PXI.cbffilename{p};
            cbrfiles=PXI.cbrfilename(p,PXI.chains(p,:)==1);
            %Step 2.1.1 cardamom_run_model in fast mode
            if isempty(cbrfiles)==0;
            [CBR]=CARDAMOM_RUN_MODEL(cbffile,cbrfiles,crmopt);
            end
            
            for f=find(fstatus==0);

            %Step 2.1.2: extract field
            carout=FIELDS(f).func(CBR,cbffile);

            
            
            %Step 2.1.3: populate map
            for s=1:numel(FIELDS(f).stats)
                %stats
                            %Step 2.1.3: extract stats


                carout_stats=FIELDS(f).stats(s).func(carout);

                
                %Declare statistic for writing to file size if p==px(1)
                sname=FIELDS(f).stats(s).name;
                if p==px(1);
                    %E.g F.mean 
                    F{f}.(sname)=zeros(size(PXI.pixelidmap,1),size(PXI.pixelidmap,2),numel(carout_stats));
                end
                
                
                %fill map with stats
                switch ndims(F{f}.(sname))
                    
                    case 3
                                            F{f}.(sname)(PXI.r(p),PXI.c(p),1:numel(carout_stats))=permute(carout_stats,[1,3,2]);

                    case 2
                                            F{f}.(sname)(PXI.r(p),PXI.c(p))=carout_stats;
                        
                end
                
                    
                    
            end
                

            end
            

    
            
    end

end

%     
% if writetofile==0;
%     FF=matfile(filenames{f},'Writable',true);
%     FF=F;
% end



    
    
%Step 3. Read all files
for f=1:numel(FIELDS)
        CARFIELDS.(FIELDS(f).name)=matfile(filenames{f},'Writable',false);
end



end



end
    

