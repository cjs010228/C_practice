#include <stdio.h>
void print_line(int num,char symbol)
{
    int i;
    for(i=0;i<num;i++)
        printf("%c",symbol);
    printf("\n");
}
void print_triangle(int size,char symbol)
{
    int i;
    for(i=size;i>0;i--)
        print_line(i,symbol);
}
