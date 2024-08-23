%{
    #include "cmm.tab.h"
    #include "ast.h"
%}

%option noyywrap

%%

"int"                                                       { return INT; }
";"                                                         { return SEMICOLON; }
","                                                         { return COMMA; }
"="                                                         { return EQUAL; }

0|[1-9]+[0-9]*                                              { yylval.ivar = atoi(yytext); return NUMBER; }
[_a-zA-Z]+[_0-9a-zA-Z]*                                     { strcpy(yylval.name, yytext); return IDENT; }

<<EOF>>                                                     { yyterminate(); }

%%