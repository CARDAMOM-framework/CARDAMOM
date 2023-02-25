#pragma once
#include <stdio.h>
int okcheck(int OK, char * msg){

printf("OKCHECK:%s\n",msg);


if (OK!=0)
{printf("OKCHECK WARNING! EXPECT ERROR!");}

fflush(stdout);
return 0;

}

