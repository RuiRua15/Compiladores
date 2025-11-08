#ifndef __SYMTAB_H__
#define __SYMTAB_H__

typedef struct Symbol
{
    char *name;
    struct Symbol *next;
} Symbol;

void symbol_add(char *name); /* adds variable name to symbol table*/

int symbol_exists(char *name); /* checks if name is in st, return 1 or 0*/

#endif