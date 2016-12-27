#include <stdio.h>
#include "sortLinklist.h"

static void JumpTheList(NODE_T *backMem, NODE_T *newMem); 
void TraverseListlist(NODE_T *head);

void LinkListSort(NODE_T *head, NODE_T *newMem)
{
    NODE_T *curNode;
    curNode = head->nextNode;
    while(1)
    {
        if (curNode == NULL)
        {
            newMem->lastNode = curNode->lastNode;
            newMem->nextNode = NULL;
            curNode->nextNode = newMem;
            break;
        }

        if (curNode->value < newMem->value)
        {
            JumpTheList(curNode, newMem);
            break;
        }

        curNode = curNode->nextNode;
    }
}

void TraverseListlist(NODE_T *head)
{
    NODE_T *curNode;
    
    curNode = head->nextNode;
    while(curNode != NULL)
    {
        printf("%d\n", curNode->value);
        curNode = curNode->nextNode;
    }

}

static void JumpTheList(NODE_T *backMem, NODE_T *newMem)
{
    newMem->lastNode = backMem->lastNode;
    newMem->nextNode = backMem;
    backMem->lastNode = newMem;
}
