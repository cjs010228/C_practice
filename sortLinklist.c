

static void JumpTheList(NODE_T *backMem, NODE_T *newMem); 

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

static void JumpTheList(NODE_T *backMem, NODE_T *newMem)
{
    newMem->lastNode = backMem->lastNode;
    newMem->nextNode = backMem;
    backMem->lastNode = newMem;
}
