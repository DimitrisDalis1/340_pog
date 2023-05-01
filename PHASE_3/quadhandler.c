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

typedef enum expr_t{
	var_e,
	tableitem_e,
	programfunc_e,
	libraryfunc_e,
	arithexpr_e,
	boolexpr_e,
	assignexpr_e,
	newtable_e,
	constnum_e,
	constbool_e,
	conststring_e,
	nil_e
}expr_t;

typedef struct expr{
    expr_t type;
	SymTableEntry* sym;
	expr* index;
	int intConst;
	double numConst;
	char* strConst;
	unsigned char boolConst;
	expr* next;
	int truelist;
	int falselist;
}expr;

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


quad*	quads = (quad*) 0;
unsigned total = 0;
unsigned int currQuad = 0;
int tempcounter=0;

unsigned programVarOffset =0; 
unsigned functionLocalOffset=0;
unsigned formalArgOffset=0;
unsigned scopeSpaceCounter=1;

scopespace_t currscopespace(){
	if(scopeSpaceCounter==1){
		return programvar;
	}else if(scopeSpaceCounter % 2 == 0){
  		return formalarg;
	}else{
		return functionlocal;
	}
}

unsigned int currscopeoffset(){
	switch(currscopespace()){
		case programvar:
			return programVarOffset;
		case functionlocal:
			return functionLocalOffset;
		case formalarg: 
			return formalArgOffset;
		default: assert(0);	
	}
}

void incurrscopeoffset(){
	switch(currscopespace()){
		case programvar:
			++programVarOffset;
			break;
		case functionlocal:
			++functionLocalOffset;
			break;
		case formalarg: 
			++formalArgOffset;
			break;
		default: assert(0);	
	}
		
}

void enterscopespace(){
	++scopeSpaceCounter;
}

void exitscopespace(){
	assert(scopeSpaceCounter>1);
	--scopeSpaceCounter;
}

void expand(){
	assert(total==currQuad);
	quad* p=(quad*) malloc(NEW_SIZE);
	if(quads){
		memcpy(p, quads, CURR_SIZE);
		free(quads);
	}
	quads=p;
	total+=EXPAND_SIZE;
}

void emit(
	iopcode op,
	expr* result,
	expr* arg1,
	expr* arg2,
	unsigned label,
	unsigned line
	){
	
	if(currQuad == total)
		expand();

	quad* p= quads+currQuad++;
	p->arg1= arg1;
	p->arg2= arg2;
	p->result=result;
	p->label=label;
	p->line= line;
	p->op=op;
}


//hidden variables functions
char* newtempname(){
	char* name=malloc(1024*sizeof(char));
	sprintf(name,"_t%u",tempcounter++);
	return name;
}

void resettemp(){ tempcounter=0; }

SymbolTableEntry* newtemp(){
	char* name=newtempname();
	SymbolTableEntry* sym=lookup_inScope_wA(hash,name,current_scope);
	if(sym==NULL){
		if(current_scope==0)
		{
			return SymTable_insert(hash,name,yylineno,NULL,current_scope,GLOBAL);

		}else{
			return SymTable_insert(hash,name,yylineno,NULL,current_scope,LOCALV);
		}
		
	}else{
		return sym;
	}

}

void resetformalargoffset(){
	formalArgOffset=0;
}

void resetfunctionlocaloffset(){
	functionLocalOffset=0;
}

void restorecurrscopeoffset(unsigned int n){
	switch(currscopespace()){
		case programvar: programVarOffset=n;
		break;
		case functionlocal: functionLocalOffset=n;
		break;
		case formalarg: formalArgOffset=n;
		break;
		default: assert(0);
	}
}

unsigned int nextquadlabel(){
	return currQuad;
}

void patchlabel(unsigned int quadNo, unsigned int label){
	assert(quadNo<currQuad);
	quads[quadNo].label=label;
}

expr* member_item(expr* lv, char* name){
	lv=emit_iftableitem(lv);
	expr* ti=newexpr(tableitem_e);
	ti->sym=lv->sym;
	ti->index= newexpr_conststring(name);
	return ti;
}
///////////////////////////////////////////////////////////////////////////
 
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

///////////////////////////////////////////////////////////////////////////////

expr* emit_iftableitem(expr* e){
	if(e->type !-tableitem_e){
		return e;
	}else{
		expr* result = newexpr(var_e);
		result->sym=newtemp();
		emit(
			tablegetelem,
			e,
			e->index,
			result
		);
		return result;
	}
}

typedef struct call{
	expr* elist;
	unsigned char method;
	char* name;
}call;

expr* make_call(expr* lv, expr* reversed_elist){
	expr* func=emit_iftableitem(lv);
	while(reversed_elist){
		emit(param,reversed_elist,NULL,NULL);
		reversed_elist=reversed_elist->next;
	}
	emit(call,func,NULL,NULL);
	expr* result=newexpr(var_e);
	result->sym=newtemp();
	emit(getretval,NULL,NULL,result);

	return result;
}

void check_arith(expr*e, const char* context){
	if ( e->type == constbool_e ||
		e->type == conststring_e ||
		e->type == nil_e ||
		e->type == newtable_e ||
		e->type == programfunc_e ||
		e->type == libraryfunc_e ||
		e->type == boolexpr_e )
	printf("Illegal expr used in %s!", context); ////////////make new function for comiletime errorrs
}

unsigned int istempname(char* s){
	return *s=='_';
}

unsigned int istempexpr(expr* e){
	return e->sym && istempname(e->sym->name); ///////get name if variable or function
}

void patchlabel (unsigned quadNo, unsigned label) {
	assert(quadNo < currQuad && !quads[quadNo].label);
	quads[quadNo].label = label;
}
expr* newexpr_constbool (unsigned int b) {
	expr* e = newexpr(constbool_e);
	e->boolConst = !!b;
	return e;
}
unsigned nextquad (void) { return currQuad; }

typedef struct stmt_t {
	int breakList;
	int contList;
	int returnlist;
}stmt_t;

void make_stmt (stmt_t* s){
	 s->breakList = s->contList = 0; 
}

int newlist (int i){ 
	quads[i].label = 0; return i; 
}

int mergelist (int l1, int l2) {
	if (!l1)
		return l2;
	else if (!l2)
		return l1;
	else {
		int i = l1;
		while (quads[i].label)
			i = quads[i].label;
		quads[i].label = l2;
		return l1;
	}
}

void patchlist (int list, int label) {
	while (list) {
		int next = quads[list].label;
		quads[list].label = label;
		list = next;
	}
}
