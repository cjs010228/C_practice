#include <stdio.h>
#include "sortLinklist.h"

static void JumpTheList(NODE_T *backMem, NODE_T *newMem); 

void LinkListSort(NODE_T *head, NODE_T *newMem)
{
    NODE_T *curNode;
    NODE_T *oldCurNode;
    oldCurNode = head;
    curNode = head->nextNode;

    while(1)
    {
        if (curNode == NULL)
        {
            if (oldCurNode == head)
            {
                newMem->lastNode = head;
                newMem->nextNode = NULL;
                head->nextNode = newMem;
                break;
            }

            newMem->lastNode = oldCurNode;
            newMem->nextNode = NULL;
            oldCurNode->nextNode = newMem;
            break;
        }

        if (newMem->value >= curNode->value)
        {
            JumpTheList(curNode, newMem);
            break;
        }

        oldCurNode = curNode;
        curNode = curNode->nextNode;
    }
}

void TraverseLinklist(NODE_T *head)
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
    backMem->lastNode->nextNode = newMem;
    backMem->lastNode = newMem;
}
