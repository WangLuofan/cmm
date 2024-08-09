%{
    #include <stdio.h>

    #include "ast.h"

    extern int yylex(void);
    extern void yyerror(const char *);

    #define YYDEBUG 1

    extern int yydebug;
    extern struct ASTNode *prog;
%}

%union {
    struct ASTNode *node;
}

%token NUMBER
%token INT FLOAT DOUBLE CHAR SHORT LONG
%token IF ELSE DO WHILE SWITCH CASE BREAK FOR RETURN CONST 
    STATIC EXTERN ENUM STRUCT UNION VOID GOTO CONTINUE TYPEDEF REGISTER 
    DEFAULT AUTO
%token SIGNED UNSIGNED

%left LPARAM RPARAM LBRACK RBRACK LBRACE RBRACE POINTTO DOT
%right NOT XOR INC DEC DREF ADDR SIZEOF
%left MUL DIV MOD
%left ADD SUB
%left LSHIFT RSHIFT
%left LT LE GT GE
%left EQ NE
%left BITAND BITXOR BITOR BITNOT
%left LOGAND LOGOR
%right QUES COLON
%right ASSIGN ADDASSIGN SUBASSIGN MULASSIGN DIVASSIGN 
    ANDASSIGN XORASSIGN ORASSIGN LSHIFTASSIGN RSHIFTASSIGN
%left COMMA
%nonassoc UMINUS

%token SYMBOL SEMICOLON

%type <node> program declaration
%start program

%%

program:
    | declaration {
        prog = $1;
    }

declaration: INT SYMBOL ASSIGN NUMBER SEMICOLON {
        $$ = new_declaration();
    }

%%