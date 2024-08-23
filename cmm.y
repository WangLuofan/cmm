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
    struct Type *ty;
    struct ASTNode *node;
}

%token <ty> INT VOID
%token <name> IDENT
%token SEMICOLON, COMMA, EQUAL
%token <ivar> NUMBER 
%token LPARAM RPARAM LBRACE RBRACE LBRACK RBRACK

%type <node> program var_decl variable var_list func_decl
%start accept

%%

accept: program {
        context->prog = $1;
    }

program: 
    | program var_decl    { $$ = newast_list($2, $1); }
    | program func_decl { $$ = newast_list($2, $1); }

var_decl: INT var_list SEMICOLON {
                $$ = newast_vardecl($1, $2);
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

func_decl: VOID IDENT LPARAM RPARAM LBRACE RBRACE {
        $$ = newast_function($1, $2, NULL, NULL);
    }
    | VOID IDENT LPARAM VOID RPARAM LBRACE RBRACE {
        $$ = newast_function($1, $2, NULL, NULL);
    }

%%