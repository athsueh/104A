
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
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


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     ROOT = 258,
     IDENT = 259,
     NUMBER = 260,
     TOK_VOID = 261,
     TOK_BOOL = 262,
     TOK_CHAR = 263,
     TOK_INT = 264,
     TOK_STRING = 265,
     TOK_IF = 266,
     TOK_ELSE = 267,
     TOK_WHILE = 268,
     TOK_RETURN = 269,
     TOK_STRUCT = 270,
     TOK_FALSE = 271,
     TOK_TRUE = 272,
     TOK_NULL = 273,
     TOK_NEW = 274,
     TOK_ARRAY = 275,
     TOK_EQ = 276,
     TOK_NE = 277,
     TOK_LT = 278,
     TOK_LE = 279,
     TOK_GT = 280,
     TOK_GE = 281,
     TOK_IDENT = 282,
     TOK_INTCON = 283,
     TOK_CHARCON = 284,
     TOK_STRINGCON = 285,
     TOK_BLOCK = 286,
     TOK_CALL = 287,
     TOK_IFELSE = 288,
     TOK_INITDECL = 289,
     TOK_POS = 290,
     TOK_NEG = 291,
     TOK_NEWARRAY = 292,
     TOK_TYPEID = 293,
     TOK_FIELD = 294,
     TOK_ORD = 295,
     TOK_CHR = 296,
     TOK_ROOT = 297,
     NEG = 298,
     POS = 299
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


