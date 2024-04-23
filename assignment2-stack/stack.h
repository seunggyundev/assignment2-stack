#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h> 

#define MAX_STACK_SIZE 100
typedef int element;

typedef struct {
    element* data;
    int capacity;
    int top;
} StackType;

void init_stack(StackType* s);
int is_empty(StackType* s);
int is_full(StackType* s);
void push(StackType* s, element item);
element pop(StackType* s);
int prec(char op);
void free_stack(StackType* s);
element peek(StackType* s);
#endif
