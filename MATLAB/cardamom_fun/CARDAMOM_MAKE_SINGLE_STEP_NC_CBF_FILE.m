function [CBFout]=CARDAMOM_MAKE_SINGLE_STEP_NC_CBF_FILE(CBF,idx,cbrfile_in,cbffile_out)

if nargin==1;idx=1;end


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

if nargin==3 & isempty(cbrfile_in)==0;
    %Step 1. Re-run model
    MD=CARDAMOM_MODEL_LIBRARY(1100,[],1);
    CBR=CARDAMOM_RUN_MODEL(CBF, cbrfile_in);
%Step 2. extract PARS

PARS=CBR.PARS;
%step 3. Replace initial conditions with pools at that timepoint
%Do this manually until further notice
keyboard

PARS(:,MD.PARAMETER_IDs.i_labile)=CBR.POOLS(:,idx,MD.POOL_IDs.C_lab);









end

if nargin==4; CARDAMOM_WRITE_NC_CBF_FILE(CBFout,cbffile);end
    
    
    
    




end

