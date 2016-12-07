/* This file is not complete.  You should fill it in with your
   solution to the programming exercise. */
#include <stdio.h>
#include "prog1.h"
#include "slp.h"
int maxargs(A_stm stm);
void interp(A_stm stm);

typedef struct table *Table_;

struct table{string id; int value; Table_ tail;};
Table_ Table(string id, int value, struct table *tail){
  Table_ t = malloc(sizeof(*t));
  t->id=id; t->value=value; t->tail=tail;
  return t;
}

Table_ getId(Table_ t, string id){
  Table_ tmp = t;
  while(tmp!=NULL){
    //printf("get %s=%d\n",tmp->id,tmp->value);
    if(strcmp(tmp->id,id)==0){
      //printf("get it\n");
      return tmp;
    }
    tmp=tmp->tail;
  }
  return NULL;
}

Table_ insertId(Table_ t, string id, int value){
  Table_ tmp = t;
  //printf("insert %s=%d\n",id,value);
  while(tmp!=NULL){
    //printf("insert find %s=%d\n",tmp->id,tmp->value);
    if(tmp->id!=NULL&&strcmp(tmp->id,id)==0){
      tmp->value=value;
      //printf("get and change\n");
      return t;
    }
    if(tmp->tail==NULL){
      //printf("insert at tail\n");
      tmp->tail=Table(id,value,NULL);
      return t;
    }
    tmp=tmp->tail;
  }
  //printf("null\n");
  t = Table(id,value,NULL);//may be wrong
  return t;
}

int getValue(Table_ t){
  if(t==NULL){
    printf("NULL Table_");
    return 0;
  }
  return t->value;
}

int max(int a, int b){
  if(a>b)
    return a;
  else return b;
}

int length(A_expList exps){
  A_expList tmp = exps;
  int i = 0;
  while(tmp->kind!=A_lastExpList){
    i++;
    tmp=tmp->u.pair.tail;
  }
  return i+1;
}

int maxargs(A_stm stm){
  switch(stm->kind){
    case A_compoundStm:
      //printf("cstm");
      return max(maxargs(stm->u.compound.stm1),maxargs(stm->u.compound.stm2));
    case A_assignStm:
      //printf("astm");
      return maxargsExp(stm->u.assign.exp);
    case A_printStm:
      //printf("pstm");
      return max(maxargsExps(stm->u.print.exps),length(stm->u.print.exps));
    default:
      return 2;
  }

}

int maxargsExps(A_expList exps){
  switch(exps->kind){
    case A_pairExpList:
      //printf("pexplist");
      return max(maxargsExp(exps->u.pair.head),maxargsExps(exps->u.pair.tail));
    case A_lastExpList:
      //printf("lexplist");
      return maxargsExp(exps->u.last);
    default:
      return 0;
  }
}

int maxargsExp(A_exp exp){
  switch(exp->kind){
    case A_idExp:
      //printf("idexp");
      return 0;
    case A_numExp:
      //printf("numexp");
      return 0;
    case A_opExp:
      //printf("opexp");
      return max(maxargsExp(exp->u.op.left),maxargsExp(exp->u.op.right));
    case A_eseqExp:
      //printf("eseqexp");
      return max(maxargs(exp->u.eseq.stm),maxargsExp(exp->u.eseq.exp));
    default:
      return 0;
  }
}

Table_ interpStm(A_stm stm, Table_ t);
int printExps(A_expList exps, Table_ t);
int interpExp(A_exp exp, Table_ t);


void interp(A_stm stm){
  Table_ t=NULL;
  t = interpStm(stm, t);
  
}

Table_ interpStm(A_stm stm, Table_ t){
    switch(stm->kind){
    case A_compoundStm:
      //printf("cstm");
      t = interpStm(stm->u.compound.stm1,t);
      t = interpStm(stm->u.compound.stm2,t);
      return t;
    case A_assignStm:
      //printf("astm");
      t = insertId(t, stm->u.assign.id, interpExp(stm->u.assign.exp,t));
      return t;
    case A_printStm:
      //printf("pstm");
      printExps(stm->u.print.exps,t);
      printf("\n");
      return t;
    default:
      return t;
  }
}

int printExps(A_expList exps, Table_ t){
  switch(exps->kind){
    case A_pairExpList:
      //printf("pexplist");
      printf("%d ",interpExp(exps->u.pair.head,t));
      printExps(exps->u.pair.tail,t);
      return 0;
    case A_lastExpList:
      //printf("lexplist");
      printf("%d ",interpExp(exps->u.last,t));
      return 0;
    default:
      printf("error:wrong exp list type\n");
      return 0;
  }
}

int interpExp(A_exp exp, Table_ t){
  switch(exp->kind){
    case A_idExp:
      //printf("idexp");
      return getValue(getId(t,exp->u.id));
    case A_numExp:
      //printf("numexp");
      return exp->u.num;
    case A_opExp:
      //printf("opexp");
      switch(exp->u.op.oper){
        case A_plus:
          return interpExp(exp->u.op.left,t)+interpExp(exp->u.op.right,t);
        case A_minus:
          return interpExp(exp->u.op.left,t)-interpExp(exp->u.op.right,t);
        case A_times:
          return interpExp(exp->u.op.left,t)*interpExp(exp->u.op.right,t);
        case A_div:
          return interpExp(exp->u.op.left,t)/interpExp(exp->u.op.right,t);
        default:
          printf("error:wrong oper type\n");
          return 0;
      }
    
    //{A_plus,A_minus,A_times,A_div} A_binop;

      
    case A_eseqExp:
      //printf("eseqexp");
      t = interpStm(exp->u.eseq.stm,t);
      return interpExp(exp->u.eseq.exp, t);
    default:
      printf("error:wrong exp type\n");
      return 0;
  }
}

/*
 *Please don't modify the main() function
 */
int main()
{
	int args;

	printf("prog\n");
	args = maxargs(prog());
	printf("args: %d\n",args);
	interp(prog());

	printf("prog_prog\n");
	args = maxargs(prog_prog());
	printf("args: %d\n",args);
	interp(prog_prog());

	printf("right_prog\n");
	args = maxargs(right_prog());
	printf("args: %d\n",args);
	interp(right_prog());

	return 0;
}
