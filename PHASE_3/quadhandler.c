#include "quadhandler.h"

quad*	quads = (quad*) 0;
unsigned total = 0;
int tempcounter=0;
extern int currQuad;
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
///////////////////////////////////////////////////////////////////////////////

expr* emit_iftableitem(expr* e){
	if(e->type !=tableitem_e){
		return e;
	}else{
		expr* result = newexpr(var_e);
		result->sym=newtemp();
		emit(
			tablegetelem,
			e,
			e->index,
			result,
			-1,
			currQuad
		);
		return result;
	}
}



expr* make_call(expr* lv, expr* reversed_elist){
	expr* func=emit_iftableitem(lv);
	while(reversed_elist){
		emit(param,reversed_elist,NULL,NULL, -1, currQuad); //2 teleftaia oti na nai
		reversed_elist=reversed_elist->next;
	}
	emit(call,func,NULL,NULL, -1, currQuad); //2 teleftaia argument apla gia test
	expr* result=newexpr(var_e);
	result->sym=newtemp();
	emit(getretval,NULL,NULL,result, -1, currQuad);

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
	printf("Illegal expr used in %s!", context); ////////////make new function for compiletime errorrs
}

unsigned int istempname(char* s){
	return *s=='_';
}

bool istempexpr(expr* e){
	if(e->sym == NULL){
		return 0;
	}
	if (e->sym->type == GLOBAL|| e->sym->type == LOCALV || e->sym->type == FORMAL )
	{
		return (( e->sym) && istempname(e->sym->value.varVal->name)); ///////get name if variable or function
	}else if(e->sym->type == USERFUNC || e->sym->type == LIBFUNC){
		return (( e->sym) && istempname(e->sym->value.funcVal->name)); ///////get name if variable or function
	}
	return 0;
}

unsigned nextquad (void) { return currQuad; }



void make_stmt (stmt_t* s){
	 s->breaklist = s->contlist = 0; 
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
