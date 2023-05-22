#include "stack.h"
stack_node* temp_=NULL;
stack_node* top1=NULL;
stack_sym_node* temp2=NULL;
stack_sym_node* top2=NULL;

struct stack* create_stack(){
    stack* temp;
    temp = malloc(sizeof(stack));
    assert(temp);
    temp->head = NULL;
    temp->length = 0;
    return temp;
}


struct sym_stack* create_sym_stack(){
    sym_stack* temp;
    temp = malloc(sizeof(sym_stack));
    assert(temp);
    temp->head = NULL;
    temp->length = 0;
    return temp;
}



void push(stack* name,int data) {
     if (name->head == NULL)
    {
        name->head  =(struct stack_node *)malloc(1*sizeof(stack_node));
        name->head->next = NULL;
        name->head->data = data;
    }
    else
    {
        temp_ =(struct stack_node *)malloc(1*sizeof(stack_node));
        temp_->next = name->head ;
        temp_->data = data;
        name->head  = temp_;
    }
    name->length++;
    //printf("Node is Inserted %d\n\n", name->head->data);
}

int pop(stack* name) {
    assert(name);
     top1 = name->head ;
 
    if (top1 == NULL)
    {
        printf("\nStack Underflow\n");
        return -1;
    }
    else
        top1 = top1->next;
    int popped = name->head ->data;
    free(name->head );
    name->head  = top1;
    name->length--;
    return popped;
}




void push_f(sym_stack* name,SymbolTableEntry *data){
     if (name->head == NULL)
    {
        name->head  =(struct stack_sym_node *)malloc(1*sizeof(stack_sym_node));
        name->head->next = NULL;
        name->head->data = data;
    }
    else
    {
        temp2 =(struct stack_sym_node *)malloc(1*sizeof(stack_sym_node));
        temp2->next = name->head ;
        temp2->data = data;
        name->head  = temp2;
    }
    name->length++;
    //printf("Node is Inserted %d\n\n", name->head->data);
}

SymbolTableEntry * pop_f(sym_stack* name) {
    assert(name);
     top2 = name->head ;
 
    if (top2 == NULL)
    {
        printf("\nStack Underflow\n");
        return NULL;
    }
    else
        top2 = top2->next;
    SymbolTableEntry *popped = name->head ->data;
    free(name->head );
    name->head  = top2;
    name->length--;
    return popped;
}
