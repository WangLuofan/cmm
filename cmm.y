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
%token SEMICOLON COMMA EQUAL
%token <ivar> NUMBER 
%token LPARAM RPARAM LBRACE RBRACE LBRACK RBRACK

%type <node> program var_decl variable var_list func_decl
%type <node> func_param func_param_list
%type <node> stmt stmt_list
%type <node> compound_stmt
%type <node> expr call_expr

%start accept

%%

accept: program {
        context->prog = $1;
    }

program: { $$ = NULL; }
    | program var_decl    { $$ = newast_list($1, $2); }
    | program func_decl { $$ = newast_list($1, $2); }

var_decl: INT var_list SEMICOLON {
                $$ = newast_vardecl($1, $2);
            }

variable: IDENT {
        $$ = newast_var($1, NULL, NULL);
    }
    | IDENT EQUAL NUMBER {
        $$ = newast_var($1, NULL, newast_num($3));
    }

var_list: variable {
        $$ = newast_list(NULL, $1);
    }
    | var_list COMMA variable {
        $$ = newast_list($1, $3);
    }

func_decl: VOID IDENT LPARAM VOID RPARAM compound_stmt {
        $$ = newast_function($1, $2, NULL, $6);
    }
    | VOID IDENT LPARAM func_param_list RPARAM compound_stmt {
        $$ = newast_function($1, $2, $4, $6);
    }

func_param: { $$ = NULL; }
    | INT IDENT {
        $$ = newast_var($2, $1, NULL);
    }

func_param_list: func_param {
        $$ = newast_list(NULL, $1);
    }
    | func_param_list COMMA func_param {
        $$ = newast_list($1, $3);
    }

expr: { $$ = NULL; }
    | NUMBER { $$ = newast_num($1); }
    | call_expr {
        $$ = $1;
    } 
    | expr COMMA expr {
        $$ = newast_node(NodeKind_CommaExpr, $1, $3);
    }

stmt: expr SEMICOLON {
        $$ = newast_node(NodeKind_ExprStmt, $1, NULL);
    }
    | var_decl {
        
    }

stmt_list:  { $$ = NULL; }
    | stmt  {
        $$ = newast_list(NULL, $1);
    }
    | stmt_list stmt {
        $$ = newast_list($1, $2);
    }

compound_stmt: LBRACE stmt_list RBRACE {
        if ($2 == NULL) {
            printf("stmt_list is NULL\n");
        }
        $$ = newast_compoundstmt(NULL, $2);
    }

call_expr: IDENT LPARAM expr RPARAM {
        $$ = newast_fncall($1, $3);
    }

%%