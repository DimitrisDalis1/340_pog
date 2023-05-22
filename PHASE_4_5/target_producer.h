#include <stdlib.h>
#include "quadhandler.h"
#define EXPAND_SIZE_V 1024
#define CURR_SIZE_V 	(total*sizeof(instruction))
#define NEW_SIZE_V	(EXPAND_SIZE_V*sizeof(instruction)+CURR_SIZE_V)

extern double* numConsts;
extern unsigned totalNumConsts;
extern char** stringConsts;
extern unsigned totalStringConsts;
extern char** namedLibfuncs;
extern userfunc* userFuncs;
extern unsigned totalUserFuncs;

extern unsigned int currInstruction;
extern unsigned int totalVmargs;

extern instruction* vmargs;
extern unsigned int currInstruction;
extern unsigned int totalVmargs;

unsigned consts_newstring(char* s);
unsigned consts_newnumber(double n);
unsigned libfuncs_newused(char* s);
unsigned userfuncs_newfunc(SymbolTableEntry* sym);

void expand_v(void);
void emit_v(instruction*);
void make_operand(expr*, vmarg*);
void make_numberoperand(vmarg*, double);
void make_booloperand(vmarg* , unsigned);
void make_retvaloperand(vmarg*);
void generateF (void);
void patch_incomplete_jumps(void);
void generate (vmopcode, quad*);



generator_func_t generators[] = {
    generate_ASSIGN,
    generate_ADD,
    generate_SUB,
    generate_MUL,
    generate_DIV,
    generate_MOD,
    generate_UMINUS,
    generate_AND,
    generate_OR,
    generate_NOT,
    generate_IF_EQ,
    generate_IF_NOTEQ,
    generate_IF_LESSEQ,
    generate_IF_GREATEREQ,
    generate_IF_LESS,
    generate_IF_GREATER,
    generate_CALL,
    generate_PARAM,
    generate_RETURN,
    generate_GETRETVAL,
    generate_FUNCSTART,
    generate_FUNCEND,
    generate_NEWTABLE,
    generate_TABLEGETELEM,
    generate_TABLESETELEM,
    generate_JUMP,
    generate_NOP
};

void generate_ADD (quad*);
void generate_SUB (quad*);
void generate_MUL (quad*);
void generate_DIV (quad*);
void generate_MOD (quad*);
void generate_NEWTABLE (quad*);
void generate_TABLEGETELEM (quad*);
void generate_TABLESETELEM (quad*);
void generate_ASSIGN (quad*);
void generate_NOP ();
void generate_JUMP (quad*);
void generate_IF_EQ (quad*);
void generate_IF_NOTEQ (quad*);
void generate_IF_GREATER (quad*);
void generate_IF_GREATEREQ (quad*);
void generate_IF_LESS (quad*);
void generate_IF_LESSEQ (quad*);
void generate_NOT (quad*);
void generate_OR (quad*);
void generate_PARAM (quad*);
void generate_CALL (quad*);
void generate_GETRETVAL (quad*);
void generate_FUNCSTART (quad*);
void generate_RETURN (quad*);
void generate_FUNCEND (quad*);

typedef void (*generator_func_t) (quad*);



typedef enum vmarg_t vmarg_t;
typedef enum vmopcode vmopcode;
typedef struct vmarg vmarg;
typedef struct instruction instruction;
typedef struct userfunc userfunc;
typedef incomplete_jump incomplete_jump;


typedef enum vmarg_t{
    label_a     =0,
    global_a    =1,
    formal_a    =2,
    local_a     =3,
    number_a    =4,
    string_a    =5,
    bool_a      =6,
    nil_a       =7,
    userfunc_a  =8,
    libfunc_a   =9,
    retval_a    =10
} vmarg_t;

typedef enum vmopcode{
    assign_v,       add_v,          sub_v,
    mul_v,          div_v,          mod_v,
    uminus_v,       and_v,          or_v,
    not_v,          jeq_v,          jne_v,
    jle_v,          jge_v,          jlt_v,
    jgt_v,          call_v,     pusharg_v,
    ret_v,          getretval_v,    funcenter_v,    
    funcexit_v,     newtable_v,  tablegetelem_v, 
    tablesetelem_v, jump_v,         nop_v 
} vmopcode;

typedef struct vmarg{ 
    vmarg_t type;
    unsigned int val;
}vmarg;

typedef struct instruction{
    vmopcode opcode;
    vmarg*  result;
    vmarg* arg1;
    vmarg* arg2;
    unsigned srcLine;
}instruction;

typedef struct userfunc{
    unsigned address;
    unsigned localSize;
    char* id;
}userfunc;

struct incomplete_jump {
        unsigned  instrNo;
        unsigned iaddress;
        incomplete_jump* next;
};
