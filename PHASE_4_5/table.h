#define AVM_TABLE_HASHSIZE  211
#define HASH_MULTIPLIER 65599

typedef enum avm_memcell_t {
    number_m    = 0,
    string_m    = 1,
    bool_m      = 2,
    table_m     = 3,
    userfunc_m  = 4,
    libfunc_m   = 5,
    nil_m       = 6,
    undef_m     = 7
} avm_memcell_t;

/*klemeno*/
typedef struct avm_table_bucket {
    avm_memcell key;
    avm_memcell value;
    struct avm_table_bucket* next;
} avm_table_bucket;

/*klemeno*/
typedef struct avm_table{
    unsigned refCounter;
    avm_table_bucket* strIndexed[AVM_TABLE_HASHSIZE];
    avm_table_bucket* numIndexed[AVM_TABLE_HASHSIZE];
    avm_table_bucket* boolIndexed[AVM_TABLE_HASHSIZE];
    avm_table_bucket* userfuncIndexed[AVM_TABLE_HASHSIZE];
    avm_table_bucket* libfuncIndexed[AVM_TABLE_HASHSIZE];
    avm_table_bucket* tableIndexed[AVM_TABLE_HASHSIZE];
    unsigned total;
}avm_table;

typedef struct avm_memcell {
    avm_memcell_t type;
    union {
        double          numVal;
        char*           strVal;
        unsigned char   boolVal;
        avm_table*      tableVal;
        unsigned        funcVal;
        char*           libfuncVal;
    } data;
} avm_memcell;


avm_memcell* avm_tablegetelem(avm_table*  table,avm_memcell* index){
    assert(table);
    assert(index);
}

void avm_tablesetelem(avm_table* table,avm_memcell* index,avm_memcell* content){
    assert(table);
    assert(index);
    assert(content);
}
