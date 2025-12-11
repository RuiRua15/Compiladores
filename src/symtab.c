#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "symtab.h"

// head of linked list
static Symbol *table_head = NULL;

// in MIPS, local variables start at -4 and go down from fp

static int current_stack_offset = 0;

Symbol *symbol_add(char *name, SymbolType type) // simple add function
{
    if (symbol_find(name))
    {
        return symbol_find(name);
    }

    Symbol *new_symbol = (Symbol *)malloc(sizeof(Symbol));
    new_symbol->name = strdup(name);
    new_symbol->type = type;

    // offset
    current_stack_offset -= 4;
    new_symbol->offset = current_stack_offset;

    // add to front
    new_symbol->next = table_head;
    table_head = new_symbol;

    return new_symbol;
}

Symbol *symbol_find(char *name) // simple exists function
{
    Symbol *current = table_head;

    while (current != NULL)
    {
        if (strcmp(current->name, name) == 0)
        {
            return current;
        }
        current = current->next;
    }
    return NULL;
}