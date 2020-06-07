%{
#include <stdio.h>
#include "lex.yy.c"
#include "tree.h"
#include "trans.h"
#include "target.h"
%}
%locations
%union {
	struct Node* treeNode;
}

%token <treeNode> INT
%token <treeNode> FLOAT
%token <treeNode> ID
%token <treeNode> SEMI
%token <treeNode> COMMA
%token <treeNode> ASSIGNOP
%token <treeNode> RELOP
%token <treeNode> PLUS
%token <treeNode> MINUS
%token <treeNode> STAR
%token <treeNode> DIV
%token <treeNode> AND
%token <treeNode> OR
%token <treeNode> DOT
%token <treeNode> NOT
%token <treeNode> TYPE
%token <treeNode> LP
%token <treeNode> RP
%token <treeNode> LB
%token <treeNode> RB
%token <treeNode> LC
%token <treeNode> RC
%token <treeNode> STRUCT
%token <treeNode> RETURN
%token <treeNode> IF
%token <treeNode> ELSE
%token <treeNode> WHILE

%type <treeNode> Program 
%type <treeNode> ExtDefList
%type <treeNode> ExtDef
%type <treeNode> ExtDecList Specifier FunDec 
%type <treeNode> CompSt
%type <treeNode> VarDec StructSpecifier OptTag DefList Tag VarList ParamDec StmtList Stmt
%type <treeNode> Exp Def 
%type <treeNode> Dec DecList Args

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE
%right ASSIGNOP
%left OR
%left AND
%left RELOP
%left PLUS MINUS
%left STAR DIV
%right UMINUS NOT
%left LP RP LB RB DOT
%%
Program : ExtDefList {
		$$ = createStrNode(@$.first_line, "1", "Program");
		addNode($$, $1);
		root = $$;

		prepare();
		argTable = NULL;
		semanticAnal(root);

		tempNum = labelNum = 0;
		struct codeList temp;
		offTable = paramTable = NULL;
		temp = translate(root);
//		printCode(temp.head);
//		printTree($$, 0);
		targetGen(temp.head);
	}
	;
ExtDefList : ExtDef ExtDefList {
		$$ = createStrNode(@$.first_line, "2", "ExtDefList");
		addNode($$, $1); addNode($$, $2); 
	}
	| /* empty */ {
	//	$$ = NULL;
		$$ = createStrNode(@$.first_line, "2", "ExtDefList");
		addNode($$, NULL);
	}
	;
ExtDef : Specifier ExtDecList SEMI {
		$$ = createStrNode(@$.first_line, "4", "ExtDef");
		addNode($$, $1); addNode($$, $2); addNode($$, $3);
	}
	| Specifier SEMI {
		$$ = createStrNode(@$.first_line, "5", "ExtDef");
		addNode($$, $1); addNode($$, $2); 
	}
	| Specifier FunDec CompSt {
		$$ = createStrNode(@$.first_line, "6", "ExtDef");
		addNode($$, $1); addNode($$, $2); addNode($$, $3);
	}
	| Specifier FunDec SEMI {
		$$ = createStrNode(@$.first_line, "1", "ExtDef");
		addNode($$, $1); addNode($$, $2); addNode($$, $3);
	}
	| error SEMI {
		$$ = NULL;
	}
	;
ExtDecList : VarDec {
		$$ = createStrNode(@$.first_line, "7", "ExtDecList");
		addNode($$, $1);
	}
	| VarDec COMMA ExtDecList {
		$$ = createStrNode(@$.first_line, "8", "ExtDecList");
		addNode($$, $1); addNode($$, $2); addNode($$, $3);
	}
	;
Specifier : TYPE {
		$$ = createStrNode(@$.first_line, "9", "Specifier");
		addNode($$, $1);
	}
	| StructSpecifier {
		$$ = createStrNode(@$.first_line, "1", "Specifier");
		addNode($$, $1);
	}
	;
StructSpecifier : STRUCT OptTag LC DefList RC {
		$$ = createStrNode(@$.first_line, "2", "StructSpecifier");
		addNode($$, $1); addNode($$, $2); addNode($$, $3);
		addNode($$, $4); addNode($$, $5);	
	}
	| STRUCT Tag {
		$$ = createStrNode(@$.first_line, "3", "StructSpecifier");
		addNode($$, $1); addNode($$, $2); 
	}
	| error RC {
		$$ = NULL;
	}
	;
OptTag : ID {
		$$ = createStrNode(@$.first_line, "4", "OptTag");
		addNode($$, $1); 
	}
	| /* empty */ {
		//$$ = NULL;
		$$ = createStrNode(@$.first_line, "4", "OptTag");
		addNode($$, NULL);
	}
	;
Tag : ID {
		$$ = createStrNode(@$.first_line, "5", "Tag");
		addNode($$, $1); 	
	}
	;
VarDec : ID {
		$$ = createStrNode(@$.first_line, "6", "VarDec");
		addNode($$, $1); 	
	}
	| VarDec LB INT RB {
		$$ = createStrNode(@$.first_line, "7", "VarDec");
		addNode($$, $1); addNode($$, $2); addNode($$, $3);
		addNode($$, $4);
	}
	;
FunDec : ID LP VarList RP {
		$$ = createStrNode(@$.first_line, "8", "FunDec");
		addNode($$, $1); addNode($$, $2); addNode($$, $3);
		addNode($$, $4);	
	}
	| ID LP RP {
		$$ = createStrNode(@$.first_line, "9", "FunDec");
		addNode($$, $1); addNode($$, $2); addNode($$, $3);	
	}
	| error RP {
		$$ = NULL;
	}
	;
VarList : ParamDec COMMA VarList {
		$$ = createStrNode(@$.first_line, "1", "VarList");
		addNode($$, $1); addNode($$, $2); addNode($$, $3);	
	}
	| ParamDec {
		$$ = createStrNode(@$.first_line, "2", "VarList");
		addNode($$, $1); 	
	}
	;
ParamDec : Specifier VarDec {
		$$ = createStrNode(@$.first_line, "3", "ParamDec");
		addNode($$, $1); addNode($$, $2);
	}
	;
CompSt : LC DefList StmtList RC {
		$$ = createStrNode(@$.first_line, "4", "CompSt");
		addNode($$, $1); addNode($$, $2); addNode($$, $3);
		addNode($$, $4);	
	}
	| error RC {
		$$ = NULL;
	}
	;
StmtList : Stmt StmtList {
		$$ = createStrNode(@$.first_line, "5", "StmtList");
		addNode($$, $1); addNode($$, $2);		
	}
	| /* empty */ {
	//	$$ = NULL;
		$$ = createStrNode(@$.first_line, "5", "StmtList");
		addNode($$, NULL);
	}
	;
Stmt : Exp SEMI {
		$$ = createStrNode(@$.first_line, "6", "Stmt");
		addNode($$, $1); addNode($$, $2);
	}
	| CompSt {
		$$ = createStrNode(@$.first_line, "7", "Stmt");
		addNode($$, $1);	
	}
	| RETURN Exp SEMI {
		$$ = createStrNode(@$.first_line, "8", "Stmt");
		addNode($$, $1); addNode($$, $2); addNode($$, $3);
	}
	| IF LP Exp RP Stmt %prec LOWER_THAN_ELSE {
		$$ = createStrNode(@$.first_line, "9", "Stmt");
		addNode($$, $1); addNode($$, $2); addNode($$, $3);
		addNode($$, $4); addNode($$, $5);				
	}
	| IF LP Exp RP Stmt ELSE Stmt {
		$$ = createStrNode(@$.first_line, "1", "Stmt");
		addNode($$, $1); addNode($$, $2); addNode($$, $3);
		addNode($$, $4); addNode($$, $5); addNode($$, $6);
		addNode($$, $7);
	}
	| WHILE LP Exp RP Stmt {
		$$ = createStrNode(@$.first_line, "2", "Stmt");
		addNode($$, $1); addNode($$, $2); addNode($$, $3);
		addNode($$, $4); addNode($$, $5);				
	}
	| error SEMI {
		$$ = NULL;
	}
	;
DefList : Def DefList {
		$$ = createStrNode(@$.first_line, "3", "DefList");
		addNode($$, $1); addNode($$, $2);
	}
	| /* empty */ {
		//$$ = NULL;
		$$ = createStrNode(@$.first_line, "4", "DefList");
		addNode($$, NULL);
	}
	;
Def : Specifier DecList SEMI {
		$$ = createStrNode(@$.first_line, "4", "Def");
		addNode($$, $1); addNode($$, $2); addNode($$, $3);
	}
	| error SEMI {
		$$ = NULL;
	}
	;
DecList : Dec {
		$$ = createStrNode(@$.first_line, "5", "DecList");
		addNode($$, $1);
	}
	| Dec COMMA DecList {
		$$ = createStrNode(@$.first_line, "6", "DecList");
		addNode($$, $1); addNode($$, $2); addNode($$, $3);	
	}
	;
Dec : VarDec {
		$$ = createStrNode(@$.first_line, "7", "Dec");
		addNode($$, $1);
	}
	| VarDec ASSIGNOP Exp {
		$$ = createStrNode(@$.first_line, "8", "Dec");
		addNode($$, $1); addNode($$, $2); addNode($$, $3);
	}
	;
Exp : Exp ASSIGNOP Exp {
		$$ = createStrNode(@$.first_line, "9", "Exp");
		addNode($$, $1); addNode($$, $2); addNode($$, $3);
	}
	| Exp AND Exp {
		$$ = createStrNode(@$.first_line, "1", "Exp");
		addNode($$, $1); addNode($$, $2); addNode($$, $3);
	}
	| Exp OR Exp {
		$$ = createStrNode(@$.first_line, "2", "Exp");
		addNode($$, $1); addNode($$, $2); addNode($$, $3);	
	}
	| Exp RELOP Exp {
		$$ = createStrNode(@$.first_line, "3", "Exp");
		addNode($$, $1); addNode($$, $2); addNode($$, $3);	
	}
	| Exp PLUS Exp {
		$$ = createStrNode(@$.first_line, "4", "Exp");
		addNode($$, $1); addNode($$, $2); addNode($$, $3);	
	}
	| Exp MINUS Exp {
		$$ = createStrNode(@$.first_line, "5", "Exp");
		addNode($$, $1); addNode($$, $2); addNode($$, $3);	
	}
	| Exp STAR Exp {
		$$ = createStrNode(@$.first_line, "6", "Exp");
		addNode($$, $1); addNode($$, $2); addNode($$, $3);	
	}
	| Exp DIV Exp {
		$$ = createStrNode(@$.first_line, "7", "Exp");
		addNode($$, $1); addNode($$, $2); addNode($$, $3);	
	}
	| LP Exp RP {
		$$ = createStrNode(@$.first_line, "8", "Exp");
		addNode($$, $1); addNode($$, $2); addNode($$, $3);	
	}
	| MINUS Exp %prec UMINUS{
		$$ = createStrNode(@$.first_line, "9", "Exp");
		addNode($$, $1); addNode($$, $2);	
	}
	| NOT Exp {
		$$ = createStrNode(@$.first_line, "1", "Exp");
		addNode($$, $1); addNode($$, $2);	
	}
	| ID LP Args RP {
		$$ = createStrNode(@$.first_line, "2", "Exp");
		addNode($$, $1); addNode($$, $2); addNode($$, $3); addNode($$, $4);	
	}
	| ID LP RP {
		$$ = createStrNode(@$.first_line, "3", "Exp");
		addNode($$, $1); addNode($$, $2); addNode($$, $3);	
	}
	| Exp LB Exp RB {
		$$ = createStrNode(@$.first_line, "4", "Exp");
		addNode($$, $1); addNode($$, $2); addNode($$, $3); addNode($$, $4);	
	}
	| Exp DOT ID {
		$$ = createStrNode(@$.first_line, "5", "Exp");
		addNode($$, $1); addNode($$, $2); addNode($$, $3);	
	}
	| ID {
		$$ = createStrNode(@$.first_line, "6", "Exp");
		addNode($$, $1);
	}
	| INT {
		$$ = createStrNode(@$.first_line, "7", "Exp");
		addNode($$, $1);
	}
	| FLOAT {
		$$ = createStrNode(@$.first_line, "8", "Exp");
		addNode($$, $1);
	}
	| error RP {
		$$ = NULL;
	}
	;
Args : Exp COMMA Args {
		$$ = createStrNode(@$.first_line, "9", "Args");
		addNode($$, $1); addNode($$, $2); addNode($$, $3);
	}
	| Exp {
		$$ = createStrNode(@$.first_line, "1", "Args");
		addNode($$, $1);
	}
	;
%%
