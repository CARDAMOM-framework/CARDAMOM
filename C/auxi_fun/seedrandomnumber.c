
#pragma once
#include <stdlib.h>  // rand(), srand()
#include <math.h>
#include <time.h> //for time
#include <stdio.h>
#include <string.h>
#include <stdint.h>  // For explicit integer types
/*triple-seeding in simple C code*/


int my_strlen(const char *string) //Function to calculate length of given string
{
    int i;
    for(i=0;string[i]!='\0';i++);
    return i;
}


int charseed(const char *charinput){
/*storing charinput contents in charmem*/
/*loop index and initializing memtot*/
    int memtot=0, n;
    /*adding up memory contents*/
    for (n=0; n<my_strlen(charinput);n++){
        memtot+=(n+256)*(int)charinput[n];
    }
    //Display seed number
    printf("Seed number = %i\n",memtot);
    /*returning total*/
    return memtot;
}



int charseed_filename(const char *charinput){
    /*storing charinput contents in charmem*/
    /*loop index and initializing memtot*/
    int n=my_strlen(charinput);
    if (n < 5) {
        fprintf(stderr, "Error: Input string must be at least 5 characters long.\n");
        return -1;
    }
  
    /*adding up memory contents*/
    
    char memtotchar=charinput[n-5];
    int memtot=(int)memtotchar;
    printf("Seed string = %s\n",charinput);
    printf("Seed character = %1s\n",&memtotchar);
    printf("Seed number = %i\n",memtot);
    /*returning total*/
    return memtot;
}


/*main seedrandomnumber function*/

int seedrandomnumber(const char *charinput){


    unsigned int seed1 = (unsigned int)charseed(charinput);
    unsigned int seed2 = (unsigned int)time(NULL);
    unsigned int seed3 = (unsigned int)(uintptr_t)&charinput;

    srand(seed1 + seed2 + seed3);

return 0;}
