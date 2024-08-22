%{
    #include "ast.h"
    #include "type.h"
    
    extern int yylex(void);
    extern void yyerror(const char *);

    #define YYDEBUG 1
    extern int yydebug;
%}

%union {
    char name[512];
    struct ASTNode *node;
}

%token INT
%token <node> IDENT
%token SEMICOLON

%type <node> program declaration
%start program

%%

program: declaration    { $$ = $1; }

declaration: INT IDENT SEMICOLON {
    $$ = newast_decl(ty_int, $2, NULL);
}

%%