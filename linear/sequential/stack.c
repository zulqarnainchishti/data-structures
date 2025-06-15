#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct{
    int *array;
    int top;
    int size;
}Stack;

void swap(int *a, int *b){
    int temp = *a;
    *a = *b;
    *b = temp;
}

Stack init(int n){
    Stack stack;
    stack.array=(int *)malloc(n*sizeof(int));
    stack.top=0;
    stack.size=n;
    return stack; 
}

Stack copy(Stack stack){
    Stack result=init(stack.size);
    for (int i = 0; i < stack.top; i++)
    {
        result.array[result.top++]=stack.array[i];
    }
    return result;
}

void clear(Stack *stack){
    stack->top=0;
}

void delete(Stack *vector){
    free(vector->array);
    vector->array=NULL;
    vector->top=0;
    vector->size=0;
}

bool isEmpty(Stack stack){
    return stack.top==0;
}

bool isFull(Stack stack){
    return stack.top==stack.size;
}

void push(Stack *stack, int value){
    if(isFull(*stack)){
        printf("Stack Overflow\n");
        return;
    }
    stack->array[stack->top++]=value;
}

int peek(Stack stack){
    if(isEmpty(stack)){
        printf("Stack Underflow\n");
        return -1;
    }
    return stack.array[stack.top-1];
}

int pop(Stack *stack){
    if(isEmpty(*stack)){
        printf("Stack Underflow\n");
        return -1;
    }
    return stack->array[--stack->top];
}

void traverse(Stack stack){
    printf("[");
    for (int i = 0; i < stack.top; i++)
    {
        printf(" %d",stack.array[i]);
    }
    printf(" ] : %d/%d\n",stack.top,stack.size);
}

void main()
{
    Stack stq1=init(5);
    push(&stq1,11);
    push(&stq1,22);
    push(&stq1,33);
    push(&stq1,44);
    push(&stq1,55);
    Stack stq2=copy(stq1);
    traverse(stq1);
    traverse(stq2);
    printf("%d\n",pop(&stq1));
    traverse(stq1);
    traverse(stq2);
}