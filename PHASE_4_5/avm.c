#include"avm.h"

LibFuncHash* libfunc_hashtable = NULL;

unsigned char executionFinished = 0;
unsigned        pc = 0;
unsigned        currLine = 0;
unsigned        codeSize = 0;
instruction*    code = NULL;
unsigned totalActuals = 0;
int disable_remove_warning = 0;

avm_memcell ax, bx, cx;
avm_memcell retval;
int    top, topsp;

/* needs binary file*/
double  consts_getnumber(unsigned index){
    return numConsts[index]; //numConsts?
}
char*   consts_getstring(unsigned index) {
    return stringConsts[index];
}
char*   libfuncs_getused(unsigned index) {
    return lib_f[index];
}


userfunc* avm_getfuncinfo(unsigned address){
    //for(int i=0 ; i< totalUserFuncs; i++){
       // if(address == address)
    //}
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
     memset(&(*t), 0, sizeof(*t));

    t->refCounter = t->total = 0;
    avm_tablebucketsinit(t->numIndexed);
    avm_tablebucketsinit(t->strIndexed);

    return t;
}

void avm_tablesetelem(avm_table* table,avm_memcell* index,avm_memcell* content){
	return;
}

void avm_tablebucketsdestroy(avm_table_bucket** p){
    for(unsigned i =0; i < AVM_TABLE_HASHSIZE; ++i, ++p){
        for(avm_table_bucket* b = *p; b;){
            avm_table_bucket* del = b;
            b = b->next;
            avm_memcellclear(del->key); //had &
            avm_memcellclear(del->value); //had &
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

extern char* typeStrings[]; //done in bool.c



avm_memcell* avm_translate_operand(vmarg* arg, avm_memcell* reg){
    assert(arg);
    if(reg)
        avm_memcellclear(reg);
    //printf("arg type = %d\n", arg->type);
    switch(arg->type){
        case global_a: return &avm_stack[AVM_STACKSIZE - 1 - arg->val];
        case local_a: return &avm_stack[topsp - arg->val];   
        case formal_a: return &avm_stack[topsp + AVM_STACKENV_SIZE + 1 + arg->val];

        case retval_a:  return &retval;

        case number_a:  {
            reg->type = number_m;
            reg->data.numVal = consts_getnumber(arg->val);
            return reg;
        }

        case string_a:  {
            reg->type = string_m;
            reg->data.strVal = strdup(consts_getstring(arg->val));
            return reg;
        }

        case bool_a:    {
            reg->type = bool_m;
            reg->data.boolVal = arg->val;
            return reg;
        }

        case nil_a: reg->type = nil_m; return reg;

        case userfunc_a:{
            reg->type = userfunc_m;
            reg->data.funcVal = arg->val;
            return reg;
        }

        case libfunc_a: {
            reg->type = libfunc_m;
            reg->data.libfuncVal = libfuncs_getused(arg->val);
            return reg;
        }

        default: assert(0);
    }
}


void avm_memcellclear(avm_memcell* m){
    if(m->type != undef_m){
        memclear_func_t f = memclearFuncs[m->type];
        if(f)
            (*f)(m);
        m->type = undef_m;
    }
}

extern void memclear_string (avm_memcell* m){
    assert(m -> data.strVal);
    free(m-> data.strVal);
}


memclear_func_t memclearFuncs[] = {
     0,  /* number */
     memclear_string,
     0,  /* bool */
     memclear_table,
     0,  /* userfunc */
     0,  /* libfunc */
     0,  /* nil */
     0   /* undef */
 };

avm_memcell* avm_tablegetelem (avm_table*  table,avm_memcell* index){
    
}


void memclear_table(avm_memcell* m){
    assert(m->data.tableVal);
    avm_tabledecrefcounter(m->data.tableVal);
}

/*
extern void avm_push_table_arg(avm_table* t){
    stack[top].type = table_m;
    avm_tableincerfcounter(stack[top].data.tale = t);
    ++totalActuals;
    avm_dec_top();
}

void avm_call_functor(avm_table* t){
    cx.type = string_m;
    cx.data.strVal = "()";
    avm_memcell* f = avm_tablegetelem(t, &cx);
    if(!f)
        avm_error("in calling table: no '()' element found!");
    else
    if (f->type == table_m)
        avm_call_functor(f->data,tableVal);
    else
    if(f->type == userfunc_a){
        avm_push_table_arg(t);
        avm_callsaveenvironment();
        pc = f -> data.funcVal;
        assert(pc < AVM_ENDING_PC && code[pc].opcode == funcenter_v);
    }
    else
        avm_error("in calling table: illegal '()' element value!");
}

*/


void avm_dec_top(void){
    if(!top){
        avm_error("stack overflow", &code[pc]);
        executionFinished = 1;
    }
    else{
        --top;
    }
}

void avm_push_envvalue(unsigned val){
    avm_stack[top].type         = number_m;
    avm_stack[top].data.numVal  = val;
    avm_dec_top();
}

void avm_callsaveenvironment(void){
    avm_push_envvalue(totalActuals);
    //assert(code[pc].opcode == call_v);
    avm_push_envvalue(pc+1);
    avm_push_envvalue(top + totalActuals + 2);
    avm_push_envvalue(topsp);
}

void avm_error(char* format,instruction* code){
    fprintf(stderr, "Error is: %s in line %u\n", format, code->srcLine);
}

void avm_warning(char* warning, instruction* code){
    fprintf(stderr, "Warning is: %s in line %u\n", warning, code->srcLine);
}

int libfunc_hash(char* id){
    assert(id);
    int result;
    if(strcmp("print",id)){
        result = 0;
    }else if (strcmp("typeof",id))
    {
         result = 1;
    }else if (strcmp("totalarguments",id))
    {
         result = 2;
    }else if (strcmp("sqrt",id))
    {
         result = 3;
    }else if (strcmp("cos",id))
    {
         result = 4;
    }else if (strcmp("sin",id))
    {
         result = 5;
    }else if (strcmp("strtonum",id))
    {
         result = 6;
    }else if (strcmp("input",id))
    {
         result = 7;
    }else if (strcmp("argument",id))
    {
         result = 8;
    }else if (strcmp("objecttotalmembers",id))
    {
         result = 9;
    }else if (strcmp("objectmemberkeys",id))
    {
         result = 10;
    }else if (strcmp("objectcopy",id))
    {
         result = 11;
    }else{
        result = 12;
    }
    return result;
}

char* number_tostring(avm_memcell* a){return "abc";}
char* string_tostring(avm_memcell* a){return "abc";}
char* bool_tostring(avm_memcell* a){return "abc";}
char* table_tostring(avm_memcell* a){return "abc";}
char* userfunc_tostring(avm_memcell* a){return "abc";}
char* libfunc_tostring(avm_memcell* a){return "abc";}
char* nil_tostring(avm_memcell* a){return "abc";}
char* undef_tostring(avm_memcell* a){return "abc";}

tostring_func_t tostringFuncs[]={
    number_tostring,
    string_tostring,
    bool_tostring,
    table_tostring,
    userfunc_tostring,
    libfunc_tostring,
    nil_tostring,
    undef_tostring
};

char* avm_tostring(avm_memcell* m){
    assert(m->type >= 0 && m->type <= undef_m);
    return (*tostringFuncs[m->type])(m);
}

char* avm_tostring(avm_memcell* cell);






unsigned avm_get_envvalue(unsigned i){
    assert(avm_stack[i].type == number_m);
    unsigned val = (unsigned) avm_stack[i].data.numVal;
    assert(avm_stack[i].data.numVal == ((double) val));
   return val;
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


void avm_initialize(void) {
    avm_initstack();
    libfunc_hashtable = malloc(sizeof(LibFuncHash));
    topsp = AVM_STACKSIZE-1;
    top   = AVM_STACKSIZE-1;

    for(int i=0; i < program_offset;i++){
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

typedef unsigned char(*tobool_func_t)(avm_memcell*);

unsigned char number_tobool(avm_memcell* m) { return m->data.numVal != 0; }
unsigned char string_tobool(avm_memcell* m) { return m->data.strVal[0] != 0; }
unsigned char bool_tobool(avm_memcell* m) { return m->data.boolVal; }
unsigned char table_tobool(avm_memcell* m) { return 1; }
unsigned char userfunc_tobool(avm_memcell* m) { return 1; }
unsigned char libfunc_tobool(avm_memcell* m) { return 1; }
unsigned char nil_tobool(avm_memcell* m) { return 0; }
unsigned char undef_tobool(avm_memcell* m) { assert(0); return 0; }


tobool_func_t toboolFuncs[]={
    number_tobool,
    string_tobool,
    bool_tobool,
    table_tobool,
    userfunc_tobool,
    libfunc_tobool,
    nil_tobool,
    undef_tobool
};



unsigned char avm_tobool (avm_memcell* m) {
    assert(m->type >= 0 && m->type < undef_m);
    return (*toboolFuncs[m->type])(m);
}


typedef double (*toarithm_func_t)(avm_memcell*);

double number_toarithm(avm_memcell* m) { return m->data.numVal; }
double string_toarithm(avm_memcell* m) { size_t length= strlen(m->data.strVal); return length;}
double bool_toarithm(avm_memcell* m) { return m->data.boolVal; } 
double table_toarithm(avm_memcell* m) { return 1; }
double userfunc_toarithm(avm_memcell* m) { return 1; }
double libfunc_toarithm(avm_memcell* m) { return 1; }
double nil_toarithm(avm_memcell* m) { return 0; }
double undef_toarithm(avm_memcell* m) { assert(0); return 0; }


toarithm_func_t toarithmFuncs[]={
    number_toarithm,
    string_toarithm,
    bool_toarithm,
    table_toarithm,
    userfunc_toarithm,
    libfunc_toarithm,
    nil_toarithm,
    undef_toarithm
};



int avm_toarithm (avm_memcell* m) {
    assert(m->type >= 0 && m->type < undef_m);
    return (*toarithmFuncs[m->type])(m);
}

char* typeStrings[] = {
    "number",
    "string",
    "bool",
    "table",
    "userfunc",
    "libfunc",
    "nil",
    "undef"
};

avm_memcell avm_stack[AVM_STACKSIZE];
int current_index = AVM_STACKSIZE - 1;

avm_memcell pop_avm_stack(){
    if(current_index > -1){
        printf("Avm stack underflow");
        exit(0);
    }else{
        --current_index;
    }
}

void push_avm_stack(avm_memcell m){
    if(current_index >= AVM_STACKSIZE){
        printf("Avm stack overflow");
        exit(0);
    }else{
        avm_stack[++current_index] = m;
    }
}

void avm_initstack(void){
    for(unsigned i = 0; i < AVM_STACKSIZE; i++){
        memset(&(avm_stack[i]), 0, sizeof(avm_stack[i]));
        avm_stack[i].type = undef_m;
    }
}
