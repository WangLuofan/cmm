#include <stdio.h>

extern FILE *yyin;
extern int yydebug;

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
    return 0;
}

void yyerror(const char *msg) {

}