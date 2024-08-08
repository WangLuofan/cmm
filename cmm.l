%{
    #include "cmm.tab.h"
    #include "cmm-def.h"
%}

%%

"int"                                                                       { return INT; }
"float"                                                                     { return FLOAT; }
"double"                                                                    { return DOUBLE; }
"char"                                                                      { return CHAR; }
"short"                                                                     { return SHORT; }
"long"                                                                      { return LONG; }
"if"                                                                        { return IF; }
"else"                                                                      { return ELSE; }
"do"                                                                        { return DO; }
"while"                                                                     { return WHILE; }
"switch"                                                                    { return SWITCH; }
"case"                                                                      { return CASE; }
"break"                                                                     { return BREAK; }
"default"                                                                   { return DEFAULT; }
"for"                                                                       { return FOR; }
"return"                                                                    { return RETURN; }
"const"                                                                     { return CONST; }
"static"                                                                    { return STATIC; }
"extern"                                                                    { return EXTERN; }
"enum"                                                                      { return ENUM; }
"struct"                                                                    { return STRUCT; }
"union"                                                                     { return UNION; }
"void"                                                                      { return VOID; }
"goto"                                                                      { return GOTO; }
"continue"                                                                  { return CONTINUE; }
"typedef"                                                                   { return TYPEDEF; }
"register"                                                                  { return REGISTER; }
"auto"                                                                      { return AUTO; }
"sizeof"                                                                    { return SIZEOF; }
"signed"                                                                    { return SIGNED; }
"unsigned"                                                                  { return UNSIGNED; }
"("                                                                         { return LPARAM; }
")"                                                                         { return RPARAM; }
"["                                                                         { return LBRACK; }
"]"                                                                         { return RBRACK; }
"{"                                                                         { return LBRACE; }
"}"                                                                         { return RBRACE; }
"."                                                                         { return DOT; }
"!"                                                                         { return NOT; }
"^"                                                                         { return XOR; }
"++"                                                                        { return INC; }
"->"                                                                        { return POINTTO; }
"--"                                                                        { return DEC; }
"&"                                                                         { return ADDR; }
"*"                                                                         { return MUL; }
"/"                                                                         { return DIV;}
"%"                                                                         { return MOD; }
"+"                                                                         { return ADD; }
"-"                                                                         { return SUB; }
"<<"                                                                        { return LSHIFT; }
">>"                                                                        { return RSHIFT; }
"<"                                                                         { return LT; }
"<="                                                                        { return LE; }
">"                                                                         { return GT; }
">="                                                                        { return GE; }
"=="                                                                        { return EQ; }
"!="                                                                        { return NE; }
"|"                                                                         { return BITOR; }
"+="                                                                        { return ADDASSIGN; }
"-="                                                                        { return SUBASSIGN; }
"*="                                                                        { return MULASSIGN; }
"/="                                                                        { return DIVASSIGN; }
"&="                                                                        { return ANDASSIGN; }
"|="                                                                        { return ORASSIGN;  }
"^="                                                                        { return XORASSIGN; }
"?"                                                                         { return QUES; }
":"                                                                         { return COLON; }
"&&"                                                                        { return LOGAND; }
"||"                                                                        { return LOGOR; }
"<<="                                                                       { return LSHIFTASSIGN; }
">>="                                                                       { return RSHIFTASSIGN; }
","                                                                         { return COMMA; }
"~"                                                                         { return BITNOT; }
";"                                                                         { return SEMICOLON; }

0[1-7][0-7]+                                                                { return INT; }
0|[1-9][0-9]+                                                               { return INT; }
0[b|B][0|1]+                                                                { return INT; }
0[x|X][0-9a-fA-F]+                                                          { return INT; }

_+[0-9]+[a-zA-Z0-9]+|_*[a-zA-Z_]+[0-9a-zA-Z_]+                              { return SYMBOL; }

%%