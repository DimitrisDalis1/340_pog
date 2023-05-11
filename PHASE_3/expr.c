#include "expr.h"
expr* newexpr(expr_t t){
	expr* e=(expr*) malloc(sizeof(expr));
	memset(e,0,sizeof(expr));
#include "expr.h"
expr* newexpr(expr_t t){
	expr* e=(expr*) malloc(sizeof(expr));
	memset(e,0,sizeof(expr));
	e->type=t;
	e->truelist = 0;
        e->falselist = 0;
	return e;
}

expr* newexpr_conststring(char* s){
	expr* e=newexpr(conststring_e);
	e->strConst= strdup(s);
		e->sym = NULL;
	return e;
}

expr* newexpr_constdouble(double i){
    expr* e = newexpr(constdouble_e);
    e->numConst = i;
    e->sym = NULL;
    return e;
}

expr* newexpr_constnil(){
    expr* e = newexpr(nil_e);
		e->sym = NULL;
    return e;
}

expr* newexpr_constbool(unsigned char b){
    expr* e = newexpr(constbool_e);
    e->boolConst = b;
	e->sym = NULL;
    return e;
}

expr* newexpr_constint(int i){
    expr* e = newexpr(constint_e);
    e->intConst = i;
	e->sym = NULL;
    return e;
}

expr* lvalue_expr(SymbolTableEntry* sym){
	assert(sym);
	expr* e=(expr*)malloc(sizeof(expr));
	memset(e,0,sizeof(expr));
	e->next=(expr*) 0;
	e->sym =sym;

	switch (sym->type){
		case GLOBAL: e->type=var_e;
		break;
		case LOCALV: e->type=var_e;
		break;
		case FORMAL: e->type=var_e;
		break;
		case USERFUNC: e->type=programfunc_e;
		break;
		case LIBFUNC: e->type=libraryfunc_e;
		break;
		default: assert(0);
	}

	return e;
}
	e->type=t;
	e->truelist = 0;
        e->falselist = 0;
	return e;
}

expr* newexpr_conststring(char* s){
	expr* e=newexpr(conststring_e);
	e->strConst= strdup(s);
		e->sym = NULL;
	return e;
}

expr* newexpr_constdouble(double i){
    expr* e = newexpr(constdouble_e);
    e->numConst = i;
    e->sym = NULL;
    return e;
}

expr* newexpr_constnil(){
    expr* e = newexpr(nil_e);
		e->sym = NULL;
    return e;
}

expr* newexpr_constbool(unsigned char b){
    expr* e = newexpr(constbool_e);
    e->boolConst = b;
	e->sym = NULL;
    return e;
}

expr* newexpr_constint(int i){
    expr* e = newexpr(constint_e);
    e->intConst = i;
	e->sym = NULL;
    return e;
}

expr* lvalue_expr(SymbolTableEntry* sym){
	assert(sym);
	expr* e=(expr*)malloc(sizeof(expr));
	memset(e,0,sizeof(expr));
	e->next=(expr*) 0;
	e->sym =sym;

	switch (sym->type){
		case GLOBAL: e->type=var_e;
		break;
		case LOCALV: e->type=var_e;
		break;
		case FORMAL: e->type=var_e;
		break;
		case USERFUNC: e->type=programfunc_e;
		break;
		case LIBFUNC: e->type=libraryfunc_e;
		break;
		default: assert(0);
	}

	return e;
}
