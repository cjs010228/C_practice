#include <stdio.h>
#include <stdbool.h>//It is essential to use the bool variable.

int main()
{
    char num_string[]="76";
	bool is_int = true;
	int convert_int;
	char convert_char;

    /*Int and char could both catch the integer, but the char has small range, 0~255 */
	convert_int = atoi(&num_string);//the argument needs to be pointer 
	convert_char = atoi(&num_string);
	printf("int save %d, sizeof = %ld\n", convert_int, sizeof(convert_int));
    printf("char save %d, sizeof = %ld\n", convert_char, sizeof(convert_char));

    /*Bool could also save int, but it only save 0 or 1*/
	printf("True is the integer: %d, sizeof = %ld\n",is_int,sizeof(is_int));

    return 0;
}

