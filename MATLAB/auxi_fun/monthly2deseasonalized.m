function TSA=monthly2deseasonalized(TS,nm,meanmedian)
%Should be rows for 2D mat
%If 3D mat, assumes 3rd dim is "seasonalized"
defval('nm',1) %nm = nanmean
defval('meanmedian',1);
TSA=monthly2seasonal(TS,nm,meanmedian);

TSA=TS-repmat(TSA,[1,size(TS,2)/12]);




end