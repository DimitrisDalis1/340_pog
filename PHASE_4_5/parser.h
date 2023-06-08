/* A Bison parser, made by GNU Bison 3.7.5.  */

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
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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

#ifndef YY_YY_PARSER_H_INCLUDED
# define YY_YY_PARSER_H_INCLUDED
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
    ID = 258,                      /* ID  */
    STRING = 259,                  /* STRING  */
    INT = 260,                     /* INT  */
    REAL = 261,                    /* REAL  */
    BREAK = 262,                   /* BREAK  */
    CONTINUE = 263,                /* CONTINUE  */
    IF = 264,                      /* IF  */
    ELSE = 265,                    /* ELSE  */
    WHILE = 266,                   /* WHILE  */
    FOR = 267,                     /* FOR  */
    FUNCTION = 268,                /* FUNCTION  */
    RETURN = 269,                  /* RETURN  */
    AND = 270,                     /* AND  */
    NOT = 271,                     /* NOT  */
    OR = 272,                      /* OR  */
    LOCAL = 273,                   /* LOCAL  */
    TRUE = 274,                    /* TRUE  */
    FALSE = 275,                   /* FALSE  */
    NIL = 276,                     /* NIL  */
    ASSIGN = 277,                  /* ASSIGN  */
    PLUS = 278,                    /* PLUS  */
    MINUS = 279,                   /* MINUS  */
    MULT = 280,                    /* MULT  */
    DIV = 281,                     /* DIV  */
    PERC = 282,                    /* PERC  */
    EQUAL = 283,                   /* EQUAL  */
    NOT_EQUAL = 284,               /* NOT_EQUAL  */
    PLUS2 = 285,                   /* PLUS2  */
    MINUS2 = 286,                  /* MINUS2  */
    BIGGER = 287,                  /* BIGGER  */
    SMALLER = 288,                 /* SMALLER  */
    BIGGER_EQUAL = 289,            /* BIGGER_EQUAL  */
    SMALLER_EQUAL = 290,           /* SMALLER_EQUAL  */
    LEFTCURLY = 291,               /* LEFTCURLY  */
    RIGHTCURLY = 292,              /* RIGHTCURLY  */
    LEFTBRACE = 293,               /* LEFTBRACE  */
    RIGHTBRACE = 294,              /* RIGHTBRACE  */
    LEFTPAR = 295,                 /* LEFTPAR  */
    RIGHTPAR = 296,                /* RIGHTPAR  */
    SEMICOLON = 297,               /* SEMICOLON  */
    COMMA = 298,                   /* COMMA  */
    COLON = 299,                   /* COLON  */
    COLON2 = 300,                  /* COLON2  */
    PERIOD = 301,                  /* PERIOD  */
    PERIOD2 = 302,                 /* PERIOD2  */
    UMINUS = 303                   /* UMINUS  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 57 "parser.y"

    unsigned int uns;
    char* stringValue;
    int intValue;
    double doubleValue;
    struct SymbolTableEntry* symbol;
    struct expr* exprNode;
    struct call_t* callNode;
    struct id_list* listId;
    struct stmt_t* stmtNode;
    struct for_t* forNode;


#line 126 "parser.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_H_INCLUDED  */
