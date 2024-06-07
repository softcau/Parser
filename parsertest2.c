#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 100

// Terminal �� Non-terminal ������ ����
typedef enum {
    vtype_token, id_token, semi_token, $
} Terminals;

typedef enum {
    CODE, VDECL
} NonTerminals;

// ���� �� top ���� ����
int stack[MAX_SIZE];
int top = -1;

// action �� goTo �迭 ����
char* action[100][100];
int goTo[100][100];

int main(void) {
    // ��ó�� �۾� : �Է¹��� ���ڿ� �ڿ� _token, $ �߰�
    char input[100] = "vtype id semi";
    char task[300] = "";
    char temp[100];
    char* token;

    // �Է� ���ڿ� ó��
    token = strtok(input, " ");
    while (token != NULL) {
        sprintf(temp, "%s_token ", token);
        strcat(task, temp);
        token = strtok(NULL, " ");
    }
    strcat(task, "$");

    // ���� �ʱ�ȭ
    push(0);

    // action �� goTo ����
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

    // �Ľ� ����
    parseInput(task);

    return 0;
}

// ���� ���� �Լ��� ����
void push(int element) {
    if (top == (MAX_SIZE - 1)) {
        printf("stack overflow");
        exit(1);
    }
    stack[++top] = element;
}

int pop() {
    if (top == -1) {
        printf("stack underflow");
        exit(1);
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
        exit(1);
    }
    return stack[top];
}

// �Ľ� �Լ� ����
void parseInput(char* input) {
    char* task_ptr = strtok(input, " ");
    int flag = 0;

    while (flag == 0) {
        if (task_ptr == NULL) {
            task_ptr = "$";
        }
        printf("%s\n", task_ptr);

        // ���� �͹̳ο� ���� ���� ����
        if (strcmp(task_ptr, "$") == 0) {
            char* actionCode = action[stack[top]][$];
            if (actionCode[0] == 's') {
                printf("none");
            }
            else if (actionCode[0] == 'r') {
                int rule = atoi(actionCode + 1);
                reduce(rule);
            }
            else if (strcmp(actionCode, "acc") == 0) {
                printf("accepted\n");
                flag = 1;
            }
        }
        else {
            int terminal;
            if (strcmp(task_ptr, "vtype_token") == 0)
                terminal = vtype_token;
            else if (strcmp(task_ptr, "id_token") == 0)
                terminal = id_token;
            else if (strcmp(task_ptr, "semi_token") == 0)
                terminal = semi_token;

            char* actionCode = action[peek()][terminal];
            if (actionCode[0] == 's') {
                int nextState = atoi(actionCode + 1);
                shift(nextState);
            }
        }
        task_ptr = strtok(NULL, " ");
    }
    displayStack();
}

// Shift ���� ����
void shift(int nextState) {
    push(nextState);
}

// Reduce ���� ����
void reduce(int rule) {
    switch (rule) {
    case 1:
        pop();
        pop();
        push(goTo[peek()][CODE]);
        break;
    case 3:
        push(goTo[peek()][CODE]);
        break;
    case 4:
        pop();
        pop();
        pop();
        push(goTo[peek()][VDECL]);
        break;
    default:
        printf("Invalid reduction rule.\n");
        exit(1);
    }
}
