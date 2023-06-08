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
        --current_index;
    }
}

void push_avm_stack(avm_memcell m){
    if(current_index >= AVM_STACKSIZE){
        printf("Avm stack overflow");
        exit(0);
    }else{
        avm_stack[++current_index] = m;
    }
}

void avm_initstack(void){
    for(unsigned i = 0; i < AVM_STACKSIZE; i++){
        memset(&(avm_stack[i]), 0, sizeof(avm_stack[i]));
        avm_stack[i].type = undef_m;
    }
}

TEST_MAIN_START{
    avm_initstack();
    return 0;
}