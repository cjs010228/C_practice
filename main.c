#include <stdio.h>

typedef struct MY_DATA_S
{
    char keyWord[16];
    char password[16];
}MY_DATA_T;

MY_DATA_T data[] = {
    {"yahoo","233two4yahoo68"},
    {"google","233two4gle68"},
    {NULL, NULL}
};

int main()
{
    int i;
    MY_DATA_T *ptr;
    unsigned char a;
    printf("size %d\n", sizeof(a));

    ptr=data;
    
    while(1)
    {
        char str[16];
        printf("enter the keyword\n");
        scanf("%s", str);
        while(1)
        {
            if(strncmp(ptr->keyWord, str)==0)
            {
                printf("%s\n",ptr->password);
                break;
            }  
            ptr++;
            if(ptr->keyWord==NULL)
                break;  
        }
    }


}
