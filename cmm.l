%{
    #include "cmm.tab.h"
%}

%option noyywrap

%%

"int"                                                       { return INT; }

%%