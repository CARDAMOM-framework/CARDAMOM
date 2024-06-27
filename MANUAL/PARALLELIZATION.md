# Parallelization

CARDAMOM is computationally expensive, so you will want to find ways to utlize parallization when you are running your model. 

# Step 1a (Personal Computer) Check how many cores you have

Mac: https://support.macincloud.com/support/solutions/articles/8000087401-how-can-i-check-the-number-of-cpu-cores-on-a-mac
Linux: https://www.cyberciti.biz/faq/check-how-many-cpus-are-there-in-linux-system/


# Step 1b (Server) Check how many cores you have access to

Find out from your IT or google your server specific command line to check cores. 
IMPORTANT: Find out from your IT how many cores you are allowed to use. This may differ from how many cores there actually are.
Actually, if you are contacting IT, also find out how much data storage you have access to, as you will potentially be generating many large files using CARDAMOM.


# Quick and Dirty Method using Command Line/Terminal (For only a few runs)

You can manually open up multiple command lines/terminals up to the maximum number of cores on your personal computer. Run CARDAMOM in each command line. 
You can use the following command lines:

MDF Run: 

< path to CARDAMOM_MDF.exe > < path to .cbf.nc file > < path to where you want the .cbr file to be created > **do not include <> in the command line
  
Forward Run: 
  
< path to CARDAMOM_RUN_MODEL.exe > < path to .cbf.nc file > < path to created .cbr file > **do not include <> in the command line



# Use the python script tutorial (For many runs)

Background Info: Each computer core has multiple threads. For example, if a core has 4 threads, it can run 4 python instances. The python tutorial script shows an example of 
running multiple MDF runs for the same site using as many threads you have available over all of the cores you have (personal computer or server). [LINK TO PYTHON TUTORIAL SCRIPT HERE]



# Bash Script for Parallelizing
[INSERT LINK TO BASH SCRIPT HERE]
