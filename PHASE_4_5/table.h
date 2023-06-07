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

typedef struct avm_table avm_table;

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


avm_memcell* avm_tablegetelem (
            avm_table*  table,
            avm_memcell*    index
);

void avm_tablesetelem(
    avm_table*  table,
    avm_memcell*    index,
    avm_memcell* content
);
