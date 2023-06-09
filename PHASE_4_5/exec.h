#ifndef EXEC_HEADER
#define EXEC_HEADER
#include "avm.h"
#include "target_producer.h"


typedef void (*execute_func_t) (instruction*);
typedef double (*arithmetic_func_t)(double x, double y);
extern void execute_cycle(void); //done in exec.c

void execute_nop (instruction*);
void execute_getretval (instruction*);
void execute_return (instruction*);


void execute_call(instruction*);

void execute_funcenter (instruction*);

void execute_funcexit(instruction*);

void execute_pusharg(instruction*);

void execute_arithmetic (instruction*);

void execute_add(instruction*);
void execute_sub(instruction*);
void execute_mul(instruction*);
void execute_div(instruction*);
void execute_mod(instruction*);
void execute_uminus(instruction*);

double add_impl (double,double);
double sub_impl (double, double);
double mul_impl (double, double);
double div_impl (double, double);
double mod_impl (double, double);


void execute_newtable(instruction*);

void execute_tablegetelem(instruction*);

void execute_tablesetelem(instruction*);

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

/*never used*/
void execute_and (instruction*);
void execute_or  (instruction*);
void execute_not (instruction*);

void execute_jeq (instruction*);

void execute_jge (instruction*);

void execute_jgt (instruction*);

void execute_jle (instruction*);

void execute_jlt (instruction*);

void execute_jne (instruction*);

void execute_jump (instruction*);



extern execute_func_t executeFuncs[];
extern arithmetic_func_t arithmeticFuncs[];

#endif