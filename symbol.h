#ifndef __SYMBOL_H__
#define __SYMBOL_H__

#include "type.h"

typedef struct Symbol {
    char *sym;
    struct Type* ty;

    int offset;
    
    int stack_size;
    struct Symbol *params;
    struct Symbol *locals;

    struct Symbol *next;
}Symbol;

struct Symbol *new_sym(const char *);

#endif