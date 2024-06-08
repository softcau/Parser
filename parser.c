#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX_SIZE 100
//입력받을 토큰 관련
/*
typedef enum {
	VTYPE, NUM, CHARACTER, BOOLSTR, LITERAL, ID, IF, ELSE, WHILE, RETURN, 
	ADDSUB, MULTDIV, ASSIGN, COMP, SEMI, COMMA, LPAREN, RPAREN, LBRACE, RBRACE, $
} Terminals;
*/
typedef enum {
	VTYPE, NUM, CHARACTER, BOOLSTR, LITERAL, ID, ADDSUB, MULTDIV, ASSIGN, COMP, SEMI, COMMA, LPAREN, RPAREN, LBRACE, RBRACE, $
} Terminals;

typedef enum{
	CODE, VDECL, ASSIGN1, RHS, EXPR, FDECL, ARG, MOREARGS, BLOCK, STMT, COND, ELSE,
	RETURN, T, F, C
}NonTerminals;

typedef struct {
	Terminals term;
} Token;

// 스택 구성 및 기능 관련

int stack[MAX_SIZE];
int top = -1;

void push(int element) {
	if (top == (MAX_SIZE - 1)) {
		printf("stack overflow");
		return;
	}
	stack[++top] = element;
}

int pop() {
	if (top == -1) {
		printf("stack underflow");
		return -1;
	}
	return stack[top--];
}

void displayStack() {
	int i;
	printf("stack display(LIFO) : ");
	for (i = top; i >= 0; i--) {
		printf("%d ", stack[i]);
	}
	puts("");
}

int peek() {
	if (top < 0) {
		printf("stack underflow");
		return -1;
	}
	return stack[top];
}

char* action[100][20];
int goTo[100][20];

void parse(Token* tokens) {
	int stack_top;
	int i = 0;
	push(0);
	while (1) {
		stack_top = peek();
		Token token = tokens[i];
		char* actionCode = action[stack_top][token.term];
		
		if (actionCode[0] == 's') { // shift
			int next = atoi(actionCode + 1);
			push(next);
			i++;
		}
		else if (actionCode[0] == 'r') { // reduce
			int rule = atoi(actionCode + 1);
			if (rule == 4) { // VDECL -> vtype id semi
				pop();
				pop();
				pop();
				stack_top = peek();
				push(goTo[stack_top][VDECL]);
			}
			else if (rule == 3) {
				stack_top = peek();
				push(goTo[stack_top][CODE]);
			}
			else if (rule == 1) {
				pop();
				stack_top = peek();
				push(goTo[stack_top][CODE]);
			}
			else {
				printf("error");
				break;
			}
		}
		else if (strcmp(actionCode, "acc") == 0) { // accept
			printf("Parsing completed successfully!\n");
			break;
		}
		else {
			printf("Parsing error occurred!\n");
			break;
		}
	}
}
//int main(int argc, char** argv) {
int main(void){
	/*FILE* file;
	char ch;
	char* buffer;
	long file_size;
	int i=0;

	if (argc < 2) {
		//printf("오류\n", argv[0]);
		return 1;
	}

	file = fopen(argv[1], "r");
	if (file == NULL) {
		printf("파일을 열 수 없습니다: %s\n", argv[1]);
		return 1;
	}

	fseek(file, 0, SEEK_END);
	file_size = ftell(file);
	fseek(file, 0, SEEK_SET);

	buffer = (char*)malloc(file_size + 3);
	if (buffer == NULL) {
		printf("메모리 할당 실패\n");
		fclose(file);
		return 1;
	}

	while ((ch = fgetc(file)) != EOF) {
		buffer[i++] = toupper(ch);
	}
	buffer[i++] = '\0';
	buffer[i++] = '$';
	buffer[i] = '\0';
	fclose(file);
	*/
	//parse(input);
	//free(buffer);
	/*push(0);
	char s1[100] = "VTYPE ID SEMI $";
	char s1_copy[100];
	strcpy(s1_copy, s1); 
	Action[0][VTYPE] = 4;
	char* ptr = strtok(s1_copy, " ");
	while (ptr != NULL) {
		printf("%s\n", ptr);
		Action[top][ptr];
		ptr = strtok(NULL, " ");
	}

	printf("Original s1: %s\n", s1);
	*/

	// Initialize action table
	action[0][VTYPE] = "s4";
	action[4][ID] = "s7";
	action[7][SEMI] = "s9";
	action[9][$] = "r4";
	action[2][$] = "r3";
	action[5][$] = "r1";
	action[1][$] = "acc";
	// Initialize goTo table
	goTo[0][VDECL] = 2;
	goTo[2][CODE] = 5;
	goTo[0][CODE] = 1;

	Terminals input[] = { {VTYPE}, {ID}, {SEMI}, {$} };
	parse(input);
	return 0;
}

vtype_token, num_token, character_token, boolstr_token, literal_token, id_token, if_token,
else_token, while_token, return_token, addsub_token, multdiv_token, assign_token, comp_token,
semi_token, comma_token, lparen_token, rparen_token, lbrace_token, rbrace_token, $