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

struct codeList translate_Exp(struct Node* node, char* place) {
	struct codeList ans;
	if(!strcmp(node->child[1]->info, "INT")) {
		InterCodes p = (InterCodes)malloc(sizeof(struct InterCodes_));
		p->code.kind = ASSIGN;
		createVar(&p->code.u.assign.left, place);
		createConstant(&p->code.u.assign.right, node->child[1]->type_int);
		p->prev = p->next = NULL;

		ans.head = ans.tail = p;
	}
	else if(!strcmp(node->child[1]->info, "ID")) {
		InterCodes p = (InterCodes)malloc(sizeof(struct InterCodes_));
		p->code.kind = ASSIGN;
		createVar(&p->code.u.assign.left, place);
		createVar(&p->code.u.assign.right, node->child[1]->type_string);
		p->prev = p->next = NULL;

		ans.head = ans.tail = p;
	}
	else if(!strcmp(node->child[1]->info, "MINUS")) {
		char* t1 = newTemp();
		struct codeList code1, code2;
		code1 = translate_Exp(node->child[2], t1);
	
		InterCodes p = (InterCodes)malloc(sizeof(struct InterCodes_));
		p->code.kind = SUB;
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
			//TODO: array and struct
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
			if(!strcmp(node->child[2]->info, "MUL")) p->code.kind = MUL;
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
	else {
		char* label1 = newLabel();
		char* label2 = newLabel();
		struct codeList code0, code1, code2, code3, code4;

		InterCodes p1 = (InterCodes)malloc(sizeof(struct InterCodes_));
		p1->code.kind = ASSIGN;
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

struct codeList translate_Cond(struct Node* node, char* t, char* f) {
	struct codeList ans;
	if(!strcmp(node->child[2]->info, "RELOP")) {
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
		
		InterCodes p1 = (InterCodes)malloc(sizeof(struct InterCodes_));
		p1->code.kind = GOTO;
		createVar(&p1->code.u.go.pos, f);
		p1->prev = p1->next = NULL;
		code4.head = code4.tail = p1;

		code1 = codePatch(code1.head, &code1.tail, &code2.head, code2.tail);
		code1 = codePatch(code1.head, &code1.tail, &code3.head, code3.tail);
		code1 = codePatch(code1.head, &code1.tail, &code4.head, code4.tail);

		ans = code1;
	}
	else if(!strcmp(node->child[2]->info, "Exp")) {
		return translate_Cond(node->child[2], f, t);	
	}
	else if(!strcmp(node->child[2]->info, "AND")) {
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
	else if(!strcmp(node->child[2]->info, "OR")) {
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
		
		InterCodes p1 = (InterCodes)malloc(sizeof(struct InterCodes_));
		p1->code.kind = GOTO;
		createVar(&p1->code.u.go.pos, f);
		p1->prev = p1->next = NULL;
		code3.head = code3.tail = p1;

		code1 = codePatch(code1.head, &code1.tail, &code2.head, code2.tail);
		code1 = codePatch(code1.head, &code1.tail, &code3.head, code3.tail);
		ans = code1;
	}
	return ans;
}

struct codeList translate_Stmt(struct Node* node) {
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
		
		InterCodes p1 = (InterCodes)malloc(sizeof(struct InterCodes_));
		p1->code.kind = GOTO;
		createVar(&p1->code.u.go.pos, label1);
		p1->prev = p1->next = NULL;
		code5.head = code5.tail = p1;

		code6 = createLabel(label3);
		code1 = codePatch(code3.head, &code3.tail, &code1.head, code1.tail);
		code1 = codePatch(code1.head, &code1.tail, &code4.head, code4.tail);
		code1 = codePatch(code1.head, &code1.tail, &code2.head, code2.tail);
		code1 = codePatch(code1.head, &code1.tail, &code5.head, code5.tail);
		code1 = codePatch(code1.head, &code1.tail, &code6.head, code6.tail);
	}
	else if(node->num < 7) {
		char* label1, label2;
		label1 = newLabel();
		label2 = newLabel();
		//TODO:
	}
	else {
	}
	return ans;
}

struct codeList translate_CompSt(struct Node* node) {
//TODO: finish it
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
}

void printCode(InterCodes node) {
	InterCodes loop;
	loop = node;
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
		printf("\n");
		loop = loop->next;
	}
}
