#ifndef QUADHANDLER_H
#define QUADHANDLER_H
#include "expr.h"
#include "stack.h"
#include "symtable.h"

extern Scope_node *head_scope_node;
extern SymTable_T* hash ;
extern int current_scope;
extern int yylineno;
extern char* yytext;
extern FILE* yyin;
#define EXPAND_SIZE 1024
#define CURR_SIZE 	(total*sizeof(quad))
#define NEW_SIZE	(EXPAND_SIZE*sizeof(quad)+CURR_SIZE)
extern bool isError;


typedef enum iopcode iopcode;
typedef enum scopespace_t scopespace_t;
typedef enum symbol_t symbol_t;
typedef struct symbol symbol;
typedef struct quad quad;
typedef struct call_t call_t;
typedef struct stmt_t stmt_t;
typedef struct for_t for_t;

typedef enum iopcode{
	assign,	add,	sub,	
	mul,	divi,	mod,
	uminus,	and,	or,
	not,	if_eq,	if_noteq,
	 if_lesseq, 
	if_greatereq,if_less,
	if_greater, call,	param,
	returnn,	getretval,	funcstart,
	funcend,	tablecreate,	tablegetelem,	
	tablesetelem,jump
}iopcode;



typedef struct symbol{
	char* name;
	symbol_t type;
    scopespace_t space;
    unsigned int offset;
    unsigned int scope;
    unsigned int line;
}symbol;

typedef struct quad{
	iopcode op;
	expr* result;
	expr* arg1;
	expr* arg2;
	unsigned int label;
	unsigned int line;
	unsigned int taddress;
}quad;


typedef struct call_t{
	expr* elist;
	unsigned char method;
	char* name;
}call_t;

typedef struct stmt_t {
	int breaklist;
	int contlist;
	int returnlist;
}stmt_t;

typedef struct for_t{
    int test;
    int enter;
} for_t;

scopespace_t currscopespace();
unsigned int currscopeoffset();
void incurrscopeoffset();
void enterscopespace();
void exitscopespace();
void expand();
void emit(iopcode, expr*, expr*, expr*, unsigned int, unsigned int);
char* newtempname();
void resettemp();
SymbolTableEntry* newtemp();
void resetformalargoffset();
void resetfunctionlocaloffset();
void restorecurrscopeoffset(unsigned int);
unsigned int nextquadlabel();
void patchlabel(unsigned int, unsigned int);
expr* member_item(expr* , char*);
expr* emit_iftableitem(expr*);
expr* make_call(expr*, expr* );
void check_arith(expr*, const char* );
unsigned int istempname(char*);
bool istempexpr(expr*);
void patchlabel(unsigned, unsigned);
unsigned nextquad(void);
void make_stmt(stmt_t*);
int newlist(int);
int mergelist(int, int);
void patchlist(int, int);
void printMedianCode();
void printExpr(expr*);
expr* result_finder(expr*, expr*);
expr* emitBoolean(expr*);
int check_for_valid_loop_stop(int);
#endif
