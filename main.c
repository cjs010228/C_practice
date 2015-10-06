#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define NUM 10
#define swap(a,b) { if(a!=b) {a^=b, b^=a, a^=b;} }

int main()
{
    int *array=malloc(NUM);
    int i;
    srand((unsigned)time(NULL));
    for(i=0;i<NUM;i++)    
    {
        *(array+i)=rand()%100;
    }
    
    for(i=0;i<NUM;i++)   
        printf("array[%d] = %d\n",i,*(array+i));
    
    printf("\n");
    
    for(i=0;i<NUM;i++)
    {
        int j;
        int smallest_index=i;       
        for(j=i;j<NUM;j++)
        {
            if(*(array+smallest_index)>*(array+j))
            {
                smallest_index = j;
            }
        }
        swap(*(array+i),*(array+smallest_index));
    }
    
    for(i=0;i<NUM;i++)   
        printf("array[%d] = %d\n",i,*(array+i));

    return 0;
}

