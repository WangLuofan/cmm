%{
    #include "cmm.tab.h"
    #include "ast.h"
%}

%option noyywrap

%%

"int"                                                       { yylval.ty = ty_int; return INT; }
"void"                                                      { yylval.ty = ty_void; return VOID; }
"{"                                                         { return LBRACE; }
"}"                                                         { return RBRACE; }
"("                                                         { return LPARAM; }
")"                                                         { return RPARAM; }
"["                                                         { return LBRACK; }
"]"                                                         { return RBRACK; }
";"                                                         { return SEMICOLON; }
","                                                         { return COMMA; }
"="                                                         { return EQUAL; }

0|[1-9]+[0-9]*                                              { yylval.ivar = atoi(yytext); return NUMBER; }
[_a-zA-Z]+[_0-9a-zA-Z]*                                     { strcpy(yylval.name, yytext); return IDENT; }

<<EOF>>                                                     { yyterminate(); }

%%