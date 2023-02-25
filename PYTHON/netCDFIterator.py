import logging
import os
from netCDF4 import Dataset
import glob

#This is a helper iterator class that iterates over the data in a netcdf file!
def NetCDFItr:

    def __init__(self, dataDir, startMonth, startYear, endMonth, endYear ):
        self.dataDir = dataDir
        self.startMonth = startMonth
        self.startYear =startYear
        self.endMonth =endMonth
        self.endYear = endYear
        #These are the iterator variables, with the year switching
        #when we need to open a new file
        self.curMonth = startMonth
        self.curYear=startYear
        self.curInputFile=None


    def __next__(self):
        if (self.curYear = self.endYear and self.curMonth > self.endMonth) or self.curYear > self.endYear:
            #this datapoint would be outside our set, stop iterating
            raise StopIteration
        if self.curMonth == 12:
            #Move to new year, open new netcdf file
            self.curMonth=0
            self.curYear++
            #A curInputFile of None indecates we need to find a new input
            self.curInputFile=None
        if self.curInputFile == None:
            fileList=glob.glob(f"{self.dataDir}/*{self.curYear}.spec")
            if len(fileList) > 1:
                logging.error(f"while reading data from {self.dataDir}, multiple files matched the required year {self.curYear}: {' '.join(fileList)}")
            if len(fileList) < 1:
                logging.error(f"while reading data from {self.dataDir}, no files matched the required year {self.curYear}")

            self.curInputFile=Dataset(fileList[0], "r")


        
