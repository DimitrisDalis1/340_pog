#ifndef EXPR_H
#define EXPR_H
#include "symtable.h"

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


typedef enum expr_t{
	var_e,
	tableitem_e,
	programfunc_e,
	libraryfunc_e,
	arithexpr_e,
	boolexpr_e,
	assignexpr_e,
	newtable_e,
	constint_e,
	constdouble_e,
	constbool_e,
	conststring_e,
	nil_e
}expr_t;

typedef struct expr{
    	expr_t type;
	SymbolTableEntry* sym;
	struct expr* index;
	int intConst;
	double numConst;
	char* strConst;
	unsigned char boolConst;
	struct expr* next;
	int truelist;
	int falselist;
}expr;

expr* newexpr(expr_t t);
expr* newexpr_conststring(char* s);
expr* newexpr_constdouble(double i);
expr* newexpr_constnil();
expr* newexpr_constbool(unsigned char b);
expr* newexpr_constint(int i);
expr* lvalue_expr(SymbolTableEntry* sym);
#endif
