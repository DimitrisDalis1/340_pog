#include "avm.h"

void avm_initialize(void) {
    avm_initstack();
    avm_init_libfuncs_hash();
    topsp = AVM_STACKSIZE-1;
    top   = AVM_STACKSIZE-1;

    for(int i=0; i < total_global_var;i++){
        avm_stack[top].data.numVal = i;
        avm_dec_top();
        topsp--;
    }
    avm_registerlibfunc("print", libfunc_print);
    avm_registerlibfunc("typeof", libfunc_typeof);
    avm_registerlibfunc("totalarguments", libfunc_totalarguments);
    avm_registerlibfunc("sqrt", libfunc_sqrt);
    avm_registerlibfunc("cos", libfunc_cos);
    avm_registerlibfunc("sin", libfunc_sin);
    avm_registerlibfunc("strtonum", libfunc_strtonum);
    avm_registerlibfunc("input", libfunc_input);
    avm_registerlibfunc("argument", libfunc_argument);
    avm_registerlibfunc("objecttotalmembers",libfunc_objecttotalmembers);
    avm_registerlibfunc("objectmemberkeys",libfunc_objectmemberkeys);
    avm_registerlibfunc("objectcopy",libfunc_objectcopy);

    pc = 1;
}