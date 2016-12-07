void SEM_transProg(A_exp exp);

typedef void* Tr_exp;
struct expty {Tr_exp exp; Ty_ty ty;};

struct expty transVar(S_table venv, S_table tenv, A_var v);
struct expty transExp(S_table venv, S_table tenv, A_exp a);
void transDec(S_table venv, S_table tenv, A_dec d);
Ty_ty transTy(S_table tenv, A_ty a);

Ty_ty actual_ty(Ty_ty ty);
bool checkType(Ty_ty a, Ty_ty b);

Ty_tyList params2tyList(S_table t, A_fieldList p);
Ty_fieldList fieldList2tyFieldList(S_table t, A_fieldList fs);
