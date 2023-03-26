# 340_pog
##################
CURRENT_WARNINGS:
##################
symtablehash.c: In function 'SymTable_insert':
symtablehash.c:181:46: warning: assignment from incompatible pointer type [-Wincompatible-pointer-types]
             temp_head->symbol->next_in_scope = temp;
                                              ^
symtablehash.c: In function 'SymTable_new':
symtablehash.c:290:12: warning: return from incompatible pointer type [-Wincompatible-pointer-types]
     return oSymTable;
            ^~~~~~~~~
c:/mingw/bin/../lib/gcc/mingw32/6.3.0/../../../libmingw32.a(main.o):(.text.startup+0xa0): undefined reference to `WinMain@16'
collect2.exe: error: ld returned 1 exit status
