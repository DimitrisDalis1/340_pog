#include"avm.h"

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
  avm_memcell *t = avm_translate_operand(instr->arg1, (avm_memcell*) 0);
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
    avm_error(tmp, &code[pc]);
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
        avm_warning(tmp, &code[pc]); //dika tous
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
        avm_error(tmp, &code[pc]);
    }
    else
        avm_tablesetelem(t->data.tableVal, i, c);

}


void avm_tableincrefcounter (avm_table* t){
    ++t->refCounter;
}

void avm_tabledecrefcounter (avm_table* t){
    assert(t->refCounter > 0);
    if(!--t->refCounter)
        avm_tabledestroy(t);
}

void avm_tablebucketsinit (avm_table_bucket ** p){
    for(unsigned i = 0; i < AVM_TABLE_HASHSIZE; ++i)
        p[i] = (avm_table_bucket*) 0;
}

avm_table* avm_tablenew(void){
    avm_table* t = (avm_table*) malloc (sizeof(avm_table));
    AVM_WIPEOUT(*t);

    t->refCounter = t->total = 0;
    avm_tablebucketsinit(t->numIndexed);
    avm_tablebucketsinit(t->strIndexed);

    return t;
}

void avm_tablebucketsdestroy(avm_table_bucket** p){
    for(unsigned i =0; i < AVM_TABLE_HASHSIZE; ++i, ++p){
        for(avm_table_bucket* b = *p; b;){
            avm_table_bucket* del = b;
            b = b->next;
            avm_memcellclear(&del->key);
            avm_memcellclear(&del->value);
            free(del);
        }
        p[i] = (avm_table_bucket*) 0;
    }
}

void avm_tabledestroy (avm_table* t){
    avm_tablebucketsdestroy(t->strIndexed);
    avm_tablebucketsdestroy(t->numIndexed);
    free(t);
}