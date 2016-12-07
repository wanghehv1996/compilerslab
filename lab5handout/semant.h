#ifndef TIGER_SEMANT_H_
#define TIGER_SEMANT_H_

#include "absyn.h"
#include "types.h"
#include "translate.h"

struct expty {Tr_exp exp; Ty_ty ty;};
struct expty expTy(Tr_exp exp, Ty_ty ty);

struct expty transExp(Tr_level level, S_table venv, S_table tenv, Tr_exp breakk, A_exp a);
struct expty transVar(Tr_level level, S_table venv, S_table tenv, Tr_exp breakk, A_var v);
Tr_exp transDec(Tr_level level, S_table venv, S_table tenv, Tr_exp breakk, A_dec d);
Ty_ty transTy(S_table tenv, A_ty t);

Ty_ty actual_ty(Ty_ty ty);
bool checkType(Ty_ty tType, Ty_ty eType);

Ty_fieldList fieldList2tyFieldList(S_table t, A_fieldList fl);
Ty_tyList params2tyList(S_table t, A_fieldList fl);

U_boolList makeFormals(A_fieldList params);
F_fragList SEM_transProg(A_exp exp);

#endif /* TIGER_SEMANT_H_ */
