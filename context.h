#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include <stdio.h>

struct ASTNode;
typedef struct Context {
    struct ASTNode *prog;

    const char *prog_name;
    const char *file;
    FILE *output;
}Context;

#endif