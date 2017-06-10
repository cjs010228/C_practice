/* After sorting, the array will be ascending from small index to big index */
#include <stdio.h>
#define swap(a,b) { if(a!=b) {a^=b, b^=a, a^=b;} }

void BubbleSort(int *array, int num)
{
    int i, float_idx;
    while(1)
    {
        /*Find bubble on the bottom*/
        for(float_idx=0;float_idx<(num-1);float_idx++)
        {
            if(*(array+(float_idx+1)) >= *(array+float_idx))
            {
                continue;
            }
            else
            {
                break;
            }
        }

        /*No bubble*/
        if(float_idx == num-1)
            break;
    
        /*Float to the top*/
        for(i=float_idx;i<(num-1);i++)
        {
            if(*(array+(i+1)) >= *(array+i))
                break;
            else
                swap(*(array+i),*(array+(i+1)));  
        }   
    }
}

void InsertionSort(int* array, int num)
{
    /* insert_idx is the index need to be moved*/
    int i, insert_idx=0, target_idx=0;
    for (insert_idx = num-2; insert_idx >= 0; insert_idx--) {
        /* Find the target position in the sorted group */
        target_idx = insert_idx;
        for (i = insert_idx; i < num; i++) {
            if(array[i] < array[insert_idx]) {
                target_idx=i;
            } else {
                continue;
            }    
        }

        /*Insert to the target position in the sorted group*/
        for (i = insert_idx; i < target_idx; i++)
        {
            swap(*(array+(i+1)),*(array+(i)));
        }    
    }  
}

void SelectionSort(int *array, int num)
{
    int i, insert_idx;
    for (insert_idx = 0; insert_idx < num; insert_idx++) {
        for (i = insert_idx; i < num; i++) {
            if (*(array+i) < *(array+insert_idx)){
                /*Select the minimum one in the unsorted group*/
                swap(array[i], array[insert_idx]);
            }
        }
    }
}

