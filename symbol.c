#include <stdlib.h>
#include <string.h>

#include "symbol.h"

struct Symbol *new_sym(const char *sym) {
    struct Symbol *symbol = (struct Symbol *)malloc(sizeof(struct Symbol));
    symbol->sym = strdup(sym);
    return symbol;
}