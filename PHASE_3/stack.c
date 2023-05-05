#include "stack.h"
myStruct* top=NULL;
myStruct* temp=NULL;
myStruct* top1=NULL;

int count = 0;
void push(int data) {
     if (top == NULL)
    {
        top =(struct node *)malloc(1*sizeof(myStruct));
        top->next = NULL;
        top->data = data;
    }
    else
    {
        temp =(struct node *)malloc(1*sizeof(myStruct));
        temp->next = top;
        temp->data = data;
        top = temp;
    }
    count++;
    printf("Node is Inserted\n\n");
}

int pop() {
     top1 = top;
 
    if (top1 == NULL)
    {
        printf("\nStack Underflow\n");
        return -1;
    }
    else
        top1 = top1->next;
    int popped = top->data;
    free(top);
    top = top1;
    count--;
    return popped;
}
