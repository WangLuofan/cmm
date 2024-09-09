%{
    #include "cmm.tab.h"
    #include "type.h"
%}

%option noyywrap

%%

"int"                                                       { yylval.ty = ty_int; return INT; }
"void"                                                      { yylval.ty = ty_void; return VOID; }
"char"                                                      { yylval.ty = ty_char; return CHAR; }
"short"                                                     { yylval.ty = ty_short; return SHORT; }
"long"                                                      { yylval.ty = ty_long; return LONG; }
"return"                                                    { return RETURN; }
"{"                                                         { return LBRACE; }
"}"                                                         { return RBRACE; }
"("                                                         { return LPARAM; }
")"                                                         { return RPARAM; }
"["                                                         { return LBRACK; }
"]"                                                         { return RBRACK; }
";"                                                         { return SEMICOLON; }
","                                                         { return COMMA; }
"="                                                         { return EQUAL; }
"+"                                                         { return ADD; }
"-"                                                         { return SUB; }
"*"                                                         { return MUL; }
"/"                                                         { return DIV; }

0|[1-9]+[0-9]*                                              { yylval.ivar = atoi(yytext); return NUMBER; }
[_a-zA-Z]+[_0-9a-zA-Z]*                                     { strcpy(yylval.name, yytext); return IDENT; }

<<EOF>>                                                     { yyterminate(); }

%%