
#pragma once
#include <stdlib.h>  // rand(), srand()
#include <time.h>
#include <math.h>
#include <time.h> //for time
#include <stdio.h>
#include <string.h>
/*triple-seeding in simple C code*/


int my_strlen(char *string) //Function to calculate length of given string
{
    int i;
    for(i=0;string[i]!='\0';i++);
    return i;
}


int charseed(char *charinput){
/*storing charinput contents in charmem*/
/*loop index and initializing memtot*/
int memtot=0, n;
/*adding up memory contents*/
for (n=0;n<my_strlen(charinput);n++){memtot=memtot+(int)charinput[n];}
/*returning total*/
return memtot;
}


/*main seedrandomnumber function*/

int seedrandomnumber(char *charinput){


long seed1=(long)charseed(charinput);

long seed2=(long)time(NULL);

long seed3=(long)&charinput;

srandom(seed1+seed2+seed3);

return 0;}
