#include <stdio.h>
#include "print.h"
#define SIZE 10
#define SYMBOL '#'

int main()
{
    int size;
    char symbol;
    scanf("%d %c",&size,&symbol);
    print_triangle(size, symbol);
    return 0;
}


