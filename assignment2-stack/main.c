#define _CRT_SECURE_NO_WARNINGS 
#define _CRT_NONSTDC_NO_DEPRECATE
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <malloc.h>
#include "stack.h"

#define MAX_EXPRESSIONS 10
#define MAX_TERMS       100
#define DELIMITERS      " \t\n"

int read_expressions(char* fname, char* expressions[]);
void print_strings(char* s[], int n);
int split_terms(char* expression, char** terms);
int infix_to_postfix(char** terms, char** post, int n_terms);
int eval(char** post, int n_post);
void free_strings(char** s, int n);

int main(int argc, char* argv[1]) {
	char* fname = argv[1];
	char* expressions[MAX_EXPRESSIONS];
	int n_expressions = read_expressions(fname, expressions);
	print_strings(expressions, n_expressions);

	char* terms[MAX_TERMS];
	char* post[MAX_TERMS];
	for (int i = 0; i < n_expressions; i++) {
		printf("중위표시수식 %s \n", expressions[i]);
		int n_terms = split_terms(expressions[i], terms);
		print_strings(terms, n_terms);
		int n_post = infix_to_postfix(terms, post, n_terms);
		int result = eval(post, n_post);
		printf("계산 결과 = %d\n", result);
		free_strings(terms, n_terms);
		free_strings(post, n_post);
	}
	free_strings(expressions, n_expressions);
	return 0;
}


int read_expressions(char* fname, char* expressions[]) {
	FILE* fp = fopen(fname, "r");
	if (fp == NULL) {
		fprintf(stderr, "파일을 열 수 없습니다. %s\n", fname);
		exit(EXIT_FAILURE);
	}
	char buf[BUFSIZ];
	int i = 0;
	while (fgets(buf, BUFSIZ, fp) != NULL) {
		expressions[i] = (char*)malloc(strlen(buf) + 1);
		strcpy(expressions[i], buf);
		i++;
	}
	fclose(fp);
	return i;
}

void print_strings(char* s[], int n) {
	for (int i = 0; i < n; i++)
		printf("%s\n", s[i]);
}

int split_terms(char* expression, char* terms[]) {
	//printf("%s\n", expression);
	int i = 0;
	char* ptr = strtok(expression, DELIMITERS);
	while (ptr != NULL) {
		terms[i] = (char*)malloc(strlen(ptr) + 1);
		strcpy(terms[i], ptr);
		ptr = strtok(NULL, DELIMITERS);
		i++;
	}
	return i;
}

// 중위 표현식을 후위 표현식으로 변환하는 함수
int infix_to_postfix(char* terms[], char* post[], int n_terms) {
	StackType s; // 스택을 초기화
	init_stack(&s);
	int j = 0; // 후위 표현식 배열의 인덱스

	// 각 용어에 대해 반복
	for (int i = 0; i < n_terms; i++) {
		char* term = terms[i];
		if (isdigit(term[0])) { // 숫자라면 후위 표현식에 추가
			post[j++] = strdup(term);
		}
		else if (term[0] == '(') { // 여는 괄호 '(' 를 만나면 스택에 푸시
			push(&s, (int)'(');
		}
		else if (term[0] == ')') { // 닫는 괄호 ')' 를 만나면 '('가 나올 때까지 스택에서 팝하여 후위 표현식에 추가
			while (!is_empty(&s) && peek(&s) != (int)'(') {
				post[j++] = (char*)malloc(2); // 메모리 할당
				post[j - 1][0] = (char)pop(&s);
				post[j - 1][1] = '\0'; // 문자열 종료
			}
			pop(&s); // 여는 괄호 '(' 제거
		}
		else { // 연산자를 만나면
			// 스택의 top에 있는 연산자의 우선순위가 현재 연산자보다 높거나 같을 때까지 팝하여 후위 표현식에 추가
			while (!is_empty(&s) && prec((char)peek(&s)) >= prec(term[0])) {
				post[j++] = (char*)malloc(2); // 메모리 할당
				post[j - 1][0] = (char)pop(&s);
				post[j - 1][1] = '\0'; // 문자열 종료
			}
			push(&s, term[0]); // 현재 연산자를 스택에 푸시
		}
	}

	// 스택에 남아 있는 모든 연산자를 팝하여 후위 표현식에 추가
	while (!is_empty(&s)) {
		post[j++] = (char*)malloc(2); // 메모리 할당
		post[j - 1][0] = (char)pop(&s);
		post[j - 1][1] = '\0'; // 문자열 종료
	}

	free_stack(&s); // 스택 메모리 해제
	return j; // 변환된 후위 표현식의 길이 반환
}

// 후위 표현식을 계산하는 함수
int eval(char* post[], int n_post) {
	StackType s; // 스택을 초기화
	init_stack(&s);
	int value; // 계산된 값을 임시로 저장할 변수

	// 후위 표현식을 순회하며 계산
	for (int i = 0; i < n_post; i++) {
		char* token = post[i];
		if (isdigit(token[0])) { // 토큰이 숫자라면
			value = atoi(token); // 문자열을 정수로 변환하고 스택에 푸시
			push(&s, value);
		}
		else { // 토큰이 연산자라면
			int op2 = pop(&s); // 두 개의 피연산자를 스택에서 팝
			int op1 = pop(&s);
			switch (token[0]) { // 연산자에 따라 계산
			case '+': value = op1 + op2; break;
			case '-': value = op1 - op2; break;
			case '*': value = op1 * op2; break;
			case '/': value = op1 / op2; break;
			}
			push(&s, value); // 계산 결과를 스택에 푸시
		}
	}

	value = pop(&s); // 계산 결과는 스택의 마지막 요소여야 함
	free_stack(&s); // 스택 메모리 해제
	return value; // 최종 계산 결과 반환
}


void free_strings(char* s[], int n) {
	for (int i = 0; i < n; i++) {
		if (s[i] != NULL) {  // 포인터가 NULL이 아닐 때만 free
			free(s[i]);
			s[i] = NULL;  // 포인터를 NULL로 설정하여 이중 해제 방지
		}
	}
}