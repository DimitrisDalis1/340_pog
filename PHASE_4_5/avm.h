#ifndef AVM_HEADER
#define AVM_HEADER
#include <stdio.h>
#include "table.h"


#define AVM_STACKENV_SIZE 4

#define AVM_ENDING_PC codeSize
#define AVM_MAX_INSTRUCTIONS (unsigned) nop_v
#define AVM_NUMACTUALS_OFFSET +4
#define AVM_SAVEDPC_OFFSET +3
#define AVM_SAVEDTOP_OFFSET +2
#define AVM_SAVEDTOPSP_OFFSET +1
#define AVM_STACKSIZE 4096

#define STACK_CHECK \
    if(top < 0){ \
    avm_error("Stack underflow!", &code[pc]); \
    exit(1); \
}else if(top > AVM_STACKSIZE) { \
    avm_error("Stack overflow!", &code[pc]); \
    exit(1); \
}; \

extern avm_memcell avm_stack[AVM_STACKSIZE];
extern avm_memcell ax, bx, cx;
extern avm_memcell retval;
extern int top, topsp;

double  consts_getnumber(unsigned);
char*   consts_getstring(unsigned);
char*   libfuncs_getused(unsigned);



userfunc* avm_getfuncinfo(unsigned);

avm_memcell* avm_translate_operand(vmarg*, avm_memcell*);


typedef void (*execute_func_t) (instruction*);


void execute_assign(instruction*);
void execute_add(instruction*);
void execute_sub(instruction*);
void execute_mul(instruction*);
void execute_div(instruction*);
void execute_mod(instruction*);
void execute_uminus(instruction*);      // USELESS
void execute_and(instruction*);         // USELESS
void execute_or(instruction*);          // USELESS
void execute_not(instruction*);         // USELESS
void execute_jeq(instruction*); 
void execute_jne(instruction*);
void execute_jle(instruction*);
void execute_jge(instruction*);
void execute_jlt(instruction*);
void execute_jgt(instruction*);
void execute_call(instruction*);
void execute_pusharg(instruction*);
void execute_return(instruction*);      // USELESS
void execute_getretval(instruction*);   // USELESS
void execute_funcenter(instruction*);
void execute_funcexit(instruction*);
void execute_newtable(instruction*);
void execute_tablegetelem(instruction*);
void execute_tablesetelem(instruction*);
void execute_jump(instruction*);
void execute_nop(instruction*);   

extern execute_func_t executeFuncs[];
typedef void (*memclear_func_t) (avm_memcell*);

extern unsigned char   executionFinished;
extern unsigned        pc ;
extern unsigned        currLine ;
extern unsigned        codeSize ;
extern instruction*    code ;
unsigned totalActuals = 0;

void execute_cycle(void);

extern memclear_func_t memclearFuncs[];

void avm_memcellclear(avm_memcell*);


extern void memclear_string (avm_memcell*);

extern void memclear_table(avm_memcell*);

extern void avm_warning(char*,instruction*);

extern execute_assign(instruction*);

void avm_assign(avm_memcell*, avm_memcell*);

void execute_call(instruction*);

void avm_error(char*,instruction*);

char* avm_tostring(avm_memcell*);


void avm_calllibfunv(char* funcName);
void avm_callseveenvironment(void);
void avm_call_functor(avm_table* t);

extern void avm_push_table_arg(avm_table* t);

void avm_call_functor(avm_table* t);




void avm_dec_top(void);

void avm_push_envvalue(unsigned val);

void avm_callsaveenvironment(void);

extern userfunc* avm_getfuncinfo(unsigned address);

void execute_funcenter (instruction* instr);

unsigned avm_get_envvalue(unsigned i);

void execute_funcexit(instuction*);

void avm_calllibfunc(char* id);

unsigned avm_totalactuals(void);

avm_memcell* avm_getactual(unsigned i);

/* Implemantation of the library function 'print'.
    it displays every argumnet at the console.
*/

void libfunc_print(void);
void libfunc_typeof(void);
void libfunc_totalarguments(void);
void libfunc_sqrt(void);
void libfunc_cos(void);
void libfunc_sin(void);
void libfunc_strtonum(void);
void libfunc_input(void);
void libfunc_argument(void);
void libfunc_objecttotalmembers(void);
void libfunc_objectmemberkeys(void);
void libfunc_objectcopy(void);

void avm_error(char*, instruction*);
void avm_warning(char*, instruction*);

/* With the following every library functiion is manually
    added in the VM library function resolution map
*/

//void avm_registerlibfunc(char* id,library_func_t addr);

extern void avm_push_table_arg(avm_table*);

void execute_pusharg(instruction*);


typedef char*(*tostring_func_t)(avm_memcell*);

extern char* number_tostring(avm_memcell*);
extern char* string_tostring(avm_memcell*);
extern char* bool_tostring(avm_memcell*);
extern char* table_tostring(avm_memcell*);
extern char* userfunc_tostring(avm_memcell*);
extern char* libfunc_tostring(avm_memcell*);
extern char* nil_tostring(avm_memcell*);
extern char* undef_tostring(avm_memcell*);

tostring_func_t tostringFuncs[];

char* avm_tostring(avm_memcell*);


/*
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

avm_memcell* avm_tablegetelem (
            avm_table*  table,
            avm_memcell*    index
);

void avm_tablesetelem(
    avm_table*  table,
    avm_memcell*    index,
    avm_memcell* content
);
*/
void execute_tablegetelem(instruction*);

void execute_tablesetelem(instruction*);

void avm_initialize(void);

void libfunc_totalarguments(void);

#endif
