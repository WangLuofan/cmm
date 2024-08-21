%{
    extern int yylex(void);
    extern void yyerror(const char *);

    #define YYDEBUG 1
    extern int yydebug;
%}

%token INT

%%

program:

%%