function PXI=CARDAMOM_PROJECT_BKP(projname,repeat)
%this function makes a CARDAMOM_PROJECT template
%All outputs should be contained in PXI
%Using PROJSCRIPT_CARDAMOM_CMS_JUL18 format as "vague" template
%
%MOTIVATION:
%CARDAMOM project creation requires centealized routines that fuction
%regardless of project resolution/type/setup
%
%- To effectively do this, project templates are centrally made here
% - Variants of primary project types can either (a) be made externally, or
% (b) options can be passed via "OPTS" argument
%
%REQUIREMENTS: 
%PXI must conain all options required to  use other "CARDAMOM_XX" commands

%Currently supported PROJTYPES
%- GCRUN
%- GLOBAL05

%TEST PR 2
if nargin==0;keyboard;end


if nargin<2;repeat=0;end


switch projname
    case 'GCRUN_NOV18_CMS_PRIOR_V1'
    PXI=GCRUN_NOV18_CMS_PRIOR_V1;%Land-surface constrained run for CMS-Flux prior
    
        case 'GCRUN_AUG20_CMS_PRIOR'
    PXI=GCRUN_AUG20_CMS_PRIOR;
    %******CONTINUE FROM HERE*****
    %PXI=GCRUN_DEC18_TDLE was only copy-pasted from PROJSCRIPT_CARDAMOM_CMS_JUL18
        case 'GCRUN_MAR19_CMS_PRIOR_V1'
             disp('UNDER CONSTRUCTION...');keyboard;
        PXI=GCRUN_MAR19_CMS_PRIOR_V1;%Land-surface constrained run for CMS-Flux prior


    case 'GCRUN_DEC18_TDLE'
    PXI=GCRUN_DEC18_TDLE;%Constrained by land-surface and atmospheric  datasets
    case 'GCRUN_FEB19_TDLE'
    PXI=GCRUN_FEB19_TDLE;%Constrained by land-surface and atmospheric  datasets (s3x3 NEE)
    case 'GCRUN_MAR19_TDLE'
    PXI=GCRUN_MAR19_TDLE;%Constrained by land-surface and atmospheric  datasets (s3x3 NEE); ID.811
    case 'GCRUN_MAR19_TDLE_2'
    PXI=GCRUN_MAR19_TDLE_2;%Constrained by land-surface and atmospheric  datasets (s3x3 NEE); ID.811. annual_unc = 0.05
    case 'GCRUN_MAR19_TDLE_3'
    PXI=GCRUN_MAR19_TDLE_3;%Constrained by land-surface and atmospheric  datasets (s3x3 NEE); ID.811. No NBE
    
    %Same as the MAR19, but with corrections on the 
    case 'GCRUN_JUL19_TDLE'
    PXI=GCRUN_JUL19_TDLE(1,[],repeat);%Constrained by land-surface and atmospheric  datasets (s3x3 NEE); ID.811
    case 'GCRUN_JUL19_TDLE_2'
    PXI=GCRUN_JUL19_TDLE(2,[],repeat);%Constrained by land-surface and atmospheric  datasets (s3x3 NEE); ID.811. annual_unc = 0.05
    case 'GCRUN_JUL19_TDLE_3'
    PXI=GCRUN_JUL19_TDLE(3,[],repeat);%Constrained by land-surface and atmospheric  datasets (s3x3 NEE); ID.811. No NBE
    case 'GCRUN_JUL19_TDLE_4'
    PXI=GCRUN_JUL19_TDLE(4,[],repeat);%Constrained by land-surface and atmospheric  datasets (s3x3 NEE); ID.811. 2010-2013 NBE mean = 0
    
    
    
    case 'GCRUN_APR20_TDLE'
    PXI=GCRUN_APR20_TDLE(1,[],repeat); %Constrained by land-surface and atmospheric  datasets (s3x3 NEE); ID.811, now spans 2001-2015
   case 'GCRUN_APR20_TDLE_5'
    PXI=GCRUN_APR20_TDLE(5,[],repeat); %Constrained by land-surface and atmospheric  datasets (s3x3 NEE); ID.812, now spans 2001-2015

    case 'GCRUN_JUN20_TDLE'
    PXI=GCRUN_JUN20_TDLE(1,[],repeat); %Constrained by land-surface and atmospheric  datasets (s3x3 NEE); ID.813, now spans 2001-2015
   case 'GCRUN_JUN20_TDLE_4'
    PXI=GCRUN_JUN20_TDLE(4,[],repeat); %Constrained by land-surface and atmospheric  datasets (s3x3 NEE); ID.813. 2010-2013 NBE mean = 0
   case 'GCRUN_JUN20_TDLE_5'
    PXI=GCRUN_JUN20_TDLE(5,[],repeat); %Constrained by land-surface and atmospheric  datasets (s3x3 NEE); ID.812, now spans 2001-2015

    
    
    
    
    
    
    %Lagged effect of 2015 fluxes
    
    
       case 'GCRUN_SEP19_ENSO_LE'
    PXI=GCRUN_SEP19_ENSO_LE(1,[],repeat);%Constrained by land-surface and atmospheric  datasets (s3x3 NEE); ID.811. 2015-2018 NBE mean = 0
    
           case 'GCRUN_NOV19_ENSO_LE'
               %Same as september ENSO LE, but longer timespan
                   PXI=GCRUN_NOV19_ENSO_LE(1,[],repeat);%Constrained by land-surface and atmospheric  datasets (s3x3 NEE); ID.811. 2010-2018 NBE mean = 0

                   
           case 'GCRUN_MAR20'
               %Full 2001-2018 record, with all OBS needed for JL analysis
                 PXI=GCRUN_MAR20(1,[],repeat);


    
    case 'GCRUN_APR19_NBE_BIOMASS'
            %TO DO: (1) make sure biomass is gridded OK, and (2) add to CBF
            %files
    PXI=GCRUN_APR19_NBE_BIOMASS;%Constrained by land-surface and atmospheric  datasets (s3x3 NEE); ID.811

    case 'GL05RUN_FEB19'    
        savefile='CARDAMOM_DISK/CBF_FILES/PXI_FILES/GL05RUN_FEB19_PXI.mat';
        if repeat==1 | isempty(dir(savefile));
        PXI=GL05RUN_FEB19;
        save(savefile,'PXI');
        else
            load(savefile);
        end

        %OBSOLETE! Change in code has lead to issues in reproducing these
        %Use AUG19
%     case 'GL05RUN_MAR19'    
%         % ID = 1000
%         % includes GRACE data
%         %maaaaybe with biomass data too
%         %Also includes relative path (to CARDAMOM disk) for easy 
%         savefile='CARDAMOM_DISK/CBF_FILES/PXI_FILES/GL05RUN_MAR19_PXI.mat';
%         if repeat==1 | isempty(dir(savefile));
%         PXI=GL05RUN_MAR19;
%         save(savefile,'PXI');
%         else
%             load(savefile);
%             disp(['Savefile = ',savefile]);
%         end
%         
         case 'GL05RUN_AUG19'    
        % ID = 1000
        % includes GRACE data & CSIF data
        %maaaaybe with biomass data too
        %Also includes relative path (to CARDAMOM disk) for easy 
        savefile='CARDAMOM_DISK/CBF_FILES/PXI_FILES/GL05RUN_AUG19_PXI.mat';
                    OPT.analysis='GL05RUN_AUG19';
            OPT.run_name='GL05RUN_AUG19';
             OPT.run_details='GL05RUN_AUG19';
        if repeat==1 | isempty(dir(savefile));
            OPT.csif=1;
        PXI=GL05RUN_MAR19(OPT);save(savefile,'PXI');
        else
            load(savefile);disp(['Savefile = ',savefile]);
        end
        
        
        
          case 'GL05RUN_APR19_NGD'    
        % ID = 1000
        %No GRACE data
        %maaaaybe with biomass data too
        %Also includes relative path (to CARDAMOM disk) for easy 
        %ADDRESS THIS ISSUE!
% %Step 1.2 run names
% if isempty(OPT);

% else
%     PXItemplate.analysis=OPT.analysis;
% PXItemplate.run_name=OPT.run_name;
% PXItemplate.run_details=OPT.run_details;
% end
        savefile='CARDAMOM_DISK/CBF_FILES/PXI_FILES/GL05RUN_APR19_NGD_PXI.mat';
        if repeat==1 | isempty(dir(savefile));
            OPT.grace=0;
            OPT.analysis='GL05RUN_APR19_NGD';
            OPT.run_name='GL05RUN_APR19_NGD';
             OPT.run_details='GL05RUN_APR19_NGD';
        PXI=GL05RUN_MAR19(OPT);
        save(savefile,'PXI');
        else
            load(savefile);
            disp(['Savefile = ',savefile]);
        end
        
        
end



%Reminders about all prohect outputs




end


function PXI=GCRUN_FEB19_TDLE(runname,s3x3);
%CONTINUE FROM HERE

%Details: This is the same as MAY18_3 (RUN 20 in NOV17)
%However, uncertainties here are set as 1/12 and 0.5 for annual & seasonal
%fluxes
%Step 1. Load drivers 
%See script in PROJSCRIPT_CARDAMOM_GLOBAL_TEMPLATE_MAY15
%TESTSCRIPT_CARDAMOM_BUCKET_NOV16.m
%Better to start with a template of RUN29 just in case
%
%
%includes a "smooth" option (s3x3);


%Turn into hydrological CBF file here
%TESTSCRIPT_CARDAMOM_BUCKET_NOV16.m
%Step 3. define gridcells



%only 
%metidx and obsidx
% index writing at end of CBD definition
%(move MET to later on)
%idx=109:180;


% tidx={1:180,109:180,109:180,109:180};
% tnbeidx={[],[],109:156,109:144};
% tbbidx={1:15,10:15,10:15,10:15};%,10:15,10:15,10:15};
% RO.sifopt=[1,1,2,2];
% RO.bbopt=[2,1,2,2];
% RO.bbunc=[-0.2,1.5,-0.2,-0.2];
% RO.gppopt=[1,1,0,0];
% RO.oco2=[0,0,0,1];
%     

RO.tidx=109:180;%Index of timesteps 
RO.tnbeidx=109:180;
RO.tbbidx=10:15;
%Options are temporary: 
%Remove once done replicating runs
%SIF options
% 0 = no sif data
% 1 = GOSAT sif data
% 2 = GOME2 data
% 3 = FLUXSAT data
RO.sifopt=3;
%BB opt:
%0 = no bb data
%1 = GFEDv4s constraint
%2 = top-down CO constraint
RO.bbopt=2;
%BB uncertainty
%+ve = log; -ve = abs
RO.bbunc=-0.2;
%Mean GPP constraint
%0. No mean GPP 
%1. Fluxcom GPP
RO.gppopt=0;
%OCO-2 options( 1 or 0)
%ri ==1 | ri==8 | ri==9 | ri==10 | ri==11  | ri==12 | ri==13  | ri==14 | ri==15  | ri==21
RO.oco2=0;

%NBE uncertainty mode
%For global uncertainty, provide two values
%For CMS-Flux uncertainty, set to CMS-Flux
RO.nbeunc = [2,0.02];
 defval('runname','');
  defval('s3x3',0);s3x3=1;
  RO.nbe_s3x3=s3x3;

%PXI options
PXI.RO=RO;
PXI.run_details='GCRUN_FEB19_TDLE: 2010-2015 run with CMS (2010-2013) and LS constraints (based on NOV17 run 20), and prescribed NBE unc';
PXI.ID=809;
PXI.run_name=['GCRUN_FEB19_TDLE',runname];
PXI.analysis='GCRUN_FEB19_TDLE';


%MASK
GC=GEOSChem_xygrids;
MASK=abs(GC.y)<=30;

%Create and summarize PXI, based on available options
PXI=create_gcrun_cbf_files(PXI,MASK);


end




function PXI=GCRUN_JUL19_TDLE(opt,runname,repeat);
%CONTINUE FROM HERE

%Details: This is the same as MAY18_3 (RUN 20 in NOV17)
%However, uncertainties here are set as 1/12 and 0.5 for annual & seasonal
%fluxes
%Step 1. Load drivers 
%See script in PROJSCRIPT_CARDAMOM_GLOBAL_TEMPLATE_MAY15
%TESTSCRIPT_CARDAMOM_BUCKET_NOV16.m
%Better to start with a template of RUN29 just in case
%
%
%includes a "smooth" option (s3x3);


%Turn into hydrological CBF file here
%TESTSCRIPT_CARDAMOM_BUCKET_NOV16.m
%Step 3. define gridcells



%only 
%metidx and obsidx
% index writing at end of CBD definition
%(move MET to later on)
%idx=109:180;


% tidx={1:180,109:180,109:180,109:180};
% tnbeidx={[],[],109:156,109:144};
% tbbidx={1:15,10:15,10:15,10:15};%,10:15,10:15,10:15};
% RO.sifopt=[1,1,2,2];
% RO.bbopt=[2,1,2,2];
% RO.bbunc=[-0.2,1.5,-0.2,-0.2];
% RO.gppopt=[1,1,0,0];
% RO.oco2=[0,0,0,1];
%     

RO.tidx=109:180;%Index of timesteps 

if opt==3;
    RO.tnbeidx=[];
else
    RO.tnbeidx=109:156;
end

RO.tbbidx=10:15;
%Options are temporary: 
%Remove once done replicating runs
%SIF options
% 0 = no sif data
% 1 = GOSAT sif data
% 2 = GOME2 data
RO.sifopt=1;
%BB opt:
%0 = no bb data
%1 = GFEDv4s constraint
%2 = top-down CO constraint
RO.bbopt=2;
%BB uncertainty
%+ve = log; -ve = abs
RO.bbunc=-0.2;
%Mean GPP constraint
%0. No mean GPP 
%1. Fluxcom GPP
RO.gppopt=0;
%OCO-2 options( 1 or 0)
%ri ==1 | ri==8 | ri==9 | ri==10 | ri==11  | ri==12 | ri==13  | ri==14 | ri==15  | ri==21
RO.oco2=0;

%NBE uncertainty mode
%For global uncertainty, provide two values
%For CMS-Flux uncertainty, set to CMS-Flux
if opt==2;
    RO.nbeunc = [2,0.05];
else
    RO.nbeunc = [2,0.02];
end

 defval('runname','');
  defval('s3x3',0);s3x3=1;
  RO.nbe_s3x3=s3x3;
  
  if opt==4;
      RO.nbezero=1;
  else
      RO.nbezero=0;
  end

%PXI options
PXI.RO=RO;
switch opt
    case 1
    PXI.run_details='GCRUN_JUL19_TDLE: 2010-2015 run with CMS (2010-2013) and LS constraints (based on NOV17 run 20), and prescribed NBE unc';
    PXI.run_name=['GCRUN_JUL19_TDLE',runname];
    PXI.analysis='GCRUN_JUL19_TDLE';
    case 2
        %'GCRUN_MAR19_TDLE_2: 2010-2015 run with CMS (2010-2013) and LS constraints (based on NOV17 run 20), and prescribed NBE unc';
    PXI.run_details='GCRUN_JUL19_TDLE: 2010-2015 run with CMS (2010-2013) and LS constraints (based on NOV17 run 20), and prescribed NBE unc (annual unc x 2.5)';
    PXI.run_name=['GCRUN_JUL19_TDLE_2',runname];
    PXI.analysis='GCRUN_JUL19_TDLE_2';
    case 3
    PXI.run_details='LS constraints only (based on NOV17 run 20)';
    PXI.run_name=['GCRUN_JUL19_TDLE_3',runname];
    PXI.analysis='GCRUN_JUL19_TDLE_3';
    %Continue, check with notes from MAR19, and ensure options are the
    %same.
    case 4
    PXI.run_details='GCRUN_JUL19_TDLE_4: Same as GCRUN_JUL19_TDLE but with NBEmean = 0';
    PXI.run_name=['GCRUN_JUL19_TDLE_4',runname];
    PXI.analysis='GCRUN_JUL19_TDLE_4';
    
end
PXI.ID=811;



%MASK
GC=GEOSChem_xygrids;
MASK=abs(GC.y)<=30;

%Create and summarize PXI, based on available options
PXI=create_gcrun_cbf_files(PXI,MASK,repeat);


end


function PXI=GCRUN_APR20_TDLE(opt,runname,repeat);
%CONTINUE FROM HERE

%Details: This is the same as MAY18_3 (RUN 20 in NOV17)
%However, uncertainties here are set as 1/12 and 0.5 for annual & seasonal
%fluxes
%Step 1. Load drivers 
%See script in PROJSCRIPT_CARDAMOM_GLOBAL_TEMPLATE_MAY15
%TESTSCRIPT_CARDAMOM_BUCKET_NOV16.m
%Better to start with a template of RUN29 just in case
%
%
%includes a "smooth" option (s3x3);


%Turn into hydrological CBF file here
%TESTSCRIPT_CARDAMOM_BUCKET_NOV16.m
%Step 3. define gridcells



%only 
%metidx and obsidx
% index writing at end of CBD definition
%(move MET to later on)
%idx=109:180;


% tidx={1:180,109:180,109:180,109:180};
% tnbeidx={[],[],109:156,109:144};
% tbbidx={1:15,10:15,10:15,10:15};%,10:15,10:15,10:15};
% RO.sifopt=[1,1,2,2];
% RO.bbopt=[2,1,2,2];
% RO.bbunc=[-0.2,1.5,-0.2,-0.2];
% RO.gppopt=[1,1,0,0];
% RO.oco2=[0,0,0,1];
%     


%OPTIONS
%opt = 1: standard
%opt = 5: with ID=812 (iWUE)

RO.tidx=1:180;%Index of timesteps 

if opt==3;
    RO.tnbeidx=[];
else
    RO.tnbeidx=109:156;
end

RO.tbbidx=1:15;
%Options are temporary: 
%Remove once done replicating runs
%SIF options
% 0 = no sif data
% 1 = GOSAT sif data
% 2 = GOME2 data
RO.sifopt=1;
%BB opt:
%0 = no bb data
%1 = GFEDv4s constraint
%2 = top-down CO constraint
RO.bbopt=2;
%BB uncertainty
%+ve = log; -ve = abs
RO.bbunc=-0.2;
%Mean GPP constraint
%0. No mean GPP 
%1. Fluxcom GPP
RO.gppopt=0;
%OCO-2 options( 1 or 0)
%ri ==1 | ri==8 | ri==9 | ri==10 | ri==11  | ri==12 | ri==13  | ri==14 | ri==15  | ri==21
RO.oco2=0;

%NBE uncertainty mode
%For global uncertainty, provide two values
%For CMS-Flux uncertainty, set to CMS-Flux
if opt==2;
    RO.nbeunc = [2,0.05];
else
    RO.nbeunc = [2,0.02];
end

 defval('runname','');
  defval('s3x3',0);s3x3=1;
  RO.nbe_s3x3=s3x3;
  RO.nbe_product='b73';
  
  if opt==4;
      RO.nbezero=1;
  else
      RO.nbezero=0;
  end

%PXI options
PXI.RO=RO;
switch opt
    case 1
    PXI.run_details='GCRUN_APR20_TDLE: 2010-2015 run with CMS (2010-2013) and LS constraints (based on NOV17 run 20), and prescribed NBE unc';
    PXI.run_name=['GCRUN_APR20_TDLE',runname];
    PXI.analysis='GCRUN_APR20_TDLE';
        case 5
    PXI.run_details='GCRUN_APR20_TDLE_5: 2010-2015 run with CMS (2010-2013) with CBR.ID=812 and LS constraints (based on NOV17 run 20), and prescribed NBE unc';
    PXI.run_name=['GCRUN_APR20_TDLE_5',runname];
    PXI.analysis='GCRUN_APR20_TDLE_5';
end

if opt==5
    PXI.ID=812;
else
PXI.ID=811;
end


%MASK
GC=GEOSChem_xygrids;
MASK=abs(GC.y)<=30;

%Create and summarize PXI, based on available options
PXI=create_gcrun_cbf_files(PXI,MASK,repeat);


end



function PXI=GCRUN_JUN20_TDLE(opt,runname,repeat);
%CONTINUE FROM HERE

%Details: This is the same as MAY18_3 (RUN 20 in NOV17)
%However, uncertainties here are set as 1/12 and 0.5 for annual & seasonal
%fluxes
%Step 1. Load drivers 
%See script in PROJSCRIPT_CARDAMOM_GLOBAL_TEMPLATE_MAY15
%TESTSCRIPT_CARDAMOM_BUCKET_NOV16.m
%Better to start with a template of RUN29 just in case
%
%
%includes a "smooth" option (s3x3);


%Turn into hydrological CBF file here
%TESTSCRIPT_CARDAMOM_BUCKET_NOV16.m
%Step 3. define gridcells



%only 
%metidx and obsidx
% index writing at end of CBD definition
%(move MET to later on)
%idx=109:180;


% tidx={1:180,109:180,109:180,109:180};
% tnbeidx={[],[],109:156,109:144};
% tbbidx={1:15,10:15,10:15,10:15};%,10:15,10:15,10:15};
% RO.sifopt=[1,1,2,2];
% RO.bbopt=[2,1,2,2];
% RO.bbunc=[-0.2,1.5,-0.2,-0.2];
% RO.gppopt=[1,1,0,0];
% RO.oco2=[0,0,0,1];
%     


%OPTIONS
%opt = 1: standard
%opt = 5: with ID=812 (iWUE)

RO.tidx=1:180;%Index of timesteps 

if opt==3;
    RO.tnbeidx=[];
else
    RO.tnbeidx=109:156;
end

RO.tbbidx=1:15;
%Options are temporary: 
%Remove once done replicating runs
%SIF options
% 0 = no sif data
% 1 = GOSAT sif data
% 2 = GOME2 data
RO.sifopt=1;
%BB opt:
%0 = no bb data
%1 = GFEDv4s constraint
%2 = top-down CO constraint
RO.bbopt=2;
%BB uncertainty
%+ve = log; -ve = abs
RO.bbunc=-0.2;
%Mean GPP constraint
%0. No mean GPP 
%1. Fluxcom GPP
RO.gppopt=0;
%OCO-2 options( 1 or 0)
%ri ==1 | ri==8 | ri==9 | ri==10 | ri==11  | ri==12 | ri==13  | ri==14 | ri==15  | ri==21
RO.oco2=0;

%NBE uncertainty mode
%For global uncertainty, provide two values
%For CMS-Flux uncertainty, set to CMS-Flux
if opt==2;
    RO.nbeunc = [2,0.05];
else
    RO.nbeunc = [2,0.02];
end

 defval('runname','');
  defval('s3x3',0);s3x3=1;
  RO.nbe_s3x3=s3x3;
  RO.nbe_product='b73';
  
  if opt==4;
      RO.nbezero=1;
  else
      RO.nbezero=0;
  end

%PXI options
PXI.RO=RO;
switch opt
    case 1
    PXI.run_details='GCRUN_JUN20_TDLE: 2010-2015 run with CMS (2010-2013) and LS constraints (based on NOV17 run 20), and prescribed NBE unc';
    PXI.run_name=['GCRUN_JUN20_TDLE',runname];
    PXI.analysis='GCRUN_JUN20_TDLE';
    
    case 4
    PXI.run_details='GCRUN_JUN20_TDLE_4: Same as GCRUN_JUL19_TDLE but with NBEmean = 0';
    PXI.run_name=['GCRUN_JUN20_TDLE_4',runname];
    PXI.analysis='GCRUN_JUN20_TDLE_4';
    
    
    case 5
    PXI.run_details='GCRUN_JUN20_TDLE_5: 2010-2015 run with CMS (2010-2013) with CBR.ID=812 and LS constraints (based on NOV17 run 20), and prescribed NBE unc';
    PXI.run_name=['GCRUN_JUN20_TDLE_5',runname];
    PXI.analysis='GCRUN_JUN20_TDLE_5';
end

if opt==5
    PXI.ID=812;
else
PXI.ID=813;
end


%MASK
GC=GEOSChem_xygrids;
MASK=abs(GC.y)<=30;

%Create and summarize PXI, based on available options
PXI=create_gcrun_cbf_files(PXI,MASK,repeat);


end




function PXI=GCRUN_SEP19_ENSO_LE(opt,runname,repeat);
%CONTINUE FROM HERE

%Details: This is the same as MAY18_3 (RUN 20 in NOV17)
%However, uncertainties here are set as 1/12 and 0.5 for annual & seasonal
%fluxes
%Step 1. Load drivers 
%See script in PROJSCRIPT_CARDAMOM_GLOBAL_TEMPLATE_MAY15
%TESTSCRIPT_CARDAMOM_BUCKET_NOV16.m
%Better to start with a template of RUN29 just in case
%
%
%includes a "smooth" option (s3x3);


%Turn into hydrological CBF file here
%TESTSCRIPT_CARDAMOM_BUCKET_NOV16.m
%Step 3. define gridcells



%only 
%metidx and obsidx
% index writing at end of CBD definition
%(move MET to later on)
%idx=109:180;


% tidx={1:180,109:180,109:180,109:180};
% tnbeidx={[],[],109:156,109:144};
% tbbidx={1:15,10:15,10:15,10:15};%,10:15,10:15,10:15};
% RO.sifopt=[1,1,2,2];
% RO.bbopt=[2,1,2,2];
% RO.bbunc=[-0.2,1.5,-0.2,-0.2];
% RO.gppopt=[1,1,0,0];
% RO.oco2=[0,0,0,1];
%     

RO.tidx=169:216;%Index of timesteps 2015-2018 (Months since 2001)

if opt==3;
    RO.tnbeidx=[];
else
    RO.tnbeidx=169:216;
end

RO.tbbidx=15;
%Options are temporary: 
%Remove once done replicating runs
%SIF options
% 0 = no sif data
% 1 = GOSAT sif data
% 2 = GOME2 data
RO.sifopt=1;
%BB opt:
%0 = no bb data
%1 = GFEDv4s constraint
%2 = top-down CO constraint
RO.bbopt=2;
%BB uncertainty
%+ve = log; -ve = abs
RO.bbunc=-0.2;
%Mean GPP constraint
%0. No mean GPP 
%1. Fluxcom GPP
RO.gppopt=0;
%OCO-2 options( 1 or 0)
%ri ==1 | ri==8 | ri==9 | ri==10 | ri==11  | ri==12 | ri==13  | ri==14 | ri==15  | ri==21
RO.oco2=0;

%NBE uncertainty mode
%For global uncertainty, provide two values
%For CMS-Flux uncertainty, set to CMS-Flux


    RO.nbeunc = [2,0.02];

%  defval('runname','');
%   defval('s3x3',0);
s3x3=0;
  RO.nbe_s3x3=s3x3;
  
  RO.nbe_product='oco2_sep19';


%PXI options
PXI.RO=RO;
switch opt
    case 1
    PXI.run_details='GCRUN_SEP19_ENSO_LE: 2015-2018 run with CMS (2015-2018) and LS constraints, and prescribed NBE unc';
    PXI.run_name=['GCRUN_SEP19_ENSO_LE',runname];
    PXI.analysis='GCRUN_SEP19_ENSO_LE';
    
end
PXI.ID=811;



%MASK
GC=GEOSChem_xygrids;
MASK=abs(GC.y)<=30;

%Create and summarize PXI, based on available options
PXI=create_gcrun_cbf_files(PXI,MASK,repeat);


end


function PXI=GCRUN_MAR20(opt,runname,repeat);
%CONTINUE FROM HERE

%Details: This is the same as MAY18_3 (RUN 20 in NOV17)
%However, uncertainties here are set as 1/12 and 0.5 for annual & seasonal
%fluxes
%Step 1. Load drivers 
%See script in PROJSCRIPT_CARDAMOM_GLOBAL_TEMPLATE_MAY15
%TESTSCRIPT_CARDAMOM_BUCKET_NOV16.m
%Better to start with a template of RUN29 just in case
%
%
%includes a "smooth" option (s3x3);


%Turn into hydrological CBF file here
%TESTSCRIPT_CARDAMOM_BUCKET_NOV16.m
%Step 3. define gridcells



%only 
%metidx and obsidx
% index writing at end of CBD definition
%(move MET to later on)
%idx=109:180;


% tidx={1:180,109:180,109:180,109:180};
% tnbeidx={[],[],109:156,109:144};
% tbbidx={1:15,10:15,10:15,10:15};%,10:15,10:15,10:15};
% RO.sifopt=[1,1,2,2];
% RO.bbopt=[2,1,2,2];
% RO.bbunc=[-0.2,1.5,-0.2,-0.2];
% RO.gppopt=[1,1,0,0];
% RO.oco2=[0,0,0,1];
%     

RO.tidx=1:216;%Index of timesteps 2009-2018 (Months since 2001)

if opt==3;
    RO.tnbeidx=[];
else
    RO.tnbeidx=109:216;
end

RO.tbbidx=15;
%Options are temporary: 
%Remove once done replicating runs
%SIF options
% 0 = no sif data
% 1 = GOSAT sif data
% 2 = GOME2 data
% 3 = FLUXSAT
RO.sifopt=3;
%BB opt:
%0 = no bb data
%1 = GFEDv4s constraint
%2 = top-down CO constraint
RO.bbopt=2;
%BB uncertainty
%+ve = log; -ve = abs
RO.bbunc=-0.2;
%Mean GPP constraint
%0. No mean GPP 
%1. Fluxcom GPP
RO.gppopt=0;
%OCO-2 options( 1 or 0)
%ri ==1 | ri==8 | ri==9 | ri==10 | ri==11  | ri==12 | ri==13  | ri==14 | ri==15  | ri==21
RO.oco2=0;

%NBE uncertainty mode
%For global uncertainty, provide two values
%For CMS-Flux uncertainty, set to CMS-Flux


    RO.nbeunc = [2,0.02];

%  defval('runname','');
%   defval('s3x3',0);
s3x3=0;
  RO.nbe_s3x3=s3x3;
  
  RO.nbe_product='cms_jan20';


%PXI options
PXI.RO=RO;
switch opt
    case 1
    PXI.run_details='GCRUN_MAR20: 2001-2018 run with CMS (2010-2018) and LS constraints, and prescribed NBE unc';
    PXI.run_name=['GCRUN_MAR20',runname];
    PXI.analysis='GCRUN_MAR20';
    
end
PXI.ID=1000;



%MASK
GC=GEOSChem_xygrids;
%Global 
MASK=GC.y>=-90;

%Create and summarize PXI, based on available options
PXI=create_gcrun_cbf_files(PXI,MASK,repeat);


end



function PXI=GCRUN_NOV19_ENSO_LE(opt,runname,repeat);
%CONTINUE FROM HERE

%Details: This is the same as MAY18_3 (RUN 20 in NOV17)
%However, uncertainties here are set as 1/12 and 0.5 for annual & seasonal
%fluxes
%Step 1. Load drivers 
%See script in PROJSCRIPT_CARDAMOM_GLOBAL_TEMPLATE_MAY15
%TESTSCRIPT_CARDAMOM_BUCKET_NOV16.m
%Better to start with a template of RUN29 just in case
%
%
%includes a "smooth" option (s3x3);


%Turn into hydrological CBF file here
%TESTSCRIPT_CARDAMOM_BUCKET_NOV16.m
%Step 3. define gridcells



%only 
%metidx and obsidx
% index writing at end of CBD definition
%(move MET to later on)
%idx=109:180;


% tidx={1:180,109:180,109:180,109:180};
% tnbeidx={[],[],109:156,109:144};
% tbbidx={1:15,10:15,10:15,10:15};%,10:15,10:15,10:15};
% RO.sifopt=[1,1,2,2];
% RO.bbopt=[2,1,2,2];
% RO.bbunc=[-0.2,1.5,-0.2,-0.2];
% RO.gppopt=[1,1,0,0];
% RO.oco2=[0,0,0,1];
%     

RO.tidx=97:216;%Index of timesteps 2009-2018 (Months since 2001)

if opt==3;
    RO.tnbeidx=[];
else
    RO.tnbeidx=169:216;
end

RO.tbbidx=15;
%Options are temporary: 
%Remove once done replicating runs
%SIF options
% 0 = no sif data
% 1 = GOSAT sif data
% 2 = GOME2 data
RO.sifopt=1;
%BB opt:
%0 = no bb data
%1 = GFEDv4s constraint
%2 = top-down CO constraint
RO.bbopt=2;
%BB uncertainty
%+ve = log; -ve = abs
RO.bbunc=-0.2;
%Mean GPP constraint
%0. No mean GPP 
%1. Fluxcom GPP
RO.gppopt=0;
%OCO-2 options( 1 or 0)
%ri ==1 | ri==8 | ri==9 | ri==10 | ri==11  | ri==12 | ri==13  | ri==14 | ri==15  | ri==21
RO.oco2=0;

%NBE uncertainty mode
%For global uncertainty, provide two values
%For CMS-Flux uncertainty, set to CMS-Flux


    RO.nbeunc = [2,0.02];

%  defval('runname','');
%   defval('s3x3',0);
s3x3=0;
  RO.nbe_s3x3=s3x3;
  
  RO.nbe_product='oco2_sep19';


%PXI options
PXI.RO=RO;
switch opt
    case 1
    PXI.run_details='GCRUN_NOV19_ENSO_LE: 2009-2018 run with CMS (2015-2018) and LS constraints, and prescribed NBE unc';
    PXI.run_name=['GCRUN_NOV19_ENSO_LE',runname];
    PXI.analysis='GCRUN_NOV19_ENSO_LE';
    
end
PXI.ID=811;



%MASK
GC=GEOSChem_xygrids;
MASK=abs(GC.y)<=30;

%Create and summarize PXI, based on available options
PXI=create_gcrun_cbf_files(PXI,MASK,repeat);


end


function PXI=GCRUN_MAR19_TDLE(runname,s3x3);
%CONTINUE FROM HERE

%Details: This is the same as MAY18_3 (RUN 20 in NOV17)
%However, uncertainties here are set as 1/12 and 0.5 for annual & seasonal
%fluxes
%Step 1. Load drivers 
%See script in PROJSCRIPT_CARDAMOM_GLOBAL_TEMPLATE_MAY15
%TESTSCRIPT_CARDAMOM_BUCKET_NOV16.m
%Better to start with a template of RUN29 just in case
%
%
%includes a "smooth" option (s3x3);


%Turn into hydrological CBF file here
%TESTSCRIPT_CARDAMOM_BUCKET_NOV16.m
%Step 3. define gridcells



%only 
%metidx and obsidx
% index writing at end of CBD definition
%(move MET to later on)
%idx=109:180;


% tidx={1:180,109:180,109:180,109:180};
% tnbeidx={[],[],109:156,109:144};
% tbbidx={1:15,10:15,10:15,10:15};%,10:15,10:15,10:15};
% RO.sifopt=[1,1,2,2];
% RO.bbopt=[2,1,2,2];
% RO.bbunc=[-0.2,1.5,-0.2,-0.2];
% RO.gppopt=[1,1,0,0];
% RO.oco2=[0,0,0,1];
%     

RO.tidx=109:180;%Index of timesteps 
RO.tnbeidx=109:156;
RO.tbbidx=10:15;
%Options are temporary: 
%Remove once done replicating runs
%SIF options
% 0 = no sif data
% 1 = GOSAT sif data
% 2 = GOME2 data
RO.sifopt=1;
%BB opt:
%0 = no bb data
%1 = GFEDv4s constraint
%2 = top-down CO constraint
RO.bbopt=2;
%BB uncertainty
%+ve = log; -ve = abs
RO.bbunc=-0.2;
%Mean GPP constraint
%0. No mean GPP 
%1. Fluxcom GPP
RO.gppopt=0;
%OCO-2 options( 1 or 0)
%ri ==1 | ri==8 | ri==9 | ri==10 | ri==11  | ri==12 | ri==13  | ri==14 | ri==15  | ri==21
RO.oco2=0;

%NBE uncertainty mode
%For global uncertainty, provide two values
%For CMS-Flux uncertainty, set to CMS-Flux
RO.nbeunc = [2,0.02];
 defval('runname','');
  defval('s3x3',0);s3x3=1;
  RO.nbe_s3x3=s3x3;

%PXI options
PXI.RO=RO;
PXI.run_details='GCRUN_MAR19_TDLE: 2010-2015 run with CMS (2010-2013) and LS constraints (based on NOV17 run 20), and prescribed NBE unc';
PXI.ID=811;
PXI.run_name=['GCRUN_MAR19_TDLE',runname];
PXI.analysis='GCRUN_MAR19_TDLE';


%MASK
GC=GEOSChem_xygrids;
MASK=abs(GC.y)<=30;

%Create and summarize PXI, based on available options
PXI=create_gcrun_cbf_files(PXI,MASK);


end

function PXI=GCRUN_MAR19_TDLE_2(runname);
%CONTINUE FROM HERE
%SAME AS GCRUN_MAR19_TDLE but annual_unc =0.05
%Details: This is the same as MAY18_3 (RUN 20 in NOV17)
%However, uncertainties here are set as 1/12 and 0.5 for annual & seasonal
%fluxes
%Step 1. Load drivers 
%See script in PROJSCRIPT_CARDAMOM_GLOBAL_TEMPLATE_MAY15
%TESTSCRIPT_CARDAMOM_BUCKET_NOV16.m
%Better to start with a template of RUN29 just in case
%
%
%includes a "smooth" option (s3x3);


%Turn into hydrological CBF file here
%TESTSCRIPT_CARDAMOM_BUCKET_NOV16.m
%Step 3. define gridcells



%only 
%metidx and obsidx
% index writing at end of CBD definition
%(move MET to later on)
%idx=109:180;


% tidx={1:180,109:180,109:180,109:180};
% tnbeidx={[],[],109:156,109:144};
% tbbidx={1:15,10:15,10:15,10:15};%,10:15,10:15,10:15};
% RO.sifopt=[1,1,2,2];
% RO.bbopt=[2,1,2,2];
% RO.bbunc=[-0.2,1.5,-0.2,-0.2];
% RO.gppopt=[1,1,0,0];
% RO.oco2=[0,0,0,1];
%     

RO.tidx=109:180;%Index of timesteps 
RO.tnbeidx=109:156;
RO.tbbidx=10:15;
%Options are temporary: 
%Remove once done replicating runs
%SIF options
% 0 = no sif data
% 1 = GOSAT sif data
% 2 = GOME2 data
RO.sifopt=1;
%BB opt:
%0 = no bb data
%1 = GFEDv4s constraint
%2 = top-down CO constraint
RO.bbopt=2;
%BB uncertainty
%+ve = log; -ve = abs
RO.bbunc=-0.2;
%Mean GPP constraint
%0. No mean GPP 
%1. Fluxcom GPP
RO.gppopt=0;
%OCO-2 options( 1 or 0)
%ri ==1 | ri==8 | ri==9 | ri==10 | ri==11  | ri==12 | ri==13  | ri==14 | ri==15  | ri==21
RO.oco2=0;

%NBE uncertainty mode
%For global uncertainty, provide two values
%For CMS-Flux uncertainty, set to CMS-Flux
RO.nbeunc = [2,0.05];
 defval('runname','');
  defval('s3x3',0);s3x3=1;
  RO.nbe_s3x3=s3x3;

%PXI options
PXI.RO=RO;
PXI.run_details='GCRUN_MAR19_TDLE_2: 2010-2015 run with CMS (2010-2013) and LS constraints (based on NOV17 run 20), and prescribed NBE unc';
PXI.ID=811;
PXI.run_name=['GCRUN_MAR19_TDLE_2',runname];
PXI.analysis='GCRUN_MAR19_TDLE_2';


%MASK
GC=GEOSChem_xygrids;
MASK=abs(GC.y)<=30;

%Create and summarize PXI, based on available options
PXI=create_gcrun_cbf_files(PXI,MASK);


end

function PXI=GCRUN_MAR19_TDLE_3(runname,s3x3);
%CONTINUE FROM HERE
%SAME AS GCRUN_MAR19_TDLE but no NBE
%Details: This is the same as MAY18_3 (RUN 20 in NOV17)
%However, uncertainties here are set as 1/12 and 0.5 for annual & seasonal
%fluxes
%Step 1. Load drivers 
%See script in PROJSCRIPT_CARDAMOM_GLOBAL_TEMPLATE_MAY15
%TESTSCRIPT_CARDAMOM_BUCKET_NOV16.m
%Better to start with a template of RUN29 just in case
%
%
%includes a "smooth" option (s3x3);


%Turn into hydrological CBF file here
%TESTSCRIPT_CARDAMOM_BUCKET_NOV16.m
%Step 3. define gridcells



%only 
%metidx and obsidx
% index writing at end of CBD definition
%(move MET to later on)
%idx=109:180;


% tidx={1:180,109:180,109:180,109:180};
% tnbeidx={[],[],109:156,109:144};
% tbbidx={1:15,10:15,10:15,10:15};%,10:15,10:15,10:15};
% RO.sifopt=[1,1,2,2];
% RO.bbopt=[2,1,2,2];
% RO.bbunc=[-0.2,1.5,-0.2,-0.2];
% RO.gppopt=[1,1,0,0];
% RO.oco2=[0,0,0,1];
%     

RO.tidx=109:180;%Index of timesteps 
RO.tnbeidx=[];
RO.tbbidx=10:15;
%Options are temporary: 
%Remove once done replicating runs
%SIF options
% 0 = no sif data
% 1 = GOSAT sif data
% 2 = GOME2 data
RO.sifopt=1;
%BB opt:
%0 = no bb data
%1 = GFEDv4s constraint
%2 = top-down CO constraint
RO.bbopt=2;
%BB uncertainty
%+ve = log; -ve = abs
RO.bbunc=-0.2;
%Mean GPP constraint
%0. No mean GPP 
%1. Fluxcom GPP
RO.gppopt=0;
%OCO-2 options( 1 or 0)
%ri ==1 | ri==8 | ri==9 | ri==10 | ri==11  | ri==12 | ri==13  | ri==14 | ri==15  | ri==21
RO.oco2=0;

%NBE uncertainty mode
%For global uncertainty, provide two values
%For CMS-Flux uncertainty, set to CMS-Flux
RO.nbeunc = [2,0.02];
 defval('runname','');
  defval('s3x3',0);s3x3=1;
  RO.nbe_s3x3=s3x3;

%PXI options
PXI.RO=RO;
PXI.run_details='GCRUN_MAR19_TDLE_3: LS constraints only (based on NOV17 run 20), otherwise same as GCRUN_MAR19_TDLE';
PXI.ID=811;
PXI.run_name=['GCRUN_MAR19_TDLE_3',runname];
PXI.analysis='GCRUN_MAR19_TDLE_3';


%MASK
GC=GEOSChem_xygrids;
MASK=abs(GC.y)<=30;

%Create and summarize PXI, based on available options
PXI=create_gcrun_cbf_files(PXI,MASK);


end


function PXI=GCRUN_APR19_NBE_BIOMASS(runname);
%CONTINUE FROM HERE

%Details: This is the same as MAY18_3 (RUN 20 in NOV17)
%However, uncertainties here are set as 1/12 and 0.5 for annual & seasonal
%fluxes
%Step 1. Load drivers 
%See script in PROJSCRIPT_CARDAMOM_GLOBAL_TEMPLATE_MAY15
%TESTSCRIPT_CARDAMOM_BUCKET_NOV16.m
%Better to start with a template of RUN29 just in case
%
%
%includes a "smooth" option (s3x3);


%Turn into hydrological CBF file here
%TESTSCRIPT_CARDAMOM_BUCKET_NOV16.m
%Step 3. define gridcells



%only 
%metidx and obsidx
% index writing at end of CBD definition
%(move MET to later on)
%idx=109:180;


% tidx={1:180,109:180,109:180,109:180};
% tnbeidx={[],[],109:156,109:144};
% tbbidx={1:15,10:15,10:15,10:15};%,10:15,10:15,10:15};
% RO.sifopt=[1,1,2,2];
% RO.bbopt=[2,1,2,2];
% RO.bbunc=[-0.2,1.5,-0.2,-0.2];
% RO.gppopt=[1,1,0,0];
% RO.oco2=[0,0,0,1];
%     

RO.tidx=109:180;%Index of timesteps 
RO.tnbeidx=109:156;
RO.tbbidx=10:15;
%Options are temporary: 
%Remove once done replicating runs
%SIF options
% 0 = no sif data
% 1 = GOSAT sif data
% 2 = GOME2 data
RO.sifopt=1;
%BB opt:
%0 = no bb data
%1 = GFEDv4s constraint
%2 = top-down CO constraint
RO.bbopt=2;
%Annual biomass
RO.annualbiomass=1;
%BB uncertainty
%+ve = log; -ve = abs
RO.bbunc=-0.2;
%Mean GPP constraint
%0. No mean GPP 
%1. Fluxcom GPP
RO.gppopt=0;
%OCO-2 options( 1 or 0)
%ri ==1 | ri==8 | ri==9 | ri==10 | ri==11  | ri==12 | ri==13  | ri==14 | ri==15  | ri==21
RO.oco2=0;

%NBE uncertainty mode
%For global uncertainty, provide two values
%For CMS-Flux uncertainty, set to CMS-Flux
RO.nbeunc = [2,0.02];
 defval('runname','');
  defval('s3x3',0);s3x3=1;
  RO.nbe_s3x3=s3x3;

%PXI options
PXI.RO=RO;
PXI.run_details='GCRUN_APR19_NBE_BIOMASS: 2010-2015 run with CMS (2010-2013) and LS constraints (based on NOV17 run 20), and prescribed NBE unc';
PXI.ID=811;
PXI.run_name=['GCRUN_APR19_NBE_BIOMASS',runname];
PXI.analysis='GCRUN_APR19_NBE_BIOMASS';


%MASK
GC=GEOSChem_xygrids;
MASK=abs(GC.y)<=30;

%Create and summarize PXI, based on available options
PXI=create_gcrun_cbf_files(PXI,MASK);


end


function PXI=GCRUN_DEC18_TDLE(runname,s3x3);
%CONTINUE FROM HERE

%Details: This is the same as MAY18_3 (RUN 20 in NOV17)
%However, uncertainties here are set as 1/12 and 0.5 for annual & seasonal
%fluxes
%Step 1. Load drivers 
%See script in PROJSCRIPT_CARDAMOM_GLOBAL_TEMPLATE_MAY15
%TESTSCRIPT_CARDAMOM_BUCKET_NOV16.m
%Better to start with a template of RUN29 just in case
%
%
%includes a "smooth" option (s3x3);


%Turn into hydrological CBF file here
%TESTSCRIPT_CARDAMOM_BUCKET_NOV16.m
%Step 3. define gridcells



%only 
%metidx and obsidx
% index writing at end of CBD definition
%(move MET to later on)
%idx=109:180;


% tidx={1:180,109:180,109:180,109:180};
% tnbeidx={[],[],109:156,109:144};
% tbbidx={1:15,10:15,10:15,10:15};%,10:15,10:15,10:15};
% RO.sifopt=[1,1,2,2];
% RO.bbopt=[2,1,2,2];
% RO.bbunc=[-0.2,1.5,-0.2,-0.2];
% RO.gppopt=[1,1,0,0];
% RO.oco2=[0,0,0,1];
%     

RO.tidx=109:180;%Index of timesteps 
RO.tnbeidx=109:156;
RO.tbbidx=10:15;
%Options are temporary: 
%Remove once done replicating runs
%SIF options
% 0 = no sif data
% 1 = GOSAT sif data
% 2 = GOME2 data
RO.sifopt=1;
%BB opt:
%0 = no bb data
%1 = GFEDv4s constraint
%2 = top-down CO constraint
RO.bbopt=2;
%BB uncertainty
%+ve = log; -ve = abs
RO.bbunc=-0.2;
%Mean GPP constraint
%0. No mean GPP 
%1. Fluxcom GPP
RO.gppopt=0;
%OCO-2 options( 1 or 0)
%ri ==1 | ri==8 | ri==9 | ri==10 | ri==11  | ri==12 | ri==13  | ri==14 | ri==15  | ri==21
RO.oco2=0;

%NBE uncertainty mode
%For global uncertainty, provide two values
%For CMS-Flux uncertainty, set to CMS-Flux
RO.nbeunc = [0.5,0.02];
 defval('runname','');
  defval('s3x3',0);
  RO.nbe_s3x3=s3x3;

%PXI options
PXI.RO=RO;
PXI.run_details='GCRUN_DEC18_TDLE: 2010-2015 run with CMS (2010-2013) and LS constraints (based on NOV17 run 20), and prescribed NBE unc';
PXI.ID=809;
PXI.run_name=['GCRUN_DEC18_TDLE',runname];
PXI.analysis='GCRUN_DEC18_TDLE';


%MASK
GC=GEOSChem_xygrids;
MASK=abs(GC.y)<=30;

%Create and summarize PXI, based on available options
PXI=create_gcrun_cbf_files(PXI,MASK);


end

%GCRUN routines
function PXI=GCRUN_NOV18_CMS_PRIOR_V1
%Step 1. 
%CMS-Flux prior fluxes, describe here
%No atmosphere
%LS constraints = GFEDv4s fire emissions, FLUXCOM GPP


RO.tidx=109:204;%Index of timesteps 
RO.tnbeidx=[];% no atmospheric data
RO.tbbidx=[];
%Options are temporary: 
%Remove once done replicating runs
%SIF options
% 0 = no sif data
% 1 = GOSAT sif data
% 2 = GOME2 data
RO.sifopt=2;
%BB opt:
%0 = no bb data
%1 = GFEDv4s constraint
%2 = top-down CO constraint
RO.bbopt=1;
%BB uncertainty
%+ve = log; -ve = abs
RO.bbunc=-0.2;
%Mean GPP constraint
%0. No mean GPP 
%1. Fluxcom GPP
RO.gppopt=1;
%OCO-2 options( 1 or 0)
%ri ==1 | ri==8 | ri==9 | ri==10 | ri==11  | ri==12 | ri==13  | ri==14 | ri==15  | ri==21
RO.oco2=0;

%NBE uncertainty mode
%For global uncertainty, provide two values
%For CMS-Flux uncertainty, set to CMS-Flux
RO.nbeunc = [0.5,0.02];
 defval('runname','');
  defval('s3x3',0);
  RO.nbe_s3x3=s3x3;
  
  RO.mco=[1e6 0 5e2 0.0005];

%PXI options
PXI.RO=RO;
PXI.run_details='GCRUN_NOV18_1: 2010-2017 and LS constraints';
PXI.ID=809;
PXI.run_name=['GCRUN_NOV18_CMS_PRIOR_V1'];
PXI.analysis='GCRUN_NOV18';%Indicates which folder these go in.

%Create and summarize PXI, based on available options
repeat=1;
if repeat==1
PXI=create_gcrun_cbf_files(PXI);
end

% %Step 2. determine output status
PXI=cardamom_pixel_output_summary(PXI);
% 



end

%GCRUN routines
function PXI=GCRUN_AUG20_CMS_PRIOR
%Step 1. 
%CMS-Flux prior fluxes, describe here
%No atmosphere
%LS constraints = GFEDv4s fire emissions, FLUXCOM GPP


RO.tidx=1:228;%Index of timesteps 
RO.tnbeidx=[];% no atmospheric data
RO.tbbidx=[];
%Options are temporary: 
%Remove once done replicating runs
%SIF options
% 0 = no sif data
% 1 = GOSAT sif data
% 2 = GOME2 data
% 3 = FLUXSAT data
RO.sifopt=3;
%BB opt:
%0 = no bb data
%1 = GFEDv4s constraint
%2 = top-down CO constraint
RO.bbopt=1;
%BB uncertainty
%+ve = log; -ve = abs
RO.bbunc=-0.2;
%Mean GPP constraint
%0. No mean GPP 
%1. Fluxcom GPP
RO.gppopt=0;
%OCO-2 options( 1 or 0)
%ri ==1 | ri==8 | ri==9 | ri==10 | ri==11  | ri==12 | ri==13  | ri==14 | ri==15  | ri==21
RO.oco2=0;

%NBE uncertainty mode
%For global uncertainty, provide two values
%For CMS-Flux uncertainty, set to CMS-Flux
RO.nbeunc = [0,0];
 defval('runname','');
  defval('s3x3',0);
  RO.nbe_s3x3=s3x3;
  
  RO.mco=[1e6 0 5e2 0.0005];

%PXI options
PXI.RO=RO;
PXI.run_details='GCRUN_AUG20: 2010-2017 and LS constraints';
PXI.ID=813;
PXI.run_name=['GCRUN_AUG20_CMS_PRIOR'];
PXI.analysis='GCRUN_AUG20_CMS_PRIOR';%Indicates which folder these go in.

%Create and summarize PXI, based on available options
repeat=1;
if repeat==1
    OPT.gcdriobs='era5';

PXI=create_gcrun_cbf_files(PXI,[],OPT);
end

% %Step 2. determine output status
PXI=cardamom_pixel_output_summary(PXI);
% 



end

function PXI=create_gcrun_cbf_files(PXI,MASK,OPT)

if nargin<3;OPT.repeat=0;end
if isstruct(OPT)==0;OPT.repeat=OPT;end
if isfield(OPT,'gcdriobs')==0;OPT.gcdriobs='erai';end
if isfield(OPT,'repeat')==0;OPT.repeat=0;end

%Load gridded drivers and observations
switch OPT.gcdriobs
    case 'erai'
repeatdriobs=0;GCDRIOBS=READ_GRID_GC_4x5_DRIVERS_OBS(repeatdriobs);
    case 'era5'
        GCDRIOBS=READ_GRID_GC_4x5_DRIVERS_OBS_ERA5;
end
        

[~,LSFRAC]=loadlandseamask('4x5');
GCLSMASK=LSFRAC>0.1;%Test with ALL regions
%Rationale = at <=10% land cover, ocean flux disproportionally contributes
%to gridcell flux, AND gridding errors result in erroneous fluxes.
GC=GEOSChem_xygrids('4x5');

defval('MASK',GC.area>0);

%Define applicable gridcells
[row,col]=find(nanmean(abs(GCDRIOBS.NBE),3)>0 & nanmean(GCDRIOBS.LAI,3)>0.1 & GCLSMASK>0 & MASK);
PXI.MASK=nanmean(abs(GCDRIOBS.NBE),3)>0 & nanmean(GCDRIOBS.LAI,3)>0.1 & GCLSMASK>0 & MASK;
%Step 4: define relevant pixel info
PXI.pixelidmap=GC.x*0;
PXI.x=GC.x;
PXI.y=GC.y;
PXI.area=GC.area;

%Step 1. Load drivers





%Step 0: make CBF template
cbffile=sprintf('CARDAMOM_DISK/CBF_FILES/RUN29/RUN29_%05i.cbf',6000);
CBF0=CARDAMOM_READ_BINARY_FILEFORMAT(cbffile);
CBF0=cardamomfun_clear_cbf_obs(CBF0);
%Add standard priors
CBF0.PARPRIORS(2)=0.5;CBF0.PARPRIORUNC(2)=1.2;
CBF0.PARPRIORS(11)=17.5;CBF0.PARPRIORUNC(11)=1.5;


%Filepath
cbffilepath=sprintf('/Users/abloom/CARDAMOM_DISK/CBF_FILES/%s/',PXI.analysis);
dirmake(cbffilepath);
%Decompress option contents
RO=PXI.RO;
tidx=RO.tidx;%Index of timesteps 
tnbeidx=RO.tnbeidx; %
tbbidx=RO.tbbidx;

%Default RO values

%Annual biomass
RO.annualbiomass=isfield(RO,'annualbiomass');

%****************************anything south of here should be generic*******************
for n=1:numel(row)
    r=row(n);
    c=col(n);
    PXI.lon(n)=GC.x(r,c);
    PXI.lat(n)=GC.y(r,c);
    PXI.r(n)=r;
    PXI.c(n)=c;
    %Henceforth pixelidmap consists of  %04s
    %File naming convention guarantees that if ixels are added/removed,
    %referencing system remains intact
    %Also facilitates external access to pixels
    PXI.pixelid{n}=sprintf('%02i%02i',r,c);
    PXI.pixelidmap(r,c)=str2num(PXI.pixelid{n});
    PXI.cbffilename{n,1}=sprintf('%s%s_%04s.cbf',cbffilepath,PXI.run_name,PXI.pixelid{n});
end

%Step 5: define and write cbf files
PXI.nodays=diff(minmax(tidx))+1;   
PXI.dates=datenum('01/01/2001')+[365.25/24+(tidx(1)-1)*365.25/12:365.25/12:+(tidx(end))*365.25/12];



%ONLY DO IF 
%(a) file does not exist, or
%(b) remakecbf==1

remake_cbf=OPT.repeat;

%
for n=1:numel(row);
    if remake_cbf==1 | isempty(dir(PXI.cbffilename{n,1}))

    r=PXI.r(n);c=PXI.c(n);
    CBF=CBF0;
    %Ensure MET and OBS are the right size
%     CBD.MET=CBD.MET(tidx,:);
%     CBD.OBS=CBD.OBS(tidx,:);
    
    CBF.ID=PXI.ID;
    CBF.nomet=9;
    %Fill in with full timeseries of project
    %Truncate to required timespan later
    N=size(GCDRIOBS.mintemp,3);
    CBF.MET(1:N,1)=GCDRIOBS.dates-datenum('01/01/2001');
    CBF.MET(1:N,2)=GCDRIOBS.mintemp(r,c,:);
    CBF.MET(:,3)=GCDRIOBS.maxtemp(r,c,:);
    CBF.MET(:,4)=GCDRIOBS.srad(r,c,:);
    CBF.MET(1:N,5)=GCDRIOBS.CO2;
    CBF.MET(1:N,6)=mod(CBF.MET(1:N,1),365.25);
    CBF.MET(:,7)=GCDRIOBS.BA(r,c,:);
    CBF.MET(:,8)=GCDRIOBS.vpd(r,c,:);
    CBF.MET(:,9)=GCDRIOBS.prec(r,c,:)*1e3;%mm/day
    CBF.LAT=PXI.lat(n);
    %LAI
    CBF.OBS.LAI(1:N)=nanzerofill(GCDRIOBS.LAI(r,c,:));
    
    
    %Writing SIF to file
    switch RO.sifopt
        case 1
        %WRITE GOSAT SIF INTO CBF FILE HERE
        SIF=squeeze(GCDRIOBS.SIF(r,c,:));
                CBF.OBSUNC.GPP.gppabs=0;

        case 2
            %BUG: pre-2010 SIF is zero (should extend back to 2007)
        SIF=squeeze(GCDRIOBS.SIFgome2(r,c,:));
                CBF.OBSUNC.GPP.gppabs=0;

        case 0
        SIF=[1:N]*0-9999;
                CBF.OBSUNC.GPP.gppabs=0;

        case 3 
            SIF=squeeze(GCDRIOBS.SIFfluxsat(r,c,:));
                    CBF.OBSUNC.GPP.gppabs=1;

    end
    
        SIF(isfinite(SIF)==0 | SIF<=0)=-9999;
        % SIFUNC(SIF<0)=-9999;
        %Entering SIF in CBF
        CBF.OBS.GPP=SIF;
        
  
  %GPP constraint 
  if RO.gppopt==1
        CBF.OTHER_OBS.MGPP.mean=mean(mean(GCDRIOBS.MPIGPPmean(r,c,:,:),3),4);
%        if CBD.OTHERPRIORS(6)<0; disp('ERROROROROROROORRROROR');keyboard;end
          CBF.OTHER_OBS.MGPP.unc=-0.2*CBF.OTHER_OBS.MGPP.mean;%disp('CBF.OTHERPRIORSUNC(6): Change in next simulation normal (from log-normal)');
        %Remove priors on Ceff;
        CBF.PARPRIORS(11)=-9999;
        CBF.PARPRIORUNC(11)=-9999;
  end
  
            
    
%CBD.OBS(tnbeidx{ri},3)=GCDRIOBS.NBE(r,c,tnbeidx{ri});
CBF.OBS.NBE(1:N)=-9999;
    if isempty(tnbeidx)==0
        
            %Choose NBE product
            
            switch RO.nbe_product
                case 'b73'
           
            if isfield(RO,'nbe_s3x3') & RO.nbe_s3x3==1;
                    CBF.OBS.NBE(tnbeidx)=GCDRIOBS.NBE73_s3x3(r,c,tnbeidx);         
            else
                     CBF.OBS.NBE(tnbeidx)=GCDRIOBS.NBE73(r,c,tnbeidx);         
            end
            
                case 'oco2_sep19'
                    
                    CBF.OBS.NBE(tnbeidx)=GCDRIOBS.NBE_OCO2_SEP19(r,c,tnbeidx);
                    
                                    case 'cms_jan20'
                    CBF.OBS.NBE(tnbeidx)=GCDRIOBS.NBE_CMS_JAN20(r,c,tnbeidx);

                                         
            end
            
            
            
            
            
            if isstr(RO.nbeunc) & strcmp(RO.nbeunc,'CMS-Flux');
            CBF.OBSUNC.NBE.seasonal_unc=GCDRIOBS.NBEunc_seasonal(r,c);
            CBF.OBSUNC.NBE.annual_unc=GCDRIOBS.NBEunc_annual(r,c);
            %Prescribing seasonal and inter-annual uncertainties here
            elseif isnumeric(RO.nbeunc) & numel(RO.nbeunc)==2
                CBF.OBSUNC.NBE.seasonal_unc=RO.nbeunc(1);
                CBF.OBSUNC.NBE.annual_unc=RO.nbeunc(2);
            end
    end
    
    %Adding OCO-2 constraint
    if RO.oco2==1;CBF.OBS.NBE(169:180)=GCDRIOBS.NBEoco2(r,c,:);end
    %*********Biomass burning  constraints here***********
        %Ensuring both datasets have fires
        %This tends to be an issue where fires are virtually 0
          %Introducing GFED BB constraint here
          if RO.bbopt>0;
              %GFED constraint
        if RO.bbopt==1
        %Write GFEDv4 data
        if mean(GCDRIOBS.FireC(r,c,:),3)>0 & max(CBF.MET(:,7))>0;CBF.OTHER_OBS.MBiomass.mean=mean(GCDRIOBS.FireC(r,c,:));
        elseif mean(GCDRIOBS.FireC(r,c,:),3)==0 & max(CBF.MET(:,7))>0; disp('no significant fire emissions included in GFED');
        end
            
        end
        
        %Top-down constraint
        if RO.bbopt==2;
        if max(GCDRIOBS.BBCO(r,c,tbbidx))>0 & max(CBF.MET(tbbidx,7))>0
        CBF.OTHER_OBS.MFire.mean=mean(GCDRIOBS.BBCO(r,c,tbbidx));
        end
        end
          end
        
          
                  %Biomass burnig Uncertainty structure
          if CBF.OTHER_OBS.MFire.mean>-9999
        if RO.bbunc<0
            %-ve option is read as absolute uncertainty by CARDAMOM
        CBF.OTHER_OBS.MFire.unc=CBF.OTHER_OBS.MFire.mean*RO.bbunc;
        elseif RO.bbunc>0;
            %+ve option is read as log-uncertainty
            CBF.OTHER_OBS.MFire.unc=RO.bbunc;
        end
          end
          
          %***********done with biomass burning constraints*********
            
    
            %Indexing
    CBF.MET=CBF.MET(tidx,:);
    obsnames={'NBE','LAI','GPP'};
    for nn=1:numel(obsnames)
    CBF.OBS.(obsnames{nn})=CBF.OBS.(obsnames{nn})(tidx);
    %CONTINUE
    end
    %CBF.nodays=diff(minmax(tidx))+1;
          
          
    %Adding mean LAI constraint only
        CBF.OTHER_OBS.MLAI.mean=mean(CBF.OBS.LAI(CBF.OBS.LAI>-9999));
        %CBD.OTHERPRIORSUNC(5)=mean(CBD.OBS(CBD.OBS(:,2)>-9999,2))*0.2;
        %CBD.OTHERPRIORUNC(5)=CBD.OTHERPRIORS(5)*0.5;
        %CBF.OTHER_OBS.MLAI.unc=-0.2*CBF.OTHER_OBS.MLAI.mean;
        CBF.OTHER_OBS.MLAI.unc=1.2;

    %*********************
    %    Biomass
    
    if RO.annualbiomass==1
        %Using YY annually gridded biomass
        CBF.OTHER_OBS.MBiomass.mean=-9999;
        CBF.OBS.ABGB=CBF.OBS.LAI*0-9999;
        CBF.OBS.ABGB(12:12:end)=GCDRIOBS.annualbiomass(r,c,10:15);
        CBF.OBSUNC.ABGB.unc=max(1.05,1+std(GCDRIOBS.annualbiomass(r,c,10:15))/mean(GCDRIOBS.annualbiomass(r,c,10:15)));

        
    elseif RO.annualbiomass==0;
        %Using Saatchi et al. mean tropical biomass
    CBF.OTHER_OBS.MBiomass.mean=nanzerofill(GCDRIOBS.TABGB(r,c));
    UNC=max(1.5,1.5*12.5*100/GCDRIOBS.TABGB(r,c));if isinf(UNC);UNC=-9999;end
    CBF.OTHER_OBS.MBiomass.unc=UNC;
        end
    
    %OSM
    CBF.PARPRIORS(23)= nanzerofill(GCDRIOBS.SOM(r,c));CBF.PARPRIORUNC(23)=1.5;
    % 
%     CBF.PARPRIORS(12)=nanzerofill(GCDRIOBS.PEAKLAIDAY(r,c)-365.25/6+365.25*2);
%     CBF.PARPRIORUNC(12)=1.5;
%     %Leaf fall
%     CBF.PARPRIORS(15)= nanzerofill(GCDRIOBS.PEAKLAIDAY(r,c)+365.25/6+365.25*2);
%     CBF.PARPRIORUNC(15)=1.5;
% 
%   


    %mean-zero NBE optiony
    if isfield(RO,'nbezero') & RO.nbezero==1
        CBF.OBS.NBE(CBF.OBS.NBE>-9999)=normvec(CBF.OBS.NBE(CBF.OBS.NBE>-9999),3);
    end
        
    
    %writetofile
    CARDAMOM_WRITE_BINARY_FILEFORMAT(CBF,PXI.cbffilename{n,1});
    disp(PXI.cbffilename{n,1});
    end
%Check for erroneous CBF fileds
end
%VPD units = hPa.
%Prec units = mm /day
% CBF2.MET(:,8)=VPD.data(r,c,1:120);
% CBF2.MET(:,9)=EPREC.data(r,c,1:120)*1e3;




end

function PXIall=cardamom_pixel_output_summary(PXIall)

%Step 1. for each PXI gridcell, count outputs and store in CAR4x5

for nn=1:numel(PXIall)
    if isempty(PXIall(nn).ID)==0
        PXI=PXIall(nn);
PXI.nochains=4;
if isfield(PXI.RO,'mco')==0
mco=[1e6 0 2e3 0.0001];
else
    mco=PXI.RO.mco;
end


bytesperparameter=2*4; %64-bit, or 8-bytes


PXI.chains=zeros(numel(PXI.lat),PXI.nochains,1);

    cbrpath=sprintf('CARDAMOM_DISK/CBR_FILES/%s',PXI.analysis);
    %counting CBR files    
    %Looping through all chains
    MA=CARDAMOM_MODEL_LIBRARY(PXI.ID);
    for p=1:numel(PXI.lat);
        for c=1:PXI.nochains
            PXI.cbrfilename{p,c}=sprintf('%s/%s_%s_%i.cbr',cbrpath,PXI.run_name,PXI.pixelid{p},c);
            PXI.supercomp_command{p,c}=sprintf('./C/projects/CARDAMOM_MDF/CARDAMOM_MDF.exe  EXPORTS/CBF_FILES/%s_%s.cbf EXPORTS/CBR_FILES/%s_%s_%i.cbr %i %i %i %f', PXI.run_name,PXI.pixelid{p},PXI.run_name,PXI.pixelid{p},c,mco);
            %store missing pixelmco=[1e6 0 2e3];
            A=dir(PXI.cbrfilename{p,c});
            bytesperfile=bytesperparameter*[PXI.RO.mco(1)./PXI.RO.mco(3)]*MA.nopars;
            if isempty(A)==0 & A.bytes==bytesperfile;PXI.chains(p,c)=1;end; %Completed full chain
            if isempty(A)==0 & A.bytes<bytesperfile;PXI.chains(p,c)=2;end; %Partially completed chain
        end
    end
%make map of chains
chainmap=PXI.MASK*0;
chainmap_partial=PXI.MASK*0;
chainmap(PXI.pixelidmap>0)=sum(double(PXI.chains==1),2);
chainmap_partial(PXI.pixelidmap>0)=sum(double(PXI.chains>0),2);
chainmap(PXI.pixelidmap==0)=NaN;
chainmap_partial(PXI.pixelidmap==0)=NaN;
PXI.chainmap=chainmap;
PXI.chainmap_partial=chainmap_partial;



        if numel(PXIall)>1;PXIall(nn)=PXI;else PXIall=PXI;end

    end
end



end

%Glpobal 05x05 grid routines
function PXItemplate=GL05RUN_MAR19(OPT)
%Step 1. Load drivers 
%See script in PROJSCRIPT_CARDAMOM_GLOBAL_TEMPLATE_MAY15
%TESTSCRIPT_CARDAMOM_BUCKET_NOV16.m
%Better to start with a template of RUN29 just in case
%New system (in the interest of memory)
%Make sure to include PXI backup
if nargin==0;OPT=[];end
%Step 1. Define all files
[~,LSFRAC]=loadlandseamask(0.5);

%1.1 load LAI for baseline criterion
LAI=quickload('DATASCRIPT_READ_MODIS_MONTHLY_LAI_MAY17(0.5)');
PXItemplate.MASK=nanmean(LAI.data,3)>0.1;


%Step 1.2 run names
if isfield(OPT,'analysis')==0;OPT.analysis='GL05RUN_MAR19';end
if isfield(OPT,'run_name')==0;OPT.run_name='GL05RUN_MAR19';end
if isfield(OPT,'run_details')==0;OPT.run_details='GL05RUN_MAR19';end


    PXItemplate.analysis=OPT.analysis;
PXItemplate.run_name=OPT.run_name;
PXItemplate.run_details=OPT.run_details;

projdir=sprintf('CARDAMOM_DISK/CBF_FILES/%s',PXItemplate.analysis);
if isempty(projdir);mkdir(projdir);end

%PXItemplate.ID=809;
PXItemplate.ID=1000;
%Step 1.2.1 declaremaps
[x,y,A]=loadworldmesh(0.5);
PXItemplate.x=x;PXItemplate.y=y;PXItemplate.area=A;
PXItemplate.nodays=180;
PXItemplate.dates=datenum('01/01/2001')+365.25/24:365.25/12:datenum('01/01/2017');

%Step 1.3 pixel info
%name convention (row_col, 1,1=SW)
PXItemplate.pixelidmap=x*0;
PXItemplate.pixelnmap=x*0;
[rows,cols]=find(PXItemplate.MASK);
for n=1:numel(rows);
    r=rows(n);
    c=cols(n);
    PXItemplate.lon(n)=x(r,c);
    PXItemplate.lat(n)=y(r,c);
    PXItemplate.r(n)=r;
    PXItemplate.c(n)=c;
    PXItemplate.pixelid{n}=sprintf('%03i%03i',r,c);
    PXItemplate.pixelidmap(r,c)=str2num(PXItemplate.pixelid{n});
    PXItemplate.pixelnmap(r,c)=n;
end

%STEP 1.4; paths and execs
%opt=1 sets relatve path
PXItemplate=make_cbf_files(PXItemplate,1);

%
%PXItemplate=CARDAMOM_PROJECT_SETUP(PXItemplate);






%Step 2. cycle through each quantity, (1) open file, (2), write, (3) close file
%Step 2.1: write empty template file
%cbffiletemplate=sprintf('CARDAMOM_DISK/CBF_FILES/RUN29/RUN29_%05i.cbf',6000);
CBFtemplate=cardamomfun_clear_cbf_obs(CARDAMOM_CBF_TEMPLATE(809));
CBFtemplate.PARPRIORS(2)=0.5;CBFtemplate.PARPRIORUNC(2)=1.2;
CBFtemplate.PARPRIORS(11)=17.5;CBFtemplate.PARPRIORUNC(2)=1.5;

CBFtemplate.MET(1:192,1:9)=-9999;
%CBD.OBS(1:192,1:3)=-9999;
%CBFtemplate.nomet=9;
CBFtemplate.ID=PXItemplate.ID;
CBFtemplate.MET(1:192,1)=[0:191]*365.25/12+365.25/24;%Units are days since a
DATA=DATASCRIPT_READ_NOAA_MAUNA_LOA_CO2_SEP17;
CBFtemplate.MET(1:192,5)=DATA.data(DATA.year>2000 & DATA.year<2017);
CBFtemplate.MET(1:192,6)=mod(CBFtemplate.MET(1:192,1),365.25);

CBFtemplate.OBS.NBE=[];
CBFtemplate.OBS.GPP=[1:192]*0-9999;
CBFtemplate.OBS.LAI=[1:192]*0-9999;
CBFtemplate.OBS.EWT=[1:192]*0-9999;


cbfyears=floor(CBFtemplate.MET(:,1)/365.25)+2001;
cbfmonths=mod([1:192]-1,12)+1;
cbfdate=CBFtemplate.MET(:,1);


%Writing template files  wit h global info only
% for n=1:numel(PXItemplate.r);
%     %Writing LAT and 2001-2016 LAI
%     CARDAMOM_WRITE_BINARY_FILEFORMAT(CBF,PXItemplate.cbffilename{n});
% end
% 
GRACE=quickload('DATASCRIPT_READ_GRACE_MASCONS_MAR19');

gracedate=GRACE.time-datenum('01/01/2001');
    cbfgrpts{1}=closest(gracedate(1),cbfdate):closest(gracedate(end),cbfdate);
    cbfgrpts{2}=closest(cbfdate(1),gracedate):closest(cbfdate(end),gracedate);

%Step 3. define gridcells
%Step 4: define relevant pixel info
%IF NEEDED, load drivers separately 
%EPREC=quickload('DATASCRIPT_READ_ERAI_MONTHLY_JUL18(''prec'',0.5)');
EPREC=quickload('DATASCRIPT_READ_ERAI_MONTHLY_JUL18(''prec'',0.5)');
ET2M=quickload('DATASCRIPT_READ_ERAI_MONTHLY_JUL18(''t2m'',0.5)');
ESRAD=quickload('DATASCRIPT_READ_ERAI_MONTHLY_JUL18(''srad'',0.5)');
ED2M=quickload('DATASCRIPT_READ_ERAI_MONTHLY_JUL18(''d2m'',0.5)');
GBA=quickload('DATASCRIPT_READ_GFED4_BA_DATA_OCT15(1)');
HWSD=quickload('DATASCRIPT_READ_HWSD_MAP(0.5)');
TABGB=quickload('DATASCRIPT_READ_SAATCHI_BIOMASS_MAP(0.5)');

 if isfield(OPT,'csif')==0;OPT.csif=0;end
 
 if OPT.csif==0;
    GSIF=quickload('DATASCRIPT_READ_GOME_2_FLUORESCENCE_SEP17');
 else
     GSIF=quickload('DATASCRIPT_READ_CSIF_DATA_APR19');
     GSIF.data05x05=GSIF.data;
 end


peaklai=projscript_approximate_peak_lai(LAI.data);
GPP=quickload('DATASCRIPT_READ_FLUXCOM_DATASETS_OCT17');
%Loading GRACE data here



%Load GFED burned area
[~,~,A025]=loadworldmesh(0.25);
BA=mapsfun_coarsen_resolution(GBA.ba,A025,2);
VPD=(SCIFUN_H2O_SATURATION_PRESSURE(ET2M.datamax) - SCIFUN_H2O_SATURATION_PRESSURE(ED2M.datamax))*10;
%For comparison: for yr=2001:2016; GF4=DATASCRIPT_READ_GFED4_DATA_MAY16(yr);GFEDCannual025x025(:,:,yr-2000)=mean(GF4.ES(:,:,:,4),3);end
% [~,~,A025]=loadworldmesh(0.25);
% GFEDCannual05x05=mapsfun_coarsen_resolution(GFEDCannual025x025,A025,2);
%save DUMPFILES/GFED_2001_2016_05x05_NOCODE.mat GFEDCannual05x05
%Units are gC/m2/month
load DUMPFILES/GFED_2001_2016_05x05_NOCODE.mat GFEDCannual05x05

idx=1:192;

for ri=1
   %Setting all time-dependent terms and writing to file
for n=1:numel(rows);
    CBF=CBFtemplate;
    r=PXItemplate.r(n);
    c=PXItemplate.c(n);
    CBF.LAT=PXItemplate.lat(n);
    %LAI
    CBF.OBS.LAI(1:192)=nanzerofill(LAI.data(r,c,:));
    
    
    %Step 1. Load CBD file
    %CBF=CARDAMOM_READ_BINARY_FILEFORMAT(PXItemplate.cbffilename{n,ri});
    %Step 2. Fill in drivers
    r=PXItemplate.r(n);c=PXItemplate.c(n);
    CBF.MET(:,2)=ET2M.datamin(r,c,idx);
    CBF.MET(:,3)=ET2M.datamax(r,c,idx);
    CBF.MET(:,4)=ESRAD.data(r,c,idx);
    CBF.MET(:,7)=BA(r,c,idx);
    CBF.MET(:,8)=VPD(r,c,idx);
    CBF.MET(:,9)=EPREC.data(r,c,idx)*1e3;%mm/day
    



    %SIF
      SIF=squeeze(GSIF.data05x05(r,c,:));
        SIF(isfinite(SIF)==0 | SIF<0)=-9999;
        % SIFUNC(SIF<0)=-9999;
        %Entering SIF in CBF
        CBF.OBS.GPP(cbfyears>=min(GSIF.year) & cbfyears<=max(GSIF.year))=SIF;
        CBF.OBSUNC.GPP.gppabs=0;
     


    EWT=squeeze(GRACE.data(r,c,:));
    EWT(isfinite(EWT)==0 )=-9999;

   if isfield(OPT,'grace')==0;OPT.grace=1;end
       

     if OPT.grace==1;
            CBF.OBS.EWT(cbfgrpts{1})=EWT(cbfgrpts{2});
    end
        

    %Write GFEDv4 data
    %This tends to be an issue where fires are virtually 0
    %COnvert to gC/m2/d
      if mean(GFEDCannual05x05(r,c,:))>0 & max(CBF.MET(:,7))>0
	 CBF.OTHER_OBS.MFire.mean=mean(GFEDCannual05x05(r,c,:),3)*12/365.25;
     CBF.OTHER_OBS.MFire.unc=1.5;
      end
     
     %Write MPI GPP with log2 uncertainty here
     %remove pars(11) constraint
        CBF.OTHER_OBS.MGPP.mean=nanzerofill(mean(mean(GPP.data(r,c,:,:),3),4));
        CBF.OTHER_OBS.MGPP.unc=1.5;

        
         CBF.OTHER_OBS.MLAI.mean=mean(CBF.OBS.LAI(CBF.OBS.LAI>-9999));
        %CBD.OTHERPRIORSUNC(5)=mean(CBD.OBS(CBD.OBS(:,2)>-9999,2))*0.2;
        %CBD.OTHERPRIORUNC(5)=CBD.OTHERPRIORS(5)*0.5;
        CBF.OTHER_OBS.MLAI.unc=1.5;

    
    %    Biomass
    CBF.OTHER_OBS.MBiomass.mean=nanzerofill(TABGB.data(r,c));
    UNC=max(1.5,1.5*12.5*100/TABGB.data(r,c));if isinf(UNC);UNC=-9999;end
     CBF.OTHER_OBS.MBiomass.unc=UNC;
    %SOM
    CBF.PARPRIORS(23)= nanzerofill(HWSD.data(r,c));
    CBF.PARPRIORUNC(23)=1.5;

    
    %writetofile
    disp(sprintf('writing %s',PXItemplate.cbffilename{n,ri}));
        CARDAMOM_WRITE_BINARY_FILEFORMAT(CBF,PXItemplate.cbffilename{n,ri});

end
end
%VPD units = hPa.
%Prec units = mm /day
% CBF2.MET(:,8)=VPD.data(r,c,1:120);
% CBF2.MET(:,9)=EPREC.data(r,c,1:120)*1e3;


end

%GENERAL/SHARED functions
function PXI=make_cbf_files(PXI,opt)

if nargin==1;opt=0;end

for n=1:numel(PXI.pixelid)
    if opt==0
    cbffilepath=sprintf('/Users/abloom/CARDAMOM_DISK/CBF_FILES/%s/',PXI.analysis);
    elseif opt==1
    cbffilepath=sprintf('CARDAMOM_DISK/CBF_FILES/%s/',PXI.analysis);
    end
    PXI.cbffilename{n,1}=sprintf('%s%s_%s.cbf',cbffilepath,PXI.run_name,PXI.pixelid{n});
end

end

function GCDRIOBS=READ_GRID_GC_4x5_DRIVERS_OBS(repeat)


%This function is a universal gridding of all/any datasets at 4x5  
YEARS=2001:2018;


defval('repeat',0);
%Dumpfile and stuff here
%GCDRIOBS 
%dumpfilename='DUMPFILES/PROJSCRIPT_CARDAMOM_CMS_GC4x5_DRIOBS_NOV18.mat';
dumpfilename='DUMPFILES/PROJSCRIPT_CARDAMOM_CMS_GC4x5_DRIOBS_OCT19.mat';

if isempty(dir(dumpfilename))==0 && repeat==1;delete(dumpfilename);end

%Create/open file
GCDRIOBS=matfile(dumpfilename,'Writable',true);
GCDRIOBS.dates=datenum(sprintf('01/01/%4i',YEARS(1)))+365.25/24:365.25/12:datenum(sprintf('01/01/%4i',YEARS(end)+1));


    %FOr any additional datasets, update & import through function below


[~,~,A05]=loadworldmesh(0.5);
[~,LSFRAC]=loadlandseamask('4x5');
[~,lsfrac05]=loadlandseamask(0.5);
[~,lsfrac025]=loadlandseamask(0.25);
[~,lsfrac1]=loadlandseamask(1);

GC=GEOSChem_xygrids;

%Read ERA-interim (new dataset function);
%2001-2017
metobsidx=1:numel(YEARS)*12;
metobsidx_str=sprintf('%i:%i',metobsidx(1),metobsidx(end));

repeatbiomass=0;
if matfilecontains(GCDRIOBS,'annualbiomass')==0 || repeat==1 || repeatbiomass==1;
    BTD=quickload('DATASCRIPT_READ_YY_BIOMASS_MAPS_APR19');
    %Only 2001-2017
    yrs=BTD.year<2018 & BTD.year>2000;
    BTD.biomass=BTD.biomass(:,:,yrs);
    M3D=repmat(lsfrac05>0.5,[1,1,size(BTD.biomass,3)]);
    %Remove sea
BTD.biomass(M3D==0)=NaN;
%Set land values to zero;
BTD.biomass(isnan(BTD.biomass) & M3D)=0;
GCDRIOBS.annualbiomass=GEOSChem_regular_grid_to_GC(BTD.biomass);% Units = gC/m2
end


if matfilecontains(GCDRIOBS,'prec')==0 || repeat==1;
EPREC=quickload('DATASCRIPT_READ_ERAI_MONTHLY_JUL18(''prec'',0.5)');EPREC.data(EPREC.data<0)=0;
GCDRIOBS.prec=indeks(GEOSChem_regular_grid_to_GC(EPREC.data),[':,:,',metobsidx_str]);
clear EPREC;
end


if matfilecontains(GCDRIOBS,'vpd')==0 || repeat==1;
ET2M=quickload('DATASCRIPT_READ_ERAI_MONTHLY_JUL18(''t2m'',0.5)');
GCDRIOBS.mintemp=indeks(GEOSChem_regular_grid_to_GC(ET2M.datamin),[':,:,',metobsidx_str]);
GCDRIOBS.maxtemp=indeks(GEOSChem_regular_grid_to_GC(ET2M.datamax),[':,:,',metobsidx_str]);

ED2M=quickload('DATASCRIPT_READ_ERAI_MONTHLY_JUL18(''d2m'',0.5)');
GCDRIOBS.vpd=GEOSChem_regular_grid_to_GC(indeks(SCIFUN_H2O_SATURATION_PRESSURE(ET2M.datamax),[':,:,',metobsidx_str]) - indeks(SCIFUN_H2O_SATURATION_PRESSURE(ED2M.datamax),[':,:,',metobsidx_str]))*10;

clear E*2M ;
end


if matfilecontains(GCDRIOBS,'srad')==0 || repeat==1;
ESRAD=quickload('DATASCRIPT_READ_ERAI_MONTHLY_JUL18(''srad'',0.5)');
GCDRIOBS.srad=indeks(GEOSChem_regular_grid_to_GC(ESRAD.data),[':,:,',metobsidx_str]);
clear ESRAD;
end

%MPI GPP
%replace "Always 0" values with NaNs
%Use lsmask05 = 0.5 as a cutoff;
%assume (a) values outside are caterorized as ls=0, and (b) values inside
%are zeros (not empty). Apply this approach to standardize across all
%datasets. prescribe all values outside 0.5 as NaN
%First step.
%mask=mean(mean(GPP.data,3),4)==0 & loadlandseamask(0.5)<1
if matfilecontains(GCDRIOBS,'MPIGPPmean')==0 || repeat==1;
GPP=quickload('DATASCRIPT_READ_FLUXCOM_DATASETS_OCT17');
M4D=repmat(lsfrac05>0.5,[1,1,size(GPP.data,3),size(GPP.data,4)]);
GPP.data(M4D==0)=NaN;GPP.data(isnan(GPP.data) & M4D)=0;
GCDRIOBS.MPIGPPmean=GEOSChem_regular_grid_to_GC(GPP.data);% Units = gC/m2/day
end

%Burned area
%NRW CODE:
repeatba=0;
if matfilecontains(GCDRIOBS,'BA')==0 || repeat==1 || repeatba==1;

GBA=[];
GCE=[];
for yr=YEARS
    GF4=DATASCRIPT_READ_GFED4_DATA_MAY16(yr);
    GBA(:,:,(yr-2001)*12+[1:12])=GF4.BA;
    GCE(:,:,(yr-2001)*12+[1:12])=GF4.ES(:,:,:,4);
end

 M4D=repmat(lsfrac025>0.5,[1,1,size(GBA,3),size(GBA,4)]);
 GBA(M4D==0)=NaN;GBA(isnan(GBA) & M4D)=0;
 GCE(M4D==0)=NaN;GCE(isnan(GCE) & M4D)=0;
 %Ensures land fire-free regions = 0 but sea regions are = NaN
 GCDRIOBS.BA=indeks(GEOSChem_regular_grid_to_GC(GBA),[':,:,',metobsidx_str]);
 GCDRIOBS.FireC=indeks(GEOSChem_regular_grid_to_GC(GCE),[':,:,',metobsidx_str])*12/365.25;%g species/m2/month
 %nantotal(nanmean(GCDRIOBS.FireC,3).*GC.area)*365.25/1e15

 disp('GFEDv4s beta does not provide 2017-18 burned area');
 disp('Using GFEDv4s 2001-2016 BA/Emissions ratio')
 for m=1:24
     BA17(:,:,m)=sum(GCDRIOBS.BA(:,:,m:12:end-24),3)./sum(GCDRIOBS.FireC(:,:,m:12:end-24),3).*GCDRIOBS.FireC(:,:,end-24+m);
 end
 %storing and replacing NaNs with zeros (Nans occur when 2010-2016 fires
 %are zero)
GCDRIOBS.BA(:,:,end-23:end)= BA17;
GCDRIOBS.BA=nan2zero(GCDRIOBS.BA);
%OLD CODE: used GFEDv4 BA data
% GBA0=quickload('DATASCRIPT_READ_GFED4_BA_DATA_OCT15(1)');
 %M4D=repmat(lsfrac025>0.5,[1,1,size(GBA.ba,3),size(GBA.ba,4)]);
% GBA.ba(M4D==0)=NaN;GBA.ba(isnan(GBA.ba) & M4D)=0;%Ensures land fire-free regions = 0 but sea regions are = NaN
% GCDRIOBS.BA=indeks(GEOSChem_regular_grid_to_GC(GBA.ba),[':,:,',eraidx]);
end

if matfilecontains(GCDRIOBS,'SOM')==0 || repeat==1
%Soil C
HWSD=quickload('DATASCRIPT_READ_HWSD_MAP');
GCDRIOBS.SOM=GEOSChem_regular_grid_to_GC(HWSD.data);

end
%LAI
% LAI=quickload('DATASCRIPT_READ_MODIS_MONTHLY_LAI');LAI.data(isfinite(LAI.data)==0)=0;
% LAI4x50=GEOSChem_grid_1x1_to_GC(LAI.data);





repeatlai=0;
if matfilecontains(GCDRIOBS,'LAI')==0 || repeat==1 || repeatlai==1
LAI=DATASCRIPT_READ_MODIS_MONTHLY_LAI_MAY17('4x5');
GCDRIOBS.LAI=LAI.data(:,:,metobsidx(1:192));
GCDRIOBS.LAI(:,:,193:metobsidx(end))=NaN;
GCDRIOBS.PEAKLAIDAY=projscript_approximate_peak_lai(GCDRIOBS.LAI(:,:,1:192));
end



%Next: ABGB
if matfilecontains(GCDRIOBS,'TABGB')==0 || repeat==1
TABGB=quickload('DATASCRIPT_READ_SAATCHI_BIOMASS_MAP(0.25)');
TABGB.data(TABGB.data==0 | lsfrac025<0.5)=NaN;
GCDRIOBS.TABGB=GEOSChem_regular_grid_to_GC(TABGB.data);
end


%Normalizing NBE and BB fluxes by land surface area
[~,LSFRAC]=loadlandseamask('4x5');


if matfilecontains(GCDRIOBS,'BBCO')==0 || repeat==1
%Load CO emissions, and store mean fire C loss (total)
DATA=quickload('PROJSCRIPT_CO_BAYESIAN_PARTITIONING_MAR17');
CO2CO=permute(ncread('OUTPUTS/CMS_FIRES_CO2CO/CMS_FIRES_CO2CO_NOV16.nc','CO2:CO'),[2,1,3]);
%mean CO emissions
%Convert to total C emissions
%CO + CO*CO2CO
%Convert to daily flux
%For comparison: 
%for yr=2001:2015; GF4=DATASCRIPT_READ_GFED4_DATA_MAY16(yr);GFEDCannual025x025(:,:,yr-2000)=mean(GF4.ES(:,:,:,4),3);end
%save DUMPFILES/GFED_2001_2015_4x5_NOCODE_v2.mat GFEDCannual025x025
%load DUMPFILES/GFED_2001_2015_4x5_NOCODE_v2.mat GFEDCannual025x025
%NaNs for sea values
%Therefore no need to normalize by land-sea mask
% M3D=repmat(lsfrac025>0.5,[1,1,size(GFEDCannual025x025,3)]);
% GFEDCannual025x025(M3D==0)=NaN;
% GFEDCannual4x5=GEOSChem_regular_grid_to_GC(GFEDCannual025x025);


% GFEDCannual1x1=mapsfun_coarsen_resolution(GFEDCannual025x025,A025,4);

for yr=2001:2015
 BBCO(:,:,yr-2000)=mean(nan2zero(DATA.COsource(:,:,DATA.year==yr,2).*(CO2CO(:,:,DATA.year==yr)+1))*12/44*12/365.25,3);
end

   GCDRIOBS.BBCO=BBCO;
   
GCDRIOBS.BBCO=GCDRIOBS.BBCO./repmat(LSFRAC,[1,1,size(GCDRIOBS.BBCO,3)]);


end
%Convert to gC/m2/d
% GCDRIOBS.BBGFED=GFEDCannual4x5*12/365.25;
if matfilecontains(GCDRIOBS,'SIF')==0 || matfilecontains(GCDRIOBS,'SIFgome2')==0 || matfilecontains(GCDRIOBS,'SIFfluxsat')==0 || repeat==1

%Load GOSAT and GOME2 SIF datasets
GOSAT=quickload('DATASCRIPT_READ_GOSAT_FLUORESCENCE(''4x5'')');
GOME2=quickload('DATASCRIPT_READ_GOME_2_FLUORESCENCE_NOV18');
%For completeness, expanding and storing missing data as  NaN
SIFgosat(1:46,1:72,[1:metobsidx(end)])=NaN;
SIFgosat(1:46,1:72,109:180)=GOSAT.SIF(1:46,1:72,GOSAT.year<=2015 & GOSAT.year>=2010);
SIFgome2(1:46,1:72,1:metobsidx(end))=NaN;
SIFgome2(1:46,1:72,73:204)=GEOSChem_regular_grid_to_GC(GOME2.data05x05(:,:,GOME2.year>=2007 & GOME2.year<=2017));


FLUXSAT=quickload('DATASCRIPT_READ_FLUXSAT_SIF_GPP');

%2001-2018
%SIFfluxsat already normalized by unit land
%GCDRIOBS.SIFfluxsat(1:46,1:72,1:216)=FLUXSAT.data./repmat(LSFRAC,[1,1,size(FLUXSAT,3)]);

GCDRIOBS.SIFfluxsat(1:46,1:72,1:216)=FLUXSAT.data;

GCDRIOBS.SIF=SIFgosat;
GCDRIOBS.SIFgome2=SIFgome2;

end




%NBE normalization OK, but ensure to limit analysis to >=10% land cover
%Rationale = oceans disproportionally contribute to gridcell flux
%otherwise.


repeatallnbe=0;



if matfilecontains(GCDRIOBS,'NBE')==0 || matfilecontains(GCDRIOBS,'NBE_CMS_JAN20')==0  || repeat==1 || repeatallnbe==1;
    
    

CMS=DATASCRIPT_READ_JUNJIE_CMS_OUTPUT_JUL18;
CMS2=DATASCRIPT_READ_JUNJIE_CMS_OUTPUT_SEP19('CMS_2015_2018_SEP19');
CMS3=DATASCRIPT_READ_JUNJIE_CMS_OUTPUT_SEP19('CMS_2010_2018_JAN20');

%Only providing b35 results here 
NBE(:,:,109:144)=CMS.post;
NBE(:,:,[1:108,145:180])=NaN;
NBE73(:,:,109:180)=CMS.post_b73;NBE73(:,:,1:108)=NaN;
NBEoco2=CMS.post_oco2;
NBE_OCO2_SEP19(:,:,169:216)=CMS2.post;NBE_OCO2_SEP19(:,:,1:168)=NaN;
NBE73_s3x3(:,:,109:180)=CMS.post_b73_s3x3;NBE73_s3x3(:,:,1:108)=NaN;
NBE_CMS_JAN20(:,:,109:216)=CMS3.post;NBE_CMS_JAN20(:,:,1:108)=NaN;
NBE_CMS_JAN20_UNC(:,:,109:216)=CMS3.post;NBE_CMS_JAN20_UNC(:,:,1:108)=NaN;

%Translate to C equivalent
%See Bowman et al., 2017 outputs

GCDRIOBS.NBE=NBE;
GCDRIOBS.NBE73=NBE73;
GCDRIOBS.NBEoco2=NBEoco2;
GCDRIOBS.NBE73_s3x3=NBE73_s3x3;
GCDRIOBS.NBE_OCO2_SEP19=NBE_OCO2_SEP19;
GCDRIOBS.NBE_CMS_JAN20=NBE_CMS_JAN20;
GCDRIOBS.NBE_CMS_JAN20_UNC=NBE_CMS_JAN20_UNC;

GCDRIOBS.NBE=GCDRIOBS.NBE./repmat(LSFRAC,[1,1,size(GCDRIOBS.NBE,3)]);
GCDRIOBS.NBE73=GCDRIOBS.NBE73./repmat(LSFRAC,[1,1,size(GCDRIOBS.NBE73,3)]);
GCDRIOBS.NBEoco2=GCDRIOBS.NBEoco2./repmat(LSFRAC,[1,1,size(GCDRIOBS.NBEoco2,3)]);
GCDRIOBS.NBE73_s3x3=GCDRIOBS.NBE73_s3x3./repmat(LSFRAC,[1,1,size(GCDRIOBS.NBE73_s3x3,3)]);
GCDRIOBS.NBE_OCO2_SEP19=GCDRIOBS.NBE_OCO2_SEP19./repmat(LSFRAC,[1,1,size(GCDRIOBS.NBE_OCO2_SEP19,3)]);
GCDRIOBS.NBE_CMS_JAN20=GCDRIOBS.NBE_CMS_JAN20./repmat(LSFRAC,[1,1,size(GCDRIOBS.NBE_CMS_JAN20,3)]);

GCDRIOBS.LSFRAC=LSFRAC;
%GCDRIOBS.BBGFED=GCDRIOBS.BBGFED./repmat(LSFRAC,[1,1,size(GCDRIOBS.BBGFED,3)]);

%Deriving NBE uncertainty stats based on Liu et al. 2010
%These can then be prescribed as annual and seasonal uncertainty values for
%each gridcell
%Step 1. Derive mean uncertainty for each gridcell
GCDRIOBS.NBEunc=mean(std(CMS.unc,[],4),3);

%Step 2. for each year, remove mean of each MCMC layer
for yr=2010:2012;
    midx=[1:12]+12*(yr-2010);
    NBEannual_mc(:,:,yr-2009,:)=mean(CMS.unc(:,:,midx,:),3);
    NBEseasonal_mc(:,:,midx,:)=CMS.unc(:,:,midx,:)-repmat(NBEannual_mc(:,:,yr-2009,:),[1,1,12,1]);
end
GCDRIOBS.NBEunc_annual=mean(std(NBEannual_mc,[],4),3);
GCDRIOBS.NBEunc_seasonal=mean(std(NBEseasonal_mc,[],4),3);
%CO2 concentrations global
%MLCO2=DATASCRIPT_READ_NOAA_MAUNA_LOA_CO2_SEP17;


end


repeatmlco2=1;
if matfilecontains(GCDRIOBS,'NBE')==0 || repeat==1 || repeatmlco2==1;
MLCO2=DATASCRIPT_READ_NOAA_MAUNA_LOA_CO2_OCT19;
GCDRIOBS.CO2=MLCO2.data(MLCO2.year>=YEARS(1) & MLCO2.year<=YEARS(end));
end

    GCDRIOBS=matfile(dumpfilename,'Writable',false);




end


function GCDRIOBS=READ_GRID_GC_4x5_DRIVERS_OBS_ERA5(repeat)


%This function is a universal gridding of all/any datasets at 4x5  
YEARS=2001:2019;


defval('repeat',0);
%Dumpfile and stuff here
%GCDRIOBS 
%dumpfilename='DUMPFILES/PROJSCRIPT_CARDAMOM_CMS_GC4x5_DRIOBS_NOV18.mat';
dumpfilename='DUMPFILES/PROJSCRIPT_CARDAMOM_CMS_GC4x5_DRIOBS_ERA5_AUG20.mat';

if isempty(dir(dumpfilename))==0 && repeat==1;delete(dumpfilename);end

%Create/open file
GCDRIOBS=matfile(dumpfilename,'Writable',true);
GCDRIOBS.dates=datenum(sprintf('01/01/%4i',YEARS(1)))+365.25/24:365.25/12:datenum(sprintf('01/01/%4i',YEARS(end)+1));


    %FOr any additional datasets, update & import through function below


[~,~,A05]=loadworldmesh(0.5);
[~,LSFRAC]=loadlandseamask('4x5');
[~,lsfrac05]=loadlandseamask(0.5);
[~,lsfrac025]=loadlandseamask(0.25);
[~,lsfrac1]=loadlandseamask(1);

GC=GEOSChem_xygrids;

%Read ERA-interim (new dataset function);
%2001-2017
metobsidx=1:numel(YEARS)*12;
%metobsidx_erai=1:numel(2001:2018)*12;
metobsidx_str=sprintf('%i:%i',metobsidx(1),metobsidx(end));
%metobsidx_str_erai=sprintf('%i:%i',metobsidx(1),metobsidx_erai(end));

repeatbiomass=0;
if matfilecontains(GCDRIOBS,'annualbiomass')==0 || repeat==1 || repeatbiomass==1;
    BTD=quickload('DATASCRIPT_READ_YY_BIOMASS_MAPS_APR19');
    %Only 2001-2017
    yrs=BTD.year<2018 & BTD.year>2000;
    BTD.biomass=BTD.biomass(:,:,yrs);
    M3D=repmat(lsfrac05>0.5,[1,1,size(BTD.biomass,3)]);
    %Remove sea
BTD.biomass(M3D==0)=NaN;
%Set land values to zero;
BTD.biomass(isnan(BTD.biomass) & M3D)=0;
GCDRIOBS.annualbiomass=GEOSChem_regular_grid_to_GC(BTD.biomass);% Units = gC/m2
end


if matfilecontains(GCDRIOBS,'prec')==0 || repeat==1;
% EPREC=quickload('DATASCRIPT_READ_ERAI_MONTHLY_JUL18(''prec'',0.5)');EPREC.data(EPREC.data<0)=0;
% GCDRIOBS.prec=indeks(GEOSChem_regular_grid_to_GC(EPREC.data),[':,:,',metobsidx_str_erai]);
% clear EPREC;
end

if matfilecontains(GCDRIOBS,'prec_e5')==0 || repeat==1;
EPREC=quickload('DATASCRIPT_READ_ERA5_MONTHLY_AUG20(''tp'',0.5,2001:2019)');
EPREC.data(EPREC.data<0)=0;
GCDRIOBS.prec=indeks(GEOSChem_regular_grid_to_GC(EPREC.data),[':,:,',metobsidx_str]);
clear EPREC;
end


if matfilecontains(GCDRIOBS,'vpd')==0 || repeat==1;
ET2M=quickload('DATASCRIPT_READ_ERA5_MONTHLY_AUG20(''t2m'',0.5,2001:2019)');
GCDRIOBS.mintemp=indeks(GEOSChem_regular_grid_to_GC(ET2M.datamin),[':,:,',metobsidx_str]);
GCDRIOBS.maxtemp=indeks(GEOSChem_regular_grid_to_GC(ET2M.datamax),[':,:,',metobsidx_str]);

ED2M=quickload('DATASCRIPT_READ_ERA5_MONTHLY_AUG20(''d2m'',0.5,2001:2019)');
GCDRIOBS.vpd=GEOSChem_regular_grid_to_GC(indeks(SCIFUN_H2O_SATURATION_PRESSURE(ET2M.datamax),[':,:,',metobsidx_str]) - indeks(SCIFUN_H2O_SATURATION_PRESSURE(ED2M.datamax),[':,:,',metobsidx_str]))*10;

clear E*2M ;
end


if matfilecontains(GCDRIOBS,'srad')==0 || repeat==1;
ESRAD=quickload('DATASCRIPT_READ_ERA5_MONTHLY_AUG20(''ssrd'',0.5,2001:2019)');
GCDRIOBS.srad=indeks(GEOSChem_regular_grid_to_GC(ESRAD.data),[':,:,',metobsidx_str]);
clear ESRAD;
end

%MPI GPP
%replace "Always 0" values with NaNs
%Use lsmask05 = 0.5 as a cutoff;
%assume (a) values outside are caterorized as ls=0, and (b) values inside
%are zeros (not empty). Apply this approach to standardize across all
%datasets. prescribe all values outside 0.5 as NaN
%First step.
%mask=mean(mean(GPP.data,3),4)==0 & loadlandseamask(0.5)<1
if matfilecontains(GCDRIOBS,'MPIGPPmean')==0 || repeat==1;
GPP=quickload('DATASCRIPT_READ_FLUXCOM_DATASETS_OCT17');
M4D=repmat(lsfrac05>0.5,[1,1,size(GPP.data,3),size(GPP.data,4)]);
GPP.data(M4D==0)=NaN;GPP.data(isnan(GPP.data) & M4D)=0;
GCDRIOBS.MPIGPPmean=GEOSChem_regular_grid_to_GC(GPP.data);% Units = gC/m2/day
end

%Burned area
%NRW CODE:
repeatba=0;
if matfilecontains(GCDRIOBS,'BA')==0 || repeat==1 || repeatba==1;

GBA=[];
GCE=[];
for yr=YEARS
    GF4=DATASCRIPT_READ_GFED4_DATA_MAY16(yr);
    GBA(:,:,(yr-2001)*12+[1:12])=GF4.BA;
    GCE(:,:,(yr-2001)*12+[1:12])=GF4.ES(:,:,:,4);
end

 M4D=repmat(lsfrac025>0.5,[1,1,size(GBA,3),size(GBA,4)]);
 GBA(M4D==0)=NaN;GBA(isnan(GBA) & M4D)=0;
 GCE(M4D==0)=NaN;GCE(isnan(GCE) & M4D)=0;
 %Ensures land fire-free regions = 0 but sea regions are = NaN
 GCDRIOBS.BA=indeks(GEOSChem_regular_grid_to_GC(GBA),[':,:,',metobsidx_str]);
 GCDRIOBS.FireC=indeks(GEOSChem_regular_grid_to_GC(GCE),[':,:,',metobsidx_str])*12/365.25;%g species/m2/month
 %nantotal(nanmean(GCDRIOBS.FireC,3).*GC.area)*365.25/1e15

 disp('GFEDv4s beta does not provide 2017-19 burned area');
 disp('Using GFEDv4s 2001-2016 BA/Emissions ratio')
 idx16=16*12;
 
 for m=1:36
     BAextra(:,:,m)=sum(GCDRIOBS.BA(:,:,m:12:idx16),3)./sum(GCDRIOBS.FireC(:,:,m:12:idx16),3).*GCDRIOBS.FireC(:,:,idx16+m);
 end
 %storing and replacing NaNs with zeros (Nans occur when 2010-2016 fires
 %are zero)
GCDRIOBS.BA(:,:,idx16+1:end)= BAextra;
GCDRIOBS.BA=nan2zero(GCDRIOBS.BA);
%OLD CODE: used GFEDv4 BA data
% GBA0=quickload('DATASCRIPT_READ_GFED4_BA_DATA_OCT15(1)');
 %M4D=repmat(lsfrac025>0.5,[1,1,size(GBA.ba,3),size(GBA.ba,4)]);
% GBA.ba(M4D==0)=NaN;GBA.ba(isnan(GBA.ba) & M4D)=0;%Ensures land fire-free regions = 0 but sea regions are = NaN
% GCDRIOBS.BA=indeks(GEOSChem_regular_grid_to_GC(GBA.ba),[':,:,',eraidx]);
end

if matfilecontains(GCDRIOBS,'SOM')==0 || repeat==1
%Soil C
HWSD=quickload('DATASCRIPT_READ_HWSD_MAP');
GCDRIOBS.SOM=GEOSChem_regular_grid_to_GC(HWSD.data);

end
%LAI
% LAI=quickload('DATASCRIPT_READ_MODIS_MONTHLY_LAI');LAI.data(isfinite(LAI.data)==0)=0;
% LAI4x50=GEOSChem_grid_1x1_to_GC(LAI.data);





repeatlai=0;
if matfilecontains(GCDRIOBS,'LAI')==0 || repeat==1 || repeatlai==1
LAI=DATASCRIPT_READ_MODIS_MONTHLY_LAI_MAY17('4x5');
GCDRIOBS.LAI=LAI.data(:,:,metobsidx(1:192));
GCDRIOBS.LAI(:,:,193:metobsidx(end))=NaN;
GCDRIOBS.PEAKLAIDAY=projscript_approximate_peak_lai(GCDRIOBS.LAI(:,:,1:192));
end



%Next: ABGB
if matfilecontains(GCDRIOBS,'TABGB')==0 || repeat==1
TABGB=quickload('DATASCRIPT_READ_SAATCHI_BIOMASS_MAP(0.25)');
TABGB.data(TABGB.data==0 | lsfrac025<0.5)=NaN;
GCDRIOBS.TABGB=GEOSChem_regular_grid_to_GC(TABGB.data);
end


%Normalizing NBE and BB fluxes by land surface area
[~,LSFRAC]=loadlandseamask('4x5');


if matfilecontains(GCDRIOBS,'BBCO')==0 || repeat==1
%Load CO emissions, and store mean fire C loss (total)
DATA=quickload('PROJSCRIPT_CO_BAYESIAN_PARTITIONING_MAR17');
CO2CO=permute(ncread('OUTPUTS/CMS_FIRES_CO2CO/CMS_FIRES_CO2CO_NOV16.nc','CO2:CO'),[2,1,3]);
%mean CO emissions
%Convert to total C emissions
%CO + CO*CO2CO
%Convert to daily flux
%For comparison: 
%for yr=2001:2015; GF4=DATASCRIPT_READ_GFED4_DATA_MAY16(yr);GFEDCannual025x025(:,:,yr-2000)=mean(GF4.ES(:,:,:,4),3);end
%save DUMPFILES/GFED_2001_2015_4x5_NOCODE_v2.mat GFEDCannual025x025
%load DUMPFILES/GFED_2001_2015_4x5_NOCODE_v2.mat GFEDCannual025x025
%NaNs for sea values
%Therefore no need to normalize by land-sea mask
% M3D=repmat(lsfrac025>0.5,[1,1,size(GFEDCannual025x025,3)]);
% GFEDCannual025x025(M3D==0)=NaN;
% GFEDCannual4x5=GEOSChem_regular_grid_to_GC(GFEDCannual025x025);


% GFEDCannual1x1=mapsfun_coarsen_resolution(GFEDCannual025x025,A025,4);

for yr=2001:2015
 BBCO(:,:,yr-2000)=mean(nan2zero(DATA.COsource(:,:,DATA.year==yr,2).*(CO2CO(:,:,DATA.year==yr)+1))*12/44*12/365.25,3);
end

   GCDRIOBS.BBCO=BBCO;
   
GCDRIOBS.BBCO=GCDRIOBS.BBCO./repmat(LSFRAC,[1,1,size(GCDRIOBS.BBCO,3)]);


end
%Convert to gC/m2/d
% GCDRIOBS.BBGFED=GFEDCannual4x5*12/365.25;
repeatallsif=0;
if matfilecontains(GCDRIOBS,'SIF')==0 || matfilecontains(GCDRIOBS,'SIFgome2')==0 || matfilecontains(GCDRIOBS,'SIFfluxsat')==0 || repeat==1 || repeatallsif==1


%Load GOSAT and GOME2 SIF datasets
GOSAT=quickload('DATASCRIPT_READ_GOSAT_FLUORESCENCE(''4x5'')');
GOME2=quickload('DATASCRIPT_READ_GOME_2_FLUORESCENCE_NOV18');
%For completeness, expanding and storing missing data as  NaN
SIFgosat(1:46,1:72,[1:metobsidx(end)])=NaN;
SIFgosat(1:46,1:72,109:180)=GOSAT.SIF(1:46,1:72,GOSAT.year<=2015 & GOSAT.year>=2010);
SIFgome2(1:46,1:72,1:metobsidx(end))=NaN;
SIFgome2(1:46,1:72,73:204)=GEOSChem_regular_grid_to_GC(GOME2.data05x05(:,:,GOME2.year>=2007 & GOME2.year<=2017));


FLUXSAT=quickload('DATASCRIPT_READ_FLUXSAT_SIF_GPP');

%2001-2018
%SIFfluxsat already normalized by unit land
%GCDRIOBS.SIFfluxsat(1:46,1:72,1:216)=FLUXSAT.data./repmat(LSFRAC,[1,1,size(FLUXSAT,3)]);
SIFfluxsat(1:46,1:72,1:metobsidx(end))=NaN;
%Only including 2007-2018
SIFfluxsat(1:46,1:72,73:216)=FLUXSAT.data(:,:,73:216);

GCDRIOBS.SIFfluxsat=SIFfluxsat;

GCDRIOBS.SIF=SIFgosat;
GCDRIOBS.SIFgome2=SIFgome2;

end




%NBE normalization OK, but ensure to limit analysis to >=10% land cover
%Rationale = oceans disproportionally contribute to gridcell flux
%otherwise.


repeatallnbe=0;



if matfilecontains(GCDRIOBS,'NBE')==0 || matfilecontains(GCDRIOBS,'NBE_CMS_JAN20')==0  || repeat==1 || repeatallnbe==1;
    
    

CMS=DATASCRIPT_READ_JUNJIE_CMS_OUTPUT_JUL18;
CMS2=DATASCRIPT_READ_JUNJIE_CMS_OUTPUT_SEP19('CMS_2015_2018_SEP19');
CMS3=DATASCRIPT_READ_JUNJIE_CMS_OUTPUT_SEP19('CMS_2010_2018_JAN20');

%Only providing b35 results here 
NBE(:,:,109:144)=CMS.post;
NBE(:,:,[1:108,145:180])=NaN;
NBE73(:,:,109:180)=CMS.post_b73;NBE73(:,:,1:108)=NaN;
NBEoco2=CMS.post_oco2;
NBE_OCO2_SEP19(:,:,169:216)=CMS2.post;NBE_OCO2_SEP19(:,:,1:168)=NaN;
NBE73_s3x3(:,:,109:180)=CMS.post_b73_s3x3;NBE73_s3x3(:,:,1:108)=NaN;
NBE_CMS_JAN20(:,:,109:216)=CMS3.post;NBE_CMS_JAN20(:,:,1:108)=NaN;
NBE_CMS_JAN20_UNC(:,:,109:216)=CMS3.post;NBE_CMS_JAN20_UNC(:,:,1:108)=NaN;

%Translate to C equivalent
%See Bowman et al., 2017 outputs

GCDRIOBS.NBE=NBE;
GCDRIOBS.NBE73=NBE73;
GCDRIOBS.NBEoco2=NBEoco2;
GCDRIOBS.NBE73_s3x3=NBE73_s3x3;
GCDRIOBS.NBE_OCO2_SEP19=NBE_OCO2_SEP19;
GCDRIOBS.NBE_CMS_JAN20=NBE_CMS_JAN20;
GCDRIOBS.NBE_CMS_JAN20_UNC=NBE_CMS_JAN20_UNC;

GCDRIOBS.NBE=GCDRIOBS.NBE./repmat(LSFRAC,[1,1,size(GCDRIOBS.NBE,3)]);
GCDRIOBS.NBE73=GCDRIOBS.NBE73./repmat(LSFRAC,[1,1,size(GCDRIOBS.NBE73,3)]);
GCDRIOBS.NBEoco2=GCDRIOBS.NBEoco2./repmat(LSFRAC,[1,1,size(GCDRIOBS.NBEoco2,3)]);
GCDRIOBS.NBE73_s3x3=GCDRIOBS.NBE73_s3x3./repmat(LSFRAC,[1,1,size(GCDRIOBS.NBE73_s3x3,3)]);
GCDRIOBS.NBE_OCO2_SEP19=GCDRIOBS.NBE_OCO2_SEP19./repmat(LSFRAC,[1,1,size(GCDRIOBS.NBE_OCO2_SEP19,3)]);
GCDRIOBS.NBE_CMS_JAN20=GCDRIOBS.NBE_CMS_JAN20./repmat(LSFRAC,[1,1,size(GCDRIOBS.NBE_CMS_JAN20,3)]);

GCDRIOBS.LSFRAC=LSFRAC;
%GCDRIOBS.BBGFED=GCDRIOBS.BBGFED./repmat(LSFRAC,[1,1,size(GCDRIOBS.BBGFED,3)]);

%Deriving NBE uncertainty stats based on Liu et al. 2010
%These can then be prescribed as annual and seasonal uncertainty values for
%each gridcell
%Step 1. Derive mean uncertainty for each gridcell
GCDRIOBS.NBEunc=mean(std(CMS.unc,[],4),3);

%Step 2. for each year, remove mean of each MCMC layer
for yr=2010:2012;
    midx=[1:12]+12*(yr-2010);
    NBEannual_mc(:,:,yr-2009,:)=mean(CMS.unc(:,:,midx,:),3);
    NBEseasonal_mc(:,:,midx,:)=CMS.unc(:,:,midx,:)-repmat(NBEannual_mc(:,:,yr-2009,:),[1,1,12,1]);
end
GCDRIOBS.NBEunc_annual=mean(std(NBEannual_mc,[],4),3);
GCDRIOBS.NBEunc_seasonal=mean(std(NBEseasonal_mc,[],4),3);
%CO2 concentrations global
%MLCO2=DATASCRIPT_READ_NOAA_MAUNA_LOA_CO2_SEP17;


end


repeatmlco2=1;
if matfilecontains(GCDRIOBS,'NBE')==0 || repeat==1 || repeatmlco2==1;
%MLCO2=DATASCRIPT_READ_NOAA_MAUNA_LOA_CO2_OCT19;
NOAACO2=DATASCRIPT_READ_NOAA_GLOBAL_CO2_AUG20;
GCDRIOBS.CO2=NOAACO2.data(NOAACO2.year>=YEARS(1) & NOAACO2.year<=YEARS(end));
end

    GCDRIOBS=matfile(dumpfilename,'Writable',false);




end


function B=nanzerofill(A);
%fills nans and <=0 as -9999
B=A;B(isnan(A) | A<=0)=-9999;
end


%Auxilliary 
function PEAKLAIDAY=projscript_approximate_peak_lai(LAIDATA)

YEAR=2000+ceil([1:120]/12);
MONTH=mod(1:120,12);MONTH(MONTH==0)=12;

PEAKLAIMONTHS=LAIDATA(:,:,1:12)*0;
COUNT=LAIDATA(:,:,1:12)*0;
PEAKLAIMONTH=LAIDATA(:,:,1)*0;
PEAKLAIMONTHframe=LAIDATA(:,:,1)*0;
for y=2001:2010
    %month of
    for m=1:12
    PEAKLAIMONTHframe(LAIDATA(:,:,YEAR==2010 & MONTH==m)==max(LAIDATA(:,:,YEAR==y),[],3))=m;
    end
    PEAKLAIMONTHS(:,:,y-2000)=PEAKLAIMONTHframe;
end

for m=1:12; COUNT(:,:,m)=sum(PEAKLAIMONTHS==m,3);end
    for m=1:12;PEAKLAIMONTH(COUNT(:,:,m)==max(COUNT,[],3))=m;end
PEAKLAIDAY=PEAKLAIMONTH*365.25/12-365.25/24;
PEAKLAIDAY(max(COUNT,[],3)==0)=NaN;
    
    


end

function yesno=matfilecontains(MAT,varname)

w=whos(MAT);
wcell=struct2cell(w);
vars=wcell(1,:);
yesno=total(strcmp(vars,varname))==1;

end
