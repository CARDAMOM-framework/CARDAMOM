function cardamom_vvuq_nccbf_summary(CBF,f)
%function cardamom_vvuq_cbf_summary(CBF)
%Summarizes contents of CBF file
%
%Written by A Bloom, Thu Apr 11, 2019

%Switch is in place in case later version met fields are not compatible!
if nargin<2;f=1;end
figure(f);clf


f=fields(CBF);
Ns=ceil(sqrt(numel(f)));

for n=1:numel(f)
    if sum(isfinite(CBF.(f{n}).values))>0
    subplot(Ns,Ns,n);
    plot(CBF.(f{n}).values,'o-');title(f{n});
    end

end
end


function [v,pts]=fill2nan(v)

pts=find(v~=-9999);
v(v==-9999)=NaN;

end

