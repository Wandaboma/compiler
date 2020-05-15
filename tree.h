#ifndef _TREE_H_
#define _TREE_H_
typedef struct Type_* Type;
typedef struct Arg_* Arg;
typedef struct FieldList_* FieldList;
typedef struct FuncList_* FuncList;

struct Type_ {
	enum {BASIC, ARRAY, STRUCTURE} kind;
	union {
		int basic;//int = 0, float = 1
		struct {Type elem; int size; } array;
		FieldList structure;
	} u;
};

struct FieldList_ {
	char* name;
	Type type;
	int line;
	int tag;
	FieldList tail;
};

int fieldLevel;

FieldList table[50];

struct Arg_ {
	char* name;
	Arg next;
};
Arg argTable;

struct FuncList_ {
	//0 undefined
	int flag;
	int line;
	char* name;
	Type type;
	FieldList paramList;
	FuncList tail;
};

FuncList funcTable;

struct Node {
	int f;
	int line, num;
	char* info;
	Type t;
	FieldList head, tail;
	struct Node* child[10];
	union {
		unsigned int type_int;
		float type_float;
		char* type_string;
	};
};

struct Node* root;
int flag;
void yyerror(char* msg);
struct Node* createIntNode(int L, unsigned int val, char* text);
struct Node* createFloatNode(int L, float val, char* text);
struct Node* createStrNode(int L, char* info, char* text);
void addNode(struct Node* top, struct Node* down);
void printTree(struct Node* top, int level);
void myStrcpy(char** str, char* temp);
void output(FieldList temp);
int typeCompare(Type t1, Type t2);
int addTable(FieldList *temp);
int paramCompare(FieldList p1, FieldList p2);
int addFuncTable(FuncList *temp);
FuncList findFunc(char* str);
Type findStruct(char* str, int level);
Type findVar(char* str, int level);
void prepare();
void semanticAnal(struct Node* node);
#endif
