avm_table* avm_tablenew (void);
void avm_tabledestroy (avm_table* t);
avm_memcell* avm_tablegetelem (avm_memcell* key);
void avm_tablesetelem (avm_memcell* key, avm_memcell* value);
void avm_tableincrefcounter (avm_table* );
void avm_tabledecrefcounter (avm_table* );
void avm_tablebucketsinit (avm_table_bucket** );
avm_table* avm_tablenew(void);
void avm_memcellclear(avm_memcell* m);
void avm_tablebucketsdestroy (avm_table_bucket**);


#define AVM_TABLE_HASHSIZE 211

typedef struct avm_table_bucket;
typedef struct avm_table;



typedef struct avm_table_bucket
{
    avm_memcell key;
    avm_memcell value;
    avm_table_bucket* next;
}avm_table_bucket;

typedef struct avm_table
{
    unsigned refCounter;
    avm_table_bucket* strIndexed[AVM_TABLE_HASHSIZE];
    avm_table_bucket* numIndexed[AVM_TABLE_HASHSIZE];
    unsigned total;
}avm_table;
