#define SIZE 1024
#include <stdio.h>
#include <assert.h>
#include<stdlib.h>
#include "symtable.h"
typedef struct stack_node{
    int data;
    struct stack_node* next;

}stack_node;

typedef struct stack_sym_node{
    SymbolTableEntry *data;
    struct stack_sym_node* next;

}stack_sym_node;


typedef struct stack
{
    struct stack_node* head;
    int length;
}stack;

typedef struct sym_stack
{
    struct stack_sym_node* head;
    int length;
}sym_stack;



struct stack* create_stack();
struct sym_stack* create_sym_stack();
void push(stack* name,int data);
void push_f(sym_stack* name,SymbolTableEntry *data);
int pop(stack* name);
SymbolTableEntry *pop_f(sym_stack* name);
