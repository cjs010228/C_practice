typedef struct NODE_S{
    struct NODE_S *lastNode;
    struct NODE_S *nextNode;
    int value;
}NODE_T;

void LinkListSort(NODE_T *head, NODE_T *newMem);
void TraverseListlist(NODE_T *head);