#include"avm.h"
//#include"../read_binary/read_binary.h"
#include<math.h>
#include<ctype.h>
#define PI 3.141592654
void execute_call(instruction* instr){
    avm_memcell* func = avm_translate_operand(&instr->result,&ax);
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
        case string_m: avm_calllinfunc(func->data.strVal); break;
        case libfunc_m: avm_calllinfunc(func->data.libfuncVal); break;
        case table_m: avm_calllinfunc(func->data.tableVal); break;
    default:{
        char* s = avm_tostring(func);
        avm_error("call : cannot bind '%s' to function!",s );
        free(s);
        executionFinished = 1;
        }
    }
}

void execute_funcenter (instruction* instr){
    avm_memcell* func = avm_translate_operand(&instr->result, &ax);
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





void libfunc_totalarguments(void){
    unsigned p_topsp = avm_get_envvalue(topsp + AVM_SAVEDTOPSP_OFFSET);
    avm_memcellclear(&retval);
    if(!p_topsp){
        avm_error("'totalarguments' called outside a function!", &code[pc]);
        retval.type = nil_m;
    }else{
        retval.type = number_m;
        retval.data.numVal = avm_get_envvalue(p_topsp + AVM_NUMACTUALS_OFFSET);
    }
}

void libfunc_typeof(void){
    unsigned i = avm_totalactuals();
    if(i != 1)
        avm_error("one argument (not %d) expected in 'typeof'!",i);
    else{
        /*Thats how a lib function returns a result.
        It has to only set the 'retval' register!
        */
        avm_memcellclear(&retval);/*don't forget to clean-it up!*/
        retval.type = string_m;
        retval.data.strVal = strdup(typeStrings[avm_getactual(0)->type]);
    }
}


void avm_calllibfunc(char* id){
    library_func_t f = avm_getlibraryfunc(id);
    if(!f){
        avm_error("unsupported lib func '%s' called!",id);
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
    //return &stack[topsp + AVM_STACKENV_SIZE + i + 1];
}


void avm_registerlibfunc(char* id,library_func_t addr);

void execute_pusharg(instruction* instr){
    avm_memcell* arg = avm_translate_operand(&instr->arg1,&ax);
    assert(arg);
    /*this is actually stack[top] = arg,but we have to use
        avm_assign.*/

    //avm_assign(&stack[top],arg);
    ++totalActuals;
    avm_dec_top();
}




//libfuncs

void libfunc_print(void){
    unsigned n = avm_totalactuals();
    for(unsigned i = 0; i<n; i++){
        char*s = avm_tostring(avm_getactual(i));
        puts(s);
        free(s);
    }
}

void libfunc_typeof(void){

}
void libfunc_totalarguments(void){
    unsigned p_topsp = avm_get_envvalue(topsp + AVM_SAVEDTOPSP_OFFSET);
    avm_memcellclear(&retval);
    if(!p_topsp){
        avm_error("'totalarguments' called outside a function!", &code[pc]);
        retval.type = nil_m;
    }else{
        retval.type = number_m;
        retval.data.numVal = avm_get_envvalue(p_topsp + AVM_NUMACTUALS_OFFSET);
    }
}
void libfunc_sqrt(void){
    if(avm_totalActuals() == 1 ){
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

            dummy = (dummy*PI)/180;
            retval.data.numVal = cos(dummy);
        }
    }
    else
    if(n!= 1){
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
     if(avm_totalActuals() == 1 ){
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
    if(avm_totalActuals() == 1){
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
    //if(totalActuals())
}
void libfunc_objectmemberkeys(void);
void libfunc_objectcopy(void);
