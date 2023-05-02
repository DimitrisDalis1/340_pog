#include "symtable.h"
#include "expr.h"
extern Scope_node *head_scope_node;
extern SymTable_T* hash ;
extern int current_scope;
extern int yylineno;
extern char* yytext;
extern FILE* yyin;
#define EXPAND_SIZE 1024
#define CURR_SIZE 	(total*sizeof(quad))
#define NEW_SIZE	(EXPAND_SIZE*sizeof(quad)+CURR_SIZE)

typedef enum iopcode iopcode;
typedef enum scopespace_t scopespace_t;
typedef enum symbol_t symbol_t;
typedef struct symbol symbol;
typedef struct quad quad;
typedef struct call call;
typedef struct stmt_t stmt_t;

typedef enum iopcode{
	assign,	add,	sub,	
	mul,	div,	mod,
	uminus,	and,	or,
	not,	if_eq,	if_noteq,
	if_greater,	call,	param,
	ret,	getretval,	funcstart,
	funcend,	tablecreate,	tablegetelem,	
	tablesetelem
}iopcode;

typedef enum scopespace_t{
	programvar,
	functionlocal,
	formalarg
}scopespace_t;

typedef enum symbol_t{
	var_s,
	programfunc_s,
	libraryfunc_s	
}symbol_t;

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
}quad;

typedef struct call{
	expr* elist;
	unsigned char method;
	char* name;
}call;

typedef struct stmt_t {
	int breakList;
	int contList;
	int returnlist;
}stmt_t;

scopespace_t currscopespace();
unsigned int currscopeoffset();
void incurrscopeoffset();
void enterscopespace();
void exitscopespace();
void expand();
void emit(iopcode, expr*, expr, expr*, unsigned int, unsigned int);
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
unsigned int istempexpr(expr*);
void patchlabel(unsigned, unsigned);
expr* newexpr_constbool(unsigned int);
unsigned nextquad(void);
void make_stmt(stmt_t*);
int newlist(int);
int mergelist(int, int);
void patchlist(int, int);
