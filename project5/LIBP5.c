#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>


int myfunc1(int ival)
{
    printf(ival);
    printf("myfunc1 running %d\n",ival);

    return ival+1;
}

int myfunc2(int ival)
{

    printf("myfunc2 running %d\n",ival);

    return ival*2;
}
