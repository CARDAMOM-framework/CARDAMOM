#pragma once
#include <stdio.h>
int okcheck(int OK,const char *msg){
if (OK==0)
{printf("CHECK:");printf("%s\n",msg);
fflush(stdout);}
else
{printf("OKCHECK WARNING! EXPECT ERROR!");}

return 0;

}

