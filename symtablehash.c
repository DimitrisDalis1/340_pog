#include "symtable.h"

unsigned int length=0;
Scope_node *head_scope_node=NULL;

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

void length_increase(unsigned int yyscore){
    Scope_node *temp_head  = head_scope_node;
    Scope_node *temp;
    int additional_nodes = yyscore - length;
    while(temp_head->next != NULL){
        temp_head = temp_head->next; //ftanei sto telos
    }
    while(additional_nodes>0){ //oso scope > tou length aujanei to length kai dimiourgei scope nodes "adeia"
        temp = create_scope(NULL,length++,NULL,temp_head); 
        temp_head->next = temp;
        temp_head = temp;
        additional_nodes--;
        length++;
    }
}

int SymTable_insert(SymTable_T* oSymTable,const char *name, const unsigned yyline ,id_list* args, const unsigned yyscope,enum SymbolType type){
    int hashIndex,flag;
    SymbolTableEntry* temp_entry;
    SymbolTableEntry* temp_hash;
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
        head_scope_node->symbol->next_in_scope = NULL;
    }else{
        Scope_node *temp_head;
        Scope_node *temp;
        temp_head = head_scope_node;
        flag=yyscope;
        if(yyscope==head_scope_node->scope){
            while(temp_head->symbol->next_in_scope!=NULL){
                temp_head->symbol=temp_head->symbol->next_in_scope;
            }
            temp_head->symbol->next_in_scope=temp_entry;
            
            printf("%s",temp_entry->value.funcVal->name);
            temp_entry->next_in_scope=NULL;
        }else{
            printf("%s",temp_entry->value.funcVal->name);
            while(flag!=0){
                if(temp_head->next==NULL){
                    temp=create_scope(temp_entry,flag,NULL,temp_head);
                    temp_head->next=temp;
                    length++;
                    temp_head->next->symbol->next_in_scope=NULL;
                    return 0;
                }else{
                   temp_head=temp_head->next;
                }
                 --flag;
            }   
                    while(temp_head->symbol->next_in_scope!=NULL){
                        temp_head->symbol=temp_head->symbol->next_in_scope;
                    }
                    temp_head->symbol->next_in_scope=temp_entry;
                    temp_entry->next_in_scope=NULL;
                
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
    SymTable_insert(oSymTable,"strtonum",0,NULL,1,LIBFUNC);
    SymTable_insert(oSymTable,"sqrt",0,NULL,1,LIBFUNC);
    SymTable_insert(oSymTable,"cos",0,NULL,2,LIBFUNC);
    SymTable_insert(oSymTable,"sin",0,NULL,3,LIBFUNC);
    return oSymTable;
}

void symtable_print(Scope_node *head){
    SymbolTableEntry *temp;
    Scope_node *ptr=head;
    assert(ptr);
    while(ptr != NULL)
    {
        printf("--------- #%d --------- \n",ptr->scope);
        temp = ptr->symbol;
        while (temp != NULL)
        {
            if (temp->type == LIBFUNC || temp->type == USERFUNC)
            {
                printf("%s ",temp->value.funcVal->name);
                if (temp->type == LIBFUNC)
                {
                   printf("[library function] ");
                }else if(temp->type == USERFUNC){
                    printf("[user function] ");
                }
                printf("(line %d) ",temp->value.funcVal->line);
                printf("(scope %d)",temp->value.funcVal->scope);
            }else{
                if(temp->type == GLOBAL){
                    printf("[global variable] ");
                }else if(temp->type == LOCAL){
                    printf("[local variable] ");
                }else if(temp->type == FORMAL){
                    printf("[formal variable] ");
                }
                printf("(line %d) ",temp->value.varVal->line);
                printf("(scope %d)",temp->value.varVal->scope);
            }
            printf("\n");
            temp = temp->next_in_scope;
        }
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
    SymTable_T* hash = malloc(sizeof(SymTable_T*));
    const char* name = "print";
    const char* name1 = "input";
    assert(hash);
    hash->hashtable=malloc(509 * sizeof(SymbolTableEntry*));
    hash->buckets=509;
    hash->size = 1;
    SymbolTableEntry* temp_entry;
    int hashIndex1=SymTable_hash("print",hash->buckets);
    int hashIndex2=SymTable_hash("input",hash->buckets);

    temp_entry = (struct SymbolTableEntry*) malloc (sizeof(SymbolTableEntry));
    temp_entry->value.funcVal=(struct Function*)malloc(sizeof(Function));
    temp_entry->next = (struct SymbolTableEntry*) malloc (sizeof(SymbolTableEntry));
    temp_entry->next->value.funcVal=(struct Function*)malloc(sizeof(Function));
    temp_entry->value.funcVal->name=strdup(name);
    temp_entry->next->value.funcVal->name=strdup(name1);
    temp_entry->type=LIBFUNC;
    temp_entry->next->type=LIBFUNC;
    temp_entry->value.funcVal->scope = 0;
    temp_entry->next->value.funcVal->scope = 0;
    temp_entry->isActive = true;
    temp_entry->next->isActive = true;
    hash->hashtable[hashIndex1]=temp_entry;
    hash->hashtable[hashIndex2]=temp_entry->next;

    

    id_list *id;
    /*
    printf("%s\n", temp_entry->value.funcVal->name );
    printf("hash index inside main: %d\n", hashIndex);
    */
    //SymTable_contains(hash, "print", 0);
    //SymTable_contains(hash, "input", 0);
    
    hash = SymTable_new();
    symtable_print(head_scope_node);
    //SymTable_insert(hash, "abcd", 1, id, 0, GLOBAL);
    //SymTable_insert(hash, "expr", 1, id, 0, LOCAL);
    
//printf("%d", SymTable_getLength(hash));

    return 0;
}
