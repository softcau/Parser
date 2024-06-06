#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 100
//입력받을 토큰 관련
typedef enum {
	VTYPE, NUM, CHARACTER, BOOLSTR, LITERAL, ID, IF, ELSE, WHILE, RETURN, 
	ADDSUB, MULTDIV, ASSIGN, COMP, SEMI, COMMA, LPAREN, RPAREN, LBRACE, RBRACE, $
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
	printf("stack display(LIFO)");
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

//파싱 테이블 관련
int Action[100][25];
int Goto[100][25];

//파싱 테이블 초기화
void initParsingTable() {
	memset(Action, 0, sizeof(Action));
	memset(Goto, 0, sizeof(Goto));
	
	Action[0][VTYPE] = 4;
	Action[2][VTYPE] = 4;
	Action[3][VTYPE] = 4;

	Action[0][$] = r3;
	Action[1][$] = acc;
	Action[2][$] = r3;
	Action[3][$] = r3;
	Action[5][$] = r1;
	Action[6][$] = r2;
	Action[7][SEMI] = 9;	
	Action[7][ASSIGN] = 11;						
	Action[7][LPAREN] = 10;
	Action[8][SEMI] = 12;
	Action[9][VTYPE] = r4;
	Action[4][ID] = 7;
	Action[9][ID] = r4;												
	Action[9][RBRACE] = r4;		
	Action[9][IF] = r4;	
	Action[9][WHILE] = r4;			
	Action[9][RETURN] = r4;	
	Action[9][$] = r4;
	Action[10][VTYPE] = 14;										
	Action[10][RPAREN] = r20;
	Action[11][ID] = 23;		
	Action[11][LITERAL] = 17;	
	Action[11][CHARACTER] = 18;	
	Action[11][BOOLSTR] = 19;			
	Action[11][LPAREN] = 22;		
	Action[11][NUM] = 24;
	Action[12][VTYPE] = r5;	
	Action[12][ID] = r5;												
	Action[12][RBRACE] = r5;		
	Action[12][IF] = r5;	
	Action[12][WHILE] = r5;			
	Action[12][RETURN] = r5;	
	Action[12][$] = r5;
	Action[13][RPAREN] = 25;
	Action[14][ID] = 26;
	Action[15][SEMI] = r6;
	Action[16][SEMI] = r7;			
	Action[16][ADDSUB] = 27;
	Action[17][SEMI] = r8;
	Action[18][SEMI] = r9;
	Action[19][SEMI] = r10;
	Action[20][SEMI] = r12;					
	Action[20][ADDSUB] = r12;	
	Action[20][MULTDIV] = 28;		
	Action[20][RPAREN] = r12;
	Action[21][SEMI] = r14;					
	Action[21][ADDSUB] = r14;	
	Action[21][MULTDIV] = r14;		
	Action[21][RPAREN] = r14;
	Action[22][ID] = 23;								
	Action[22][LPAREN] = 22;		
	Action[22][NUM] = 24;
	Action[23][SEMI] = r16;					
	Action[23][ADDSUB] = r16;	
	Action[23][MULTDIV] = r16;		
	Action[23][RPAREN] = r16;
	Action[24][SEMI] = r17;					
	Action[24][ADDSUB] = r17;	
	Action[24][MULTDIV] = r17;		
	Action[24][RPAREN] = r17;
	Action[25][LBRACE] = 30;
	Action[26][RPAREN] = r22;				
	Action[26][COMMA] = 32;
	Action[27][ID] = 23;								
	Action[27][LPAREN] = 22;		
	Action[27][NUM] = 24;
	Action[28][ID] = 23;								
	Action[28][LPAREN] = 22;		
	Action[28][NUM] = s24;
	Action[29][ADDSUB] = 27;			
	Action[29][RPAREN] = 35;
	Action[30][VTYPE] = 42;	
	Action[30][ID] = 43;	
	Action[30][RBRACE] = r24;		
	Action[30][IF] = 40;	
	Action[30][WHILE] = 41;			
	Action[30][RETURN] = r24;
	Action[31][RPAREN] = r19;
	Action[32][VTYPE] = 44;
	Action[33][SEMI] = r11;					
	Action[33][ADDSUB] = r11;	
	Action[33][MULTDIV] = 28;		
	Action[33][RPAREN] = r11;
	Action[34][SEMI] = r13;					
	Action[34][ADDSUB] = r13;	
	Action[34][MULTDIV] = r13;		
	Action[34][RPAREN] = r13;
	Action[35][SEMI] = r15;					
	Action[35][ADDSUB] = r15;	
	Action[35][MULTDIV] = r15;		
	Action[35][RPAREN] = r15;
	Action[36][RETURN] = 46;
	Action[37][VTYPE] = 42;	
	Action[37][ID] = 43;												
	Action[37][RBRACE] = r24;		
	Action[37][IF] = 40;	
	Action[37][WHILE] = 41;			
	Action[37][RETURN] = r24;
	Action[38][VTYPE] = r25;	
	Action[38][ID] = r25;											
	Action[38][RBRACE] = r25;		
	Action[38][IF] = r25;	
	Action[38][WHILE] = r25;			
	Action[38][RETURN] = r25;
	Action[39][SEMI] = 48;
	Action[40][LPAREN] = 49;
	Action[41][LPAREN] = 50;
	Action[42][ID] = 51;
	Action[43][ASSIGN] = 11;
	Action[44][ID] = 52;
	Action[45][RBRACE] = 53;
	Action[46][ID] = 23;			
	Action[46][LITERAL] = 17;	
	Action[46][CHARACTER] = 18;	
	Action[46][BOOLSTR] = 19;			
	Action[46][LPAREN] = 22;		
	Action[46][NUM] = 24;
	Action[47][RBRACE] = r23;						
	Action[47][RETURN] = r23;
	Action[48][VTYPE] = r26;	
	Action[48][ID] = r26;												
	Action[48][RBRACE] = r26;		
	Action[48][IF] = r26;	
	Action[48][WHILE] = r26;			
	Action[48][RETURN] = r26;
	Action[49][BOOLSTR] = 56;
	Action[50][BOOLSTR] = 56;
	Action[51][SEMI] = 9;	
	Action[51][ASSIGN] = 11;
	Action[52][RPAREN] = r22;				
	Action[52][COMMA] = 32;
	Action[53][VTYPE] = r18;																				
	Action[53][$] = r18;
	Action[54][SEMI] = 59;
	Action[55][RPAREN] = 60;
	Action[56][RPAREN] = r31;							
	Action[56][COMP] = 62;
	Action[57][RPAREN] = 63;
	Action[58][RPAREN] = r21;
	Action[59][RBRACE] = r34;
	Action[60][LBRACE] = 64;
	Action[61][RPAREN] = r29;
	Action[62][BOOLSTR] = 56;
	Action[63][LBRACE] = 66;
	Action[64][VTYPE] = 42;	
	Action[64][ID] = 43;												
	Action[64][RBRACE] = r24;		
	Action[64][IF] = 40;	
	Action[64][WHILE] = 41;			
	Action[64][RETURN] = r24;
	Action[65][RPAREN] = r30;
	Action[66][VTYPE] = 42;	
	Action[66][ID] = 43;	
	Action[66][RBRACE] = r24;		
	Action[66][IF] = 40;	
	Action[66][WHILE] = 41;			
	Action[66][RETURN] = r24;
	Action[67][RBRACE] = 69;
	Action[68][RBRACE] = 70;
	Action[69][VTYPE] = r33;	
	Action[69][ID] = r33;	
	Action[69][RBRACE] = r33;		
	Action[69][IF] = r33;	
	Action[69][WHILE] = r33;		
	Action[69][ELSE] = 72;	
	Action[69][RETURN] = r33;
	Action[70][VTYPE] = r28;	
	Action[70][ID] = r28;	 
	Action[70][RBRACE] = r28;		
	Action[70][IF] = r28;	
	Action[70][WHILE] = r28;			
	Action[70][RETURN] = r28;
	Action[71][VTYPE] = r27;	
	Action[71][ID] = r27; 
	Action[71][RBRACE] = r27;		
	Action[71][IF] = r27;	
	Action[71][WHILE] = r27;			
	Action[71][RETURN] = r27;
	Action[72][LBRACE] = 73;
	Action[73][VTYPE] = 42;	
	Action[73][ID] = 43; 
	Action[73][RBRACE] = r24;		
	Action[73][IF] = 40;	
	Action[73][WHILE] = 41;
	Action[73][RETURN] = r24;
	Action[74][RBRACE] = 75;
	Action[75][VTYPE] = r32;	
	Action[75][ID] = r32;	
	Action[75][RBRACE] = r32;	
	Action[75][IF] = r32;	
	Action[75][WHILE] = r32; 
	Action[75][RETURN] = r32;

	Goto[0][CODE] = 1;	
	Goto[0][VDECL] = 2;	
	Goto[4][ASSIGN1] = 8;					
	Goto[0][FDECL] = 3;							
	Goto[69][ELSE] = 71;	
	Goto[36][RETURN] = 45;
	Goto[2][CODE] = 5;	
	Goto[2][VDECL] = 2;	
	Goto[30][ASSIGN1] = 39;
	Goto[3][CODE] = 6;	
	Goto[3][VDECL] = 2;	
	Goto[37][ASSIGN1] = 39;					
	Goto[2][FDECL] = 3;
	Goto[30][VDECL] = 38;						
	Goto[3][FDECL] = 3;
	Goto[37][VDECL] = 38;
	Goto[64][VDECL] = 38;
	Goto[66][VDECL] = 38;	
	Goto[42][ASSIGN1] = 8;
	Goto[64][ASSIGN1] = 39;
	Goto[66][ASSIGN1] = 39;						
	Goto[10][ARG] = 13;
	Goto[11][RHS] = 15;	
	Goto[11][EXPR] = 16;	
	Goto[11][T] = 20;	
	Goto[11][F] = 21;
	Goto[46][RHS] = 54;
	Goto[73][VDECL] = 38;
	Goto[73][ASSIGN1] = 39;
	Goto[22][EXPR] = 29;	
	Goto[22][T] = 20;	
	Goto[22][F] = 21;
	Goto[26][MOREARGS] = 31;
	Goto[27][T] = 33;	
	Goto[27][F] = 21;
	Goto[28][F] = 34;
	Goto[30][BLOCK] = 36;	
	Goto[30][STMT] = 37;
	Goto[37][BLOCK] = 47;	
	Goto[37][STMT] = 37;
	Goto[46][EXPR] = 16;	
	Goto[46][T] = 20;	
	Goto[46][F] = 21;
	Goto[49][COND] = 55;
	Goto[50][COND] = 57;
	Goto[52][MOREARGS] = 58;
	Goto[56][C] = 61;
	Goto[62][COND] = 65;
	Goto[64][BLOCK] = 67;	
	Goto[64][STMT] = 37;
	Goto[66][BLOCK] = 68;	
	Goto[66][STMT] = 37;
	Goto[73][BLOCK] = 74;	
	Goto[73][STMT] = 37;
	
	/* parsing table 구현*/

}

	

	

int main(int argc, char** argv) {

	initParsingTable();
	FILE* file;
	char ch;
	char* buffer;
	long file_size;
	int i=0;

	if (argc < 2) {
		printf("오류\n", argv[0]);
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

	/*sequence input 구현*/

	Token input[] = { VTYPE, {ID}, {SEMI} };

	/*stack inout 구현*/
	parse(input);
	push(0);

	free(buffer);
	
	return 0;
}


