%{
    extern int yylex(void);
    void yyerror(const char *);

    #define YYDEBUG 0

    extern int yydebug;
%}

%token INT FLOAT DOUBLE CHAR SHORT LONG
%token IF ELSE DO WHILE SWITCH CASE BREAK FOR RETURN CONST 
    STATIC EXTERN ENUM STRUCT UNION VOID GOTO CONTINUE TYPEDEF REGISTER 
    DEFAULT AUTO
%token SIZEOF SIGNED UNSIGNED

%%

program:

%%

void yyerror(const char *msg) {

}