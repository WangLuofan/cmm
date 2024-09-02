#include "context.h"
#include "codegen.h"
#include "semantic.h"
#include "cmm.tab.h"

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

extern FILE* yyin;
extern int yydebug;
extern int yyparse(struct Context *);

struct ASTNode;

void yyerror(struct Context *context, const char *msg) {
    fprintf(stderr, "%s: %s\n", context->prog_name, msg);
}

int main(int argc, char **argv) {

    struct Context *context = (struct Context *)malloc(sizeof(struct Context));
    context->prog_name = argv[0];

    if (argc <= 1) {
        yyerror(context, "no input files");
        return -1;
    }

    context->file = argv[1];

    yyin = fopen(argv[1], "r");
    if (!yyin) {
        yyerror(context, "no shuch file or directory");
        return -1;
    }

    yydebug = 1;

    int ret = yyparse(context);
    if (yyin) {
        fclose(yyin);
    }

    if (ret) {
        return -1;
    }

    context->output = fopen("output.s", "w");
    if (!context->output) {
        return -1;
    }

    if (!SEM_prog(context->prog)) {
        emit(context);
    }

    if (context->output) {
        fclose(context->output);
    }

    return 0;
}