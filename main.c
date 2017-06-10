#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sort.h"
#include "sortLinklist.h"
#define NUM 10
#define SORT_TYPE 2

int main()
{
#if 0    
    srand((unsigned)time(NULL));
    int cnt;
    NODE_T *head;
    head = malloc(sizeof(NODE_T));
    head->nextNode = NULL;
    head->lastNode = NULL;
    head->value = 0;
    
    for(cnt = 0; cnt < 10; cnt++)
    {
        NODE_T *newMem;
        newMem = malloc(sizeof(NODE_T));
        newMem->nextNode = NULL;
        newMem->lastNode = NULL;
        newMem->value = rand()%100;
        printf("new mem %d\n",newMem->value);
        

        LinkListSort(head, newMem);
    }
    TraverseLinklist(head);
#endif
#if 1    
    int *array=malloc(sizeof(int)*NUM);
    int i;

    srand((unsigned)time(NULL));
    for(i=0;i<NUM;i++)    
    {
        *(array+i)=rand()%100;
    }
  
    //int test[10]={92,93,90,69,91,43,5,73,47,94};
    //array=test;

    for(i=NUM-1;i>=0;i--) {printf("%d ",*(array+i));}
        printf("\n");  

    switch(SORT_TYPE)
    {
        case 1:
            BubbleSort(array, NUM);
            break;
        case 2:
            InsertionSort(array, NUM);
            break;
        case 3:
            SelectionSort(array, NUM);
            break;
        default:
            BubbleSort(array, NUM);
    }

    for(i=NUM-1;i>=0;i--) {printf("%d ",*(array+i));}
        printf("\n");   

    return 0;
#endif    
}

