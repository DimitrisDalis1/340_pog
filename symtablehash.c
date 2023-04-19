#include "symtable.h"

unsigned int length=0;
Scope_node *head_scope_node=NULL;
SymTable_T* hash;
int current_scope=0;



Scope_node *create_scope(SymbolTableEntry *symbol,unsigned int scope,Scope_node *next,Scope_node *previous){
    Scope_node *new_scope =  malloc(sizeof(Scope_node));
    assert(new_scope);
    new_scope->symbol = symbol;
    new_scope->scope = scope;
    new_scope->next = next;
    new_scope->previous = previous;
    return new_scope;
}

SymbolTableEntry* SymTable_insert(SymTable_T* oSymTable,const char *name, const unsigned yyline ,id_list* args, const unsigned yyscope,enum SymbolType type){
    int hashIndex,flag,scope_counter=0;
    SymbolTableEntry* temp_entry=NULL;
    SymbolTableEntry* temp_hash=NULL;
    SymbolTableEntry *temp_head_entry=NULL;
    Scope_node *temp_head=NULL;
    Scope_node *temp=NULL;
    Scope_node *temp_index=NULL;
    SymbolTableEntry *temp_index_symbol=NULL;
    char *tempkey;
    temp_entry = (struct SymbolTableEntry*) malloc (sizeof(SymbolTableEntry));
    /*Initialize*/
    if(type==GLOBAL || type==LOCALV || type==FORMAL){
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
        (head_scope_node)->symbol->next=NULL;
        
    }else{
        temp_head = head_scope_node;
        temp_head_entry= head_scope_node->symbol;
        flag=yyscope;
        if(yyscope==temp_head->scope){
            if(temp_head_entry->next_in_scope==NULL){
                temp_head_entry->next_in_scope=temp_entry;
                temp_entry->next_in_scope=NULL;
                temp_entry->next=NULL;
                return temp_entry;
            }else{
                while(temp_head_entry->next_in_scope!=NULL){
                    temp_head_entry=temp_head_entry->next_in_scope;
                }
             
            }
            temp_head_entry->next_in_scope=temp_entry;
            temp_entry->next_in_scope=NULL;
            temp_head_entry= head_scope_node->symbol;    
            return temp_entry;
        }else{
            temp_index=temp_head;
            while(flag>0){
                if(temp_index->next==NULL){
                    temp=create_scope(NULL,++length,NULL,temp_index);
                    temp_index->next=temp;
                    temp_index=temp_index->next;
                    
                }else{
                        temp_index=temp_index->next;
                }
                
                 --flag;
            }   
            temp_index_symbol=temp_index->symbol;
            if(temp_index_symbol==NULL){
                temp_index->symbol=temp_entry;
                temp_index->symbol->next_in_scope=NULL;
                return temp_entry;
            }else{
                while(temp_index_symbol->next_in_scope!=NULL){
                    temp_index_symbol=temp_index_symbol->next_in_scope;
                }
            }  
                      
                temp_index_symbol->next_in_scope=temp_entry;
                temp_entry->next_in_scope=NULL;
                temp_index_symbol=temp_index->symbol;
               return temp_entry;
           
        }
    }
    /*Check if it should be expaned or not*/
    return NULL;
}

/* Return a hash code for name. */
static unsigned int SymTable_hash(const char *name,unsigned int SIZE){
  size_t ui;
  unsigned int uiHash = 0U;
  for (ui = 0U; name[ui] != '\0'; ui++)
    uiHash = uiHash * HASH_MULTIPLIER + name[ui];
  return uiHash % SIZE;
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
                }else if(temp->type == LOCALV){
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


/*Get the length of the Hash Table*/
unsigned int SymTable_getLength(SymTable_T *oSymTable){
    assert(oSymTable);
    return oSymTable->size;
}

int main()
{
    SymTable_T *s = SymTable_new();

    SymTable_insert(s, "a", 0, NULL, 1, LOCALV);
    SymTable_insert(s, "b", 0, NULL, 2, LOCALV);
    SymTable_insert(s, "c", 0, NULL, 7, LOCALV);
    SymTable_insert(s, "d", 0, NULL, 2, LOCALV);
    SymTable_insert(s, "aa", 0, NULL, 3, USERFUNC);
    SymTable_insert(s, "ab", 0, NULL, 3, USERFUNC);
    SymTable_insert(s, "ac", 0, NULL, 3, LOCALV);
    SymTable_insert(s, "ad", 0, NULL, 3, LOCALV);
    SymTable_insert(s, "ba", 0, NULL, 5, LOCALV);
    SymTable_insert(s, "bb", 0, NULL, 3, LOCALV);
    SymTable_insert(s, "bc", 0, NULL, 5, LOCALV);
    SymTable_insert(s, "bd", 0, NULL, 3, LOCALV);
    SymTable_insert(s, "ca", 0, NULL, 2, LOCALV);

    symtable_print(head_scope_node, s);

    return 0;
}
