

#define SIZE 1024
#include <stdio.h>
#include <assert.h>
#include<stdlib.h>
typedef struct stack_node{
    int data;
    struct stack_node* next;

}stack_node;

typedef struct stack
{
    struct stack_node* head;
    int length;
}stack;


struct stack* create_stack();
void push(stack* name,int data);
int pop(stack* name);
