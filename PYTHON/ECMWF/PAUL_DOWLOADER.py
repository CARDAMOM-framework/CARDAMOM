import DOWNLOAD_ECMWF_0.5deg_MONTHLY_DRIVERS_FOR_CARDAMOM

#Probably nothing like this but general idea is here lol

years=[2001,2002];
months=[1,2,3];

for m in months:
  for yr in years:
    DOWNLOAD_ECMWF_MONTHLY_DRIVERS_FOR_CARDAMOM(m, yr);
