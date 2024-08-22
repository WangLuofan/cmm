%{
    #include "cmm.tab.h"
    #include "ast.h"
%}

%option noyywrap

%%

"int"                                                       { return INT; }
";"                                                         { return SEMICOLON; }

[_a-zA-Z]+[_0-9a-zA-Z]?                                     { yylval.node = newast_var(yytext); return IDENT; }

<<EOF>>                                                     { yyterminate(); }

%%