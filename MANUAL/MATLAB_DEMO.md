Follow these steps *once*. Once you’ve successfully made it to step 4, you can then start matlab and you’re ready to use all CARDAMOM routines without any further action.
Note: PC implementation not supported (but do add notes if you’ve managed to do this!).

*What happens when you run CARDAMOM_DEMO:* \
CARDAMOM_DEMO.m will locally compile the CARDAMOM C code (using “gcc” C compiler). CARDAMOM (called from within matlab) will then show some attempts to find a starting point with probability > 0, and will then start accepting/rejecting parameter samples (note: this is a shorter-than-usual run only for demo purposes). The CARDAMOM_DEMO script will then make a figure with some of the retrieved parameter results. Completion of the CARDAMOM script means all components of CARDAMOM are operational.

Notes:
    + This works great on macs and (theoretically) on linux. I have no idea what happens on Windows.
    + Please report questions and issues encountered in FAQ & FEI section below
    + Works with Matlab 2017b and later versions. Compatibility with earlier versions is likely straightforward.
    
#### Run Matlab Demo
1. Start matlab
2. Set CARDAMOM environmental variables in matlab
    * To do this:
        - Create or edit existing matlab startup.m file: e.g. type “edit startup.m” in matlab command window to edit new or existing startup file.
        - Copy lines from CARDAMOM_2.1.6c/MATLAB/startup_template.m into startup.m file, and adapt as instructed in startup_template.m comments.
        - Save changes to startup.m
        - Make a directory called “DUMPFILES” by typing “mkdir DUMPFILES” in matlab command window OR type "mkdir DUMFILES" in terminal console (in which case, ensure you are in the desired working directory where startup.m is).
    * Tips: 
        - Type “pwd” right after starting matlab to find out current working directory
        - paths in “startup.m” should either be absolute paths, or relative to current working directory
        - to avoid github issues, either:
            + the current working directory needs to be outside the “CARDAMOM” folder, or equivalent github cloned folder (recommended)
            + (b) make a “.gitignore” file and ensure github ignores all user-made files within the “CARDAMOM_2.1.6c” folder. 
3. Quit & restart matlab 
4. Type “which startup” in matlab command window 
    * check matlab is pointing at correct startup.m file
    * In case you encounter issues, more info here (https://www.mathworks.com/help/matlab/ref/startup.html)
5. Run CARDAMOM_DEMO.
    * This is a test script to ensure all works well. If you get to the end of the demo script, then you’ve got full CARDAMOM functionality! 
    * Type “CARDAMOM_DEMO” in matlab command window
