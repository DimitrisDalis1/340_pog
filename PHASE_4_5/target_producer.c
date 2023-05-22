#include "target_producer.h"

double* numConsts=(double*)0;
unsigned totalNumConsts=0;
char** stringConsts=(char**)0;
unsigned totalStringConsts=0;
char** namedLibfuncs=(char**)0;
unsigned totalNamedFuncs=0;
userfunc* userFuncs=(userfunc*)0;
unsigned totalUserFuncs=0;

unsigned int currInstruction = 0;
unsigned int totalVmargs = 0;

instruction* vmargs=(instruction*) 0;
unsigned int currInstruction = 0;
unsigned int totalVmargs = 0;

unsigned consts_newstring(char* s){
    unsigned indexx;
    for(unsigned i=0; i<totalStringConsts; ++i) {
        if(strcmp(stringConsts[i], s)==0)
            return i;
    }
    if (totalStringConsts==0)
        stringConsts = malloc(sizeof(char*));
    else 
        stringConsts = realloc(stringConsts, sizeof(char*)*(totalStringConsts+1));

    string_consts[totalStringConsts] = strdup(s);
    indexx=totalStringConsts++;
    return indexx;
}

unsigned consts_newnumber(double n){
    unsigned indexx;
    for(unsigned i=0;i<totalNumConsts;i++){
        if(numConsts[i]==n) return i;
    }
    if(totalNumConsts==0){
        numConsts=malloc(sizeof(double));
    }else numConsts= realloc(numConsts,sizeof(double*(totalNumConsts+1)));
    
    numConsts[totalNumConsts]=n;
    indexx=totalNumConsts++;
    return indexx;
}

unsigned libfuncs_newused(char* s){
    unsigned indexx;
    for(unsigned i=0; i<totalNamedFuncs; ++i) {
        if(strcmp(namedLibfuncs[i], s)==0)
            return i;
    }
    if (totalNamedFuncs==0)
        namedLibfuncs = malloc(sizeof(char*));
    else 
        namedLibfuncs = realloc(namedLibfuncs, sizeof(char*)*(totalNamedFuncs+1));

    namedLibfuncs[totalNamedFuncs] = strdup(s);
    indexx=totalNamedFuncs++;
    return indexx;
}

unsigned userfuncs_newfunc(SymbolTableEntry* sym){
    unsigned indexx;
    for(unsigned i=0; i<totalUserFuncs;i++){ 
        if(userFuncs[i].address == sym->value.funcVal->iaddress)
            return i;
    } 
    if (totalUserFuncs==0)
        userFuncs = malloc(sizeof(userfunc));
    else 
        userFuncs = realloc(userFuncs, sizeof(userfunc)*(totalUserFuncs+1));
    
    userFuncs[totalUserFuncs].address = sym->value->iaddress; //na to valw sto funcprefix ston parser $$->sym->address= ..;
    userFuncs[totalUserFuncs].localSize = sym->value.funcVal->totalLocals; 
    userFuncs[totalUserFuncs].id = sym->value.funcVal->name;  
    indexx=totalUserFuncs++;
    return indexx;
}


void expand_v(){
	assert(totalVmargs==currInstruction);
	instruction* t = (instruction *) malloc(NEW_SIZE_V);
	if (vmargs){
		memcpy(t,vmargs, CURR_SIZE_V);
		free(vmargs);
	}
	vmargs = t;
	totalVmargs += EXPAND_SIZE;

}

void emit_v(instruction* t){
	assert(t);
	if(totalVmargs==currInstruction)
		expand_v();
    instruction * v=vmargs+(currInstruction++);
    v->opcode=t->opcode;
    v->result=t->result;
    v->arg1=t->arg1;
    v->arg2=t->arg2;
    v->srcLine=t->srcLine;
}

void make_operand(expr* e, vmarg* arg){
    switch(e->type){
        case var_e:
        case tableitem_e:
        case arithexpr_e:
        case boolexpr_e:

        case newtable_e: {
            assert(e->sym);
            arg->val = e->sym->offset;

            switch(e->sym->space){
                case programvar:    arg->type = global_a;   break;
                case functionlocal: arg->type = local_a;    break;
                case formalarg:     arg->type = formal_a;   break;
                default: assert(0);
            }
            break;
        }

        case constbool_e: {
            arg->val = e->boolConst;
            arg->type = bool_a;
            break;
        }

        case conststring_e: {
            arg->val = consts_newstring(e->strConst);
            arg->type = string_a; 
            break;
        }

        case constint_e: {
            arg->val = consts_newnumber(e->intConst);
            arg->type = number_a;
            break;
        }

        case constdouble_e: {
            arg->val = consts_newnumber(e->numConst);
            arg->type = number_a;
            break;
        }

        case nil_e: arg->type = nil_a;  break;

        case programfunc_e: {
            arg->type = userfunc_a;
            arg->val = userfuncs_newfunc(e->sym); 
            break;
        }

        case libraryfunc_e: {
            arg->type = libfunc_a;
            arg->val = libfuncs_newused(e->sym->value.funcVal->name);
            break;
        }

        default: {
            assert(0);
        }
    }
}


void make_numberoperand(vmarg* arg, double val){
    arg->val = consts_newnumber(val);
    arg->type = number_a;
}

void make_booloperand(vmarg* arg, unsigned val){
    arg->val = val;
    arg->type = bool_a;
}

void make_retvaloperand(vmarg* arg){
    arg->type = retval_a;
}


void generateF (void) {
    for(unsigned i = 0; i < total; ++i){
        (*generators[quads[i].op]) (quads+1);
    }
}

incomplete_jump *head_incomplete_jump = NULL;

incomplete_jump* ij_head = (incomplete_jump*) 0;
unsigned ij_total = 0;

void add_incomplete_jump(unsigned instrNo, unsigned iaddress){

}

void patch_incomplete_jumps() {
    while(head_incomplete_jump != NULL){
        if (head_incomplete_jump->iaddress == intermediate code size)
            instructions[head_incomplete_jump->instrNo].result = target code size;
        else
        instructions[head_incomplete_jump->instrNo].result = quads[head_incomplete_jump->iaddress].taddress;
    }
}

void generate (vmopcode op, quad* q) {
    instruction* t=malloc(sizeof(instruction));
    t->opcode = op;
    t->srcLine=q->line;
    if(q->arg1!=NULL) {
        t->arg1 = malloc(sizeof(vmarg));
        make_operand(q->arg1, t->arg1);
    }
    if(q->arg2!=NULL) {
        t->arg2 = malloc(sizeof(vmarg));
        make_operand(q->arg2, t->arg2);
    }
    if(q->result!=NULL) {
        t->result = malloc(sizeof(vmarg));
        make_operand(q->result, t->result);
    }
    q->taddress = nextinstructionlabel();
    emit_v(t);
}
void generate_ADD (quad* q) { generate(add_v, q); }
void generate_SUB (quad* q) { generate(sub_v, q); }
void generate_MUL (quad* q) { generate(mul_v, q); }
void generate_DIV (quad* q) { generate(div_v, q); }
void generate_MOD (quad* q) { generate(mod_v, q); }

void generate_NEWTABLE (quad* q) { generate(newtable_v, q); }
void generate_TABLEGETELM (quad* q) { generate(tablegetelem_v, q); }
void generate_TABLESETELEM (quad* q) { generate(tablesetelem_v, q); }
void generate_ASSIGN (quad* q) { generate(assign_v, q); }
void generate_NOP () { 
    instruction *t=malloc(sizeof(instruction)); 
    t->opcode=nop_v; 
    emit_v(t);

}

void generate_relational (vmopcode op,quad* q) {
    instruction *t=malloc(sizeof(instruction)); 
    t->opcode = op;
    t->srcLine=q->line;
     if(q->arg1!=NULL){
        t->arg1 = malloc(sizeof(vmarg));
        make_operand(q->arg1, t->arg1);
    }
    if(q->arg2!=NULL){
        t->arg2 = malloc(sizeof(vmarg));
        make_operand(q->arg2, t->arg2);
    }
    t->result = malloc(sizeof(vmarg));
    t->result->type=label_a;
    if (q->label < currprocessedquad(q))
        t->result->value= quads[q->label].taddress;
    else
        add_incomplete_jump(nextinstructionlabel(), q->label);
   
    q->taddress = nextinstructionlabel();
    emit_v(t);
}

void generate_JUMP (quad* q) { generate_relational(jump_v, q); }
void generate_IF_EQ (quad* q) { generate_relational(jeq_v, q); }
void generate_IF_NOTEQ(quad* q) { generate_relational(jne_v, q); }
void generate_IF_GREATER (quad* q) { generate_relational(jgt_v, q); }
void generate_IF_GREATEREQ(quad* q) { generate_relational(jge_v, q); }
void generate_IF_LESS (quad* q) { generate_relational(jlt_v, q); }
void generate_IF_LESSEQ (quad* q) { generate_relational(jle_v, q); }

void generate_NOT (quad* q) {
    quad.taddress = nextinstructionlabel();
    instruction t;
    t.opcode = jeq;
    make_operand(q->arg1, &t.arg1);
    make_booloperand(&t.arg2, false);
    t.result.type = label_a;
    t.result.value = nextinstructionlabel()+3;
    emit(t);
    t.opcode = assign;
    make_booloperand(&t.arg1, false);
    reset_operand(&t.arg2);
    make_operand(q->result, &t.result);
    emit(t); 
    t.opcode = jump;
    reset_operand (&t.arg1);
    reset_operand(&t.arg2);
    t.result.type = label_a;
    t.result.value = nextinstructionlabel()+2;
    emit(t);
    t.opcode = assign;
    make_booloperand(&t.arg1, true);
    reset_operand(&t.arg2);
    make_operand(q->result, &t.result);
    emit(t);
}

void generate_OR (quad* q) {
    q->taddress = nextinstructionlabel();
    instruction *t=malloc(sizeof(instruction)); 
    t->opcode = jeq_v;
    t->srcLine=q->line;
    if(q->arg1!=NULL){
        t->arg1 = malloc(sizeof(vmarg));
        make_operand(q->arg1, t->arg1);
    }
    
    make_booloperand(t->arg2, true);
    t->result->type = label_a;
    t->result->value = nextinstructionlabel()+4;
    emit_v(t);

    if(q->arg2!=NULL){
        t->arg1 = malloc(sizeof(vmarg));
        make_operand(q->arg2, t->arg1);
    }
    t->result->value = nextinstructionlabel()+3;
    emit_v(t); 
    t->opcode = assign_v;
    make_booloperand(t->arg1, false);
    reset_operand(t->arg2);
    make_operand(q->result, t->result);
    emit_v(t);
    t->opcode = jump_v;
    reset_operand (t->arg1);
    reset_operand(t->arg2);
    t->result->type = label_a;
    t->result->value = nextinstructionlabel()+2;
    emit_v(t);
    t->opcode = assign;
    make_booloperand(t->arg1, true);
    reset_operand(t->arg2);
    make_operand(q->result, t->result);
    emit_v(t);
} 

void generate_PARAM(quad* q) {
    q->taddress = nextinstructionlabel();
    instruction *t=malloc(sizeof(instruction)); 
    t->opcode = pusharg_v;
    t->srcLine=q->line;
    if(q->arg1!=NULL){
        t->arg1 = malloc(sizeof(vmarg));
        make_operand(q->arg1, t->arg1);
    }
    emit_v(t);
}
void generate_CALL(quad* q) {
    q->taddress = nextinstructionlabel();
    instruction *t=malloc(sizeof(instruction)); 
    t->opcode = call_v;
    t->srcLine=q->line;
    if(q->arg1!=NULL){
        t->arg1 = malloc(sizeof(vmarg));
        make_operand(q->arg1, t->arg1);
    }
    emit_v(t);
}
void generate_GETRETVAL(quad* q) {
    q->taddress = nextinstructionlabel();
    instruction *t=malloc(sizeof(instruction)); 
    t->opcode = assign_v;
    t->srcLine=q->line;
    if(q->result!=NULL){
        t->result = malloc(sizeof(vmarg));
        make_operand(q->result, t->result);
    }
    t->arg1 = malloc(sizeof(vmarg));
    make_retvaloperand(t->arg1);
    emit_v(t);
}
void  generate_FUNCSTART(quad* q){
    SymbolTableEntry* f = q->result->sym;
   // f->address = nextinstructionlabel();
   // userfunctions.add(f->id,f->taddress,f->tatallocals);
   // push(funstack,f);
   
    q->taddress = nextinstructionlabel();
    instruction *t=malloc(sizeof(instruction)); 
    t->opcode = funcenter_v;
    t->srcLine=q->line;
    if(q->result!=NULL){
        t->result = malloc(sizeof(vmarg));
        make_operand(q->result, t->result);
        push_funcstart_label();
    }
    emit_v(t);
}

void generate_RETURN(quad* q){

   // f=top(funstack);
    //append(f.returnList,nextinstructionlabel());

    

    q->taddress = nextinstructionlabel();
    instruction *t=malloc(sizeof(instruction)); 
    t->opcode = jump_v;
    t->srcLine=q->line;
    //reset_operand(t->arg1);
    //reset_operand(t->arg2);
    
    if(q->arg1!=NULL){
        t->arg1 = malloc(sizeof(vmarg));
        make_operand(q->arg1, t->arg1);
    }
    t->result = malloc(sizeof(vmarg));
    make_retvaloperand(t->result);
    t->result->type = label_a;
    emit_v(t);

}

void generate_FUNCEND(quad* q){
    SymbolTableEntry* f = pop(funcstack);
    backpatch(f.returnList,nextinstructionlabel());
    pop_funcstart_label();   
    q->taddress = nextinstructionlabel();
    instruction *t=malloc(sizeof(instruction)); 
    t->opcode = funcexit_v;
    t->srcLine=q->line;
    if(q->result!=NULL){
        t->result = malloc(sizeof(vmarg));
        make_operand(q->result, t->result);
    }
    emit_v(t);
}
