SymbolTableEntry* lookup_Scope(SymTable_T* sym, const char* key, unsigned int scope)
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

SymbolTableEntry* SymTable_contains(SymTable_T *oSymTable, const char *pcKey, unsigned int scope){
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
               return temp;
            }
        }else{
            if (strcmp(temp->value.funcVal->name, pcKey) == 0 && temp->isActive == true){ printf("%s\n",pcKey); return temp;}

        }
        temp= temp->next;
    }
    oSymTable->size++;
    return NULL;
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
    SymTable_contains(hash, "print", 0);
    SymTable_contains(hash, "input", 0);
    /*
    hash = SymTable_new();
    SymTable_insert(hash, "abcd", 1, id, 0, GLOBAL);
    SymTable_insert(hash, "expr", 1, id, 0, LOCAL);
    */
//printf("%d", SymTable_getLength(hash));

    return 0;
}
