#include "linklist_stack.h"
#include <stdio.h>
#include <stdlib.h>
NODE_T* head = NULL;

int main() {


    Push(1, 5);
    Push(2, 8);
    Push(3, 7);

    //Transverse();

    Pop();
    Pop();
    Pop();
    Pop();
}

void Push(int key, int value){
    NODE_T* current = malloc(sizeof(NODE_T));
    current->key = key;
    current->value = value;
    printf("Push %d %d\n", current->key, current->value);

    if(head == NULL) {
        head = current;
        current->next = NULL;
    } else {
        current->next = head;
        head = current;
    }
}
void Pop(void) {
    static NODE_T *previous;
    printf("Pop %d %d\n", head->key, head->value);
    
    previous = head;
    head = head->next;

    free(previous);
    previous = NULL;
}
void Transverse(void);