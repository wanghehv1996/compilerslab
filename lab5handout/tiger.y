%{
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
%}


%union {
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
}

%type <exp> exp seqexp
%type <expList> explist expseq
%type <var> lvalue
%type <dec> dec
%type <decList> declist
%type <fieldList> tyfields //follow the name of fulu
%type <field> tyfield
%type <ty> ty
%type <fundec> fundec
%type <fundecList> fundeclist
%type <namety> tydec
%type <nametyList> tydeclist
%type <efieldList> efieldlist
%type <efield> efield
%type <sym> id

%token <sval> ID STRING
%token <ival> INT

%token
  COMMA COLON SEMICOLON LPAREN RPAREN LBRACK RBRACK
  LBRACE RBRACE DOT
  PLUS MINUS TIMES DIVIDE EQ NEQ LT LE GT GE
  AND OR ASSIGN
  ARRAY IF THEN ELSE WHILE FOR TO DO LET IN END OF
  BREAK NIL
  FUNCTION VAR TYPE

%nonassoc THEN
%nonassoc DO LBRACK ELSE OF ASSIGN FUNCTION TYPE 
%left OR
%left AND
%left EQ NEQ LE LT GT GE
%left PLUS MINUS
%left TIMES DIVIDE
%left UMINUS

%start program
%%
program:    exp                    {absyn_root=$1;}

id:   ID                           {$$=S_Symbol($1);}

exp:	lvalue                       {$$=A_VarExp(EM_tokPos, $1);}
			|NIL                         {$$=A_NilExp(EM_tokPos);}
			|INT                         {$$=A_IntExp(EM_tokPos, $1);}
			|STRING                      {$$=A_StringExp(EM_tokPos, $1);}
			|MINUS exp %prec UMINUS      {$$=A_OpExp(EM_tokPos, A_minusOp, A_IntExp(EM_tokPos, 0), $2);}

/* seq exp */
			|LPAREN seqexp RPAREN        {$$=$2;}

/* op exp */
			|exp OR exp             {$$=A_IfExp(EM_tokPos, $1, A_IntExp(EM_tokPos,1), $3);}
			|exp AND exp            {$$=A_IfExp(EM_tokPos, $1, $3, A_IntExp(EM_tokPos,0));}
			

			|exp PLUS exp           {$$=A_OpExp(EM_tokPos, A_plusOp, $1, $3);}
			|exp MINUS exp          {$$=A_OpExp(EM_tokPos, A_minusOp, $1, $3);}
			|exp TIMES exp          {$$=A_OpExp(EM_tokPos, A_timesOp, $1, $3);}
			|exp DIVIDE exp         {$$=A_OpExp(EM_tokPos, A_divideOp, $1, $3);}
			|exp EQ exp             {$$=A_OpExp(EM_tokPos, A_eqOp, $1, $3);}
			|exp NEQ exp            {$$=A_OpExp(EM_tokPos, A_neqOp, $1, $3);}
			|exp LT exp             {$$=A_OpExp(EM_tokPos, A_ltOp, $1, $3);}
			|exp GT exp             {$$=A_OpExp(EM_tokPos, A_gtOp, $1, $3);}
			|exp LE exp             {$$=A_OpExp(EM_tokPos, A_leOp, $1, $3);}
			|exp GE exp             {$$=A_OpExp(EM_tokPos, A_geOp, $1, $3);}
			
/* call exp */
			|id LPAREN explist RPAREN     {$$=A_CallExp(EM_tokPos, $1, $3);}
/* record exp ???*/
			|id LBRACE efieldlist RBRACE  {$$=A_RecordExp(EM_tokPos, $1, $3);}
/* assign exp */
			|lvalue ASSIGN exp            {$$=A_AssignExp(EM_tokPos, $1, $3);}
/* array exp*/
			|id LBRACK exp RBRACK OF exp  {$$=A_ArrayExp(EM_tokPos, $1, $3, $6);}
/* if exp */
			|IF exp THEN exp ELSE exp     {$$=A_IfExp(EM_tokPos, $2, $4, $6);}
			|IF exp THEN exp              {$$=A_IfExp(EM_tokPos, $2, $4, A_NilExp(EM_tokPos));}//do not use null
/* while exp */			
			|WHILE exp DO exp             {$$=A_WhileExp(EM_tokPos, $2, $4);}
/* for exp */
			|FOR id ASSIGN exp TO exp DO exp  {$$=A_ForExp(EM_tokPos, $2, $4, $6, $8);}
/* break exp */
			|BREAK                        {$$=A_BreakExp(EM_tokPos);}
/* let exp ???*/
			|LET declist IN seqexp END       {$$=A_LetExp(EM_tokPos, $2,$4);}




lvalue:  id            {$$=A_SimpleVar(EM_tokPos, $1);}
			   |lvalue DOT id             {$$=A_FieldVar(EM_tokPos, $1, $3);}//dont use id a.c.b
			   |id LBRACK exp RBRACK      {$$=A_SubscriptVar(EM_tokPos,A_SimpleVar(EM_tokPos, $1), $3);}//func
			   |lvalue LBRACK exp RBRACK  {$$=A_SubscriptVar(EM_tokPos, $1, $3);}//have to add if call function a.func();



//build for recordexp
efield:                            {$$=NULL;}
         |id EQ exp                {$$=A_Efield($1, $3);}

efieldlist:  efield                {if($1==NULL) $$ = NULL; else $$=A_EfieldList($1, NULL);}
			   |efield	COMMA efieldlist {$$=A_EfieldList($1, $3);}

//mutiple exp
explist:                            {$$=NULL;}
			   |exp                       {$$=A_ExpList($1, NULL);}
			   |exp COMMA explist         {$$=A_ExpList($1, $3);}

expseq:                             {$$=NULL;}
			   |exp                       {$$=A_ExpList($1, NULL);}
			   |exp SEMICOLON expseq      {$$=A_ExpList($1, $3);}

seqexp:  expseq                     {$$=A_SeqExp(EM_tokPos, $1);}

//
declist: dec                           {$$=A_DecList($1, NULL);}
			   |dec declist                  {$$=A_DecList($1, $2);}

dec:     VAR id ASSIGN exp					   {$$=A_VarDec(EM_tokPos, $2, NULL, $4);}
         |VAR id COLON id ASSIGN exp   {$$=A_VarDec(EM_tokPos, $2, $4, $6);}
         |fundeclist                   {$$=A_FunctionDec(EM_tokPos, $1);}
         |tydeclist                    {$$=A_TypeDec(EM_tokPos,$1);}



ty:      id                         {$$=A_NameTy(EM_tokPos, $1);}//namety
         |LBRACE tyfields RBRACE    {$$=A_RecordTy(EM_tokPos, $2);}//recordty
         |ARRAY OF id               {$$=A_ArrayTy(EM_tokPos, $3);}//array



/* field for recordty */
tyfield: id COLON id                {$$=A_Field(EM_tokPos, $1, $3);}

tyfields:	                          {$$=NULL;}
         |tyfield                   {$$=A_FieldList($1, NULL);}
         |tyfield COMMA tyfields    {$$=A_FieldList($1, $3);}



/* function dec */
fundec:     FUNCTION id LPAREN tyfields RPAREN EQ exp {$$=A_Fundec(EM_tokPos,$2, $4, NULL, $7);}
			     |FUNCTION id LPAREN tyfields RPAREN COLON id EQ exp {$$=A_Fundec(EM_tokPos,$2, $4, $7, $9);}
			   
fundeclist: fundec                  {$$=A_FundecList($1, NULL);}
            |fundec fundeclist      {$$=A_FundecList($1, $2);}

/* type dec */
tydec:      TYPE id EQ ty           {$$=A_Namety($2, $4);}

tydeclist:  tydec                   {$$=A_NametyList($1, NULL);}
            |tydec tydeclist        {$$=A_NametyList($1, $2);}

         







