%{
    #include "cmm.tab.h"
%}

%%

"int"           { return INT; }
"float"         { return FLOAT; }
"double"        { return DOUBLE; }
"char"          { return CHAR; }
"short"         { return SHORT; }
"long"          { return LONG; }
"if"            { return IF; }
"else"          { return ELSE; }
"do"            { return DO; }
"while"         { return WHILE; }
"switch"        { return SWITCH; }
"case"          { return CASE; }
"break"         { return BREAK; }
"default"       { return DEFAULT; }
"for"           { return FOR; }
"return"        { return RETURN; }
"const"         { return CONST; }
"static"        { return STATIC; }
"extern"        { return EXTERN; }
"enum"          { return ENUM; }
"struct"        { return STRUCT; }
"union"         { return UNION; }
"void"          { return VOID; }
"goto"          { return GOTO; }
"continue"      { return CONTINUE; }
"typedef"       { return TYPEDEF; }
"register"      { return REGISTER; }
"auto"          { return AUTO; }
"sizeof"        { return SIZEOF; }
"signed"        { return SIGNED; }
"unsigned"      { return UNSIGNED; }

%%