typedef struct NODE_S{
    int key;
    int value;
    struct NODE_S *next;
}NODE_T;

void Push(int key, int value);
void Pop(void);
void Transverse(void);