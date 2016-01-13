#include <stdio.h>
#include <string.h>

int main(int argc,char **argv)
{
    int i;
    for(i=0;i<argc;i++)
    {
        printf("%d th is %s\n",i,argv[i]);
    }

    if(strcmp(argv[1],"HELLO")==0)
    {
        printf("mmmm\n");
    }
    
}