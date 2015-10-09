#include <stdio.h>
#include <string.h>
char smallest_character(char*);

int main()
{
    char smallest_chr;
    char test_string[]="HeythisisBrandon";
    printf("size is %lu\n",strlen(test_string));
    smallest_chr=smallest_character(test_string);
    printf("smallest is '%c' %d\n",smallest_chr,smallest_chr);
    return 0;
}

char smallest_character(char* str)
{
    int i=0, smallest_index=0;
    printf("size is %lu\n",strlen(str));
    while(1)
    {
        if(str[smallest_index] >= str[i])
            smallest_index = i;
        i++;
        if(str[i]=='\0')
            break;        
        
        
    }
    return str[smallest_index];
}

