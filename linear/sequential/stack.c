#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void swap(int *a, int *b){
    int temp = *a;
    *a = *b;
    *b = temp;
}

typedef struct{
    int size;
    int *array;
    int top;
}Stack;

Stack init(int n){
    Stack stack;
    stack.size=n;
    stack.array=(int *)malloc(n*sizeof(int));
    stack.top=-1;
    return stack; 
}

Stack copy(Stack stack){
    Stack result=init(stack.size);
    for (int i = 0; i < stack.top+1; i++)
    {
        result.array[++result.top]=stack.array[i];
    }
    return result;
}

void clear(Stack *stack){
    stack->top=-1;
}

void delete(Stack *vector){
    vector->size=0;
    free(vector->array);
    vector->array=NULL;
    vector->top=-1;
}

bool isEmpty(Stack stack){
    return stack.top==-1;
}

bool isFull(Stack stack){
    return stack.top==stack.size-1;
}

int length(Stack stack){
    return stack.top+1;
}

void push(Stack *stack, int value){
    if(isFull(*stack)==1){
        printf("Stack Overflow\n");
        return;
    }
    stack->array[++stack->top]=value;
}

int peek(Stack stack){
    if(isEmpty(stack)==1){
        printf("Stack Underflow\n");
        return -1;
    }
    return stack.array[stack.top];
}

int pop(Stack *stack){
    if(isEmpty(*stack)==1){
        printf("Stack Underflow\n");
        return -1;
    }
    return stack->array[stack->top--];
}

void traverse(Stack stack){
    printf("[");
    for (int i = 0; i < stack.top+1; i++)
    {
        printf(" %d",stack.array[i]);
    }
    printf(" ] : %d/%d\n",stack.top+1,stack.size);
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
    pop(&stq1);
    traverse(stq1);
    traverse(stq2);
}