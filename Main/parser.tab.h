/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

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

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     DECLARE = 258,
     SET = 259,
     PRINT = 260,
     INPUT = 261,
     FROM = 262,
     USER = 263,
     VALUE = 264,
     IF = 265,
     ELSE = 266,
     ELSEIF = 267,
     WHILE = 268,
     FOR = 269,
     STEP = 270,
     THEN = 271,
     ENDIF = 272,
     END = 273,
     STOP = 274,
     DONE = 275,
     TO = 276,
     EQ = 277,
     NE = 278,
     IDENTIFIER = 279,
     NUMBER = 280
   };
#endif
/* Tokens.  */
#define DECLARE 258
#define SET 259
#define PRINT 260
#define INPUT 261
#define FROM 262
#define USER 263
#define VALUE 264
#define IF 265
#define ELSE 266
#define ELSEIF 267
#define WHILE 268
#define FOR 269
#define STEP 270
#define THEN 271
#define ENDIF 272
#define END 273
#define STOP 274
#define DONE 275
#define TO 276
#define EQ 277
#define NE 278
#define IDENTIFIER 279
#define NUMBER 280




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 11 "parser.y"
{
    int num;
    char *str;
    ASTNode *node;
}
/* Line 1529 of yacc.c.  */
#line 105 "parser.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

