#include <stdio.h>
#include "string.h"
#define my_name "Brandon"

int main()
{
	char s1[] = my_name;
	char s2[20];
    strcpy(s2,s1);//Both of the two variable need to occupy the memory, and could not be a pointer.
    printf("%s\n",s2);
    return 0;
}

