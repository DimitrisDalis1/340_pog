#include"avm.h"

void avm_assign(avm_memcell* lv, avm_memcell* rv){
    if(lv == rv) /*same cells? destructive to assign!*/
        return;

    if(lv->type == table_m && rv->type == table_m && lv->data.tableVal == rv->data.tableVal)
    return;

    if(rv->type == undef_m) /*from undefined r-value? warn!*/
        avm_warning("assigning from 'undef' content! ",&code[pc]);

    avm_memcellclear(lv); /*clear old cell contents. */
    memcpy(lv,rv,sizeof(avm_memcell)); /* in c++ dispatch instead*/
    /*now take care of copied values or reference counting*/
    if(lv->type == string_m)
        lv->data.strVal = strdup(rv->data.strVal);
    else
    if(lv->type == table_m)
        avm_tableincrefcounter(lv->data.tableVal);
}

extern void execute_assign (instruction* instr){
    avm_memcell* lv = avm_translate_operand(&instr->result,(avm_memcell*)0);
    avm_memcell* rv = avm_translate_operand(&instr->arg1,&ax);

    //assert(lv && (&stack[N-1] >= lv && lv > &stack[top] || lv==&retval));
    //assert(rv);
    avm_assign(lv,rv);
}

