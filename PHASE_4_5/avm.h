#ifndef AVM_HEADER
#define AVM_HEADER
#include <stdio.h>
#include<math.h>
#include<ctype.h>
#define PI 3.141592654
#include "exec.h"
#include "target_producer.h"

#define AVM_TABLE_HASHSIZE  211
#define HASH_MULTIPLIER 65599
extern int disable_remove_warning;

#define AVM_STACKENV_SIZE 4

#define AVM_ENDING_PC codeSize+1
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

typedef struct avm_memcell avm_memcell;

typedef enum avm_memcell_t {
    number_m    = 0,
    string_m    = 1,
    bool_m      = 2,
    table_m     = 3,
    userfunc_m  = 4,
    libfunc_m   = 5,
    nil_m       = 6,
    undef_m     = 7
} avm_memcell_t;

typedef struct avm_table_bucket {
    struct avm_memcell* key;
    struct avm_memcell* value;
    struct avm_table_bucket* next;
} avm_table_bucket;


typedef struct avm_table{
    unsigned refCounter;
    avm_table_bucket* strIndexed[AVM_TABLE_HASHSIZE];
    avm_table_bucket* numIndexed[AVM_TABLE_HASHSIZE];
    avm_table_bucket* boolIndexed[AVM_TABLE_HASHSIZE];
    avm_table_bucket* userfuncIndexed[AVM_TABLE_HASHSIZE];
    avm_table_bucket* libfuncIndexed[AVM_TABLE_HASHSIZE];
    avm_table_bucket* tableIndexed[AVM_TABLE_HASHSIZE];
    unsigned total;
}avm_table;

typedef struct avm_memcell {
    avm_memcell_t type;
    union {
        double          numVal;
        char*           strVal;
        unsigned char   boolVal;
        avm_table*      tableVal;
        unsigned        funcVal;
        char*           libfuncVal;
    } data;
} avm_memcell;


typedef void (*memclear_func_t) (avm_memcell*);

typedef void (*library_func_t)(void);

/*Struct wiht a counter and an array for the library functions*/
typedef struct LibFuncHash{
    unsigned counter;
    library_func_t LibTable[12];
}LibFuncHash;


typedef char*(*tostring_func_t)(avm_memcell*);



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

extern unsigned char avm_tobool (avm_memcell* m);
extern int avm_toarithm (avm_memcell* m);



avm_table* avm_tablenew (void);
void avm_tabledestroy (avm_table* t);
avm_memcell* avm_tablegetelem(avm_table*  table,avm_memcell* index);
void avm_tablesetelem(avm_table* table,avm_memcell* index,avm_memcell* content);
void avm_tableincrefcounter (avm_table* );
void avm_tabledecrefcounter (avm_table* );
void avm_tablebucketsinit (avm_table_bucket** );
avm_table* avm_tablenew(void);
void avm_memcellclear(avm_memcell* m);
void avm_tablebucketsdestroy (avm_table_bucket**);




extern unsigned        pc ;
extern unsigned        currLine ;
extern unsigned        codeSize ;
extern instruction*    code ;
extern unsigned totalActuals;



extern memclear_func_t memclearFuncs[]; 

void avm_memcellclear(avm_memcell*); //done in avm.c


extern void memclear_string (avm_memcell*); //done in avm.c

extern void memclear_table(avm_memcell*); //done in avm.c


void avm_assign(avm_memcell*, avm_memcell*); //done in assign.c

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



extern LibFuncHash* libfunc_hashtable;

int libfunc_hash(char* id); // done in avm.c

void avm_registerlibfunc(char* id,library_func_t addr); //done in avm.c

extern void avm_push_table_arg(avm_table*);

extern char* typeStrings[]; //done in bool.c

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

void avm_initialize(void); //done in avm_main.c, uparxei kapou auth?

void avm_initstack(void);

void libfunc_totalarguments(void); //done in functions.c

extern void avm_tablesetelem(avm_table* table,avm_memcell* index,avm_memcell* content);


extern int disable_remove_warning;

#endif
