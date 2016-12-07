#ifndef FRAME_H_
#define FRAME_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "temp.h"
#include "tree.h"
#include "absyn.h"

typedef struct F_frame_ *F_frame;
typedef struct F_access_ *F_access;

typedef struct F_accessList_ *F_accessList;

struct F_accessList_ {
	F_access head;
	F_accessList tail;
};

typedef struct F_frag_ *F_frag;
struct F_frag_ {
	enum {F_stringFrag, F_procFrag} kind;
	union {
		struct {
			Temp_label label;
			string str;
		} stringg;
		struct {
			T_stm body;
			F_frame frame;
		} proc;
	} u;
};

typedef struct F_fragList_ *F_fragList;
struct F_fragList_ {
	F_frag head;
	F_fragList tail;
};

extern const int F_WORD_SIZE;


F_frame F_newFrame(Temp_label name, U_boolList formals);
Temp_label F_name(F_frame frame);
F_accessList F_formals(F_frame frame);
F_access F_allocLocal(F_frame frame, bool escape);

F_frag F_StringFrag(Temp_label label, string str);
F_frag F_ProcFrag(T_stm body, F_frame frame);
F_fragList F_FragList(F_frag head, F_fragList tail);


Temp_temp F_FP(void);
T_exp F_Exp(F_access access, T_exp framePtr);
T_exp F_externalCall(string str, T_expList args);
T_stm F_procEntryExit1(F_frame frame, T_stm stm);



#endif
