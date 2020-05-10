%option yylineno
%{
#include <stdio.h>
#include <string.h>
#include "tree.h"
#include "syntax.tab.h"
int yycolumn = 1;
#define YY_USER_ACTION \
	yylloc.first_line = yylloc.last_line = yylineno; \
	yylloc.first_column = yycolumn; \
	yylloc.last_column = yycolumn + yyleng - 1; \
	yycolumn += yyleng;
%}
digit [0-9]
letter [_a-zA-z]
STRUCT struct
RETURN return
IF if
ELSE else
WHILE while
TYPE int|float
SEMI ;
COMMA ,
AND &&
OR \|\|
RELOP >|<|>=|<=|==|!=
ASSIGNOP =
LP \(
RP \)
LB \[
RB \]
LC \{
RC \}
PLUS \+
MINUS -
STAR \*
DIV \/
DOT \.
NOT !
ID [a-z_A-Z][a-zA-Z_0-9]*
INT [1-9][0-9]*|0
OCTO 0[0-7]+ 
HEX 0[x|X][0-9a-fA-F]+
FLOAT1 [.0-9]+[Ee][+-]*[0-9]+
FLOAT2 [0-9]+\.[0-9]+
COMMENT1 "/*"([^\*]|(\*)*[^\*/])*(\*)*"*/"
COMMENT2 "//"[^\n]*
%%
{COMMENT1} {
//	printf("COMMENT1\n");
}
{COMMENT2} {
//	printf("COMMENT\n");
}
{STRUCT} { 
//		printf("reserved word struct\n");
			yylval.treeNode = createStrNode(yylineno, yytext, "STRUCT");
			return STRUCT;
}
{RETURN} {
//		printf("reserved word return\n");
			yylval.treeNode = createStrNode(yylineno, yytext, "RETURN");
			return RETURN;
}
{IF} {
//	printf("reserved word if\n");
	yylval.treeNode = createStrNode(yylineno, yytext, "IF");
	return IF;
}
{ELSE} { 
//	printf("reserved word else\n");
		yylval.treeNode = createStrNode(yylineno, yytext, "ELSE");
		return ELSE;
}
{WHILE} { 
//	printf("reserved word while\n");
	yylval.treeNode = createStrNode(yylineno, yytext, "WHILE");	
	  return WHILE;
}
{SEMI} {
//	printf("SEMI\n");
	yylval.treeNode = createStrNode(yylineno, yytext, "SEMI");	
	return SEMI;
}
{COMMA} {
//	printf("COMMA\n");
		yylval.treeNode = createStrNode(yylineno, yytext, "COMMA");
		return COMMA; 
}
{AND} {
//	printf("AND\n");
		yylval.treeNode = createStrNode(yylineno, yytext, "AND");		
		return AND;
}
{OR} {
//	printf("OR\n");
		yylval.treeNode = createStrNode(yylineno, yytext, "OR");		
		return OR;
}
{RELOP} {
//	printf("RELOP\n");
		yylval.treeNode = createStrNode(yylineno, yytext, "RELOP");
		return RELOP;
}
{ASSIGNOP} {
//	printf("ASSIGNOP\n");
		yylval.treeNode = createStrNode(yylineno, yytext, "ASSIGNOP");
		return ASSIGNOP;
}
{LP} { 
//	printf("LP\n");
		yylval.treeNode = createStrNode(yylineno, yytext, "LP");
		return LP;
} 
{RP} { 
//	printf("RP\n");
		yylval.treeNode = createStrNode(yylineno, yytext, "RP");
		return RP;
}
{LC} { 
//	printf("LC\n"); 
		yylval.treeNode = createStrNode(yylineno, yytext, "LC");
		return LC;
}
{RC} { 
//	printf("RC\n");
		yylval.treeNode = createStrNode(yylineno, yytext, "RC");
		return RC;
}
{LB} { 
//	printf("LB\n");
		yylval.treeNode = createStrNode(yylineno, yytext, "LB");
		return LB;
}
{RB} { 
//	printf("RB\n");
		yylval.treeNode = createStrNode(yylineno, yytext, "RB");
		return RB;
}
{PLUS} { 
//	printf("PLUS\n");
		yylval.treeNode = createStrNode(yylineno, yytext, "PLUS");
		return PLUS;
}
{MINUS} {
//	printf("MINUS\n");
		yylval.treeNode = createStrNode(yylineno, yytext, "MINUS");
		 return MINUS;
}
{STAR} {
//	printf("STAR\n");
		yylval.treeNode = createStrNode(yylineno, yytext, "STAR");
		return STAR;
}
{DIV} {
//	printf("DIV\n");
		yylval.treeNode = createStrNode(yylineno, yytext, "DIV");
		return DIV;
}
{DOT} {
//	printf("DOT\n");
		yylval.treeNode = createStrNode(yylineno, yytext, "DOT");
		return DOT;
}
{NOT} {
//	printf("NOT\n");
		yylval.treeNode = createStrNode(yylineno, yytext, "NOT");
		return NOT;
}
{TYPE} {
//	printf("TYPE\n");
		yylval.treeNode = createStrNode(yylineno, yytext,"TYPE");
		return TYPE;
}
{OCTO} { 
//	printf("OCTO\n");
		int L = strlen(yytext);
		unsigned int temp = 0;
		for(int i = 1; i < L; i++) {
			temp = temp * 8 + (yytext[i] - '0');
		}
		yylval.treeNode = createIntNode(yylineno, temp, "INT");
		return INT;
}
{HEX} { 
//	printf("HEX\n");
		int L = strlen(yytext);
		unsigned int temp = 0;
		for(int i = 2; i < L; i++) {
			temp = temp * 16;
			if('0' <= yytext[i] && yytext[i] <= '9') {
				temp = temp + yytext[i] - '0';
			}
			else {
				if('a' <= yytext[i] && yytext[i] <= 'f') {
					temp = temp + yytext[i] - 'a' + 10;
				}
				else {
					temp = temp + yytext[i] - 'A' + 10;
				}
			}
		}
		yylval.treeNode = createIntNode(yylineno, temp, "INT");
		return INT;
}
{INT} { 
//	printf("INT\n");
		int L = strlen(yytext);
		unsigned int temp = 0;
		for(int i = 0; i < L; i++) {
			temp = temp * 10 + yytext[i] - '0';
		}
		yylval.treeNode = createIntNode(yylineno, temp, "INT");
		return INT;
}
{FLOAT1} { 
//	printf("FLOAT\n");
		int i, L1;
		float temp1, temp2, temp3;
		temp1 = temp2 = temp3 = 0;
		L1 = 0;
		int L = strlen(yytext);
		for(i = 0; i < L; i++) {
			if(yytext[i] == '.') break;
			temp1 = temp1 * 10 + yytext[i] - '0';
		}
		i++;
		for(; i < L; i++) {
			if(yytext[i] == 'e' || yytext[i] == 'E') break;
			temp2 = temp2 * 10 + yytext[i] - '0';
			L1++;
		}
		float p = 10;
		if(yytext[i + 1] == '+' || yytext[i + 1] == '-') {
			i++;
			if(yytext[i] == '-') p = 0.1;
		}
		i++;
		for(; i < L; i++) {
			temp3 = temp3 * 10 + yytext[i] - '0';
		}	
		for(i = 1; i <= L1; i++) {
			temp2 = temp2 * 0.1;
		}
		temp1 = temp1 + temp2;
		for(i = 1; i <= temp3; i++) {
			temp1 = temp1 * p;
		}

		yylval.treeNode = createFloatNode(yylineno, temp1, "FLOAT");
		return FLOAT;
}
{FLOAT2} {
//printf("FLOAT:%s\n", yytext);
	int i, L, L1;
	float temp1, temp2;
	temp1 = temp2 = 0;
	L1 = 0;
	L = strlen(yytext);
	for(i = 0; i < L; i++) {
		if(yytext[i] == '.') break;
		temp1 = temp1 * 10 + yytext[i] - '0';
	}
	i++;
	for(; i < L; i++) {
		temp2 = temp2 * 10 + yytext[i] - '0';
		L1++;
	}
	for(i = 1; i <= L1; i++) {
		temp2 = temp2 * 0.1;
	}
	temp1 = temp1 + temp2;
	yylval.treeNode = createFloatNode(yylineno, temp1, "FLOAT");
	return FLOAT;
}
{ID} { 
//	printf("This is an ID: %s\n", yytext);
		yylval.treeNode = createStrNode(yylineno, yytext, "ID");
		return ID;
}
[ \t]+ {}
\n {
	yycolumn = 1;
}
. {
	flag = 0;
	printf("Error type A at Line %d: Mysterious characters \'%s\'\n", yylineno, yytext);
}
%%
