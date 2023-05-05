
#define SIZE 1024
#include <stdio.h>
#include<stdlib.h>
typedef struct myStruct{
    int data;
    myStruct* next;

}myStruct;

void push(int data);
int pop();
