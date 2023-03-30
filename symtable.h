#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#define HASH_MULTIPLIER 65599
#define MAX_SIZE 65521

typedef struct symtable_s SymTable_T;
typedef struct id_list id_list;
typedef struct SymbolTableEntry SymbolTableEntry;
typedef struct Scope_node Scope_node;

typedef struct Variable{
    const char *name;
    unsigned int scope;
    unsigned int line;
}Variable;

typedef struct Function{
    const char *name;
    id_list *args;
    unsigned int scope;
    unsigned int line;
}Function;

enum SymbolType{
    GLOBAL,LOCAL,FORMAL,USERFUNC,LIBFUNC
};

struct SymbolTableEntry{
    bool isActive;

    union{
        Variable *varVal;
        Function *funcVal;

    }value;
    enum SymbolType type;
    SymbolTableEntry* next;
    SymbolTableEntry *next_in_scope;
};

typedef struct symtable_s
{
    SymbolTableEntry **hashtable; //why do we have this name 
    unsigned int buckets;
    unsigned int size;
}SymTable_T;

SymTable_T* SymTable_new(void);

void SymTable_free(SymTable_T* oSymTable);

unsigned int SymTable_getLength(SymTable_T* oSymTable);

int SymTable_insert(SymTable_T* , const char *, const unsigned, id_list* , const unsigned, enum SymbolType);

int SymTable_remove(SymTable_T* oSymTable, const char *pcKey);

int SymTable_contains(SymTable_T* oSymTable, const char *pcKey, unsigned int);

int SymTable_get(SymTable_T* oSymTable, const char *pcKey);

typedef struct id_node{
    char *id;
    struct id_node *next;
}id_node;

typedef struct id_list{
    unsigned int length;
    struct id_node *head;
}id_list;



struct Scope_node{
    SymbolTableEntry *symbol;
    unsigned int scope;
    Scope_node *next;
    Scope_node *previous;
};

int id_list_contains(id_list *, const char *);
static unsigned int SymTable_hash(const char*, unsigned int);
static void expand(SymTable_T*);
