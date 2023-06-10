#include "quadhandler.h"
extern int  yylineno;
quad*	quads = (quad*) 0;
unsigned total = 0;
int tempcounter=0;
extern int currQuad;
unsigned programVarOffset =0; 
unsigned functionLocalOffset=0;
unsigned formalArgOffset=0;
unsigned scopeSpaceCounter=1;
int print_flag=0;
extern int program_offset;
int check_for_valid_loop_stop(int counter){
	if(counter > 0)
		return 1;
	else 
		return 0;
}

void printExpr(expr* argExpr){
	if(argExpr==NULL){
		printf("%-*s \t",10,"\t");
	}else{
		if(argExpr->type == var_e){
				printf("%-*s \t",20,argExpr->sym->value.varVal->name);
			}else if (argExpr->type == tableitem_e){
				printf("%-*s \t",20,argExpr->sym->value.varVal->name);
			}else if (argExpr->type == programfunc_e){
				printf("%-*s \t",20,argExpr->sym->value.funcVal->name);
			}else if (argExpr->type == libraryfunc_e){
				printf("%-*s \t",20,argExpr->sym->value.funcVal->name);
			}else if (argExpr->type == arithexpr_e){
				printf("%-*s \t",20,argExpr->sym->value.varVal->name);
			}else if (argExpr->type == boolexpr_e){
				printf("%-*s \t",20,argExpr->sym->value.varVal->name);
			}else if (argExpr->type == assignexpr_e){
				printf("%-*s \t",20,argExpr->sym->value.varVal->name);
			}else if (argExpr->type == newtable_e){
				printf("%-*s \t",20,argExpr->sym->value.varVal->name);
			}else if (argExpr->type == constint_e){
				printf("%-*d \t",20,argExpr->intConst);
			}else if (argExpr->type == constdouble_e){
				printf("%-*f \t",20,argExpr->numConst);
			}else if (argExpr->type == constbool_e){
				char* tmp=malloc(10);
				argExpr->boolConst=='t'? strcat(tmp,"true\0"):strcat(tmp,"false\0");
				printf("%-*s \t",20,tmp);
			}else if (argExpr->type == conststring_e && print_flag==0){
								printf("\"%-*s \t",20,argExpr->strConst);
				print_flag=0;
			}else if (argExpr->type == conststring_e && print_flag==1){
				if(strpbrk(argExpr->strConst,"\"")==NULL){
				printf("%-*s \t",20,argExpr->strConst);}
				else{ printf("\"%-*s \t",20,argExpr->strConst);}

				print_flag=0;
			}else if (argExpr->type == nil_e){
				printf("%-*s \t",20,"NIL");
			}
	}


}
void printMedianCode(){
    char opcode_array[27][25]={
		"assign\0","add\0","sub\0",
		"mul\0","div\0","mod\0",
		"uminus\0","and\0","or\0",
		"not\0","if_eq\0","if_noteq\0","if_greater\0",
		"if_lesseq\0", "if_less\0",
		"if_greatereq\0","call\0","param\0",
		"ret\0","getretval\0","funcstart\0",
		"funcend\0","tablecreate\0","tablegetelem\0",    
		"tablesetelem\0","jump\0","return\0"};

    int line_for_print = 1;
        printf("\n----------------------------------------------------------------------------------------------------------------------------------------------------------\n");    

    printf("\t\t\t\t\t\t\t\t\tQUADS\t\t\t\t\t\t\t\t\n");
    printf("----------------------------------------------------------------------------------------------------------------------------------------------------------\n");    

    printf("quad#\t\t\topcdode\t\t\tresult\t\t\targ1\t\t\targ2\t\t\tlabel\t\t\tline\n");
    printf("----------------------------------------------------------------------------------------------------------------------------------------------------------\n");    
    //quads[i].label = 0;
    
    for(int i = 1; i < currQuad; i++){
        printf("%-*d \t",20,line_for_print);
        printf("%-*s \t", 20, opcode_array[quads[i].op]);
		 if(quads[i].op== assign){
			printExpr(quads[i].result);
			printExpr(quads[i].arg1);
			printf("%-*s \t",15,"\t");
        }else if(quads[i].op == add || quads[i].op== sub || quads[i].op == mul || quads[i].op == divi || quads[i].op== mod){
            		printExpr(quads[i].result);
			printExpr(quads[i].arg1);
			printExpr(quads[i].arg2);
	    }else if(quads[i].op == uminus || quads[i].op == not){
            		printExpr(quads[i].result);
			printExpr(quads[i].arg1);
			printf("%-*s \t",15,"\t");
	    }else if(quads[i].op == and ||quads[i].op== or ){
		    	printExpr(quads[i].result);
			printExpr(quads[i].arg1);
			printExpr(quads[i].arg2);
	    }else if(quads[i].op== if_eq || quads[i].op== if_noteq || quads[i].op == if_lesseq || quads[i].op== if_greatereq || quads[i].op == if_less || quads[i].op == if_greater){
            		printExpr(quads[i].result);
			printExpr(quads[i].arg1);
			printExpr(quads[i].arg2);
        }else if(quads[i].op == call || quads[i].op == returnn || quads[i].op == jump || quads[i].op == tablecreate || quads[i].op == funcend || quads[i].op == getretval || quads[i].op== param ){
		    	printExpr(quads[i].result);
			printf("%-*s \t",20,"\t");
			printf("%-*s \t",5,"\t");
	    }else if(quads[i].op == funcstart){
			print_flag=1;	
            		printExpr(quads[i].result);
			printf("%-*s \t",20,"\t");
			printf("%-*s \t",5,"\t");
      
        }else if(quads[i].op == tablegetelem){
			
			print_flag=1;
			
            		printExpr(quads[i].result);
			printExpr(quads[i].arg1);
			printExpr(quads[i].arg2);       
        }else if(quads[i].op == tablesetelem){
			print_flag=1;
           		printExpr(quads[i].result);
			printExpr(quads[i].arg1);
			printExpr(quads[i].arg2);  
	    }
		
		(quads[i].label==-1  || quads[i].op == funcstart || quads[i].op == funcend)?printf("%-*s \t",20,""):printf("%-*d \t",20,quads[i].label);
		printf("%-*d \t",20,quads[i].line);
        line_for_print++;
        printf("\n");
    }
    return;
}

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

	quad* p= quads+(currQuad++);
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
			program_offset++;
			sym= SymTable_insert(hash,name,yylineno,NULL,current_scope,GLOBAL);
			sym->space=programvar;
			return sym;


		}else{
			sym=SymTable_insert(hash,name,yylineno,NULL,current_scope,LOCALV);
			sym->space=functionlocal;
			return sym;

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
			result,
			e,
			e->index,
			-1,
			yylineno
		);
		return result;
	}
}

expr* make_call(expr* lv, expr* reversed_elist){
	expr* func=emit_iftableitem(lv);
	
	if(reversed_elist!=NULL){
	expr* prev=reversed_elist;
    	expr* cur= reversed_elist->next;
	reversed_elist=reversed_elist->next;
	prev->next=NULL;
 
	while(reversed_elist!=NULL){
		reversed_elist=reversed_elist->next;
		cur->next=prev;
		prev=cur;
		cur=reversed_elist;	
	}
	reversed_elist=prev;
	}
	else reversed_elist=NULL;
	while(reversed_elist){
		emit(param,reversed_elist,NULL,NULL, -1, currQuad); //2 teleftaia oti na nai
		reversed_elist=reversed_elist->next;
	}
	emit(call,func,NULL,NULL, -1, currQuad); //2 teleftaia argument apla gia test
	expr* result=newexpr(var_e);
	result->sym=newtemp();
	emit(getretval,result,NULL,NULL, -1, currQuad);

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
	printf("Illegal expr used");}

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
	 s->breaklist= 0;
	s->returnlist = 0;
	s->contlist = 0;  
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

expr* result_finder(expr* a1, expr* a2){
	SymbolTableEntry* result;
	expr* temp;
	if(a1->sym && a1->sym->type<2 && istempname(a1->sym->value.varVal->name))
	{
		result = a1->sym;
	}else if(a2->sym && a2->sym->type<2 && istempname(a2->sym->value.varVal->name))
	{
		result = a2->sym;
	}else{
		result = newtemp();
	}
	temp = lvalue_expr(result);
	return temp;
}

expr* emitBoolean(expr* ex){
	
    if(ex->type == boolexpr_e ){
        patchlist(ex->truelist, nextquad());
       

        expr* tmp = newexpr(boolexpr_e);
        tmp->sym = newtemp();
        
        emit(assign, tmp, newexpr_constbool('t'), NULL, -1, yylineno);
        emit(jump, NULL, NULL, NULL, nextquad() + 2 , yylineno);
	patchlist(ex->falselist, nextquad());
        emit(assign, tmp, newexpr_constbool('f'), NULL, -1, yylineno);
        
        return tmp;
    }else  return ex;

}
