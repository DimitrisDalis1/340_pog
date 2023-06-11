/* A Bison parser, made by GNU Bison 3.7.5.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30705

/* Bison version string.  */
#define YYBISON_VERSION "3.7.5"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 2 "parser.y"

    #include <stdio.h>
    #include "quadhandler.h"
    #include "target_producer.h"
    #include "avm.h"
    #define for_each_time(item, list) \
	for(T * item = list->head; item != NULL; item = item->next)
    int yyerror (char* yaccProvidedMessage);
    int yylex(void);
    extern Scope_node *head_scope_node;
    extern SymTable_T* hash ;
    extern int current_scope;
    extern int yylineno;
    extern char* yytext;
    extern FILE* yyin;
    FILE* yyout_y; 
    int scope=0;
    int block_count = 0;
    bool isFunct = false;
    bool isError=false;
    int function_scope_count = 0;
    int unnamed_counter=0;
    int global_check = 0;
    int sim_loops = 0; //simultaneous loops active
    int sim_funcs = 0;
    int currQuad=0;
    int offset=-1;
	int program_offset=0;
    int loopcounter=0;
	int offset_= -1; 
    //extern unsigned functionLocalOffset;
    //extern unsigned formalArgOffset;
    extern int tempcounter;
	extern unsigned char   executionFinished;
    extern unsigned formalArgOffset;  
        int b_n_b = 0; //before loop
    int b_a = 0;   //after loop
    int b_n_bf = 0; //before func
    int b_af = 0; //after func
    bool c_f = false;
    bool isFunct1=false;
    bool call_flag = false;

    int isFunc_loop = 0; //exoume function call mesa se active loopa

    bool called_from_func = false;
	stack* stack_;
	stack* func_stack;
	stack* loopcounterstack;


#line 123 "parser.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "parser.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_ID = 3,                         /* ID  */
  YYSYMBOL_STRING = 4,                     /* STRING  */
  YYSYMBOL_INT = 5,                        /* INT  */
  YYSYMBOL_REAL = 6,                       /* REAL  */
  YYSYMBOL_BREAK = 7,                      /* BREAK  */
  YYSYMBOL_CONTINUE = 8,                   /* CONTINUE  */
  YYSYMBOL_IF = 9,                         /* IF  */
  YYSYMBOL_ELSE = 10,                      /* ELSE  */
  YYSYMBOL_WHILE = 11,                     /* WHILE  */
  YYSYMBOL_FOR = 12,                       /* FOR  */
  YYSYMBOL_FUNCTION = 13,                  /* FUNCTION  */
  YYSYMBOL_RETURN = 14,                    /* RETURN  */
  YYSYMBOL_AND = 15,                       /* AND  */
  YYSYMBOL_NOT = 16,                       /* NOT  */
  YYSYMBOL_OR = 17,                        /* OR  */
  YYSYMBOL_LOCAL = 18,                     /* LOCAL  */
  YYSYMBOL_TRUE = 19,                      /* TRUE  */
  YYSYMBOL_FALSE = 20,                     /* FALSE  */
  YYSYMBOL_NIL = 21,                       /* NIL  */
  YYSYMBOL_ASSIGN = 22,                    /* ASSIGN  */
  YYSYMBOL_PLUS = 23,                      /* PLUS  */
  YYSYMBOL_MINUS = 24,                     /* MINUS  */
  YYSYMBOL_MULT = 25,                      /* MULT  */
  YYSYMBOL_DIV = 26,                       /* DIV  */
  YYSYMBOL_PERC = 27,                      /* PERC  */
  YYSYMBOL_EQUAL = 28,                     /* EQUAL  */
  YYSYMBOL_NOT_EQUAL = 29,                 /* NOT_EQUAL  */
  YYSYMBOL_PLUS2 = 30,                     /* PLUS2  */
  YYSYMBOL_MINUS2 = 31,                    /* MINUS2  */
  YYSYMBOL_BIGGER = 32,                    /* BIGGER  */
  YYSYMBOL_SMALLER = 33,                   /* SMALLER  */
  YYSYMBOL_BIGGER_EQUAL = 34,              /* BIGGER_EQUAL  */
  YYSYMBOL_SMALLER_EQUAL = 35,             /* SMALLER_EQUAL  */
  YYSYMBOL_LEFTCURLY = 36,                 /* LEFTCURLY  */
  YYSYMBOL_RIGHTCURLY = 37,                /* RIGHTCURLY  */
  YYSYMBOL_LEFTBRACE = 38,                 /* LEFTBRACE  */
  YYSYMBOL_RIGHTBRACE = 39,                /* RIGHTBRACE  */
  YYSYMBOL_LEFTPAR = 40,                   /* LEFTPAR  */
  YYSYMBOL_RIGHTPAR = 41,                  /* RIGHTPAR  */
  YYSYMBOL_SEMICOLON = 42,                 /* SEMICOLON  */
  YYSYMBOL_COMMA = 43,                     /* COMMA  */
  YYSYMBOL_COLON = 44,                     /* COLON  */
  YYSYMBOL_COLON2 = 45,                    /* COLON2  */
  YYSYMBOL_PERIOD = 46,                    /* PERIOD  */
  YYSYMBOL_PERIOD2 = 47,                   /* PERIOD2  */
  YYSYMBOL_UMINUS = 48,                    /* UMINUS  */
  YYSYMBOL_YYACCEPT = 49,                  /* $accept  */
  YYSYMBOL_program = 50,                   /* program  */
  YYSYMBOL_stmt = 51,                      /* stmt  */
  YYSYMBOL_stmts = 52,                     /* stmts  */
  YYSYMBOL_arithop = 53,                   /* arithop  */
  YYSYMBOL_relop = 54,                     /* relop  */
  YYSYMBOL_55_1 = 55,                      /* $@1  */
  YYSYMBOL_56_2 = 56,                      /* $@2  */
  YYSYMBOL_boolexpr = 57,                  /* boolexpr  */
  YYSYMBOL_58_3 = 58,                      /* $@3  */
  YYSYMBOL_59_4 = 59,                      /* $@4  */
  YYSYMBOL_expr = 60,                      /* expr  */
  YYSYMBOL_term = 61,                      /* term  */
  YYSYMBOL_assignexpr = 62,                /* assignexpr  */
  YYSYMBOL_primary = 63,                   /* primary  */
  YYSYMBOL_lvalue = 64,                    /* lvalue  */
  YYSYMBOL_member = 65,                    /* member  */
  YYSYMBOL_call = 66,                      /* call  */
  YYSYMBOL_callsuffix = 67,                /* callsuffix  */
  YYSYMBOL_normcall = 68,                  /* normcall  */
  YYSYMBOL_methodcall = 69,                /* methodcall  */
  YYSYMBOL_elist = 70,                     /* elist  */
  YYSYMBOL_const = 71,                     /* const  */
  YYSYMBOL_objectdef = 72,                 /* objectdef  */
  YYSYMBOL_indexed = 73,                   /* indexed  */
  YYSYMBOL_indexedelem = 74,               /* indexedelem  */
  YYSYMBOL_funcname = 75,                  /* funcname  */
  YYSYMBOL_funcprefix = 76,                /* funcprefix  */
  YYSYMBOL_funcargs = 77,                  /* funcargs  */
  YYSYMBOL_78_5 = 78,                      /* $@5  */
  YYSYMBOL_funcblockstart = 79,            /* funcblockstart  */
  YYSYMBOL_funcblockend = 80,              /* funcblockend  */
  YYSYMBOL_funcdef = 81,                   /* funcdef  */
  YYSYMBOL_82_6 = 82,                      /* $@6  */
  YYSYMBOL_idlist = 83,                    /* idlist  */
  YYSYMBOL_block = 84,                     /* block  */
  YYSYMBOL_85_7 = 85,                      /* $@7  */
  YYSYMBOL_86_8 = 86,                      /* $@8  */
  YYSYMBOL_ifprefix = 87,                  /* ifprefix  */
  YYSYMBOL_if = 88,                        /* if  */
  YYSYMBOL_elseprefix = 89,                /* elseprefix  */
  YYSYMBOL_loopstart = 90,                 /* loopstart  */
  YYSYMBOL_loopend = 91,                   /* loopend  */
  YYSYMBOL_loopstmt = 92,                  /* loopstmt  */
  YYSYMBOL_whilestart = 93,                /* whilestart  */
  YYSYMBOL_whilecond = 94,                 /* whilecond  */
  YYSYMBOL_95_9 = 95,                      /* $@9  */
  YYSYMBOL_whilestmt = 96,                 /* whilestmt  */
  YYSYMBOL_N = 97,                         /* N  */
  YYSYMBOL_M = 98,                         /* M  */
  YYSYMBOL_forprefix = 99,                 /* forprefix  */
  YYSYMBOL_100_10 = 100,                   /* $@10  */
  YYSYMBOL_forstmt = 101,                  /* forstmt  */
  YYSYMBOL_returnstmt = 102,               /* returnstmt  */
  YYSYMBOL_103_11 = 103,                   /* $@11  */
  YYSYMBOL_104_12 = 104                    /* $@12  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

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
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
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
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  76
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   549

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  49
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  56
/* YYNRULES -- Number of rules.  */
#define YYNRULES  116
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  201

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   303


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
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
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   140,   140,   145,   147,   156,   162,   169,   176,   182,
     197,   212,   218,   225,   232,   242,   251,   259,   266,   274,
     282,   291,   305,   319,   334,   349,   349,   364,   364,   381,
     380,   421,   420,   460,   464,   469,   474,   479,   485,   490,
     518,   547,   572,   597,   623,   648,   654,   687,   692,   697,
     701,   707,   713,   802,   853,   869,   874,   890,   910,   916,
     926,   932,   962,   971,   976,   983,   994,  1006,  1014,  1031,
    1032,  1037,  1042,  1047,  1052,  1057,  1063,  1073,  1089,  1092,
    1100,  1109,  1115,  1123,  1189,  1189,  1204,  1212,  1217,  1216,
    1241,  1271,  1306,  1311,  1310,  1341,  1340,  1370,  1380,  1389,
    1401,  1408,  1413,  1417,  1422,  1427,  1427,  1436,  1448,  1454,
    1459,  1459,  1471,  1484,  1483,  1503,  1502
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "ID", "STRING", "INT",
  "REAL", "BREAK", "CONTINUE", "IF", "ELSE", "WHILE", "FOR", "FUNCTION",
  "RETURN", "AND", "NOT", "OR", "LOCAL", "TRUE", "FALSE", "NIL", "ASSIGN",
  "PLUS", "MINUS", "MULT", "DIV", "PERC", "EQUAL", "NOT_EQUAL", "PLUS2",
  "MINUS2", "BIGGER", "SMALLER", "BIGGER_EQUAL", "SMALLER_EQUAL",
  "LEFTCURLY", "RIGHTCURLY", "LEFTBRACE", "RIGHTBRACE", "LEFTPAR",
  "RIGHTPAR", "SEMICOLON", "COMMA", "COLON", "COLON2", "PERIOD", "PERIOD2",
  "UMINUS", "$accept", "program", "stmt", "stmts", "arithop", "relop",
  "$@1", "$@2", "boolexpr", "$@3", "$@4", "expr", "term", "assignexpr",
  "primary", "lvalue", "member", "call", "callsuffix", "normcall",
  "methodcall", "elist", "const", "objectdef", "indexed", "indexedelem",
  "funcname", "funcprefix", "funcargs", "$@5", "funcblockstart",
  "funcblockend", "funcdef", "$@6", "idlist", "block", "$@7", "$@8",
  "ifprefix", "if", "elseprefix", "loopstart", "loopend", "loopstmt",
  "whilestart", "whilecond", "$@9", "whilestmt", "N", "M", "forprefix",
  "$@10", "forstmt", "returnstmt", "$@11", "$@12", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303
};
#endif

#define YYPACT_NINF (-167)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-114)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     198,  -167,  -167,  -167,  -167,   -13,    -7,    25,  -167,    26,
      39,    31,   302,    79,  -167,  -167,  -167,   302,     0,     0,
      48,   241,   279,  -167,   106,    89,  -167,   198,  -167,  -167,
    -167,   326,  -167,  -167,  -167,    17,  -167,    41,  -167,  -167,
    -167,  -167,  -167,  -167,   198,  -167,    82,  -167,  -167,  -167,
    -167,  -167,  -167,   302,  -167,  -167,  -167,    76,   302,  -167,
    -167,    11,   115,    14,    41,    14,    93,   198,   302,   473,
      10,    19,  -167,    78,    98,  -167,  -167,  -167,  -167,  -167,
     302,   302,   302,   302,   302,  -167,  -167,   302,   302,   302,
     302,  -167,   302,  -167,  -167,   302,   302,   141,   142,  -167,
    -167,  -167,   302,   302,   143,   113,   112,   144,  -167,  -167,
     302,   108,   302,  -167,   347,   114,  -167,   155,    -1,  -167,
     302,  -167,   120,  -167,   125,  -167,  -167,    11,    11,  -167,
    -167,  -167,   302,   302,   304,   304,   304,   304,   473,   410,
      45,  -167,   130,   431,    53,  -167,  -167,  -167,  -167,   198,
     302,   198,  -167,    56,  -167,   -30,  -167,   125,  -167,   302,
     473,  -167,   302,   302,   302,   501,   501,  -167,  -167,   302,
    -167,  -167,   169,  -167,  -167,   389,  -167,  -167,  -167,   452,
      73,   514,   488,    74,  -167,    86,  -167,  -167,  -167,  -167,
     302,  -167,  -167,  -167,  -167,   174,  -167,   368,  -167,  -167,
    -167
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       3,    52,    72,    70,    71,     0,     0,     0,   104,     0,
      82,   115,     0,     0,    74,    75,    73,     0,     0,     0,
      95,    69,     0,    13,     0,     0,    14,     2,    35,    36,
      37,     0,    34,    33,    45,    47,    55,    48,    51,    49,
     109,    86,    12,    11,     0,     5,     0,     6,   108,     7,
       8,     9,    10,     0,   110,    81,    83,     0,     0,    40,
      53,    39,     0,    41,     0,    43,     0,     0,     0,    67,
       0,     0,    78,     0,     0,    54,     1,    15,    29,    31,
       0,     0,     0,     0,     0,    25,    27,     0,     0,     0,
       0,     4,     0,    42,    44,     0,    69,     0,     0,    61,
      63,    64,     0,    69,     0,     0,     0,    98,   105,   101,
      69,     0,    69,   114,     0,     0,    94,     0,     0,    76,
       0,    77,     0,    38,    50,   109,   109,    16,    17,    18,
      19,    20,     0,     0,    21,    22,    24,    23,    46,     0,
       0,    56,     0,     0,     0,    58,    84,    88,   100,     0,
       0,     0,   107,     0,    97,     0,   116,     0,    96,     0,
      68,    79,    69,     0,     0,    26,    28,    57,    65,    69,
      59,    60,    92,    87,    99,     0,   102,   108,   109,     0,
       0,    30,    32,     0,    90,     0,    89,   106,   103,   101,
       0,    80,    62,    66,    85,     0,   108,     0,    91,   112,
     111
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -167,  -167,   -25,   116,  -167,  -167,  -167,  -167,  -167,  -167,
    -167,   -12,  -167,  -167,  -167,     2,  -167,    32,  -167,  -167,
    -167,   -95,  -167,  -167,  -167,    58,  -167,  -167,  -167,  -167,
    -167,  -167,   -18,  -167,  -167,    75,  -167,  -167,  -167,  -167,
    -167,  -167,  -167,    -5,  -167,  -167,  -167,  -167,  -166,  -119,
    -167,  -167,  -167,  -167,  -167,  -167
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,    25,    26,    27,    28,    29,   132,   133,    30,   125,
     126,    31,    32,    33,    34,    35,    36,    37,    99,   100,
     101,    70,    38,    39,    71,    72,    56,    40,    41,   172,
     106,   186,    42,   173,   185,    43,    66,    67,    44,    45,
     149,   151,   188,   152,    46,   109,   150,    47,   110,   105,
      48,   112,    49,    50,    57,    58
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      59,   140,    77,     1,    74,    61,   163,   164,   144,    69,
      73,   189,   178,   120,    78,   153,    79,   155,    13,   107,
      63,    65,    80,    81,    82,    83,    84,    85,    86,    51,
     199,    87,    88,    89,    90,    52,    82,    83,    84,    92,
      62,   111,    55,   159,   115,    24,   114,    93,    94,   119,
      64,    64,    95,   120,    96,    95,   118,    96,   121,   190,
      97,    98,   122,    97,    98,    53,    54,   180,   127,   128,
     129,   130,   131,  -113,   183,   134,   135,   136,   137,   102,
     138,   103,    60,   139,    69,   -93,   168,   104,   120,    76,
     143,    69,    77,    78,   171,    79,   120,   177,    69,   120,
      69,    80,    81,    82,    83,    84,    85,    86,   160,    75,
      87,    88,    89,    90,   192,   193,   120,   120,   113,   123,
     165,   166,   108,    78,   174,    79,   176,   194,    10,   195,
     116,    80,    81,    82,    83,    84,    85,    86,   175,   124,
      87,    88,    89,    90,   141,   142,   145,   179,    20,   154,
      69,   181,   182,   146,   148,   157,    68,    69,     1,     2,
       3,     4,     5,     6,     7,   162,     8,     9,    10,    11,
     169,    12,   184,    13,    14,    15,    16,   198,   197,    17,
     161,   147,     0,   117,   196,    18,    19,     0,     0,     0,
       0,    20,   158,    21,     0,    22,     0,    23,     0,     0,
      24,     1,     2,     3,     4,     5,     6,     7,     0,     8,
       9,    10,    11,     0,    12,     0,    13,    14,    15,    16,
       0,     0,    17,     0,     0,     0,     0,     0,    18,    19,
       0,     0,     0,     0,    20,     0,    21,     0,    22,     0,
      23,     0,     0,    24,     1,     2,     3,     4,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    12,     0,    13,
      14,    15,    16,     0,     0,    17,     0,     0,     0,     0,
       0,    18,    19,     0,     0,     0,     0,    68,     0,    21,
       0,    22,     1,     2,     3,     4,    24,     0,     0,     0,
       0,     0,    10,     0,     0,    12,     0,    13,    14,    15,
      16,     0,     0,    17,     0,     1,     2,     3,     4,    18,
      19,     0,     0,     0,     0,     0,     0,    21,    12,    22,
      13,    14,    15,    16,    24,     0,    17,    80,    81,    82,
      83,    84,    18,    19,     0,     0,  -114,  -114,  -114,  -114,
      21,    78,    22,    79,     0,     0,     0,    24,     0,    80,
      81,    82,    83,    84,    85,    86,     0,     0,    87,    88,
      89,    90,    78,     0,    79,     0,     0,     0,    91,     0,
      80,    81,    82,    83,    84,    85,    86,     0,     0,    87,
      88,    89,    90,    78,     0,    79,     0,     0,     0,   156,
       0,    80,    81,    82,    83,    84,    85,    86,     0,     0,
      87,    88,    89,    90,    78,     0,    79,     0,     0,     0,
     200,     0,    80,    81,    82,    83,    84,    85,    86,     0,
       0,    87,    88,    89,    90,    78,     0,    79,     0,     0,
     187,     0,     0,    80,    81,    82,    83,    84,    85,    86,
       0,     0,    87,    88,    89,    90,    78,     0,    79,   167,
       0,     0,     0,     0,    80,    81,    82,    83,    84,    85,
      86,     0,     0,    87,    88,    89,    90,    78,     0,    79,
     170,     0,     0,     0,     0,    80,    81,    82,    83,    84,
      85,    86,     0,     0,    87,    88,    89,    90,    78,   191,
      79,     0,     0,     0,     0,     0,    80,    81,    82,    83,
      84,    85,    86,    78,     0,    87,    88,    89,    90,     0,
       0,    80,    81,    82,    83,    84,    85,    86,     0,     0,
      87,    88,    89,    90,    80,    81,    82,    83,    84,  -114,
    -114,     0,     0,    87,    88,    89,    90,    80,    81,    82,
      83,    84,    85,    86,     0,     0,    87,    88,    89,    90
};

static const yytype_int16 yycheck[] =
{
      12,    96,    27,     3,    22,    17,   125,   126,   103,    21,
      22,   177,    42,    43,    15,   110,    17,   112,    18,    44,
      18,    19,    23,    24,    25,    26,    27,    28,    29,    42,
     196,    32,    33,    34,    35,    42,    25,    26,    27,    22,
      40,    53,     3,    44,    62,    45,    58,    30,    31,    39,
      18,    19,    38,    43,    40,    38,    68,    40,    39,   178,
      46,    47,    43,    46,    47,    40,    40,   162,    80,    81,
      82,    83,    84,    42,   169,    87,    88,    89,    90,    38,
      92,    40,     3,    95,    96,    37,    41,    46,    43,     0,
     102,   103,   117,    15,    41,    17,    43,    41,   110,    43,
     112,    23,    24,    25,    26,    27,    28,    29,   120,     3,
      32,    33,    34,    35,    41,    41,    43,    43,    42,    41,
     132,   133,    40,    15,   149,    17,   151,    41,    13,    43,
      37,    23,    24,    25,    26,    27,    28,    29,   150,    41,
      32,    33,    34,    35,     3,     3,     3,   159,    36,    41,
     162,   163,   164,    40,    10,    41,    36,   169,     3,     4,
       5,     6,     7,     8,     9,    40,    11,    12,    13,    14,
      40,    16,     3,    18,    19,    20,    21,     3,   190,    24,
     122,   106,    -1,    67,   189,    30,    31,    -1,    -1,    -1,
      -1,    36,    37,    38,    -1,    40,    -1,    42,    -1,    -1,
      45,     3,     4,     5,     6,     7,     8,     9,    -1,    11,
      12,    13,    14,    -1,    16,    -1,    18,    19,    20,    21,
      -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,    30,    31,
      -1,    -1,    -1,    -1,    36,    -1,    38,    -1,    40,    -1,
      42,    -1,    -1,    45,     3,     4,     5,     6,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    16,    -1,    18,
      19,    20,    21,    -1,    -1,    24,    -1,    -1,    -1,    -1,
      -1,    30,    31,    -1,    -1,    -1,    -1,    36,    -1,    38,
      -1,    40,     3,     4,     5,     6,    45,    -1,    -1,    -1,
      -1,    -1,    13,    -1,    -1,    16,    -1,    18,    19,    20,
      21,    -1,    -1,    24,    -1,     3,     4,     5,     6,    30,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    38,    16,    40,
      18,    19,    20,    21,    45,    -1,    24,    23,    24,    25,
      26,    27,    30,    31,    -1,    -1,    32,    33,    34,    35,
      38,    15,    40,    17,    -1,    -1,    -1,    45,    -1,    23,
      24,    25,    26,    27,    28,    29,    -1,    -1,    32,    33,
      34,    35,    15,    -1,    17,    -1,    -1,    -1,    42,    -1,
      23,    24,    25,    26,    27,    28,    29,    -1,    -1,    32,
      33,    34,    35,    15,    -1,    17,    -1,    -1,    -1,    42,
      -1,    23,    24,    25,    26,    27,    28,    29,    -1,    -1,
      32,    33,    34,    35,    15,    -1,    17,    -1,    -1,    -1,
      42,    -1,    23,    24,    25,    26,    27,    28,    29,    -1,
      -1,    32,    33,    34,    35,    15,    -1,    17,    -1,    -1,
      41,    -1,    -1,    23,    24,    25,    26,    27,    28,    29,
      -1,    -1,    32,    33,    34,    35,    15,    -1,    17,    39,
      -1,    -1,    -1,    -1,    23,    24,    25,    26,    27,    28,
      29,    -1,    -1,    32,    33,    34,    35,    15,    -1,    17,
      39,    -1,    -1,    -1,    -1,    23,    24,    25,    26,    27,
      28,    29,    -1,    -1,    32,    33,    34,    35,    15,    37,
      17,    -1,    -1,    -1,    -1,    -1,    23,    24,    25,    26,
      27,    28,    29,    15,    -1,    32,    33,    34,    35,    -1,
      -1,    23,    24,    25,    26,    27,    28,    29,    -1,    -1,
      32,    33,    34,    35,    23,    24,    25,    26,    27,    28,
      29,    -1,    -1,    32,    33,    34,    35,    23,    24,    25,
      26,    27,    28,    29,    -1,    -1,    32,    33,    34,    35
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,     4,     5,     6,     7,     8,     9,    11,    12,
      13,    14,    16,    18,    19,    20,    21,    24,    30,    31,
      36,    38,    40,    42,    45,    50,    51,    52,    53,    54,
      57,    60,    61,    62,    63,    64,    65,    66,    71,    72,
      76,    77,    81,    84,    87,    88,    93,    96,    99,   101,
     102,    42,    42,    40,    40,     3,    75,   103,   104,    60,
       3,    60,    40,    64,    66,    64,    85,    86,    36,    60,
      70,    73,    74,    60,    81,     3,     0,    51,    15,    17,
      23,    24,    25,    26,    27,    28,    29,    32,    33,    34,
      35,    42,    22,    30,    31,    38,    40,    46,    47,    67,
      68,    69,    38,    40,    46,    98,    79,    51,    40,    94,
      97,    60,   100,    42,    60,    81,    37,    52,    60,    39,
      43,    39,    43,    41,    41,    58,    59,    60,    60,    60,
      60,    60,    55,    56,    60,    60,    60,    60,    60,    60,
      70,     3,     3,    60,    70,     3,    40,    84,    10,    89,
      95,    90,    92,    70,    41,    70,    42,    41,    37,    44,
      60,    74,    40,    98,    98,    60,    60,    39,    41,    40,
      39,    41,    78,    82,    51,    60,    51,    41,    42,    60,
      70,    60,    60,    70,     3,    83,    80,    41,    91,    97,
      98,    37,    41,    41,    41,    43,    92,    60,     3,    97,
      42
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    49,    50,    50,    51,    51,    51,    51,    51,    51,
      51,    51,    51,    51,    52,    52,    53,    53,    53,    53,
      53,    54,    54,    54,    54,    55,    54,    56,    54,    58,
      57,    59,    57,    60,    60,    60,    60,    60,    61,    61,
      61,    61,    61,    61,    61,    61,    62,    63,    63,    63,
      63,    63,    64,    64,    64,    64,    65,    65,    65,    65,
      66,    66,    66,    67,    67,    68,    69,    70,    70,    70,
      71,    71,    71,    71,    71,    71,    72,    72,    73,    73,
      74,    75,    75,    76,    78,    77,    79,    80,    82,    81,
      83,    83,    83,    85,    84,    86,    84,    87,    88,    88,
      89,    90,    91,    92,    93,    95,    94,    96,    97,    98,
     100,    99,   101,   103,   102,   104,   102
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     1,     1,     1,     2,
       2,     1,     1,     1,     1,     2,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     0,     4,     0,     4,     0,
       5,     0,     5,     1,     1,     1,     1,     1,     3,     2,
       2,     2,     2,     2,     2,     1,     3,     1,     1,     1,
       3,     1,     1,     2,     2,     1,     3,     4,     3,     4,
       4,     2,     6,     1,     1,     3,     5,     1,     3,     0,
       1,     1,     1,     1,     1,     1,     3,     3,     1,     3,
       5,     1,     0,     2,     0,     6,     0,     0,     0,     5,
       1,     3,     0,     0,     3,     0,     4,     4,     2,     4,
       1,     0,     0,     3,     1,     0,     4,     3,     0,     0,
       0,     8,     7,     0,     3,     0,     4
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
# ifndef YY_LOCATION_PRINT
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yykind < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yykind], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
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






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
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
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
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
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* program: stmts  */
#line 141 "parser.y"
{
	(yyval.stmtNode) = (yyvsp[0].stmtNode);
	fprintf(yyout_y, "program -> stmt(asteraki)\n");
}
#line 1444 "parser.c"
    break;

  case 3: /* program: %empty  */
#line 145 "parser.y"
  {}
#line 1450 "parser.c"
    break;

  case 4: /* stmt: expr SEMICOLON  */
#line 148 "parser.y"
{
	(yyval.stmtNode) = malloc(sizeof(stmt_t));
	make_stmt((yyval.stmtNode));

	fprintf(yyout_y, "stmt -> expr;\n");
	(yyvsp[-1].exprNode)=emitBoolean((yyvsp[-1].exprNode));
	tempcounter = 0;
}
#line 1463 "parser.c"
    break;

  case 5: /* stmt: if  */
#line 157 "parser.y"
{
	(yyval.stmtNode) = (yyvsp[0].stmtNode);
	fprintf(yyout_y, "stmt -> ifstmt\n");
	tempcounter = 0;
}
#line 1473 "parser.c"
    break;

  case 6: /* stmt: whilestmt  */
#line 163 "parser.y"
{
	(yyval.stmtNode) = malloc(sizeof(stmt_t));
	make_stmt((yyval.stmtNode));
	fprintf(yyout_y, "stmt -> whilestmt\n");
	tempcounter = 0;
}
#line 1484 "parser.c"
    break;

  case 7: /* stmt: forstmt  */
#line 170 "parser.y"
{
	(yyval.stmtNode) = malloc(sizeof(stmt_t));
	make_stmt((yyval.stmtNode));
	fprintf(yyout_y, "stmt -> forstmt\n");
	tempcounter = 0;
}
#line 1495 "parser.c"
    break;

  case 8: /* stmt: returnstmt  */
#line 177 "parser.y"
{
	(yyval.stmtNode) = (yyvsp[0].stmtNode);
	fprintf(yyout_y, "stmt -> returnstmt\n");
	tempcounter = 0;
}
#line 1505 "parser.c"
    break;

  case 9: /* stmt: BREAK SEMICOLON  */
#line 183 "parser.y"
{
	if (check_for_valid_loop_stop(loopcounter) == 0){
		fprintf(stderr,"Break not inside of loop in line %d\n", yylineno);
		isError = true;
	}
	(yyvsp[-1].stmtNode) = malloc(sizeof(stmt_t));
	make_stmt((yyvsp[-1].stmtNode));
	(yyvsp[-1].stmtNode)->breaklist = newlist(nextquad());
	emit(jump, NULL, NULL, NULL, 0, currQuad);

	(yyval.stmtNode) = (yyvsp[-1].stmtNode);
	fprintf(yyout_y, "stmt -> break;\n");
	tempcounter = 0;
}
#line 1524 "parser.c"
    break;

  case 10: /* stmt: CONTINUE SEMICOLON  */
#line 198 "parser.y"
{
	if (check_for_valid_loop_stop(loopcounter) == 0){
		fprintf(stderr,"Continue not inside of loop in line %d\n", yylineno);
		isError = true;
	}
	(yyvsp[-1].stmtNode) = malloc(sizeof(stmt_t));
	make_stmt((yyvsp[-1].stmtNode));
	(yyvsp[-1].stmtNode)->contlist = newlist(nextquad());
	emit(jump, NULL, NULL, NULL, 0, yylineno);

	(yyval.stmtNode) = (yyvsp[-1].stmtNode);
	fprintf(yyout_y, "stmt -> continue;\n");
	tempcounter = 0;
}
#line 1543 "parser.c"
    break;

  case 11: /* stmt: block  */
#line 213 "parser.y"
{
	(yyval.stmtNode) = (yyvsp[0].stmtNode);
	fprintf(yyout_y, "stmt -> block\n");
	tempcounter = 0;
}
#line 1553 "parser.c"
    break;

  case 12: /* stmt: funcdef  */
#line 219 "parser.y"
{
	(yyval.stmtNode) = malloc(sizeof(stmt_t));
	make_stmt((yyval.stmtNode));
	fprintf(yyout_y, "stmt -> funcdef\n");
	tempcounter = 0;
}
#line 1564 "parser.c"
    break;

  case 13: /* stmt: SEMICOLON  */
#line 226 "parser.y"
{
	(yyval.stmtNode) = malloc(sizeof(stmt_t));
	make_stmt((yyval.stmtNode));
	fprintf(yyout_y, "stmt -> ;\n");
	tempcounter = 0;
}
#line 1575 "parser.c"
    break;

  case 14: /* stmts: stmt  */
#line 233 "parser.y"
{
	//
	(yyval.stmtNode) = malloc(sizeof(stmt_t));
	make_stmt((yyval.stmtNode));
	(yyval.stmtNode)->breaklist =(yyvsp[0].stmtNode)->breaklist;
	(yyval.stmtNode)->contlist = (yyvsp[0].stmtNode)->contlist;
	(yyval.stmtNode)->returnlist =(yyvsp[0].stmtNode)->returnlist,
	fprintf(yyout_y, "stmts -> stmt\n");
}
#line 1589 "parser.c"
    break;

  case 15: /* stmts: stmts stmt  */
#line 243 "parser.y"
{
	(yyvsp[-1].stmtNode)->breaklist = mergelist((yyvsp[-1].stmtNode)->breaklist, (yyvsp[0].stmtNode)->breaklist);
	(yyvsp[-1].stmtNode)->contlist = mergelist((yyvsp[-1].stmtNode)->contlist, (yyvsp[0].stmtNode)->contlist);
	(yyvsp[-1].stmtNode)->returnlist = mergelist((yyvsp[-1].stmtNode)->returnlist, (yyvsp[0].stmtNode)->returnlist);
	(yyval.stmtNode) = (yyvsp[-1].stmtNode);
	fprintf(yyout_y, "stmts -> stmts stmt\n");
}
#line 1601 "parser.c"
    break;

  case 16: /* arithop: expr PLUS expr  */
#line 252 "parser.y"
{
	check_arith((yyvsp[-2].exprNode), "");
	check_arith((yyvsp[0].exprNode), "");
	(yyval.exprNode) = result_finder((yyvsp[-2].exprNode), (yyvsp[0].exprNode));
	emit(add, (yyval.exprNode), (yyvsp[-2].exprNode), (yyvsp[0].exprNode), -1, yylineno);
	fprintf(yyout_y, "arithop -> expr + expr\n");
}
#line 1613 "parser.c"
    break;

  case 17: /* arithop: expr MINUS expr  */
#line 260 "parser.y"
{
	check_arith((yyvsp[-2].exprNode), "");
	check_arith((yyvsp[0].exprNode), "");
	(yyval.exprNode) = result_finder((yyvsp[-2].exprNode), (yyvsp[0].exprNode));
	emit(sub, (yyval.exprNode), (yyvsp[-2].exprNode), (yyvsp[0].exprNode), -1, yylineno);
}
#line 1624 "parser.c"
    break;

  case 18: /* arithop: expr MULT expr  */
#line 267 "parser.y"
{
	check_arith((yyvsp[-2].exprNode), "");
	check_arith((yyvsp[0].exprNode), "");
	(yyval.exprNode) = result_finder((yyvsp[-2].exprNode), (yyvsp[0].exprNode));
	emit(mul, (yyval.exprNode), (yyvsp[-2].exprNode), (yyvsp[0].exprNode), -1, yylineno);
	fprintf(yyout_y, "arithop -> expr - expr\n");
}
#line 1636 "parser.c"
    break;

  case 19: /* arithop: expr DIV expr  */
#line 275 "parser.y"
{
	check_arith((yyvsp[-2].exprNode), "");
	check_arith((yyvsp[0].exprNode), "");
	(yyval.exprNode) = result_finder((yyvsp[-2].exprNode), (yyvsp[0].exprNode));
	emit(divi, (yyval.exprNode), (yyvsp[-2].exprNode), (yyvsp[0].exprNode), -1, yylineno);
	fprintf(yyout_y, "arithop -> expr / expr\n");
}
#line 1648 "parser.c"
    break;

  case 20: /* arithop: expr PERC expr  */
#line 283 "parser.y"
{
	check_arith((yyvsp[-2].exprNode), "");
	check_arith((yyvsp[0].exprNode), "");
	(yyval.exprNode) = result_finder((yyvsp[-2].exprNode), (yyvsp[0].exprNode));
	emit(mod, (yyval.exprNode), (yyvsp[-2].exprNode), (yyvsp[0].exprNode), -1, yylineno);
	fprintf(yyout_y, "arithop -> expr % expr\n");
}
#line 1660 "parser.c"
    break;

  case 21: /* relop: expr BIGGER expr  */
#line 292 "parser.y"
{
	fprintf(yyout_y, "expr -> expr > expr\n");
	(yyval.exprNode) = newexpr(boolexpr_e);
	(yyvsp[-2].exprNode) = emitBoolean((yyvsp[-2].exprNode));
	(yyvsp[0].exprNode) = emitBoolean((yyvsp[0].exprNode));

	(yyval.exprNode)->truelist = newlist(nextquad());
	(yyval.exprNode)->falselist = newlist(nextquad() + 1);

	emit(if_greater, NULL, (yyvsp[-2].exprNode), (yyvsp[0].exprNode), 0, yylineno);
	emit(jump, NULL, NULL, NULL, 0, yylineno);
}
#line 1677 "parser.c"
    break;

  case 22: /* relop: expr SMALLER expr  */
#line 306 "parser.y"
{
	fprintf(yyout_y, "expr -> expr < expr\n");
	(yyval.exprNode) = newexpr(boolexpr_e);

	(yyvsp[-2].exprNode) = emitBoolean((yyvsp[-2].exprNode));
	(yyvsp[0].exprNode) = emitBoolean((yyvsp[0].exprNode));

	(yyval.exprNode)->truelist = newlist(nextquad());
	(yyval.exprNode)->falselist = newlist(nextquad() + 1);

	emit(if_less, NULL, (yyvsp[-2].exprNode), (yyvsp[0].exprNode), 0, yylineno);
	emit(jump, NULL, NULL, NULL, 0, yylineno);
}
#line 1695 "parser.c"
    break;

  case 23: /* relop: expr SMALLER_EQUAL expr  */
#line 320 "parser.y"
{
	fprintf(yyout_y, "expr -> expr <= expr\n");
	(yyval.exprNode) = newexpr(boolexpr_e);
	check_arith((yyvsp[-2].exprNode), "");
	check_arith((yyvsp[0].exprNode), "");
	(yyvsp[-2].exprNode) = emitBoolean((yyvsp[-2].exprNode));
	(yyvsp[0].exprNode) = emitBoolean((yyvsp[0].exprNode));

	(yyval.exprNode)->truelist = newlist(nextquad());
	(yyval.exprNode)->falselist = newlist(nextquad() + 1);

	emit(if_lesseq, NULL, (yyvsp[-2].exprNode), (yyvsp[0].exprNode), 0, yylineno);
	emit(jump, NULL, NULL, NULL, 0, yylineno);
}
#line 1714 "parser.c"
    break;

  case 24: /* relop: expr BIGGER_EQUAL expr  */
#line 335 "parser.y"
{
	fprintf(yyout_y, "expr -> expr <= expr\n");
	(yyval.exprNode) = newexpr(boolexpr_e);

	(yyvsp[-2].exprNode) = emitBoolean((yyvsp[-2].exprNode));
	(yyvsp[0].exprNode) = emitBoolean((yyvsp[0].exprNode));

	(yyval.exprNode)->truelist = newlist(nextquad());
	(yyval.exprNode)->falselist = newlist(nextquad() + 1);

	emit(if_greatereq, NULL, (yyvsp[-2].exprNode), (yyvsp[0].exprNode), 0, yylineno);
	emit(jump, NULL, NULL, NULL, 0, yylineno);
}
#line 1732 "parser.c"
    break;

  case 25: /* $@1: %empty  */
#line 349 "parser.y"
             {}
#line 1738 "parser.c"
    break;

  case 26: /* relop: expr EQUAL $@1 expr  */
#line 351 "parser.y"
{
	fprintf(yyout_y, "expr -> expr == expr\n");
	(yyval.exprNode) = newexpr(boolexpr_e);

	(yyvsp[-3].exprNode) = emitBoolean((yyvsp[-3].exprNode));
	(yyvsp[0].exprNode) = emitBoolean((yyvsp[0].exprNode));

	(yyval.exprNode)->truelist = newlist(nextquad());
	(yyval.exprNode)->falselist = newlist(nextquad() + 1);

	emit(if_eq, NULL, (yyvsp[-3].exprNode), (yyvsp[0].exprNode), 0, yylineno);
	emit(jump, NULL, NULL, NULL, 0, yylineno);
}
#line 1756 "parser.c"
    break;

  case 27: /* $@2: %empty  */
#line 364 "parser.y"
                 {}
#line 1762 "parser.c"
    break;

  case 28: /* relop: expr NOT_EQUAL $@2 expr  */
#line 366 "parser.y"
{
	fprintf(yyout_y, "expr -> expr != expr\n");
	(yyval.exprNode) = newexpr(boolexpr_e);

	(yyvsp[-3].exprNode) = emitBoolean((yyvsp[-3].exprNode));
	(yyvsp[0].exprNode) = emitBoolean((yyvsp[0].exprNode));

	(yyval.exprNode)->truelist = newlist(nextquad());
	(yyval.exprNode)->falselist = newlist(nextquad() + 1);

	emit(if_noteq, NULL, (yyvsp[-3].exprNode), (yyvsp[0].exprNode), 0, yylineno);
	emit(jump, NULL, NULL, NULL, 0, yylineno);
}
#line 1780 "parser.c"
    break;

  case 29: /* $@3: %empty  */
#line 381 "parser.y"
{
	if ((yyvsp[-1].exprNode) == NULL)
	{
	}
	if ((yyvsp[-1].exprNode)->type == boolexpr_e)
	{
	}
	else
	{

		(yyvsp[-1].exprNode)->truelist = newlist(nextquad());
		(yyvsp[-1].exprNode)->falselist = newlist(nextquad() + 1);

		emit(if_eq, NULL, (yyvsp[-1].exprNode), newexpr_constbool('t'), 0, yylineno);
		emit(jump, NULL, NULL, NULL, 0, yylineno);
	}
}
#line 1802 "parser.c"
    break;

  case 30: /* boolexpr: expr AND $@3 M expr  */
#line 399 "parser.y"
{
	fprintf(yyout_y, "expr -> expr and expr\n");

	if ((yyvsp[0].exprNode) == NULL)
	{
	}
	if ((yyvsp[0].exprNode)->type == boolexpr_e)
	{
	}
	else
	{
		(yyvsp[0].exprNode)->truelist = newlist(nextquad());
		(yyvsp[0].exprNode)->falselist = newlist(nextquad() + 1);
		emit(if_eq, NULL, (yyvsp[0].exprNode), newexpr_constbool('t'), 0, yylineno);
		emit(jump, NULL, NULL, NULL, 0, yylineno);
	}
	(yyval.exprNode) = newexpr(boolexpr_e);
	patchlist((yyvsp[-4].exprNode)->truelist, (yyvsp[-1].uns));
	(yyval.exprNode)->truelist = (yyvsp[0].exprNode)->truelist;
	(yyval.exprNode)->falselist = mergelist((yyvsp[-4].exprNode)->falselist, (yyvsp[0].exprNode)->falselist);
}
#line 1828 "parser.c"
    break;

  case 31: /* $@4: %empty  */
#line 421 "parser.y"
{
	if ((yyvsp[-1].exprNode) == NULL)
	{
	}
	if ((yyvsp[-1].exprNode)->type == boolexpr_e)
	{
	}
	else
	{
		(yyvsp[-1].exprNode)->truelist = newlist(nextquad());
		(yyvsp[-1].exprNode)->falselist = newlist(nextquad() + 1);
		emit(if_eq, NULL, (yyvsp[-1].exprNode), newexpr_constbool('t'), 0, yylineno);
		emit(jump, NULL, NULL, NULL, 0, yylineno);
	}
}
#line 1848 "parser.c"
    break;

  case 32: /* boolexpr: expr OR $@4 M expr  */
#line 437 "parser.y"
{
	fprintf(yyout_y, "expr -> expr or expr\n");

	if ((yyvsp[0].exprNode) == NULL)
	{
	}
	if ((yyvsp[0].exprNode)->type == boolexpr_e)
	{
	}
	else
	{
		(yyvsp[0].exprNode)->truelist = newlist(nextquad());
		(yyvsp[0].exprNode)->falselist = newlist(nextquad() + 1);
		emit(if_eq, NULL, (yyvsp[0].exprNode), newexpr_constbool('t'), 0, yylineno);
		emit(jump, NULL, NULL, NULL, 0, yylineno);
	}
	(yyval.exprNode) = newexpr(boolexpr_e);
	patchlist((yyvsp[-4].exprNode)->falselist, (yyvsp[-1].uns));
	(yyval.exprNode)->truelist = mergelist((yyvsp[-4].exprNode)->truelist, (yyvsp[0].exprNode)->truelist);
	(yyval.exprNode)->falselist = (yyvsp[0].exprNode)->falselist;
	fprintf(yyout_y, "boolexpr -> expr OR\n");
}
#line 1875 "parser.c"
    break;

  case 33: /* expr: assignexpr  */
#line 461 "parser.y"
{	(yyval.exprNode)=(yyvsp[0].exprNode);
	fprintf(yyout_y, "expr -> assignexpr\n");
}
#line 1883 "parser.c"
    break;

  case 34: /* expr: term  */
#line 465 "parser.y"
{
	(yyval.exprNode)=(yyvsp[0].exprNode);
	fprintf(yyout_y, "expr -> term\n");
}
#line 1892 "parser.c"
    break;

  case 35: /* expr: arithop  */
#line 470 "parser.y"
{
	(yyval.exprNode) = (yyvsp[0].exprNode);
	fprintf(yyout_y, "expr -> arithop\n");
}
#line 1901 "parser.c"
    break;

  case 36: /* expr: relop  */
#line 475 "parser.y"
{
	(yyval.exprNode) = (yyvsp[0].exprNode);
	fprintf(yyout_y, "expr -> relop\n");
}
#line 1910 "parser.c"
    break;

  case 37: /* expr: boolexpr  */
#line 480 "parser.y"
{
	(yyval.exprNode) = (yyvsp[0].exprNode);
	fprintf(yyout_y, "expr -> boolexpr\n");
}
#line 1919 "parser.c"
    break;

  case 38: /* term: LEFTPAR expr RIGHTPAR  */
#line 486 "parser.y"
{
	(yyval.exprNode) = (yyvsp[-1].exprNode);
	fprintf(yyout_y, "term -> ( expr )\n");
}
#line 1928 "parser.c"
    break;

  case 39: /* term: MINUS expr  */
#line 491 "parser.y"
{
	if ((yyvsp[0].exprNode) == NULL)
	{
		(yyval.exprNode) = NULL;
	}
	else if ((yyvsp[0].exprNode)->type == programfunc_e || (yyvsp[0].exprNode)->type == libraryfunc_e)
	{
		fprintf(stderr, "Term cannot be a library or a program function \n"); // check if we should terminate it here
		isError = true;
	}

	check_arith((yyvsp[0].exprNode), "");

	if (istempexpr((yyvsp[0].exprNode)))
	{
		emit(uminus, (yyvsp[0].exprNode), (yyvsp[0].exprNode), NULL, -1, yylineno);
		(yyval.exprNode) = (yyvsp[0].exprNode);
	}
	else
	{
		(yyval.exprNode) = newexpr(arithexpr_e);
		(yyval.exprNode)->sym = newtemp();

		emit(uminus, (yyval.exprNode), (yyvsp[0].exprNode), NULL, -1, yylineno); // _t0 = -x;
	}
	fprintf(yyout_y, "term -> -expr\n");
}
#line 1960 "parser.c"
    break;

  case 40: /* term: NOT expr  */
#line 519 "parser.y"
{
	if ((yyvsp[0].exprNode) == NULL)
	{
		(yyval.exprNode) = NULL;
	}
	else
	{(yyval.exprNode) = newexpr(boolexpr_e);
			(yyval.exprNode)->sym = (yyvsp[0].exprNode)->sym;
		if ((yyvsp[0].exprNode)->type == boolexpr_e)
		{
			(yyval.exprNode)->truelist = (yyvsp[0].exprNode)->falselist;
			(yyval.exprNode)->falselist = (yyvsp[0].exprNode)->truelist;

		}
		else
		{
			(yyvsp[0].exprNode)->truelist = newlist(nextquad());
			(yyvsp[0].exprNode)->falselist = newlist(nextquad() + 1);
			emit(if_eq, NULL, (yyvsp[0].exprNode), newexpr_constbool('t'), 0, yylineno);
			emit(jump, NULL, NULL, NULL, 0, yylineno);

			
			(yyval.exprNode)->truelist = (yyvsp[0].exprNode)->falselist;
			(yyval.exprNode)->falselist = (yyvsp[0].exprNode)->truelist;
		}
	}
	fprintf(yyout_y, "term -> not expr\n");
}
#line 1993 "parser.c"
    break;

  case 41: /* term: PLUS2 lvalue  */
#line 548 "parser.y"
{
	check_arith((yyvsp[0].exprNode), "");
	if (((yyvsp[0].exprNode)) != NULL && (((yyvsp[0].exprNode))->sym->type == USERFUNC || ((yyvsp[0].exprNode))->sym->type == LIBFUNC))
	{
		fprintf(stderr, "Error,value cannnot be a function in line %d and scope %d \n", yylineno, current_scope);
		isError = true;
	}
	if ((yyvsp[0].exprNode)->type == tableitem_e)
	{
		(yyval.exprNode) = emit_iftableitem((yyvsp[0].exprNode));
		emit(add, (yyval.exprNode), (yyval.exprNode), newexpr_constint(1), -1, yylineno);
		emit(tablesetelem, (yyvsp[0].exprNode), (yyvsp[0].exprNode)->index, (yyval.exprNode), -1, yylineno);
	}
	else
	{
		expr *num = newexpr_constint(1);
		emit(add, (yyvsp[0].exprNode), (yyvsp[0].exprNode), num, -1, yylineno);
		(yyval.exprNode) = newexpr(arithexpr_e);
		(yyval.exprNode)->sym = newtemp();
		emit(assign, (yyval.exprNode), (yyvsp[0].exprNode), NULL, -1, yylineno);
	}

	fprintf(yyout_y, "term -> ++lvalue\n");
}
#line 2022 "parser.c"
    break;

  case 42: /* term: lvalue PLUS2  */
#line 573 "parser.y"
{
	check_arith((yyvsp[-1].exprNode), "");
	if (((yyvsp[-1].exprNode)) != NULL && (((yyvsp[-1].exprNode))->sym->type == USERFUNC || ((yyvsp[-1].exprNode))->sym->type == LIBFUNC))
	{
		fprintf(stderr, "Error,value cannnot be a function in line %d and scope %d \n", yylineno, current_scope);
		isError = true;
	}
	(yyval.exprNode) = newexpr(var_e);
	(yyval.exprNode)->sym = newtemp();
	if ((yyvsp[-1].exprNode)->type == tableitem_e)
	{
		expr *val = emit_iftableitem((yyvsp[-1].exprNode));
		emit(assign, (yyval.exprNode), val, NULL, -1, yylineno);
		emit(add, val, val, newexpr_constint(1), -1, yylineno);
		emit(tablesetelem, (yyvsp[-1].exprNode), (yyvsp[-1].exprNode)->index, val, -1, yylineno);
	}
	else
	{
		emit(assign, (yyval.exprNode), (yyvsp[-1].exprNode), NULL, -1, yylineno);
		emit(add, (yyvsp[-1].exprNode), (yyvsp[-1].exprNode), newexpr_constint(1), -1, yylineno);
	}

	fprintf(yyout_y, "term -> lvalue++\n");
}
#line 2051 "parser.c"
    break;

  case 43: /* term: MINUS2 lvalue  */
#line 598 "parser.y"
{
	check_arith((yyvsp[0].exprNode), "");
	if (((yyvsp[0].exprNode)) != NULL && (((yyvsp[0].exprNode))->sym->type == USERFUNC || ((yyvsp[0].exprNode))->sym->type == LIBFUNC))
	{
		fprintf(stderr, "Error,value cannnot be a function in line %d and scope %d \n", yylineno, current_scope);
		isError = true;
	}
	if ((yyvsp[0].exprNode)->type == tableitem_e)
	{
		(yyval.exprNode) = emit_iftableitem((yyvsp[0].exprNode));
		expr *num = newexpr_constint(1);
		emit(sub, (yyval.exprNode), (yyval.exprNode), num, -1, yylineno); // ENDEXETAI NA EXXEI TYPO TO DEUTERO ORISMA
		emit(tablesetelem, (yyvsp[0].exprNode), (yyvsp[0].exprNode)->index, (yyval.exprNode), -1, yylineno);
	}
	else
	{
		emit(sub, (yyvsp[0].exprNode), (yyvsp[0].exprNode), newexpr_constint(1), -1, yylineno);
		(yyval.exprNode) = newexpr(arithexpr_e);
		(yyval.exprNode)->sym = newtemp();


		emit(assign, (yyval.exprNode), (yyvsp[0].exprNode), NULL, -1, yylineno);
	}
	fprintf(yyout_y, "term -> --lvalue\n");
}
#line 2081 "parser.c"
    break;

  case 44: /* term: lvalue MINUS2  */
#line 624 "parser.y"
{
	check_arith((yyvsp[-1].exprNode), "");
	if (((yyvsp[-1].exprNode)) != NULL && (((yyvsp[-1].exprNode))->sym->type == USERFUNC || ((yyvsp[-1].exprNode))->sym->type == LIBFUNC))
	{
		fprintf(stderr, "Error,value cannnot be a function in line %d and scope %d \n", yylineno, current_scope);
		isError = true;
	}
	(yyval.exprNode) = newexpr(var_e);
	(yyval.exprNode)->sym = newtemp();
	if ((yyvsp[-1].exprNode)->type == tableitem_e)
	{
		expr *val = emit_iftableitem((yyvsp[-1].exprNode));
		emit(assign, (yyval.exprNode), val, NULL, -1, yylineno);
		emit(sub, val, val, newexpr_constint(1), -1, yylineno);
		emit(tablesetelem, (yyvsp[-1].exprNode), (yyvsp[-1].exprNode)->index, val, -1, yylineno);
	}
	else
	{
		emit(assign, (yyval.exprNode), (yyvsp[-1].exprNode), NULL, -1, yylineno);
		emit(sub, (yyvsp[-1].exprNode), (yyvsp[-1].exprNode), newexpr_constint(1), -1, yylineno);
	}

	fprintf(yyout_y, "term -> lvalue--\n");
}
#line 2110 "parser.c"
    break;

  case 45: /* term: primary  */
#line 649 "parser.y"
{
	(yyval.exprNode) = (yyvsp[0].exprNode);
	fprintf(yyout_y, "term -> primary\n");
}
#line 2119 "parser.c"
    break;

  case 46: /* assignexpr: lvalue ASSIGN expr  */
#line 655 "parser.y"
{
	fprintf(yyout_y, "assignexpr -> lvalue = expr\n");
	if (call_flag == false)
	{

		if ((yyvsp[-2].exprNode) != NULL && ((yyvsp[-2].exprNode))->type == USERFUNC || ((yyvsp[-2].exprNode))->type == LIBFUNC)
		{
			fprintf(stderr, "Error,value cannot be assigned to a function in line %d and scope %d \n", yylineno, current_scope);
			isError = true;
		}
	}
	if((yyvsp[0].exprNode)->type== boolexpr_e){
            (yyvsp[0].exprNode) =emitBoolean((yyvsp[0].exprNode));
        }
	if (((yyvsp[-2].exprNode))->type == tableitem_e)
	{   
		emit(tablesetelem, (yyvsp[-2].exprNode), (yyvsp[-2].exprNode)->index, (yyvsp[0].exprNode), -1, yylineno);
		(yyval.exprNode) = emit_iftableitem((yyvsp[-2].exprNode));
		((yyval.exprNode))->type = assignexpr_e;
	}   
	else
	{	assert((yyvsp[-2].exprNode));
		emit(assign, (yyvsp[-2].exprNode), (yyvsp[0].exprNode), NULL, -1, yylineno);
		(yyval.exprNode)=newexpr(assignexpr_e);
		
		SymbolTableEntry *temp = newtemp();
		(yyval.exprNode)->sym = temp;
		
		emit(assign, (yyval.exprNode), (yyvsp[-2].exprNode), NULL, -1, yylineno);
	}
}
#line 2155 "parser.c"
    break;

  case 47: /* primary: lvalue  */
#line 688 "parser.y"
{
	fprintf(yyout_y, "primary -> lvalue\n");
	(yyval.exprNode) = emit_iftableitem((yyvsp[0].exprNode));
}
#line 2164 "parser.c"
    break;

  case 48: /* primary: call  */
#line 693 "parser.y"
{
	fprintf(yyout_y, "primary -> call\n");
	(yyval.exprNode) = (yyvsp[0].exprNode);
}
#line 2173 "parser.c"
    break;

  case 49: /* primary: objectdef  */
#line 698 "parser.y"
{	(yyval.exprNode)=(yyvsp[0].exprNode);
	fprintf(yyout_y, "primary -> objectdef\n");
	}
#line 2181 "parser.c"
    break;

  case 50: /* primary: LEFTPAR funcdef RIGHTPAR  */
#line 702 "parser.y"
{
	(yyval.exprNode) = newexpr(programfunc_e);
	(yyval.exprNode)->sym = (yyvsp[-1].exprNode)->sym;
	fprintf(yyout_y, "primary -> ( funcdef )\n");
}
#line 2191 "parser.c"
    break;

  case 51: /* primary: const  */
#line 708 "parser.y"
{
	fprintf(yyout_y, "primary -> const\n");
	(yyval.exprNode) = (yyvsp[0].exprNode);
}
#line 2200 "parser.c"
    break;

  case 52: /* lvalue: ID  */
#line 714 "parser.y"
{
	fprintf(yyout_y, "lvalue -> id\n");
	int temp_scope = current_scope;
	SymbolTableEntry *entry = lookup_inScope(hash, (char *)(yyvsp[0].stringValue), current_scope);
	while (entry == NULL && temp_scope != 0)
	{
		entry = lookup_inScope(hash, (char *)(yyvsp[0].stringValue), temp_scope);
		temp_scope--;
	}

	if (entry == NULL)
	{
		temp_scope = current_scope;
		while (entry == NULL && temp_scope != 0)
		{
			entry = lookup_inScope(hash, (char *)(yyvsp[0].stringValue), temp_scope);
			temp_scope--;
		}
		if (entry == NULL)
		{
			entry = lookup_inScope(hash, (char *)(yyvsp[0].stringValue), 0);
			if (entry == NULL)
			{
				
				if (current_scope == 0)
				{
					offset_++;
					entry = SymTable_insert(hash, (char *)(yyvsp[0].stringValue), yylineno, NULL, current_scope, GLOBAL);
					entry->space = programvar;
					 program_offset++;
					entry->offset =currscopeoffset();
					
				}
				else
				{
					offset_++;
					entry = SymTable_insert(hash, (char *)(yyvsp[0].stringValue), yylineno, NULL, current_scope, LOCALV);
					entry->space = functionlocal;
					entry->offset = currscopeoffset();
					
				}
				incurrscopeoffset();

				(yyval.exprNode) = lvalue_expr(entry);
			}
			else
			{

				(yyval.exprNode) = lvalue_expr(entry);
			}
		}
		else
		{
			if (entry->type == USERFUNC || entry->type == LIBFUNC)
			{
				fprintf(stderr, "Cannot access local function in line %d and scope %d \n", yylineno, current_scope);
				isError = true;
			}
			else
				fprintf(stderr, "Cannot access local variable in line %d and scope %d \n", yylineno, current_scope);
				isError = true;
		}
	}
	else if (entry->type != USERFUNC && entry->type != FORMAL)
	{
		if (lookup_inScope(hash, (char *)(yyvsp[0].stringValue), current_scope) == NULL && (b_af <= sim_funcs) && b_af > 0)
		{
			fprintf(stderr, "Cannot access %s in line %d\n", (yyvsp[0].stringValue), yylineno);
			isError = true;
		}

		(yyval.exprNode) = lvalue_expr(entry);
	}
	else if (entry->type == FORMAL) // an einai formal h dothesa
	{
		if (lookup_inScope(hash, (char *)(yyvsp[0].stringValue), current_scope) == NULL && b_af <= sim_funcs && b_af > 0) // psakse ena panw
		{
			fprintf(stderr, "Cannot access formal %s in line %d\n", (yyvsp[0].stringValue), yylineno);
			isError = true;
		}

		(yyval.exprNode) = lvalue_expr(entry);
	}
	else 
	(yyval.exprNode) = lvalue_expr(entry);
	printf("OFFSET LVALUE %d %d\n",entry->offset, entry->space);
	
}
#line 2293 "parser.c"
    break;

  case 53: /* lvalue: LOCAL ID  */
#line 803 "parser.y"
{
	// edw tha psaksw an exoume sto IDIO SCOPE ksanaorismenh me to idio onoma mia metablhth
	fprintf(yyout_y, "lvalue -> local id\n");
	SymbolTableEntry *entry = lookup_inScope(hash, (char *)(yyvsp[0].stringValue), current_scope);

	if (entry == NULL)
	{
		entry = lookup_inBucket(hash, (char *)(yyvsp[0].stringValue), current_scope); // htan 1

		if (entry != NULL && entry->type == LIBFUNC && current_scope != 0) /*htan sketo scope*/
		{
			fprintf(stderr, "Shadow libfunc in line %d and scope %d \n", yylineno, current_scope);
			isError = true;
			(yyval.exprNode) = NULL;
		}
		else
		{
			
			if (current_scope == 0)
			{
				offset_++;
				entry = SymTable_insert(hash, (char *)(yyvsp[0].stringValue), yylineno, NULL, current_scope, GLOBAL);
				entry->space = programvar;
				entry->offset = currscopeoffset();
				program_offset++;
				
			}
			else
			{
				offset_++;
				entry = SymTable_insert(hash, (char *)(yyvsp[0].stringValue), yylineno, NULL, current_scope, LOCALV);
				entry->space = functionlocal;
				entry->offset = currscopeoffset();
				
			}
			incurrscopeoffset();

			(yyval.exprNode) = lvalue_expr(entry);
		}
	}
	else
	{
		fprintf(stderr,"Error: In line %d variable %s is already defined\n", yylineno, (yyvsp[0].stringValue));
		isError = true;
		(yyval.exprNode) = lvalue_expr(entry);
	}
	
	(yyval.exprNode) = lvalue_expr(entry);
	
}
#line 2348 "parser.c"
    break;

  case 54: /* lvalue: COLON2 ID  */
#line 854 "parser.y"
{
	SymbolTableEntry *entry = lookup_inScope(hash, (char *)(yyvsp[0].stringValue), 0);
	if (entry == NULL)
	{
		fprintf(stderr, "Global not found in line %d and scope %d \n", yylineno, current_scope);
		isError = true;
		(yyval.exprNode)=newexpr(nil_e);
	}
	else{
		entry->space = currscopespace();
		entry->offset = currscopeoffset();
}
		(yyval.exprNode) = lvalue_expr(entry);
	fprintf(yyout_y, "lvalue -> ::id\n");
}
#line 2368 "parser.c"
    break;

  case 55: /* lvalue: member  */
#line 870 "parser.y"
{	(yyval.exprNode)=(yyvsp[0].exprNode);
	fprintf(yyout_y, "lvalue -> member\n");
}
#line 2376 "parser.c"
    break;

  case 56: /* member: lvalue PERIOD ID  */
#line 875 "parser.y"
{
	if ((yyvsp[-2].exprNode) == NULL)
	{
		fprintf(stderr, "lvalue not declared in line %d and scope %d \n", yylineno, current_scope);
		isError = true;
	}
	else if (((yyvsp[-2].exprNode))->sym->type == USERFUNC || ((yyvsp[-2].exprNode))->sym->type == LIBFUNC)
	{
		fprintf(stderr, "Cant use function name as an lvalue.id in line %d and scope %d \n", yylineno, current_scope);
		isError = true;
	}
	fprintf(yyout_y, "member -> lvalue.id\n");
	(yyval.exprNode) = member_item((yyvsp[-2].exprNode), (yyvsp[0].stringValue));
	
}
#line 2396 "parser.c"
    break;

  case 57: /* member: lvalue LEFTBRACE expr RIGHTBRACE  */
#line 891 "parser.y"
{
	if ((yyvsp[-3].exprNode) == NULL)
	{
		fprintf(stderr, "lvalue not declared in line %d and scope %d \n", yylineno, current_scope);
		isError = true;
	}
	else if (((yyvsp[-3].exprNode))->sym->type == USERFUNC || ((yyvsp[-3].exprNode))->sym->type == LIBFUNC)
	{
		fprintf(stderr, "Cannot use function name as an lvalue[] in line %d and scope %d \n", yylineno, current_scope);
		isError = true;
	}
	fprintf(yyout_y, "member -> lvalue [ expr ]\n");
	(yyvsp[-1].exprNode)=emitBoolean((yyvsp[-1].exprNode));
	(yyvsp[-3].exprNode) = emit_iftableitem((yyvsp[-3].exprNode));
	(yyval.exprNode) = newexpr(tableitem_e);
	(yyval.exprNode)->sym = (yyvsp[-3].exprNode)->sym;
	(yyval.exprNode)->index = (yyvsp[-1].exprNode);
	
}
#line 2420 "parser.c"
    break;

  case 58: /* member: call PERIOD ID  */
#line 911 "parser.y"
{
	(yyval.exprNode) = member_item((yyvsp[-2].exprNode), (yyvsp[0].stringValue));
	call_flag = true;
	fprintf(yyout_y, "member -> call.id\n");
}
#line 2430 "parser.c"
    break;

  case 59: /* member: call LEFTBRACE expr RIGHTBRACE  */
#line 917 "parser.y"
{	(yyvsp[-1].exprNode)=emitBoolean((yyvsp[-1].exprNode));
	(yyvsp[-3].exprNode) = emit_iftableitem((yyvsp[-3].exprNode));
	(yyval.exprNode) = newexpr(tableitem_e);
	(yyval.exprNode)->sym = (yyvsp[-3].exprNode)->sym;
	(yyval.exprNode)->index = (yyvsp[-1].exprNode);

	fprintf(yyout_y, "member -> call [ expr ]\n");
}
#line 2443 "parser.c"
    break;

  case 60: /* call: call LEFTPAR elist RIGHTPAR  */
#line 927 "parser.y"
{
	(yyval.exprNode) = make_call((yyvsp[-3].exprNode), (yyvsp[-1].exprNode));
	call_flag = false;
	fprintf(yyout_y, "call -> call ( elist )\n");
}
#line 2453 "parser.c"
    break;

  case 61: /* call: lvalue callsuffix  */
#line 933 "parser.y"
{

	(yyvsp[-1].exprNode) = emit_iftableitem((yyvsp[-1].exprNode));
	if ((yyvsp[0].callNode)->method)
	{
		expr *t = (yyvsp[-1].exprNode);
		(yyvsp[-1].exprNode) = emit_iftableitem(member_item(t, (yyvsp[0].callNode)->name));
		expr *tmp;
		if ((yyvsp[0].callNode)->elist != NULL)
		{
			tmp = (yyvsp[0].callNode)->elist;
			while (tmp->next != NULL)
			{
				tmp = tmp->next;
			}
			tmp->next = NULL;
			t->next = (yyvsp[0].callNode)->elist;
			(yyvsp[0].callNode)->elist = t;
		}
	}
	call_flag = false;
	if (!(yyvsp[-1].exprNode))
	{
		fprintf(stderr, "Function not declared in line %d and scope %d \n", yylineno, current_scope);
		isError = true;
	}
	(yyval.exprNode) = make_call((yyvsp[-1].exprNode), (yyvsp[0].callNode)->elist);
	fprintf(yyout_y, "call -> lvalue callsuffix\n");
}
#line 2487 "parser.c"
    break;

  case 62: /* call: LEFTPAR funcdef RIGHTPAR LEFTPAR elist RIGHTPAR  */
#line 963 "parser.y"
{
	expr *func = newexpr(programfunc_e);
	func->sym = (yyvsp[-4].exprNode)->sym;
	(yyval.exprNode) = make_call(func, (yyvsp[-1].exprNode));
	call_flag = false;
	fprintf(yyout_y, "call -> ( funcdef ) ( elist )\n");
}
#line 2499 "parser.c"
    break;

  case 63: /* callsuffix: normcall  */
#line 972 "parser.y"
{
	(yyval.callNode) = (yyvsp[0].callNode);
	fprintf(yyout_y, "callsuffix -> normcall\n");
}
#line 2508 "parser.c"
    break;

  case 64: /* callsuffix: methodcall  */
#line 977 "parser.y"
{
	(yyval.callNode) = (yyvsp[0].callNode);

	fprintf(yyout_y, "callsuffix -> methodcall\n");
}
#line 2518 "parser.c"
    break;

  case 65: /* normcall: LEFTPAR elist RIGHTPAR  */
#line 984 "parser.y"
{
	(yyval.callNode) = malloc(sizeof(call_t));
	(yyval.callNode)->elist = (yyvsp[-1].exprNode);
	(yyval.callNode)->method = 0;
	(yyval.callNode)->name = NULL;
	fprintf(yyout_y, "normcall -> ( elist )\n");
}
#line 2530 "parser.c"
    break;

  case 66: /* methodcall: PERIOD2 ID LEFTPAR elist RIGHTPAR  */
#line 995 "parser.y"
{

	(yyval.callNode) = malloc(sizeof(call_t));
	(yyval.callNode)->elist = (yyvsp[-1].exprNode);
	(yyval.callNode)->method = 1;
	(yyval.callNode)->name = (yyvsp[-3].stringValue);
	fprintf(yyout_y, "methodcall -> ..id ( elist )\n");
}
#line 2543 "parser.c"
    break;

  case 67: /* elist: expr  */
#line 1006 "parser.y"
              {
		if((yyvsp[0].exprNode)->type==boolexpr_e){
				(yyvsp[0].exprNode)=emitBoolean((yyvsp[0].exprNode));
			}

		(yyval.exprNode)=(yyvsp[0].exprNode);
		(yyval.exprNode)->next=NULL;
		fprintf(yyout_y,"elist -> expr\n");}
#line 2556 "parser.c"
    break;

  case 68: /* elist: elist COMMA expr  */
#line 1014 "parser.y"
                           { 
		expr* tmp;
			if((yyvsp[-2].exprNode)!=NULL && (yyvsp[0].exprNode)!=NULL){
			tmp=(yyvsp[-2].exprNode);
			while(tmp->next){
				tmp=tmp->next;
				}
			if((yyvsp[0].exprNode)->type==boolexpr_e){
				(yyvsp[0].exprNode)=emitBoolean((yyvsp[0].exprNode));
			}
			tmp->next=(yyvsp[0].exprNode);
			(yyvsp[0].exprNode)->next=NULL;
			(yyval.exprNode)=(yyvsp[-2].exprNode);
			
			}
            fprintf(yyout_y,"elist -> elist , expr\n");
		}
#line 2578 "parser.c"
    break;

  case 69: /* elist: %empty  */
#line 1031 "parser.y"
          {(yyval.exprNode)=NULL;fprintf(yyout_y,"elist -> NULL\n");}
#line 2584 "parser.c"
    break;

  case 70: /* const: INT  */
#line 1033 "parser.y"
{
	fprintf(yyout_y, "const -> INT\n");
	(yyval.exprNode) = newexpr_constint((yyvsp[0].intValue));
}
#line 2593 "parser.c"
    break;

  case 71: /* const: REAL  */
#line 1038 "parser.y"
{
	fprintf(yyout_y, "const -> REAL\n");
	(yyval.exprNode) = newexpr_constdouble((yyvsp[0].doubleValue));
}
#line 2602 "parser.c"
    break;

  case 72: /* const: STRING  */
#line 1043 "parser.y"
{
	fprintf(yyout_y, "const -> STRING\n");
	(yyval.exprNode) = newexpr_conststring((yyvsp[0].stringValue));
}
#line 2611 "parser.c"
    break;

  case 73: /* const: NIL  */
#line 1048 "parser.y"
{
	fprintf(yyout_y, "const -> nil\n");
	(yyval.exprNode) = newexpr_constnil();
}
#line 2620 "parser.c"
    break;

  case 74: /* const: TRUE  */
#line 1053 "parser.y"
{
	fprintf(yyout_y, "const -> TRUE\n");
	(yyval.exprNode) = newexpr_constbool('t');
}
#line 2629 "parser.c"
    break;

  case 75: /* const: FALSE  */
#line 1058 "parser.y"
{
	fprintf(yyout_y, "const -> FALSE\n");
	(yyval.exprNode) = newexpr_constbool('f');
}
#line 2638 "parser.c"
    break;

  case 76: /* objectdef: LEFTBRACE elist RIGHTBRACE  */
#line 1064 "parser.y"
{
	expr *t = newexpr(newtable_e);
	t->sym = newtemp();
	emit(tablecreate, t, NULL, NULL, -1, yylineno);
	for (int i = 0; (yyvsp[-1].exprNode); (yyvsp[-1].exprNode) = (yyvsp[-1].exprNode)->next)
		emit(tablesetelem, t, newexpr_constint(i++), (yyvsp[-1].exprNode), -1, yylineno);
	(yyval.exprNode) = t;
	fprintf(yyout_y, "objectdef -> { elist }\n");
}
#line 2652 "parser.c"
    break;

  case 77: /* objectdef: LEFTBRACE indexed RIGHTBRACE  */
#line 1074 "parser.y"
{
	expr *t = newexpr(newtable_e);
	t->sym = newtemp();
	emit(tablecreate, t, NULL, NULL, -1, yylineno);
	while ((yyvsp[-1].exprNode) != NULL)
	{
		emit(tablesetelem, t, (yyvsp[-1].exprNode)->index, (yyvsp[-1].exprNode), -1, yylineno);
		(yyvsp[-1].exprNode) = (yyvsp[-1].exprNode)->next;
	}
	(yyval.exprNode) = t;

	fprintf(yyout_y, "objectdef -> { indexed }\n");
}
#line 2670 "parser.c"
    break;

  case 78: /* indexed: indexedelem  */
#line 1089 "parser.y"
                    {
		(yyval.exprNode)=(yyvsp[0].exprNode);
		fprintf(yyout_y,"indexed -> indexedelem\n");}
#line 2678 "parser.c"
    break;

  case 79: /* indexed: indexed COMMA indexedelem  */
#line 1092 "parser.y"
                                   {
		 while((yyvsp[-2].exprNode)->next){  
                   (yyvsp[-2].exprNode) = (yyvsp[-2].exprNode)->next;
                 }        
                 (yyvsp[-2].exprNode)->next = (yyvsp[0].exprNode);
		fprintf(yyout_y,"indexed -> indexed , indexedelem\n");}
#line 2689 "parser.c"
    break;

  case 80: /* indexedelem: LEFTCURLY expr COLON expr RIGHTCURLY  */
#line 1101 "parser.y"
{
	(yyvsp[-3].exprNode)=emitBoolean((yyvsp[-3].exprNode));
	(yyvsp[-1].exprNode)=emitBoolean((yyvsp[-1].exprNode));
	(yyvsp[-1].exprNode)->index = (yyvsp[-3].exprNode);
	fprintf(yyout_y, "indexedelem -> { expr : expr }\n");
	(yyval.exprNode) = (yyvsp[-1].exprNode);
}
#line 2701 "parser.c"
    break;

  case 81: /* funcname: ID  */
#line 1110 "parser.y"
{
	(yyval.stringValue) = (yyvsp[0].stringValue);
	fprintf(yyout_y, "funcname -> ID\n");
}
#line 2710 "parser.c"
    break;

  case 82: /* funcname: %empty  */
#line 1115 "parser.y"
{
	char *my_name = malloc(50 * (sizeof(char)));
	sprintf(my_name, "_myfync%d", unnamed_counter++);
	(yyval.stringValue) = my_name;
	fprintf(yyout_y, "funcname -> NULL\n");
}
#line 2721 "parser.c"
    break;

  case 83: /* funcprefix: FUNCTION funcname  */
#line 1124 "parser.y"
{
	
	
	SymbolTableEntry *search = lookup_inScope(hash, (char *)(yyvsp[0].stringValue), 0);
	if (search != NULL)
	{
		if (search->type == LIBFUNC)
		{
			fprintf(stderr, "Userfunc shadows libraryfunn in line %d and scope %d \n", yylineno, current_scope);
			isError = true;
		}
		else if (current_scope == 0)
		{
			fprintf(stderr, "Found symbol with same name in line %d and scope %d \n", yylineno, current_scope);
			isError = true;
		}
		(yyval.exprNode) = malloc(sizeof(expr));
		(yyval.exprNode)->sym = search;
	}
	int temp = current_scope;
	search = lookup_inScope(hash, (char *)(yyvsp[0].stringValue), current_scope);
	if (search == NULL)
	{
		(yyval.exprNode) = newexpr(programfunc_e);
		(yyval.exprNode)->sym = SymTable_insert(hash, (char *)(yyvsp[0].stringValue), yylineno, NULL, temp, USERFUNC);
		(yyval.exprNode)->sym->space=currscopespace();
		(yyval.exprNode)->sym->offset=currscopeoffset();

	}
	else
	{
		if (search->type == USERFUNC || search->type == LIBFUNC)
		{
			fprintf(stderr, "Function redefinition in line %d and scope %d \n", yylineno, current_scope);
			isError = true;
		}
		else
		{
			fprintf(stderr, "Function %s declared with same name as variable in line %d and scope %d \n", (yyvsp[0].stringValue), yylineno, current_scope);
			isError = true;
		}
		(yyval.exprNode) = malloc(sizeof(expr));
		(yyval.exprNode)->sym = search;
	}
	(yyval.exprNode)->iaddress = nextquadlabel(); 
	//incurrscopeoffset();
	int *tmp = malloc(sizeof(int));
	*tmp = nextquadlabel();
	push(func_stack, *tmp);
	emit(jump, NULL, NULL, NULL, 0, yylineno);
	expr* func=newexpr(programfunc_e);
	func->sym=(yyval.exprNode)->sym;
	func->sym->value.funcVal->name=(yyvsp[0].stringValue);
	func->sym->address=nextquadlabel();
	func->iaddress=nextquadlabel();
	emit(funcstart,func, NULL, NULL, -1, yylineno);

	push(stack_, offset_); 
	offset_ = -1;
	enterscopespace();
	resetformalargoffset(); // Start formals from zero
	
	fprintf(yyout_y, "funcprefix -> FUNCTION funcname\n");}
#line 2789 "parser.c"
    break;

  case 84: /* $@5: %empty  */
#line 1189 "parser.y"
                            {
	
	increase_scope();
	
	isFunct1 = true;
	isFunct = true;
}
#line 2801 "parser.c"
    break;

  case 85: /* funcargs: funcprefix M LEFTPAR $@5 idlist RIGHTPAR  */
#line 1197 "parser.y"
{
	(yyvsp[-5].exprNode)->sym->value.funcVal->args = (yyvsp[-1].listId);
	enterscopespace();	
	resetfunctionlocaloffset(); 
	fprintf(yyout_y, "funcargs -> funcprefix M ( idlist )\n");}
#line 2811 "parser.c"
    break;

  case 86: /* funcblockstart: %empty  */
#line 1204 "parser.y"
{
	offset_ = -1;
	sim_funcs++;
	push(loopcounterstack, loopcounter); 
	loopcounter = 0;
	fprintf(yyout_y, "funcblockstart -> NULL\n");}
#line 2822 "parser.c"
    break;

  case 87: /* funcblockend: %empty  */
#line 1212 "parser.y"
{
	fprintf(yyout_y, "funcblockend -> NULL\n");}
#line 2829 "parser.c"
    break;

  case 88: /* $@6: %empty  */
#line 1217 "parser.y"
{
	int curr_scope_offset = currscopeoffset();
	fprintf(yyout_y, "funcdef -> funcargs funcblockstart block\n");
}
#line 2838 "parser.c"
    break;

  case 89: /* funcdef: funcargs funcblockstart block $@6 funcblockend  */
#line 1222 "parser.y"
{
	int fof = pop(func_stack);
	assert((yyval.exprNode));
	(yyvsp[-4].exprNode)->sym->value.funcVal->totalLocals = offset_ + 1;
	emit(funcend, (yyval.exprNode), NULL, NULL, -1, yylineno);
	(yyval.exprNode) = (yyvsp[-4].exprNode);
	loopcounter = pop(loopcounterstack);
	sim_funcs--;
	offset_ = pop(stack_);
	restorecurrscopeoffset(offset_);
	exitscopespace();
	
	patchlabel(fof,nextquad());
	// printf("%d",fof);
	patchlist((yyvsp[-2].stmtNode)->returnlist,nextquad()-1);
	fprintf(yyout_y, "funcdef -> funcblockend\n");
}
#line 2860 "parser.c"
    break;

  case 90: /* idlist: ID  */
#line 1242 "parser.y"
        {
		//check if it's a gloabal lib function
		if (lookup_inScope(hash,(yyvsp[0].stringValue),0) != NULL && lookup_inScope(hash,(yyvsp[0].stringValue),0)->type == LIBFUNC)
		{
			fprintf(stderr,"This formal argument shadows function from libary in line %d and scope %d \n",yylineno,current_scope);
			isError = true;
			
		}else if(lookup_inScope(hash,(yyvsp[0].stringValue),current_scope)!=NULL)
		{
			fprintf(stderr,"This formal argument already exists in given scope %s  in line %d", (yyvsp[0].stringValue),current_scope,yylineno); 
			isError = true;       
			
		}else{
		
		//insertion in the idlist and saving the idlist
		
		(yyval.listId) = create_id_list();
		insert((yyval.listId),(yyvsp[0].stringValue));

		//insertion in the symtable/scopelist
		SymbolTableEntry* temp;
		temp = SymTable_insert(hash, (yyvsp[0].stringValue), yylineno , NULL , current_scope, FORMAL);
		temp->offset = currscopeoffset();
		temp->space = formalarg;
		printf("OFFSET FOmal %d %d\n",temp->offset, temp->space);

		incurrscopeoffset();
		fprintf(yyout_y,"idlist -> id\n");}
	}
#line 2894 "parser.c"
    break;

  case 91: /* idlist: idlist COMMA ID  */
#line 1272 "parser.y"
        {
	//check if it's a gloabal lib function
    if (lookup_inScope(hash,(const char *)(yyvsp[0].stringValue),0) != NULL && lookup_inScope(hash,(const char*)(yyvsp[0].stringValue),0)->type == LIBFUNC)
    {
        fprintf(stderr,"This formal argument shadows function from libary in line %d and scope %d \n",yylineno,current_scope);
	isError = true;
        
    }else if(lookup_inScope(hash,(const char*)(yyvsp[0].stringValue),current_scope)!=NULL)
    {
        fprintf(stderr,"This formal argument already exists in given scope in line %d and scope %d \n",yylineno,current_scope); 
	isError = true;      
        
    }else{
    
    //insertion in the idlist and saving the idlist
    insert((yyvsp[-2].listId),(yyvsp[0].stringValue));
    (yyval.listId) = (yyvsp[-2].listId); //not sure if this is functional 
	
	//insertion in the symtable/scopelist
	SymbolTableEntry* temp;
		temp =SymTable_insert(hash, (const char*)(yyvsp[0].stringValue), yylineno , NULL , current_scope, FORMAL); /*to 2o orisma htan $1 kai de douleue to print (obviously, afou to ena einai idlist kai to allo string)*/
		//temp->offset = formalArgOffset++;
		//temp->space = formalarg;
		temp->space = formalarg;
		temp->offset = currscopeoffset();
		printf("OFFSET FOmal %d %d\n",temp->offset, temp->space);

		incurrscopeoffset();
	fprintf(yyout_y,"idlist -> idlist , ID\n");
}
    fprintf(yyout_y,"idlist -> idlist , id\n");
	
	}
#line 2932 "parser.c"
    break;

  case 92: /* idlist: %empty  */
#line 1306 "parser.y"
        {    
		(yyval.listId) = create_id_list();
 		fprintf(yyout_y,"idlist ->  \n"); 
	}
#line 2941 "parser.c"
    break;

  case 93: /* $@7: %empty  */
#line 1311 "parser.y"
{
	if (sim_funcs != 0)
	{
		b_af++;
	}
	if (isFunct == true)
	{
		isFunct = false;
	}
	else
	{
		increase_scope();
	}
}
#line 2960 "parser.c"
    break;

  case 94: /* block: LEFTCURLY $@7 RIGHTCURLY  */
#line 1326 "parser.y"
{
	(yyval.stmtNode) = malloc(sizeof(stmt_t));
	make_stmt((yyval.stmtNode));
	fprintf(yyout_y, "block -> { }\n");
	decrease_scope();
	if (sim_funcs == b_af && sim_funcs > 0)
	{
		b_af--;
	}
	else if (sim_funcs < b_af)
	{
		b_af--;
	}
}
#line 2979 "parser.c"
    break;

  case 95: /* $@8: %empty  */
#line 1341 "parser.y"
{
	if (sim_funcs != 0)
	{
		b_af++;
	}
	if (isFunct == true)
	{
		isFunct = false;
	}
	else
	{
		increase_scope();
	}
}
#line 2998 "parser.c"
    break;

  case 96: /* block: LEFTCURLY $@8 stmts RIGHTCURLY  */
#line 1356 "parser.y"
{
	(yyval.stmtNode) = (yyvsp[-1].stmtNode);
	fprintf(yyout_y, "block -> { stmts }\n");
	decrease_scope();
	if (sim_funcs == b_af && sim_funcs > 0)
	{
		b_af--;
	}
	else if (sim_funcs < b_af)
	{
		b_af--;
	}
}
#line 3016 "parser.c"
    break;

  case 97: /* ifprefix: IF LEFTPAR expr RIGHTPAR  */
#line 1371 "parser.y"
{
	(yyvsp[-1].exprNode) = emitBoolean((yyvsp[-1].exprNode));
	emit(if_eq, NULL, (yyvsp[-1].exprNode), newexpr_constbool('t'), nextquad() + 2, yylineno);
	(yyval.uns) = nextquad();
	emit(jump, NULL, NULL, NULL, 0, yylineno);
	fprintf(yyout_y, "ifprefix -> IF ( expr )\n");
}
#line 3028 "parser.c"
    break;

  case 98: /* if: ifprefix stmt  */
#line 1381 "parser.y"
        {
		if((yyvsp[0].stmtNode)!=NULL){
		(yyval.stmtNode) = (yyvsp[0].stmtNode);
		}else { (yyval.stmtNode) = malloc(sizeof(stmt_t));
make_stmt((yyval.stmtNode));}
		patchlabel((yyvsp[-1].uns), nextquad());
		fprintf(yyout_y, "if -> ifprefix stmt\n");
	}
#line 3041 "parser.c"
    break;

  case 99: /* if: ifprefix stmt elseprefix stmt  */
#line 1390 "parser.y"
{
	patchlabel((yyvsp[-3].uns), (yyvsp[-1].uns) + 1);
	patchlabel((yyvsp[-1].uns), nextquad());
	(yyval.stmtNode) = malloc(sizeof(stmt_t));

	(yyval.stmtNode)->breaklist = mergelist((yyvsp[-2].stmtNode)->breaklist, (yyvsp[0].stmtNode)->breaklist);
	(yyval.stmtNode)->contlist = mergelist((yyvsp[-2].stmtNode)->contlist, (yyvsp[0].stmtNode)->contlist);
	(yyval.stmtNode)->returnlist = mergelist((yyvsp[-2].stmtNode)->returnlist, (yyvsp[0].stmtNode)->returnlist);
	fprintf(yyout_y, "if -> ifprefix stmt elseprefix stmt\n");
}
#line 3056 "parser.c"
    break;

  case 100: /* elseprefix: ELSE  */
#line 1402 "parser.y"
{
	(yyval.uns) = nextquad();
	emit(jump, NULL, NULL, NULL, 0, yylineno);
}
#line 3065 "parser.c"
    break;

  case 101: /* loopstart: %empty  */
#line 1408 "parser.y"
{
	++loopcounter;
}
#line 3073 "parser.c"
    break;

  case 102: /* loopend: %empty  */
#line 1413 "parser.y"
{
	--loopcounter;
}
#line 3081 "parser.c"
    break;

  case 103: /* loopstmt: loopstart stmt loopend  */
#line 1418 "parser.y"
{
	(yyval.stmtNode) = (yyvsp[-1].stmtNode);
}
#line 3089 "parser.c"
    break;

  case 104: /* whilestart: WHILE  */
#line 1423 "parser.y"
{
	(yyval.uns) = nextquad();
}
#line 3097 "parser.c"
    break;

  case 105: /* $@9: %empty  */
#line 1427 "parser.y"
                    { sim_loops++; }
#line 3103 "parser.c"
    break;

  case 106: /* whilecond: LEFTPAR $@9 expr RIGHTPAR  */
#line 1429 "parser.y"
{
	(yyvsp[-1].exprNode) = emitBoolean((yyvsp[-1].exprNode));
	emit(if_eq, NULL, (yyvsp[-1].exprNode), newexpr_constbool('t'), nextquad() + 2, yylineno);
	(yyval.uns) = nextquad();
	emit(jump, NULL, NULL, NULL, 0, yylineno);
}
#line 3114 "parser.c"
    break;

  case 107: /* whilestmt: whilestart whilecond loopstmt  */
#line 1437 "parser.y"
{
	fprintf(yyout_y, "whilestmt -> while ( expr ) stmt\n");
	emit(jump, NULL, NULL, NULL, (yyvsp[-2].uns), yylineno);
	int bl = 0, cl = 0;
	patchlabel((yyvsp[-1].uns), nextquad());
	patchlist((yyvsp[0].stmtNode)->breaklist, nextquad());
	patchlist((yyvsp[0].stmtNode)->contlist, (yyvsp[-2].uns));
	(yyval.stmtNode) = (yyvsp[0].stmtNode);
}
#line 3128 "parser.c"
    break;

  case 108: /* N: %empty  */
#line 1448 "parser.y"
{
	(yyval.uns) = nextquad();
	emit(jump, NULL, NULL, NULL, 0, yylineno);
	fprintf(yyout_y, "N -> NULL \n");
}
#line 3138 "parser.c"
    break;

  case 109: /* M: %empty  */
#line 1454 "parser.y"
{
	(yyval.uns) = nextquad();
	fprintf(yyout_y, "M -> NULL\n");
}
#line 3147 "parser.c"
    break;

  case 110: /* $@10: %empty  */
#line 1459 "parser.y"
                        { sim_loops++; }
#line 3153 "parser.c"
    break;

  case 111: /* forprefix: FOR LEFTPAR $@10 elist SEMICOLON M expr SEMICOLON  */
#line 1461 "parser.y"
{
	(yyval.forNode) = malloc(sizeof(for_t));
	(yyval.forNode)->test = (yyvsp[-2].uns);
	(yyvsp[-1].exprNode) = emitBoolean((yyvsp[-1].exprNode));

	(yyval.forNode)->enter = nextquad();
	emit(if_eq, NULL, (yyvsp[-1].exprNode), newexpr_constbool('t'), 0, yylineno);
	fprintf(yyout_y, "forprefix -> for ( \n");
}
#line 3167 "parser.c"
    break;

  case 112: /* forstmt: forprefix N elist RIGHTPAR N loopstmt N  */
#line 1472 "parser.y"
{
	patchlabel((yyvsp[-6].forNode)->enter, (yyvsp[-2].uns) + 1); // true jump
	patchlabel((yyvsp[-5].uns), nextquad());	   // false jump
	patchlabel((yyvsp[-2].uns), (yyvsp[-6].forNode)->test);	   // loop jump
	patchlabel((yyvsp[0].uns), (yyvsp[-5].uns) + 1);		   // closure jump

	patchlist((yyvsp[-1].stmtNode)->breaklist, nextquad());
	patchlist((yyvsp[-1].stmtNode)->contlist, (yyvsp[-5].uns) + 1);
	(yyval.stmtNode) = (yyvsp[-1].stmtNode);
	fprintf(yyout_y, "forstmt -> forprefix N elist ( N loopstmt N \n");
}
#line 3183 "parser.c"
    break;

  case 113: /* $@11: %empty  */
#line 1484 "parser.y"
{
	if (sim_funcs == 0)
	{
		fprintf(stderr, "Error: Return statement not inside of a function in line %d\n", yylineno);
		isError = true;
	}
}
#line 3195 "parser.c"
    break;

  case 114: /* returnstmt: RETURN $@11 SEMICOLON  */
#line 1492 "parser.y"
{
	(yyval.stmtNode) = malloc(sizeof(stmt_t));
	make_stmt((yyval.stmtNode));
	
	
	emit(returnn, NULL, NULL, NULL, -1, nextquad());
	emit(jump,NULL,NULL,NULL,0,currQuad);
	(yyval.stmtNode)->returnlist = newlist(nextquad()-1);
	fprintf(yyout_y, "returnstmt -> return ;\n");
}
#line 3210 "parser.c"
    break;

  case 115: /* $@12: %empty  */
#line 1503 "parser.y"
{
	if (sim_funcs == 0)
	{
		fprintf(stderr, "Return statement not inside of a function in line %d\n", yylineno);
		isError = true;
	}
}
#line 3222 "parser.c"
    break;

  case 116: /* returnstmt: RETURN $@12 expr SEMICOLON  */
#line 1511 "parser.y"
{
	(yyvsp[-1].exprNode) = emitBoolean((yyvsp[-1].exprNode));
	(yyval.stmtNode) = malloc(sizeof(stmt_t));
	make_stmt((yyval.stmtNode));
	emit(returnn, (yyvsp[-1].exprNode), NULL, NULL, -1, nextquad());
	emit(jump,NULL,NULL,NULL,0,currQuad);
	(yyval.stmtNode)->returnlist = newlist(nextquad()-1);
	fprintf(yyout_y, "returnstmt -> return expr ;\n");
}
#line 3236 "parser.c"
    break;


#line 3240 "parser.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
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

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
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
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
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
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

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


#if !defined yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturn;
#endif


/*-------------------------------------------------------.
| yyreturn -- parsing is finished, clean up and return.  |
`-------------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 1523 "parser.y"

int main(int argc, char** argv)
{
	stack_ = create_stack();
	loopcounterstack = create_stack();
	func_stack=create_stack();
    //programVarStack = create_stack();
    yyout_y = fopen("yacc_output.txt", "w");
    hash = SymTable_new();
    //symtable_print(head_scope_node,hash);
    if(argc > 1 ){
        if(!(yyin = fopen(argv[1], "r"))) {
            fprintf(stderr, "cannot read file: %s\n", argv[1]);
            isError = true;
            return 1;
        }
    }
    else
        yyin= stdin;
	
	expand();
	expand_v();
	emit(jump,NULL,NULL,NULL,0,0);
	
    yyparse();
    symtable_print(head_scope_node,hash);
    if(!isError){
    	printMedianCode();
	generateF();
	printInstructions();
	instrToBinary();
	readBinary();
		avm_initialize();
   
	while(executionFinished == 0)
       		 execute_cycle();	
    avm_memcellclear(&ax);
    avm_memcellclear(&bx);
    avm_memcellclear(&cx);
    avm_memcellclear(&retval);
    //return 100;
    }else{
	printf("Could not produce Median Code because of compile time errors \n");
	}
    return 0;
}
