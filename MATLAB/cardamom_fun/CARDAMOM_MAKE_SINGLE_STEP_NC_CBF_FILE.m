function [CBFout]=CARDAMOM_MAKE_SINGLE_STEP_NC_CBF_FILE(CBF,idx,cbffile)

if nargin==1;idx=1;


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

if nargin==3;CARDAMOM_WRITE_NC_CBF_FILE(CBFout,cbffile);end
    
    
    
    




end

