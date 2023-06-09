#ifndef AVM_HEADER
#define AVM_HEADER
#include <stdio.h>
#include "table.h"
#include "target_producer.h"


#define AVM_STACKENV_SIZE 4

#define AVM_ENDING_PC insr_s
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

extern unsigned char   executionFinished;
extern int user,str,num,fun;
extern userfunc* userfs;
extern char** str_c;
extern double* numbers;
extern char** lib_f;
extern instruction * instrs;

extern avm_memcell avm_stack[AVM_STACKSIZE];
extern avm_memcell ax, bx, cx;
extern avm_memcell retval;
extern int top, topsp;

double  consts_getnumber(unsigned);
char*   consts_getstring(unsigned);
char*   libfuncs_getused(unsigned);



userfunc* avm_getfuncinfo(unsigned); //NEEDS IMPLEMENTATION IN avm.c

avm_memcell* avm_translate_operand(vmarg*, avm_memcell*); //done in avm.c


typedef void (*execute_func_t) (instruction*);


void execute_assign(instruction*); //done in assign.c
void execute_add(instruction*); //done in arithm.c
void execute_sub(instruction*); //done in arithm.c
void execute_mul(instruction*); //done in arithm.c
void execute_div(instruction*); //done in arithm.c
void execute_mod(instruction*); //done in arithm.c
void execute_uminus(instruction*);      // USELESS
void execute_and(instruction*);         // USELESS
void execute_or(instruction*);          // USELESS
void execute_not(instruction*);         // USELESS
void execute_jeq(instruction*); //done in bool.c
void execute_jne(instruction*); //done in bool.c
void execute_jle(instruction*); //done in bool.c
void execute_jge(instruction*); //done in bool.c
void execute_jlt(instruction*); //done in bool.c
void execute_jgt(instruction*); //done in bool.c
void execute_call(instruction*); //done in function.c
void execute_pusharg(instruction*); //done in function.c
void execute_return(instruction*);      // USELESS
void execute_getretval(instruction*);   // USELESS
void execute_funcenter(instruction*); //done in function.c
void execute_funcexit(instruction*); //done in function.c
void execute_newtable(instruction*); //done in table.c
void execute_tablegetelem(instruction*); //done in table.c
void execute_tablesetelem(instruction*); //done in table.c
void execute_jump(instruction*); //done in bool.c
void execute_nop(instruction*);   //USELESS

extern execute_func_t executeFuncs[];
typedef void (*memclear_func_t) (avm_memcell*);

extern unsigned        pc ;
extern unsigned        currLine ;
extern unsigned        codeSize ;
extern instruction*    code ;
extern unsigned totalActuals;

extern void execute_cycle(void); //done in exec.c

extern memclear_func_t memclearFuncs[]; 

void avm_memcellclear(avm_memcell*); //done in avm.c


extern void memclear_string (avm_memcell*); //done in avm.c

extern void memclear_table(avm_memcell*); //done in avm.c

//avm.c
void avm_error(char*,instruction*); // NEEDS IMPLEMENTATION
extern void avm_warning(char*,instruction*); // NEEDS IMPLEMENTATION

/*extern execute_assign(instruction*); */

void avm_assign(avm_memcell*, avm_memcell*); //done in assign.c

void execute_call(instruction*); //done in functions.c



char* avm_tostring(avm_memcell*);  //done in avm.c


void avm_calllibfunc(char*); //done in functions.c
void avm_callsaveenvironment(void);  //done in avm.c
void avm_call_functor(avm_table*); //in avm.c bit in comments

extern void avm_push_table_arg(avm_table* t); // In avm.c NOT IMPLEMENTED

void avm_call_functor(avm_table* t); // In avm.c NOT IMPLEMENTED

void avm_dec_top(void); //done in avm.c

void avm_push_envvalue(unsigned); //done in avm.c

void avm_callsaveenvironment(void); //done in avm.c


extern userfunc* avm_getfuncinfo(unsigned address); //NEEDS IMPLEMENTATION IN  avm.c


unsigned avm_get_envvalue(unsigned); //done in functions.c


void avm_calllibfunc(char*); //done in functions.c

unsigned avm_totalactuals(void); //done in functions.c

avm_memcell* avm_getactual(unsigned i); //done in functions.c


extern void libfunc_print(void);  //done in function.c
extern void libfunc_typeof(void); //done in function.c
extern void libfunc_totalarguments(void); //done in function.c
extern void libfunc_sqrt(void); //done in function.c
extern void libfunc_cos(void); //done in function.c
extern void libfunc_sin(void); //done in function.c
extern void libfunc_strtonum(void); //done in function.c
extern void libfunc_input(void); //done in function.c
extern void libfunc_argument(void); //done in function.c
extern void libfunc_objecttotalmembers(void); //done in function.c
extern void libfunc_objectmemberkeys(void); //done in function.c
extern void libfunc_objectcopy(void); //done in function.c

void avm_error(char*, instruction*); //NEEDS IMPEMENTATION
void avm_warning(char*, instruction*); //NEED IMPLEMENTATION

/* With the following every library functiion is manually
    added in the VM library function resolution map
*/
typedef void (*library_func_t)(void);

/*Struct wiht a counter and an array for the library functions*/
typedef struct LibFuncHash{
    unsigned counter;
    library_func_t LibTable[12];
}LibFuncHash;

extern LibFuncHash* libfunc_hashtable;

int libfunc_hash(char* id); // done in avm.c

void avm_registerlibfunc(char* id,library_func_t addr); //done in avm.c

extern void avm_push_table_arg(avm_table*);

void execute_pusharg(instruction*); //done in function.c

extern char* typeStrings[]; //done in bool.c

typedef char*(*tostring_func_t)(avm_memcell*);

extern char* number_tostring(avm_memcell*);   //done in bool.c
extern char* string_tostring(avm_memcell*);  //done in bool.c
extern char* bool_tostring(avm_memcell*);   //done in bool.c
extern char* table_tostring(avm_memcell*); //done in bool.c
extern char* userfunc_tostring(avm_memcell*); //done in bool.c
extern char* libfunc_tostring(avm_memcell*); //done in bool.c
extern char* nil_tostring(avm_memcell*);    //done in bool.c
extern char* undef_tostring(avm_memcell*); //done in bool.c

//tostring_func_t tostringFuncs[];

char* avm_tostring(avm_memcell*); //done in avm.c

void execute_tablegetelem(instruction*); //done in table.c

void execute_tablesetelem(instruction*); //done in table.c

void avm_initialize(void); //done in avm_main.c, uparxei kapou auth?

void avm_initstack(void);

void libfunc_totalarguments(void); //done in functions.c

extern void avm_tablesetelem(avm_table* table,avm_memcell* index,avm_memcell* content);


extern int disable_remove_warning;

#endif
