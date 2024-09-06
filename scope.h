#ifndef __SCOPE_H__
#define __SCOPE_H__

typedef struct HashMap;
typedef struct Scope {
    struct HashMap *symtab;
    struct Scope *next;
}Scope;

extern struct Scope *scope;

void push_scope(void);

void pop_scope(void);

void put_scope(const char *, void *);

void* find_scope(const char *);

#endif