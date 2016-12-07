#include <stdio.h>
#include "util.h"
#include "table.h"
#include "symbol.h"
#include "absyn.h"
#include "temp.h"
#include "tree.h"
#include "printtree.h"
#include "frame.h"
#include "translate.h"


F_fragList fragList = NULL;
F_fragList stringList = NULL;

Tr_exp Tr_Ex(T_exp ex);
Tr_exp Tr_Nx(T_stm nx);
Tr_exp Tr_Cx(patchList trues, patchList falses, T_stm stm);

T_exp unEx(Tr_exp e);
T_stm unNx(Tr_exp e);
struct Cx unCx(Tr_exp e);

patchList PatchList(Temp_label *head, patchList tail);
void doPatch(patchList pList, Temp_label label);
patchList joinPatch(patchList fList, patchList sList);

Tr_accessList getLevelAccessList(Tr_level level);
Tr_access Tr_Access(Tr_level level, F_access access);
T_expList Tr_expList2T_expList(Tr_expList list);

Tr_exp Tr_StaticLink(Tr_level funLevel, Tr_level level);


Tr_exp Tr_Ex(T_exp ex){
	Tr_exp trEx = malloc(sizeof(*trEx));
	trEx->kind = Tr_ex;
	trEx->u.ex = ex;
	return trEx;
}

Tr_exp Tr_Nx(T_stm nx){
	Tr_exp trNx = malloc(sizeof(*trNx));
	trNx->kind = Tr_nx;
	trNx->u.nx = nx;
	return trNx;
}

Tr_exp Tr_Cx(patchList trues, patchList falses, T_stm stm){
	Tr_exp trCx = malloc(sizeof(*trCx));
	trCx->kind = Tr_cx;
	trCx->u.cx.trues = trues;
	trCx->u.cx.falses = falses;
	trCx->u.cx.stm = stm;
	return trCx;
}

void doPatch(patchList pList, Temp_label label){
	for (; pList; pList = pList->tail)//match each
		*(pList->head) = label;
}

patchList joinPatch(patchList fList, patchList sList){
	if (!fList) return sList;
	for (; fList->tail; fList = fList->tail);//goto tail
	fList->tail = sList;
	return fList;
}

T_exp unEx(Tr_exp e){
	switch(e->kind) {
		case Tr_ex:
			return e->u.ex;
		case Tr_nx:
			return T_Eseq(e->u.nx, T_Const(0));
		case Tr_cx:{
			Temp_temp r = Temp_newtemp();
			Temp_label t = Temp_newlabel(), f = Temp_newlabel();
			doPatch(e->u.cx.trues, t);
			doPatch(e->u.cx.falses, f);
			return T_Eseq(T_Move(T_Temp(r), T_Const(1)),
				T_Eseq(e->u.cx.stm,
					T_Eseq(T_Label(f),
						T_Eseq(T_Move(T_Temp(r), T_Const(0)),
							T_Eseq(T_Label(t), T_Temp(r))))));
		}
		default:
			assert(0);//err
	}
	return NULL;
}

T_stm unNx(Tr_exp e){
	switch(e->kind) {
		case Tr_ex:
			return T_Exp(e->u.ex);
		case Tr_nx:
			return e->u.nx;
		case Tr_cx:{
			Temp_temp r = Temp_newtemp();
			Temp_label t = Temp_newlabel(), f = Temp_newlabel();
			doPatch(e->u.cx.trues, t);
			doPatch(e->u.cx.falses, f);
			return T_Exp(T_Eseq(T_Move(T_Temp(r), T_Const(1)),
				T_Eseq(e->u.cx.stm,
					T_Eseq(T_Label(f),
						T_Eseq(T_Move(T_Temp(r), T_Const(0)),
							T_Eseq(T_Label(t), T_Temp(r)))))));
		}
		default:
			assert(0);//err for naiive implement we need this
	}
	return NULL;
}

struct Cx unCx(Tr_exp e){
	switch(e->kind) {
		case Tr_ex:{
			struct Cx cx;
			cx.stm = T_Cjump(T_eq, e->u.ex, T_Const(0), NULL, NULL);
			cx.trues = PatchList(&(cx.stm->u.CJUMP.false), NULL);
			cx.falses = PatchList(&(cx.stm->u.CJUMP.true), NULL);
			return cx;
		}
		case Tr_cx:
			return e->u.cx;
		default:
			assert(0);//err
	}

}

//do the outest level
Tr_level outer = NULL;
Tr_level Tr_outermost(void){
	if (!outer)
		outer = Tr_newLevel(NULL, Temp_newlabel(), NULL);
	return outer;
}

Tr_accessList getLevelAccessList(Tr_level level){
	Tr_accessList listHead = NULL;
	Tr_accessList listItr = NULL;
	F_accessList accessList = F_formals(level->frame)->tail;

	if(accessList){
		Tr_access access = Tr_Access(level, accessList->head);
		listHead = Tr_AccessList(access, NULL);
		listItr = listHead;
		accessList = accessList->tail;
	}
	while (accessList) {
		Tr_access access = Tr_Access(level, accessList->head);
		listItr->tail = Tr_AccessList(access, NULL);
		listItr = listItr->tail;
		accessList = accessList->tail;
	}
	return listHead;
}



patchList PatchList(Temp_label *head, patchList tail){
	patchList pList = malloc(sizeof(*pList));
	pList->head = head;
	pList->tail = tail;
	return pList;
}

Tr_access Tr_Access(Tr_level level, F_access access){
	Tr_access traccess = malloc(sizeof(*traccess));
	traccess->level = level;
	traccess->access = access;
	return traccess;
}

Tr_access Tr_allocLocal(Tr_level level, bool escape){
	Tr_access local = malloc(sizeof(*local));
	local->level = level;
	local->access = F_allocLocal(level->frame, escape);
	return local;
}

Tr_accessList Tr_formals(Tr_level level){
	return level->formals;
}

Tr_accessList Tr_AccessList(Tr_access head, Tr_accessList tail){
	Tr_accessList list = malloc(sizeof(*list));
	list->head = head;
	list->tail = tail;
	return list;
}

Tr_level Tr_newLevel(Tr_level parent, Temp_label name, U_boolList formals){
	Tr_level level = malloc(sizeof(*level));
	level->parent = parent;
	level->name = name;
	level->frame = F_newFrame(name, U_BoolList(TRUE, formals));
	level->formals = getLevelAccessList(level);
	return level;
}

Tr_exp Tr_StaticLink(Tr_level funLevel, Tr_level level){
	T_exp addr = T_Temp(F_FP());
	//find the last and gocha
	while (level != funLevel->parent) {
		F_access staticLink = F_formals(level->frame)->head;
		addr = F_Exp(staticLink, addr);
		level = level->parent;
	}
	return Tr_Ex(addr);
}

//new explist
Tr_expList Tr_ExpList(void){
	Tr_expList list = malloc(sizeof(*list));
	list->head = NULL;
	list->tail = NULL;
	return list;
}

//add to tail
void Tr_ExpListAdd2Tail(Tr_expList list, Tr_exp expr){
	if (list->tail) {//used2bhead
		Tr_node node = malloc(sizeof(*node));
		node->expr = expr;
		node->next = NULL;
		list->tail->next = node;
		list->tail = list->tail->next;
	} else {
		list->head = malloc(sizeof(*list->head));
		list->head->expr = expr;
		list->head->next = NULL;
		list->tail = list->head;
	}
}

//add to head
void Tr_ExpListAdd(Tr_expList list, Tr_exp expr){
	if (list->head) {
		Tr_node node = malloc(sizeof(*node));
		node->expr = expr;
		node->next = list->head;
		list->head = node;
	} else {
		list->head = malloc(sizeof(*list->head));
		list->head->expr = expr;
		list->head->next = NULL;
		list->tail = list->head;
	}
}



T_expList Tr_expList2T_expList(Tr_expList list){
	T_expList resList = NULL;
	T_expList tailList = NULL;
	Tr_node iter = list->head;
	if(iter){
		resList = T_ExpList(unEx(iter->expr), NULL);
		tailList = resList;
		iter = iter->next;
	}
	while (iter) {
		tailList->tail = T_ExpList(unEx(iter->expr), NULL);
		tailList = tailList->tail;
		iter = iter->next;
	}
	return resList;
}


Tr_exp Tr_simpleVar(Tr_access access, Tr_level level){
	T_exp addr = T_Temp(F_FP());
	while (level != access->level) {
		F_access staticLink = F_formals(level->frame)->head;
		addr = F_Exp(staticLink, addr);
		level = level->parent;
	}
	return Tr_Ex(F_Exp(access->access, addr));
}

Tr_exp Tr_fieldVar(Tr_exp recordBase, int offset){
	return Tr_Ex(T_Mem(T_Binop(T_plus, unEx(recordBase), T_Const(offset * F_WORD_SIZE))));
}

Tr_exp Tr_subscriptVar(Tr_exp arrayBase, Tr_exp index){
	//mem(arrbase+index*fwordsize)
	return Tr_Ex(T_Mem(T_Binop(T_plus, unEx(arrayBase),
				T_Binop(T_mul, unEx(index), T_Const(F_WORD_SIZE)))));
}


//add to list!
Tr_exp Tr_stringExp(string str){
	Temp_label strLabel = Temp_newlabel();
	F_frag fragment = F_StringFrag(strLabel, str);
	stringList = F_FragList(fragment, stringList);
	return Tr_Ex(T_Name(strLabel));
}

Tr_exp Tr_intExp(int n){
	return Tr_Ex(T_Const(n));
}

//Temp_temp nilTemp = NULL;
Tr_exp Tr_nilExp(void){
	//if (!nilTemp){
		Temp_temp nilTemp = Temp_newtemp();
		T_stm alloc = T_Move(T_Temp(nilTemp),
			F_externalCall(String("initRecord"), T_ExpList(T_Const(0 * F_WORD_SIZE), NULL)));
		return Tr_Ex(T_Eseq(alloc, T_Temp(nilTemp)));;
	//}
	//return Tr_Ex(T_Temp(nilTemp));
}

Tr_exp Tr_noExp(void){
	return Tr_Ex(T_Const(0));
}


Tr_exp Tr_seqExp(Tr_expList list){
	//do one by one
	T_exp result = unEx(list->head->expr);
	Tr_node p;
	for (p = list->head->next; p; p = p->next)
		result = T_Eseq(T_Exp(unEx(p->expr)), result);
	return Tr_Ex(result);
}

Tr_exp Tr_arrayExp(Tr_exp size, Tr_exp init){
	return Tr_Ex(F_externalCall(String("initArray"), T_ExpList(unEx(size), T_ExpList(unEx(init), NULL))));
}

Tr_exp Tr_recordExp(int n, Tr_expList list){
	Temp_temp newtemp = Temp_newtemp();
	T_stm alloc = T_Move(T_Temp(newtemp), F_externalCall(String("initRecord"), T_ExpList(T_Const(n * F_WORD_SIZE), NULL)));
	int count = n - 1;//as itr
	Tr_node p = list->head->next;
	//initial
	T_stm stms = T_Move(T_Mem(T_Binop(T_plus, T_Temp(newtemp), T_Const(count * F_WORD_SIZE))), unEx(list->head->expr));
	count--;
	//do one by one
	while (p){
		stms = T_Seq(T_Move(T_Mem(T_Binop(T_plus, T_Temp(newtemp), T_Const(count * F_WORD_SIZE))), unEx(p->expr)), stms);
		p = p->next;
		count--;
	}
	return Tr_Ex(T_Eseq(T_Seq(alloc, stms), T_Temp(newtemp)));
}

Tr_exp Tr_letExp(Tr_expList list){
	T_exp exps = unEx(list->head->expr);
	Tr_node p = list->head->next;
	while (p){
		exps = T_Eseq(T_Exp(unEx(p->expr)), exps);
		p = p->next;
	}
	return Tr_Ex(exps);
}

Tr_exp Tr_doneExp(void){//just a label for loop90
	return Tr_Ex(T_Name(Temp_newlabel()));
}

Tr_exp Tr_breakExp(Tr_exp breakk){
	T_stm s = unNx(breakk);
	//if (s->kind == T_LABEL)
		return Tr_Nx(T_Jump(T_Name(s->u.LABEL), Temp_LabelList(s->u.LABEL, NULL)));
	//else assert(0);
}


Tr_exp Tr_whileExp(Tr_exp test, Tr_exp done, Tr_exp body){//need divide
	Temp_label testLabel = Temp_newlabel();
	Temp_label bodyLabel = Temp_newlabel();
	return Tr_Ex(T_Eseq(T_Jump(T_Name(testLabel), Temp_LabelList(testLabel, NULL)),//jump new label90
				T_Eseq(T_Label(bodyLabel), T_Eseq(unNx(body),//label for body and then do label
					T_Eseq(T_Label(testLabel),//label for test
						T_Eseq(T_Cjump(T_eq, unEx(test), T_Const(0), unEx(done)->u.NAME, bodyLabel),//judge and jump
								T_Eseq(T_Label(unEx(done)->u.NAME), T_Const(0))))))));
}


Tr_exp Tr_ifExp(Tr_exp test, Tr_exp then, Tr_exp elsee){
	Temp_label trueLabel = Temp_newlabel();
	Temp_label falseLabel = Temp_newlabel();
	Tr_exp result = NULL;
	struct Cx cond = unCx(test);
	doPatch(cond.trues, trueLabel);
	doPatch(cond.falses, falseLabel);
	if (!elsee){
		if (then->kind == Tr_cx) {//cx
	  result = Tr_Nx(T_Seq(cond.stm, T_Seq(T_Label(trueLabel),
	 									 T_Seq(then->u.cx.stm, T_Label(falseLabel)))));
	 	} else if (then->kind == Tr_nx) {//nx
			result = Tr_Nx(T_Seq(cond.stm, T_Seq(T_Label(trueLabel),
												T_Seq(then->u.nx, T_Label(falseLabel)))));
		} else {//ex
			result = Tr_Nx(T_Seq(cond.stm, T_Seq(T_Label(trueLabel),
												T_Seq(T_Exp(unEx(then)), T_Label(falseLabel)))));
		}

	}else{
		Temp_label joinLabel = Temp_newlabel();
		Temp_temp r = Temp_newtemp();
		T_stm joinJmp = T_Jump(T_Name(joinLabel), Temp_LabelList(joinLabel, NULL));
		if (elsee->kind == Tr_ex) {
			result = Tr_Ex(T_Eseq(cond.stm,
											T_Eseq(T_Label(trueLabel),
												T_Eseq(T_Move(T_Temp(r), unEx(then)),
													T_Eseq(joinJmp, T_Eseq(T_Label(falseLabel),
														T_Eseq(T_Move(T_Temp(r), unEx(elsee)),
															T_Eseq(joinJmp,
																T_Eseq(T_Label(joinLabel), T_Temp(r))))))))));
		} else {
			T_stm thenStm;
			if (then->kind == Tr_ex) thenStm = T_Exp(then->u.ex);
			else thenStm = (then->kind == Tr_nx) ? then->u.nx : then->u.cx.stm;

			T_stm elseeStm = (elsee->kind == Tr_nx) ? elsee->u.nx : elsee->u.cx.stm;
			result = Tr_Nx(T_Seq(cond.stm,
											T_Seq(T_Label(trueLabel),
												T_Seq(thenStm,
													T_Seq(joinJmp,
														T_Seq(T_Label(falseLabel),
															T_Seq(elseeStm,
																T_Seq(joinJmp, T_Label(joinLabel)))))))));
		}
	}
	return result;
}

Tr_exp Tr_assignExp(Tr_exp var, Tr_exp exp){
	return Tr_Nx(T_Move(unEx(var), unEx(exp)));
}

Tr_exp Tr_arithExp(A_oper op, Tr_exp left, Tr_exp right){
	T_binOp oper;
	if(op==A_plusOp)
		oper = T_plus;
	else if(op == A_minusOp)
		oper = T_minus;
	else if(op == A_timesOp)
		oper = T_mul;
	else oper = T_div;
	return Tr_Ex(T_Binop(oper, unEx(left), unEx(right)));
}

Tr_exp Tr_relExp(A_oper op, Tr_exp left, Tr_exp right){
	T_relOp oper;
	if(op==A_ltOp)
		oper = T_lt;
	else if(op == A_leOp)
		oper = T_le;
	else if(op == A_gtOp)
		oper = T_gt;
	else oper = T_ge;
	T_stm condStm = T_Cjump(oper, unEx(left), unEx(right), NULL, NULL);
	patchList trues = PatchList(&condStm->u.CJUMP.true, NULL);
	patchList falses = PatchList(&condStm->u.CJUMP.false, NULL);
	return Tr_Cx(trues, falses, condStm);
}


Tr_exp Tr_eqExp(A_oper op, Tr_exp left, Tr_exp right){
	T_relOp oper;
	if (op == A_eqOp) oper = T_eq;
	else oper = T_ne;
	T_stm condStm = T_Cjump(oper, unEx(left), unEx(right), NULL, NULL);
	patchList trues = PatchList(&condStm->u.CJUMP.true, NULL);
	patchList falses = PatchList(&condStm->u.CJUMP.false, NULL);
	return Tr_Cx(trues, falses, condStm);
}

Tr_exp Tr_eqStringExp(A_oper op, Tr_exp left, Tr_exp right){
	T_exp result = F_externalCall(String("stringEqual"),
		T_ExpList(unEx(left), T_ExpList(unEx(right), NULL)));
	if (op == A_eqOp)
		return Tr_Ex(result);
	else {
		if(result->kind == T_CONST && result->u.CONST == 1)
		//T_Const(0) : T_Const(1);
			return Tr_Ex(T_Const(0));
		else
			return Tr_Ex(T_Const(1));
	}
}

//simple
Tr_exp Tr_eqRef(A_oper op, Tr_exp left, Tr_exp right){
	return Tr_Ex(T_Const(0));
}

//add to list!
Tr_exp Tr_callExp(Tr_level level, Tr_level funLevel, Temp_label funLabel, Tr_expList argList){
	Tr_ExpListAdd(argList, Tr_StaticLink(funLevel, level));
	T_expList args = Tr_expList2T_expList(argList);
	return Tr_Ex(T_Call(T_Name(funLabel), args));
}


void Tr_procEntryExit(Tr_level level, Tr_exp body, Tr_accessList formals){
	F_frag pfrag;
	if(body == NULL){
		pfrag = F_ProcFrag(NULL, level->frame);
	}else
		pfrag = F_ProcFrag(unNx(body), level->frame);
	fragList = F_FragList(pfrag, fragList);
}

F_fragList Tr_getFragList(void){
	F_fragList cursor = NULL, prev = NULL;
	for (cursor = stringList; cursor; cursor = cursor->tail)
		prev = cursor;
	if (prev) prev->tail = fragList;
	return stringList ? stringList : fragList;
}
