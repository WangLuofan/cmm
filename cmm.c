#include "cmm.tab.h"

#include <stdio.h>

extern FILE* yyin;
extern int yyparse(void);

void yyerror(const char *msg) {
  fprintf(stderr, "%s\n", msg);
}

int main(int argc, char **argv) {

  if (argc <= 1) {
    yyerror("no input files");
    return -1;
  }

  yyin = fopen(argv[1], "r");
  if (!yyin) {
    yyerror("no shuch file or directory");
    return -1;
  }

  int ret = yyparse();
  if (yyin) {
    fclose(yyin);
  }

  return 0;
}