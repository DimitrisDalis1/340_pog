#define EXPAND_SIZE 1024
#define CURR_SIZE 	(total*sizeof(quad))
#define NEW_SIZE	(EXPAND_SIZE*sizeof(quad)+CURR_SIZE)

typedef enum iopcode{
	assign,	add,	sub,	
	mul,	div,	mod,
	uminus,	and,	or,
	not,	if_eq,	if_noteq,
	if_greater,	call,	param,
	ret,	getretvsl,	funcstart,
	funcend,	tablecreate,	tablegetelem,	
	tablesetelem
}iopcode;

enum scopespace_{
	programvar,
	functionlocal,
	formalarg
};

enum sumbol_t{
	var_s,
	programfunc_s,
	libraryfunc_s	
};

struct expr;

struct symbol{
	symbol_t type,
	char* name,
scopespace_t space,
unsigned offset,
unsigned scope,
unsigned line
};

unsigned programVarOffset =0; 
unsigned functionLocalOffset=0;
unsigned formal ArgOffset=0;
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

distributetype(vars, type) {
for each v in vars do {
if v.class = pointervar then
v.size = 4;
else if v.class = arrayvar then
v.size = arraytotal(v.dims)*type.size;
else if v.class = arraypointervar then
v.size = arraytotal(v.dims)*4;
else
v.size = type.size;
v.offset =getcurroffset();
inccurroffset(v.size);
}
}


typedef struct quad{
	iopcode op;
	expr* result;
	expr* arg1;
	expr* arg2;
	unsigned label;
	unsigned line;
}quad;

quad*	quads = (quad*) 0;
unsigned total = 0;
unsigned int currQuad = 0;
int tempcounter=0;
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


