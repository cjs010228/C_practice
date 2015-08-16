#include "name_print.h"
#include "mem_set.h"

int main()
{
	int num = 0x4D;
	mem_set(&num, 42, sizeof(num));
	printf("num = %d\n length = %lu\n",num,sizeof(num));
    //name_print(my_name);
    return 0;
}

/*void name_print(char* name)
{
    printf("%s\n",name);
}*/
