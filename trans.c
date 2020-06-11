#include "tree.h"
#include "trans.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char* newTemp() {
	tempNum++;
	char str1[20] = "t";
	char str2[10];
	sprintf(str2, "%d", tempNum);
	strcat(str1, str2);
	char* t;
	myStrcpy(&t, str1);
	//addOffTable(t, 4);
	return t;
}

char* newLabel() {
	labelNum++;
	char str1[25] = "label";
	char str2[10];
	sprintf(str2, "%d", labelNum);
	strcat(str1, str2);
	char* t;
	myStrcpy(&t, str1);
	return t;
}

struct codeList codePatch(InterCodes a, InterCodes* b, InterCodes* c, InterCodes d) {
	//TODO:watch out for NULL patch
	if(a == NULL || (*b) == NULL) {
		struct codeList ans;
		ans.head = *c;
		ans.tail = d;
		return ans;
	}
	if((*c) == NULL || d == NULL) {
		struct codeList ans;
		ans.head = a;
		ans.tail = *b;
		return ans;
	}
	struct codeList ans;
	(*b)->next = *c;
    (*c)->prev = *b;
	ans.head = a;
	ans.tail = d;
	return ans;
}

void createVar(Operand* t, char* name) {
	(*t) = (Operand)malloc(sizeof(struct Operand_));
	(*t)->kind = VARIABLE;
	myStrcpy(&(*t)->u.val_na, name);
}

void createConstant(Operand* t, int q) {
	(*t) = (Operand)malloc(sizeof(struct Operand_));
	(*t)->kind = CONSTANT;
	(*t)->u.value = q;
}

void createLocation(Operand* t, char* name) {
	(*t) = (Operand)malloc(sizeof(struct Operand_));
	(*t)->kind = LOCATION;
	myStrcpy(&(*t)->u.val_na, name);
}

void createAddress(Operand* t, char* name) {
	(*t) = (Operand)malloc(sizeof(struct Operand_));
	(*t)->kind = ADDRESS;
	myStrcpy(&(*t)->u.val_na, name);
}

struct codeList createLabel(char* name) {
	InterCodes p = (InterCodes)malloc(sizeof(struct InterCodes_));
	p->code.kind = LABEL;
	p->code.u.label.pos = (Operand)malloc(sizeof(struct Operand_));
	p->code.u.label.pos->kind = VARIABLE;
	myStrcpy(&p->code.u.label.pos->u.val_na, name);
	p->prev = p->next = NULL;
	struct codeList ans;
	ans.head = ans.tail = p;
	return ans;
}

struct codeList createGoto(char* name) {
	InterCodes p = (InterCodes)malloc(sizeof(struct InterCodes_));
	p->code.kind = GOTO;
	createVar(&p->code.u.go.pos, name);
	p->prev = p->next = NULL;
	struct codeList ans;
	ans.head = ans.tail = p;
	return ans;
}

struct codeList translate_Exp(struct Node* node, char* place) {
	struct codeList ans;
	if (!strcmp(node->child[1]->info, "LP")) {
		return translate_Exp(node->child[2], place);
	}
	else if(!strcmp(node->child[1]->info, "INT")) {
		InterCodes p = (InterCodes)malloc(sizeof(struct InterCodes_));
		p->code.kind = ASSIGN;
		char* t1 = newTemp();
		if(place == NULL) myStrcpy(&place, t1);
		createVar(&p->code.u.assign.left, place);
		createConstant(&p->code.u.assign.right, node->child[1]->type_int);
		p->prev = p->next = NULL;

		ans.head = ans.tail = p;
	}
	else if(!strcmp(node->child[1]->info, "ID") && node->num == 1) {
		InterCodes p = (InterCodes)malloc(sizeof(struct InterCodes_));
		p->code.kind = ASSIGN;
		char* t1 = newTemp();
		if(place == NULL) myStrcpy(&place, t1);
		createVar(&p->code.u.assign.left, place);
		createVar(&p->code.u.assign.right, node->child[1]->type_string);
		p->prev = p->next = NULL;

		ans.head = ans.tail = p;
	}
	else if(!strcmp(node->child[1]->info, "MINUS")) {
		//printf("Hello\n");
		//printf("1\n");
		char* t1 = newTemp();
		struct codeList code1, code2;
		code1 = translate_Exp(node->child[2], t1);
	
		InterCodes p = (InterCodes)malloc(sizeof(struct InterCodes_));
		p->code.kind = SUB;
		char* t2 = newTemp();
		if(place == NULL) myStrcpy(&place, t2);
		createVar(&p->code.u.binop.result, place);
		createConstant(&p->code.u.binop.op1, 0);
		createVar(&p->code.u.binop.op2, t1);
		p->prev = p->next = NULL;
		
		code2.head = code2.tail = p;
		code1 = codePatch(code1.head, &code1.tail, &code2.head, code2.tail);
		ans = code1;
		
	}
	else if(!strcmp(node->child[2]->info, "ASSIGNOP")) {
		if(!strcmp(node->child[1]->child[1]->info, "ID")) {
			char* t1 = newTemp();
			struct codeList code1, code2, code3;
			code1 = translate_Exp(node->child[3], t1);
			
			InterCodes p1 = (InterCodes)malloc(sizeof(struct InterCodes_));
			p1->code.kind = ASSIGN;
			createVar(&p1->code.u.assign.left, node->child[1]->child[1]->type_string);
			createVar(&p1->code.u.assign.right, t1);
			p1->prev = p1->next = NULL;
			code2.head = code2.tail = p1;

			if(place != NULL) {
				InterCodes p2 = (InterCodes)malloc(sizeof(struct InterCodes_));
				p2->code.kind = ASSIGN;
				createVar(&p2->code.u.assign.left, place);
				createVar(&p2->code.u.assign.right, node->child[1]->child[1]->type_string);
				p2->prev = p2->next = NULL;
				code3.head = code3.tail = p2;
				code2 = codePatch(code2.head, &code2.tail, &code3.head, code3.tail);
			}
			code1 = codePatch(code1.head, &code1.tail, &code2.head, code2.tail);
			ans = code1;
		}
		else {
			char* t1 = newTemp();
			char* t2 = newTemp();
			struct codeList code1, code2, code3, code4;
			code1 = translate_Location(node->child[1], t1);
			code2 = translate_Exp(node->child[3], t2);
			
			InterCodes p1 = (InterCodes)malloc(sizeof(struct InterCodes_));
			p1->code.kind = ASSIGN;
			createLocation(&p1->code.u.assign.left, t1);
			createVar(&p1->code.u.assign.right, t2);
			p1->prev = p1->next = NULL;
			code3.head = code3.tail = p1;

			if(place != NULL) {
				InterCodes p2 = (InterCodes)malloc(sizeof(struct InterCodes_));
				p2->code.kind = ASSIGN;
				createVar(&p2->code.u.assign.left, place);
				createLocation(&p2->code.u.assign.right, t1);
				p2->prev = p2->next = NULL;
				code4.head = code4.tail = p2;
				code3 = codePatch(code3.head, &code3.tail, &code4.head, code4.tail);
			}

			code1 = codePatch(code1.head, &code1.tail, &code2.head, code2.tail);
			code1 = codePatch(code1.head, &code1.tail, &code3.head, code3.tail);
			ans = code1;
		}
	}
	else if(!strcmp(node->child[2]->info, "PLUS") || !strcmp(node->child[2]->info, "MINUS") 
		|| !strcmp(node->child[2]->info, "STAR") || !strcmp(node->child[2]->info, "DIV")) {
		char* t1 = newTemp();
		char* t2 = newTemp();
		struct codeList code1, code2, code3;
		code1 = translate_Exp(node->child[1], t1);
		code2 = translate_Exp(node->child[3], t2);	
		if(place != NULL) {
			InterCodes p = (InterCodes)malloc(sizeof(struct InterCodes_));
			if(!strcmp(node->child[2]->info, "PLUS")) p->code.kind = ADD;
			if(!strcmp(node->child[2]->info, "MINUS")) p->code.kind = SUB;
			if(!strcmp(node->child[2]->info, "STAR")) {
				p->code.kind = MUL;
			}
			if(!strcmp(node->child[2]->info, "DIV")) p->code.kind = DIVI;
			createVar(&p->code.u.binop.result, place);
			createVar(&p->code.u.binop.op1, t1);
			createVar(&p->code.u.binop.op2, t2);
			p->prev = p->next = NULL;
			code3.head = code3.tail = p;
			code2 = codePatch(code2.head, &code2.tail, &code3.head, code3.tail);
		}
		code1 = codePatch(code1.head, &code1.tail, &code2.head, code2.tail);
		ans = code1;
	}
	else if(!strcmp(node->child[1]->info, "ID") && node->num == 3) {
		InterCodes p = (InterCodes)malloc(sizeof(struct InterCodes_));
		if(!strcmp(node->child[1]->type_string, "read")) {
			p->code.kind = READ;
			char* t1 = newTemp();
			if(place == NULL) myStrcpy(&place, t1);
			createVar(&p->code.u.read.name, place);
		}
		else {
			p->code.kind = CALL;
			char* t1 = newTemp();
			if(place == NULL) myStrcpy(&place, t1);
			createVar(&p->code.u.call.op1, place);
			createVar(&p->code.u.call.name, node->child[1]->type_string);
		}
		p->prev = p->next = NULL;
		ans.head = ans.tail = p;
	}
	else if(!strcmp(node->child[1]->info, "ID") && node->num == 4) {
		struct argStr* argList;
		argList = NULL;
		struct codeList code1;
		code1 = translate_Args(node->child[3], &argList);
		if(!strcmp(node->child[1]->type_string, "write")) {
			struct codeList code2, code3;
			InterCodes p1 = (InterCodes)malloc(sizeof(struct InterCodes_));
			p1->code.kind = WRITE;
			createVar(&p1->code.u.write.name, argList->str);
			p1->prev = p1->next = NULL;
			code2.head = code2.tail = p1;
			
			code1 = codePatch(code1.head, &code1.tail, &code2.head, code2.tail);
			//TODO:add place := #0
		}
		else {
			struct codeList code2, code3, code4;
			code2.head = code2.tail = NULL;
			struct argStr* temp = argList;
			while(temp != NULL) {
				InterCodes p = (InterCodes)malloc(sizeof(struct InterCodes_));
				p->code.kind = ARG;
				createVar(&p->code.u.arg.name, temp->str);
				p->prev = p->next = NULL;
				struct codeList tempc;
				tempc.head = tempc.tail = p;
				if(code2.head == NULL) code2 = tempc;
					else code2 = codePatch(code2.head, &code2.tail, &tempc.head, tempc.tail);
				temp = temp->next;
			}
		
			char* t1 = newTemp();
			if(place == NULL) myStrcpy(&place, t1);
			InterCodes p1 = (InterCodes)malloc(sizeof(struct InterCodes_));
			p1->code.kind = CALL;
			createVar(&p1->code.u.call.op1, place);
			createVar(&p1->code.u.call.name, node->child[1]->type_string);
			p1->prev = p1->next = NULL;
			code3.head = code3.tail = p1;
			
			code1 = codePatch(code1.head, &code1.tail, &code2.head, code2.tail);
			code1 = codePatch(code1.head, &code1.tail, &code3.head, code3.tail);
		}
		ans = code1;
	}
	else if(!strcmp(node->child[2]->info, "LB") || !strcmp(node->child[2]->info, "DOT")) {
		char* t1 = newTemp();
		struct codeList code1, code2;
		code1 = translate_Location(node, t1);
		
		InterCodes p = (InterCodes)malloc(sizeof(struct InterCodes_));
		p->code.kind = ASSIGN;
		
		char* t2 = newTemp();
		if(place == NULL) myStrcpy(&place, t2);
		
		createVar(&p->code.u.assign.left, place);
		createLocation(&p->code.u.assign.right, t1);
		p->prev = p->next = NULL;
		code2.head = code2.tail = p;

		code1 = codePatch(code1.head, &code1.tail, &code2.head, code2.tail);
		ans = code1;
	}
	else {
		
		char* label1 = newLabel();
		char* label2 = newLabel();
		struct codeList code0, code1, code2, code3, code4;

		InterCodes p1 = (InterCodes)malloc(sizeof(struct InterCodes_));
		p1->code.kind = ASSIGN;
		
		char* t1 = newTemp();
		if(place == NULL) myStrcpy(&place, t1);

		createVar(&p1->code.u.assign.left, place);
		createConstant(&p1->code.u.assign.right, 0);
		p1->prev = p1->next = NULL;
		code0.head = code0.tail = p1;
		code1 = translate_Cond(node, label1, label2);
		code2 = createLabel(label1);

		InterCodes p3 = (InterCodes)malloc(sizeof(struct InterCodes_));
		p3->code.kind = ASSIGN;

		createVar(&p3->code.u.assign.left, place);
		createConstant(&p3->code.u.assign.right, 1);
		p3->prev = p3->next = NULL;
		code3.head = code3.tail = p3;
		
		code4 = createLabel(label2);

		code0 = codePatch(code0.head, &code0.tail, &code1.head, code1.tail);
		code0 = codePatch(code0.head, &code0.tail, &code2.head, code2.tail);
		code0 = codePatch(code0.head, &code0.tail, &code3.head, code3.tail);
		code0 = codePatch(code0.head, &code0.tail, &code4.head, code4.tail);
		ans = code0;
	}
	return ans;
}

int findArg(char* name) {
	int flag = 0;
	Arg temp = argTable;
	while(temp != NULL) {
		if(!strcmp(name, temp->name)) {
			flag = 1;
			break;
		}
		temp = temp->next;
	}
	return flag;
}

struct codeList translate_Location(struct Node* node, char* place) {
	struct codeList ans;
	if(!strcmp(node->child[1]->info, "ID")) {
		InterCodes p = (InterCodes)malloc(sizeof(struct InterCodes_));
		p->code.kind = ASSIGN;
		createVar(&p->code.u.assign.left, place);
		if(findArg(node->child[1]->type_string)) createVar(&p->code.u.assign.right, node->child[1]->type_string);
			else createAddress(&p->code.u.assign.right, node->child[1]->type_string);
		p->prev = p->next = NULL;
		
		struct codeList code1;
		code1.head = code1.tail = p;
		ans = code1;
	}
	else if(!strcmp(node->child[2]->info, "LB")) {
		struct codeList code1, code2, code3, code4;
		char* t1 = newTemp();
		code1 = translate_Location(node->child[1], t1);
		char* t2 = newTemp();
		code2 = translate_Exp(node->child[3], t2);
		
		int temp = getSize(node->t);
		char* t3 = newTemp();
		InterCodes p1 = (InterCodes)malloc(sizeof(struct InterCodes_));
		p1->code.kind = MUL;
		createVar(&p1->code.u.binop.result, t3);
		createVar(&p1->code.u.binop.op1, t2);
		createConstant(&p1->code.u.binop.op2, temp);
		p1->prev = p1->next = NULL;
		code3.head = code3.tail = p1;
	
		InterCodes p2 = (InterCodes)malloc(sizeof(struct InterCodes_));
		p2->code.kind = ADD;
		createVar(&p2->code.u.binop.result, place);
		createVar(&p2->code.u.binop.op1, t1);
		createVar(&p2->code.u.binop.op2, t3);
		p2->prev = p2->next = NULL;
		code4.head = code4.tail = p2;
		
		code1 = codePatch(code1.head, &code1.tail, &code2.head, code2.tail);
		code1 = codePatch(code1.head, &code1.tail, &code3.head, code3.tail);
		code1 = codePatch(code1.head, &code1.tail, &code4.head, code4.tail);
		ans = code1;
	}
	else {
		struct codeList code1, code2;
		char* t1 = newTemp();
		code1 = translate_Location(node->child[1], t1);

		int size = 0;
		FieldList temp = node->child[1]->t->u.structure;
		while(temp != NULL) {
			if(!strcmp(temp->name, node->child[3]->type_string)) break;
			size = size + getSize(temp->type);
			temp = temp->tail;
		}

		InterCodes p = (InterCodes)malloc(sizeof(struct InterCodes_));
		p->code.kind = ADD;
		createVar(&p->code.u.binop.result, place);
		createVar(&p->code.u.binop.op1, t1);
		createConstant(&p->code.u.binop.op2, size);
		p->prev = p->next = NULL;
		code2.head = code2.tail = p;

		code1 = codePatch(code1.head, &code1.tail, &code2.head, code2.tail);
		ans = code1;
	}
	return ans;
}

struct codeList translate_Args(struct Node* node, struct argStr** argList) {
	struct codeList ans;
	char* t1 = newTemp();
	struct codeList code1;
	if(node->child[1]->t->kind == BASIC) {
		code1 = translate_Exp(node->child[1], t1);
	}
	else {
		code1 = translate_Location(node->child[1], t1);
	}
	
	struct argStr* temp = (struct argStr*)malloc(sizeof(struct argStr));
	myStrcpy(&temp->str, t1);
	temp->next = (*argList);
	(*argList) = temp;
		
	if(node->num == 1) {
		ans = code1;
	}
	else {
		struct codeList code2;
		code2 = translate_Args(node->child[3], argList);
		code1 = codePatch(code1.head, &code1.tail, &code2.head, code2.tail);
		ans = code1;
	}
	return ans;
}

struct codeList translate_Cond(struct Node* node, char* t, char* f) {
	struct codeList ans;
	if(node->num > 1 && !strcmp(node->child[2]->info, "RELOP")) {
		char* t1 = newTemp();
		char* t2 = newTemp();
		struct codeList code1, code2;
		code1 = translate_Exp(node->child[1], t1);
		code2 = translate_Exp(node->child[3], t2);
		InterCodes p = (InterCodes)malloc(sizeof(struct InterCodes_));
		p->code.kind = CON;
		createVar(&p->code.u.con.op1, t1);
		createVar(&p->code.u.con.op2, node->child[2]->type_string);
		createVar(&p->code.u.con.op3, t2);
		createVar(&p->code.u.con.pos, t);
		p->prev = p->next = NULL;
		struct codeList code3, code4;
		code3.head = code3.tail = p;
		
		code4 = createGoto(f);

		code1 = codePatch(code1.head, &code1.tail, &code2.head, code2.tail);
		code1 = codePatch(code1.head, &code1.tail, &code3.head, code3.tail);
		code1 = codePatch(code1.head, &code1.tail, &code4.head, code4.tail);
		ans = code1;
	}
	else if(node->num > 1 && !strcmp(node->child[1]->info, "LP")) {
		return translate_Cond(node->child[2], t, f);
	}
	else if(node->num == 2 && !strcmp(node->child[1]->info, "NOT")) {
		return translate_Cond(node->child[2], f, t);	
	}
	else if(node->num > 1 && !strcmp(node->child[2]->info, "AND")) {
		char* label1 = newLabel();
		struct codeList code1, code2;
		code1 = translate_Cond(node->child[1], label1, f);
		code2 = translate_Cond(node->child[3], t, f);

		struct codeList code;
		code = createLabel(label1);

		code1 = codePatch(code1.head, &code1.tail, &code.head, code.tail);
		code1 = codePatch(code1.head, &code1.tail, &code2.head, code2.tail);
		ans = code1;
	}
	else if(node->num > 1 && !strcmp(node->child[2]->info, "OR")) {
		char* label1 = newLabel();
		struct codeList code1, code2;
		code1 = translate_Cond(node->child[1], t, label1);
		code2 = translate_Cond(node->child[3], t, f);
		
		struct codeList code;
		code = createLabel(label1);
		
		code1 = codePatch(code1.head, &code1.tail, &code.head, code.tail);
		code1 = codePatch(code1.head, &code1.tail, &code2.head, code2.tail);
		ans = code1;
	}
	else {
		char* t1 = newTemp();
		struct codeList code1, code2, code3;
		code1 = translate_Exp(node, t1);
		
		InterCodes p = (InterCodes)malloc(sizeof(struct InterCodes_));
		p->code.kind = CON;
		createVar(&p->code.u.con.op1, t1);
		createVar(&p->code.u.con.op2, "!=");
		createConstant(&p->code.u.con.op3, 0);
		createVar(&p->code.u.con.pos, t);
		p->prev = p->next = NULL;
		code2.head = code2.tail = p;
		
		code3 = createGoto(f);

		code1 = codePatch(code1.head, &code1.tail, &code2.head, code2.tail);
		code1 = codePatch(code1.head, &code1.tail, &code3.head, code3.tail);
		ans = code1;
	}
	return ans;
}

struct codeList translate_Stmt(struct Node* node) {
//printf("    %s\n", node->info);
	struct codeList ans;
	if(!strcmp(node->child[1]->info, "Exp")) {
		return translate_Exp(node->child[1], NULL);
	}
	else if(!strcmp(node->child[1]->info, "CompSt")) {
		return translate_CompSt(node->child[1]);
	}
	else if(!strcmp(node->child[1]->info, "RETURN")) {
		char* t1 = newTemp();
		struct codeList code1, code2;
		code1 = translate_Exp(node->child[2], t1);
		
		InterCodes p = (InterCodes)malloc(sizeof(struct InterCodes_));
		p->code.kind = BACK;
		createVar(&p->code.u.back.op1, t1);
		p->prev = p->next = NULL;
		code2.head = code2.tail = p;

		code1 = codePatch(code1.head, &code1.tail, &code2.head, code2.tail);
		ans = code1;
	}
	else if(!strcmp(node->child[1]->info, "WHILE")) {
		char* label1 = newLabel();
		char* label2 = newLabel();
		char* label3 = newLabel();
		struct codeList code1, code2, code3, code4, code5, code6;
		code1 = translate_Cond(node->child[3], label2, label3);
		code2 = translate_Stmt(node->child[5]);
		code3 = createLabel(label1);
		code4 = createLabel(label2);
		
		code5 = createGoto(label1);

		code6 = createLabel(label3);
		code1 = codePatch(code3.head, &code3.tail, &code1.head, code1.tail);
		code1 = codePatch(code1.head, &code1.tail, &code4.head, code4.tail);
		code1 = codePatch(code1.head, &code1.tail, &code2.head, code2.tail);
		code1 = codePatch(code1.head, &code1.tail, &code5.head, code5.tail);
		code1 = codePatch(code1.head, &code1.tail, &code6.head, code6.tail);
		ans = code1;
	}
	else if(node->num < 7) {
		char* label1 = newLabel();
		char* label2 = newLabel();
		struct codeList code1, code2, code3, code4;
		code1 = translate_Cond(node->child[3], label1, label2);
		code2 = translate_Stmt(node->child[5]);
		code3 = createLabel(label1);
		code4 = createLabel(label2);

		code1 = codePatch(code1.head, &code1.tail, &code3.head, code3.tail);
		code1 = codePatch(code1.head, &code1.tail, &code2.head, code2.tail);
		code1 = codePatch(code1.head, &code1.tail, &code4.head, code4.tail);
		ans = code1;
	}
	else {
		char* label1 = newLabel();
		char* label2 = newLabel();
		char* label3 = newLabel();
		struct codeList code1, code2, code3, code4, code5, code6, code7;
		code1 = translate_Cond(node->child[3], label1, label2);
		code2 = translate_Stmt(node->child[5]);
		code3 = translate_Stmt(node->child[7]);
		code4 = createLabel(label1);
		code5 = createGoto(label3);
		code6 = createLabel(label2);
		code7 = createLabel(label3);
		
		code1 = codePatch(code1.head, &code1.tail, &code4.head, code4.tail);
		code1 = codePatch(code1.head, &code1.tail, &code2.head, code2.tail);
		code1 = codePatch(code1.head, &code1.tail, &code5.head, code5.tail);
		code1 = codePatch(code1.head, &code1.tail, &code6.head, code6.tail);
		code3 = codePatch(code3.head, &code3.tail, &code7.head, code7.tail);
		code1 = codePatch(code1.head, &code1.tail, &code3.head, code3.tail);
		ans = code1;
	}
	return ans;
}

struct codeList translate_CompSt(struct Node* node) {
//TODO: consider def
//	return translate(node->child[3]);
	return translate(node);
}

struct codeList translate_FunDec(struct Node* node) {
	offTemp = 0;
	paramTemp = 0;
	/*sizeTemp = (struct offset*)malloc(sizeof(struct offset));
	myStrcpy(&sizeTemp->str, node->child[1]->type_string);
	sizeTemp->t = 0;
	sizeTemp->next = sizeTable;
	sizeTable = sizeTemp;*/
	
	struct codeList ans;
	InterCodes p = (InterCodes)malloc(sizeof(struct InterCodes_));
	p->code.kind = FUNC;
	createVar(&p->code.u.func.name, node->child[1]->type_string);
	p->prev = p->next = NULL;
	
	struct codeList code1;
	code1.head = code1.tail = p;
	
	if(node->num > 3) {
		struct codeList code2;
		code2 = translate_VarList(node->child[3]);
		code1 = codePatch(code1.head, &code1.tail, &code2.head, code2.tail);
	}
	ans = code1;

	return ans;
}

struct codeList translate_VarList(struct Node* node) {
	struct codeList ans;

	struct codeList code1;
	code1 = translate_ParamDec(node->child[1]);
	if(node->num > 1) {
		struct codeList code2;
		code2 = translate_VarList(node->child[3]);
		code1 = codePatch(code1.head, &code1.tail, &code2.head, code2.tail);
	}
	ans = code1;
	return ans;
}

void addParamTable(char* str) {
	struct offset* temp = (struct offset*)malloc(sizeof(struct offset));
	myStrcpy(&temp->str, str);
	temp->t = paramTemp;
	paramTemp = paramTemp + 4;
	temp->next = paramTable;
	paramTable = temp;
}

struct codeList translate_ParamDec(struct Node* node) {
	char* str = translate_VarDec(node->child[2]);
	InterCodes p = (InterCodes)malloc(sizeof(struct InterCodes_));
	p->code.kind = PARAM;
	createVar(&p->code.u.param.name, str);
	p->prev = p->next = NULL;

	//addParamTable(str);
	struct codeList ans;
	ans.head = ans.tail = p;
	return ans;
}

char* translate_VarDec(struct Node* node) {
	if(node->num == 1) return node->child[1]->type_string;
	else return translate_VarDec(node->child[1]);
}

int getSize(Type t) {
	int s;
	if(t->kind == BASIC) return 4;
	if(t->kind == ARRAY) {
		s = 1;
		Type temp = t;
		while(temp->kind == ARRAY) {
			s = s * temp->u.array.size;
			temp = temp->u.array.elem;
		}
		s = s * getSize(temp);
	}
	else {
		s = 0;
		FieldList temp = t->u.structure;
		while(temp != NULL) {
			s = s + getSize(temp->type);
			temp = temp->tail;
		}
	}
	return s;
}

void addOffTable(char* str, int size) {
	struct offset *temp = (struct offset*)malloc(sizeof(struct offset));
	myStrcpy(&temp->str, str);
	temp->t = offTemp;
	offTemp = offTemp + size;
	//sizeTemp->t = offTemp;
	temp->next = offTable;
	offTable = temp;
}

struct codeList translate_Dec(struct Node* node) {
	struct codeList ans, code1, code2, code3;
	code1.head = code1.tail = NULL;
	char* str = translate_VarDec(node->child[1]);
	//TODO: add vairable type here	
	int L = 4;
	if(node->child[1]->head->type->kind != BASIC) {
		L = getSize(node->child[1]->head->type);
		InterCodes p1 = (InterCodes)malloc(sizeof(struct InterCodes_));
		p1->code.kind = DEC;
		createVar(&p1->code.u.dec.name, str);
		createConstant(&p1->code.u.dec.size, L);
		p1->prev = p1->next = NULL;
		code1.head = code1.tail = p1;
	}
	
	//addOffTable(str, L);

	if(node->num == 3) {
		char* t1 = newTemp();
		code2 = translate_Exp(node->child[3], t1);
		
		InterCodes p = (InterCodes)malloc(sizeof(struct InterCodes_));
		p->code.kind = ASSIGN;
		createVar(&p->code.u.assign.left, str);
		createVar(&p->code.u.assign.right, t1);
		p->prev = p->next = NULL;
		struct codeList code3;
		code3.head = code3.tail = p;
		
		if(code1.head == NULL) code1 = code2;
		else code1 = codePatch(code1.head, &code1.tail, &code2.head, code2.tail);
		code1 = codePatch(code1.head, &code1.tail, &code3.head, code3.tail);
	}
	ans = code1;
	return ans;
}

struct codeList translate(struct Node* node){
//	printf("%s\n", node->info);
	struct codeList ans;
	ans.head = ans.tail = NULL;
	if(!strcmp(node->info, "Exp")) {
		ans = translate_Exp(node, NULL);
	}
	else if(!strcmp(node->info, "Stmt")) {
		ans = translate_Stmt(node);
	}
	else if(!strcmp(node->info, "FunDec")) {
		ans = translate_FunDec(node);
	}
	else if(!strcmp(node->info, "Dec")) {
		ans = translate_Dec(node);
	}
	else {
		for(int i = 1; i <= node->num; i++) {
			if(node->child[i] != NULL) {
				struct codeList tempAns = translate(node->child[i]);
				if(ans.head == NULL) ans = tempAns;
				else if(tempAns.head != NULL) {
					//let the link glue together
					ans.tail->next = tempAns.head;
					tempAns.head->prev = ans.tail;
					ans.tail = tempAns.tail;
				}
			}
		}
	}
	return ans;
}

void printOp(Operand p) {
	if(p->kind == VARIABLE) printf("%s", p->u.val_na);
	else if(p->kind == CONSTANT) printf("#%d", p->u.value);
	else if(p->kind == ADDRESS) printf("&%s", p->u.val_na);
	else if(p->kind == LOCATION) printf("*%s", p->u.val_na);
}

void printCode(InterCodes node) {
	//freopen("workdir/a.ir", "w", stdout);
	InterCodes loop;
	loop = node;
	int f = 0;
	while(loop != NULL) {
		if(loop->code.kind == ASSIGN) {
			Operand tleft = loop->code.u.assign.left;
			Operand tright = loop->code.u.assign.right;
			printOp(tleft);
			printf(" := ");
			printOp(tright);
		}
		else if(loop->code.kind == ADD || loop->code.kind == SUB || loop->code.kind == MUL || loop->code.kind == DIVI) {
			printOp(loop->code.u.binop.result);
			printf(" := ");
			printOp(loop->code.u.binop.op1);
			if(loop->code.kind == ADD) printf(" + ");
			if(loop->code.kind == SUB) printf(" - ");
			if(loop->code.kind == MUL) printf(" * ");
			if(loop->code.kind == DIVI) printf(" / ");
			printOp(loop->code.u.binop.op2);
		}
		else if(loop->code.kind == LABEL) {
			printf("LABEL ");
			printOp(loop->code.u.label.pos);
			printf(" :");
		}
		else if(loop->code.kind == CON) {
			printf("IF ");
			printOp(loop->code.u.con.op1);
			printf(" ");
			printOp(loop->code.u.con.op2);
			printf(" ");
			printOp(loop->code.u.con.op3);
			printf(" GOTO ");
			printOp(loop->code.u.con.pos);
		}
		else if(loop->code.kind == GOTO) {
			printf("GOTO ");
			printOp(loop->code.u.go.pos);
		}
		else if(loop->code.kind == BACK) {
			printf("RETURN ");
			printOp(loop->code.u.back.op1);
		}
		else if(loop->code.kind == READ) {
			printf("READ ");
			printOp(loop->code.u.read.name);
		}
		else if(loop->code.kind == WRITE) {
			printf("WRITE ");
			printOp(loop->code.u.write.name);
		}
		else if(loop->code.kind == CALL) {
			printOp(loop->code.u.call.op1);
			printf(" := CALL ");	
			printOp(loop->code.u.call.name);
		}
		else if(loop->code.kind == ARG) {
			printf("ARG ");
			printOp(loop->code.u.arg.name);
		}
		else if(loop->code.kind == FUNC) {
			printf("FUNCTION ");
			printOp(loop->code.u.func.name);
			printf(" :");
			f = 1;
		}
		else if(loop->code.kind == PARAM) {
			printf("PARAM ");
			printOp(loop->code.u.param.name);
		}
		else if(loop->code.kind == DEC) {
			if(f == 1) {
				printf("DEC ");
				printOp(loop->code.u.dec.name);
				printf(" %d", loop->code.u.dec.size->u.value);
			}
		}
		printf("\n");
		loop = loop->next;
	}
	//fclose(stdout);
}
