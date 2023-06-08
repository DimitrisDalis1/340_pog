#include"avm.h"

unsigned char executionFinished = 0;
unsigned        pc = 0;
unsigned        currLine = 0;
unsigned        codeSize = 0;
instruction*    code = NULL;
unsigned totalActuals = 0;

avm_memcell ax, bx, cx;
avm_memcell retval;
int    top, topsp;

/* needs binary file*/
double  consts_getnumber(unsigned index){
    return numConsts[index]; //numConsts?
}
char*   consts_getstring(unsigned index) {
    return stringConsts[index];
}
char*   libfuncs_getused(unsigned index) {
    return lib_f[index];
}


userfunc* avm_getfuncinfo(unsigned address){
    //for(int i=0 ; i< totalUserFuncs; i++){
       // if(address == address)
    //}
}

extern char* typeStrings[]; //done in bool.c



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

avm_memcell* avm_tablegetelem (avm_table*  table,avm_memcell* index){
    
}


void memclear_table(avm_memcell* m){
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

void avm_call_functor(avm_table* t){
    cx.type = string_m;
    cx.data.strVal = "()";
    avm_memcell* f = avm_tablegetelem(t, &cx);
    if(!f)
        avm_error("in calling table: no '()' element found!");
    else
    if (f->type == table_m)
        avm_call_functor(f->data,tableVal);
    else
    if(f->type == userfunc_a){
        avm_push_table_arg(t);
        avm_callsaveenvironment();
        pc = f -> data.funcVal;
        assert(pc < AVM_ENDING_PC && code[pc].opcode == funcenter_v);
    }
    else
        avm_error("in calling table: illegal '()' element value!");
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
    fprintf(stderr, "Error is: %s in line %u\n", format, code->srcLine);
}

void avm_warning(char* warning, instruction* code){
    fprintf(stderr, "Warning is: %s in line %u\n", warning, code->srcLine);
}

int libfunc_hash(char* id){
    assert(id);
    int result;
    if(strcmp("print",id)){
        result = 0;
    }else if (strcmp("typeof",id))
    {
         result = 1;
    }else if (strcmp("totalarguments",id))
    {
         result = 2;
    }else if (strcmp("sqrt",id))
    {
         result = 3;
    }else if (strcmp("cos",id))
    {
         result = 4;
    }else if (strcmp("sin",id))
    {
         result = 5;
    }else if (strcmp("strtonum",id))
    {
         result = 6;
    }else if (strcmp("input",id))
    {
         result = 7;
    }else if (strcmp("argument",id))
    {
         result = 8;
    }else if (strcmp("objecttotalmembers",id))
    {
         result = 9;
    }else if (strcmp("objectmemberkeys",id))
    {
         result = 10;
    }else if (strcmp("objectcopy",id))
    {
         result = 11;
    }else{
        result = 12;
    }
    return result;
}

char* number_tostring(avm_memcell* a){return "abc";}
char* string_tostring(avm_memcell* a){return "abc";}
char* bool_tostring(avm_memcell* a){return "abc";}
char* table_tostring(avm_memcell* a){return "abc";}
char* userfunc_tostring(avm_memcell* a){return "abc";}
char* libfunc_tostring(avm_memcell* a){return "abc";}
char* nil_tostring(avm_memcell* a){return "abc";}
char* undef_tostring(avm_memcell* a){return "abc";}

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


