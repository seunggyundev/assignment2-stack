#include "stack.h"

void init_stack(StackType* s) {
    s->capacity = MAX_STACK_SIZE;
    s->data = (element*)malloc(s->capacity * sizeof(element));
    if (s->data == NULL) {
        fprintf(stderr, "메모리 allocation 실패.\n");
        exit(EXIT_FAILURE);
    }
    s->top = -1;
}

int is_empty(StackType* s) {
    return s->top == -1;
}

int is_full(StackType* s) {
    return s->top == s->capacity - 1;
}

void push(StackType* s, element item) {
    if (is_full(s)) {
        fprintf(stderr, "스택이 찼습니다.\n");
        exit(EXIT_FAILURE);
    }
    s->data[++s->top] = item;
}

element pop(StackType* s) {
    if (is_empty(s)) {
        fprintf(stderr, "스택이 비었습니다.\n");
        exit(EXIT_FAILURE);
    }
    return s->data[s->top--];
}

element peek(StackType* s) {
    if (is_empty(s)) {
        fprintf(stderr, "스택이 비었습니다.\n");
        exit(EXIT_FAILURE);
    }
    return s->data[s->top];
}

int prec(char op) {
    switch (op) {
    case '+': case '-': return 1;
    case '*': case '/': return 2;
    default: return 0;  // '(' or 알 수 없는 characters
    }
}

void free_stack(StackType* s) {
    free(s->data);
}
