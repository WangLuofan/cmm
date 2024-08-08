%{
    #include <stdio.h>

    extern int yylex(void);
    extern void yyerror(const char *);

    #define YYDEBUG 1

    extern int yydebug;
%}

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

%%

func_decl: ret_type SYMBOL LPARAM RPARAM combond_stmt
    | ret_type SYMBOL LPARAM VOID RPARAM combond_stmt

ret_type: signed builtin_type

builtin_type: VOID
    | CHAR
    | SHORT
    | INT
    | LONG
    | LONG INT
    | LONG LONG
    | LONG LONG INT
    | FLOAT
    | DOUBLE
    | LONG DOUBLE

signed: 
    | SIGNED
    | UNSIGNED

combond_stmt: LBRACE RBRACE

arith_expr: NUMBER
    | SUB NUMBER %prec UMINUS
    | arith_expr MUL arith_expr
    | arith_expr DIV arith_expr
    | arith_expr MOD arith_expr
    | arith_expr ADD arith_expr
    | arith_expr SUB arith_expr

expr: 
    | arith_expr
    | ternary_expr
    | LPARAM arith_expr RPARAM

ternary_expr: expr QUES expr COLON

expr_list: expr SEMICOLON
    | expr expr_list

assign_stmt: builtin_type SYMBOL EQ expr

if_stmt:

while_stmt:

dowhile_stmt:

return_stmt:

switch_stmt:

%%