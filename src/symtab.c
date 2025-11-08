#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "symtab.h"

// head of linked list
static Symbol *table_head = NULL;

void symbol_add(char *name) // simple add function
{
    if (symbol_exists(name))
    {
        printf("SEMANTIC ERROR: Duplicate declaration of '%s'\n", name);
        exit(1);
    }

    Symbol *new_symbol = (Symbol *)malloc(sizeof(Symbol));
    new_symbol->name = strdup(name);

    // add to front
    new_symbol->next = table_head;
    table_head = new_symbol;
}

int symbol_exists(char *name) // simple exists function
{
    Symbol *current = table_head;

    while (current != NULL)
    {
        if (strcmp(current->name, name) == 0)
        {
            return 1;
        }
        current = current->next;
    }
    return 0;
}