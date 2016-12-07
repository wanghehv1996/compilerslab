%{
#include <string.h>
#include "util.h"
#include "symbol.h"
#include "errormsg.h"
#include "absyn.h"
#include "y.tab.h"

int charPos=1;

int yywrap(void)
{
 charPos=1;
 return 1;
}

void adjust(void)
{
 EM_tokPos=charPos;
 charPos+=yyleng;
}
/*
* Please don't modify the lines above.
* You can add C declarations of your own below.
*/

%}
  /* You can add lex definitions here. */

%%
  /*
  * Below are some examples, which you can wipe out
  * and write reguler expressions and actions of your own.
  */

" "|\t  {adjust(); continue;}
\n	 {adjust(); EM_newline(); continue;}
for  {adjust(); return FOR;}
[0-9]+	 {adjust(); yylval.ival=atoi(yytext); return INT;}

"\"\""  {adjust(); yylval.sval=String("");  return STRING;}
"/*"([^*/])*"*/"  {adjust(); continue;}

"\""([^"])*"\""  {
  adjust();
  int newlen = strlen(yytext)-2;
  yytext[strlen(yytext)-1]=0;
  char newtext[newlen];
  int j = 0;
  int i = 1;
  for(i = 1; i<newlen+1;i++){
    if(yytext[i]=='\\'&&i<newlen){
      switch(yytext[i+1]){
        case 'n':
          i++;
          newtext[j]='\n';
          break;
        case 't':
          i++;
          newtext[j]='\t';
          break;
        case 'b':
          i++;
          newtext[j]='\b';
          break;
        default:
          break;
      }

    }else{
      newtext[j]=yytext[i];
    }
    j++;
  }
  newtext[j]=0;
  yylval.sval=String(newtext);
  return STRING;}


while  {adjust(); return WHILE;}
to  {adjust(); return TO;}
break  {adjust(); return BREAK;}
let  {adjust(); return LET;}
in  {adjust(); return IN;}
end  {adjust(); return END;}
function  {adjust(); return FUNCTION;}
var  {adjust(); return VAR;}
type  {adjust(); return TYPE;}
array  {adjust(); return ARRAY;}
if  {adjust(); return IF;}
then  {adjust(); return THEN;}
else  {adjust(); return ELSE;}
do  {adjust(); return DO;}
of  {adjust(); return OF;}
nil  {adjust(); return NIL;}

","	 {adjust(); return COMMA;}
":="  {adjust(); return ASSIGN;}
":"  {adjust(); return COLON;}
";"  {adjust(); return SEMICOLON;}
"("  {adjust(); return LPAREN;}
")"  {adjust(); return RPAREN;}
"["  {adjust(); return LBRACK;}
"]"  {adjust(); return RBRACK;}
"{"  {adjust(); return LBRACE;}
"}"  {adjust(); return RBRACE;}
"."  {adjust(); return DOT;}
"+"  {adjust(); return PLUS;}
"-"  {adjust(); return MINUS;}
"*"  {adjust(); return TIMES;}
"/"  {adjust(); return DIVIDE;}
"="  {adjust(); return EQ;}
"!="|"<>"  {adjust(); return NEQ;}
"<="  {adjust(); return LE;}
"<"  {adjust(); return LT;}
">="  {adjust(); return GE;}
">"  {adjust(); return GT;}
"&"  {adjust(); return AND;}
"|"  {adjust(); return OR;}
[a-zA-Z]([a-zA-Z0-9]|[_])*  {adjust(); yylval.sval=String(yytext);  return ID;}
.	 {adjust(); EM_error(EM_tokPos,"illegal token");}
