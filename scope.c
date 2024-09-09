#include <stdio.h>
#include <stdlib.h>

#include "scope.h"
#include "hashmap.h"

struct Scope *scope = NULL;

void push_scope(void) {
    struct Scope *sc = (struct Scope *)malloc(sizeof(struct Scope));
    sc->symtab = hashmap_new();

    if (scope) {
        sc->next = scope;
    }

    scope = sc;
}

void pop_scope(void) {
    struct Scope *sc = scope;

    scope = scope->next;
    free(sc);
}

void* find_scope(const char *symbol) {
    struct Scope *sc = scope;

    while (sc) {
        void *entry = hashmap_get(sc->symtab, symbol);
        if (entry) {
            return entry;
        }
        sc = sc->next;
    }

    return NULL;
}

void put_scope(const char *symbol, void *entry) {
    hashmap_put(scope->symtab, symbol, entry);
}