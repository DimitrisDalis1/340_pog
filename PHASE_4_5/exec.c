#include "avm.h"

execute_func_t executeFuncs[] = {
    execute_assign,         //0
    execute_add,            //1
    execute_sub,            //2
    execute_mul,            //3
    execute_div,            //4
    execute_mod,            //5
    execute_uminus,         //6
    execute_and,            //7
    execute_or,             //8
    execute_not,            //9
    execute_jeq,            //10
    execute_jne,            //11
    execute_jle,            //12
    execute_jge,            //13
    execute_jlt,            //14
    execute_jgt,            //15
    execute_call,           //16
    execute_pusharg,        //17
    execute_return,         //18
    execute_getretval,      //19
    execute_funcenter,      //20
    execute_funcexit,       //21
    execute_newtable,       //22
    execute_tablegetelem,   //23
    execute_tablesetelem,   //24
    execute_jump,           //25
    execute_nop             //26
};

void execute_cycle(void){   
    if(executionFinished){
        return;
    }
    else if(pc == AVM_ENDING_PC){
        executionFinished = 1;
        return;
    }
    else{
        assert(pc < AVM_ENDING_PC);
        instruction* instr = &instrs[pc];
        assert(
            instr->opcode >= 0 &&
            instr->opcode <= AVM_MAX_INSTRUCTIONS
        );
        if(instr->srcLine){
            currLine = instr->srcLine;
        }
        unsigned oldPC = pc;
        STACK_CHECK //checks for stack over flow or under flow
        executeFuncs[instr->opcode](instr);
	
        if(pc == oldPC){
            ++pc;
        }

    }
}



// never used
void execute_nop (instruction* instr) { assert(0); return; }
void execute_getretval (instruction* instr) { assert(0); return; }
void execute_return (instruction* instr) { assert(0); return; }


void execute_call(instruction* instr){
    avm_memcell* func = avm_translate_operand(instr->result,&ax);
    assert(func);
    avm_callsaveenvironment(); 
    switch (func->type)
    {
    case userfunc_m :{
        pc = userfs[func->data.funcVal].address;
        assert(pc < AVM_ENDING_PC);

        assert(instrs[pc].opcode == funcenter_v);
        break;
    }
        /* code */
        case string_m: {avm_calllibfunc(func->data.strVal); break;}
        case libfunc_m: { avm_calllibfunc(func->data.libfuncVal); break;}
        case table_m:{ /*avm_calllibfunc(func->data.tableVal);*/ break;} //implement it ,we gettableelement and settableelement
    default:{
        char* s = avm_tostring(func);
        fprintf(stderr,"call : cannot bind '%s' to function!",s );
        free(s);
        executionFinished = 1;
        }
    }
}

void execute_funcenter (instruction* instr){
    avm_memcell* func = avm_translate_operand(instr->result, &ax);
    assert(func);
    assert(pc == userfs[func->data.funcVal].address); /* Func address should match PC. */
    /* Callee actions here. */
    totalActuals = 0;
    userfunc* funcInfo = userfuncs_getfunc(pc);
    topsp = top;
    top = top - funcInfo -> localSize; 
}

void execute_funcexit(instruction* unused){
    unsigned oldTop = top;
    top = avm_get_envvalue(topsp + AVM_SAVEDTOP_OFFSET);
    pc = avm_get_envvalue(topsp + AVM_SAVEDPC_OFFSET);
    topsp = avm_get_envvalue(topsp + AVM_SAVEDTOPSP_OFFSET);

    while(++oldTop <= top)
      avm_memcellclear(&avm_stack[oldTop]);
}

void execute_pusharg(instruction* instr){
	
	avm_memcell* arg = avm_translate_operand(instr->result,&ax);
	
    assert(arg);
    /*this is actually stack[top] = arg,but we have to use
        avm_assign.*/
	
    avm_assign(&avm_stack[top],arg);
    ++totalActuals;
    avm_dec_top();
	
}

double add_impl (double x, double y) { return x+y; }
double sub_impl (double x, double y) { return x-y; }
double mul_impl (double x, double y) { return x*y; }
double div_impl (double x, double y) { assert(y); return x/y; }     // Maybe needs more errorchecking
double mod_impl (double x, double y) { //Maybe needs more errorchecking
    assert(y);
    //assert(x > 0);
    //assert(y > 0);
    return ((unsigned) x) % ((unsigned) y);
}


/* Dispatcher for arithmetic functions. */
arithmetic_func_t arithmeticFuncs[] = {
    add_impl,
    sub_impl,
    mul_impl,
    div_impl,
    mod_impl
};

void execute_arithmetic (instruction* instr) {
    avm_memcell* lv =  avm_translate_operand(instr->result, (avm_memcell*) 0);
    avm_memcell* rv1 = avm_translate_operand(instr->arg1, &ax);
    avm_memcell* rv2 = avm_translate_operand(instr->arg2, &bx);

    //assert(lv && (&stack[N-1] >= lv && lv > &stack[top] || lv==&retval));
    assert(rv1 && rv2);

    if(rv1->type != number_m || rv2->type != number_m) {
        avm_error("not a number in arithmetic", &instrs[pc]);
        executionFinished = 1;
    }
    else {
        arithmetic_func_t op = arithmeticFuncs[instr->opcode - add_v];
        avm_memcellclear(lv);
        lv->type            = number_m;
        lv->data.numVal     = (*op)(rv1->data.numVal, rv2->data.numVal);
	    }
}


void execute_add(instruction* instr) { execute_arithmetic(instr); return; }
void execute_sub(instruction* instr) { execute_arithmetic(instr); return; }
void execute_mul(instruction* instr) { execute_arithmetic(instr); return; }
void execute_div(instruction* instr) { execute_arithmetic(instr); return; }
void execute_mod(instruction* instr) { execute_arithmetic(instr); return; }
void execute_uminus(instruction* instr) {assert(0); return;} //useless

void execute_newtable(instruction* instr){
  avm_memcell * lv = avm_translate_operand(instr->result, (avm_memcell*) 0);
  //assert(lv && (&avm_stack[N-1] >= lv && lv > &avm_stack[top]));
  avm_memcellclear(lv);
  lv->type = table_m;
  lv->data.tableVal = avm_tablenew();
  avm_tableincrefcounter(lv->data.tableVal);

}

void execute_tablegetelem(instruction * instr){
	avm_memcell *lv = avm_translate_operand(instr->result, (avm_memcell*) 0);
 	avm_memcell *t = avm_translate_operand(instr->arg1, (avm_memcell*) 0); //seg
  	avm_memcell *i = avm_translate_operand(instr->arg2, &ax);

  //assert(lv && &avm_stack[N-1] >= lv && lv > &avm_stack[top] || lv == &retval);
  //assert(t && &avm_stack[N-1] >= t && t > &avm_stack[top]);
  assert(i);

  avm_memcellclear(lv);
  lv->type = nil_m; //default value
  if (t->type != table_m)
  {
    char tmp[1024];
    sprintf(tmp, "Illegal use of type %s as table!", typeStrings[t->type]);
    avm_error(tmp, &instrs[pc]);
  }else{
    avm_memcell * content = avm_tablegetelem(t->data.tableVal, i);
    if(content){
      avm_assign(lv, content);
    }else{
      if(disable_remove_warning)
        disable_remove_warning--;
      else{
        char* ts = avm_tostring(t);
        char* is = avm_tostring(i);
        char tmp[1024]; //dika tous
        sprintf(tmp, "%s[%s] not found!", ts, is);  //dika tous
        avm_warning(tmp, &instrs[pc]); //dika tous
        free(ts);
        free(is);
      }
    }
  }
}

void execute_tablesetelem(instruction* instr) {
    avm_memcell* t = avm_translate_operand(instr->result, (avm_memcell*) 0);
    avm_memcell* i = avm_translate_operand(instr->arg1, &ax);
    avm_memcell* c = avm_translate_operand(instr->arg2, &bx);

    // assert(t && &stack[N-1] >= t && t > stack[top]);
    assert(i && c);

    if(t->type != table_m){
        char tmp[1024];
        sprintf(tmp, "illegal use of type %s as a table!", typeStrings[t->type]);
        avm_error(tmp, &instrs[pc]);
    }
    else
        avm_tablesetelem(t->data.tableVal, i, c);

}

extern void execute_assign (instruction* instr){
    avm_memcell* lv = avm_translate_operand(instr->result,(avm_memcell*)0);
    avm_memcell* rv = avm_translate_operand(instr->arg1,&ax);

    //assert(lv && (&avm_stack[AVM_STACKSIZE-1] >= lv && lv > &avm_stack[top] || lv==&retval));
    assert(rv);
    avm_assign(lv,rv);

}

/*never used*/
void execute_and (instruction* instr) { assert(0); return; }
void execute_or  (instruction* instr) { assert(0); return; }
void execute_not (instruction* instr) { assert(0); return; }

void execute_jeq (instruction* instr) {
    assert(instr->result->type == label_a);

    avm_memcell* rv1 = avm_translate_operand(instr->arg1, &ax);
    avm_memcell* rv2 = avm_translate_operand(instr->arg2, &bx);

    unsigned int result = 0;

    if (rv1->type == undef_m || rv2->type == undef_m)
        avm_error("\'undef\' involved in equality", &instrs[pc]);
    else
    if (rv1->type == nil_m || rv2->type == nil_m)
        result = ((rv1->type == nil_m && rv2->type == nil_m)?1:0);
    else
    if (rv1->type == bool_m || rv2->type == bool_m){
        result = ((avm_tobool(rv1) == avm_tobool(rv2))?1:0);
	
	}
    else
    if(rv1->type != rv2->type){
        char tmp[1024];
        sprintf(tmp, "%s == %s is illegal!", typeStrings[rv1->type], typeStrings[rv2->type]);
        avm_error(tmp, &instrs[pc]);
    }
    else if(rv1->type == string_m && rv2->type == string_m){
        result = (!strcmp(rv1->data.strVal, rv2->data.strVal))?1:0;
    }
    else {
	
        result = ((avm_toarithm(rv1) == avm_toarithm(rv2))?1:0);
        //equality check with dispatching 
    }
    if(!executionFinished && result)
        pc = instr->result->val-1; 
}

void execute_jge (instruction* instr) {
    assert(instr->result->type == label_a);
    
    avm_memcell* rv1 = avm_translate_operand(instr->arg1, &ax);
    avm_memcell* rv2 = avm_translate_operand(instr->arg2, &bx);

    unsigned int  result = 0;

    if (rv1->type == undef_m || rv2->type == undef_m)
        avm_error("\'undef\' involved in jge", &code[pc]);
    else
    if (rv1->type == nil_m || rv2->type == nil_m)
        result = ((rv1->type == nil_m && rv2->type == nil_m)?1:0);
    else
    if (rv1->type == bool_m || rv2->type == bool_m)
        result =((avm_tobool(rv1) >= avm_tobool(rv2))?1:0);
    else
    if(rv1->type != rv2->type){
        char tmp[1024];
        sprintf(tmp, "%s >= %s is illegal!", typeStrings[rv1->type], typeStrings[rv2->type]);
        avm_error(tmp, &instrs[pc]);
    }
    else {
        result = ((avm_toarithm(rv1) >= avm_toarithm(rv2))?1:0);
    }

    if(!executionFinished && result)
        pc = instr->result->val-1;
}

void execute_jgt (instruction* instr) {
    assert(instr->result->type == label_a);
    
    avm_memcell* rv1 = avm_translate_operand(instr->arg1, &ax);
    avm_memcell* rv2 = avm_translate_operand(instr->arg2, &bx);

    unsigned int result = 0;

    if (rv1->type == undef_m || rv2->type == undef_m)
        avm_error("\'undef\' involved in jgt", &instrs[pc]);
    else
    if (rv1->type == nil_m || rv2->type == nil_m)
        result = ((rv1->type == nil_m && rv2->type == nil_m)?1:0);
    else
    if (rv1->type == bool_m || rv2->type == bool_m)
        result =  ((avm_tobool(rv1)> avm_tobool(rv2))?1:0);
    else
    if(rv1->type != rv2->type){
        char tmp[1024];
        sprintf(tmp, "%s > %s is illegal!", typeStrings[rv1->type], typeStrings[rv2->type]);
        avm_error(tmp, &instrs[pc]);
    }
    else {
        result = ((avm_toarithm(rv1) >avm_toarithm(rv2))?1:0);
    }

    if(!executionFinished && result)
        pc = instr->result->val-1;

}

void execute_jle (instruction* instr) {
    assert(instr->result->type == label_a);
    
    avm_memcell* rv1 = avm_translate_operand(instr->arg1, &ax);
    avm_memcell* rv2 = avm_translate_operand(instr->arg2, &bx);

    unsigned int result = 0;

    if (rv1->type == undef_m || rv2->type == undef_m)
        avm_error("\'undef\' involved in jle", &instrs[pc]);
    else
    if (rv1->type == nil_m || rv2->type == nil_m)
        result = ((rv1->type == nil_m && rv2->type == nil_m)?1:0);
    else
    if (rv1->type == bool_m || rv2->type == bool_m)
        result =((avm_tobool(rv1) <= avm_tobool(rv2))?1:0);
    else
    if(rv1->type != rv2->type){
        char tmp[1024];
        sprintf(tmp, "%s <= %s is illegal!", typeStrings[rv1->type], typeStrings[rv2->type]);
        avm_error(tmp, &instrs[pc]);
    }
    else {
        result = ((avm_toarithm(rv1) <= avm_toarithm(rv2))?1:0);
    }

    if(!executionFinished && result)
        pc = instr->result->val-1;
}

void execute_jlt (instruction* instr) {
    assert(instr->result->type == label_a);
    
    avm_memcell* rv1 = avm_translate_operand(instr->arg1, &ax);
    avm_memcell* rv2 = avm_translate_operand(instr->arg2, &bx);

    unsigned int result = 0;

    if (rv1->type == undef_m || rv2->type == undef_m)
        avm_error("\'undef\' involved in jlt", &instrs[pc]);
    else
    if (rv1->type == nil_m || rv2->type == nil_m)
        result =((rv1->type == nil_m && rv2->type == nil_m)?1:0);
    else
    if (rv1->type == bool_m || rv2->type == bool_m)
        result = ((avm_tobool(rv1)< avm_tobool(rv2))?1:0);
    else
    if(rv1->type != rv2->type){
        char tmp[1024];
        sprintf(tmp, "%s < %s is illegal!", typeStrings[rv1->type], typeStrings[rv2->type]);
        avm_error(tmp, &instrs[pc]);
    }
    else {
        result = ((avm_toarithm(rv1) < avm_toarithm(rv2))?1:0); 
    }

    if(!executionFinished && result)
        pc = instr->result->val-1;
}

void execute_jne (instruction* instr) {
    assert(instr->result->type == label_a);
    
    avm_memcell* rv1 = avm_translate_operand(instr->arg1, &ax);
    avm_memcell* rv2 = avm_translate_operand(instr->arg2, &bx);

    unsigned char result = 0;

    if (rv1->type == undef_m || rv2->type == undef_m)
        avm_error("\'undef\' involved in inequality",&instrs[pc]);
    else
    if (rv1->type == nil_m || rv2->type == nil_m)
        result = ((rv1->type == nil_m && rv2->type == nil_m)?1:0);
    else
    if (rv1->type == bool_m || rv2->type == bool_m){
        result = ((avm_tobool(rv1) !=avm_tobool(rv2))?1:0); 
	}
    else
    if(rv1->type != rv2->type){
        char tmp[1024];
        sprintf(tmp, "%s != %s is illegal!", typeStrings[rv1->type], typeStrings[rv2->type]);
        avm_error(tmp,&instrs[pc]);
    }
    else {
        result = ((avm_toarithm(rv1) != avm_toarithm(rv2))?1:0); 
    }

    if(!executionFinished && result)
        pc = instr->result->val-1;

}


void execute_jump (instruction* instr) {
    assert(instr->result->type == label_a);

    if(!executionFinished)
        pc = instr->result->val-1;
}
