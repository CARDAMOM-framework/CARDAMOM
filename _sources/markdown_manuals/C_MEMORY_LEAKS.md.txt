# Diagnose and resolve memory leaks


##FAQ

##- Segmentation fault, malloc fault, or "aborted" fault, what should I do?

###on Mac OS
Use "-fsanitize=address" argument in CARDAMOM_COMPILE debug mode
Step 1. Run CARDAMOM_COMPILE with -d

Example: ./CARDAMOM/BASH/CARDAMOM_COMPILE.sh -d

Step 2. Copy compilationn command, and add "-fsanitize=address", and re-compile

Example for CARDAMOM_RUN_MODEL:
gcc ./CARDAMOM/BASH/../C/projects/CARDAMOM_GENERAL/CARDAMOM_RUN_MODEL.c -g -o ./CARDAMOM/BASH/../C/projects/CARDAMOM_GENERAL/CARDAMOM_RUN_MODEL_debug.exe -lm -L/opt/homebrew/Cellar/netcdf/4.8.1_3/lib -lnetcdf -I/opt/homebrew/Cellar/netcdf/4.8.1_3/include -fsanitize=address


Step 3. Run new debug executable

CARDAMOM/C/projects/CARDAMOM_GENERAL/CARDAMOM_RUN_MODEL_debug.exe Desktop/1_AT-Neu_exp6.cbf.nc Desktop/1_AT-Neu_exp6.cbf.cbr dumpoutputs.cbr.nc









"gcc -g -fsanitize=address -o leak leak.c"
this will output leak 



- Memory usage too high on parallel cluster, what should I do?

Option 1. Check memory usage with Mac.
Option 2. 




Longer term strategies
- Check out D
