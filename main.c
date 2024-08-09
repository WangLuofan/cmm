#include <stdio.h>
#include <stdlib.h>

#include "ast.h"
#include "codegen.h"

extern FILE *yyin;
extern int yydebug;

struct ASTNode *prog = NULL;

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "No Input Files\n");
        return -1;
    }

    yyin = fopen(argv[1], "r");
    if (!yyin) {
        fprintf(stderr, "%s Cannot open file\n", argv[1]);
        return -1;
    }
    
    yydebug = 1;

    int ret = yyparse();

    if (yyin) {
        fclose(yyin);
    }

    if (ret != 0) {
        yyerror("Syntax Errror.\n");
        return -1;
    }

    FILE *outfile = fopen("outfile.s", "w+");
    if (!outfile) {
        return -1;
    }

    emit_code(prog, outfile);

    if (outfile) {
        fclose(outfile);
    }

    return 0;
}

void yyerror(const char *msg) {
    fprintf(stderr, msg);
}