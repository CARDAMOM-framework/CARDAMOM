function GRUN=DATASCRIPT_READ_GRUN_OCT25

fname='../DATA/GRUN/GRUN_v1_GSWP3_WGS84_05_1902_2014.nc';

GRUN.data=permute(ncread(fname,'Runoff'),[2,1,3]);
GRUN.units='mm/day';
GRUN.year = ceil([1:size(GRUN.data,3)]/12)+1901;
GRUN.month = mod([1:size(GRUN.data,3)]-1,12)+1;
