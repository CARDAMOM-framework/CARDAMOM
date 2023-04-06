function [CBFout,PARS]=CARDAMOM_MAKE_SINGLE_STEP_NC_CBF_FILE(CBF,idx,CBR,cbffile_out)

if nargin==1;idx=1;end
if nargout==2;PARS=[];end


if ischar(CBF);CBF=CARDAMOM_READ_NC_CBF_FILE(CBF);end

N=numel(CBF.time.values);

CBFout=CBF;
fnames=fields(CBF);
for n=1:numel(fnames);
    if isfield(CBF.(fnames{n}),'values') & numel((CBF.(fnames{n}).values))==N
        CBFout.(fnames{n}).values=CBF.(fnames{n}).values(idx);
    end
end

%Leaving two first timesteps for deltat calculation
CBFout.time.values=CBF.time.values(1:2);

if nargin>=3 & isempty(CBR)==0;
    %Step 1. Re-run model
    MD=CARDAMOM_MODEL_LIBRARY(1100,[],1);
    if isstring(CBR)
        CBR=CARDAMOM_RUN_MODEL(CBF, CBR);
    end
%Step 2. extract PARS

PARS=CBR.PARS;
%step 3. Replace initial conditions with pools at that timepoint
%Do this manually until further notice

PARS(:,MD.PARAMETER_IDs.i_labile)=CBR.POOLS(:,idx,MD.POOL_IDs.C_lab);
PARS(:,MD.PARAMETER_IDs.i_foliar)=CBR.POOLS(:,idx,MD.POOL_IDs.C_fol);
PARS(:,MD.PARAMETER_IDs.i_root)=CBR.POOLS(:,idx,MD.POOL_IDs.C_roo);
PARS(:,MD.PARAMETER_IDs.i_wood)=CBR.POOLS(:,idx,MD.POOL_IDs.C_woo);
PARS(:,MD.PARAMETER_IDs.i_lit)=CBR.POOLS(:,idx,MD.POOL_IDs.C_lit);
PARS(:,MD.PARAMETER_IDs.i_cwd)=CBR.POOLS(:,idx,MD.POOL_IDs.C_cwd);
PARS(:,MD.PARAMETER_IDs.i_som)=CBR.POOLS(:,idx,MD.POOL_IDs.C_som);

%Soil moisture
PARS(:,MD.PARAMETER_IDs.i_LY1_SM)=CBR.POOLS(:,idx,MD.POOL_IDs.H2O_LY1)./CBR.PARS(:,MD.PARAMETER_IDs.LY1_por)./CBR.PARS(:,MD.PARAMETER_IDs.LY1_z)/1000;
PARS(:,MD.PARAMETER_IDs.i_LY2_SM)=CBR.POOLS(:,idx,MD.POOL_IDs.H2O_LY2)./CBR.PARS(:,MD.PARAMETER_IDs.LY2_por)./CBR.PARS(:,MD.PARAMETER_IDs.LY2_z)/1000;
PARS(:,MD.PARAMETER_IDs.i_LY3_SM)=CBR.POOLS(:,idx,MD.POOL_IDs.H2O_LY3)./CBR.PARS(:,MD.PARAMETER_IDs.LY3_por)./CBR.PARS(:,MD.PARAMETER_IDs.LY3_z)/1000;

%snow
PARS(:,MD.PARAMETER_IDs.i_SWE)=CBR.POOLS(:,idx,MD.POOL_IDs.H2O_SWE);

%Energy (J per mm H2O)
%Remove heat storage and divide by H2O mm
PARS(:,MD.PARAMETER_IDs.i_LY1_E)=(CBR.POOLS(:,idx,MD.POOL_IDs.E_LY1) - CBR.PARS(:,MD.PARAMETER_IDs.LY1_vhc).*CBR.POOLS(:,idx,MD.POOL_IDs.D_TEMP_LY1).*CBR.PARS(:,MD.PARAMETER_IDs.LY1_z))./CBR.POOLS(:,idx,MD.POOL_IDs.H2O_LY1);
PARS(:,MD.PARAMETER_IDs.i_LY2_E)=(CBR.POOLS(:,idx,MD.POOL_IDs.E_LY2) - CBR.PARS(:,MD.PARAMETER_IDs.LY2_vhc).*CBR.POOLS(:,idx,MD.POOL_IDs.D_TEMP_LY2).*CBR.PARS(:,MD.PARAMETER_IDs.LY2_z))./CBR.POOLS(:,idx,MD.POOL_IDs.H2O_LY2);
PARS(:,MD.PARAMETER_IDs.i_LY3_E)=(CBR.POOLS(:,idx,MD.POOL_IDs.E_LY3) - CBR.PARS(:,MD.PARAMETER_IDs.LY3_vhc).*CBR.POOLS(:,idx,MD.POOL_IDs.D_TEMP_LY3).*CBR.PARS(:,MD.PARAMETER_IDs.LY3_z))./CBR.POOLS(:,idx,MD.POOL_IDs.H2O_LY3);









end

if nargin==4; CARDAMOM_WRITE_NC_CBF_FILE(CBFout,cbffile_out);end
    
    
    
    




end

