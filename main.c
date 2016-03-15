#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define NUM 10
#define swap(a,b) { if(a!=b) {a^=b, b^=a, a^=b;} }

int main()
{
	int *array=malloc(NUM);
	int i, j, float_idx,k,counter=0;

	srand((unsigned)time(NULL));
	for(i=0;i<NUM;i++)    
	{
		*(array+i)=rand()%100;
	}
	
	while(1)
	{
		/*Find bubble ont the bottom*/
		for(float_idx=0;float_idx<(NUM-1);float_idx++)
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
		if(float_idx == NUM-1)
			break;
	
        /*Float to the top*/
		for(j=float_idx;j<(NUM-1);j++)
		{
			if(*(array+(j+1)) >= *(array+j))
				break;
			else
				swap(*(array+j),*(array+(j+1)));  
		}  

		for(k=NUM-1;k>=0;k--) {printf("%d ",*(array+k));}
		printf("\n");   
 
	}

	return 0;
}

