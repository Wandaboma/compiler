#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tree.h"
extern FILE* yyin;
extern int yylineno;
//extern int yydebug;
int main(int argc, char** argv) {
	if (argc <= 1) return 1;
	FILE* f = fopen(argv[1], "r");
	filename = argv[2];
	if (!f)
	{
		perror(argv[1]);
		return 1;
	}
	flag = 1;
	fieldLevel = 0;
	memset(table, 0, sizeof(table));
	yyrestart(f);
//	yydebug = 1;
	yyparse();
	return 0;
}

void yyerror(char* msg) {
	flag = 0;
	printf("Error type B at Line %d: %s\n", yylineno, msg);
}

struct Node* createIntNode(int L, unsigned int val, char* text) {
	struct Node* temp = (struct Node*)malloc(sizeof(struct Node));
	temp->f = 0;
	temp->line = L;
	temp->num = 0;
	temp->info = (char*)malloc(sizeof(char)*strlen(text));
	strcpy(temp->info, text);
	temp->type_int = val;
	return temp;
}

struct Node* createFloatNode(int L, float val, char* text) {
	struct Node* temp = (struct Node*)malloc(sizeof(struct Node));
	temp->f = 0;
	temp->line = L;
	temp->num = 0;
	temp->info = (char*)malloc(sizeof(char)*strlen(text));
	strcpy(temp->info, text);
	temp->type_float = val;
	return temp;
}

struct Node* createStrNode(int L, char* info, char* text) {
	struct Node* temp = (struct Node*)malloc(sizeof(struct Node));
	temp->f = 0;
	temp->line = L;
	temp->num = 0;
	temp->info = (char*)malloc(sizeof(char)*strlen(text));
	strcpy(temp->info, text);
	temp->type_string = (char*)malloc(sizeof(char)*strlen(text));
	strcpy(temp->type_string, info);
	return temp;
}

void addNode(struct Node* top, struct Node* down) {
//	if(down == NULL) return;
	top->num = top->num + 1;
	top->child[top->num] = down;
}

void printTree(struct Node* top, int level) {
	int i;
	if(flag == 0) return;
	if(top == NULL) return;
	for(i = 1; i <= level; i++) {
		printf("  ");
	}
	
	if(strcmp(top->info, "ID") == 0) printf("ID: %s\n", top->type_string);
	else if(strcmp(top->info, "TYPE") == 0) printf("TYPE: %s\n", top->type_string);
	else if(strcmp(top->info, "INT") == 0) printf("INT: %u\n", top->type_int);
	else if(strcmp(top->info, "FLOAT") == 0) printf("FLOAT: %f\n", top->type_float);
	else if(top->type_string[0] <= '9' && top->type_string[0] >= '0') printf("%s (%d)\n", top->info, top->line);
	else printf("%s\n", top->info);

	for(i = 1; i <= top->num; i++) {
		printTree(top->child[i], level + 1);
	}
}

void myStrcpy(char** str, char* temp) {
	*str = (char *) malloc(sizeof(char) * strlen(temp));
	strcpy(*str, temp);
}

void output(FieldList temp){
//only for debugging
	printf("Name: %s       ", temp->name);
	Type t = temp->type;
	if(t->kind == BASIC) printf("BASIC\n");
	else if(t->kind == ARRAY){
		printf("ARRAY\n");
		while(1){
			printf("Size: [%d]  ", t->u.array.size);
			if(t->u.array.elem->kind != ARRAY) {
				printf("Type: %d \n", t->u.array.elem->kind);
				break;
			}
			t = t->u.array.elem;
		}
	}
	else{
		printf("STRUCT\n");
		FieldList p = t->u.structure;
		while(p != NULL){
			output(p);
			p = p->tail;
		}
	}
}

int typeCompare(Type t1, Type t2) {
	//type compatibility
	if (t1 == t2) return 1;
	if(t1->kind != t2->kind) return 0;
	if (t1->kind == BASIC) {
		if(t1->u.basic == t2->u.basic) return 1;
			else return 0;
	}
	else if(t1->kind == ARRAY) {
		int dim1 = 0, dim2 = 0;
		Type temp1, temp2;
		temp1 = t1;
		temp2 = t2;
		while(temp1->kind == ARRAY) {
			dim1++;
			temp1 = temp1->u.array.elem;
		}
		while(temp2->kind == ARRAY) {
			dim2++;
			temp2 = temp2->u.array.elem;
		}
		if(dim1 != dim2) return 0;
		if(typeCompare(temp1, temp2)) return 1;
			else return 0;
	}
	else{
		FieldList temp1 = t1->u.structure;
		FieldList temp2 = t2->u.structure;
		while(1){
			if((temp1 == NULL && temp2 != NULL) || (temp1 != NULL && temp2 == NULL))
				return 0;
			if(temp1 == NULL && temp2 == NULL) return 1;
			if(!typeCompare(temp1->type, temp2->type)) return 0;
			temp1 = temp1->tail;
			temp2 = temp2->tail;
		}
	}
}

void addArgTable(char* name) {
	Arg temp = (Arg) malloc(sizeof(struct Arg_));
	myStrcpy(&temp->name, name);
	temp->next = argTable;
	argTable = temp;
}

int addTable(FieldList *temp) {
	
	flag = 1;
	if(table[fieldLevel] == NULL) {
		FieldList node;
		for(int i = 0; i < fieldLevel; i++) {
			node = table[i];
			while(node != NULL) {
				if(strcmp(node->name, (*temp)->name) == 0 && (node->tag ||(*temp)->tag)) {
					flag = 0;
					break;
				}
				node = node->tail;
			}
		}
		if(strcmp((*temp)->name, "unknown") == 0) flag = 1;
		if(flag) {
			table[fieldLevel] = *temp;
			(*temp)->tail = NULL;
		}
	}
	else {
		FieldList node = table[fieldLevel];
		while(node != NULL){
			if(strcmp(node->name, (*temp)->name) == 0) {
				flag = 0;
				break;
			}
			node = node->tail;
		}
		
		for(int i = 0; i < fieldLevel; i++){
			node = table[i];
			while(node != NULL) {
				if(strcmp(node->name, (*temp)->name) == 0 && (node->tag || (*temp)->tag)) {
					flag = 0;
					break;
				}
				node = node->tail;
			}
		}

		if(strcmp((*temp)->name, "unknown") == 0) flag = 1;
		if (flag) {
			(*temp)->tail = table[fieldLevel];
			table[fieldLevel] = (*temp);
		}
	}
	return flag;
}

int paramCompare(FieldList p1, FieldList p2){
	while(1) {
		if((p1 == NULL && p2 != NULL) || (p1 != NULL && p2 == NULL))
			return 0;
		if(p1 == NULL && p2 == NULL) return 1;
		if(!typeCompare(p1->type, p2->type)) return 0;
		p1 = p1->tail;
		p2 = p2->tail;
	}
}

int addFuncTable(FuncList *temp) {
//return false when error happens;
	FuncList node = findFunc((*temp)->name);
	if(node == NULL) {
		(*temp)->tail = funcTable;
		funcTable = *temp;
		return 1;
	}
	else {
		if(paramCompare(node->paramList, (*temp)->paramList)) {
			if(!typeCompare(node->type, (*temp)->type)) return 0;
			//TODO: Do I need to keep the param name consistent?
			if(node->flag == 0 && (*temp)->flag == 1) {
				node->flag = 1;
			}
			return 1;
		}
		else return 0;
	}
}

void prepare(){
	Type intType = (Type)malloc(sizeof(struct Type_));
	intType->kind = BASIC;
	intType->u.basic = 0;

	//read
	FuncList newFunc = (FuncList)malloc(sizeof(struct FuncList_));
	newFunc->flag = 1;
	myStrcpy(&newFunc->name, "read");
	newFunc->type = intType;
	newFunc->paramList = NULL;
	newFunc->tail = NULL;
	addFuncTable(&newFunc);

	//write
	FuncList temp = (FuncList)malloc(sizeof(struct FuncList_));
	temp->flag = 1;
	myStrcpy(&temp->name, "write");
	temp->type = intType;
	FieldList param = (FieldList)malloc(sizeof(struct FieldList_));
	param->type = intType;
	param->tail = NULL;
	temp->paramList = param;
	temp->tail = NULL;
	addFuncTable(&temp);
}

Type findStruct(char* str, int level) {
	Type ans = NULL;
	FieldList node = table[level];
	while(node != NULL){
		//output(node);
		if(strcmp(node->name, str) == 0) {
			ans = node->type;
			break;
		}
		node = node->tail;
	}
	if (ans != NULL) return ans;
	else if(level != 0) ans = findStruct(str, level - 1);
	return ans;
}

Type findVar(char* str, int level) {
	Type ans = NULL;
	FieldList node = table[level];
	while(node != NULL) {
		if(strcmp(node->name, str) == 0 && node->tag == 0) {
			ans = node->type;
			break;
		}
		node = node->tail;
	}
	if (ans != NULL) return ans;
	else if(level != 0) ans = findVar(str, level - 1);
	return ans;
}

FuncList findFunc(char* str) {
	FuncList ans = NULL;
	FuncList node = funcTable;
	while(node != NULL) {
		if(strcmp(node->name, str) == 0) {
			ans = node;
			break;
		}
		node = node->tail;
	}
	return ans;
}

void semanticAnal(struct Node* node) {
//	printf("Node: %s\n", node->info);
	
	if(strcmp(node->info, "Program") == 0){
		semanticAnal(node->child[1]);
		FuncList temp = funcTable;
		while(temp != NULL) {
			if(temp->flag == 0) 
				printf("Error type 18 at Line %d: Undefined function \"%s\".\n", temp->line, temp->name);
			temp = temp->tail;
		}
	}
	else if(strcmp(node->info, "ExtDef") == 0) {
		semanticAnal(node->child[1]);//get type and store in t
		if(strcmp(node->child[2]->info, "ExtDecList") == 0) {
			//vardec
			node->child[2]->t = node->child[1]->t;
			semanticAnal(node->child[2]);
		}
		else if(strcmp(node->child[2]->info, "SEMI") == 0) {
			//pure struct
		}
		else {
			//function def
			if(strcmp(node->child[3]->info, "SEMI") == 0) {
				FuncList newFunc = (FuncList)malloc(sizeof(struct FuncList_));
				newFunc->line = node->line;
				newFunc->flag = 0;
				myStrcpy(&newFunc->name, node->child[2]->child[1]->type_string);
				newFunc->type = node->child[1]->t;	
				semanticAnal(node->child[2]);
				newFunc->paramList = node->child[2]->head;
				if(!addFuncTable(&newFunc)) 
					printf("Error type 19 at Line %d: Inconsistent declaration of function \"%s\".\n", node->line, newFunc->name);
			}
			else {
				FuncList temp = findFunc(node->child[2]->child[1]->type_string);
				if(temp != NULL && temp->flag) {
					printf("Error type 4 at Line %d: Redefined function \"%s\".\n", node->line, temp->name);
				}
				else {
					FuncList newFunc = (FuncList)malloc(sizeof(struct FuncList_));
					newFunc->line = node->line;
					newFunc->flag = 1;
					myStrcpy(&newFunc->name, node->child[2]->child[1]->type_string);
					newFunc->type = node->child[1]->t;
					semanticAnal(node->child[2]);
					newFunc->paramList = node->child[2]->head;
					if(!addFuncTable(&newFunc)) 
						printf("Error type 19 at Line %d: Inconsistent declaration of function \"%s\".\n", node->line, newFunc->name);
					else {
						fieldLevel++;
						FieldList temp = newFunc->paramList;
						//paramcopy;
						while(temp != NULL) {
							FieldList q = (FieldList)malloc(sizeof(struct FieldList_));
							myStrcpy(&q->name, temp->name);
							q->type = temp->type;
							if(q->type->kind != BASIC) addArgTable(q->name);
							q->line = temp->line;
							q->tail = NULL;
							q->tag = 0;
							if (!addTable(&q)) 
								printf("Error type 3 at Line %d: Redefined variable \"%s\".\n", node->line, q->name);
							temp = temp->tail;
						}
						fieldLevel--;
						//Compst
						node->child[3]->t = newFunc->type;
						semanticAnal(node->child[3]);
					}
				}
			}
		}
	}
	else if(strcmp(node->info, "ExtDecList") == 0) {
		if(node->num == 1) {
			node->child[1]->t = node->t;
			semanticAnal(node->child[1]);
			if (!addTable(&node->child[1]->head)) 
				printf("Error type 3 at Line %d: Redefined variable \"%s\".\n", node->line, node->child[1]->head->name);
		}
		else {
			node->child[1]->t = node->t;
			node->child[3]->t = node->t;
			semanticAnal(node->child[1]);
			if (!addTable(&node->child[1]->head))
				printf("Error type 3 at Line %d: Redefined variable \"%s\".\n", node->line, node->child[1]->head->name);
			semanticAnal(node->child[3]);
		}
	}
	else if (strcmp(node->info, "Specifier") == 0) {
		if(strcmp(node->child[1]->info, "TYPE") == 0) {
			Type temp = (Type)malloc(sizeof(struct Type_));
			temp->kind = BASIC;
			if(strcmp(node->child[1]->type_string, "int") == 0) 
				temp->u.basic = 0;	
			else temp->u.basic = 1;
			node->t = temp;
		}
		else {
			//struct Specifier
			semanticAnal(node->child[1]);
			node->t = node->child[1]->t;
		}
	}
	else if (strcmp(node->info, "StructSpecifier") == 0) {
		if(strcmp(node->child[2]->info, "Tag") == 0) {
			char* str = node->child[2]->child[1]->type_string;
			Type temp = findStruct(str, fieldLevel);
			if(temp == NULL) {
				printf("Error type 17 at Line %d: Undefined structure \"%s\".\n", node->line, str);	
				//assume it's INT to continue process
				Type t = (Type)malloc(sizeof(struct Type_));
				t->kind = BASIC;
				t->u.basic = 0;
				node->t = t;
			}
			else node->t = temp; 
		}
		else {
			//optTag full struct definition
			FieldList newStruct = (FieldList)malloc(sizeof(struct FieldList_));
			if(node->child[2]->child[1] != NULL) myStrcpy(&newStruct->name, node->child[2]->child[1]->type_string);
			else myStrcpy(&newStruct->name, "unknown");
			newStruct->tag = 1;
			Type t = (Type)malloc(sizeof(struct Type_));
			newStruct->type = t;
			t->kind = STRUCTURE;

			node->child[4]->f = 1;
			semanticAnal(node->child[4]);
			newStruct->type->u.structure = node->child[4]->head;

			int check = 1;
			FieldList temp = newStruct->type->u.structure;
			FieldList q;
			while(temp != NULL) {
				q = newStruct->type->u.structure;
				while(q != NULL) {
					if(q != temp && strcmp(q->name, temp->name) == 0) {
						check = 0;
						break;
					}
					q = q->tail;
				}
				if (!check) break;
				temp = temp->tail;
			}
			if(!check) printf("Error type 15 at Line %d: Redefined field \"%s\".\n", q->line, q->name);

			if(!addTable(&newStruct)) 
				printf("Error type 16 at Line %d: Duplicated name \"%s\".\n", node->line, newStruct->name);
			node->t = newStruct->type;
		}
	}
	else if(strcmp(node->info, "DefList") == 0) {
		if(node->f) {
			//struct deflist
			if(node->child[1] != NULL) {
				node->child[1]->f = 1;
				node->child[2]->f = 1;
				semanticAnal(node->child[1]);
				semanticAnal(node->child[2]);
				node->head = node->child[1]->head;
				node->child[1]->tail->tail = node->child[2]->head;
				node->tail = node->child[2]->tail;
			}
			else {
				node->head = NULL;
				node->tail = NULL;
			}
		}
		else{
			//just var dec need add to table
			if(node->child[1] != NULL) {
				semanticAnal(node->child[1]);
				semanticAnal(node->child[2]);
			}
		}
	}
	else if(strcmp(node->info, "Def") == 0) {
		semanticAnal(node->child[1]);
		node->child[2]->f = node->f;
		node->child[2]->t = node->child[1]->t;
		semanticAnal(node->child[2]);
		node->head = node->child[2]->head;
		node->tail = node->child[2]->tail;
	}
	else if(strcmp(node->info, "DecList") == 0) {
		node->child[1]->f = node->f;
		node->child[1]->t = node->t;
		if(node->num == 1) {
			semanticAnal(node->child[1]);
			if(node->f) {
				node->head = node->child[1]->head;
				node->tail = node->child[1]->tail;
			}
		}
		else {
			node->child[3]->t = node->t;
			node->child[3]->f = node->f;
			semanticAnal(node->child[1]);
			semanticAnal(node->child[3]);
			if(node->f) {
				node->head = node->child[1]->head;
				node->child[1]->tail->tail = node->child[3]->head;
				node->tail = node->child[3]->tail;
			}
		}
	}
	else if(strcmp(node->info, "Dec") == 0) {
		//TODO: consider ASSIGNOP
		node->child[1]->t = node->t;
		semanticAnal(node->child[1]);
		node->head = node->child[1]->head;
		node->tail = node->child[1]->tail;
		if(!node->f) {
			if(node->num > 1) {
				semanticAnal(node->child[3]);
				if(!typeCompare(node->child[1]->head->type, node->child[3]->t))
					printf("Error type 5 at Line %d: Type mismatched for assignment.\n", node->line);
			}
			if(!addTable(&node->child[1]->head)) 
				printf("Error type 3 at Line %d: Redefined variable \"%s\".\n", node->line, node->child[1]->head->name);
		}
		else {
			if(node->num > 1) printf("Error type 15 at Line %d: Initialize a field of structure \"%s\".\n", node->line, node->child[1]->head->name);
		}
	}
	else if(strcmp(node->info, "VarDec") == 0) {
		if(node->num == 1) {
			FieldList temp = (FieldList)malloc(sizeof(struct FieldList_));
			temp->tail = NULL;
			temp->tag = 0;
			temp->line = node->line;
			myStrcpy(&temp->name, node->child[1]->type_string);
			temp->type = node->t;
			node->head = temp;
			node->tail = temp;
		}
		else {
			node->child[1]->t = node->t;
			semanticAnal(node->child[1]);
			FieldList temp = node->child[1]->head;
			Type t = temp->type;
			if(t->kind != ARRAY) {
				Type p = (Type)malloc(sizeof(struct Type_));
				p->kind = ARRAY;
				p->u.array.elem = t;
				p->u.array.size = node->child[3]->type_int;
				temp->type = p;
			}
			else {
				while(t->kind == ARRAY && t->u.array.elem->kind == ARRAY){
					t = t->u.array.elem;
				}
				Type p = (Type)malloc(sizeof(struct Type_));
				p->kind = ARRAY;
				p->u.array.elem = t->u.array.elem;
				p->u.array.size = node->child[3]->type_int;
				t->u.array.elem = p;
			}

			node->head = temp;
			node->tail = temp;
		}
	}
	else if(strcmp(node->info, "FunDec") == 0) {
		if(node->num < 4) {
			node->head = NULL;
			node->tail = NULL;
		}
		else {
			semanticAnal(node->child[3]);
			node->head = node->child[3]->head;
			node->tail = node->child[3]->tail;
		}
	}
	else if(strcmp(node->info, "VarList") == 0) {
		if(node->num == 1) {
			semanticAnal(node->child[1]);
			node->head = node->child[1]->head;
			node->tail = node->child[1]->tail;
		}
		else {
			semanticAnal(node->child[1]);
			semanticAnal(node->child[3]);
			node->head = node->child[1]->head;
	//		if(node->child[1]->head == node->child[1]->tail) printf("Hello\n");
			node->child[1]->tail->tail = node->child[3]->head;
			node->tail = node->child[3]->tail;
		//	printf("%s     %s\n", node->head->name, node->head->tail->name);
		}
	}
	else if(strcmp(node->info, "ParamDec") == 0) {
		semanticAnal(node->child[1]);
		node->child[2]->t = node->child[1]->t;
		semanticAnal(node->child[2]);
		node->head = node->child[2]->head;
		node->tail = node->child[2]->tail;
	}
	else if(strcmp(node->info, "CompSt") == 0) {
		fieldLevel++;
		semanticAnal(node->child[2]);
		node->child[3]->t = node->t;
		semanticAnal(node->child[3]);
		table[fieldLevel] = NULL;
		fieldLevel--;
	}
	else if(strcmp(node->info, "StmtList") == 0) {
		if(node->child[1] != NULL){
			node->child[1]->t = node->t;
			node->child[2]->t = node->t;
			semanticAnal(node->child[1]);
			semanticAnal(node->child[2]);
		}	
	}
	else if(strcmp(node->info, "Stmt") == 0) {
		if(node->num == 1) {
		//CompSt
			node->child[1]->t = node->t;
			semanticAnal(node->child[1]);
		}
		else if(node->num == 2) {
		//Exp SEMI
			semanticAnal(node->child[1]);
		}
		else if(node->num == 3) {
			semanticAnal(node->child[2]);
			if(!typeCompare(node->t, node->child[2]->t)) 
				printf("Error type 8 at Line %d: Type mismatched for return.\n", node->line);
		}
		else if(node->num == 7) {
			semanticAnal(node->child[3]);
			node->child[5]->t = node->t;
			node->child[7]->t = node->t;
			semanticAnal(node->child[5]);
			semanticAnal(node->child[7]);
		}
		else{
			if(strcmp(node->child[1]->info, "IF") == 0) {
				semanticAnal(node->child[3]);
				node->child[5]->t = node->t;
				semanticAnal(node->child[5]);
			}
			else {
				semanticAnal(node->child[3]);
				node->child[5]->t = node->t;
				semanticAnal(node->child[5]);
			}
		}
	}
	else if(strcmp(node->info, "Exp") == 0) {
		//define exp->t through the process
		Type q = (Type)malloc(sizeof(struct Type_));
		q->kind = BASIC;
		q->u.basic = 0;
		node->t = q;
		if(node->num == 1) {
			if(strcmp(node->child[1]->info, "ID") == 0) {
				Type temp = findVar(node->child[1]->type_string, fieldLevel);
				if(temp == NULL) 
					printf("Error type 1 at Line %d: Undefined variable \"%s\".\n", node->line, node->child[1]->type_string);
				else node->t = temp;
			}
			else if(strcmp(node->child[1]->info, "INT") == 0) {
				Type temp = (Type)malloc(sizeof(struct Type_));
				temp->kind = BASIC;
				temp->u.basic = 0;
				node->t = temp;
			}
			else {
				Type temp = (Type)malloc(sizeof(struct Type_));
				temp->kind = BASIC;
				temp->u.basic = 1;
				node->t = temp;
			}
		}
		else if (strcmp(node->child[1]->info, "LP") == 0){
			semanticAnal(node->child[2]);
			node->t = node->child[2]->t;
		}
		else if (strcmp(node->child[1]->info, "MINUS") == 0) {
			semanticAnal(node->child[2]);
			node->t = node->child[2]->t;
		}
		else if (strcmp(node->child[1]->info, "NOT") == 0){
			semanticAnal(node->child[2]);
			node->t = node->child[2]->t;
		}
		else if(strcmp(node->child[1]->info, "Exp") == 0) {
			if(strcmp(node->child[2]->info, "LB") == 0) {
				semanticAnal(node->child[1]);
				if(node->child[1]->t->kind != ARRAY) 
					printf("Error type 10 at Line %d: is not an array.\n", node->line);
				
				semanticAnal(node->child[3]);
				if(node->child[3]->t->kind != BASIC || node->child[3]->t->u.basic != 0)
					printf("Error type 12 at Line %d: is not an integer.\n", node->line);

				node->t = node->child[1]->t->u.array.elem;
			}
			else if(strcmp(node->child[2]->info, "DOT") == 0) {
				semanticAnal(node->child[1]);
				if(node->child[1]->t->kind != STRUCTURE)
					printf("Error type 13 at Line %d: Illegal use of \".\".\n", node->line);
				else {
				
					FieldList temp = node->child[1]->t->u.structure;
					int check = 0;
					while(temp != NULL) {
						if(strcmp(temp->name, node->child[3]->type_string) == 0) {
							check = 1;
							break;
						}
						temp = temp->tail;
					}
					//TODO:error return type modification
					if(!check) printf("Error type 14 at Line %d: Non-existent field \"%s\".\n", node->line, node->child[3]->type_string);
						else node->t = temp->type;  
					}
			}
			else if(strcmp(node->child[2]->info, "ASSIGNOP") == 0){
				int check = 0;
				struct Node* temp = node->child[1];
				if(temp->num == 1 && strcmp(temp->child[1]->info, "ID") == 0) check = 1;
				if(temp->num == 3 && strcmp(temp->child[1]->info, "Exp") == 0 &&
					strcmp(temp->child[2]->info, "DOT") == 0 &&
					strcmp(temp->child[3]->info, "ID") == 0) check = 1;
				if(temp->num == 4 && strcmp(temp->child[1]->info, "Exp") == 0 &&
					strcmp(temp->child[2]->info, "LB") == 0 && 
					strcmp(temp->child[3]->info, "Exp") == 0 &&
					strcmp(temp->child[4]->info, "RB") == 0) check = 1;
				if(!check) 
					printf("Error type 6 at Line %d: The left-hand side of an assignment must be a variable.\n", node->line);
				
				semanticAnal(node->child[1]);
				semanticAnal(node->child[3]);
				node->t = node->child[1]->t;
				if(!typeCompare(node->child[1]->t, node->child[3]->t))
					printf("Error type 5 at Line %d: Type mismatched for assignment.\n", node->line);
			}
			else {
				//A and B
				semanticAnal(node->child[1]);
				semanticAnal(node->child[3]);
				node->t = node->child[1]->t;
				if(!typeCompare(node->child[1]->t, node->child[3]->t))
					printf("Error type 7 at Line %d: Type mismatched for operands.\n", node->line);
			}
		}
		else {
			//function
			FuncList temp = findFunc(node->child[1]->type_string);
			if(temp == NULL) {
				Type temp1 = findStruct(node->child[1]->type_string, fieldLevel);
				if(temp1 != NULL) printf("Error type 11 at Line %d: \"%s\" is not a function.\n", node->line, node->child[1]->type_string);
					else printf("Error type 2 at Line %d: Undefined function \"%s\".\n", node->line, node->child[1]->type_string);
			}
			else {
				FieldList head;
				head = NULL;
				if(node->num > 3) {
					semanticAnal(node->child[3]);
					head = node->child[3]->head;
				}
				//compare args
				if(!paramCompare(head, temp->paramList)) 
					printf("Error type 9 at Line %d: Function not applicable for arguments.\n", node->line);
				node->t = temp->type;
			}
		}
	}
	else if(strcmp(node->info, "Args") == 0) {
		if(node->num == 1) {
			semanticAnal(node->child[1]);
			FieldList temp = (FieldList)malloc(sizeof(struct FieldList_));
			temp->type = node->child[1]->t;
			node->head = temp;
			node->tail = temp;
		}
		else {
			semanticAnal(node->child[1]);
			FieldList temp = (FieldList)malloc(sizeof(struct FieldList_));
			temp->type = node->child[1]->t;
			semanticAnal(node->child[3]);
			node->head = temp;
			temp->tail = node->child[3]->head;
			node->tail = node->child[3]->tail;
		}
	}
	else {
		for(int i = 1; i <= node->num; i++)
			if(node->child[i] != NULL) semanticAnal(node->child[i]);
	}
}
