#include"avm.h"

//UNCOMMENT THIS TO RUN THE TEST MAIN FUNCTION
//#define TEST
#ifdef TEST
#define TEST_MAIN_START int main(void)
#else
#define TEST_MAIN_START int definitely_not_main(void)
#endif

#include <stdio.h>

avm_memcell avm_stack[AVM_STACKSIZE];
int current_index = AVM_STACKSIZE - 1;

avm_memcell pop_avm_stack(){
    if(current_index > -1){
        printf("Avm stack underflow");
        exit(0);
    }else{
        //code that pops and returns the memcell
        --current_index;
    }
}

void push_avm_stack(avm_memcell m){
    if(current_index >= AVM_STACKSIZE){
        printf("Avm stack overflow");
        exit(0);
    }else{
        //code that pops and returns the memcell
        avm_stack[++current_index] = m;
    }
}

//stole that
TEST_MAIN_START{
    avm_initstack();
    return 0;
}