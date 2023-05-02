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
	e->doubleConst = NULL;
	e->boolConst = NULL;
	e->intConst = NULL;
	e->sym = NULL;
	return e;
}

expr* newexpr_constdouble(double i){
    expr* e = newexpr(constdouble_e);
    e->doubleConst = i;
	e->strConst= NULL;
	e->boolConst = NULL;
	e->intConst = NULL;
	e->sym = NULL;
    return e;
}

expr* newexpr_constnil(){
    expr* e = newexpr(nil_e);
	e->doubleConst = NULL;
	e->strConst= NULL;
	e->boolConst = NULL;
	e->intConst = NULL;
	e->sym = NULL;
    return e;
}

expr* newexpr_constbool(unsigned char b){
    expr* e = newexpr(constbool_e);
    e->boolConst = b;
	e->doubleConst = NULL;
	e->strConst= NULL;
	e->intConst = NULL;
	e->sym = NULL;
    return e;
}

expr* newexpr_constint(int i){
    expr* e = newexpr(constint_e);
    e->intConst = i;
	e->doubleConst = NULL;
	e->strConst= NULL;
	e->boolConst = NULL;
	e->sym = NULL;
    return e;
}

expr* lvalue_expr(SymTableEntry* sym){
	assert(sym);
	expr* e=(expr*)malloc(sizeof(expr));
	memset(e,0,sizeof(expr));
	e->next=(expr*) 0;
	e->sym =sym;

	switch (sym->type){
		case var_s: e->type=var_e;
		break;
		case programfunc_s: e->type=programfunc_e;
		break;
		case libraryfunc_s: e->type=libraryfunc_e;
		break;
		default: assert(0);
	}

	return e;
}
