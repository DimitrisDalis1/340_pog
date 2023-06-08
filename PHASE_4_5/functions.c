#include "avm.h"
#include<math.h>
#include<ctype.h>
#define PI 3.141592654
LibFuncHash* libfunc_hashtable = NULL;

void execute_call(instruction* instr){
    avm_memcell* func = avm_translate_operand(instr->result,&ax);
    assert(func);
    switch (func->type)
    {
    case userfunc_m :{
        avm_callsaveenvironment();
        pc = func->data.funcVal;
        assert(pc < AVM_ENDING_PC);
        assert(code[pc].opcode == funcenter_v);
        break;
    }
        /* code */
        case string_m: {avm_calllibfunc(func->data.strVal); break;}
        case libfunc_m: {avm_calllibfunc(func->data.libfuncVal); break;}
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
    assert(pc == func->data.funcVal); /* Func address should match PC. */
    /* Callee actions here. */
    totalActuals = 0;
    userfunc* funcInfo = avm_getfuncinfo(pc);
    topsp = top;
    top = top - funcInfo -> localSize; 
}


unsigned avm_get_envvalue(unsigned i){
    assert(avm_stack[i].type == number_m);
    unsigned val = (unsigned) avm_stack[i].data.numVal;
    assert(avm_stack[i].data.numVal == ((double) val));
   return val;
}

void execute_funcexit(instruction* unused){
    unsigned oldTop = top;
    top = avm_get_envvalue(topsp + AVM_SAVEDTOP_OFFSET);
    pc = avm_get_envvalue(topsp + AVM_SAVEDPC_OFFSET);
    topsp = avm_get_envvalue(topsp + AVM_SAVEDTOPSP_OFFSET);

   // while(++oldTop <= top)
        //avm_memcellclear(&stack[oldTop]);
}




void avm_registerlibfunc(char* id,library_func_t addr){
    assert(id);
    int index = libfunc_hash(id);
    libfunc_hashtable->LibTable[index] = addr;
    libfunc_hashtable->counter = libfunc_hashtable->counter + 1;
}

library_func_t avm_getlibraryfunc(char* id){
    assert(id);
    int index = libfunc_hash(id);
    if (libfunc_hashtable->LibTable[index] != NULL) {
        return libfunc_hashtable->LibTable[index];
    }
    return NULL;
}


void avm_calllibfunc(char* id){
    library_func_t f = avm_getlibraryfunc(id);
    if(!f){
        fprintf(stderr,"unsupported lib func '%s' called!",id); //is it wrong?
        executionFinished = 1;
    }
    else{
        /*Notice that enter function and  exit function
        are called manually!*/

        avm_callsaveenvironment();
        topsp = top; /*enter function sequence . no stack locals.*/
        totalActuals = 0;
        (*f)(); /*call lib function*/
        if(!executionFinished) /*an error may naturally occur inside*/
            execute_funcexit((instruction*)0); /*retrun seq*/
    }
}

unsigned avm_totalactuals(void){
    return avm_get_envvalue(topsp + AVM_NUMACTUALS_OFFSET);
}

avm_memcell* avm_getactual(unsigned i){
    assert(i < avm_totalactuals());
    return &avm_stack[topsp + AVM_STACKENV_SIZE + i + 1];
}



void execute_pusharg(instruction* instr){
    avm_memcell* arg = avm_translate_operand(instr->arg1,&ax);
    assert(arg);
    /*this is actually stack[top] = arg,but we have to use
        avm_assign.*/

    //avm_assign(&stack[top],arg);
    ++totalActuals;
    avm_dec_top();
}




//libfuncs

/* Implemantation of the library function 'print'.
    it displays every argumnet at the console.
*/

void libfunc_print(void){
    unsigned n = avm_totalactuals();
    for(unsigned i = 0; i<n; i++){
        char*s = avm_tostring(avm_getactual(i));
        puts(s);
        free(s);
    }
}

void libfunc_typeof(void){
    unsigned n = avm_totalactuals();
    if(avm_totalactuals() == 1){
        avm_memcellclear(&retval);
        retval.type = string_m;
        retval.data.strVal = strdup(typeStrings[avm_getactual(0)->type]);
    }else{
        avm_error("Error : only one parameter in function typeof!",&code[pc]);
        retval.type=nil_m;
    }
}
void libfunc_totalarguments(void){
    unsigned p_topsp = avm_get_envvalue(topsp + AVM_SAVEDTOPSP_OFFSET);
    avm_memcellclear(&retval);
    if(!p_topsp){
        avm_error("Error : 'totalarguments' called outside a function!", &code[pc]);
        retval.type = nil_m;
    }else{
        retval.type = number_m;
        retval.data.numVal = avm_get_envvalue(p_topsp + AVM_NUMACTUALS_OFFSET);
    }
}
void libfunc_sqrt(void){
    if(avm_totalactuals() == 1 ){
        if(avm_getactual(0)->type != number_a){
            avm_error("Error : the parameter should be a number in sqrt!",&code[pc]);
        }else if(avm_getactual(0)->data.numVal < 0){
            avm_error("Error : the parameter cannot be a negative number!",&code[pc]);
        }else{
            avm_memcellclear(&retval);
            retval.type = number_m;
            double tmp = avm_getactual(0)->data.numVal;
            retval.data.numVal = sqrt(tmp);
        }
    }else{
        avm_error("Error : only one parameter in function sqrt!",&code[pc]);
    }
}



void libfunc_cos(void){
    double result;
    if(avm_totalactuals() == 1)
    {
        if(avm_getactual(0)->type != number_m){
            avm_error("In function cos, given input is not of type: number_m", &code[pc]);
            retval.type = nil_m;
        }else
        {
            avm_memcellclear(&retval);
            retval.type = number_m;
            double dummy = avm_getactual(0)->data.numVal;

            dummy = (dummy*PI)/180;
            retval.data.numVal = cos(dummy);
        }
    }
    else{
        avm_error("Only one parameter is allowed in function cos!\n", &code[pc]);
    }
}

void libfunc_sin(void){
    double result;
    unsigned int n = avm_totalactuals();
    if(n == 1)
    {
        if(avm_getactual(0)->type != number_m){
            avm_error("In function cos, given input is not of type: number_m", &code[pc]);
            retval.type = nil_m;
        }else
        {
            avm_memcellclear(&retval);
            retval.type = number_m;
            double dummy = avm_getactual(0)->data.numVal;

            retval.data.numVal = sin(dummy);
        }
    }
    else
    if(n!= 1){
        avm_error("Only one parameter is allowed in function cos!\n", &code[pc]);
    }
}

void libfunc_strtonum(void){
     if(avm_totalactuals() == 1 ){
        if(avm_getactual(0)->type != string_m){
            avm_error("Error : the parameter should be a string in strtonum!",&code[pc]);
            retval.type = nil_m;
            return;
        }else{
            avm_memcellclear(&retval);
            retval.type = number_m;
            char* tmp = avm_getactual(0)->data.strVal;
            int i = 0;
            int counter = 0;
            while (i<strlen(tmp))
            {
                if(i==0 && tmp[i]=='-'){
                    continue;
                }
                if(!isdigit(tmp[i]) && tmp[i]!='.'){
                    avm_error("Error : the string parameter should only contain numbers in strtonum!",&code[pc]);
                    retval.type = nil_m;
                    return;
                }
                if(tmp[i] == '.')
                    counter++;
                i++;
            }
            if(counter>1){
                avm_error("Error : the string parameter contains multiple '.' in strtonum!",&code[pc]);
                retval.type = nil_m;
                return;
            }
            retval.data.numVal = atof(tmp);
        }
    }else{
        avm_error("Error : only one parameter in function strtonum!",&code[pc]);
    }
}

void libfunc_input(void){
    unsigned n = avm_totalactuals();
    if(n == 0)
    {
        int check_for_mul_dots = 0;
        int flag_for_number = 0;
        size_t curr_size = 1024;
        size_t counter = 0;
        char *s = malloc(sizeof(char)*curr_size);
        while(counter < curr_size)
        {
            s[counter] = getc(stdin);
            if(s[counter] == '\n'){
                s[counter] = '\0';
                break;
            }
            counter++;
            if(counter == curr_size)
            {
                s = (char*) realloc(s, 2*curr_size);
                curr_size = curr_size * 2;
            }
        }

        if(counter > 0)
        {
            int i = 0;
            while(i < counter){
                if(i==0 && s[i]=='-'){
                    continue;
                }
                if(!isdigit(s[i])){
                    flag_for_number = 1;
                    break;
                }
                if(s[i] == '.')
                {
                    check_for_mul_dots++;
                }
                if(check_for_mul_dots>=2)
                {
                    flag_for_number = 1;
                    break;
                }
                i++;
            }
        }

        avm_memcellclear(&retval);
        if(counter == 0)
        {
            retval.type = string_m;
            retval.data.strVal = strdup("");
        }else
        if(strcmp(s, "true") == 0)
        {
            retval.type=bool_m;
            retval.data.boolVal = 1;
        }else
        if(strcmp(s, "false") == 0){
            retval.type=bool_m;
            retval.data.boolVal = 0;
        }else
        if(strcmp(s, "nill") == 0){
            retval.type = nil_m;
        }else
        if(flag_for_number == 0){
            retval.type=number_m;
            retval.data.numVal = atof(s);
        }else
        {
            retval.type = string_m;
            retval.data.strVal = strdup(s);
        }

    }
    else{
        avm_warning("No need for input in function: libfunc_input\n", &code[pc]);
    }
}

void libfunc_argument(void){
    if(avm_totalactuals() == 1){
        if (avm_getactual(0)->type == number_m)
        {
            if(avm_getactual(0)->data.numVal >  avm_get_envvalue(avm_get_envvalue(topsp + AVM_SAVEDTOPSP_OFFSET) + AVM_NUMACTUALS_OFFSET)-1 ){ //klemmeno full
                avm_error("Error : library function_argument argument out of range!",&code[pc]);
                retval.type=nil_m;
                return;
            }
            avm_memcellclear(&retval);

            unsigned n = avm_get_envvalue(topsp + AVM_SAVEDTOPSP_OFFSET);
            unsigned data = avm_getactual(0)->data.numVal;
            avm_memcell* argument = &avm_stack[n +  AVM_STACKENV_SIZE + data + 1];

            if(argument->type != string_m){
                retval.data = argument->data;
                retval.type = argument->type; 
            }else{
                retval.data.strVal = strdup(argument->data.strVal);
                retval.type = argument->type; 
            } 
        }else{
            avm_error("Error : only one parameter in libfunc_argument",&code[pc]);
        } 
        
    }else{
        avm_error("Error : only one parameter in libfunc_argument",&code[pc]);
    }
}

void libfunc_objecttotalmembers(void){
    if(avm_totalactuals() == 1){
        if(avm_getactual(0)->type == table_m){
            avm_table* temp = avm_getactual(0)->data.tableVal;
            unsigned total = temp->total;
            avm_memcellclear(&retval);
            retval.type = number_m;
            retval.data.numVal = total;
        }else{
            avm_error("Error : only table type parameter in libfunc_objecttotalmembers",&code[pc]);
        }
    }else{
        avm_error("Error : only one parameter in libfunc_objecttotalmembers",&code[pc]);
    }
}

void libfunc_objectmemberkeys(void){
    int i;
    int j = 0;
    if(avm_totalactuals() == 1){
        if(avm_getactual(0)->type == table_m){
            avm_table* temp = avm_getactual(0)->data.tableVal;
            avm_table* temp_new = avm_tablenew();
            unsigned temp_size = temp->total;
            avm_memcell *new_key;
        for (i = 0; i < AVM_TABLE_HASHSIZE; i++) {
        avm_table_bucket* temp_loop = temp->strIndexed[i];

        while (temp_loop != NULL) {
            new_key->data.numVal = j;
            avm_table_bucket* new_bucket = (avm_table_bucket*)malloc(sizeof(avm_table_bucket));
            new_bucket->key = new_key;
            new_bucket->value = temp_loop->key;
            new_bucket->next = temp_new->strIndexed[j];
            temp_new->numIndexed[j] = new_bucket;
            temp_new->total++;
            j++;
            temp_loop = temp_loop->next;
        }

        temp_loop = temp->numIndexed[i];
        while (temp_loop != NULL) {
            new_key->data.numVal = j;
            avm_table_bucket* new_bucket = (avm_table_bucket*)malloc(sizeof(avm_table_bucket));
            new_bucket->key = new_key;
            new_bucket->value = temp_loop->key;
            new_bucket->next = temp_new->numIndexed[j];
            temp_new->numIndexed[j] = new_bucket;
            temp_new->total++;
            j++;
            temp_loop = temp_loop->next;
        }

        temp_loop = temp->boolIndexed[i];
        while (temp != NULL) {
            new_key->data.numVal = j;
            avm_table_bucket* new_bucket = (avm_table_bucket*)malloc(sizeof(avm_table_bucket));
            new_bucket->key = new_key;
            new_bucket->value = temp_loop->key;
            new_bucket->next = temp_new->boolIndexed[j];
            temp_new->numIndexed[j] = new_bucket;
            temp_new->total++;
            j++;
            temp_loop = temp_loop->next;
        }

        temp_loop = temp->userfuncIndexed[i];
        while (temp != NULL) {
            new_key->data.numVal = j;
            avm_table_bucket* new_bucket = (avm_table_bucket*)malloc(sizeof(avm_table_bucket));
            new_bucket->key = new_key;
            new_bucket->value = temp_loop->key;
            new_bucket->next = temp_new->userfuncIndexed[j];
            temp_new->numIndexed[j] = new_bucket;
            temp_new->total++;
            j++;
            temp_loop = temp_loop->next;
        }

        temp_loop  = temp->libfuncIndexed[i];
        while (temp != NULL) {
            new_key->data.numVal = j;
            avm_table_bucket* new_bucket = (avm_table_bucket*)malloc(sizeof(avm_table_bucket));
            new_bucket->key = new_key;
            new_bucket->value = temp_loop->key;
            new_bucket->next = temp_new->libfuncIndexed[j];
            temp_new->numIndexed[j] = new_bucket;
            temp_new->total++;
            j++;
            temp_loop = temp_loop->next;
        }


        temp_loop  = temp->tableIndexed[i];
        while (temp != NULL) {
            new_key->data.numVal = j;
            avm_table_bucket* new_bucket = (avm_table_bucket*)malloc(sizeof(avm_table_bucket));
            new_bucket->key = new_key;
            new_bucket->value = temp_loop->key;
            new_bucket->next = temp_new->tableIndexed[j];
            temp_new->numIndexed[j] = new_bucket;
            temp_new->total++;
            j++;
            temp_loop = temp_loop->next;
        }
    
    }
    retval.type = table_m;
    retval.data.tableVal = temp_new;
    avm_tableincrefcounter(retval.data.tableVal);

        }else{
            avm_error("Error : only table type parameter in libfunc_objecttotalmembers",&code[pc]);
        }
    }else{
        avm_error("Error : only one parameter in libfunc_objecttotalmembers",&code[pc]);
    }

}


void libfunc_objectcopy(void){
    if(avm_totalactuals() == 1){
        if(avm_getactual(0)->type == table_m){
            avm_table *temp = avm_getactual(0)->data.tableVal;
            avm_table *new_table = avm_tablenew();
             for (int i = 0; i < AVM_TABLE_HASHSIZE; i++) {
                avm_table_bucket* temp_bucket = temp->strIndexed[i];

                while (temp != NULL) {
                    avm_tablesetelem(new_table, temp_bucket->key, avm_tablegetelem(temp, temp_bucket->key));
                    temp_bucket = temp_bucket->next;
                }
                temp_bucket = temp ->numIndexed[i];
                while (temp != NULL) {
                    avm_tablesetelem(new_table, temp_bucket->key, avm_tablegetelem(temp, temp_bucket->key));
                    temp_bucket = temp_bucket->next;
                }

                temp_bucket = temp ->boolIndexed[i];
                while (temp != NULL) {
                    avm_tablesetelem(new_table, temp_bucket->key, avm_tablegetelem(temp, temp_bucket->key));
                    temp_bucket = temp_bucket->next;
                }
                temp_bucket = temp ->userfuncIndexed[i];
                while (temp != NULL) {
                    avm_tablesetelem(new_table, temp_bucket->key, avm_tablegetelem(temp, temp_bucket->key));
                    temp_bucket = temp_bucket->next;
                }
                temp_bucket = temp ->libfuncIndexed[i];
                while (temp != NULL) {
                    avm_tablesetelem(new_table, temp_bucket->key, avm_tablegetelem(temp, temp_bucket->key));
                    temp_bucket = temp_bucket->next;
                }
                temp_bucket = temp ->tableIndexed[i];
                while (temp != NULL) {
                    avm_tablesetelem(new_table, temp_bucket->key, avm_tablegetelem(temp, temp_bucket->key));
                    temp_bucket = temp_bucket->next;
                }

            }
            new_table->total = temp->total;
            avm_tableincrefcounter(new_table);
            retval.type = table_m;
            retval.data.tableVal = new_table;
        }else{
            avm_error("Error : only table type parameter in libfunc_objectcopy",&code[pc]);
        }
    }else{
        avm_error("Error : only one parameter in libfunc_objectcopy",&code[pc]);
    }
}