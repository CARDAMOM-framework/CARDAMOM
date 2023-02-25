function TSA=monthly2cumulativeannual(TS,dim)
%Should be rows for 2D mat

%default = 2 dims
defval('dim',ndims(TS))

TSA=0;

if dim==2;
TS=squeeze(TS);
if total(size(TS))>numel(TS) & size(TS,1)>size(TS,2);TS=TS';end



N=size(TS,2)/12;
for yr=1:N;idx=[1:12]+(yr-1)*12;
TSA(idx)=cumsum(TS(:,idx));end

elseif dim==3
    
    
N=size(TS,3)/12;

for yr=1:N;
    idx=[1:12]+(yr-1)*12;
    TSA(idx)=cumsum(TS(:,:,idx,:,:));end
    

else 
    error('number of dims not supported, edit code accordingly!')
end
    
    
end