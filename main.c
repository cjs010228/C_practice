#include <stdio.h>
#include <stdlib.h>

#define STUDENT_NUM 45

typedef struct student{
    int id;
    int score;
}SInfo;

void Sort1(SInfo*);

int main()
{
    srand(0);

    int id = 0;
    SInfo student[STUDENT_NUM];
    for(id = 1; id <=STUDENT_NUM; id++)
    {
        student[id-1].id=id;
        student[id-1].score=rand()%100;
    }

    Sort1(&student);

    return 0;
}

void Sort1(SInfo* student)
{
    int i;
    //for(i = 0; i<sizeof(*student)/sizeof(SInfo); i++)
    for(i = 0; i<STUDENT_NUM; i++)
    {
        printf("%d %d\n",student[i].id,student[i].score);
    }
}