#include <stdio.h>
#include "util.h"
#include "errormsg.h"
#include "symbol.h"
#include "absyn.h"
#include "types.h"
#include "env.h"
#include "temp.h"
#include "tree.h"
#include "frame.h"
#include "semant.h"
#include "translate.h"
#include "printtree.h"

static S_symbol loopVar;
static bool checkAssignLeft = FALSE;

struct expty expTy(Tr_exp exp, Ty_ty ty){
	struct expty e;
	e.exp = exp;
	e.ty = ty;
	return e;
}

F_fragList SEM_transProg(A_exp exp){
	struct expty et;
	S_table tenv = E_base_tenv();
	S_table venv = E_base_venv();
	et = transExp(Tr_outermost(), venv, tenv, NULL, exp);
	FILE *fp;
	fp=fopen("tree.txt","wr");
	T_stmList stml = NULL;
	switch(et.exp->kind){
		case(Tr_ex):
			stml = T_StmList(T_Exp( et.exp->u.ex), NULL);
			break;
		case(Tr_nx):
			stml = T_StmList(et.exp->u.nx, NULL);
			break;
		case(Tr_cx):
			stml = T_StmList(et.exp->u.cx.stm, NULL);
	}
	printStmList (fp, stml) ;
	Tr_procEntryExit(Tr_outermost(), et.exp, NULL);//manual add a blank level for test
	return Tr_getFragList();
}


struct expty transVar(Tr_level level, S_table venv, S_table tenv, Tr_exp breakk, A_var v)
{
	switch(v->kind) {
		// var id a
		case A_simpleVar:{
			E_enventry varEnt = S_look(venv, v->u.simple);

			if(checkAssignLeft && loopVar != NULL && !strcmp(S_name(v->u.simple),S_name(loopVar))){
				EM_error(v->pos, "loop variable can't be assigned");

			}checkAssignLeft = FALSE;

			Tr_exp translation = Tr_noExp();

			if (!varEnt) {
				EM_error(v->pos, "undefined variable %s", S_name(v->u.simple));
				return expTy(translation, Ty_Int());
			}
			if(varEnt->kind != E_varEntry) {
				EM_error(v->pos, "undefined variable %s", S_name(v->u.simple));
				return expTy(translation, Ty_Int());
			}

			translation = Tr_simpleVar(varEnt->u.var.access, level);
			return expTy(translation, actual_ty(varEnt->u.var.ty));
		}
		//var record a.b
		case A_fieldVar:{
			struct expty var = transVar(level, venv, tenv, breakk, v->u.field.var);
			if (var.ty->kind != Ty_record) {
				EM_error(v->u.field.var->pos, "not a record type");
				return expTy(Tr_noExp(), Ty_Int());
			}
			Ty_fieldList fieldList = var.ty->u.record;
			int offset = 0;
			while (fieldList) {
				if (fieldList->head->name == v->u.field.sym)
					return expTy(Tr_fieldVar(var.exp, offset), actual_ty(fieldList->head->ty));
				fieldList= fieldList->tail;
				offset++;
			}
			EM_error(v->pos, "field %s doesn't exist", S_name(v->u.field.sym));
			return expTy(Tr_noExp(), Ty_Int());
		}
		//var a[b]
		case A_subscriptVar:{
			struct expty var = transVar(level, venv, tenv, breakk, v->u.subscript.var);
			Tr_exp translation = Tr_noExp();
			if (var.ty->kind != Ty_array) {
				EM_error(v->u.subscript.var->pos, "array type required");
				//EM_error(v->pos, "array type required");
				return expTy(translation, Ty_Int());
			}
			struct expty index = transExp(level, venv, tenv, breakk, v->u.subscript.exp);
			if (index.ty->kind != Ty_int) {
				EM_error(v->u.subscript.exp->pos, "int required");
				//EM_error(v->pos, "int required");
			} else {
				translation =  Tr_subscriptVar(var.exp, index.exp);
			}
			return expTy(translation, actual_ty(var.ty->u.array));
		}
		default:
			EM_error(v->pos, "error var type");//assert(0);
	}

}

struct expty transExp(Tr_level level, S_table venv, S_table tenv, Tr_exp breakk, A_exp a)
{
	switch (a->kind) {
		case A_varExp:
			return transVar(level, venv, tenv, breakk, a->u.var);
		case A_intExp:
			return expTy(Tr_intExp(a->u.intt), Ty_Int());
		case A_stringExp:
			return expTy(Tr_stringExp(a->u.stringg), Ty_String());
		case A_nilExp:
			return expTy(Tr_nilExp(), Ty_Nil());


		case A_opExp:{
			A_oper oper = a->u.op.oper;
			struct expty left = transExp(level, venv, tenv, breakk, a->u.op.left);
			struct expty right = transExp(level, venv, tenv, breakk, a->u.op.right);
			Tr_exp translation = Tr_noExp();

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
					return expTy(Tr_arithExp(oper, left.exp, right.exp), Ty_Int());
			}

			//== & <> should compare
			if (oper == A_eqOp || oper == A_neqOp) {
				switch(left.ty->kind) {
					case Ty_int:
						if (!checkType(right.ty, left.ty))
							EM_error(a->u.op.left->pos, "same type required");
						translation = Tr_eqExp(oper, left.exp, right.exp);
						break;
					case Ty_string:
						if (!checkType(right.ty, left.ty))
							EM_error(a->u.op.left->pos, "same type required");
						translation = Tr_eqStringExp(oper, left.exp, right.exp);
						break;
					case Ty_array:
						if (right.ty->kind != left.ty->kind)
							EM_error(a->u.op.left->pos, "same type required");
						translation = Tr_eqRef(oper, left.exp, right.exp);
						break;
					case Ty_record:
						if (right.ty->kind != Ty_record && right.ty->kind != Ty_nil)
							EM_error(a->u.op.left->pos, "same type required");
						translation = Tr_eqRef(oper, left.exp, right.exp);
						break;
				}
				return expTy(translation, Ty_Int());
			}
			if(oper <=A_geOp){
				if (right.ty->kind != left.ty->kind) {
					EM_error(a->u.op.right->pos,
						"%s expression given for RHS; expected %s",
						Ty_ToString(right.ty), Ty_ToString(left.ty));
				}
				switch(left.ty->kind) {
					case Ty_int:
						translation = Tr_relExp(oper, left.exp, right.exp);
						break;
					case Ty_string:
						translation = Tr_noExp();
						break;
					default:
						translation = Tr_noExp();
				}
				return expTy(translation, Ty_Int());
			}
			assert(0 && "Invalid operator in expression");
			return expTy(Tr_noExp(), Ty_Int());
		}
		case A_callExp:	{
			A_expList calList = NULL;
			Ty_tyList argtyList;
			E_enventry callEnt = S_look(venv, a->u.call.func);
			Tr_exp translation = Tr_noExp();
			Tr_expList argList = Tr_ExpList();

			if(!callEnt||callEnt->kind!=E_funEntry){
				EM_error(a->pos, "undefined function %s\n", S_name(a->u.call.func));
				return expTy(translation, Ty_Int());
			}
			//match arg one by one can not use transexp twice here
			argtyList = callEnt->u.fun.formals;
			calList = a->u.call.args;
			while (calList && argtyList) {
				struct expty calArg = transExp(level, venv, tenv, breakk, calList->head);
				if (!checkType(calArg.ty, argtyList->head))
					EM_error(a->pos, "para type mismatch");
				Tr_ExpListAdd2Tail(argList, calArg.exp);
				argtyList = argtyList->tail;
				calList = calList->tail;
			}

			if (!calList&& argtyList)
				EM_error(a->pos, "less params in function %s\n", S_name(a->u.call.func));
			if (calList && !argtyList)
				EM_error(a->pos,"too many params in function %s\n", S_name(a->u.call.func));
			translation = Tr_callExp(level, callEnt->u.fun.level, callEnt->u.fun.label, argList);
			return expTy(translation, actual_ty(callEnt->u.fun.result));
		}

		case A_recordExp:
		{
			Ty_ty recordTy = S_look(tenv, a->u.record.typ);
			recordTy = actual_ty(recordTy);
			//no record type
			if (!recordTy) {
				EM_error(a->pos, "undefined type");
				return expTy(Tr_noExp(), Ty_Record(NULL));
			}
			//diff record type
			if (recordTy->kind != Ty_record)
				EM_error(a->pos, "%s is not a record type", S_name(a->u.record.typ));

			Ty_fieldList rectyList = recordTy->u.record;
			A_efieldList expList = a->u.record.fields;
			Tr_expList list = Tr_ExpList();
			int n = 0;
			//match rec one by one
			while(rectyList && expList) {
				struct expty recItem = transExp(level, venv, tenv, breakk, expList->head->exp);
				//if (expList->head->name != rectyList->head->name)
				//	EM_error(a->pos, "%s not a valid field name", expList->head->name);
				if (!checkType(rectyList->head->ty, recItem.ty))
					EM_error(a->pos, "unmatched name: type in %s", S_name(a->u.record.typ));
				Tr_ExpListAdd(list, recItem.exp);
				expList = expList->tail;
				rectyList = rectyList->tail;
				n++;
			}
			if (!rectyList && !expList)
				return expTy(Tr_recordExp(n, list), recordTy);
			if (rectyList) {
				EM_error(a->pos, "less fields in %s", S_name(a->u.record.typ));
			}
			if (expList) {
				EM_error(a->pos, "too many field in %s", S_name(a->u.record.typ));
			}
			return expTy(Tr_recordExp(n, list), recordTy);
		}
		case A_arrayExp:{
			Ty_ty arrayTy = S_look(tenv, a->u.array.typ);
			arrayTy = actual_ty(arrayTy);
			//no array ty
			if (!arrayTy) {
				EM_error(a->pos, "undeined array type %s", S_name(a->u.array.typ));
				return expTy(Tr_noExp(), Ty_Int());
			}
			//diff array kind
			if (arrayTy->kind != Ty_array) {
				EM_error(a->pos, "%s is not a array type", S_name(a->u.array.typ));
				return expTy(Tr_noExp(), Ty_Int());
			}
			struct expty size = transExp(level, venv, tenv, breakk, a->u.array.size);
			struct expty init = transExp(level, venv, tenv, breakk, a->u.array.init);
			//size not int
			if (size.ty->kind != Ty_int){
				EM_error(a->pos, "array size should be int %s", S_name(a->u.array.typ));
				return expTy(Tr_noExp(), Ty_Int());
			}
			if (!checkType(arrayTy->u.array, init.ty)){
				EM_error(a->pos, "type mismatch");
				return expTy(Tr_noExp(), Ty_Int());
			}
			return expTy(Tr_arrayExp(size.exp, init.exp), arrayTy);
		}

		case A_seqExp:{
			struct expty expr = expTy(Tr_noExp(), Ty_Void());
			A_expList seq = a->u.seq;
			Tr_expList explist = Tr_ExpList();
			//trans every exp in seq
			while (seq) {
				expr = transExp(level, venv, tenv, breakk, seq->head);
				Tr_ExpListAdd(explist, expr.exp);
				seq = seq->tail;
			}
			//if empty add one default part to it
			if (!explist || !explist->head)
				Tr_ExpListAdd(explist, expr.exp);
			return expTy(Tr_seqExp(explist), expr.ty);
		}
		case A_assignExp:{
			//check loop var
			//checkAssignLeft = TRUE;
			struct expty var = transVar(level, venv, tenv, breakk, a->u.assign.var);
			//checkAssignLeft = FALSE;

			struct expty exp = transExp(level, venv, tenv, breakk, a->u.assign.exp);
			if (!checkType(var.ty, exp.ty))
				EM_error(a->u.assign.exp->pos,  "unmatched assign exp");
				//EM_error(a->pos, "unmatched assign exp");
			return expTy(Tr_assignExp(var.exp, exp.exp), Ty_Void());
		}
		case A_whileExp:{
			struct expty test = transExp(level, venv, tenv, breakk, a->u.whilee.test);
			if (test.ty->kind != Ty_int)
				EM_error(a->u.whilee.test->pos, "int required");
				//EM_error(a->pos, "int required");

			//check body
			Tr_exp doneExp = Tr_doneExp();
			struct expty body = transExp(level, venv, tenv, breakk, a->u.whilee.body);
			if (body.ty->kind != Ty_void)
				EM_error(a->u.whilee.body->pos, "while body must produce no value");
			return expTy(Tr_whileExp(test.exp, doneExp, body.exp), Ty_Void());
		}
		case A_breakExp:{
			Tr_exp translation = Tr_noExp();
			if (breakk)
				translation = Tr_breakExp(breakk);
			else
				EM_error(a->pos, "break should inside a loop ");
			return expTy(translation, Ty_Void());
		}
		case A_forExp:{
			// struct expty lo = transExp(level, venv, tenv, breakk, a->u.forr.lo);
			// struct expty hi = transExp(level, venv, tenv, breakk, a->u.forr.hi);
			// loopVar = a->u.forr.var;
			//
			// if (lo.ty != Ty_Int() || hi.ty != Ty_Int()) {
			// 	EM_error(a->pos, "for exp's range type is not integer");
			// 	//EM_error(a->pos, "loop variable can't be assigned");
			// }
			// //scope
			// S_beginScope(venv);
			// //add var into scope
			// transDec(level, venv, tenv, breakk, A_VarDec(a->pos, a->u.forr.var, S_Symbol("int"), a->u.forr.lo));
			// struct expty body = transExp(level, venv, tenv, breakk, a->u.forr.body);
			// //check body kind
			// if (body.ty->kind != Ty_void){
			// 	EM_error(a->u.forr.body->pos, "for body must produce no value");
			// 	return expTy(NULL, Ty_Void());
			// }
			// S_endScope(venv);
			// loopVar = NULL;
			// return body;


			A_dec i = A_VarDec(a->pos, a->u.forr.var, NULL, a->u.forr.lo);
			A_dec limit = A_VarDec(a->pos, S_Symbol("limit"), NULL, a->u.forr.hi);
			A_dec test = A_VarDec(a->pos, S_Symbol("test"), NULL, A_IntExp(a->pos, 1));
			A_exp testExp = A_VarExp(a->pos, A_SimpleVar(a->pos, S_Symbol("test")));
			A_exp iExp = A_VarExp(a->pos, A_SimpleVar(a->pos, a->u.forr.var));
			A_exp limitExp = A_VarExp(a->pos, A_SimpleVar(a->pos, S_Symbol("limit")));
			A_exp increment = A_AssignExp(a->pos,
				A_SimpleVar(a->pos, a->u.forr.var),
				A_OpExp(a->pos, A_plusOp, iExp, A_IntExp(a->pos, 1)));
			A_exp setFalse = A_AssignExp(a->pos,
					A_SimpleVar(a->pos, S_Symbol("test")), A_IntExp(a->pos, 0));
					/* The let expression we pass to this function */
			A_exp letExp = A_LetExp(a->pos,
					A_DecList(i, A_DecList(limit, A_DecList(test, NULL))),
						A_SeqExp(a->pos,
							A_ExpList(
								A_IfExp(a->pos,
									A_OpExp(a->pos, A_leOp, a->u.forr.lo, a->u.forr.hi),
										A_WhileExp(a->pos, testExp,
											A_SeqExp(a->pos,
												A_ExpList(a->u.forr.body,
													A_ExpList(
														A_IfExp(a->pos,
															A_OpExp(a->pos, A_ltOp, iExp, limitExp),increment, setFalse),
															NULL)))), NULL),NULL)));

				struct expty e = transExp(level, venv, tenv, breakk, letExp);
				return e;
		}
		case A_ifExp:{
			struct expty test = transExp(level, venv, tenv, breakk, a->u.iff.test);
			if (test.ty->kind != Ty_int)
				EM_error(a->u.iff.test->pos, "integer required");
			struct expty then = transExp(level, venv, tenv, breakk, a->u.iff.then);
			struct expty elsee;//cannot use trans here which will cause segfault
			if (a->u.iff.elsee) {
				elsee = transExp(level, venv, tenv, breakk, a->u.iff.elsee);
				//use kind == kind or tycheck(a,b) are both ok here
				if (a->u.iff.elsee->kind != A_nilExp && then.ty->kind != elsee.ty->kind) {
					EM_error(a->u.iff.elsee->pos, "then exp and else exp type mismatch");
				}
				//need to return here
				return expTy(Tr_ifExp(test.exp, then.exp, elsee.exp), then.ty);
			}

			//if there is no else, if cannot be in a func. then must be no value;
			if (then.ty->kind != Ty_void)
				EM_error(a->u.iff.then->pos, "if-then exp's body must produce no value");
			return expTy(Tr_ifExp(test.exp, then.exp, NULL), Ty_Void());
		}
		case A_letExp:
		{
			Tr_expList list = Tr_ExpList();
			S_beginScope(venv);
			S_beginScope(tenv);
			A_decList decs = a->u.let.decs;
			while (decs){
				Tr_exp trexp = transDec(level, venv, tenv, breakk, decs->head);
				Tr_ExpListAdd(list, trexp);
				decs = decs->tail;
			}
			struct expty body = transExp(level, venv, tenv, breakk, a->u.let.body);
			//neednt check the body type anymore cuz in function let body can return sth
			Tr_ExpListAdd(list, body.exp);
			S_endScope(venv);
			S_endScope(tenv);
			return expTy(Tr_seqExp(list), body.ty);
		}
		default:
			return expTy(NULL, Ty_Void());
	}
}


Tr_exp transDec(Tr_level level, S_table venv, S_table tenv, Tr_exp breakk, A_dec d)
{
	switch (d->kind) {
		case A_varDec:{
			struct expty init = transExp(level, venv, tenv, breakk, d->u.var.init);
			//dec that dont mention type
			Tr_access access = Tr_allocLocal(level, TRUE);
			if (!d->u.var.typ){
				if (init.ty->kind == Ty_nil)
					EM_error(d->pos, "init should not be nil without type specified");
				S_enter(venv, d->u.var.var, E_VarEntry(access, init.ty));
				return Tr_assignExp(Tr_simpleVar(access, level), init.exp);
			}
			//find typ
			Ty_ty typ = S_look(tenv, d->u.var.typ);
			typ = actual_ty(typ);
			if (!typ) {
				EM_error(d->pos, "undefined type %s", S_name(d->u.var.typ));
				typ = Ty_Void();//give a default ty
			}
			if (!checkType(typ, init.ty))
				EM_error(d->pos, "type mismatch");
			S_enter(venv, d->u.var.var, E_VarEntry(access, typ));
			return Tr_assignExp(Tr_simpleVar(access, level), init.exp);
		}
		case A_functionDec:{
			Ty_ty result;
			A_fundecList functionl = d->u.function;
			A_fundec function;


			//first loop add func into scope
			while(functionl){
				result = NULL;
				function = functionl->head;
				//neednt check function same name

				if (function->result) {
					result = S_look(tenv, function->result);
					if (!result) {
						EM_error(function->pos, "undefined type for return type");
						result = Ty_Void();
					}
				}
				else
				 result = Ty_Void();
			  //check param type
	 			//need change params to tylist to add entry

				Ty_tyList formalTys = params2tyList(tenv, function->params);
				U_boolList formals = makeFormals(function->params);
				Temp_label funLabel = Temp_newlabel();
				Tr_level funLevel = Tr_newLevel(level, funLabel, formals);
				S_enter(venv, function->name, E_FunEntry(funLevel, funLabel, formalTys, result));
				functionl = functionl->tail;
			}
			//check body
			E_enventry funEntry = NULL;
			functionl = d->u.function;
			while (functionl) {
				function = functionl->head;
				funEntry = S_look(venv, function->name);
				//scope
				S_beginScope(venv);
				//add params and check param and got accesslist
				Ty_tyList paramTys = funEntry->u.fun.formals;
				A_fieldList paramFld = function->params;
				Tr_accessList accessList = Tr_formals(funEntry->u.fun.level);
				while (paramFld && paramTys) {
					S_enter(venv, paramFld->head->name, E_VarEntry(accessList->head, paramTys->head));
					paramFld = paramFld->tail;
					paramTys = paramTys->tail;
					accessList = accessList->tail;
				}
				//check body
				struct expty body = transExp(funEntry->u.fun.level, venv, tenv,breakk, function->body);
				//check return value here
				if (!checkType(funEntry->u.fun.result, body.ty))
						EM_error(function->pos, "procedure returns value");
				//procentry here
				Tr_procEntryExit(funEntry->u.fun.level, body.exp, accessList);
				S_endScope(venv);
				//to next function
				functionl = functionl->tail;
			}
			return Tr_noExp();
		}

		case A_typeDec:{
			// type a = int
			A_nametyList nameList = d->u.type;

			/* "headers" first */
			while(nameList){
				Ty_ty namety = S_look(tenv, nameList->head->name);
				//if find the type name
				if(namety){
					//neednt check this time?
					//EM_error(d->pos, "two types have the same name");
				}
				S_enter(tenv, nameList->head->name, Ty_Name(nameList->head->name, NULL));
				nameList = nameList->tail;
			}

			//check type cycle and edit the entry
			Ty_ty resultTy;
			bool typeCycle = TRUE;
			nameList = d->u.type;
			//only do naiive check
			while (nameList) {
				resultTy = transTy(tenv, nameList->head->ty);
				//this means no type cycle
				if (resultTy->kind != Ty_name){
					typeCycle = FALSE;
				}

				//edit the table entry
				Ty_ty nameTy = S_look(tenv, nameList->head->name);
				nameTy->u.name.ty = resultTy;
				//do next
				nameList = nameList->tail;
			}
			if (typeCycle)
				EM_error(d->pos,"illegal type cycle: cycle must contain record, array");
			return Tr_noExp();
		}
	}
	assert(0);
}

U_boolList makeFormals(A_fieldList params){
	U_boolList listHead = NULL;
	U_boolList listItr = NULL;
	if(params){
		listHead = U_BoolList(TRUE, NULL);
		listItr = listHead;
		params = params->tail;
	}

	while (params) {
		listItr->tail = U_BoolList(TRUE, NULL);
		listItr = listItr->tail;
		params = params->tail;
	}
	return listHead;
}

Ty_ty transTy(S_table tb, A_ty ty) {
	switch (ty->kind) {
	case A_nameTy:{
		Ty_ty tyTy = S_look(tb, ty->u.name);
		//neednt check here??
		if (!tyTy) {
			EM_error(ty->pos, "undefined type %s", S_name(ty->u.name));
		// 	return Ty_Int();
		}
		return tyTy;
	}
	case A_arrayTy:{
		Ty_ty tyTy = S_look(tb, ty->u.array);
		//neednt check here??
		if (!tyTy){
		 	EM_error(ty->pos, "undefined type %s", S_name(ty->u.array));
		// 	return Ty_Array(tyTy);
		}
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
		//printf("no ty");
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
