#include "tree.h"
#include "trans.h"
#include "target.h"
#include <stdio.h>
#include <string.h>

int dataSize;
void initialize() {
	printf(".data\n");
	printf("_prompt: .asciiz \"Enter an integer:\"\n");
	printf("_ret: .asciiz \"\\n\"\n");
	printf(".globl main\n");
	printf(".text\n");
	printf("read:\n");
	printf("li $v0, 4\n");
	printf("la $a0, _prompt\n");
	printf("syscall\n");
	printf("li $v0, 5\n");
	printf("syscall\n");
	printf("jr $ra\n");
	printf("write:\n");
	printf("li $v0, 1\n");
	printf("syscall\n");
	printf("li $v0, 4\n");
	printf("la $a0, _ret\n");
	printf("syscall\n");
	printf("move $v0, $0\n");
	printf("jr $ra\n");
}

int getOffset(char* name) {
//+ if it's var, - if it's reg 
	struct offset* temp = offTable;
	while(temp != NULL) {
		if (!strcmp(name, temp->str)) return -temp->t;
		temp = temp->next;
	}

	temp = paramTable;
	while(temp != NULL) {
		if(!strcmp(name, temp->str)) return temp->t + 8;
		temp = temp->next;
	}
}

void getVal(Operand p, char* dst) {
	//value stored in dst
	if(p->kind == CONSTANT) 
		printf("li $%s, %d\n", dst, p->u.value);
	else {
		int tOffset = getOffset(p->u.val_na);
		if(p->kind == VARIABLE) 
			printf("lw $%s, %d($fp)\n", dst, tOffset);
		else if(p->kind == ADDRESS) 
			printf("addi $%s, $fp, %d\n", dst, tOffset);
		else if(p->kind == LOCATION) {
			printf("lw $t8, %d($fp)\n", tOffset);
			printf("lw $%s, 0($t8)\n", dst);
		}
	}
}

void setVal(Operand p, char* src) {
	//store value from src
	int tOffset = getOffset(p->u.val_na);
	printf("add $t8, $fp, %d\n", tOffset);
	if(p->kind != VARIABLE)
		printf("lw $t8, 0($t8)\n");
	printf("sw $%s, 0($t8)\n", src);
}


void genAssign(InterCodes node) {
	getVal(node->code.u.assign.right, "t0");
	setVal(node->code.u.assign.left, "t0");	
}

void genBinop(InterCodes node) {
	getVal(node->code.u.binop.op1, "t0");
	getVal(node->code.u.binop.op2, "t1");
	if (node->code.kind == ADD) printf("add $t0, $t0, $t1\n");
	else if(node->code.kind == SUB) printf("sub $t0, $t0, $t1\n");
	else if(node->code.kind == MUL) printf("mul $t0, $t0, $t1\n");
	else {
		printf("div $t0, $t1\n");
		printf("mflo $t0\n");
	}
	setVal(node->code.u.binop.result, "t0");
}

void genCon(InterCodes node) {
	getVal(node->code.u.con.op1, "t0");
	getVal(node->code.u.con.op3, "t1");
	if(!strcmp(node->code.u.con.op2->u.val_na, "==")) printf("beq");
	else if(!strcmp(node->code.u.con.op2->u.val_na, "!=")) printf("bne");
	else if(!strcmp(node->code.u.con.op2->u.val_na, ">")) printf("bgt");
	else if(!strcmp(node->code.u.con.op2->u.val_na, "<")) printf("blt");
	else if(!strcmp(node->code.u.con.op2->u.val_na, ">=")) printf("bge");
	else if(!strcmp(node->code.u.con.op2->u.val_na, "<=")) printf("ble");
	printf(" $t0, $t1, %s\n", node->code.u.con.pos->u.val_na);
}

void genReturn(InterCodes node) {
	getVal(node->code.u.back.op1, "t0");
	printf("move $v0, $t0\n");
	printf("jr $ra\n");
}

void genArg(InterCodes node) {	
	printf("addi $sp , $sp, -4\n");
	getVal(node->code.u.arg.name, "t0");
	printf("sw $t0, 0($sp)\n");
}

void genRead(InterCodes node) {
	printf("addi $sp, $sp, -4\n");
	printf("sw $ra, 0($sp)\n");
	printf("jal read\n");
	printf("lw $ra, 0($sp)\n");
	printf("addi $sp, $sp, 4\n");
	setVal(node->code.u.read.name, "v0");
}

void genWrite(InterCodes node) {
	getVal(node->code.u.write.name, "a0");
	printf("addi $sp, $sp, -4\n");
	printf("sw $ra, 0($sp)\n");
	printf("jal write\n");
	printf("lw $ra, 0($sp)\n");
	printf("addi $sp, $sp, 4\n");
}

void genCall(InterCodes node) {
	printf("addi $sp, $sp, -8\n"); 
	printf("sw $ra, 4($sp)\n");
	printf("sw $fp, 0($sp)\n");
	printf("jal %s\n", node->code.u.call.name->u.val_na);
	printf("move $sp, $fp\n");
	printf("lw $fp, 0($sp)\n");
	printf("lw $ra, 4($sp)\n");
	
//	printf("move $fp, $sp\n");
//	printf("addi $sp, $sp, %d\n", -dataSize);
	setVal(node->code.u.call.op1, "v0");
}

int getDataSize(char* name) {
	struct offset* temp = sizeTable;
	while(temp != NULL) {
		if(!strcmp(temp->str, name)) return temp->t;
		temp = temp->next;
	}
	return 0;
}

void targetGen(InterCodes node) {
	freopen(filename, "w", stdout);
//	freopen("workdir/a.ir", "w", stdout);
	initialize();
	
	InterCodes loop;
	loop = node;
	while(loop != NULL) {
		printCode(loop);
		if(loop->code.kind == FUNC) {
			printf("%s:\n", loop->code.u.func.name->u.val_na);
			printf("move $fp, $sp\n");
			dataSize = getDataSize(loop->code.u.func.name->u.val_na);
			printf("addi $sp, $sp, %d\n", -dataSize);
		}
		else if(loop->code.kind == LABEL) 
			printf("%s:\n", loop->code.u.label.pos->u.val_na);
		else if(loop->code.kind == GOTO)
			printf("j %s\n", loop->code.u.go.pos->u.val_na);
		else if(loop->code.kind == ASSIGN) 
			genAssign(loop);
		else if(loop->code.kind == ADD || loop->code.kind == SUB || loop->code.kind == MUL
			|| loop->code.kind == DIVI)
			genBinop(loop);
		else if(loop->code.kind == CON) 
			genCon(loop);
		else if(loop->code.kind == BACK)
			genReturn(loop);
		else if(loop->code.kind == WRITE)
			genWrite(loop);
		else if(loop->code.kind == READ) 
			genRead(loop);
		else if(loop->code.kind == ARG) 
			genArg(loop);
		else if(loop->code.kind == CALL) 
			genCall(loop);
		loop = loop->next;
	}

	fclose(stdout);
}

