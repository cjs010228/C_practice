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
    int i,ist_idx=0,tg_idx=0;
    for(ist_idx=num-2; ist_idx>=0; ist_idx--)
    {
        /*Find the target position in the sorted group*/
        for(i=ist_idx,tg_idx=ist_idx;i<(num);i++)
        {
            if(*(array+(i)) < *(array+ist_idx))
            {
                tg_idx=i;
            }  
            else
            {
                continue;
            }    
        }

        /*Insert to the target position in the sorted group*/
        for(i=ist_idx; i<tg_idx; i++)
        {
            swap(*(array+(i+1)),*(array+(i)));
        }    
    }  
}

void SelectionSort(int *array, int num)
{
    int i,ist_pos,cand=0;
    for(ist_pos=num-1; ist_pos>=0; ist_pos--)
    {
        /*Select the candidate(Max or min) in the unsorted group*/
        for(i=0,cand=i;i<(ist_pos);i++)
        {
            if(*(array+(i+1)) > *(array+cand))
                cand=i+1;
            else
                continue;
        }

        /*Put the candidate to the tail of the sorted group*/
        swap(*(array+ist_pos),*(array+(cand)));
    }
}
