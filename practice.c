#include <stdio.h>

int main(int argv,char *argc[])
{
    int x=1,y=2,z[10];
    int *ip = &x;
    printf(*ip);
    return 0;
}
