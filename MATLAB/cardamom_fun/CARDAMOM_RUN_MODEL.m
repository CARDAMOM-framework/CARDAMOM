 function [CBR,CBF]=CARDAMOM_RUN_MODEL(CBFin,PARS,OPT);
%CBR=CARDAMOM_RUN_MODEL(CBF,PARS,compile,extended)
%
%INPUTS:
% - CBF: CARDAMOM "CBF" structure or.cbf format filename 
% - PARS: CARDAMOM optimized parameter values. Accepted formats include (a) NxP
% double array consisting of N solutions (rows) x P  model parameters (columns);
% (b) .cbr format filename string, or (c)  matlab cell array with multiple .cbr format
% filenames.
% - compile: (1 or 0): ensures latest version of C code is compiled
% (default = 1);
% -extended: determines the post-processing level of CARDAMOM state
% variables carried out in thus function.
%Options: 
%extended = 0 - fastest run (returns complete output with no post-processing). 
%extended = 1 - included explicit definitions of water stress, LAI, etc
%extended = 2 - includes parameter names, definitions, etc. 
%
%OUTPUTS
% - CBR structure containing all CARDAMOM outputs (fluxes, pools etc). 
%
%Last modified by A.A. Bloom 2019/10/21
%Version 1.4


%"STORE" option allows for files to be stored and to be re-read later on
CBF=CBFin;


    
    
Dpath='DUMPFILES';

if nargin<3 | isempty(OPT); OPT=[];end
if isfield(OPT,'MODEL')==0;
    OPT.MODEL.ID=0;;%CBF ID can be used to bypass call to CARDAMOM_READ_BINARY_FILEFORMAT
    OPT.MODEL.MA=[];end;%MA can be used to bypass call to CARDAMOM_MODEL_LIBRARY
if isfield(OPT,'STORE')==0;OPT.STORE=0;end;%CBF ID can be used to bypass 
if isfield(OPT,'extended')==0;OPT.extended=1;end;%CBF ID can be used to bypass 
if isfield(OPT,'compile')==0;OPT.compile=1;end

if isfield(OPT,'Cpath')==0;OPT.Cpath=getenv('CARDAMOM_C_PATH');end
if isfield(OPT,'command_only')==0;OPT.command_only=0;end


if OPT.STORE==1 & isstruct(CBF);warning('"STORE" option will be ignored as no .cbf file provided for reference');OPT.STORE=0;end



%channel=1;%NOTE: this option is not available yet%If running in parallel, make sure to run on separate "channel"
%Now ensuring no clashes by (a) unique file id with date, and (b) deleting
%files when done
channel=datestr(now,'yyyy-mm-dd-HH-MM-SS-FFF');

if OPT.compile==1
CARDAMOM_COMPILE(OPT.Cpath);
end



if iscell(CBF);CBF=CBF{1};end

%STEP 2. Save CARDAMOM drivers, and CARDAMOM parameters in files
if isstruct(CBF)
    %Here MD is a CARDAMOM data structure and PARS is a NxM array with N
    %samples of M parameters
cbffile=sprintf('%s/tempcardametfile%s.cbf.nc',Dpath,channel);
%writing parameters to file
%writing met drivers to file
CARDAMOM_WRITE_NC_CBF_FILE(CBF,cbffile);
OPT.MODEL.ID=CBF.ID.values;
%number of parameter samples
end
  

if ischar(CBF);
    
    %Here BOTH inputs are files
    %met file (open for MD structure)
    cbffile=CBF;
    if OPT.MODEL.ID==0;
        
        if strcmp(CBF(end-6:end),'.cbf.nc')==1
            OPT.MODEL.ID=ncread(CBF,'ID');
        else
        CBF=CARDAMOM_READ_NC_CBF_FILE(cbffile);
    %parameter file
    %
        OPT.MODEL.ID=CBF.ID.values;
        end
    end
    %MA=CARDAMOM_MODEL_LIBRARY_OLD(CBF);
    %Model substitution:
    
end


%Calling CARDAMOM_MODEL_LIBRARY unless known model info
if isempty(OPT.MODEL.MA)
   OPT.MODEL.MA=CARDAMOM_MODEL_LIBRARY(OPT.MODEL.ID);
end


  

%default value is PARS=[];
%if so, uses last parameter file
%This is for testing purposes only.




%****This part is likely obsolete****
% if  ischar(PARS)
% parfile=PARS;
%     %number of parameter sets 
%     fd=fopen(parfile);if fd==-1;disp('WARNING: file not opened, expect error!!');end;av=fread(fd,inf,'real*8');fclose(fd);N=numel(av)/MA.nopars;
%     PARS=readbinarymat(parfile,[N,MA.nopars])';
% end



%STEP 3. Run C code and save output to a single binary file(within C code).


%COMPILATION FUNCTION IS HERE
%
%make sure compilation has been made
%enter exact compilation code HERE:

if OPT.STORE==0
fluxfile=sprintf('%s/tempcardafluxfile%s.cbr.nc',Dpath,channel);
parfile=sprintf('%s/tempcardaparfile%s.bin',Dpath,channel);

else
    %unique identifier based on name,location and creation date of file
    fdir=dir(cbffile);
    fname=[fdir.folder,'/',fdir.name(1:end-4),fdir.date];fname(fname=='/')='_';fname(fname==' ')='_';fname(fname==':')='_';fname(fname=='-')='_';
    
    storepath=[fdir.folder];storepath(storepath=='/')='_';
    storepath=[Dpath,storepath];
    if isempty(dir(storepath));mkdir(storepath);end
    
    
    fluxfile=sprintf('%s/%s_auxi.bin',storepath, fname);storefilestatus(1) = isfile(fluxfile);
    parfile=sprintf('%s/%s_pars.bin',storepath, fname);storefilestatus(5) = isfile(parfile);

    nostorefiles=0;
    if sum(double(storefilestatus))<5; nostorefiles=1;end

end


%If multiple files are provided, all required information is derived here
if (iscell(PARS) | ischar(PARS)) & (OPT.STORE==0  | nostorefiles==1)
    OPT.MODEL.MA.latterhalf=1; 
    if  ischar(PARS) & strcmp('START',PARS(end-4:end));OPT.MODEL.MA.latterhalf=0;end
    [PARS,ANCILLARY]=CARDAMOM_READ_BINARY_FILEFORMAT(PARS,OPT.MODEL.MA);
elseif OPT.STORE==1
    PARS=readbinarymat(parfile,[0,OPT.MODEL.MA.nopars])';
end





%Either read existing files, or read in stored files
command_str=[];
if OPT.STORE==0 | nostorefiles==1
    %CONTINUE
    fd=fopen(parfile,'w');
fwrite(fd,PARS','real*8');
fclose(fd);



command_str=sprintf('%s/projects/CARDAMOM_GENERAL/CARDAMOM_RUN_MODEL.exe %s %s %s',OPT.Cpath, cbffile,parfile,fluxfile);
                       %run command
                       disp('C executable command');
                       disp(command_str);
                       disp('******************');
%Command_str

%Otherwise files are ready to read!




if OPT.command_only==0
uf=unix(command_str);
if uf>0; disp('ERROR! Execution of CARDAMOM_RUN_MODEL.exe failed, entering keyboard mode');
    keyboard
else
    disp('MODEL run success!!');
end

end


end

%Read outputs
 CBR=READ_OUTPUTS(PARS,fluxfile,OPT);

%READ AND PROCESS OUTPUTS
if OPT.MODEL.ID==1101
    CBR=PROCESS_OUTPUTS_1101(CBR,OPT);
elseif OPT.MODEL.ID==1080
        CBR=PROCESS_OUTPUTS_1080(CBR,OPT);

else 
    CBR=PROCESS_OUTPUTS_OLD(CBR,OPT);
end
 
 if OPT.STORE==0
delete(sprintf('%s/tempcar*%s*',Dpath,channel));
 end

 %LAI
%  
% LAI=CBR.POOLS(:,:,MD.POOL_IDs.C_fol)./CBR.PARS(:,MD.PARAMETER_IDs.LCMA);
% LAI1=LAI(:,1:end-1);
% LAI2=LAI(:,2:end);
% CBR.LAI=    (LAI1+    LAI2)*0.5;

 end
 
 
 
 function CBR=READ_OUTPUTS(PARS,fluxfile,OPT)
 
 
%STEP 3. Read binary data file
N=size(PARS,1);


 disp('***************NOTE: CARDAMOM_RUN_MODEL changed output dims on July 21 2014*************************')

Nd=0;%size(CBF.MET,1);Nd = 0 allowed, since readbinarymat can figure out how many met timesteps needed
%CBR.FLUXES=permute(readbinarymat(fluxfile,[Nd*0,OPT.MODEL.MA.nofluxes,N]),[3,2,1]);
CBR.FLUXES=permute(ncread(fluxfile,'FLUXES'),[3,2,1]);
%for fluxes consistent with pools: removing first pool AS IT IS CONSISTENT
%with pool start values
%CBR.POOLS=permute(readbinarymat(poolfile,[(Nd+1)*0,OPT.MODEL.MA.nopools,N]),[3,2,1]);
CBR.POOLS=permute(ncread(fluxfile,'POOLS'),[3,2,1]);
CBR.POOLS=CBR.POOLS(:,2:end,:);
%reading EDC diagnostics
%CBR.EDCDIAG=permute(readbinarymat(edcdfile,[1,100,N],2),[3,1,2]);
CBR.EDCDIAG=permute(ncread(fluxfile,'EDCD'),[3,1,2]);
%reading probability fields
CBR.PROB=ncread(fluxfile,'PROB')';
CBR.LIKELIHOODS=ncread(fluxfile,'LIKELIHOODS')';
%storing parameters (for completeness)
CBR.PARS=PARS;
%extras
if exist('ANCILLARY') & isempty(ANCILLARY)==0
CBR.STEPS=ANCILLARY.STEP;
CBR.START=ANCILLARY.START;
CBR.chainid=ANCILLARY.chainid;end

disp('Step 3:ALL CARDAMOM_RUN_MODEL.c outputs successfully loaded!');
end
 
function CBR=PROCESS_OUTPUTS_1101(CBR,OPT);
MD=CARDAMOM_MODEL_LIBRARY(OPT.MODEL.ID);

%STEP 4. Arrange data in structure
%NEE = Resp - GPP. 
%This flux CORRECTLY does not include fires.
%That would be NBE (Net Biospheric Exchange).
%Shuang made changes here, modified Rh scheme (1010 and 1011) use different
%fluxes,consistant with DALEC source code, April 2021


CBR.NBE=sum(CBR.FLUXES(:,:,[MD.FLUX_IDs.resp_auto,MD.FLUX_IDs.rh_co2,MD.FLUX_IDs.f_total]),3) - CBR.FLUXES(:,:,MD.FLUX_IDs.gpp);

CBR.GPP=CBR.FLUXES(:,:,MD.FLUX_IDs.gpp);

CBR.ET=CBR.FLUXES(:,:,MD.FLUX_IDs.et);




%LAI
CBR.LAI=CBR.POOLS(:,:,MD.POOL_IDs.C_fol)./CBR.PARS(:,MD.PARAMETER_IDs.LCMA);
    


% keyboard;
% 
% 
% DR.deltat=deltat;
% 
% %LAI


%*************Exporting info on fluxes and pools****************
% if OPT.extended==2 &OPT.MODEL.ID==1;
%     CBR.fluxnames={'1. GPP','2. temperature factor (*)','3. Rauto','4. GPP->Foliar','5. GPP->Labile','6. GPP->Root','7. GPP->Wood','8. Labile->Foliar','9. Leaf fall factor (*)','10. Leaf->Litter','11. Wood->SOC','12. Root->Litter','13. Heterotrophic Respiration (litter)',...
%         '14. Heterotrophic respiration (SOC)','15. Litter->SOM','16. Labile release factor (*)','17. Fires','18.Fires (labile)','19.Fires (foliar)','20.Fires (root)','21.Fires (wood)','22.Fires (litter)','23.Fires (SOC)','24. Fire mortality: Labile -> Litter','25. Fire mortality: Foliar -> Litter',...
%         '26. Fire mortality: Root -> Litter','27. Fire mortality: Wood -> SOC','28. Fire mortality: Litter -> SOC'};
%     CBR.poolnames={'Labile','Foliar','Root','Wood','Litter','SOM'};
%     CBR.parameternames={'P1. Litter transfer to SOC at 0C (*)','P2. GPP->Rauto (**)','P3. GPP->Foliar (**)','P4. GPP->Root (**)','P5. 1 / (annual leaf loss fraction)','P6. Root turnover (*)','P7. Wood turnover (*)','P8. Litter turnover at 0C (*)',...
%         '9. SOC turnover at 0C (*)','P10. Temperature dependence exponent factor','P11. Canopy efficiency','P12. Leaf onset day','P13. GPP->Labile (**)','P14. Leaf onset period (days)','P15. Leaf fall day','P16. Leaf fall period','P17. Leaf Carbon mass per area',...
%         '18. Labile C at t=0','P19. Foliar C at t=0','P20. Root C at t=0','P21. Wood C at t=0','P22. Litter C at t=0','P23. SOC C at t=0'};
%     CBR.parameter_details{1}='(*) Turnover rates are daily: composite turnover rates (e.g. aggregated monthly turnover) can be calculated as 1-(1-tor)^timestep (NOTE: CARDAMOM timestep = 31 days)';
%     CBR.parameter_details{2}='(**) Net allocation fractions can be derived from parameters P2,P3,P4,P13 (marked as **) as follows: Frau = P2; Flab = (1- Frau)*P13; Ffol = (1-Frau - Flab)*P3; Froo = (1-Frau - Flab - Ffol)*P4;Fwoo = 1-Frau - Flab - Ffol - Fwoo;';
%     CBR.flux_details{1}='All fluxes are gC m-2 day-1; All quantities are fluxes except those denoted as (*)';
%     CBR.general_info={'See Bloom & Williams 2015 (doi:10.5194/bg-12-1299-2015) and Bloom et al., 2016 (doi:10.5194/bg-12-1299-2015)'};
%     %Correct "dates bug" here
%     %Note: dates are internally consistent and therefore "dates bug" does not affect output/results/conclusions.
%     CBR.DRIVERS=CBF.MET;CBR.DRIVERS(:,[1,5])=CBR.DRIVERS(:,[1,5])+1;
%     CBR.PARS(:,[12,15])=mod(CBR.PARS(:,[12,15])+1,365.25);
%     CBR.driver_names={'Timestep (days since Jan 1 2001 00:00)','Min temperature [C]','Max temperature [C]','Global Radiation [MJ/m2/day]','CO2 [ppm]','Day of year','Burned Area'};
%     CBR.NBE=CBR.NBE;
%     CBR.NBE_details={'Net Biospheric Exchange [gC m-2 day-1] = Rauto + Rhet + Fires - GPP'};
%     CBR=rmfield(CBR,'PROB');CBR=rmfield(CBR,'EDCDIAG');CBR=rmfield(CBR,'NEE');CBR=rmfield(CBR,'GPP');
% end



% 
% Obsolete, is currently incompatible with script.
% if OPT.command_only==1;
%     CBR=command_str;
% end


 CBR.run_mode='forward';        
 
 
 

 end


function CBR=PROCESS_OUTPUTS_1080(CBR,OPT);
MD=CARDAMOM_MODEL_LIBRARY(OPT.MODEL.ID);

%STEP 4. Arrange data in structure
%NEE = Resp - GPP. 
%This flux CORRECTLY does not include fires.
%That would be NBE (Net Biospheric Exchange).
%Shuang made changes here, modified Rh scheme (1010 and 1011) use different
%fluxes,consistant with DALEC source code, April 2021


CBR.NBE=CBR.FLUXES(:,:,MD.FLUX_IDs.nbe);

CBR.GPP=CBR.FLUXES(:,:,MD.FLUX_IDs.gpp);

CBR.ET=CBR.FLUXES(:,:,MD.FLUX_IDs.et);





if OPT.extended==1
    

        %LMA is par 11
    CBR.LAI1=CBR.POOLS(:,:,MD.POOL_IDs.C_fol_1)./CBR.PARS(:,MD.PARAMETER_IDs.LCMA_1);
    CBR.LAI2=CBR.POOLS(:,:,MD.POOL_IDs.C_fol_2)./CBR.PARS(:,MD.PARAMETER_IDs.LCMA_2);
    CBR.LAI=    (CBR.LAI1+    CBR.LAI2)*0.5;
   
            %LMA is par 11
    CBR.GPP1=CBR.FLUXES(:,:,MD.FLUX_IDs.gpp_1);
    CBR.GPP2=CBR.FLUXES(:,:,MD.FLUX_IDs.gpp_2);
    
       CBR.ET1=CBR.FLUXES(:,:,MD.FLUX_IDs.et_1);
    CBR.ET2=CBR.FLUXES(:,:,MD.FLUX_IDs.et_2);
   
    
end
    


CBR.ALLOC.f_auto_1= CBR.PARS(:,MD.PARAMETER_IDs.f_auto_1);
CBR.ALLOC.f_auto_2= CBR.PARS(:,MD.PARAMETER_IDs.f_auto_2);
CBR.ALLOC.f_ffol_1= CBR.PARS(:,MD.PARAMETER_IDs.f_auto_1);
CBR.ALLOC.f_ffol_2= CBR.PARS(:,MD.PARAMETER_IDs.f_auto_2);

%CONTINUE
% AF.ffol = CBR.PARS(:,3).*(1- AF.fauto);
% AF.flab= CBR.PARS(:,13).*(1- AF.fauto - AF.ffol);
%  AF.froo= CBR.PARS(:,4).*(1- AF.fauto - AF.ffol - AF.flab);
% AF.fwoo= 1 - AF.fauto - AF.ffol - AF.flab - AF.froo;
% 
% AF.all=[AF.fauto,AF.ffol,AF.flab,AF.froo,AF.fwoo];



 CBR.run_mode='forward';        
 
 
 

 end


 
function CBR=PROCESS_OUTPUTS_OLD(CBR,OPT);
MD=CARDAMOM_MODEL_LIBRARY(OPT.MODEL.ID);

%STEP 4. Arrange data in structure
%NEE = Resp - GPP. 
%This flux CORRECTLY does not include fires.
%That would be NBE (Net Biospheric Exchange).
%Shuang made changes here, modified Rh scheme (1010 and 1011) use different
%fluxes,consistant with DALEC source code, April 2021
if OPT.MODEL.ID==1010 || OPT.MODEL.ID==1011 || OPT.MODEL.ID==1012 
    CBR.NEE=sum(CBR.FLUXES(:,:,[3,37]),3)-CBR.FLUXES(:,:,1);
    if OPT.MODEL.ID>1;CBR.NBE=sum(CBR.FLUXES(:,:,[3,37]),3)-CBR.FLUXES(:,:,1)+CBR.FLUXES(:,:,17);else CBR.NBE=CBR.NEE;end
elseif OPT.MODEL.ID==1100
    CBR.NEE=sum(CBR.FLUXES(:,:,[3,48]),3)-CBR.FLUXES(:,:,1);
    if OPT.MODEL.ID>1;CBR.NBE=sum(CBR.FLUXES(:,:,[3,48]),3)-CBR.FLUXES(:,:,1)+CBR.FLUXES(:,:,19);else CBR.NBE=CBR.NEE;end
else
    CBR.NEE=sum(CBR.FLUXES(:,:,[3,13,14]),3)-CBR.FLUXES(:,:,1);
    if OPT.MODEL.ID>1;CBR.NBE=sum(CBR.FLUXES(:,:,[3,13,14]),3)-CBR.FLUXES(:,:,1)+CBR.FLUXES(:,:,17);else CBR.NBE=CBR.NEE;end
end

%Fossil fuel option 
if OPT.MODEL.ID==1200; CBR.FF= CBR.FLUXES(:,:,31);end

%GPP
CBR.GPP=CBR.FLUXES(:,:,1);

if OPT.extended==1
    
    
  %Water stress
  if size(CBR.POOLS,3)>6
      if OPT.MODEL.ID<=8 | any(ismember([801,802,803,804,805,806,807,808,809,810,811,812,813,10,1000,1001,1002,1003,1005,1009],OPT.MODEL.ID))
    CBR.H2OSTRESS=min([CBR.PARS(:,27), CBR.POOLS(:,1:end-1,7)]./repmat(CBR.PARS(:,26),[1,size(CBR.POOLS(:,:,2),2)]),1);
    elseif OPT.MODEL.ID==1030 | OPT.MODEL.ID==1031 | OPT.MODEL.ID==1032 | OPT.MODEL.ID==1060;
        CBR.PAWSTRESS=min([CBR.PARS(:,27), CBR.POOLS(:,1:end-1,7)]./repmat(CBR.PARS(:,26),[1,size(CBR.POOLS(:,:,2),2)]),1);        
        CBR.VPDSTRESS=1./(1+repmat(CBF.MET(:,8)',[size(CBR.PARS(:,37),1),1])./repmat(CBR.PARS(:,37),[1,size(CBF.MET(:,8),1)]));
        CBR.H2OSTRESS=CBR.PAWSTRESS.*CBR.VPDSTRESS;
      elseif OPT.MODEL.ID==9
          CBR.H2OSTRESS=1-exp(-[CBR.PARS(:,27), CBR.POOLS(:,1:end-1,7)]./repmat(CBR.PARS(:,26),[1,size(CBR.POOLS(:,:,2),2)]));
      end
end
end



% keyboard;
% 
% 
% DR.deltat=deltat;
% 
% %LAI


%*************Exporting info on fluxes and pools****************
% if OPT.extended==2 &OPT.MODEL.ID==1;
%     CBR.fluxnames={'1. GPP','2. temperature factor (*)','3. Rauto','4. GPP->Foliar','5. GPP->Labile','6. GPP->Root','7. GPP->Wood','8. Labile->Foliar','9. Leaf fall factor (*)','10. Leaf->Litter','11. Wood->SOC','12. Root->Litter','13. Heterotrophic Respiration (litter)',...
%         '14. Heterotrophic respiration (SOC)','15. Litter->SOM','16. Labile release factor (*)','17. Fires','18.Fires (labile)','19.Fires (foliar)','20.Fires (root)','21.Fires (wood)','22.Fires (litter)','23.Fires (SOC)','24. Fire mortality: Labile -> Litter','25. Fire mortality: Foliar -> Litter',...
%         '26. Fire mortality: Root -> Litter','27. Fire mortality: Wood -> SOC','28. Fire mortality: Litter -> SOC'};
%     CBR.poolnames={'Labile','Foliar','Root','Wood','Litter','SOM'};
%     CBR.parameternames={'P1. Litter transfer to SOC at 0C (*)','P2. GPP->Rauto (**)','P3. GPP->Foliar (**)','P4. GPP->Root (**)','P5. 1 / (annual leaf loss fraction)','P6. Root turnover (*)','P7. Wood turnover (*)','P8. Litter turnover at 0C (*)',...
%         '9. SOC turnover at 0C (*)','P10. Temperature dependence exponent factor','P11. Canopy efficiency','P12. Leaf onset day','P13. GPP->Labile (**)','P14. Leaf onset period (days)','P15. Leaf fall day','P16. Leaf fall period','P17. Leaf Carbon mass per area',...
%         '18. Labile C at t=0','P19. Foliar C at t=0','P20. Root C at t=0','P21. Wood C at t=0','P22. Litter C at t=0','P23. SOC C at t=0'};
%     CBR.parameter_details{1}='(*) Turnover rates are daily: composite turnover rates (e.g. aggregated monthly turnover) can be calculated as 1-(1-tor)^timestep (NOTE: CARDAMOM timestep = 31 days)';
%     CBR.parameter_details{2}='(**) Net allocation fractions can be derived from parameters P2,P3,P4,P13 (marked as **) as follows: Frau = P2; Flab = (1- Frau)*P13; Ffol = (1-Frau - Flab)*P3; Froo = (1-Frau - Flab - Ffol)*P4;Fwoo = 1-Frau - Flab - Ffol - Fwoo;';
%     CBR.flux_details{1}='All fluxes are gC m-2 day-1; All quantities are fluxes except those denoted as (*)';
%     CBR.general_info={'See Bloom & Williams 2015 (doi:10.5194/bg-12-1299-2015) and Bloom et al., 2016 (doi:10.5194/bg-12-1299-2015)'};
%     %Correct "dates bug" here
%     %Note: dates are internally consistent and therefore "dates bug" does not affect output/results/conclusions.
%     CBR.DRIVERS=CBF.MET;CBR.DRIVERS(:,[1,5])=CBR.DRIVERS(:,[1,5])+1;
%     CBR.PARS(:,[12,15])=mod(CBR.PARS(:,[12,15])+1,365.25);
%     CBR.driver_names={'Timestep (days since Jan 1 2001 00:00)','Min temperature [C]','Max temperature [C]','Global Radiation [MJ/m2/day]','CO2 [ppm]','Day of year','Burned Area'};
%     CBR.NBE=CBR.NBE;
%     CBR.NBE_details={'Net Biospheric Exchange [gC m-2 day-1] = Rauto + Rhet + Fires - GPP'};
%     CBR=rmfield(CBR,'PROB');CBR=rmfield(CBR,'EDCDIAG');CBR=rmfield(CBR,'NEE');CBR=rmfield(CBR,'GPP');
% end




if any(ismember([1000,1001,1002,1003,1005,1030,1031,1032,1060],OPT.MODEL.ID))
    %Accounting for time offset
    CBR.EWT=[CBR.PARS(:,27), CBR.POOLS(:,:,7)]+[CBR.PARS(:,36),CBR.POOLS(:,:,8)];
    CBR.EWT=CBR.EWT(:,2:end)/2+CBR.EWT(:,1:end-1)/2;
    CBR.EWT=CBR.EWT-repmat(mean(CBR.EWT,2),[1,size(CBR.EWT,2)]);

    %export runoff
    

%Runoff from PAW and PUW 
    %Wrong: CBR.RO=CBR.FLUXES(:,:,30)-CBR.FLUXES(:,:,31)+CBR.FLUXES(:,:,32);
    if OPT.MODEL.ID==1001 | OPT.MODEL.ID==1003 | OPT.MODEL.ID==1060;
        CBR.RO=CBR.FLUXES(:,:,30)+CBR.FLUXES(:,:,32)+CBR.FLUXES(:,:,33);
    elseif OPT.MODEL.ID==1000 | OPT.MODEL.ID==1002 | OPT.MODEL.ID==1030 | OPT.MODEL.ID==1031 | OPT.MODEL.ID==1032;
        CBR.RO=CBR.FLUXES(:,:,30)+CBR.FLUXES(:,:,32);
    end
    
elseif any(ismember([811,812,813],OPT.MODEL.ID)) 
    %Plant-available EWT
    CBR.EWT=[CBR.PARS(:,27), CBR.POOLS(:,:,7)];
    CBR.EWT=CBR.EWT(:,2:end)/2+CBR.EWT(:,1:end-1)/2;
    CBR.EWT=CBR.EWT-repmat(mean(CBR.EWT,2),[1,size(CBR.EWT,2)]);
    
    CBR.RO=CBR.FLUXES(:,:,30);

end
    
    
    

if any(ismember([809,811,812,813,1000,1001,1002,1003,1005,1009,1030,1031,1032,1060,1012],OPT.MODEL.ID))

    %export ET 
    CBR.ET=CBR.FLUXES(:,:,29);
elseif any(ismember(1100,OPT.MODEL.ID))
    CBR.ET=CBR.FLUXES(:,:,33);
end


    %Export fire C emissions
    if OPT.MODEL.ID==1100
        CBR.FIR=CBR.FLUXES(:,:,19);
    else
        CBR.FIR=CBR.FLUXES(:,:,17);
    end
    %Export respiration
    if OPT.MODEL.ID==1010 || OPT.MODEL.ID==1011 || OPT.MODEL.ID==1012  % shuang added for DALEC-JCR models 
        CBR.RHE=sum(CBR.FLUXES(:,:,37),3);
    elseif OPT.MODEL.ID==1100
        CBR.RHE=sum(CBR.FLUXES(:,:,48),3);
    else
        CBR.RHE=sum(CBR.FLUXES(:,:,13:14),3);
    end
    %Export autotrophic respiration
    CBR.RAU=sum(CBR.FLUXES(:,:,3),3);




% 
% Obsolete, is currently incompatible with script.
% if OPT.command_only==1;
%     CBR=command_str;
% end


 CBR.run_mode='forward';        
 
 
 

 end












