#include "name_print.h"
#include "mem_set.h"

int main()
{
	mem_set(my_name, 42, strlen(my_name));
    name_print(my_name);
    return 0;
}

/*void name_print(char* name)
{
    printf("%s\n",name);
}*/
