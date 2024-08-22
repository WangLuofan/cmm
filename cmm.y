%{
    #include "ast.h"
    #include "type.h"
    #include "context.h"
    
    extern int yylex(void);
    extern void yyerror(struct Context*, const char *);

    #define YYDEBUG 1
    extern int yydebug;
%}

%parse-param { struct Context *context }

%union {
    struct ASTNode *node;
}

%token INT
%token <node> IDENT
%token SEMICOLON

%type <node> program declaration
%start program

%%

program: declaration    { context->prog = $1; }

declaration: INT IDENT SEMICOLON {
    $$ = newast_decl(ty_int, $2, NULL);
}

%%