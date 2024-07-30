/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Using locations.  */
#define YYLSP_NEEDED 0



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




/* Copy the first part of user declarations.  */
#line 1 "css_syntax.y"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "css_lex.h"
#include "parser.h"

#define _POSIX_SOURCE
#define YYPARSE_PARAM yyparam
#define YYERROR_VERBOSE 1
//#define YYDEBUG 1



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

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
/* Line 193 of yacc.c.  */
#line 180 "css_syntax.c"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

// DFH
int yyerror(char *s);
int yylex (YYSTYPE *);
void init_yylex(const char *buffer, int buf_len);
void end_yylex();

/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 193 "css_syntax.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   157

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  39
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  32
/* YYNRULES -- Number of rules.  */
#define YYNRULES  85
/* YYNRULES -- Number of states.  */
#define YYNSTATES  99

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   285

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    33,    35,    32,     2,    34,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    38,    31,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    36,     2,    37,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     7,    11,    12,    16,    17,    20,    21,
      23,    25,    27,    31,    33,    35,    37,    39,    41,    43,
      44,    46,    51,    53,    57,    59,    63,    66,    69,    71,
      73,    76,    78,    80,    83,    88,    92,    96,   100,   103,
     106,   109,   111,   115,   118,   121,   123,   126,   128,   130,
     132,   134,   136,   138,   140,   142,   144,   146,   148,   150,
     152,   154,   156,   158,   160,   165,   169,   171,   172,   174,
     176,   180,   183,   186,   188,   190,   192,   194,   196,   198,
     200,   202,   204,   206,   208,   210
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      40,     0,    -1,    43,    42,    41,    -1,    50,    43,    41,
      -1,    -1,    45,    43,    42,    -1,    -1,    43,    44,    -1,
      -1,    28,    -1,    29,    -1,    30,    -1,     3,    46,    31,
      -1,     6,    -1,    26,    -1,    32,    -1,    33,    -1,    34,
      -1,    35,    -1,    -1,     5,    -1,    51,    36,    52,    37,
      -1,    53,    -1,    53,    35,    51,    -1,    65,    -1,    65,
      31,    52,    -1,    54,    61,    -1,    54,    62,    -1,    54,
      -1,    62,    -1,    53,     1,    -1,     1,    -1,    55,    -1,
      55,    54,    -1,    56,    63,    59,    57,    -1,    56,    63,
      57,    -1,    56,    59,    57,    -1,    56,    63,    59,    -1,
      56,    63,    -1,    56,    59,    -1,    56,    57,    -1,    56,
      -1,    64,    59,    57,    -1,    64,    59,    -1,    64,    57,
      -1,    64,    -1,    60,    57,    -1,    60,    -1,    58,    -1,
       5,    -1,    12,    -1,    13,    -1,    14,    -1,    19,    -1,
      20,    -1,    21,    -1,    18,    -1,    25,    -1,    16,    -1,
      15,    -1,    23,    -1,    22,    -1,    17,    -1,    24,    -1,
      49,    38,    67,    66,    -1,    49,    38,    67,    -1,     1,
      -1,    -1,     4,    -1,    68,    -1,    67,    48,    68,    -1,
      67,     1,    -1,    47,    69,    -1,    69,    -1,     7,    -1,
       6,    -1,     8,    -1,     9,    -1,    10,    -1,    11,    -1,
       5,    -1,    70,    -1,    26,    -1,    27,    -1,    24,    -1,
      17,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    98,    98,   104,   116,   120,   121,   125,   126,   130,
     131,   132,   136,   140,   141,   165,   166,   170,   171,   172,
     176,   180,   196,   206,   221,   224,   235,   243,   251,   252,
     253,   254,   258,   259,   272,   282,   292,   302,   312,   322,
     332,   342,   352,   362,   372,   382,   392,   402,   412,   425,
     429,   430,   431,   435,   436,   437,   441,   445,   449,   450,
     454,   464,   481,   485,   489,   498,   507,   508,   514,   518,
     519,   529,   533,   541,   545,   546,   547,   548,   549,   550,
     551,   552,   553,   554,   562,   567
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "IMPORT_SYM", "IMPORTANT_SYM", "IDENT",
  "STRING", "NUMBER", "PERCENTAGE", "LENGTH", "EMS", "EXS",
  "LINK_PSCLASS_AFTER_IDENT", "VISITED_PSCLASS_AFTER_IDENT",
  "ACTIVE_PSCLASS_AFTER_IDENT", "FIRST_LINE_AFTER_IDENT",
  "FIRST_LETTER_AFTER_IDENT", "HASH_AFTER_IDENT", "CLASS_AFTER_IDENT",
  "LINK_PSCLASS", "VISITED_PSCLASS", "ACTIVE_PSCLASS", "FIRST_LINE",
  "FIRST_LETTER", "HASH", "CLASS", "URL", "RGB", "CDO", "CDC", "CSL",
  "';'", "'-'", "'+'", "'/'", "','", "'{'", "'}'", "':'", "$accept",
  "stylesheet", "rulesets", "imports", "comments", "comment", "import",
  "string_or_url", "unary_operator", "operator", "property", "ruleset",
  "selectors", "declarations", "selector", "simple_selectors",
  "simple_selector", "element_name", "pseudo_class",
  "solitary_pseudo_class", "class", "solitary_class", "pseudo_element",
  "solitary_pseudo_element", "id", "solitary_id", "declaration", "prio",
  "expr", "term", "value", "hexcolor", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,    59,    45,    43,    47,    44,   123,   125,    58
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    39,    40,    41,    41,    42,    42,    43,    43,    44,
      44,    44,    45,    46,    46,    47,    47,    48,    48,    48,
      49,    50,    51,    51,    52,    52,    53,    53,    53,    53,
      53,    53,    54,    54,    55,    55,    55,    55,    55,    55,
      55,    55,    55,    55,    55,    55,    55,    55,    55,    56,
      57,    57,    57,    58,    58,    58,    59,    60,    61,    61,
      62,    62,    63,    64,    65,    65,    65,    65,    66,    67,
      67,    67,    68,    68,    69,    69,    69,    69,    69,    69,
      69,    69,    69,    69,    70,    70
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     3,     3,     0,     3,     0,     2,     0,     1,
       1,     1,     3,     1,     1,     1,     1,     1,     1,     0,
       1,     4,     1,     3,     1,     3,     2,     2,     1,     1,
       2,     1,     1,     2,     4,     3,     3,     3,     2,     2,
       2,     1,     3,     2,     2,     1,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     4,     3,     1,     0,     1,     1,
       3,     2,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       8,     0,     6,     1,     0,     9,    10,    11,     0,     7,
       8,    13,    14,     0,    31,    49,    53,    54,    55,    61,
      60,    63,    57,     2,     8,     0,     0,    28,    32,    41,
      48,    47,    29,    45,     6,    12,     0,     0,    30,     0,
      59,    58,    26,    27,    33,    50,    51,    52,    62,    56,
      40,    39,    38,    46,    44,    43,     5,     3,    66,    20,
       0,     0,    24,    23,    36,    35,    37,    42,     0,    21,
       0,    34,    80,    75,    74,    76,    77,    78,    79,    85,
      84,    82,    83,    15,    16,     0,     0,    69,    73,    81,
      25,    72,    71,    68,    17,    18,     0,    64,    70
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     1,    23,     8,     2,     9,    10,    13,    85,    96,
      60,    24,    25,    61,    26,    27,    28,    29,    50,    30,
      51,    31,    42,    32,    52,    33,    62,    97,    86,    87,
      88,    89
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -37
static const yytype_int16 yypact[] =
{
     -37,     2,    26,   -37,    -5,   -37,   -37,   -37,    86,   -37,
     -37,   -37,   -37,   -19,   -37,   -37,   -37,   -37,   -37,   -37,
     -37,   -37,   -37,   -37,   -37,   -17,    13,   113,    38,   132,
     -37,    80,   -37,   139,    26,   -37,    45,    10,   -37,    95,
     -37,   -37,   -37,   -37,   -37,   -37,   -37,   -37,   -37,   -37,
     -37,    80,   139,   -37,   -37,    80,   -37,   -37,   -37,   -37,
     -21,   -15,    -7,   -37,   -37,   -37,    80,   -37,    71,   -37,
      10,   -37,   -37,   -37,   -37,   -37,   -37,   -37,   -37,   -37,
     -37,   -37,   -37,   -37,   -37,   116,    -1,   -37,   -37,   -37,
     -37,   -37,   -37,   -37,   -37,   -37,    71,   -37,   -37
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -37,   -37,    -8,     1,     3,   -37,   -37,   -37,   -37,   -37,
     -37,   -37,    -2,   -30,   -37,    16,   -37,   -37,   -13,   -37,
      19,   -37,   -37,    24,   -37,   -37,   -37,   -37,   -37,   -36,
     -24,   -37
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -68
static const yytype_int8 yytable[] =
{
      92,    11,     3,    93,   -19,   -19,   -19,   -19,   -19,   -19,
     -19,    58,    35,    34,    38,    59,   -19,    68,    53,    37,
      54,    12,    69,   -19,    70,   -19,   -19,    36,    57,     4,
     -65,   -19,   -19,    94,    95,    56,   -65,    63,    64,    65,
      90,   -67,    67,    15,    44,    -4,    14,   -67,    39,   -22,
      15,    43,    55,    71,     5,     6,     7,    16,    17,    18,
      98,    91,    21,    22,    16,    17,    18,    19,    20,    21,
      22,    66,     0,     5,     6,     7,    72,    73,    74,    75,
      76,    77,    78,     0,     0,     0,    -4,    14,    79,     0,
       0,    15,    45,    46,    47,    80,    14,    81,    82,     0,
      15,     0,     0,    83,    84,    16,    17,    18,    19,    20,
      21,    22,     0,     0,    16,    17,    18,    19,    20,    21,
      22,    72,    73,    74,    75,    76,    77,    78,    40,    41,
       0,     0,     0,    79,     0,    19,    20,     0,     0,     0,
      80,     0,    81,    82,    45,    46,    47,     0,     0,    48,
      49,    45,    46,    47,     0,     0,     0,    49
};

static const yytype_int8 yycheck[] =
{
       1,     6,     0,     4,     5,     6,     7,     8,     9,    10,
      11,     1,    31,    10,     1,     5,    17,    38,    31,    36,
      33,    26,    37,    24,    31,    26,    27,    24,    36,     3,
      31,    32,    33,    34,    35,    34,    37,    39,    51,    52,
      70,    31,    55,     5,    28,     0,     1,    37,    35,    36,
       5,    27,    33,    66,    28,    29,    30,    19,    20,    21,
      96,    85,    24,    25,    19,    20,    21,    22,    23,    24,
      25,    52,    -1,    28,    29,    30,     5,     6,     7,     8,
       9,    10,    11,    -1,    -1,    -1,     0,     1,    17,    -1,
      -1,     5,    12,    13,    14,    24,     1,    26,    27,    -1,
       5,    -1,    -1,    32,    33,    19,    20,    21,    22,    23,
      24,    25,    -1,    -1,    19,    20,    21,    22,    23,    24,
      25,     5,     6,     7,     8,     9,    10,    11,    15,    16,
      -1,    -1,    -1,    17,    -1,    22,    23,    -1,    -1,    -1,
      24,    -1,    26,    27,    12,    13,    14,    -1,    -1,    17,
      18,    12,    13,    14,    -1,    -1,    -1,    18
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    40,    43,     0,     3,    28,    29,    30,    42,    44,
      45,     6,    26,    46,     1,     5,    19,    20,    21,    22,
      23,    24,    25,    41,    50,    51,    53,    54,    55,    56,
      58,    60,    62,    64,    43,    31,    43,    36,     1,    35,
      15,    16,    61,    62,    54,    12,    13,    14,    17,    18,
      57,    59,    63,    57,    57,    59,    42,    41,     1,     5,
      49,    52,    65,    51,    57,    57,    59,    57,    38,    37,
      31,    57,     5,     6,     7,     8,     9,    10,    11,    17,
      24,    26,    27,    32,    33,    47,    67,    68,    69,    70,
      52,    69,     1,     4,    34,    35,    48,    66,    68
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (&yylval, YYLEX_PARAM)
#else
# define YYLEX yylex (&yylval)
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */






/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  /* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;

  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 98 "css_syntax.y"
    {
						*(struct selector_list_t**) yyparam = (yyvsp[(3) - (3)].selector_list);
					}
    break;

  case 3:
#line 104 "css_syntax.y"
    {
								struct selector_list_t *pos = (yyvsp[(1) - (3)].selector_list);
								if (pos != NULL) {
									while (pos->next != NULL) {
										pos = pos->next;
									}
									pos->next = (yyvsp[(3) - (3)].selector_list);
								} else {
									(yyvsp[(1) - (3)].selector_list) = (yyvsp[(3) - (3)].selector_list);
								}
								(yyval.selector_list) = (yyvsp[(1) - (3)].selector_list);
							}
    break;

  case 4:
#line 116 "css_syntax.y"
    { (yyval.selector_list) = NULL;  }
    break;

  case 13:
#line 140 "css_syntax.y"
    { (yyval.lexeme) = (yyvsp[(1) - (1)].lexeme); }
    break;

  case 14:
#line 141 "css_syntax.y"
    {
			char *begin = (yyvsp[(1) - (1)].lexeme);
			char *end = (yyvsp[(1) - (1)].lexeme) + strlen((yyvsp[(1) - (1)].lexeme));

			/* Skip url( */
			begin += 4;
			/* skip whitespace */
			while (*begin == ' ')
				++begin;

			/* Skip ) */
			end -= 2;
			/* skip whitespace */
			while (*end == ' ')
				--end;

			end[1] = 0;

			(yyval.lexeme) = strdup(begin);
			free((yyvsp[(1) - (1)].lexeme));
		}
    break;

  case 15:
#line 165 "css_syntax.y"
    { (yyval.letter) = '-'; }
    break;

  case 16:
#line 166 "css_syntax.y"
    { (yyval.letter) = '+'; }
    break;

  case 17:
#line 170 "css_syntax.y"
    { (yyval.letter) = '/'; }
    break;

  case 18:
#line 171 "css_syntax.y"
    { (yyval.letter) = ','; }
    break;

  case 19:
#line 172 "css_syntax.y"
    {(yyval.letter) = ' '; }
    break;

  case 20:
#line 176 "css_syntax.y"
    { (yyval.lexeme) = (yyvsp[(1) - (1)].lexeme); }
    break;

  case 21:
#line 180 "css_syntax.y"
    {
										struct selector_list_t *pos = (yyvsp[(1) - (4)].selector_list);
										while (pos != NULL) {
											struct property_t *i = (yyvsp[(3) - (4)].property);
											while (i != NULL) {
												i->count++;
												i = i->next;
											}
											pos->selector->property = (yyvsp[(3) - (4)].property);
											pos = pos->next;
										}
										(yyval.selector_list) = (yyvsp[(1) - (4)].selector_list);
									}
    break;

  case 22:
#line 196 "css_syntax.y"
    {
				if ((yyvsp[(1) - (1)].selector) != NULL) {
					(yyval.selector_list) = (struct selector_list_t*)
						malloc (sizeof(struct selector_list_t));
					(yyval.selector_list)->selector = (yyvsp[(1) - (1)].selector);
					(yyval.selector_list)->next = NULL;
				} else {
					(yyval.selector_list) = NULL;
				}
			}
    break;

  case 23:
#line 206 "css_syntax.y"
    {
								if ((yyvsp[(1) - (3)].selector) != NULL) {
									struct selector_list_t *new;
									new = (struct selector_list_t*)
										malloc (sizeof(struct selector_list_t));
									new->selector = (yyvsp[(1) - (3)].selector);
									new->next = (yyvsp[(3) - (3)].selector_list);
									(yyval.selector_list) = new;
								} else {
									(yyval.selector_list) = (yyvsp[(3) - (3)].selector_list);
								}
							}
    break;

  case 24:
#line 221 "css_syntax.y"
    {
									(yyval.property) = (yyvsp[(1) - (1)].property);
								}
    break;

  case 25:
#line 224 "css_syntax.y"
    {
									if ((yyvsp[(1) - (3)].property) != NULL) {
										(yyvsp[(1) - (3)].property)->next = (yyvsp[(3) - (3)].property);
										(yyval.property) = (yyvsp[(1) - (3)].property);
									} else {
										(yyval.property) = (yyvsp[(3) - (3)].property);
									}
								}
    break;

  case 26:
#line 235 "css_syntax.y"
    {
										struct selector_t *pos = (yyvsp[(1) - (2)].selector);
										while (pos->next != NULL) {
											pos = pos->next;
										}
										pos->pseudo_element = (yyvsp[(2) - (2)].pseudo_element);
										(yyval.selector) = (yyvsp[(1) - (2)].selector);
									}
    break;

  case 27:
#line 243 "css_syntax.y"
    {
												struct selector_t *pos = (yyvsp[(1) - (2)].selector);
												while (pos->next != NULL) {
													pos = pos->next;
												}
												pos->next = (yyvsp[(2) - (2)].selector);
												(yyval.selector) = (yyvsp[(1) - (2)].selector);
											}
    break;

  case 28:
#line 251 "css_syntax.y"
    { (yyval.selector) = (yyvsp[(1) - (1)].selector); }
    break;

  case 29:
#line 252 "css_syntax.y"
    { (yyval.selector) = (yyvsp[(1) - (1)].selector); }
    break;

  case 30:
#line 253 "css_syntax.y"
    { (yyval.selector) = NULL; }
    break;

  case 31:
#line 254 "css_syntax.y"
    { (yyval.selector) = NULL; }
    break;

  case 32:
#line 258 "css_syntax.y"
    { (yyval.selector) = (yyvsp[(1) - (1)].selector); }
    break;

  case 33:
#line 259 "css_syntax.y"
    {
										(yyvsp[(1) - (2)].selector)->next = (yyvsp[(2) - (2)].selector);
										(yyval.selector) = (yyvsp[(1) - (2)].selector);
									}
    break;

  case 34:
#line 272 "css_syntax.y"
    {
										(yyval.selector) = (struct selector_t*)
											malloc(sizeof(struct selector_t));
										(yyval.selector)->element_name = (yyvsp[(1) - (4)].lexeme);
										(yyval.selector)->id = (yyvsp[(2) - (4)].lexeme);
										(yyval.selector)->e_class = (yyvsp[(3) - (4)].lexeme);
										(yyval.selector)->pseudo_class = (yyvsp[(4) - (4)].pseudo_class);
										(yyval.selector)->pseudo_element = 0;
										(yyval.selector)->next = NULL;
									 }
    break;

  case 35:
#line 282 "css_syntax.y"
    {
									(yyval.selector) = (struct selector_t*)
										malloc(sizeof(struct selector_t));
									(yyval.selector)->element_name = (yyvsp[(1) - (3)].lexeme);
									(yyval.selector)->id = (yyvsp[(2) - (3)].lexeme);
									(yyval.selector)->e_class = NULL;
									(yyval.selector)->pseudo_class = (yyvsp[(3) - (3)].pseudo_class);
									(yyval.selector)->pseudo_element = 0;
									(yyval.selector)->next = NULL;
								}
    break;

  case 36:
#line 292 "css_syntax.y"
    {
										(yyval.selector) = (struct selector_t*)
											malloc(sizeof(struct selector_t));
										(yyval.selector)->element_name = (yyvsp[(1) - (3)].lexeme);
										(yyval.selector)->id = NULL;
										(yyval.selector)->e_class = (yyvsp[(2) - (3)].lexeme);
										(yyval.selector)->pseudo_class = (yyvsp[(3) - (3)].pseudo_class);
										(yyval.selector)->pseudo_element = 0;
										(yyval.selector)->next = NULL;
									}
    break;

  case 37:
#line 302 "css_syntax.y"
    {
							(yyval.selector) = (struct selector_t*)
								malloc(sizeof(struct selector_t));
							(yyval.selector)->element_name = (yyvsp[(1) - (3)].lexeme);
							(yyval.selector)->id = (yyvsp[(2) - (3)].lexeme);
							(yyval.selector)->e_class = (yyvsp[(3) - (3)].lexeme);
							(yyval.selector)->pseudo_class = 0;
							(yyval.selector)->pseudo_element = 0;
							(yyval.selector)->next = NULL;
						}
    break;

  case 38:
#line 312 "css_syntax.y"
    {
						(yyval.selector) = (struct selector_t*)
							malloc(sizeof(struct selector_t));
						(yyval.selector)->element_name = (yyvsp[(1) - (2)].lexeme);
						(yyval.selector)->id = (yyvsp[(2) - (2)].lexeme);
						(yyval.selector)->e_class = NULL;
						(yyval.selector)->pseudo_class = 0;
						(yyval.selector)->pseudo_element = 0;
						(yyval.selector)->next = NULL;
					}
    break;

  case 39:
#line 322 "css_syntax.y"
    {
							(yyval.selector) = (struct selector_t*)
								malloc(sizeof(struct selector_t));
							(yyval.selector)->element_name = (yyvsp[(1) - (2)].lexeme);
							(yyval.selector)->id = NULL;
							(yyval.selector)->e_class = (yyvsp[(2) - (2)].lexeme);
							(yyval.selector)->pseudo_class = 0;
							(yyval.selector)->pseudo_element = 0;
							(yyval.selector)->next = NULL;
						}
    break;

  case 40:
#line 332 "css_syntax.y"
    {
								(yyval.selector) = (struct selector_t*)
									malloc(sizeof(struct selector_t));
								(yyval.selector)->element_name = (yyvsp[(1) - (2)].lexeme);
								(yyval.selector)->id = NULL;
								(yyval.selector)->e_class = NULL;
								(yyval.selector)->pseudo_class = (yyvsp[(2) - (2)].pseudo_class);
								(yyval.selector)->pseudo_element = 0;
								(yyval.selector)->next = NULL;
							}
    break;

  case 41:
#line 342 "css_syntax.y"
    {
					(yyval.selector) = (struct selector_t*)
						malloc(sizeof(struct selector_t));
					(yyval.selector)->element_name = (yyvsp[(1) - (1)].lexeme);
					(yyval.selector)->id = NULL;
					(yyval.selector)->e_class = NULL;
					(yyval.selector)->pseudo_class = 0;
					(yyval.selector)->pseudo_element = 0;
					(yyval.selector)->next = NULL;
				}
    break;

  case 42:
#line 352 "css_syntax.y"
    {
										(yyval.selector) = (struct selector_t*)
											malloc(sizeof(struct selector_t));
										(yyval.selector)->element_name = NULL;
										(yyval.selector)->id = (yyvsp[(1) - (3)].lexeme);
										(yyval.selector)->e_class = (yyvsp[(2) - (3)].lexeme);
										(yyval.selector)->pseudo_class = (yyvsp[(3) - (3)].pseudo_class);
										(yyval.selector)->pseudo_element = 0;
										(yyval.selector)->next = NULL;
									}
    break;

  case 43:
#line 362 "css_syntax.y"
    {
						(yyval.selector) = (struct selector_t*)
							malloc(sizeof(struct selector_t));
						(yyval.selector)->element_name = NULL;
						(yyval.selector)->id = (yyvsp[(1) - (2)].lexeme);
						(yyval.selector)->e_class = (yyvsp[(2) - (2)].lexeme);
						(yyval.selector)->pseudo_class = 0;
						(yyval.selector)->pseudo_element = 0;
						(yyval.selector)->next = NULL;
					}
    break;

  case 44:
#line 372 "css_syntax.y"
    {
								(yyval.selector) = (struct selector_t*)
									malloc(sizeof(struct selector_t));
								(yyval.selector)->element_name = NULL;
								(yyval.selector)->id = (yyvsp[(1) - (2)].lexeme);
								(yyval.selector)->e_class = NULL;
								(yyval.selector)->pseudo_class = (yyvsp[(2) - (2)].pseudo_class);
								(yyval.selector)->pseudo_element = 0;
								(yyval.selector)->next = NULL;
							}
    break;

  case 45:
#line 382 "css_syntax.y"
    {
					(yyval.selector) = (struct selector_t*)
						malloc(sizeof(struct selector_t));
					(yyval.selector)->element_name = NULL;
					(yyval.selector)->id = (yyvsp[(1) - (1)].lexeme);
					(yyval.selector)->e_class = NULL;
					(yyval.selector)->pseudo_class = 0;
					(yyval.selector)->pseudo_element = 0;
					(yyval.selector)->next = NULL;
				}
    break;

  case 46:
#line 392 "css_syntax.y"
    {
									(yyval.selector) = (struct selector_t*)
										malloc(sizeof(struct selector_t));
									(yyval.selector)->element_name = NULL;
									(yyval.selector)->id = NULL;
									(yyval.selector)->e_class = (yyvsp[(1) - (2)].lexeme);
									(yyval.selector)->pseudo_class = (yyvsp[(2) - (2)].pseudo_class);
									(yyval.selector)->pseudo_element = 0;
									(yyval.selector)->next = NULL;
								}
    break;

  case 47:
#line 402 "css_syntax.y"
    {
						(yyval.selector) = (struct selector_t*)
							malloc(sizeof(struct selector_t));
						(yyval.selector)->element_name = NULL;
						(yyval.selector)->id = NULL;
						(yyval.selector)->e_class = (yyvsp[(1) - (1)].lexeme);
						(yyval.selector)->pseudo_class = 0;
						(yyval.selector)->pseudo_element = 0;
						(yyval.selector)->next = NULL;
					}
    break;

  case 48:
#line 412 "css_syntax.y"
    {
							(yyval.selector) = (struct selector_t*)
								malloc(sizeof(struct selector_t));
							(yyval.selector)->element_name = NULL;
							(yyval.selector)->id = NULL;
							(yyval.selector)->e_class = NULL;
							(yyval.selector)->pseudo_class = (yyvsp[(1) - (1)].pseudo_class);
							(yyval.selector)->pseudo_element = 0;
							(yyval.selector)->next = NULL;
						}
    break;

  case 49:
#line 425 "css_syntax.y"
    { (yyval.lexeme) = (yyvsp[(1) - (1)].lexeme); }
    break;

  case 50:
#line 429 "css_syntax.y"
    { (yyval.pseudo_class) = PS_CLASS_LINK; }
    break;

  case 51:
#line 430 "css_syntax.y"
    { (yyval.pseudo_class) = PS_CLASS_VISITED; }
    break;

  case 52:
#line 431 "css_syntax.y"
    { (yyval.pseudo_class) = PS_CLASS_ACTIVE; }
    break;

  case 53:
#line 435 "css_syntax.y"
    { (yyval.pseudo_class) = PS_CLASS_LINK; }
    break;

  case 54:
#line 436 "css_syntax.y"
    { (yyval.pseudo_class) = PS_CLASS_VISITED; }
    break;

  case 55:
#line 437 "css_syntax.y"
    { (yyval.pseudo_class) = PS_CLASS_ACTIVE; }
    break;

  case 56:
#line 441 "css_syntax.y"
    { (yyval.lexeme) = (yyvsp[(1) - (1)].lexeme); }
    break;

  case 57:
#line 445 "css_syntax.y"
    { (yyval.lexeme) = (yyvsp[(1) - (1)].lexeme); }
    break;

  case 58:
#line 449 "css_syntax.y"
    { (yyval.pseudo_element) = PS_ELEMENT_FIRST_LETTER; }
    break;

  case 59:
#line 450 "css_syntax.y"
    { (yyval.pseudo_element) = PS_ELEMENT_FIRST_LINE; }
    break;

  case 60:
#line 454 "css_syntax.y"
    {
					(yyval.selector) = (struct selector_t*)
						malloc(sizeof(struct selector_t));
					(yyval.selector)->element_name = NULL;
					(yyval.selector)->id = NULL;
					(yyval.selector)->e_class = NULL;
					(yyval.selector)->pseudo_class = 0;
					(yyval.selector)->pseudo_element = PS_ELEMENT_FIRST_LETTER;
					(yyval.selector)->next = NULL;
				}
    break;

  case 61:
#line 464 "css_syntax.y"
    {
					(yyval.selector) = (struct selector_t*)
						malloc(sizeof(struct selector_t));
					(yyval.selector)->element_name = NULL;
					(yyval.selector)->id = NULL;
					(yyval.selector)->e_class = NULL;
					(yyval.selector)->pseudo_class = 0;
					(yyval.selector)->pseudo_element = PS_ELEMENT_FIRST_LINE;
					(yyval.selector)->next = NULL;
				}
    break;

  case 62:
#line 481 "css_syntax.y"
    { (yyval.lexeme) = (yyvsp[(1) - (1)].lexeme); }
    break;

  case 63:
#line 485 "css_syntax.y"
    { (yyval.lexeme) = (yyvsp[(1) - (1)].lexeme); }
    break;

  case 64:
#line 489 "css_syntax.y"
    {
								(yyval.property) = (struct property_t*)
									malloc(sizeof(struct property_t));
								(yyval.property)->name = (yyvsp[(1) - (4)].lexeme);
								(yyval.property)->val = (yyvsp[(3) - (4)].lexeme);
								(yyval.property)->important = 1;
								(yyval.property)->count = 0;
								(yyval.property)->next = NULL;
							}
    break;

  case 65:
#line 498 "css_syntax.y"
    {
								(yyval.property) = (struct property_t*)
									malloc(sizeof(struct property_t));
								(yyval.property)->name = (yyvsp[(1) - (3)].lexeme);
								(yyval.property)->val = (yyvsp[(3) - (3)].lexeme);
								(yyval.property)->important = 0;
								(yyval.property)->count = 0;
								(yyval.property)->next = NULL;
							}
    break;

  case 66:
#line 507 "css_syntax.y"
    { (yyval.property) = NULL; }
    break;

  case 67:
#line 508 "css_syntax.y"
    {
								(yyval.property) = NULL;
							}
    break;

  case 68:
#line 514 "css_syntax.y"
    { }
    break;

  case 69:
#line 518 "css_syntax.y"
    { (yyval.lexeme) = (yyvsp[(1) - (1)].lexeme); }
    break;

  case 70:
#line 519 "css_syntax.y"
    {
							char *s = (char*) malloc (strlen((yyvsp[(1) - (3)].lexeme))+strlen((yyvsp[(3) - (3)].lexeme))+2);
							strcpy(s, (yyvsp[(1) - (3)].lexeme));
							s[strlen(s)+1] = 0;
							s[strlen(s)] = (yyvsp[(2) - (3)].letter);
							strcat(s, (yyvsp[(3) - (3)].lexeme));
							free((yyvsp[(1) - (3)].lexeme));
							free((yyvsp[(3) - (3)].lexeme));
							(yyval.lexeme) = s;
						}
    break;

  case 71:
#line 529 "css_syntax.y"
    { (yyval.lexeme) = (yyvsp[(1) - (2)].lexeme); }
    break;

  case 72:
#line 533 "css_syntax.y"
    {
							char *s = (char*) malloc(strlen((yyvsp[(2) - (2)].lexeme))+2);
							s[0] = (yyvsp[(1) - (2)].letter);
							s[1] = 0;
							strcat(s, (yyvsp[(2) - (2)].lexeme));
							free((yyvsp[(2) - (2)].lexeme));
							(yyval.lexeme) = s;
						}
    break;

  case 73:
#line 541 "css_syntax.y"
    { (yyval.lexeme) = (yyvsp[(1) - (1)].lexeme); }
    break;

  case 74:
#line 545 "css_syntax.y"
    { (yyval.lexeme) = (yyvsp[(1) - (1)].lexeme); }
    break;

  case 75:
#line 546 "css_syntax.y"
    { (yyval.lexeme) = (yyvsp[(1) - (1)].lexeme); }
    break;

  case 76:
#line 547 "css_syntax.y"
    { (yyval.lexeme) = (yyvsp[(1) - (1)].lexeme); }
    break;

  case 77:
#line 548 "css_syntax.y"
    { (yyval.lexeme) = (yyvsp[(1) - (1)].lexeme); }
    break;

  case 78:
#line 549 "css_syntax.y"
    { (yyval.lexeme) = (yyvsp[(1) - (1)].lexeme); }
    break;

  case 79:
#line 550 "css_syntax.y"
    { (yyval.lexeme) = (yyvsp[(1) - (1)].lexeme); }
    break;

  case 80:
#line 551 "css_syntax.y"
    { (yyval.lexeme) = (yyvsp[(1) - (1)].lexeme); }
    break;

  case 81:
#line 552 "css_syntax.y"
    { (yyval.lexeme) = (yyvsp[(1) - (1)].lexeme); }
    break;

  case 82:
#line 553 "css_syntax.y"
    { (yyval.lexeme) = (yyvsp[(1) - (1)].lexeme); }
    break;

  case 83:
#line 554 "css_syntax.y"
    { (yyval.lexeme) = (yyvsp[(1) - (1)].lexeme); }
    break;

  case 84:
#line 562 "css_syntax.y"
    { 
			(yyval.lexeme) = (char*) malloc (strlen((yyvsp[(1) - (1)].lexeme))+2);
			sprintf((yyval.lexeme), "#%s", (yyvsp[(1) - (1)].lexeme));
			free((yyvsp[(1) - (1)].lexeme));
		}
    break;

  case 85:
#line 567 "css_syntax.y"
    { 
						(yyval.lexeme) = (char*) malloc (strlen((yyvsp[(1) - (1)].lexeme))+2);
						sprintf((yyval.lexeme), "#%s", (yyvsp[(1) - (1)].lexeme));
						free((yyvsp[(1) - (1)].lexeme));
					}
    break;


/* Line 1267 of yacc.c.  */
#line 2173 "css_syntax.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


#line 574 "css_syntax.y"


int yyerror(char *s) {
#if YYDEBUG
	fprintf(stderr, "Error: %s\n", s);
#endif
	return 0;
}

struct selector_list_t* css_parse(const char *buffer, int buf_len) {
	struct selector_list_t *ret = NULL;
	//yydebug = 1;
	init_yylex(buffer, buf_len);
	yyparse(&ret);
	end_yylex();
	return ret;
}

