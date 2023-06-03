#include"avm.h"


unsigned char   executionFinished = 0;
unsigned        pc = 0;
unsigned        currLine = 0;
unsigned        codeSize = 0;
instruction*    code = NULL;
unsigned totalActuals = 0;

avm_memcell ax, bx, cx;
avm_memcell retval;
int    top, topsp;

/* needs binary file
double  consts_getnumber(unsigned index){
    return number_consts[index];
}
char*   consts_getstring(unsigned index) {
    return string_consts[index];
}
char*   libfuncs_getused(unsigned index) {
    return libfuncs[index];
}
*/

userfunc* avm_getfuncinfo(unsigned adress);

extern char* typeStrings[];



avm_memcell* avm_translate_operand(vmarg* arg, avm_memcell* reg){
    assert(arg);
    if(reg)
        avm_memcellclear(reg);
    //printf("arg type = %d\n", arg->type);
    switch(arg->type){
        case global_a: return &avm_stack[AVM_STACKSIZE - 1 - arg->val];
        case local_a: return &avm_stack[topsp - arg->val];   
        case formal_a: return &avm_stack[topsp + AVM_STACKENV_SIZE + 1 + arg->val];

        case retval_a:  return &retval;

        case number_a:  {
            reg->type = number_m;
            reg->data.numVal = consts_getnumber(arg->val);
            return reg;
        }

        case string_a:  {
            reg->type = string_m;
            reg->data.strVal = strdup(consts_getstring(arg->val));
            return reg;
        }

        case bool_a:    {
            reg->type = bool_m;
            reg->data.boolVal = arg->val;
            return reg;
        }

        case nil_a: reg->type = nil_m; return reg;

        case userfunc_a:{
            reg->type = userfunc_m;
            reg->data.funcVal = arg->val;
            return reg;
        }

        case libfunc_a: {
            reg->type = libfunc_m;
            reg->data.libfuncVal = libfuncs_getused(arg->val);
            return reg;
        }

        default: assert(0);
    }
}

void execute_cycle(void){
    if(executionFinished)
        return;
    else
    if(pc == AVM_ENDING_PC){
        executionFinished = 1;
        return;
    }
    else{
        assert(pc < AVM_ENDING_PC);
        instruction* instr = code + pc;
        assert(
            instr->opcode >=0 &&
            instr->opcode <= AVM_MAX_INSTRUCTIONS
        );
        if(instr -> srcLine)
            currLine = instr->srcLine;
        unsigned oldPC = pc;
        (*executeFuncs[instr->opcode])(instr);
        if (pc == oldPC)
            ++pc;
    }
}



void avm_memcellclear(avm_memcell* m){
    if(m->type != undef_m){
        memclear_func_t f = memclearFuncs[m->type];
        if(f)
            (*f)(m);
        m->type = undef_m;
    }
}

extern void memclear_string (avm_memcell* m){
    assert(m -> data.strVal);
    free(m-> data.strVal);
}

extern void memclear_table(avm_memcell* m){
    assert(m->data.tableVal);
    avm_tabledecrefcounter(m->data.tableVal);
}

memclear_func_t memclearFuncs[] = {
     0,  /* number */
     memclear_string,
     0,  /* bool */
     memclear_table,
     0,  /* userfunc */
     0,  /* libfunc */
     0,  /* nil */
     0   /* undef */
 };

avm_memcell* avm_tablegetelem (
            avm_table*  table,
            avm_memcell*    index
);

void avm_tablesetelem(
    avm_table*  table,
    avm_memcell*    index,
    avm_memcell* content
);

extern void memclear_table(avm_memcell* m){
    assert(m->data.tableVal);
    avm_tabledecrefcounter(m->data.tableVal);
}

/*
extern void avm_push_table_arg(avm_table* t){
    stack[top].type = table_m;
    avm_tableincerfcounter(stack[top].data.tale = t);
    ++totalActuals;
    avm_dec_top();
}
*/


void avm_dec_top(void){
    if(!top){
        avm_error("stack overflow", &code[pc]);
        executionFinished = 1;
    }
    else{
        --top;
    }
}

void avm_push_envvalue(unsigned val){
    avm_stack[top].type         = number_m;
    avm_stack[top].data.numVal  = val;
    avm_dec_top();
}

void avm_callsaveenvironment(void){
    avm_push_envvalue(totalActuals);
    //assert(code[pc].opcode == call_v);
    avm_push_envvalue(pc+1);
    avm_push_envvalue(top + totalActuals + 2);
    avm_push_envvalue(topsp);
}

void avm_error(char* format,instruction* code){
    printf("error");
}

void avm_warning(char* warning, instruction* code){
    printf("warning");
}

tostring_func_t tostringFuncs[]={
    number_tostring,
    string_tostring,
    bool_tostring,
    table_tostring,
    userfunc_tostring,
    libfunc_tostring,
    nil_tostring,
    undef_tostring
};

char* avm_tostring(avm_memcell* m){
    assert(m->type >= 0 && m->type <= undef_m);
    return (*tostringFuncs[m->type])(m);
}

char* avm_tostring(avm_memcell* cell);
