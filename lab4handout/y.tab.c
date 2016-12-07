#ifndef lint
static const char yysccsid[] = "@(#)yaccpar	1.9 (Berkeley) 02/21/93";
#endif

#define YYBYACC 1
#define YYMAJOR 1
#define YYMINOR 9
#define YYPATCH 20140101

#define YYEMPTY        (-1)
#define yyclearin      (yychar = YYEMPTY)
#define yyerrok        (yyerrflag = 0)
#define YYRECOVERING() (yyerrflag != 0)

#define YYPREFIX "yy"

#define YYPURE 0

#line 2 "tiger.y"
#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "symbol.h"
#include "errormsg.h"
#include "absyn.h"

int yylex(void); /* function prototype */
A_exp absyn_root;

void yyerror(char *s)
{
 EM_error(EM_tokPos, "%s", s);
 exit(1);
}
#line 20 "tiger.y"
#ifdef YYSTYPE
#undef  YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
#endif
#ifndef YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
typedef union {
	int pos;
	int ival;
	string sval;
	S_symbol sym;
	A_var var;
	A_exp exp;
	/* et cetera */
	A_dec dec;
	A_ty ty;
	A_decList decList;
	A_expList expList;
	A_field field;
	A_fieldList fieldList;
	A_fundec fundec;
	A_fundecList fundecList;
	A_namety namety;
	A_nametyList nametyList;
	A_efield efield;
	A_efieldList efieldList;
} YYSTYPE;
#endif /* !YYSTYPE_IS_DECLARED */
#line 64 "y.tab.c"

/* compatibility with bison */
#ifdef YYPARSE_PARAM
/* compatibility with FreeBSD */
# ifdef YYPARSE_PARAM_TYPE
#  define YYPARSE_DECL() yyparse(YYPARSE_PARAM_TYPE YYPARSE_PARAM)
# else
#  define YYPARSE_DECL() yyparse(void *YYPARSE_PARAM)
# endif
#else
# define YYPARSE_DECL() yyparse(void)
#endif

/* Parameters sent to lex. */
#ifdef YYLEX_PARAM
# define YYLEX_DECL() yylex(void *YYLEX_PARAM)
# define YYLEX yylex(YYLEX_PARAM)
#else
# define YYLEX_DECL() yylex(void)
# define YYLEX yylex()
#endif

/* Parameters sent to yyerror. */
#ifndef YYERROR_DECL
#define YYERROR_DECL() yyerror(const char *s)
#endif
#ifndef YYERROR_CALL
#define YYERROR_CALL(msg) yyerror(msg)
#endif

extern int YYPARSE_DECL();

#define ID 257
#define STRING 258
#define INT 259
#define COMMA 260
#define COLON 261
#define SEMICOLON 262
#define LPAREN 263
#define RPAREN 264
#define LBRACK 265
#define RBRACK 266
#define LBRACE 267
#define RBRACE 268
#define DOT 269
#define PLUS 270
#define MINUS 271
#define TIMES 272
#define DIVIDE 273
#define EQ 274
#define NEQ 275
#define LT 276
#define LE 277
#define GT 278
#define GE 279
#define AND 280
#define OR 281
#define ASSIGN 282
#define ARRAY 283
#define IF 284
#define THEN 285
#define ELSE 286
#define WHILE 287
#define FOR 288
#define TO 289
#define DO 290
#define LET 291
#define IN 292
#define END 293
#define OF 294
#define BREAK 295
#define NIL 296
#define FUNCTION 297
#define VAR 298
#define TYPE 299
#define UMINUS 300
#define YYERRCODE 256
static const short yylhs[] = {                           -1,
    0,   17,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    5,    5,    5,    5,   16,   16,   15,   15,    3,    3,
    3,    4,    4,    4,    2,    7,    7,    6,    6,    6,
    6,   10,   10,   10,    9,    8,    8,    8,   11,   11,
   12,   12,   13,   14,   14,
};
static const short yylen[] = {                            2,
    1,    1,    1,    1,    1,    1,    2,    3,    3,    3,
    3,    3,    3,    3,    3,    3,    3,    3,    3,    3,
    4,    4,    3,    6,    6,    4,    4,    8,    1,    5,
    1,    3,    4,    4,    0,    3,    1,    3,    0,    1,
    3,    0,    1,    3,    1,    1,    2,    4,    6,    1,
    1,    1,    3,    3,    3,    0,    1,    3,    7,    9,
    1,    2,    4,    1,    2,
};
static const short yydefred[] = {                         0,
    2,    6,    5,    0,    0,    0,    0,    0,    0,   29,
    4,    0,    0,    0,    0,    0,    0,   45,    7,    0,
    0,    0,    0,    0,    0,    0,    0,    0,   50,    0,
   51,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    8,    0,    0,    0,    0,    0,    0,   47,    0,   62,
   65,    0,    0,   13,   14,    0,    0,    0,    0,    0,
    0,    0,    0,    0,   32,    0,    0,    0,    0,    0,
    0,    0,   44,    0,    0,    0,    0,    0,    0,    0,
    0,   34,    0,   21,    0,   22,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,   63,   52,   30,
   41,    0,   38,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   58,   55,    0,   53,
   54,    0,    0,    0,    0,    0,
};
static const short yydgoto[] = {                         12,
   16,   17,   78,   18,   14,   26,   27,  101,  102,  108,
   28,   29,   30,   31,   80,   81,   15,
};
static const short yysindex[] = {                      -144,
    0,    0,    0, -144, -144, -144, -144, -245, -193,    0,
    0,    0,  645, -260, -228,  -47, -241,    0,    0,  483,
  -93, -254, -245, -245, -245, -193, -267, -261,    0, -259,
    0, -144, -144, -144, -144, -144, -144, -144, -144, -144,
 -144, -144, -144, -144, -245, -144, -144, -144, -245, -144,
    0, -144, -144, -144, -225, -258, -213,    0, -144,    0,
    0, -149, -149,    0,    0, -202, -202, -202, -202, -202,
 -202,  685,  443,  565,    0,  645, -188, -209,  605, -191,
 -163, -167,    0,  525,  645,  -27, -245, -245, -144, -253,
 -165,    0, -144,    0, -182,    0, -245, -144, -144, -144,
 -135, -129, -128, -150,  645, -245, -159,    0,    0,    0,
    0, -144,    0,  645,  645,  -68, -255, -245, -245, -144,
 -131, -245,  645, -144, -245, -144,    0,    0,  645,    0,
    0,  645, -132,  645, -144,  645,
};
static const short yyrindex[] = {                         0,
    0,    0,    0, -118,    0,    0,    0,    0,    0,    0,
    0,    0,  149,   81,   41, -264,    0,    0,    0,    0,
    0,    0,    0,    0,    0, -142,    0, -197,    0, -172,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, -111,    0, -247, -262,
    0,    0,    0,    0,    0,    0,    0,    0, -139,    0,
    0,  121,  161,    0,    0,  201,  241,  284,  324,  364,
  404,  444,  484,    0,    0,  524, -109,    0,    0,    0,
 -112,    0,    0,  726,  566,    0, -106,    0,    0,    0,
    0,    0, -111,    0,    1,    0, -247,    0,    0,    0,
    0, -257,    0,    0, -265, -108,    0,    0,    0,    0,
    0,    0,    0, -242,  606,    0,    0, -170,    0,    0,
    0,    0,  646,    0,    0,    0,    0,    0, -232,    0,
    0,  686,    0, -219,    0, -181,
};
static const short yygindex[] = {                         0,
   10,  100,   69,  113,    0,    0,  138,  -98,    0,    0,
    0,  137,    0,  136,   70,    0,   51,
};
#define YYTABLESIZE 1025
static const short yytable[] = {                         43,
   33,   42,   88,    1,   44,  125,   57,  121,   45,   13,
   57,    1,   35,  106,   19,   20,   21,   36,  126,  127,
   35,   46,   51,   89,   59,   36,   48,   54,   43,  107,
   42,   48,   48,   48,   47,   23,   48,   87,   49,   25,
   31,   62,   63,   64,   65,   66,   67,   68,   69,   70,
   71,   72,   73,   74,   94,   76,   77,   79,   22,   49,
   90,   84,   85,   86,   49,   49,   49,   32,   33,   34,
   35,   93,   59,   55,   56,   57,   96,   59,   59,   59,
    3,   32,   33,   34,   35,   36,   37,   38,   39,   40,
   41,   42,   43,   56,   61,   75,   97,   56,  105,   82,
   61,   61,   77,   23,   24,   25,   98,  114,  115,  116,
   60,  112,    1,    2,    3,   60,   60,   60,    4,   64,
   11,  123,   34,   35,   64,   64,    5,  110,  117,  129,
  118,  120,  119,  132,  122,  134,  130,  103,  104,    6,
  109,  135,    7,    8,  136,   42,    9,   82,    1,   46,
   10,   11,   39,   42,   40,   37,  103,   56,   91,   56,
   12,  111,   83,   58,   60,   61,  113,    0,  103,  128,
    0,    0,  131,    0,    0,  133,   32,   33,   34,   35,
   36,   37,   38,   39,   40,   41,   42,   43,    0,    0,
    0,    0,    0,    0,    0,    0,   53,    0,    0,    0,
   15,   32,   33,   34,   35,   36,   37,   38,   39,   40,
   41,   42,   43,    0,   50,    0,    0,    0,    0,    0,
    0,  124,   32,   33,   34,   35,   36,   37,   38,   39,
   40,   41,   42,   43,    0,    0,    0,    0,    0,    0,
   16,    0,   32,   33,   34,   35,   36,   37,   38,   39,
   40,   41,   42,   43,    0,    0,    0,    0,    0,    0,
   33,  100,   33,    0,   33,   33,   33,    0,   33,   33,
   33,   33,   33,   33,   33,   33,   33,   33,   33,   33,
   33,   33,   33,   17,    0,   33,   33,    0,    0,   33,
   33,    0,   33,   33,    0,    0,    0,   33,   33,   33,
   31,    0,   31,    0,   31,    0,   31,    0,   31,   31,
   31,   31,   31,   31,   31,   31,   31,   31,   31,   31,
   31,   31,   31,   19,    0,   31,   31,    0,    0,   31,
   31,    0,   31,   31,    0,    0,    0,   31,   31,   31,
    3,    0,    3,    0,    3,    0,    3,    0,    3,    0,
    3,    3,    3,    3,    3,    3,    3,    3,    3,    3,
    3,    3,    0,   18,    0,    3,    3,    0,    0,    3,
    3,    0,    3,    3,    0,    0,    0,    3,    3,    3,
   11,    0,   11,    0,   11,    0,   11,    0,   11,    0,
   11,   11,    0,    0,   11,   11,   11,   11,   11,   11,
   11,   11,    0,   20,    0,   11,   11,    0,    0,   11,
   11,    0,   11,   11,    0,    0,    0,   11,   11,   11,
   12,    0,   12,    0,   12,    0,   12,    0,   12,    0,
   12,   12,    0,    0,   12,   12,   12,   12,   12,   12,
   12,   12,    0,   10,    0,   12,   12,    0,    0,   12,
   12,    0,   12,   12,    0,    0,    0,   12,   12,   12,
   15,    0,   15,    0,   15,    0,   15,    0,   15,    0,
    0,    0,    0,    0,   15,   15,   15,   15,   15,   15,
   15,   15,    0,    9,    0,   15,   15,    0,    0,   15,
   15,    0,   15,   15,    0,    0,    0,   15,   15,   15,
   16,    0,   16,    0,   16,    0,   16,    0,   16,    0,
    0,    0,    0,    0,   16,   16,   16,   16,   16,   16,
   16,   16,    0,   23,    0,   16,   16,    0,    0,   16,
   16,    0,   16,   16,    0,    0,    0,   16,   16,   16,
    0,    0,    0,   17,    0,   17,    0,   17,    0,   17,
    0,   17,    0,    0,    0,    0,    0,   17,   17,   17,
   17,   17,   17,   17,   17,   27,    0,    0,   17,   17,
    0,    0,   17,   17,    0,   17,   17,    0,    0,    0,
   17,   17,   17,   19,    0,   19,    0,   19,    0,   19,
    0,   19,    0,    0,    0,    0,    0,   19,   19,   19,
   19,   19,   19,   19,   19,   25,    0,    0,   19,   19,
    0,    0,   19,   19,    0,   19,   19,    0,    0,    0,
   19,   19,   19,   18,    0,   18,    0,   18,    0,   18,
    0,   18,    0,    0,    0,    0,    0,   18,   18,   18,
   18,   18,   18,   18,   18,   24,    0,    0,   18,   18,
    0,    0,   18,   18,    0,   18,   18,    0,    0,    0,
   18,   18,   18,   20,    0,   20,    0,   20,    0,   20,
    0,   20,    0,    0,    0,    0,    0,   20,   20,   20,
   20,   20,   20,   20,   20,   28,    0,    0,   20,   20,
    0,    0,   20,   20,    0,   20,   20,    0,    0,    0,
   20,   20,   20,   10,    0,   10,    0,   10,    0,   10,
    0,   10,   32,   33,   34,   35,   36,   37,   38,   39,
   40,   41,   42,   10,   10,   26,    0,    0,   10,   10,
    0,    0,   10,   10,    0,   10,   10,    0,    0,    0,
   10,   10,   10,    9,    0,    9,    0,    9,    0,    9,
    0,    9,   32,   33,   34,   35,   36,   37,   38,   39,
   40,   41,   42,   43,    9,    0,    0,   52,    9,    9,
    0,    0,    9,    9,    0,    9,    9,    0,    0,    0,
    9,    9,    9,   23,    0,   23,    0,   23,    0,   23,
    0,   23,    0,    0,   32,   33,   34,   35,   36,   37,
   38,   39,   40,   41,   42,   43,    0,    0,   23,   23,
   99,    0,   23,   23,    0,   23,   23,    0,    0,    0,
   23,   23,   23,    0,    0,   27,    0,   27,    0,   27,
   92,   27,    0,   27,   32,   33,   34,   35,   36,   37,
   38,   39,   40,   41,   42,   43,    0,    0,    0,    0,
   27,   27,    0,    0,   27,   27,    0,   27,   27,    0,
    0,    0,   27,   27,   27,   25,    0,   25,    0,   25,
   95,   25,    0,   25,   32,   33,   34,   35,   36,   37,
   38,   39,   40,   41,   42,   43,    0,    0,    0,    0,
   25,   25,    0,    0,   25,   25,    0,   25,   25,    0,
    0,    0,   25,   25,   25,   24,    0,   24,    0,   24,
    0,   24,    0,   24,   32,   33,   34,   35,   36,   37,
   38,   39,   40,   41,   42,   43,    0,    0,    0,    0,
   24,   24,    0,    0,   24,   24,    0,   24,   24,    0,
    0,    0,   24,   24,   24,   28,    0,   28,    0,   28,
    0,   28,    0,   28,   32,   33,   34,   35,   36,   37,
   38,   39,   40,   41,    0,    0,    0,    0,    0,    0,
   28,   28,    0,    0,   28,   28,    0,   28,   28,    0,
    0,    0,   28,   28,   28,   26,    0,   26,    0,   26,
    0,   26,    0,   26,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
   26,    0,    0,    0,   26,   26,    0,   26,   26,    0,
    0,    0,   26,   26,   26,
};
static const short yycheck[] = {                        264,
    0,  264,  261,  257,  265,  261,  264,  106,  269,    0,
  268,  257,  260,  267,    5,    6,    7,  260,  274,  118,
  268,  282,  264,  282,  292,  268,  292,  282,  293,  283,
  293,  297,  298,  299,  263,  297,  265,  263,  267,  299,
    0,   32,   33,   34,   35,   36,   37,   38,   39,   40,
   41,   42,   43,   44,  264,   46,   47,   48,    8,  292,
  274,   52,   53,   54,  297,  298,  299,  270,  271,  272,
  273,  260,  292,   23,   24,   25,  268,  297,  298,  299,
    0,  270,  271,  272,  273,  274,  275,  276,  277,  278,
  279,  280,  281,  264,  292,   45,  260,  268,   89,   49,
  298,  299,   93,  297,  298,  299,  274,   98,   99,  100,
  292,  294,  257,  258,  259,  297,  298,  299,  263,  292,
    0,  112,  272,  273,  297,  298,  271,  293,  264,  120,
  260,  282,  261,  124,  294,  126,  268,   87,   88,  284,
   90,  274,  287,  288,  135,  264,  291,   97,    0,  292,
  295,  296,  264,  293,  264,  268,  106,  264,   59,  268,
    0,   93,   50,   26,   28,   30,   97,   -1,  118,  119,
   -1,   -1,  122,   -1,   -1,  125,  270,  271,  272,  273,
  274,  275,  276,  277,  278,  279,  280,  281,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  290,   -1,   -1,   -1,
    0,  270,  271,  272,  273,  274,  275,  276,  277,  278,
  279,  280,  281,   -1,  262,   -1,   -1,   -1,   -1,   -1,
   -1,  290,  270,  271,  272,  273,  274,  275,  276,  277,
  278,  279,  280,  281,   -1,   -1,   -1,   -1,   -1,   -1,
    0,   -1,  270,  271,  272,  273,  274,  275,  276,  277,
  278,  279,  280,  281,   -1,   -1,   -1,   -1,   -1,   -1,
  260,  289,  262,   -1,  264,  265,  266,   -1,  268,  269,
  270,  271,  272,  273,  274,  275,  276,  277,  278,  279,
  280,  281,  282,    0,   -1,  285,  286,   -1,   -1,  289,
  290,   -1,  292,  293,   -1,   -1,   -1,  297,  298,  299,
  260,   -1,  262,   -1,  264,   -1,  266,   -1,  268,  269,
  270,  271,  272,  273,  274,  275,  276,  277,  278,  279,
  280,  281,  282,    0,   -1,  285,  286,   -1,   -1,  289,
  290,   -1,  292,  293,   -1,   -1,   -1,  297,  298,  299,
  260,   -1,  262,   -1,  264,   -1,  266,   -1,  268,   -1,
  270,  271,  272,  273,  274,  275,  276,  277,  278,  279,
  280,  281,   -1,    0,   -1,  285,  286,   -1,   -1,  289,
  290,   -1,  292,  293,   -1,   -1,   -1,  297,  298,  299,
  260,   -1,  262,   -1,  264,   -1,  266,   -1,  268,   -1,
  270,  271,   -1,   -1,  274,  275,  276,  277,  278,  279,
  280,  281,   -1,    0,   -1,  285,  286,   -1,   -1,  289,
  290,   -1,  292,  293,   -1,   -1,   -1,  297,  298,  299,
  260,   -1,  262,   -1,  264,   -1,  266,   -1,  268,   -1,
  270,  271,   -1,   -1,  274,  275,  276,  277,  278,  279,
  280,  281,   -1,    0,   -1,  285,  286,   -1,   -1,  289,
  290,   -1,  292,  293,   -1,   -1,   -1,  297,  298,  299,
  260,   -1,  262,   -1,  264,   -1,  266,   -1,  268,   -1,
   -1,   -1,   -1,   -1,  274,  275,  276,  277,  278,  279,
  280,  281,   -1,    0,   -1,  285,  286,   -1,   -1,  289,
  290,   -1,  292,  293,   -1,   -1,   -1,  297,  298,  299,
  260,   -1,  262,   -1,  264,   -1,  266,   -1,  268,   -1,
   -1,   -1,   -1,   -1,  274,  275,  276,  277,  278,  279,
  280,  281,   -1,    0,   -1,  285,  286,   -1,   -1,  289,
  290,   -1,  292,  293,   -1,   -1,   -1,  297,  298,  299,
   -1,   -1,   -1,  260,   -1,  262,   -1,  264,   -1,  266,
   -1,  268,   -1,   -1,   -1,   -1,   -1,  274,  275,  276,
  277,  278,  279,  280,  281,    0,   -1,   -1,  285,  286,
   -1,   -1,  289,  290,   -1,  292,  293,   -1,   -1,   -1,
  297,  298,  299,  260,   -1,  262,   -1,  264,   -1,  266,
   -1,  268,   -1,   -1,   -1,   -1,   -1,  274,  275,  276,
  277,  278,  279,  280,  281,    0,   -1,   -1,  285,  286,
   -1,   -1,  289,  290,   -1,  292,  293,   -1,   -1,   -1,
  297,  298,  299,  260,   -1,  262,   -1,  264,   -1,  266,
   -1,  268,   -1,   -1,   -1,   -1,   -1,  274,  275,  276,
  277,  278,  279,  280,  281,    0,   -1,   -1,  285,  286,
   -1,   -1,  289,  290,   -1,  292,  293,   -1,   -1,   -1,
  297,  298,  299,  260,   -1,  262,   -1,  264,   -1,  266,
   -1,  268,   -1,   -1,   -1,   -1,   -1,  274,  275,  276,
  277,  278,  279,  280,  281,    0,   -1,   -1,  285,  286,
   -1,   -1,  289,  290,   -1,  292,  293,   -1,   -1,   -1,
  297,  298,  299,  260,   -1,  262,   -1,  264,   -1,  266,
   -1,  268,  270,  271,  272,  273,  274,  275,  276,  277,
  278,  279,  280,  280,  281,    0,   -1,   -1,  285,  286,
   -1,   -1,  289,  290,   -1,  292,  293,   -1,   -1,   -1,
  297,  298,  299,  260,   -1,  262,   -1,  264,   -1,  266,
   -1,  268,  270,  271,  272,  273,  274,  275,  276,  277,
  278,  279,  280,  281,  281,   -1,   -1,  285,  285,  286,
   -1,   -1,  289,  290,   -1,  292,  293,   -1,   -1,   -1,
  297,  298,  299,  260,   -1,  262,   -1,  264,   -1,  266,
   -1,  268,   -1,   -1,  270,  271,  272,  273,  274,  275,
  276,  277,  278,  279,  280,  281,   -1,   -1,  285,  286,
  286,   -1,  289,  290,   -1,  292,  293,   -1,   -1,   -1,
  297,  298,  299,   -1,   -1,  260,   -1,  262,   -1,  264,
  266,  266,   -1,  268,  270,  271,  272,  273,  274,  275,
  276,  277,  278,  279,  280,  281,   -1,   -1,   -1,   -1,
  285,  286,   -1,   -1,  289,  290,   -1,  292,  293,   -1,
   -1,   -1,  297,  298,  299,  260,   -1,  262,   -1,  264,
  266,  266,   -1,  268,  270,  271,  272,  273,  274,  275,
  276,  277,  278,  279,  280,  281,   -1,   -1,   -1,   -1,
  285,  286,   -1,   -1,  289,  290,   -1,  292,  293,   -1,
   -1,   -1,  297,  298,  299,  260,   -1,  262,   -1,  264,
   -1,  266,   -1,  268,  270,  271,  272,  273,  274,  275,
  276,  277,  278,  279,  280,  281,   -1,   -1,   -1,   -1,
  285,  286,   -1,   -1,  289,  290,   -1,  292,  293,   -1,
   -1,   -1,  297,  298,  299,  260,   -1,  262,   -1,  264,
   -1,  266,   -1,  268,  270,  271,  272,  273,  274,  275,
  276,  277,  278,  279,   -1,   -1,   -1,   -1,   -1,   -1,
  285,  286,   -1,   -1,  289,  290,   -1,  292,  293,   -1,
   -1,   -1,  297,  298,  299,  260,   -1,  262,   -1,  264,
   -1,  266,   -1,  268,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  285,   -1,   -1,   -1,  289,  290,   -1,  292,  293,   -1,
   -1,   -1,  297,  298,  299,
};
#define YYFINAL 12
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 300
#define YYTRANSLATE(a) ((a) > YYMAXTOKEN ? (YYMAXTOKEN + 1) : (a))
#if YYDEBUG
static const char *yyname[] = {

"end-of-file",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"ID","STRING","INT","COMMA",
"COLON","SEMICOLON","LPAREN","RPAREN","LBRACK","RBRACK","LBRACE","RBRACE","DOT",
"PLUS","MINUS","TIMES","DIVIDE","EQ","NEQ","LT","LE","GT","GE","AND","OR",
"ASSIGN","ARRAY","IF","THEN","ELSE","WHILE","FOR","TO","DO","LET","IN","END",
"OF","BREAK","NIL","FUNCTION","VAR","TYPE","UMINUS","illegal-symbol",
};
static const char *yyrule[] = {
"$accept : program",
"program : exp",
"id : ID",
"exp : lvalue",
"exp : NIL",
"exp : INT",
"exp : STRING",
"exp : MINUS exp",
"exp : LPAREN seqexp RPAREN",
"exp : exp OR exp",
"exp : exp AND exp",
"exp : exp PLUS exp",
"exp : exp MINUS exp",
"exp : exp TIMES exp",
"exp : exp DIVIDE exp",
"exp : exp EQ exp",
"exp : exp NEQ exp",
"exp : exp LT exp",
"exp : exp GT exp",
"exp : exp LE exp",
"exp : exp GE exp",
"exp : id LPAREN explist RPAREN",
"exp : id LBRACE efieldlist RBRACE",
"exp : lvalue ASSIGN exp",
"exp : id LBRACK exp RBRACK OF exp",
"exp : IF exp THEN exp ELSE exp",
"exp : IF exp THEN exp",
"exp : WHILE exp DO exp",
"exp : FOR id ASSIGN exp TO exp DO exp",
"exp : BREAK",
"exp : LET declist IN seqexp END",
"lvalue : id",
"lvalue : lvalue DOT id",
"lvalue : id LBRACK exp RBRACK",
"lvalue : lvalue LBRACK exp RBRACK",
"efield :",
"efield : id EQ exp",
"efieldlist : efield",
"efieldlist : efield COMMA efieldlist",
"explist :",
"explist : exp",
"explist : exp COMMA explist",
"expseq :",
"expseq : exp",
"expseq : exp SEMICOLON expseq",
"seqexp : expseq",
"declist : dec",
"declist : dec declist",
"dec : VAR id ASSIGN exp",
"dec : VAR id COLON id ASSIGN exp",
"dec : fundeclist",
"dec : tydeclist",
"ty : id",
"ty : LBRACE tyfields RBRACE",
"ty : ARRAY OF id",
"tyfield : id COLON id",
"tyfields :",
"tyfields : tyfield",
"tyfields : tyfield COMMA tyfields",
"fundec : FUNCTION id LPAREN tyfields RPAREN EQ exp",
"fundec : FUNCTION id LPAREN tyfields RPAREN COLON id EQ exp",
"fundeclist : fundec",
"fundeclist : fundec fundeclist",
"tydec : TYPE id EQ ty",
"tydeclist : tydec",
"tydeclist : tydec tydeclist",

};
#endif

int      yydebug;
int      yynerrs;

int      yyerrflag;
int      yychar;
YYSTYPE  yyval;
YYSTYPE  yylval;

/* define the initial stack-sizes */
#ifdef YYSTACKSIZE
#undef YYMAXDEPTH
#define YYMAXDEPTH  YYSTACKSIZE
#else
#ifdef YYMAXDEPTH
#define YYSTACKSIZE YYMAXDEPTH
#else
#define YYSTACKSIZE 10000
#define YYMAXDEPTH  10000
#endif
#endif

#define YYINITSTACKSIZE 200

typedef struct {
    unsigned stacksize;
    short    *s_base;
    short    *s_mark;
    short    *s_last;
    YYSTYPE  *l_base;
    YYSTYPE  *l_mark;
} YYSTACKDATA;
/* variables for the parser stack */
static YYSTACKDATA yystack;

#if YYDEBUG
#include <stdio.h>		/* needed for printf */
#endif

#include <stdlib.h>	/* needed for malloc, etc */
#include <string.h>	/* needed for memset */

/* allocate initial stack or double stack size, up to YYMAXDEPTH */
static int yygrowstack(YYSTACKDATA *data)
{
    int i;
    unsigned newsize;
    short *newss;
    YYSTYPE *newvs;

    if ((newsize = data->stacksize) == 0)
        newsize = YYINITSTACKSIZE;
    else if (newsize >= YYMAXDEPTH)
        return -1;
    else if ((newsize *= 2) > YYMAXDEPTH)
        newsize = YYMAXDEPTH;

    i = (int) (data->s_mark - data->s_base);
    newss = (short *)realloc(data->s_base, newsize * sizeof(*newss));
    if (newss == 0)
        return -1;

    data->s_base = newss;
    data->s_mark = newss + i;

    newvs = (YYSTYPE *)realloc(data->l_base, newsize * sizeof(*newvs));
    if (newvs == 0)
        return -1;

    data->l_base = newvs;
    data->l_mark = newvs + i;

    data->stacksize = newsize;
    data->s_last = data->s_base + newsize - 1;
    return 0;
}

#if YYPURE || defined(YY_NO_LEAKS)
static void yyfreestack(YYSTACKDATA *data)
{
    free(data->s_base);
    free(data->l_base);
    memset(data, 0, sizeof(*data));
}
#else
#define yyfreestack(data) /* nothing */
#endif

#define YYABORT  goto yyabort
#define YYREJECT goto yyabort
#define YYACCEPT goto yyaccept
#define YYERROR  goto yyerrlab

int
YYPARSE_DECL()
{
    int yym, yyn, yystate;
#if YYDEBUG
    const char *yys;

    if ((yys = getenv("YYDEBUG")) != 0)
    {
        yyn = *yys;
        if (yyn >= '0' && yyn <= '9')
            yydebug = yyn - '0';
    }
#endif

    yynerrs = 0;
    yyerrflag = 0;
    yychar = YYEMPTY;
    yystate = 0;

#if YYPURE
    memset(&yystack, 0, sizeof(yystack));
#endif

    if (yystack.s_base == NULL && yygrowstack(&yystack)) goto yyoverflow;
    yystack.s_mark = yystack.s_base;
    yystack.l_mark = yystack.l_base;
    yystate = 0;
    *yystack.s_mark = 0;

yyloop:
    if ((yyn = yydefred[yystate]) != 0) goto yyreduce;
    if (yychar < 0)
    {
        if ((yychar = YYLEX) < 0) yychar = 0;
#if YYDEBUG
        if (yydebug)
        {
            yys = yyname[YYTRANSLATE(yychar)];
            printf("%sdebug: state %d, reading %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
    }
    if ((yyn = yysindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: state %d, shifting to state %d\n",
                    YYPREFIX, yystate, yytable[yyn]);
#endif
        if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack))
        {
            goto yyoverflow;
        }
        yystate = yytable[yyn];
        *++yystack.s_mark = yytable[yyn];
        *++yystack.l_mark = yylval;
        yychar = YYEMPTY;
        if (yyerrflag > 0)  --yyerrflag;
        goto yyloop;
    }
    if ((yyn = yyrindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
        yyn = yytable[yyn];
        goto yyreduce;
    }
    if (yyerrflag) goto yyinrecovery;

    yyerror("syntax error");

    goto yyerrlab;

yyerrlab:
    ++yynerrs;

yyinrecovery:
    if (yyerrflag < 3)
    {
        yyerrflag = 3;
        for (;;)
        {
            if ((yyn = yysindex[*yystack.s_mark]) && (yyn += YYERRCODE) >= 0 &&
                    yyn <= YYTABLESIZE && yycheck[yyn] == YYERRCODE)
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: state %d, error recovery shifting\
 to state %d\n", YYPREFIX, *yystack.s_mark, yytable[yyn]);
#endif
                if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack))
                {
                    goto yyoverflow;
                }
                yystate = yytable[yyn];
                *++yystack.s_mark = yytable[yyn];
                *++yystack.l_mark = yylval;
                goto yyloop;
            }
            else
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: error recovery discarding state %d\n",
                            YYPREFIX, *yystack.s_mark);
#endif
                if (yystack.s_mark <= yystack.s_base) goto yyabort;
                --yystack.s_mark;
                --yystack.l_mark;
            }
        }
    }
    else
    {
        if (yychar == 0) goto yyabort;
#if YYDEBUG
        if (yydebug)
        {
            yys = yyname[YYTRANSLATE(yychar)];
            printf("%sdebug: state %d, error recovery discards token %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
        yychar = YYEMPTY;
        goto yyloop;
    }

yyreduce:
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: state %d, reducing by rule %d (%s)\n",
                YYPREFIX, yystate, yyn, yyrule[yyn]);
#endif
    yym = yylen[yyn];
    if (yym)
        yyval = yystack.l_mark[1-yym];
    else
        memset(&yyval, 0, sizeof yyval);
    switch (yyn)
    {
case 1:
#line 81 "tiger.y"
	{absyn_root=yystack.l_mark[0].exp;}
break;
case 2:
#line 83 "tiger.y"
	{yyval.sym=S_Symbol(yystack.l_mark[0].sval);}
break;
case 3:
#line 85 "tiger.y"
	{yyval.exp=A_VarExp(EM_tokPos, yystack.l_mark[0].var);}
break;
case 4:
#line 86 "tiger.y"
	{yyval.exp=A_NilExp(EM_tokPos);}
break;
case 5:
#line 87 "tiger.y"
	{yyval.exp=A_IntExp(EM_tokPos, yystack.l_mark[0].ival);}
break;
case 6:
#line 88 "tiger.y"
	{yyval.exp=A_StringExp(EM_tokPos, yystack.l_mark[0].sval);}
break;
case 7:
#line 89 "tiger.y"
	{yyval.exp=A_OpExp(EM_tokPos, A_minusOp, A_IntExp(EM_tokPos, 0), yystack.l_mark[0].exp);}
break;
case 8:
#line 92 "tiger.y"
	{yyval.exp=yystack.l_mark[-1].exp;}
break;
case 9:
#line 95 "tiger.y"
	{yyval.exp=A_IfExp(EM_tokPos, yystack.l_mark[-2].exp, A_IntExp(EM_tokPos,1), yystack.l_mark[0].exp);}
break;
case 10:
#line 96 "tiger.y"
	{yyval.exp=A_IfExp(EM_tokPos, yystack.l_mark[-2].exp, yystack.l_mark[0].exp, A_IntExp(EM_tokPos,0));}
break;
case 11:
#line 99 "tiger.y"
	{yyval.exp=A_OpExp(EM_tokPos, A_plusOp, yystack.l_mark[-2].exp, yystack.l_mark[0].exp);}
break;
case 12:
#line 100 "tiger.y"
	{yyval.exp=A_OpExp(EM_tokPos, A_minusOp, yystack.l_mark[-2].exp, yystack.l_mark[0].exp);}
break;
case 13:
#line 101 "tiger.y"
	{yyval.exp=A_OpExp(EM_tokPos, A_timesOp, yystack.l_mark[-2].exp, yystack.l_mark[0].exp);}
break;
case 14:
#line 102 "tiger.y"
	{yyval.exp=A_OpExp(EM_tokPos, A_divideOp, yystack.l_mark[-2].exp, yystack.l_mark[0].exp);}
break;
case 15:
#line 103 "tiger.y"
	{yyval.exp=A_OpExp(EM_tokPos, A_eqOp, yystack.l_mark[-2].exp, yystack.l_mark[0].exp);}
break;
case 16:
#line 104 "tiger.y"
	{yyval.exp=A_OpExp(EM_tokPos, A_neqOp, yystack.l_mark[-2].exp, yystack.l_mark[0].exp);}
break;
case 17:
#line 105 "tiger.y"
	{yyval.exp=A_OpExp(EM_tokPos, A_ltOp, yystack.l_mark[-2].exp, yystack.l_mark[0].exp);}
break;
case 18:
#line 106 "tiger.y"
	{yyval.exp=A_OpExp(EM_tokPos, A_gtOp, yystack.l_mark[-2].exp, yystack.l_mark[0].exp);}
break;
case 19:
#line 107 "tiger.y"
	{yyval.exp=A_OpExp(EM_tokPos, A_leOp, yystack.l_mark[-2].exp, yystack.l_mark[0].exp);}
break;
case 20:
#line 108 "tiger.y"
	{yyval.exp=A_OpExp(EM_tokPos, A_geOp, yystack.l_mark[-2].exp, yystack.l_mark[0].exp);}
break;
case 21:
#line 111 "tiger.y"
	{yyval.exp=A_CallExp(EM_tokPos, yystack.l_mark[-3].sym, yystack.l_mark[-1].expList);}
break;
case 22:
#line 113 "tiger.y"
	{yyval.exp=A_RecordExp(EM_tokPos, yystack.l_mark[-3].sym, yystack.l_mark[-1].efieldList);}
break;
case 23:
#line 115 "tiger.y"
	{yyval.exp=A_AssignExp(EM_tokPos, yystack.l_mark[-2].var, yystack.l_mark[0].exp);}
break;
case 24:
#line 117 "tiger.y"
	{yyval.exp=A_ArrayExp(EM_tokPos, yystack.l_mark[-5].sym, yystack.l_mark[-3].exp, yystack.l_mark[0].exp);}
break;
case 25:
#line 119 "tiger.y"
	{yyval.exp=A_IfExp(EM_tokPos, yystack.l_mark[-4].exp, yystack.l_mark[-2].exp, yystack.l_mark[0].exp);}
break;
case 26:
#line 120 "tiger.y"
	{yyval.exp=A_IfExp(EM_tokPos, yystack.l_mark[-2].exp, yystack.l_mark[0].exp, A_NilExp(EM_tokPos));}
break;
case 27:
#line 122 "tiger.y"
	{yyval.exp=A_WhileExp(EM_tokPos, yystack.l_mark[-2].exp, yystack.l_mark[0].exp);}
break;
case 28:
#line 124 "tiger.y"
	{yyval.exp=A_ForExp(EM_tokPos, yystack.l_mark[-6].sym, yystack.l_mark[-4].exp, yystack.l_mark[-2].exp, yystack.l_mark[0].exp);}
break;
case 29:
#line 126 "tiger.y"
	{yyval.exp=A_BreakExp(EM_tokPos);}
break;
case 30:
#line 128 "tiger.y"
	{yyval.exp=A_LetExp(EM_tokPos, yystack.l_mark[-3].decList,yystack.l_mark[-1].exp);}
break;
case 31:
#line 133 "tiger.y"
	{yyval.var=A_SimpleVar(EM_tokPos, yystack.l_mark[0].sym);}
break;
case 32:
#line 134 "tiger.y"
	{yyval.var=A_FieldVar(EM_tokPos, yystack.l_mark[-2].var, yystack.l_mark[0].sym);}
break;
case 33:
#line 135 "tiger.y"
	{yyval.var=A_SubscriptVar(EM_tokPos,A_SimpleVar(EM_tokPos, yystack.l_mark[-3].sym), yystack.l_mark[-1].exp);}
break;
case 34:
#line 136 "tiger.y"
	{yyval.var=A_SubscriptVar(EM_tokPos, yystack.l_mark[-3].var, yystack.l_mark[-1].exp);}
break;
case 35:
#line 141 "tiger.y"
	{yyval.efield=NULL;}
break;
case 36:
#line 142 "tiger.y"
	{yyval.efield=A_Efield(yystack.l_mark[-2].sym, yystack.l_mark[0].exp);}
break;
case 37:
#line 144 "tiger.y"
	{if(yystack.l_mark[0].efield==NULL) yyval.efieldList = NULL; else yyval.efieldList=A_EfieldList(yystack.l_mark[0].efield, NULL);}
break;
case 38:
#line 145 "tiger.y"
	{yyval.efieldList=A_EfieldList(yystack.l_mark[-2].efield, yystack.l_mark[0].efieldList);}
break;
case 39:
#line 148 "tiger.y"
	{yyval.expList=NULL;}
break;
case 40:
#line 149 "tiger.y"
	{yyval.expList=A_ExpList(yystack.l_mark[0].exp, NULL);}
break;
case 41:
#line 150 "tiger.y"
	{yyval.expList=A_ExpList(yystack.l_mark[-2].exp, yystack.l_mark[0].expList);}
break;
case 42:
#line 152 "tiger.y"
	{yyval.expList=NULL;}
break;
case 43:
#line 153 "tiger.y"
	{yyval.expList=A_ExpList(yystack.l_mark[0].exp, NULL);}
break;
case 44:
#line 154 "tiger.y"
	{yyval.expList=A_ExpList(yystack.l_mark[-2].exp, yystack.l_mark[0].expList);}
break;
case 45:
#line 156 "tiger.y"
	{yyval.exp=A_SeqExp(EM_tokPos, yystack.l_mark[0].expList);}
break;
case 46:
#line 159 "tiger.y"
	{yyval.decList=A_DecList(yystack.l_mark[0].dec, NULL);}
break;
case 47:
#line 160 "tiger.y"
	{yyval.decList=A_DecList(yystack.l_mark[-1].dec, yystack.l_mark[0].decList);}
break;
case 48:
#line 162 "tiger.y"
	{yyval.dec=A_VarDec(EM_tokPos, yystack.l_mark[-2].sym, NULL, yystack.l_mark[0].exp);}
break;
case 49:
#line 163 "tiger.y"
	{yyval.dec=A_VarDec(EM_tokPos, yystack.l_mark[-4].sym, yystack.l_mark[-2].sym, yystack.l_mark[0].exp);}
break;
case 50:
#line 164 "tiger.y"
	{yyval.dec=A_FunctionDec(EM_tokPos, yystack.l_mark[0].fundecList);}
break;
case 51:
#line 165 "tiger.y"
	{yyval.dec=A_TypeDec(EM_tokPos,yystack.l_mark[0].nametyList);}
break;
case 52:
#line 169 "tiger.y"
	{yyval.ty=A_NameTy(EM_tokPos, yystack.l_mark[0].sym);}
break;
case 53:
#line 170 "tiger.y"
	{yyval.ty=A_RecordTy(EM_tokPos, yystack.l_mark[-1].fieldList);}
break;
case 54:
#line 171 "tiger.y"
	{yyval.ty=A_ArrayTy(EM_tokPos, yystack.l_mark[0].sym);}
break;
case 55:
#line 176 "tiger.y"
	{yyval.field=A_Field(EM_tokPos, yystack.l_mark[-2].sym, yystack.l_mark[0].sym);}
break;
case 56:
#line 178 "tiger.y"
	{yyval.fieldList=NULL;}
break;
case 57:
#line 179 "tiger.y"
	{yyval.fieldList=A_FieldList(yystack.l_mark[0].field, NULL);}
break;
case 58:
#line 180 "tiger.y"
	{yyval.fieldList=A_FieldList(yystack.l_mark[-2].field, yystack.l_mark[0].fieldList);}
break;
case 59:
#line 185 "tiger.y"
	{yyval.fundec=A_Fundec(EM_tokPos,yystack.l_mark[-5].sym, yystack.l_mark[-3].fieldList, NULL, yystack.l_mark[0].exp);}
break;
case 60:
#line 186 "tiger.y"
	{yyval.fundec=A_Fundec(EM_tokPos,yystack.l_mark[-7].sym, yystack.l_mark[-5].fieldList, yystack.l_mark[-2].sym, yystack.l_mark[0].exp);}
break;
case 61:
#line 188 "tiger.y"
	{yyval.fundecList=A_FundecList(yystack.l_mark[0].fundec, NULL);}
break;
case 62:
#line 189 "tiger.y"
	{yyval.fundecList=A_FundecList(yystack.l_mark[-1].fundec, yystack.l_mark[0].fundecList);}
break;
case 63:
#line 192 "tiger.y"
	{yyval.namety=A_Namety(yystack.l_mark[-2].sym, yystack.l_mark[0].ty);}
break;
case 64:
#line 194 "tiger.y"
	{yyval.nametyList=A_NametyList(yystack.l_mark[0].namety, NULL);}
break;
case 65:
#line 195 "tiger.y"
	{yyval.nametyList=A_NametyList(yystack.l_mark[-1].namety, yystack.l_mark[0].nametyList);}
break;
#line 1012 "y.tab.c"
    }
    yystack.s_mark -= yym;
    yystate = *yystack.s_mark;
    yystack.l_mark -= yym;
    yym = yylhs[yyn];
    if (yystate == 0 && yym == 0)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: after reduction, shifting from state 0 to\
 state %d\n", YYPREFIX, YYFINAL);
#endif
        yystate = YYFINAL;
        *++yystack.s_mark = YYFINAL;
        *++yystack.l_mark = yyval;
        if (yychar < 0)
        {
            if ((yychar = YYLEX) < 0) yychar = 0;
#if YYDEBUG
            if (yydebug)
            {
                yys = yyname[YYTRANSLATE(yychar)];
                printf("%sdebug: state %d, reading %d (%s)\n",
                        YYPREFIX, YYFINAL, yychar, yys);
            }
#endif
        }
        if (yychar == 0) goto yyaccept;
        goto yyloop;
    }
    if ((yyn = yygindex[yym]) && (yyn += yystate) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yystate)
        yystate = yytable[yyn];
    else
        yystate = yydgoto[yym];
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: after reduction, shifting from state %d \
to state %d\n", YYPREFIX, *yystack.s_mark, yystate);
#endif
    if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack))
    {
        goto yyoverflow;
    }
    *++yystack.s_mark = (short) yystate;
    *++yystack.l_mark = yyval;
    goto yyloop;

yyoverflow:
    yyerror("yacc stack overflow");

yyabort:
    yyfreestack(&yystack);
    return (1);

yyaccept:
    yyfreestack(&yystack);
    return (0);
}
