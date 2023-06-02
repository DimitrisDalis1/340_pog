#include"avm.h"
//#include"../read_binary/read_binary.h"
#include<math.h>
#include<ctype.h>
void execute_call(instruction* instr){
    avm_memcell* func = avm_translate_operand(&instr->result,&ax);
    assert(func);
    switch (func->type)
    {
    case userfunc_m :{
        avm_callsaveenvironment();
        pc = func->data.funcVal;
        assert(pc < AVM_ENDING_PC);
        assert(code[pc].opcode == funcenter_v);
        break;
    }
        /* code */
        case string_m: avm_calllinfunc(func->data.strVal); break;
        case libfunc_m: avm_calllinfunc(func->data.libfuncVal); break;
        case table_m: avm_calllinfunc(func->data.tableVal); break;
    default:{
        char* s = avm_tostring(func);
        avm_error("call : cannot bind '%s' to function!",s )
        free(s);
        executionFinished = 1;
        }
    }
}

void execute_funcenter (instruction* instr){
    avm_memcell* func = avm_translate_operand(&isntr->result, &ax);
    assert(func);
    assert(pc == func->data.funcVal); /* Func address should match PC. */
    /* Callee actions here. */
    totalActuals = 0;
    userfunc* funcInfo = avm_getfuncinfo(pc);
    topsp = top;
    top = top - funcInfo -> localSize; 
}

unsigned avm_get_envvalue(unsigned i){
    assert(stack[i].type = number_m);
    unsigned val = (unsigned) stack[i].data.numVal;
    assert(stack[i].data.numVal == ((double) val));
    return val;
}

void execute_funcexit(instuction* unused){
    unsigned oldTop = top;
    top = avm_get_envvalue(topsp + AVM_SAVEDTOP_OFFSET);
    pc = avm_get_envvalue(topsp + AVM_SAVEDPC_OFFSET);
    topsp = avm_get_envvalue(topsp + AVM_SAVEDTOPSP_OFFSET);

    while(++oldTop <= top)
        avm_memcellclear(&stack[oldTop]);
}





void libfunc_totalarguments(void){
    unsigned p_topsp = avm_get_envvalue(topsp + AVM_SAVEDTOPSP_OFFSET);
    avm_memcellclear(&retval);
    if(!p_topsp){
        avm_error("'totalarguments' called outside a function!", &code[pc]);
        retval.type = nil_m;
    }else{
        retval.type = number_m;
        retval.data.numVal = avm_get_envvalue(p_topsp + AVM_NUMACTUALS_OFFSET);
    }
}

void libfunc_typeof(void){
    unsigned i = avm_totalactuals();
    if(n != 1)
        avm_error("one argument (not %d) expected in 'typeof'!",n);
    else{
        /*Thats how a lib function returns a result.
        It has to only set the 'retval' register!
        */
        avm_memcellclear(&retval);/*don't forget to clean-it up!*/
        retval.type = string_m;
        retval.data.strVal = strdup(typeStrings[avm_getactual(0)->type]);
    }
}


void avm_calllibfunc(char* id){
    library_func_t f = avm_getlibraryfunc(id);
    if(!f){
        avm_error("unsupported lib func '%s' called!",id);
        executionFinished = 1;
    }
    else{
        /*Notice that enter function and  exit function
        are called manually!*/

        avm_callsaveenvironment();
        topsp = top; /*enter function sequence . no stack locals.*/
        totalActuals = 0;
        (*f)(); /*call lib function*/
        if(!executionFinished) /*an error may naturally occur inside*/
            execute_funcexit((instruction*)0); /*retrun seq*/
    }
}

unsigned avm_totalactuals(void){
    return avm_get_envvalue(topsp + AVM_NUMACTUALS_OFFSET);
}

avm_memcell* avm_getactual(unsigned i){
    assert(i < avm_totalactuals());
    return &stack[topsp + AVM_STACKENV_SIZE + i + 1];
}


void avm_registerlibfunc(char* id,library_func_t addr);

void execute_pusharg(instruction* instr){
    avm_memcell* arg = avm_translate_operand(&instr->arg1,&ax);
    assert(arg);
    /*this is actually stack[top] = arg,but we have to use
        avm_assign.*/

    avm_assign(&stack[top],arg);
    ++totalActuals;
    avm_dec_top();
}




//libfuncs

void libfunc_print(void){
    unsigned n = avm_totalactuals();
    for(unsigned i = 0; i<n; i++){
        char*s = avm_tostring(avm_getactual(i));
        puts(s);
        free(s);
    }
}

void libfunc_typeof(void);
void libfunc_totalarguments(void);
void libfunc_sqrt(void);
void libfunc_cos(void);
void libfunc_sin(void);
void libfunc_strtonum(void);
int is_number(char * str);
void libfunc_input(void);
void libfunc_argument(void);
void libfunc_objecttotalmembers(void);