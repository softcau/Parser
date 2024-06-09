#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 10000

//Terminal 및 Nonterminal을 enum 형식을 활용하여 정의
typedef enum {
	vtype_token, num_token, character_token, boolstr_token, literal_token, id_token, if_token, 
	else_token, while_token, return_token, addsub_token, multdiv_token, assign_token, comp_token,
	semi_token, comma_token, lparen_token, rparen_token, lbrace_token, rbrace_token, $
}Terminals;

typedef enum {
	CODE, VDECL, ASSIGN, RHS, EXPR, FDECL, ARG, MOREARGS, BLOCK, STMT, COND, ELSE,
	RETURN, T, F, C
}NonTerminals;

// Tree 구조 구현을 위한 함수 생성
typedef struct TreeNode {
	char term[20];
	struct TreeNode* child[20];
	int child_count;
} TreeNode;

TreeNode* stackNodes[MAX_SIZE];
int nodeTop = -1;

void pushNode(TreeNode* node) {
	stackNodes[++nodeTop] = node;
}

TreeNode* popNode() {
	return stackNodes[nodeTop--];
}

TreeNode* createNode(char* term) {
	TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
	strcpy(node->term, term);
	node->child_count = 0;
	return node;
}

void addChild(TreeNode* parent, TreeNode* child) {
	parent->child[parent->child_count++] = child;
}

void printTree(TreeNode* node, int level) {
	for (int i = 0; i < level; i++) {
		printf("  ");
	}
	printf("%s\n", node->term);
	for (int i = 0; i < node->child_count; i++) {
		printTree(node->child[i], level + 1);
	}
}

// 스택 생성 및 기능 구현(push, pop, displayStack, peek)
int stack[MAX_SIZE];
int top = -1;

void push(int element) {
	if (top == (MAX_SIZE - 1)) {
		printf("stack overflow\n");
		return;
	}
	stack[++top] = element;
} 

int pop() {
	if (top == -1) {
		printf("stack underflow\n");
		return -1;
	}
	return stack[top--];
}

void displayStack() {
	printf("stack display : ");
	for (int i = top; i >= 0; i--) {
		printf("%d ", stack[i]);
	}
	puts("");
}

int peek() {
	if (top < 0) {
		printf("stack underflow\n");
		return -1;
	}
	return stack[top];
}

// SLR table(action, goTo) 생성
char* action[100][25];
int goTo[100][25];

// CFG 기반 Reduce 규칙 구현
void reduce(int rule) {
	TreeNode* parent;
	switch (rule) {
	case 1: // CODE -> VDECL CODE
		pop();
		pop();
		push(goTo[peek()][CODE]);

		parent = createNode("CODE");
		addChild(parent, popNode());
		addChild(parent, popNode());
		pushNode(parent);

		break;
	case 2: // CODE -> FDECL CODE
		pop();
		pop();
		push(goTo[peek()][CODE]);

		parent = createNode("CODE");
		addChild(parent, popNode()); // CODE
		addChild(parent, popNode()); // FDECL
		pushNode(parent);

		break;
	case 3: // CODE -> ''
		push(goTo[peek()][CODE]);

		parent = createNode("CODE");
		addChild(parent, createNode("ε"));
		pushNode(parent);

		break;
	case 4: // VDECL->vtype id semi
		pop();
		pop();
		pop();
		push(goTo[peek()][VDECL]);

		parent = createNode("VDECL");
		addChild(parent, popNode()); // semi
		addChild(parent, popNode()); // id
		addChild(parent, popNode()); // vtype
		pushNode(parent);
		break;
	case 5: // VDECL->vtype ASSIGN semi
		pop();
		pop();
		pop();
		push(goTo[peek()][VDECL]);

		parent = createNode("VDECL");
		addChild(parent, popNode()); // semi
		addChild(parent, popNode()); // ASSIGN
		addChild(parent, popNode()); // vtype
		pushNode(parent);

		break;
	case 6: // ASSIGN->id assign RHS
		pop();
		pop();
		pop();
		push(goTo[peek()][ASSIGN]);

		parent = createNode("ASSIGN");
		addChild(parent, popNode()); // RHS
		addChild(parent, popNode()); // assign
		addChild(parent, popNode()); // id
		pushNode(parent);

		break;
	case 7: // RHS->EXPR
		pop();
		push(goTo[peek()][RHS]);

		parent = createNode("RHS");
		addChild(parent, popNode()); // EXPR
		pushNode(parent);

		break;
	case 8: // RHS->literal
		pop();
		push(goTo[peek()][RHS]);

		parent = createNode("RHS");
		addChild(parent, popNode()); // literal
		pushNode(parent);

		break;
	case 9: // RHS->character
		pop();
		push(goTo[peek()][RHS]);

		parent = createNode("RHS");
		addChild(parent, popNode()); // character
		pushNode(parent);

		break;
	case 10: // RHS->boolstr
		pop();
		push(goTo[peek()][RHS]);

		parent = createNode("RHS");
		addChild(parent, popNode()); // boolstr
		pushNode(parent);

		break;
	case 11: // EXPR->EXPR addsub T
		pop();
		pop();
		pop();
		push(goTo[peek()][EXPR]);

		parent = createNode("EXPR");
		addChild(parent, popNode()); // T
		addChild(parent, popNode()); // addsub
		addChild(parent, popNode()); // EXPR
		pushNode(parent);

		break;
	case 12: // EXPR->T
		pop();
		push(goTo[peek()][EXPR]);

		parent = createNode("EXPR");
		addChild(parent, popNode()); // T
		pushNode(parent);

		break;
	case 13: // T->T multdiv F
		pop();
		pop();
		pop();
		push(goTo[peek()][T]);

		parent = createNode("T");
		addChild(parent, popNode()); // F
		addChild(parent, popNode()); // multdiv
		addChild(parent, popNode()); // T
		pushNode(parent);

		break;
	case 14: // T->F
		pop();
		push(goTo[peek()][T]);

		parent = createNode("T");
		addChild(parent, popNode()); // F
		pushNode(parent);

		break;
	case 15: // F->lparen EXPR rparen
		pop();
		pop();
		pop();
		push(goTo[peek()][F]);

		parent = createNode("F");
		addChild(parent, popNode()); // rparen
		addChild(parent, popNode()); // EXPR
		addChild(parent, popNode()); // lparen
		pushNode(parent);

		break;
	case 16: // F->id
		pop();
		push(goTo[peek()][F]);

		parent = createNode("F");
		addChild(parent, popNode()); // id
		pushNode(parent);

		break;
	case 17: // F->num
		pop();
		push(goTo[peek()][F]);

		parent = createNode("F");
		addChild(parent, popNode()); // num
		pushNode(parent);

		break;
	case 18: // FDECL->vtype id lparen ARG rparen lbrace BLOCK RETURN rbrace
		for (int i = 0; i < 9; i++) {
			pop();
		}
		push(goTo[peek()][FDECL]);

		parent = createNode("FDECL");
		addChild(parent, popNode()); // rbrace
		addChild(parent, popNode()); // RETURN
		addChild(parent, popNode()); // BLOCK
		addChild(parent, popNode()); // lbrace
		addChild(parent, popNode()); // rparen
		addChild(parent, popNode()); // ARG
		addChild(parent, popNode()); // lparen
		addChild(parent, popNode()); // id
		addChild(parent, popNode()); // vtype
		pushNode(parent);

		break;
	case 19: // ARG->vtype id MOREARGS
		pop();
		pop();
		pop();
		push(goTo[peek()][ARG]);

		parent = createNode("ARG");
		addChild(parent, popNode()); // MOREARGS
		addChild(parent, popNode()); // id
		addChild(parent, popNode()); // vtype
		pushNode(parent);

		break;
	case 20: // ARG -> ''
		push(goTo[peek()][ARG]);

		parent = createNode("ARG");
		addChild(parent, createNode("ε"));
		pushNode(parent);

		break;
	case 21: // MOREARGS->comma vtype id MOREARGS
		pop();
		pop();
		pop();
		pop();
		push(goTo[peek()][MOREARGS]);

		parent = createNode("MOREARGS");
		addChild(parent, popNode()); // MOREARGS
		addChild(parent, popNode()); // id
		addChild(parent, popNode()); // vtype
		addChild(parent, popNode()); // comma
		pushNode(parent);

		break;
	case 22: // MOREARGS -> ''
		push(goTo[peek()][MOREARGS]);

		parent = createNode("MOREARGS");
		addChild(parent, createNode("ε"));
		pushNode(parent);

		break;
	case 23: // BLOCK->STMT BLOCK
		pop();
		pop();
		push(goTo[peek()][BLOCK]);

		parent = createNode("BLOCK");
		addChild(parent, popNode()); // BLOCK
		addChild(parent, popNode()); // STMT
		pushNode(parent);

		break;
	case 24: // BLOCK -> ''
		push(goTo[peek()][BLOCK]);

		parent = createNode("BLOCK");
		addChild(parent, createNode("ε"));
		pushNode(parent);

		break;
	case 25: // STMT->VDECL
		pop();
		push(goTo[peek()][STMT]);

		parent = createNode("STMT");
		addChild(parent, popNode()); // VDECL
		pushNode(parent);

		break;
	case 26: // STMT->ASSIGN semi
		pop();
		pop();
		push(goTo[peek()][STMT]);

		parent = createNode("STMT");
		addChild(parent, popNode()); // semi
		addChild(parent, popNode()); // ASSIGN
		pushNode(parent);

		break;
	case 27: // STMT -> if lparen COND rparen lbrace BLOCK rbrace ELSE
		for (int i = 0; i < 8; i++) {
			pop();
		}
		push(goTo[peek()][STMT]);

		parent = createNode("STMT");
		addChild(parent, popNode()); // ELSE
		addChild(parent, popNode()); // rbrace
		addChild(parent, popNode()); // BLOCK
		addChild(parent, popNode()); // lbrace
		addChild(parent, popNode()); // rparen
		addChild(parent, popNode()); // COND
		addChild(parent, popNode()); // lparen
		addChild(parent, popNode()); // if
		pushNode(parent);

		break;
	case 28: // STMT -> while lparen COND rparen lbrace BLOCK rbrace
		for (int i = 0; i < 7; i++) {
			pop();
		}
		push(goTo[peek()][STMT]);

		parent = createNode("STMT");
		addChild(parent, popNode()); // rbrace
		addChild(parent, popNode()); // BLOCK
		addChild(parent, popNode()); // lbrace
		addChild(parent, popNode()); // rparen
		addChild(parent, popNode()); // COND
		addChild(parent, popNode()); // lparen
		addChild(parent, popNode()); // while
		pushNode(parent);

		break;
	case 29: // COND->boolstr C
		pop();
		pop();
		push(goTo[peek()][COND]);

		parent = createNode("COND");
		addChild(parent, popNode()); // C
		addChild(parent, popNode()); // boolstr
		pushNode(parent);

		break;
	case 30: // C->comp COND
		pop();
		pop();
		push(goTo[peek()][C]);

		parent = createNode("C");
		addChild(parent, popNode()); // COND
		addChild(parent, popNode()); // comp
		pushNode(parent);

		break;
	case 31: // C -> ''
		push(goTo[peek()][C]);

		parent = createNode("C");
		addChild(parent, createNode("ε"));
		pushNode(parent);

		break;
	case 32: // ELSE -> else lbrace BLOCK rbrace
		pop();
		pop();
		pop();
		pop();
		push(goTo[peek()][ELSE]);

		parent = createNode("ELSE");
		addChild(parent, popNode()); // rbrace
		addChild(parent, popNode()); // BLOCK
		addChild(parent, popNode()); // lbrace
		addChild(parent, popNode()); // else
		pushNode(parent);

		break;
	case 33: // ELSE -> ''
		push(goTo[peek()][ELSE]);

		parent = createNode("ELSE");
		addChild(parent, createNode("ε"));
		pushNode(parent);

		break;
	case 34: // RETURN -> return RHS semi
		pop();
		pop();
		pop();
		push(goTo[peek()][RETURN]);

		parent = createNode("RETURN");
		addChild(parent, popNode()); // semi
		addChild(parent, popNode()); // RHS
		addChild(parent, popNode()); // return
		pushNode(parent);

		break;
	default:
		printf("error");
		exit(1);
	}
}

//parsing 반복을 위한 제어 변수(input이 accept된 경우 1로 바뀜.)
int flag = 0;

// table 구성에 따른 shift/reduce별 동작 구현
char* shift_reduce(char* task_ptr, char* actionRule) {
	//actionRule은 s4, r1과 같이 테이블 내의 rule을 의미
	if (actionRule == NULL) { // 스택의 state와 해당 token에 대응하는 규칙이 없을 경우
		printf("%s\n", "Rejected! - No Action in Parsing Table.");
		exit(1);
	}

	int num = atoi(actionRule + 1); // s4, r1에서 4,1과 같이 state만 추출

	if (actionRule[0] == 's') { // Shift
		push(num);

		TreeNode* leaf = createNode(task_ptr);
		pushNode(leaf);
		task_ptr = strtok(NULL, " ");
	}
	else if (actionRule[0] == 'r') { // Reduce
		reduce(num);
	}
	else if (strcmp(actionRule, "acc") == 0) { // 주어진 input이 accept 된 경우
		printf("Accepted\n");
		
		flag = 1;
	}
	return task_ptr;
}


// 파싱 수행
void parse(char* task_ptr) {
	while (flag == 0) {
		if (task_ptr == NULL) { // 주어진 input에 대해 모두 shift한 경우, task_ptr을 $로 유지
			task_ptr = "$";
		}
		printf("처리중인 토큰 : %s\n", task_ptr);

		//각 input terminal에 대응하는 작업 수행
		if (strcmp(task_ptr, "vtype_token") == 0) {//next input token이 vtype인 경우
			task_ptr = shift_reduce(task_ptr, action[peek()][vtype_token]); // table에서 현재 state와 vtype에 대응하는 규칙을 읽음.
		}
		else if (strcmp(task_ptr, "num_token") == 0) {
			task_ptr = shift_reduce(task_ptr, action[peek()][num_token]);
		}
		else if (strcmp(task_ptr, "character_token") == 0) {
			task_ptr = shift_reduce(task_ptr, action[peek()][character_token]);
		}
		else if (strcmp(task_ptr, "boolstr_token") == 0) {
			task_ptr = shift_reduce(task_ptr, action[peek()][boolstr_token]);
		}
		else if (strcmp(task_ptr, "literal_token") == 0) {
			task_ptr = shift_reduce(task_ptr, action[peek()][literal_token]);
		}
		else if (strcmp(task_ptr, "id_token") == 0) {
			task_ptr = shift_reduce(task_ptr, action[peek()][id_token]);
		}
		else if (strcmp(task_ptr, "if_token") == 0) {
			task_ptr = shift_reduce(task_ptr, action[peek()][if_token]);
		}
		else if (strcmp(task_ptr, "else_token") == 0) {
			task_ptr = shift_reduce(task_ptr, action[peek()][else_token]);
		}
		else if (strcmp(task_ptr, "while_token") == 0) {
			task_ptr = shift_reduce(task_ptr, action[peek()][while_token]);
		}
		else if (strcmp(task_ptr, "return_token") == 0) {
			task_ptr = shift_reduce(task_ptr, action[peek()][return_token]);
		}
		else if (strcmp(task_ptr, "addsub_token") == 0) {
			task_ptr = shift_reduce(task_ptr, action[peek()][addsub_token]);
		}
		else if (strcmp(task_ptr, "multdiv_token") == 0) {
			task_ptr = shift_reduce(task_ptr, action[peek()][multdiv_token]);
		}
		else if (strcmp(task_ptr, "assign_token") == 0) {
			task_ptr = shift_reduce(task_ptr, action[peek()][assign_token]);
		}
		else if (strcmp(task_ptr, "comp_token") == 0) {
			task_ptr = shift_reduce(task_ptr, action[peek()][comp_token]);
		}
		else if (strcmp(task_ptr, "semi_token") == 0) {
			task_ptr = shift_reduce(task_ptr, action[peek()][semi_token]);
		}
		else if (strcmp(task_ptr, "comma_token") == 0) {
			task_ptr = shift_reduce(task_ptr, action[peek()][comma_token]);
		}
		else if (strcmp(task_ptr, "comma_token") == 0) {
			task_ptr = shift_reduce(task_ptr, action[peek()][comma_token]);
		}
		else if (strcmp(task_ptr, "lparen_token") == 0) {
			task_ptr = shift_reduce(task_ptr, action[peek()][lparen_token]);
		}
		else if (strcmp(task_ptr, "rparen_token") == 0) {
			task_ptr = shift_reduce(task_ptr, action[peek()][rparen_token]);
		}
		else if (strcmp(task_ptr, "lbrace_token") == 0) {
			task_ptr = shift_reduce(task_ptr, action[peek()][lbrace_token]);
		}
		else if (strcmp(task_ptr, "rbrace_token") == 0) {
			task_ptr = shift_reduce(task_ptr, action[peek()][rbrace_token]);
		}
		else if (strcmp(task_ptr, "$") == 0) {
			task_ptr = shift_reduce(task_ptr, action[peek()][$]);
			if (flag == 1) { break; }
		}
		else { // 주어진 input에 잘못된 token이 들어있는 경우
			printf("Error detected.\n");
			printf("Please check input sequence.\n");
			exit(1);
		}
		displayStack();
	}
}

//스택 생성 및 SLR Parsing table 구성
void init() {
	// 스택 생성
	push(0);

	// SLR Parsing table 구성
	{
		action[0][vtype_token] = "s4";
		action[2][vtype_token] = "s4";
		action[3][vtype_token] = "s4";
		action[9][vtype_token] = "r4";
		action[10][vtype_token] = "s14";
		action[0][$] = "r3";
		action[1][$] = "acc";
		action[2][$] = "r3";
		action[3][$] = "r3";
		action[5][$] = "r1";
		action[6][$] = "r2";
		action[7][semi_token] = "s9";
		action[8][semi_token] = "s12";
		action[7][assign_token] = "s11";
		action[7][lparen_token] = "s10";
		action[4][id_token] = "s7";
		action[9][id_token] = "r4";
		action[9][rbrace_token] = "r4";
		action[9][if_token] = "r4";
		action[9][while_token] = "r4";
		action[9][return_token] = "r4";
		action[9][$] = "r4";
		action[10][rparen_token] = "r20";
		action[11][id_token] = "s23";
		action[11][literal_token] = "s17";
		action[11][character_token] = "s18";
		action[11][boolstr_token] = "s19";
		action[11][lparen_token] = "s22";
		action[11][num_token] = "s24";
		action[12][vtype_token] = "r5";
		action[12][id_token] = "r5";
		action[12][rbrace_token] = "r5";
		action[12][if_token] = "r5";
		action[12][while_token] = "r5";
		action[12][return_token] = "r5";
		action[12][$] = "r5";
		action[13][rparen_token] = "s25";
		action[14][id_token] = "s26";
		action[15][semi_token] = "r6";
		action[16][semi_token] = "r7";
		action[16][addsub_token] = "s27";
		action[17][semi_token] = "r8";
		action[18][semi_token] = "r9";
		action[19][semi_token] = "r10";
		action[20][semi_token] = "r12";
		action[20][addsub_token] = "r12";
		action[20][multdiv_token] = "s28";
		action[20][rparen_token] = "r12";
		action[21][semi_token] = "r14";
		action[21][addsub_token] = "r14";
		action[21][multdiv_token] = "r14";
		action[21][rparen_token] = "r14";
		action[22][id_token] = "s23";
		action[22][lparen_token] = "s22";
		action[22][num_token] = "s24";
		action[23][semi_token] = "r16";
		action[23][addsub_token] = "r16";
		action[23][multdiv_token] = "r16";
		action[23][rparen_token] = "r16";
		action[24][semi_token] = "r17";
		action[24][addsub_token] = "r17";
		action[24][multdiv_token] = "r17";
		action[24][rparen_token] = "r17";
		action[25][lbrace_token] = "s30";
		action[26][rparen_token] = "r22";
		action[26][comma_token] = "s32";
		action[27][id_token] = "s23";
		action[27][lparen_token] = "s22";
		action[27][num_token] = "s24";
		action[28][id_token] = "s23";
		action[28][lparen_token] = "s22";
		action[28][num_token] = "s24";
		action[29][addsub_token] = "s27";
		action[29][rparen_token] = "s35";
		action[30][vtype_token] = "s42";
		action[30][id_token] = "s43";
		action[30][rbrace_token] = "r24";
		action[30][if_token] = "s40";
		action[30][while_token] = "s41";
		action[30][return_token] = "r24";
		action[31][rparen_token] = "r19";
		action[32][vtype_token] = "s44";
		action[33][semi_token] = "r11";
		action[33][addsub_token] = "r11";
		action[33][multdiv_token] = "s28";
		action[33][rparen_token] = "r11";
		action[34][semi_token] = "r13";
		action[34][addsub_token] = "r13";
		action[34][multdiv_token] = "r13";
		action[34][rparen_token] = "r13";
		action[35][semi_token] = "r15";
		action[35][addsub_token] = "r15";
		action[35][multdiv_token] = "r15";
		action[35][rparen_token] = "r15";
		action[36][return_token] = "s46";
		action[37][vtype_token] = "s42";
		action[37][id_token] = "s43";
		action[37][rbrace_token] = "r24";
		action[37][if_token] = "s40";
		action[37][while_token] = "s41";
		action[37][return_token] = "r24";
		action[38][vtype_token] = "r25";
		action[38][id_token] = "r25";
		action[38][rbrace_token] = "r25";
		action[38][if_token] = "r25";
		action[38][while_token] = "r25";
		action[38][return_token] = "r25";
		action[39][semi_token] = "s48";
		action[40][lparen_token] = "s49";
		action[41][lparen_token] = "s50";
		action[42][id_token] = "s51";
		action[43][assign_token] = "s11";
		action[44][id_token] = "s52";
		action[45][rbrace_token] = "s53";
		action[46][id_token] = "s23";
		action[46][literal_token] = "s17";
		action[46][character_token] = "s18";
		action[46][boolstr_token] = "s19";
		action[46][lparen_token] = "s22";
		action[46][num_token] = "s24";
		action[47][rbrace_token] = "r23";
		action[47][return_token] = "r23";
		action[48][vtype_token] = "r26";
		action[48][id_token] = "r26";
		action[48][rbrace_token] = "r26";
		action[48][if_token] = "r26";
		action[48][while_token] = "r26";
		action[48][return_token] = "r26";
		action[49][boolstr_token] = "s56";
		action[50][boolstr_token] = "s56";
		action[51][semi_token] = "s9";
		action[51][assign_token] = "s11";
		action[52][rparen_token] = "r22";
		action[52][comma_token] = "s32";
		action[53][vtype_token] = "r18";
		action[53][$] = "r18";
		action[54][semi_token] = "s59";
		action[55][rparen_token] = "s60";
		action[56][rparen_token] = "r31";
		action[56][comp_token] = "s62";
		action[57][rparen_token] = "s63";
		action[58][rparen_token] = "r21";
		action[59][rbrace_token] = "r34";
		action[60][lbrace_token] = "s64";
		action[61][rparen_token] = "r29";
		action[62][boolstr_token] = "s56";
		action[63][lbrace_token] = "s66";
		action[64][vtype_token] = "s42";
		action[64][id_token] = "s43";
		action[64][rbrace_token] = "r24";
		action[64][if_token] = "s40";
		action[64][while_token] = "s41";
		action[64][return_token] = "r24";
		action[65][rparen_token] = "r30";
		action[66][vtype_token] = "s42";
		action[66][id_token] = "s43";
		action[66][rbrace_token] = "r24";
		action[66][if_token] = "s40";
		action[66][while_token] = "s41";
		action[66][return_token] = "r24";
		action[67][rbrace_token] = "s69";
		action[68][rbrace_token] = "s70";
		action[69][vtype_token] = "r33";
		action[69][id_token] = "r33";
		action[69][rbrace_token] = "r33";
		action[69][if_token] = "r33";
		action[69][while_token] = "r33";
		action[69][else_token] = "s72";
		action[69][return_token] = "r33";
		action[70][vtype_token] = "r28";
		action[70][id_token] = "r28";
		action[70][rbrace_token] = "r28";
		action[70][if_token] = "r28";
		action[70][while_token] = "r28";
		action[70][return_token] = "r28";
		action[71][vtype_token] = "r27";
		action[71][id_token] = "r27";
		action[71][rbrace_token] = "r27";
		action[71][if_token] = "r27";
		action[71][while_token] = "r27";
		action[71][return_token] = "r27";
		action[72][lbrace_token] = "s73";
		action[73][vtype_token] = "s42";
		action[73][id_token] = "s43";
		action[73][rbrace_token] = "r24";
		action[73][if_token] = "s40";
		action[73][while_token] = "s41";
		action[73][return_token] = "r24";
		action[74][rbrace_token] = "s75";
		action[75][vtype_token] = "r32";
		action[75][id_token] = "r32";
		action[75][rbrace_token] = "r32";
		action[75][if_token] = "r32";
		action[75][while_token] = "r32";
		action[75][return_token] = "r32";

		goTo[0][CODE] = 1;
		goTo[0][VDECL] = 2;
		goTo[4][ASSIGN] = 8;
		goTo[0][FDECL] = 3;
		goTo[69][ELSE] = 71;
		goTo[36][RETURN] = 45;
		goTo[2][CODE] = 5;
		goTo[2][VDECL] = 2;
		goTo[30][ASSIGN] = 39;
		goTo[3][CODE] = 6;
		goTo[3][VDECL] = 2;
		goTo[37][ASSIGN] = 39;
		goTo[2][FDECL] = 3;
		goTo[30][VDECL] = 38;
		goTo[3][FDECL] = 3;
		goTo[37][VDECL] = 38;
		goTo[64][VDECL] = 38;
		goTo[66][VDECL] = 38;
		goTo[42][ASSIGN] = 8;
		goTo[64][ASSIGN] = 39;
		goTo[66][ASSIGN] = 39;
		goTo[10][ARG] = 13;
		goTo[11][RHS] = 15;
		goTo[11][EXPR] = 16;
		goTo[11][T] = 20;
		goTo[11][F] = 21;
		goTo[46][RHS] = 54;
		goTo[73][VDECL] = 38;
		goTo[73][ASSIGN] = 39;
		goTo[22][EXPR] = 29;
		goTo[22][T] = 20;
		goTo[22][F] = 21;
		goTo[26][MOREARGS] = 31;
		goTo[27][T] = 33;
		goTo[27][F] = 21;
		goTo[28][F] = 34;
		goTo[30][BLOCK] = 36;
		goTo[30][STMT] = 37;
		goTo[37][BLOCK] = 47;
		goTo[37][STMT] = 37;
		goTo[46][EXPR] = 16;
		goTo[46][T] = 20;
		goTo[46][F] = 21;
		goTo[49][COND] = 55;
		goTo[50][COND] = 57;
		goTo[52][MOREARGS] = 58;
		goTo[56][C] = 61;
		goTo[62][COND] = 65;
		goTo[64][BLOCK] = 67;
		goTo[64][STMT] = 37;
		goTo[66][BLOCK] = 68;
		goTo[66][STMT] = 37;
		goTo[73][BLOCK] = 74;
		goTo[73][STMT] = 37;
	}
}

int main(int argc, char** argv) {
	
	// input sequence 입력 및 저장
	char input[MAX_SIZE] ;
	if (argc == 2) {
		FILE* fp = fopen(argv[1], "rt");
		if (fp == NULL) {
			printf("file open error");
			exit(1);
		}
		if (fgets(input, MAX_SIZE, fp) != NULL) {
			printf("input sequence : %s\n", input);
		}
		fclose(fp);
	}else {
		printf("This program can read only one file at once.\n");
		printf("Please execute your program in this manner : Parser.exe textfile.txt\n");
		exit(1);
	}

	// 전처리 과정 수행 : _token 및 $ 추가
	char task[500] = "";
	char temp[500];
	char* token = strtok(input, " ");
	while (token != NULL) {
		sprintf(temp, "%s_token ", token);
		strcat(task, temp);
		token = strtok(NULL, " ");
	}
	strcat(task, "$");

	//스택 생성 및 parsing table 구성, 초기 터미널에 대해 포인터 설정
	init();
	char* task_ptr = strtok(task, " ");

	// SLR Table 및 CFG 기반 parsing 수행
	parse(task_ptr);
	
	// 트리 생성 및 출력
	TreeNode* root = popNode();
	printTree(root, 0);
	
	return 0;
}