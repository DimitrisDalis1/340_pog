#ifndef EXPR_H
#define EXPR_H
#include "symtable.h"
#include <stdlib.h>
#include <string.h>

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
    	
	struct SymbolTableEntry* sym;
	struct expr* index;
	int intConst;
	double numConst;
	char* strConst;
	unsigned char boolConst;
	struct expr* next;
	enum expr_t type;
	int truelist;
	int falselist;
	int iaddress;
}expr;

expr* newexpr(expr_t t);
expr* newexpr_conststring(char* s);
expr* newexpr_constdouble(double i);
expr* newexpr_constnil();
expr* newexpr_constbool(unsigned char b);
expr* newexpr_constint(int i);
expr* lvalue_expr(struct SymbolTableEntry* sym);
#endif