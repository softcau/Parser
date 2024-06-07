#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#define MAX_SIZE 100

typedef enum {
	vtype_token, id_token,semi_token, $
}Terminals ;

typedef enum {
	CODE, VDECL
}NonTerminals;

int stack[MAX_SIZE];
int top = -1;
char* action[100][100] ;
int goTo[100][100];

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
	printf("stack display : ");
	for (int i = top; i >= 0; i--) {
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
int main(void) {
	/* 전처리 작업 : 입력받은 문자열 뒤에 _token, $ 추가 */
	char input[100] = "vtype id semi";
	char task[300]="";
	char temp[100];
	char* token;
	token = strtok(input, " ");
	while (token != NULL) {
		sprintf(temp, "%s_token ", token);
		strcat(task, temp);
		token = strtok(NULL, " ");
	}
	strcat(task, "$");
	
	/* 스택 생성 */
	push(0);

	/*action setting*/
	action[0][vtype_token] = "s4";
	action[4][id_token] = "s7";
	action[7][semi_token] = "s9";
	action[9][$] = "r4";
	action[2][$] = "r3";
	action[1][$] = "acc";
	action[5][$] = "r1";

	goTo[0][VDECL] = 2;
	goTo[2][CODE] = 5;
	goTo[0][CODE] = 1;

	/* 초기 터미널에 대해 포인터 설정*/
	char* task_ptr = strtok(task, " ");
	int flag = 0;

	while (flag==0) {
		if (task_ptr == NULL) {
			task_ptr = "$";
		}
		printf("%s\n", task_ptr);
		if (strcmp(task_ptr, "vtype_token")==0) {
				char* actionCode = action[peek()][vtype_token];
				if (actionCode[0] == 's') {
					int next = atoi(actionCode + 1);
					push(next);
				}
		}
		else if (strcmp(task_ptr, "id_token") == 0) {
			char* actionCode = action[peek()][id_token];
			if (actionCode[0] == 's') {
				int next = atoi(actionCode + 1);
				push(next);
			}
		}
		else if (strcmp(task_ptr, "semi_token") == 0) {
			char* actionCode = action[peek()][semi_token];
			if (actionCode[0] == 's') {
				int next = atoi(actionCode + 1);
				push(next);
			}
		}
		else if (strcmp(task_ptr, "$") == 0) {
			char* actionCode = action[stack[top]][$];
			if (actionCode[0] == 's') {
				printf("none");
			}
			else if (actionCode[0] == 'r') {
				int rule = atoi(actionCode + 1);
				if (rule == 4) {
					pop();
					pop();
					pop();
					push(goTo[peek()][VDECL]);
				}
				else if (rule == 3) {
					push(goTo[peek()][CODE]);
				}
				else if (rule == 1) {
					pop();
					pop();
					push(goTo[peek()][CODE]);
				}
			}
			else if (strcmp(actionCode, "acc")==0) {
				printf("accepted\n");
				flag = 1;
				
			}
		}
		task_ptr = strtok(NULL, " ");
	}
	displayStack();
	return 0;
}