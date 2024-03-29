#ifndef _TRANS_H_
#define _TRANS_H_
typedef struct Operand_* Operand;
typedef struct InterCodes_* InterCodes;
struct Operand_ {
	enum { VARIABLE, CONSTANT, ADDRESS, LOCATION} kind;
//addr &      location *
	union {
		int var_no;
		int value;
		char* val_na;
	} u;
};

struct InterCode {//one line of code
	enum {ASSIGN, ADD, SUB, MUL, DIVI, LABEL, CON, GOTO, BACK,
		READ, WRITE, CALL, ARG, FUNC, PARAM, DEC} kind;
	union {
		struct {Operand right, left;} assign;
		struct {Operand result, op1, op2;} binop;
		struct {Operand pos;} label; 
		struct {Operand pos;} go;
		struct {Operand op1, op2, op3, pos;} con;
		struct {Operand op1;} back;
		struct {Operand name;} read;
		struct {Operand name;} write;
		struct {Operand op1, name;} call;
		struct {Operand name;} arg;
		struct {Operand name;} func;
		struct {Operand name;} param;
		struct {Operand name, size;} dec;
	} u;
};

struct InterCodes_ {//code structure with pointer
	struct InterCode code;
	InterCodes prev, next;
};

struct codeList{
	InterCodes head, tail;
};

int tempNum, labelNum;

struct argStr{
	char* str;
	struct argStr* next;
};

int offTemp, paramTemp;
struct offset{
	char* str;
	int t;
	struct offset* next;
};
struct offset* sizeTemp;
struct offset* offTable;
struct offset* paramTable;
struct offset* sizeTable;

struct codeList translate(struct Node* node);
void printOp(Operand p);
void printCode(InterCodes node);
char* newTemp();
char* newLabel();
struct codeList createLabel(char* name);
struct codeList createGoto(char* name);
void createVar(Operand* t, char* name);
void createConstant(Operand* t, int q);
void createLocation(Operand* t, char* name);
void createAddress(Operand* t, char* name);
struct codeList codePatch(InterCodes a, InterCodes* b, InterCodes* c, InterCodes d);
struct codeList translate_Exp(struct Node* node, char* place);
struct codeList translate_Cond(struct Node* node, char* t, char* f);
struct codeList translate_Stmt(struct Node* node);
struct codeList translate_CompSt(struct Node* node);
struct codeList translate_Args(struct Node* node, struct argStr ** argList);
struct codeList translate_FunDec(struct Node* node);
struct codeList translate_VarList(struct Node* node);
struct codeList translate_ParamDec(struct Node* node);
char* translate_VarDec(struct Node* node);
struct codeList translate_Dec(struct Node* node);
struct codeList translate_Location(struct Node* node, char* place);
int getSize(Type t);
void addOffTable(char* str, int size);
#endif
