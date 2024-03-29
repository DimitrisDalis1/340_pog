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
int    top=0, topsp=0;

/* needs binary file*/
double  consts_getnumber(unsigned index){
	
    return numbers[index]; //numConsts?
}
char*   consts_getstring(unsigned index) {
    return str_c[index];
}
char*   libfuncs_getused(unsigned index) {
    return lib_f[index];
}



userfunc* userfuncs_getfunc(unsigned address){
    for(int i=0 ; i < user; i++){
        userfunc* temp = &userfs[i];
        if(address == temp->address){
            return temp;
        }
    }
    return NULL;
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

extern char* typeStrings[]; 





avm_memcell* avm_translate_operand(vmarg* arg, avm_memcell* reg){
    assert(arg);
    if(reg)
        avm_memcellclear(reg);
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
		char* fixed=consts_getstring(arg->val);
		int length=strlen(fixed);
		if(!strcmp(&fixed[length-1],"\""))
		fixed[length-1]='\0';
	
            reg->data.strVal = strdup(fixed);
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

int strHash(char *id){
    int result = atoi(id) % AVM_TABLE_HASHSIZE;
    return result;
}
int numHash(int x){
    int result = x % AVM_TABLE_HASHSIZE;
    return result;
}
//int boolHash(bool){

//}
//int userfuncHash(userfunc*);

avm_table_bucket* avm_tablelookup(avm_table* table, avm_memcell* index) {
    avm_table_bucket* temp= (avm_table_bucket*) 0;
	int key;
    if(index->type == number_m){
    	 key = numHash(index->data.numVal);
         temp = table->numIndexed[key];
	if(temp != NULL){printf("prin loopa temp numval is %lf \n",temp->value->data.numVal); }else{printf("used in set\n");};
	 printf("prin loopa index numval is %lf \n",index->data.numVal);
	 

         while(temp !=NULL){ 
		printf("loop index numval is %lf \n",index->data.numVal);
             if(temp != NULL){printf("loop temp numval is %lf \n",temp->key->data.numVal);}
		if(temp->key->data.numVal == index->data.numVal){
			return temp;
		}
                temp = temp->next;
	}
		printf("end loop index numval is %lf \n",index->data.numVal);
             	 if(temp != NULL){printf("end loop temp numval is %lf \n",temp->key->data.numVal);}


	}else if(index->type == string_m){
	key = strHash(index->data.strVal);
        temp = table->strIndexed[key];
        while(temp){
            if(!strcmp(temp->value->data.strVal ,index->data.strVal)){
                return temp;
            }
            temp = temp->next;
        }
	}
    return temp;
}

void avm_tablesetelem(avm_table* table,avm_memcell* index,avm_memcell* content){
	avm_table_bucket* new_bucket;
	avm_table_bucket *start;
    assert(table);
    assert(index);
    if(index->type != string_m && index->type != number_m){
        avm_error("table index can only be a string or a number",&instrs[pc]);
    }
    if(index->type == number_m){
        int key = numHash(index->data.numVal);
        avm_table_bucket *temp =  avm_tablelookup(table, index);
	if(content->type == nil_m){
		if(temp != NULL) {
            		avm_memcellclear(temp->key);
            		avm_memcellclear(temp->value);
        	}
		if(table->total > 0) 
            		table->total--;
        	return;
	}
        if(temp == NULL){
		 start = table->numIndexed[key];
            	new_bucket = malloc(sizeof(avm_table_bucket));
		new_bucket->key = index;
        	new_bucket->value = content;
		new_bucket->next = table->numIndexed[key];
		table->numIndexed[key] = new_bucket;

		//new_bucket->next = start;
		//table->numIndexed[key] = new_bucket;
            	
          }else{
		avm_assign(temp->value, content);
	}
	table->total++;
}else if(index->type == string_m){
     int key = strHash(index->data.strVal);
        avm_table_bucket *temp =  avm_tablelookup(table, index);
	if(content->type == nil_m){
		if(temp != NULL) {
            		avm_memcellclear(temp->key);
            		avm_memcellclear(temp->value);
        	}
		if(table->total > 0) 
            		table->total--;
        	return;
	}
        if(temp == NULL){
		 start = table->strIndexed[key];
            	new_bucket = malloc(sizeof(avm_table_bucket));
		if(!new_bucket){ avm_error("Not enough space \n",&instrs[pc]);}
		new_bucket->key= index;
        	new_bucket->value= content;
		table->strIndexed[key] = new_bucket;
            	new_bucket->next = start;
          }else{
		avm_assign(temp->value, content);
	}
	table->total++;
}
}

avm_memcell* avm_tablegetelem (avm_table*  table,avm_memcell* index){
printf("psaxnoume index numval is %lf \n",index->data.numVal);
             
    assert(table);
    assert(index);
	

    avm_memcell* result = (avm_memcell*) 0;
    avm_table_bucket* temp = avm_tablelookup(table, index);
    if(temp == NULL){printf("here i am commiting suicide");}
    if(temp){ result = temp->value;}
    return result;
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
        avm_error("stack overflow", &instrs[pc]);
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
    assert(instrs[pc].opcode == call_v);
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
	
    if(!strcmp("print",id)){
        result = 0;
    }else if (!strcmp("typeof",id))
    {
         result = 1;
    }else if (!strcmp("totalarguments",id))
    {
         result = 2;
    }else if (!strcmp("sqrt",id))
    {
         result = 3;
    }else if (!strcmp("cos",id))
    {
         result = 4;
    }else if (!strcmp("sin",id))
    {
         result = 5;
    }else if (!strcmp("strtonum",id))
    {
         result = 6;
    }else if (!strcmp("input",id))
    {
         result = 7;
    }else if (!strcmp("argument",id))
    {
         result = 8;
    }else if (!strcmp("objecttotalmembers",id))
    {
         result = 9;
    }else if (!strcmp("objectmemberkeys",id))
    {
         result = 10;
    }else if (!strcmp("objectcopy",id))
    {
         result = 11;
    }else{
        result = 12;
    }
    return result;
	

}



char* number_tostring(avm_memcell* a){
    int n;
    n = getNumberOfDigits(a->data.numVal);
    char* buffer = malloc(40*sizeof(char));
    sprintf(buffer,"%0.3f",a->data.numVal);
    return buffer;
}
int getNumberOfDigits(int a){
	int count = 0;
	while(a > 0){
		count++;
		a = a/10;
	}
	return count;
}
char* string_tostring(avm_memcell* a){
	char* buffer =strdup(a->data.strVal);
	return buffer;}
char* bool_tostring(avm_memcell* a){
	if(a->data.boolVal == 0)
		return strdup("false");
	else
		return strdup("true");
}
char* table_tostring(avm_memcell* a){
	char *buffer = malloc(12 * sizeof(char*));
	char *dummy_buffer = malloc(sizeof(char*));
	strcat(buffer, "[");

	int i = 0;
	while (i < 211)
	{
		avm_table_bucket* temp = a->data.tableVal->strIndexed[i];
		while(temp != NULL){
			strcat(buffer, "{");
			strcat(buffer, temp->key->data.strVal);
			strcat(buffer, " : ");
			strcat(buffer, avm_tostring((temp->value)));
			strcat(buffer, "}");
			
			temp= temp->next;
		}

		temp = a->data.tableVal->numIndexed[i];
        	while (temp != NULL) {
            		strcat(buffer, "{");
            		sprintf(dummy_buffer, "%d", (int)temp->key->data.numVal);
           		strcat(buffer, dummy_buffer);
            		strcat(buffer, " : ");
            		strcat(buffer, avm_tostring((temp->value)));
            		strcat(buffer, "}");

            		temp = temp->next;
        	}
		i++;
	}

	strcat(buffer, "]");
	return buffer;
}
char* userfunc_tostring(avm_memcell* a){
	char* buff;
	buff = strdup(userfs[a->data.funcVal].id);	
	return buff;
}
char* libfunc_tostring(avm_memcell* a){
	char* buff;
	buff = strdup(a->data.libfuncVal);	
	return buff;

}
char* nil_tostring(avm_memcell* a){return strdup("nil");}
char* undef_tostring(avm_memcell* a){return strdup("undef");}



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


unsigned avm_get_envvalue(unsigned i){
   assert(avm_stack[i].type == number_m);
    unsigned val = (unsigned) avm_stack[i].data.numVal;
   assert(avm_stack[i].data.numVal == ((double) val));
   return val;
}


typedef void (*library_func_t)(void);
library_func_t libraryFuncs[] = {
   	libfunc_print,
	libfunc_typeof,
    	libfunc_totalarguments,
    	libfunc_sqrt,
   	libfunc_cos,
   	libfunc_sin,
	libfunc_strtonum,
	libfunc_input,
	libfunc_argument,
	libfunc_objecttotalmembers,
	libfunc_objectmemberkeys,
	libfunc_objectcopy
};

library_func_t avm_getlibraryfunc(char* id) {
	int i=libfunc_hash(id);
   if(i<12){
            return libraryFuncs[i];
	}
    return (library_func_t) 0;
}


void avm_calllibfunc(char* id){
    library_func_t f = avm_getlibraryfunc(id);
    if(!f){
        fprintf(stderr,"unsupported lib func '%s' called!",id); //is it wrong?
        executionFinished = 1;
    }
    else{
        
        ///////////////////////////////avm_callsaveenvironment();
        topsp = top; /*enter function sequence . no stack locals.*/
        totalActuals = 0;
        (*f)(); /*call lib function*/
        if(!executionFinished) /*an error may naturally occur inside*/
            execute_funcexit((instruction*)0); /*retrun seq*/
    }
}

unsigned avm_totalactuals(void){    return avm_get_envvalue(topsp + AVM_NUMACTUALS_OFFSET);
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
        char* s = avm_tostring(avm_getactual(i));
      printf("%s",s);
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
        avm_error("Error : only one parameter in function typeof!",&instrs[pc]);
        retval.type=nil_m;
    }
}
void libfunc_totalarguments(void){
    unsigned env = avm_get_envvalue(topsp + AVM_SAVEDTOPSP_OFFSET);
    avm_memcellclear(&retval);
    if(!env){
        avm_error("Error : 'totalarguments' called outside a funct!", &instrs[pc]);
        retval.type = nil_m;
    }else{
        retval.type = number_m;
        retval.data.numVal = avm_get_envvalue(env + AVM_NUMACTUALS_OFFSET);
    }
}
void libfunc_sqrt(void){
    if(avm_totalactuals() == 1 ){
        if(avm_getactual(0)->type != number_m){
            avm_error("Error : the parameter should be a number in sqrt!",&instrs[pc]);
	    return;
        }else if(avm_getactual(0)->data.numVal < 0){
            avm_error("Error : the parameter cannot be a negative number!",&instrs[pc]);
	    return;
        }else{
            avm_memcellclear(&retval);
            retval.type = number_m;
            double tmp = avm_getactual(0)->data.numVal;
            retval.data.numVal = sqrt(tmp);
        }
    }else{
        avm_error("Error : only one parameter in function sqrt!",&instrs[pc]);
	retval.type = nil_m;

    }
}



void libfunc_cos(void){
    double result;
	double dummy;
    if(avm_totalactuals() == 1)
    {
        if(avm_getactual(0)->type != number_m){
            avm_error("In function cos, given input is not of type: number_m", &instrs[pc]);
            retval.type = nil_m;
        }else
        {
            avm_memcellclear(&retval);
            retval.type = number_m;
            dummy = avm_getactual(0)->data.numVal;

            dummy = (dummy*PI)/180;
            retval.data.numVal = cos(dummy);
        }
    }
    else{
        avm_error("Only one parameter is allowed in function cos!\n", &instrs[pc]);
	retval.type = nil_m;

    }
}

void libfunc_sin(void){
    double result;
    unsigned int n = avm_totalactuals();
    if(n == 1)
    {
        if(avm_getactual(0)->type != number_m){
            avm_error("In function cos, given input is not of type: number_m", &instrs[pc]);
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
        avm_error("Only one parameter is allowed in function cos!\n", &instrs[pc]);
	retval.type = nil_m;
    }
}

void libfunc_strtonum(void){
     if(avm_totalactuals() == 1 ){
        if(avm_getactual(0)->type != string_m){
            avm_error("Error : the parameter should be a string in strtonum!",&instrs[pc]);
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
                    avm_error("Error : the string parameter should only contain numbers in strtonum!",&instrs[pc]);
                    retval.type = nil_m;
                    return;
                }
                if(tmp[i] == '.')
                    counter++;
                i++;
            }
            if(counter>1){
                avm_error("Error : the string parameter contains multiple '.' in strtonum!",&instrs[pc]);
                retval.type = nil_m;
                return;
            }
            retval.data.numVal = atof(tmp);
        }
    }else{
        avm_error("Error : only one parameter in function strtonum!",&instrs[pc]);
	retval.type=nil_m;

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
        avm_warning("No need for input in function: libfunc_input\n", &instrs[pc]);
    }
}

void libfunc_argument(void){
    if(avm_totalactuals() == 1){
        if (avm_getactual(0)->type == number_m)
        {
            if(avm_getactual(0)->data.numVal >  avm_get_envvalue(avm_get_envvalue(topsp + AVM_SAVEDTOPSP_OFFSET) + AVM_NUMACTUALS_OFFSET)-1 ){ //klemmeno full
                avm_error("Error : library function_argument argument out of range!",&instrs[pc]);
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
            avm_error("Error : only one parameter in libfunc_argument",&instrs[pc]);
		retval.type=nil_m;

        } 
        
    }else{
        avm_error("Error : only one parameter in libfunc_argument",&instrs[pc]);
	retval.type=nil_m;

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
            avm_error("Error : only table type parameter in libfunc_objecttotalmembers",&instrs[pc]);
		retval.type=nil_m;
       
 }
    }else{
        avm_error("Error : only one parameter in libfunc_objecttotalmembers",&instrs[pc]);
	retval.type=nil_m;

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
            avm_error("Error : only table type parameter in libfunc_objecttotalmembers",&instrs[pc]);
       		retval.type=nil_m;

	 }
    }else{
        avm_error("Error : only one parameter in libfunc_objecttotalmembers",&instrs[pc]);
   		retval.type=nil_m;

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
            avm_error("Error : only table type parameter in libfunc_objectcopy",&instrs[pc]);
       		retval.type=nil_m;

	 }
    }else{
        avm_error("Error : only one parameter in libfunc_objectcopy",&instrs[pc]);
	retval.type=nil_m;

    }
}



void avm_assign(avm_memcell* lv, avm_memcell* rv){
    if(lv == rv) /*same cells? destructive to assign!*/
        return;

    if(lv->type == table_m && rv->type == table_m && lv->data.tableVal == rv->data.tableVal)
    return;

    if(rv->type == undef_m) /*from undefined r-value? warn!*/
        avm_warning("assigning from 'undef' content! ",&instrs[pc]);

    avm_memcellclear(lv); /*clear old cell contents. */
    memcpy(lv,rv,sizeof(avm_memcell));    /*now take care of copied values or reference counting*/
    if(lv->type == string_m)
        lv->data.strVal = strdup(rv->data.strVal);
    else
    if(lv->type == table_m)
        avm_tableincrefcounter(lv->data.tableVal);
}




void avm_initialize(void) {
    avm_initstack();
    top = AVM_STACKSIZE-program_offset-1;
    topsp   =  AVM_STACKSIZE-1;
 	}

typedef unsigned int(*tobool_func_t)(avm_memcell*);

unsigned int number_tobool(avm_memcell* m) { return m->data.numVal != 0; }
unsigned int  string_tobool(avm_memcell* m) { return m->data.strVal[0] != 0; }
unsigned int bool_tobool(avm_memcell* m) { return m->data.boolVal; }
unsigned int  table_tobool(avm_memcell* m) { return 1; }
unsigned int userfunc_tobool(avm_memcell* m) { return 1; }
unsigned int libfunc_tobool(avm_memcell* m) { return 1; }
unsigned int nil_tobool(avm_memcell* m) { return 0; }
unsigned int undef_tobool(avm_memcell* m) { assert(0); return 0; }


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



unsigned int avm_tobool (avm_memcell* m) {
    assert(m->type >= 0 && m->type < undef_m);
    return (*toboolFuncs[m->type])(m);
}





typedef double (*toarithm_func_t)(avm_memcell*);

double number_toarithm(avm_memcell* m) { return m->data.numVal; }
double string_toarithm(avm_memcell* m) { size_t length= strlen(m->data.strVal); return length;}
double bool_toarithm(avm_memcell* m) {  assert(0); return 0; } 
double table_toarithm(avm_memcell* m) { return m->data.tableVal->total; }
double userfunc_toarithm(avm_memcell* m) { return m->data.numVal; }
double libfunc_toarithm(avm_memcell* m) { return strlen(m->data.libfuncVal); }
double nil_toarithm(avm_memcell* m) {  assert(0); return 0; }
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
        avm_stack[current_index++] = m;
    }
}

void avm_initstack(void){
    for(unsigned i = 0; i < AVM_STACKSIZE; i++){
        memset(&(avm_stack[i]), 0, sizeof(avm_stack[i]));
        avm_stack[i].type = undef_m;
    }
}
