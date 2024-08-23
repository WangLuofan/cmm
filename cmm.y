%{
    #include "ast.h"
    #include "type.h"
    #include "context.h"

    #include <stdint.h>
    
    extern int yylex(void);
    extern void yyerror(struct Context*, const char *);

    #define YYDEBUG 1

    extern int yydebug;
%}

%parse-param { struct Context *context }

%union {
    char name[512];
    unsigned long long ivar;
    struct ASTNode *node;
}

%token INT
%token <name> IDENT
%token SEMICOLON, COMMA, EQUAL
%token <ivar> NUMBER

%type <node> program decl_list variable var_list
%start program

%%

program: decl_list    { context->prog = $1; }

decl_list: INT var_list SEMICOLON {
                $$ = newast_decl(ty_int, $2);
            }

variable: IDENT {
        $$ = newast_var($1, NULL);
    }
    | IDENT EQUAL NUMBER {
        $$ = newast_var($1, newast_num($3));
    }

var_list: variable {
        $$ = newast_list($1, NULL);
    }
    | var_list COMMA variable {
        $$ = newast_list($3, $1);
    }

%%