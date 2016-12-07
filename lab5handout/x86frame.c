#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "symbol.h"
#include "temp.h"
#include "table.h"
#include "tree.h"
#include "frame.h"

struct F_access_ {
	enum { inFrame, inReg } kind;
	union {
		int offset;
		Temp_temp reg;
	} u;
};
//mentioned by cl
const int F_WORD_SIZE = 4;//for stack move
//
struct F_frame_ {
	Temp_label name;
	F_accessList formals;
	int local_count;
};

F_access InFrame(int offset);
F_access InReg(Temp_temp reg);
F_accessList F_AccessList(F_access head, F_accessList tail);
F_accessList makeFormalAccessList(F_frame f, U_boolList formals);

F_accessList F_AccessList(F_access head, F_accessList tail){
	F_accessList list = malloc(sizeof(*list));
	list->head = head;
	list->tail = tail;
	return list;
}

F_accessList makeFormalAccessList(F_frame f, U_boolList formals){
	F_accessList headList = NULL, tailList = NULL;
	//int i = 0;
	while (formals) {
		static int i = 0;
		F_access access = NULL;
		if (i < 6 && !formals->head) {//6 for f_k
			access = InReg(Temp_newtemp());
		} else {
			access = InFrame((2 + i) * F_WORD_SIZE);
		}
		if (headList) {
			tailList->tail = F_AccessList(access, NULL);
			tailList = tailList->tail;
		} else {
			headList = F_AccessList(access, NULL);
			tailList = headList;
		}
		formals = formals->tail;
		i++;
	}
	return headList;
}

F_access InFrame(int offset){
	F_access fa = malloc(sizeof(*fa));
	fa->kind = inFrame;
	fa->u.offset = offset;
	return fa;
}

F_access InReg(Temp_temp reg){
	F_access fa = malloc(sizeof(*fa));
	fa->kind = inReg;
	fa->u.reg = reg;
	return fa;
}

//Temp_map F_tempMap = NULL;
//static void (string str, Temp_temp temp){
//	if (!F_tempMap) {
//		F_tempMap = Temp_name();
//	}
//	Temp_enter(F_tempMap, temp, str);
//}

F_frame F_newFrame(Temp_label name, U_boolList formals){
	F_frame f = malloc(sizeof(*f));
	f->name = name;
	f->formals = makeFormalAccessList(f, formals);
	f->local_count = 0;
	return f;
}
Temp_label F_name(F_frame f){
	return f->name;
}

F_accessList F_formals(F_frame f){
	return f->formals;
}

F_access F_allocLocal(F_frame f, bool escape){
	f->local_count++;
	if (escape)
		return InFrame(F_WORD_SIZE * (- f->local_count));
	return InReg(Temp_newtemp());
}

F_frag F_StringFrag(Temp_label label, string str){
	F_frag strfrag = malloc(sizeof(*strfrag));
	strfrag->kind = F_stringFrag;
	strfrag->u.stringg.label = label;
	strfrag->u.stringg.str = str;
	return strfrag;
}

F_frag F_ProcFrag(T_stm body, F_frame frame){
	F_frag pfrag = malloc(sizeof(*pfrag));
	pfrag->kind = F_procFrag;
	pfrag->u.proc.body = body;
	pfrag->u.proc.frame = frame;
	return pfrag;
}

F_fragList F_FragList(F_frag head, F_fragList tail){
	F_fragList fl = malloc(sizeof(*fl));
	fl->head = head;
	fl->tail = tail;
	return fl;
}

//naiive do firstand dont add to map ("ebp", fp)here;
static Temp_temp fp = NULL;
Temp_temp F_FP(void){
	if (!fp) {
		fp = Temp_newtemp();//add

	}
	return fp;
}


T_exp F_Exp(F_access access, T_exp framePtr){
	if (access->kind == inFrame) {//frame
		return T_Mem(T_Binop(T_plus, framePtr, T_Const(access->u.offset)));
	} else {//access
		return T_Temp(access->u.reg);
	}
}

T_exp F_externalCall(string str, T_expList args){
	return T_Call(T_Name(Temp_namedlabel(str)), args);
}

T_stm F_procEntryExit1(F_frame frame, T_stm stm){
	return stm;//naiive
}

static Temp_tempList returnSink = NULL;
