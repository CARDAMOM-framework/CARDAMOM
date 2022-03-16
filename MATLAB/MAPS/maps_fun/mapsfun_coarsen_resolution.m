function FC=mapsfun_coarsen_resolution(F,A,fac)
%Applicable for 2d and 3d matrices
%Inputs
%F = field at finer grid
%A = area (or area weight) for each grid
%Resolution coarsness factor (fac = 1; no change; fac = 2; resolution is
%two times coarser, etc.)
%
%Note on NaN values: treats NaN values as missing values; FC is in units
%per unit area with finite values.

FC=repmat(zeros(size(F(:,:,1))./[fac,fac]),[1,1,size(F,3)]);
        for l=1:size(F,3);
            W=zeros(size(A)./[fac,fac]);

for r=1:fac
    for c=1:fac
 FC(:,:,l)=FC(:,:,l)+nan2zero(F(r:fac:end,c:fac:end,l).*A(r:fac:end,c:fac:end));
                W=W+A(r:fac:end,c:fac:end,:).*isfinite(F(r:fac:end,c:fac:end,l));

        end
end
    FC(:,:,l)=FC(:,:,l)./W;
end



end