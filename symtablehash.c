#include "symtable.h"

unsigned int length=0;
Scope_node *head_scope_node=NULL;
SymTable_T* hash ;

int insert(id_list *ptr,char *name){
    assert(ptr);
    id_list *x = malloc(sizeof(id_list));
    x = ptr->head;
    if (id_list_contains(ptr,name))
    {
        return 0;
    }
    if(ptr->head == NULL){
        ptr->head = x;
    }else{
        x = ptr-> head;
    }
    while(x->next != NULL){
        x=x->next;
    }
    x->id = malloc(sizeof(strlen(name)+1));
    x->id = strdup(name);
    assert(x);
    x->next=NULL;
    ptr->length++;
    return 1;
}

int id_list_contains(id_list *x, const char *name){
    struct id_list* temp;
    assert(x);
    assert(name);

    /*Go to each node and see if the keys match*/
    temp = x -> head;
    while (temp!=NULL)
    {
        if (strcmp(temp->id,name)==0) return 1;
        temp=temp->next;
    }
    return 0;
}

void id_list_free(id_list *ptr){ 
    struct id_list* tmp;
    if (ptr) return; /*if it is null do not  do anything*/

   /*Free each node of the list*/
   while (ptr->head!=NULL)
    {
        tmp=ptr->head;
        ptr->head=tmp->next;
        free(tmp->id);
        free(tmp);
    }
    free(ptr);
}

Scope_node *create_scope(SymbolTableEntry *symbol,unsigned int scope,Scope_node *next,Scope_node *previous){
    Scope_node *new_scope =  malloc(sizeof(Scope_node));
    assert(new_scope);
    new_scope->symbol = symbol;
    new_scope->scope = scope;
    new_scope->next = next;
    new_scope->previous = previous;
    return new_scope;
}

int SymTable_insert(SymTable_T* oSymTable,const char *name, const unsigned yyline ,id_list* args, const unsigned yyscope,enum SymbolType type){
    int hashIndex,flag,scope_counter=0;
    SymbolTableEntry* temp_entry=NULL;
    SymbolTableEntry* temp_hash=NULL;
    SymbolTableEntry *temp_head_entry=NULL;
    Scope_node *temp_head=NULL;
    Scope_node *temp=NULL;
    Scope_node *temp_index=NULL;
    char *tempkey;
    temp_entry = (struct SymbolTableEntry*) malloc (sizeof(SymbolTableEntry));
    /*Initialize*/
    if(type==GLOBAL || type==LOCAL || type==FORMAL){
        temp_entry->value.varVal=(struct Variable*)malloc(sizeof(Variable));
        temp_entry->isActive=true;
        temp_entry->type=type;
        temp_entry->value.varVal->line=yyline;
        temp_entry->value.varVal->scope=yyscope;
        temp_entry->value.varVal->name=strdup(name);
        
        //add id to the correct scope list
    }else{
        temp_entry->value.funcVal=(struct Function*)malloc(sizeof(Function));
        temp_entry->isActive=true;
        temp_entry->type=type;
        temp_entry->value.funcVal->line=yyline;
       //////////// temp_entry->value.funcVal->args=args;
        temp_entry->value.funcVal->scope=yyscope;
        //add id to the correct scope list
        temp_entry->value.funcVal->name=strdup(name);
    }
    /*Generate the hash index*/
    hashIndex=SymTable_hash(name,oSymTable->buckets);
    
    /*Put it in the hash table*/
   temp_hash=oSymTable->hashtable[hashIndex];
    if(temp_hash == NULL){
        oSymTable->hashtable[hashIndex]=temp_entry;
        oSymTable->size++;
       
    }else{
        while(temp_hash->next != NULL){
            temp_hash=temp_hash->next;
        }
        temp_hash->next=temp_entry;
        
    }

    temp_entry->next=NULL;
    
    if(head_scope_node == NULL){
        head_scope_node = create_scope(temp_entry,0,NULL,NULL);
        (head_scope_node)->symbol->next_in_scope=NULL;
    }else{
        temp_head = head_scope_node;
        temp_head_entry= head_scope_node->symbol;
        flag=yyscope;
        if(yyscope==temp_head->scope){
            if(temp_head_entry->next_in_scope==NULL){
                temp_head_entry->next_in_scope=temp_entry;
                temp_entry->next_in_scope=NULL;
                return 0;
            }else{
                while(temp_head_entry->next_in_scope!=NULL){
                    temp_head_entry=temp_head_entry->next_in_scope;
                }
            }
            temp_head_entry->next_in_scope=temp_entry;
            temp_entry->next_in_scope=NULL;
            return 0;
        }else{
            temp_index=temp_head;
            while(flag>0){
                if(temp_index->next==NULL && flag>1){
                    temp=create_scope(NULL,++length,NULL,temp_index);
                    temp_index->next=temp;
                    temp_index=temp;
                }else {
                    if(temp_index->next==NULL && flag==1){
                        temp=create_scope(temp_entry,yyscope,NULL,temp_index);
                        temp_index->next=temp;
                        ++length;
                        temp_index->next->symbol->next_in_scope=NULL;
                        return 0;
                    }else{
                        temp_index=temp_index->next;
                    }
                }
                
                 --flag;
            }   

            if(temp_index->symbol==NULL){
                temp_index->symbol=temp_entry;
                temp_entry->next_in_scope=NULL;
                return 0;
            }else{
                while(temp_index->symbol->next_in_scope!=NULL){
                    temp_index->symbol=temp_index->symbol->next_in_scope;
                }
            }            
            temp_index->symbol->next_in_scope=temp_entry;
            temp_entry->next_in_scope=NULL;
            return 0;
        }
    }
    /*Check if it should be expaned or not*/
    expand(oSymTable);
    return 1;
}

/* Return a hash code for name. */
static unsigned int SymTable_hash(const char *name,unsigned int SIZE){
  size_t ui;
  unsigned int uiHash = 0U;
  for (ui = 0U; name[ui] != '\0'; ui++)
    uiHash = uiHash * HASH_MULTIPLIER + name[ui];
  return uiHash % SIZE;
} 

SymbolTableEntry* lookup_inScope(SymTable_T* sym, const char* key, unsigned int scope)
{
    int hashIndex;
    SymbolTableEntry *temp;
    //temp = malloc(sizeof(SymbolTableEntry*));
    assert(sym);
    assert(key);


    /*Generate the hash index*/
    hashIndex=SymTable_hash(key,sym->buckets);
    //printf("hash index inside sym_cont: %d\n", hashIndex);

    /*Search if there is a cell with this key*/
    temp = sym->hashtable[hashIndex];
    if(temp == NULL){ printf("is null\n");}
    while (temp!=NULL)
    {
        //Kanoume mesa sto bucket traverse mexri na broume to idio onoma entolhs
        
        if(temp->type==GLOBAL||temp->type==LOCAL||temp->type==FORMAL){
            if (strcmp(temp->value.varVal->name,key) == 0 && temp->value.varVal->scope == scope)
            {
               return temp;
            }
        }else {
            if (strcmp(temp->value.funcVal->name, key) == 0 && (temp->isActive == true) && (temp->value.funcVal->scope = scope)){
                return temp;
            }
        }
        temp= temp->next_in_scope;
    }
    return NULL;
}

static void expand(SymTable_T *oSymTable){
    unsigned int SIZE,BUCKETS;
    struct SymbolTableEntry *temp,*prev,*next,*list;
    int i,hashkey;
    list=NULL;
    prev=NULL;
    BUCKETS=oSymTable->buckets;
    SIZE=oSymTable->size;

    /*Check if the the hashtable can be expanded or not*/
    if (BUCKETS==MAX_SIZE || SIZE<BUCKETS) return;

    /*Determine next size*/
    if (BUCKETS==509){
        BUCKETS=1021;
    }else if(BUCKETS==1021){
        BUCKETS=2053;
    }else if(BUCKETS==2053){
        BUCKETS=4093;
    }else if(BUCKETS==4093){
        BUCKETS=8191;
    }else if(BUCKETS==8191){
        BUCKETS=16381;
    }else if(BUCKETS==16381){
        BUCKETS=32771;
    }else if(BUCKETS==32771){
        BUCKETS=MAX_SIZE;
    }
    

    /*Make a list*/
    /*Pointer that points to the first element of the hashtable */
    /*Then connect the last element of the 1st cell to the 1st element of the second cell etc...*/
    list=oSymTable->hashtable[0];
    for ( i = 0; i < oSymTable->buckets; i++)
    {
        if(prev!=NULL && i!=oSymTable->buckets-1) prev->next = oSymTable->hashtable[i];
        temp=oSymTable->hashtable[i];
        while (temp!=NULL)
        {
            prev=temp;
            temp=temp->next;
        }
    }
    for ( i = 0; i < oSymTable->buckets; i++)
    {
        oSymTable->hashtable[i]=NULL;
    }
    

    /*Put the list to the new Buckets*/
    oSymTable->buckets=BUCKETS;
    oSymTable->hashtable=realloc(oSymTable->hashtable,oSymTable->buckets * sizeof(SymbolTableEntry));
    assert(oSymTable->hashtable);

    /*Add them*/
    temp=list;
    while (temp)
    {
        next=temp->next;
        if(temp->type==GLOBAL|temp->type==LOCAL|temp->type==FORMAL){
               hashkey=SymTable_hash(temp->value.varVal->name,oSymTable->buckets);
        }else{
               hashkey=SymTable_hash(temp->value.funcVal->name,oSymTable->buckets);
        }
        temp->next = oSymTable->hashtable[hashkey];  /*so we dont lose the other elements*/
        oSymTable->hashtable[hashkey] = temp;  
        temp = next;
    }
}

SymTable_T* SymTable_new(void){
    SymTable_T *oSymTable=malloc(sizeof(SymTable_T*));
    int i;
    assert(oSymTable);
    oSymTable->hashtable=malloc(509 * sizeof(SymbolTableEntry*));
    for(i=0;i<509;i++){
        oSymTable->hashtable[i]=NULL;    
    }
    oSymTable->buckets=509;
    oSymTable->size=0;
    SymTable_insert(oSymTable,"print",0,NULL,0,LIBFUNC);
    SymTable_insert(oSymTable,"input",0,NULL,0,LIBFUNC);
    SymTable_insert(oSymTable,"objectmemberkeys",0,NULL,0,LIBFUNC);
    SymTable_insert(oSymTable,"objecttotalmembers",0,NULL,0,LIBFUNC);
    SymTable_insert(oSymTable,"objectcopy",0,NULL,0,LIBFUNC);
    SymTable_insert(oSymTable,"totalarguments",0,NULL,0,LIBFUNC);
    SymTable_insert(oSymTable,"argument",0,NULL,0,LIBFUNC);
    SymTable_insert(oSymTable,"typeof",0,NULL,0,LIBFUNC);
    SymTable_insert(oSymTable,"strtonum",0,NULL,0,LIBFUNC);
    SymTable_insert(oSymTable,"strtonum",0,NULL,0,LIBFUNC);
    SymTable_insert(oSymTable,"cos",0,NULL,0,LIBFUNC);
    SymTable_insert(oSymTable,"sin",0,NULL,0,LIBFUNC);
    return oSymTable;
}

void symtable_print(Scope_node *head,SymTable_T*  hashtable){
    SymbolTableEntry *temp;
    int length;
    Scope_node *ptr=head;
    assert(ptr);
    printf("                        SYNTAX ANALYSIS                             \n");
    while(ptr != NULL)
    {
        printf("-------------------------   Scope #%d   ------------------------- \n",ptr->scope);
        temp = ptr->symbol;
        while (temp != NULL)
        {
            if (temp->type == LIBFUNC || temp->type == USERFUNC)
            {
                printf("%-*s \t",20,temp->value.funcVal->name);
                if (temp->type == LIBFUNC)
                {
                   printf(" [library function] ");
                }else if(temp->type == USERFUNC){
                   printf("  [user  function]  ");
                }
                printf(" (line %d) ",temp->value.funcVal->line);
                printf(" (scope %d)",temp->value.funcVal->scope);
            }else{
                printf("%-*s \t",20,temp->value.varVal->name);
                if(temp->type == GLOBAL){
                   printf(" [global  variable] ");
                }else if(temp->type == LOCAL){
                   printf(" [local   variable] ");
                }else if(temp->type == FORMAL){
                   printf(" [formal  variable] ");
                }
                printf(" (line %d) ",temp->value.varVal->line);
                printf(" (scope %d) ",temp->value.varVal->scope);
            }
            printf("\n");
            temp = temp->next_in_scope;
        }
        printf("\n");
        ptr = ptr->next;
    }
    return;
}




void scope_deactivate(Scope_node *ScopeTable){
    struct Scope_node *temp;
    temp = ScopeTable;
    if (ScopeTable) return; /*if it is null do not  do anything*/
    while(temp->symbol->next_in_scope != NULL){
        temp->symbol->isActive = false;
        temp->symbol->next_in_scope = temp->next->symbol->next_in_scope;
    }
    return;
}


/*Get the length of the Hash Table*/
unsigned int SymTable_getLength(SymTable_T *oSymTable){
    assert(oSymTable);
    return oSymTable->size;
}


int SymTable_contains(SymTable_T *oSymTable, const char *pcKey, unsigned int scope){
    int hashIndex;
    SymbolTableEntry *temp;
    //temp = malloc(sizeof(SymbolTableEntry*));
    assert(oSymTable);
    assert(pcKey);


    /*Generate the hash index*/
    hashIndex=SymTable_hash(pcKey,oSymTable->buckets);
    //printf("hash index inside sym_cont: %d\n", hashIndex);

    /*Search if there is a cell with this key*/
    temp = oSymTable->hashtable[hashIndex];
    if(temp == NULL){ printf("is null\n");}
    while (temp!=NULL)
    {
        //Kanoume mesa sto bucket traverse mexri na broume to idio onoma entolhs
        
        if(temp->type==GLOBAL||temp->type==LOCAL||temp->type==FORMAL){
            if (strcmp(temp->value.varVal->name,pcKey) == 0 && temp->value.varVal->scope == scope)
            {
               return 1;
            }
        }else{
            if (strcmp(temp->value.funcVal->name, pcKey) == 0 && temp->isActive == true){ printf("%s\n",pcKey); return 1;}

        }
        temp= temp->next;
    }
    oSymTable->size++;
    return 0;
}

int main()
{
    hash = SymTable_new();
    symtable_print(head_scope_node,hash);
    
    return 0;
}
