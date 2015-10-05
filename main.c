#include <stdio.h>
#include <stdbool.h>//It is essential to use the bool variable.
#include <string.h>
#include <stdlib.h>

void Dup_str_func(char[], char**);

int main()
{
    char str[]="My name is Brandon";
	char *Dup_str;
    Dup_str_func(str, &Dup_str);

    printf("origin string: %s\n",str);
    printf("dup string: %s\n",Dup_str);

    return 0;
}

void Dup_str_func(char orin[], char **dup)
{
    int i;
    printf("%p\n",dup);
    (*dup) = malloc(strlen(orin));
    printf("%p\n",dup);
    for(i=0;orin[i]!='\0';i++)
    {        
         (*dup)[i] = orin[i];
    }
}
