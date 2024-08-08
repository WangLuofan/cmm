/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_CMM_TAB_H_INCLUDED
# define YY_YY_CMM_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    NUMBER = 258,                  /* NUMBER  */
    INT = 259,                     /* INT  */
    FLOAT = 260,                   /* FLOAT  */
    DOUBLE = 261,                  /* DOUBLE  */
    CHAR = 262,                    /* CHAR  */
    SHORT = 263,                   /* SHORT  */
    LONG = 264,                    /* LONG  */
    IF = 265,                      /* IF  */
    ELSE = 266,                    /* ELSE  */
    DO = 267,                      /* DO  */
    WHILE = 268,                   /* WHILE  */
    SWITCH = 269,                  /* SWITCH  */
    CASE = 270,                    /* CASE  */
    BREAK = 271,                   /* BREAK  */
    FOR = 272,                     /* FOR  */
    RETURN = 273,                  /* RETURN  */
    CONST = 274,                   /* CONST  */
    STATIC = 275,                  /* STATIC  */
    EXTERN = 276,                  /* EXTERN  */
    ENUM = 277,                    /* ENUM  */
    STRUCT = 278,                  /* STRUCT  */
    UNION = 279,                   /* UNION  */
    VOID = 280,                    /* VOID  */
    GOTO = 281,                    /* GOTO  */
    CONTINUE = 282,                /* CONTINUE  */
    TYPEDEF = 283,                 /* TYPEDEF  */
    REGISTER = 284,                /* REGISTER  */
    DEFAULT = 285,                 /* DEFAULT  */
    AUTO = 286,                    /* AUTO  */
    SIGNED = 287,                  /* SIGNED  */
    UNSIGNED = 288,                /* UNSIGNED  */
    LPARAM = 289,                  /* LPARAM  */
    RPARAM = 290,                  /* RPARAM  */
    LBRACK = 291,                  /* LBRACK  */
    RBRACK = 292,                  /* RBRACK  */
    LBRACE = 293,                  /* LBRACE  */
    RBRACE = 294,                  /* RBRACE  */
    POINTTO = 295,                 /* POINTTO  */
    DOT = 296,                     /* DOT  */
    NOT = 297,                     /* NOT  */
    XOR = 298,                     /* XOR  */
    INC = 299,                     /* INC  */
    DEC = 300,                     /* DEC  */
    DREF = 301,                    /* DREF  */
    ADDR = 302,                    /* ADDR  */
    SIZEOF = 303,                  /* SIZEOF  */
    MUL = 304,                     /* MUL  */
    DIV = 305,                     /* DIV  */
    MOD = 306,                     /* MOD  */
    ADD = 307,                     /* ADD  */
    SUB = 308,                     /* SUB  */
    LSHIFT = 309,                  /* LSHIFT  */
    RSHIFT = 310,                  /* RSHIFT  */
    LT = 311,                      /* LT  */
    LE = 312,                      /* LE  */
    GT = 313,                      /* GT  */
    GE = 314,                      /* GE  */
    EQ = 315,                      /* EQ  */
    NE = 316,                      /* NE  */
    BITAND = 317,                  /* BITAND  */
    BITXOR = 318,                  /* BITXOR  */
    BITOR = 319,                   /* BITOR  */
    BITNOT = 320,                  /* BITNOT  */
    LOGAND = 321,                  /* LOGAND  */
    LOGOR = 322,                   /* LOGOR  */
    QUES = 323,                    /* QUES  */
    COLON = 324,                   /* COLON  */
    ASSIGN = 325,                  /* ASSIGN  */
    ADDASSIGN = 326,               /* ADDASSIGN  */
    SUBASSIGN = 327,               /* SUBASSIGN  */
    MULASSIGN = 328,               /* MULASSIGN  */
    DIVASSIGN = 329,               /* DIVASSIGN  */
    ANDASSIGN = 330,               /* ANDASSIGN  */
    XORASSIGN = 331,               /* XORASSIGN  */
    ORASSIGN = 332,                /* ORASSIGN  */
    LSHIFTASSIGN = 333,            /* LSHIFTASSIGN  */
    RSHIFTASSIGN = 334,            /* RSHIFTASSIGN  */
    COMMA = 335,                   /* COMMA  */
    UMINUS = 336,                  /* UMINUS  */
    SYMBOL = 337,                  /* SYMBOL  */
    SEMICOLON = 338                /* SEMICOLON  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_CMM_TAB_H_INCLUDED  */
