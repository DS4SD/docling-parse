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
  IMPORT_SYM = 258,
  IMPORTANT_SYM = 259,
  IDENT = 260,
  STRING = 261,
  NUMBER = 262,
  PERCENTAGE = 263,
  LENGTH = 264,
  EMS = 265,
  EXS = 266,
  LINK_PSCLASS_AFTER_IDENT = 267,
  VISITED_PSCLASS_AFTER_IDENT = 268,
  ACTIVE_PSCLASS_AFTER_IDENT = 269,
  FIRST_LINE_AFTER_IDENT = 270,
  FIRST_LETTER_AFTER_IDENT = 271,
  HASH_AFTER_IDENT = 272,
  CLASS_AFTER_IDENT = 273,
  LINK_PSCLASS = 274,
  VISITED_PSCLASS = 275,
  ACTIVE_PSCLASS = 276,
  FIRST_LINE = 277,
  FIRST_LETTER = 278,
  HASH = 279,
  CLASS = 280,
  URL = 281,
  RGB = 282,
  CDO = 283,
  CDC = 284,
  CSL = 285
};
#endif
/* Tokens.  */
#define IMPORT_SYM 258
#define IMPORTANT_SYM 259
#define IDENT 260
#define STRING 261
#define NUMBER 262
#define PERCENTAGE 263
#define LENGTH 264
#define EMS 265
#define EXS 266
#define LINK_PSCLASS_AFTER_IDENT 267
#define VISITED_PSCLASS_AFTER_IDENT 268
#define ACTIVE_PSCLASS_AFTER_IDENT 269
#define FIRST_LINE_AFTER_IDENT 270
#define FIRST_LETTER_AFTER_IDENT 271
#define HASH_AFTER_IDENT 272
#define CLASS_AFTER_IDENT 273
#define LINK_PSCLASS 274
#define VISITED_PSCLASS 275
#define ACTIVE_PSCLASS 276
#define FIRST_LINE 277
#define FIRST_LETTER 278
#define HASH 279
#define CLASS 280
#define URL 281
#define RGB 282
#define CDO 283
#define CDC 284
#define CSL 285




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 17 "css_syntax.y"
{
  char *lexeme;
  char letter;
  struct property_t *property;
  struct selector_t *selector;
  struct selector_list_t *selector_list;
  int pseudo_class;
  int pseudo_element;
}
/* Line 1529 of yacc.c.  */
#line 119 "css_syntax.h"
  YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif


