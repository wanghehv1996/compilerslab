#include <stdio.h>
#include "util.h"
#include "errormsg.h"
#include "symbol.h"
#include "absyn.h"
#include "types.h"
#include "env.h"
#include "semant.h"

/*Lab4: Your implementation of lab4*/
static S_symbol loopVar;
static bool checkAssignLeft = FALSE;

struct expty expTy(Tr_exp exp, Ty_ty ty) {
	struct expty e;
	e.exp = exp;
	e.ty  = ty;
	return e;
}


void SEM_transProg(A_exp exp){
	struct expty et;
	S_table t = E_base_tenv();
	S_table v = E_base_venv();
	et = transExp(v, t, exp);
}

struct expty transVar(S_table venv, S_table tenv, A_var v) {

	switch (v->kind) {
		// var id a
		case A_simpleVar:{
			E_enventry varEnt = S_look(venv, v->u.simple);

			//check if is the loop var assign
			if(checkAssignLeft && loopVar != NULL && !strcmp(S_name(v->u.simple),S_name(loopVar))){
				EM_error(v->pos, "loop variable can't be assigned");
				checkAssignLeft = FALSE;
			}

			if (!varEnt) {
				EM_error(v->pos, "undefined variable %s", S_name(v->u.simple));
				return expTy(NULL, Ty_Int());
			}
			if(varEnt->kind != E_varEntry) {
				EM_error(v->pos, "undefined variable %s", S_name(v->u.simple));
				return expTy(NULL, Ty_Int());
			}
			return expTy(NULL, actual_ty(varEnt->u.var.ty));
		}
		//var record a.b
		case A_fieldVar:{
			struct expty var = transVar(venv, tenv, v->u.field.var);
			if (var.ty->kind != Ty_record) {
				EM_error(v->pos, "not a record type");
				return expTy(NULL, Ty_Record(NULL));
			}
			Ty_fieldList fieldList = var.ty->u.record;
			while (fieldList) {
				if(fieldList->tail==NULL)//modify: the tail of fl should be the same type
				if(fieldList->head->name == v->u.field.sym)
					return expTy(NULL, actual_ty(fieldList->head->ty));
				fieldList = fieldList->tail;
			}
			EM_error(v->pos, "field %s doesn't exist", S_name(v->u.field.sym));
			return expTy(NULL, Ty_Record(NULL));
		}
		//var a[b]
		case A_subscriptVar:{

			struct expty var  = transVar(venv, tenv, v->u.subscript.var);
			if (var.ty->kind != Ty_array){
				EM_error(v->pos, "array type required");
				return expTy(NULL, Ty_Array(NULL));
			}
			struct expty exp = transExp(venv, tenv, v->u.subscript.exp);
			if (exp.ty->kind != Ty_int) {
				EM_error(v->pos, "int required");
				return expTy(NULL, Ty_Array(NULL));
			}
			return expTy(NULL, actual_ty(var.ty->u.array));
		}
		default:
			EM_error(v->pos, "error var type");
	}
}

struct expty transExp(S_table v, S_table t, A_exp a){

	switch (a->kind) {

	case A_varExp:
		return transVar(v, t, a->u.var);
	case A_intExp:
		return expTy(NULL, Ty_Int());
	case A_stringExp:
		return expTy(NULL, Ty_String());
	case A_nilExp:
		return expTy(NULL, Ty_Nil());


	case A_opExp:{//wait for modify
		A_oper oper = a->u.op.oper;
		struct expty left  = transExp(v, t, a->u.op.left);
		struct expty right = transExp(v, t, a->u.op.right);
		//oper = +-*/
		if (oper >= A_plusOp && oper <= A_divideOp) {
			//check l=r and dont return right now
			if(left.ty->kind != right.ty->kind)
				EM_error(a->u.op.left->pos, "same type required");
			if (left.ty->kind != Ty_int){
				EM_error(a->u.op.left->pos, "integer required");
			}
			if (right.ty->kind != Ty_int) {
				EM_error(a->u.op.right->pos, "integer required");
			}
			return expTy(NULL, Ty_Int());
		}
		//oper = ><=
		if (oper <= A_geOp) {
			//== & <> should compare
			if (oper == A_eqOp || oper == A_neqOp) {
				if (!checkType(left.ty,right.ty))
					EM_error(a->u.op.left->pos, "same type required");
			} else {
			//> < should be same type
				if(left.ty->kind != right.ty->kind)
					EM_error(a->u.op.left->pos, "same type required");
			}
			return expTy(NULL, Ty_Int());
		}
		EM_error(a->u.op.left->pos,"op error");
		return expTy(NULL, Ty_Int());
	}


	case A_callExp:{
		E_enventry callEnt = S_look(v, a->u.call.func);
		//no func
		if(!callEnt||callEnt->kind!=E_funEntry){
			EM_error(a->pos, "undefined function %s\n", S_name(a->u.call.func));
			return expTy(NULL, Ty_Void());
		}

		Ty_tyList argList = callEnt->u.fun.formals;
		A_expList calList = a->u.call.args;
		//match arg one by one
		while (argList && calList) {
			struct expty calArg = transExp(v, t, calList->head);
			if (!checkType(calArg.ty, argList->head)){
				EM_error(a->pos, "para type mismatch");
				goto wrongcond;
			}
			argList = argList->tail;
			calList = calList->tail;
		}
		if(!argList && !calList)
			goto rightcond;
		if (calList) {
			EM_error(a->pos,"too many params in function %s\n", S_name(a->u.call.func));
			goto wrongcond;
		}
		if (argList) {
			EM_error(a->pos, "less params in function %s\n", S_name(a->u.call.func));
			goto wrongcond;
		}

		wrongcond:
		//arg not match
			EM_error(a->pos, "arg not match %s\n", S_name(a->u.call.func));
			return expTy(NULL, Ty_Void());

		rightcond:
		//right
			return expTy(NULL, actual_ty(callEnt->u.fun.result));

	}


	case A_recordExp:{
		Ty_ty recordTy = actual_ty(S_look(t, a->u.record.typ));
		//no record type
	  if (!recordTy) {
			EM_error(a->pos, "undefined type %s (debug recordExp)", S_name(a->u.record.typ));
			return expTy(NULL, Ty_Record(NULL));
		}
		//diff record type
		if (recordTy->kind != Ty_record){
			EM_error(a->pos, "%s is not a record type", S_name(a->u.record.typ));
			return expTy(NULL, Ty_Record(NULL));
		}

		Ty_fieldList recList = recordTy->u.record;
		A_efieldList expList = a->u.record.fields;
		//match rec one by one
		while(recList && expList) {
			struct expty recItem = transExp(v, t, expList->head->exp);
			if(!checkType(recList->head->ty, recItem.ty)||(recList->head->name != expList->head->name)){
				EM_error(a->pos, "unmatched name: type in %s", S_name(a->u.record.typ));
				return expTy(NULL, Ty_Record(NULL));
			}
			recList = recList->tail;
			expList = expList->tail;
		}
		if (!recList && !expList)
			return expTy(NULL, recordTy);
		if (recList) {
			EM_error(a->pos, "less fields in %s", S_name(a->u.record.typ));
		}
		if (expList) {
			EM_error(a->pos, "too many field in %s", S_name(a->u.record.typ));
		}
		return expTy(NULL, Ty_Record(NULL));

	}
	case A_arrayExp:{
		Ty_ty arrayTy = actual_ty(S_look(t, a->u.array.typ));
		//no array ty
		if (!arrayTy) {
			EM_error(a->pos, "undeined array type %s", S_name(a->u.array.typ));
			return expTy(NULL, Ty_Array(NULL));
		}
		//diff array kind
		if (arrayTy->kind != Ty_array) {
			EM_error(a->pos, "%s is not a array type", S_name(a->u.array.typ));
			return expTy(NULL, Ty_Array(NULL));
		}
		struct expty size = transExp(v, t, a->u.array.size);
		struct expty init = transExp(v, t, a->u.array.init);
		//size not int
		if (size.ty->kind != Ty_int) {
			EM_error(a->pos, "array size should be int %s", S_name(a->u.array.typ));
			return expTy(NULL, Ty_Array(NULL));
		}
		//diff type
		if (!checkType(init.ty, arrayTy->u.array)){
			EM_error(a->pos, "type mismatch");
			return expTy(NULL, Ty_Array(NULL));
		}
		//right cond
		return expTy(NULL, arrayTy);
	}
	case A_seqExp:{
		A_expList list = a->u.seq;
		//no list
		if (!list) {
			return expTy(NULL, Ty_Void());
		}
		//trans every exp in seq
		while (list->tail) {
			transExp(v, t, list->head);
			list = list->tail;
		}
		//right cond
		return transExp(v, t, list->head);
	}
	case A_assignExp:{
		struct expty var = transVar(v, t, a->u.assign.var);
		//check loop var
		checkAssignLeft = TRUE;
		struct expty exp = transExp(v, t, a->u.assign.exp);
		if (!checkType(var.ty, exp.ty)) {
			EM_error(a->pos, "unmatched assign exp");
			return expTy(NULL, Ty_Void());
		}
		return expTy(NULL, Ty_Void());
	}

	case A_whileExp:{
		struct expty test = transExp(v, t, a->u.whilee.test);
		//kind not int
		if (test.ty->kind != Ty_int) {
			EM_error(a->pos, "int required");
			return expTy(NULL, Ty_Void());
		}
		//check body
		struct expty body = transExp(v, t, a->u.whilee.body);
		if (body.ty->kind != Ty_void){
			EM_error(a->u.whilee.body->pos, "while body must produce no value");
			return expTy(NULL, Ty_Void());
		}
		return expTy(NULL, Ty_Void());
	}

	case A_breakExp:
		return expTy(NULL, Ty_Void());

	case A_forExp:{
		struct expty lo = transExp(v, t, a->u.forr.lo);
		struct expty hi = transExp(v, t, a->u.forr.hi);
		loopVar = a->u.forr.var;

		if (lo.ty != Ty_Int() || hi.ty != Ty_Int()) {
			EM_error(a->pos, "for exp's range type is not integer");
			//EM_error(a->pos, "loop variable can't be assigned");
		}
		//scope
		S_beginScope(v);
		//add var into scope
		transDec(v, t, A_VarDec(a->pos, a->u.forr.var, S_Symbol("int"), a->u.forr.lo));
		struct expty body = transExp(v, t, a->u.forr.body);
		//check body kind
		if (body.ty->kind != Ty_void){
			EM_error(a->u.forr.body->pos, "for body must produce no value");
			return expTy(NULL, Ty_Void());
		}
		S_endScope(v);
		loopVar = NULL;
		return body;
	}

	case A_ifExp:{
		//check test type but DONT RETURN first
		struct expty test = transExp(v, t, a->u.iff.test);
		if (test.ty->kind != Ty_int)
			EM_error(a->u.iff.test->pos, "integer required");
		//check then and else
		struct expty then = transExp(v, t, a->u.iff.then);
		struct expty elsee = transExp(v, t, a->u.iff.elsee);

		//else exist
		//if in a func, then and else can be some return type
		if (a->u.iff.elsee != NULL && a->u.iff.elsee->kind != A_nilExp) {
			if (then.ty->kind != elsee.ty->kind) {
				EM_error(a->u.iff.elsee->pos, "then exp and else exp type mismatch");
			}
			//need to return here
			return expTy(NULL, then.ty);
		}

		//if there is no else, if cannot be in a func. then must be no value;
		if (then.ty->kind != Ty_void){
			EM_error(a->u.iff.then->pos, "if-then exp's body must produce no value");
			return expTy(NULL, Ty_Void());
		}
		return expTy(NULL, Ty_Void());
	}

	case A_letExp:{
		//scope
		S_beginScope(v);
		S_beginScope(t);
		//check dec
		A_decList decs = a->u.let.decs;
		while(decs) {
			transDec(v, t, decs->head);
			decs = decs->tail;
		}
		//check body
		struct expty body = transExp(v, t, a->u.let.body);
		if (body.ty->kind != Ty_void){
			EM_error(a->u.let.body->pos, "let body must produce no value");
			return expTy(NULL, Ty_Void());
		}
		S_endScope(v);
		S_endScope(t);
		return body;
	}


	default:
		return expTy(NULL, Ty_Void());
	}
}

void transDec(S_table v, S_table t, A_dec d) {

	switch (d->kind) {
	case A_varDec:{
		struct expty init = transExp(v, t, d->u.var.init);
		//dec that dont mention type
		if (!d->u.var.typ) {
			if (init.ty->kind == Ty_nil) {
				EM_error(d->pos, "init should not be nil without type specified");
				S_enter(v, d->u.var.var, E_VarEntry(Ty_Void()));
				return;
			}
			S_enter(v, d->u.var.var, E_VarEntry(init.ty));
			return;
		}
		//find typ
		Ty_ty typ = S_look(t, d->u.var.typ);
		if (!typ) {
			EM_error(d->pos, "undefined type %s", S_name(d->u.var.typ));
			return;
		}
		if (!checkType(typ, init.ty)) {
			EM_error(d->pos, "type mismatch");
			S_enter(v, d->u.var.var, E_VarEntry(typ));
			return;
		}
		S_enter(v, d->u.var.var, E_VarEntry(typ));
		return;
	}
	case A_functionDec:{
		Ty_ty result;
		A_fundecList functionl = d->u.function;
		A_fundec function;
		//first loop add func into scope
		while (functionl) {
			function = functionl->head;
			//check name
			if(S_look(v, function->name)){
				EM_error(d->pos, "two functions have the same name");
				functionl = functionl->tail;
				continue;
			}
			//check result type
			if (function->result) {
			 result = S_look(t, function->result);
				if (!result) {
					EM_error(function->pos, "undefined type for return type");
					result = Ty_Void();
				}
			}
			else
				result = Ty_Void();
			//check param type
			//need change params to tylist to add entry
			Ty_tyList paramTy = params2tyList(t, function->params);
			S_enter(v, function->name, E_FunEntry(paramTy, result));
			functionl = functionl->tail;
		}

		//check body
		functionl = d->u.function;


		while (functionl) {
			function = functionl->head;
			//scope
			S_beginScope(v);
			//add params and check param
			Ty_tyList paramTy =params2tyList(t, function->params);
			A_fieldList paramFld = function->params;
			while (paramFld && paramTy) {
				S_enter(v, paramFld->head->name, E_VarEntry(paramTy->head));
				paramFld = paramFld->tail;
				paramTy = paramTy->tail;
			}
			//check body
			struct expty body = transExp(v, t, function->body);

			//check return value here
			E_enventry funcEnt = S_look(v, function->name);
			if (!checkType(funcEnt->u.fun.result, body.ty)) {
				EM_error(function->pos, "procedure returns value");
			}
			S_endScope(v);

			//to next function
			functionl = functionl->tail;
		}
		return;
	}
	case A_typeDec:{
		// type a = int
		A_nametyList nametyList = d->u.type;
		while (nametyList){
			Ty_ty namety = S_look(t, nametyList->head->name);
			//if find the type name
			if(namety){
				//if name same
				//neednt check if the type is diff
				//if(transTy(t, nl->head->ty) != namety->u.name.ty)
				EM_error(d->pos, "two types have the same name");
			}
			//add blank entry first
			S_enter(t, nametyList->head->name, Ty_Name(nametyList->head->name,NULL));
			nametyList = nametyList->tail;
		}


		//check type cycle and edit the entry
		Ty_ty resultTy;
		bool typeCycle = TRUE;
		nametyList = d->u.type;
		//only do naiive check
		while (nametyList) {
			resultTy = transTy(t, nametyList->head->ty);
			//this means no type cycle
			if (resultTy->kind != Ty_name){
				typeCycle = FALSE;
			}
			//edit the table entry
			Ty_ty namety = S_look(t, nametyList->head->name);
			namety->u.name.ty = resultTy;
			//do next
			nametyList = nametyList->tail;
		}
		if (typeCycle) EM_error(d->pos,"illegal type cycle: cycle must contain record, array");
		return;
	}
	default:
		return;
	}
}

Ty_ty transTy(S_table tb, A_ty ty) {
	switch (ty->kind) {
	case A_nameTy:{
		Ty_ty tyTy = S_look(tb, ty->u.name);
		//neednt check here??
		// if (!tyTy) {
		// 	EM_error(ty->pos, "undefined type %s", S_name(ty->u.name));
		// 	return Ty_Int();
		// }
		return tyTy;
	}
	case A_arrayTy:{
		Ty_ty tyTy = S_look(tb, ty->u.array);
		//neednt check here??
		// if (!tyTy){
		// 	EM_error(ty->pos, "undefined type %s", S_name(ty->u.array));
		// 	return Ty_Array(tyTy);
		// }
		return Ty_Array(tyTy);
	}
	case A_recordTy:{
		Ty_fieldList fieldTys = fieldList2tyFieldList(tb, ty->u.record);
		return Ty_Record(fieldTys);
	}
	default:
		return Ty_Int();
	}
}



Ty_ty actual_ty(Ty_ty ty){
	if (!ty) {
		printf("no ty");
		return ty;
	}
	if (ty->kind == Ty_name)
		return actual_ty(ty->u.name.ty);
	else
		return ty;
}

bool checkType(Ty_ty a, Ty_ty b) {
	Ty_ty actualA = actual_ty(a);
	Ty_ty actualB = actual_ty(b);
	//Ty_record, Ty_nil, Ty_int, Ty_string, Ty_array,Ty_name, Ty_void
	if(actualA->kind==Ty_nil||actualB->kind==Ty_nil)
		return TRUE;
	if(actualA->kind != Ty_record && actualA->kind != Ty_array && actualA->kind==actualB->kind)
		return TRUE;
	//if record or array, only actual ty equal means sametype
	if(actualA == actualB)
		return TRUE;
	return FALSE;
}



Ty_fieldList fieldList2tyFieldList(S_table t, A_fieldList fl) {
	Ty_fieldList listItr = NULL, listHead = NULL;
	Ty_ty tyItem;

	//do the first one
	if (fl) {
		tyItem = S_look(t, fl->head->typ);
		//find no ty
		if (!tyItem) {
			EM_error(fl->head->pos, "undefined type %s", S_name(fl->head->typ));
		} else {
			listItr = Ty_FieldList(Ty_Field(fl->head->name, tyItem), NULL);
			listHead = listItr;
		}
		fl = fl->tail;
	}

	while (fl) {
		tyItem = S_look(t, fl->head->typ);
		//find no ty
		if (!tyItem) {
			EM_error(fl->head->pos, "undefined type %s", S_name(fl->head->typ));
		} else {
			listItr->tail = Ty_FieldList(Ty_Field(fl->head->name, tyItem), NULL);
			listItr = listItr->tail;
		}
		fl = fl->tail;
	}
	return listHead;
}

Ty_tyList params2tyList(S_table t, A_fieldList fl) {
	Ty_tyList listItr = NULL, listHead = NULL;
	Ty_ty tyItem;

	//do the first one
	if(fl){
		tyItem = S_look(t, fl->head->typ);
		//find no ty
		if(!tyItem) {
			EM_error(fl->head->pos, "undefined type %s", S_name(fl->head->typ));
			tyItem = Ty_Int();
		}
		listItr = Ty_TyList(tyItem, NULL);
		listHead = listItr;
		fl = fl->tail;
	}

	while (fl) {
		tyItem = S_look(t, fl->head->typ);
		//find no ty
		if(!tyItem) {
			EM_error(fl->head->pos, "undefined type %s", S_name(fl->head->typ));
			tyItem = Ty_Int();
		}
		//add to tylist
		listItr->tail = Ty_TyList(tyItem, NULL);
		listItr = listItr->tail;
		fl = fl->tail;
	}

	return listHead;
}
