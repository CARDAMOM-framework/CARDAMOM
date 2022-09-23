Diagnose and resolve memory leaks







##FAQ

##- Segmentation fault, what should I do?

###on Mac OS
Use "-fsanitize=address" argument in CARDAMOM_COMPILE debug mode
Step 1. Run CARDAMOM_COMPILE with -d 
Step 2. Copy compilationn command, and add "-fsanitize=address", and re-compile
Step 3. Run new debug executable


"gcc -g -fsanitize=address -o leak leak.c"
this will output leak 



- Memory usage too high on parallel cluster, what should I do?

Option 1. Check memory usage with Mac.
Option 2. 




Longer term strategies
- Check out D
