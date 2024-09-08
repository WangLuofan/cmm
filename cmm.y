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

%left ADD SUB
%left MUL

%token <ty> INT VOID CHAR SHORT LONG RETURN
%token <name> IDENT
%token SEMICOLON COMMA EQUAL
%token <ivar> NUMBER 
%token LPARAM RPARAM LBRACE RBRACE LBRACK RBRACK

%type <ty> type_ident
%type <node> program var_decl variable var_list func_decl
%type <node> func_param func_param_list call_arg_list
%type <node> stmt stmt_list
%type <node> compound_stmt return_stmt
%type <node> expr call_expr

%start accept

%%

accept: program {
        context->prog = $1;
    }

program: { $$ = NULL; }
    | program var_decl    { $$ = newast_list($1, $2); }
    | program func_decl { $$ = newast_list($1, $2); }

type_ident: VOID { $$ = $1; }
    | CHAR { $$ = $1; }
    | INT {$$ = $1; }
    | SHORT { $$ = $1; }
    | LONG { $$ = $1; }

var_decl: type_ident var_list SEMICOLON {
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

func_decl: type_ident IDENT LPARAM VOID RPARAM compound_stmt {
        $$ = newast_function($1, $2, NULL, $6);
    }
    | type_ident IDENT LPARAM func_param_list RPARAM compound_stmt {
        $$ = newast_function($1, $2, $4, $6);
    }

func_param: { $$ = NULL; }
    | type_ident IDENT {
        $$ = newast_var($2, $1, NULL);
    }

func_param_list: func_param {
        $$ = newast_list(NULL, $1);
    }
    | func_param_list COMMA func_param {
        $$ = newast_list($1, $3);
    }

expr: { $$ = NULL; }
    | NUMBER { 
        $$ = newast_num($1); 
    }
    | IDENT {
        $$ = newast_var($1, NULL, NULL);
    }
    | call_expr {
        $$ = $1;
    }
    | expr ADD expr {
        $$ = newast_arith(ArithKind_Add, $1, $3);
    }
    | expr SUB expr {
        $$ = newast_arith(ArithKind_Sub, $1, $3);
    }
    | expr MUL expr {
        $$ = newast_arith(ArithKind_Mul, $1, $3);
    }
    | LPARAM expr RPARAM {
        $$ = $2;
    }

stmt: expr SEMICOLON {
        $$ = newast_node(NodeKind_ExprStmt, $1, NULL);
    }
    | var_decl {
        $$ = $1;
    }
    | return_stmt {
        $$ = $1;
    }

stmt_list:  { $$ = NULL; }
    | stmt  {
        $$ = newast_list(NULL, $1);
    }
    | stmt_list stmt {
        $$ = newast_list($1, $2);
    }

compound_stmt: LBRACE stmt_list RBRACE {
        $$ = newast_compoundstmt($2);
    }

return_stmt: RETURN expr SEMICOLON {
        $$ = newast_node(NodeKind_Return, $2, NULL);
    }

call_arg_list: { $$ = NULL; }
    | expr {
        $$ = newast_list(NULL, $1);
    }
    | call_arg_list COMMA expr {
        $$ = newast_list($1, $3);
    }

call_expr: IDENT LPARAM call_arg_list RPARAM {
        $$ = newast_fncall($1, $3);
    }

%%