function GRUN=DATASCRIPT_READ_GRUN_OCT25

fname='../DATA/GRUN/GRUN_v1_GSWP3_WGS84_05_1902_2014.nc';

GRUN.data=permute(ncread(fname,'Runoff'),[2,1,3]);
GRUN.units='mm/day';
GRUN.time=round(ncread(fname,'time')*12/365.25)+1;
GRUN.month=mod(GRUN.time-1,12)+1;
GRUN.year=floor((GRUN.time-1)/12)+1902;

