#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * @brief Represents a Stack data structure.
 * @details This structure holds the array, the index of the top element, and the maximum capacity of the stack.
 */
typedef struct
{
    int *array;   /**< Pointer to the dynamically allocated array that stores stack elements. */
    int top;      /**< Index of the top element in the stack. Points to the next available position. */
    int capacity; /**< Maximum number of elements the stack can hold. */
} Stack;

/**
 * @brief Initializes a new stack with a specified capacity.
 * @param capacity The maximum number of elements the stack can hold.
 * @return A new Stack object.
 */
Stack init(const int capacity)
{
    if (capacity <= 0)
    {
        perror("Invalid capacity for Stack");
        exit(EXIT_FAILURE);
    }
    Stack stack;
    stack.array = (int *)malloc(capacity * sizeof(int));
    if (stack.array == NULL)
    {
        perror("Failed to initialize Stack");
        exit(EXIT_FAILURE);
    }
    stack.top = 0;
    stack.capacity = capacity;
    return stack;
}

/**
 * @brief Creates a deep copy of an existing stack.
 * @param stack The stack to be copied.
 * @return A new stack containing the same elements as the original.
 */
Stack copy(const Stack stack)
{
    Stack copied = init(stack.capacity);
    for (int i = 0; i < stack.top; i++)
        copied.array[copied.top++] = stack.array[i];
    return copied;
}

/**
 * @brief Clears all elements from the stack, making it empty.
 * @param stack A pointer to the stack to be cleared.
 */
void clear(Stack *stack)
{
    stack->top = 0;
}

/**
 * @brief Deallocates the memory used by the stack.
 * @param vector A pointer to the stack to be deleted.
 * @note After calling this function, the stack pointer should not be used.
 */
void delete(Stack *vector)
{
    free(vector->array);
    vector->array = NULL;
    vector->top = 0;
    vector->capacity = 0;
}

/**
 * @brief Checks if the stack is empty.
 * @param stack The stack to check.
 * @return True if the stack is empty, false otherwise.
 */
bool isEmpty(const Stack stack)
{
    return stack.top == 0;
}

/**
 * @brief Checks if the stack is full.
 * @param stack The stack to check.
 * @return True if the stack is full, false otherwise.
 */
bool isFull(const Stack stack)
{
    return stack.top == stack.capacity;
}

/**
 * @brief Pushes a new element onto the top of the stack.
 * @param stack A pointer to the stack to push the element onto.
 * @param value The integer value to be pushed.
 * @note Prints "Stack Overflow" if the stack is full.
 */
void push(Stack *stack, const int value)
{
    if (isFull(*stack))
    {
        printf("Stack Overflow\n");
        return;
    }
    stack->array[stack->top++] = value;
}

/**
 * @brief Returns the top element of the stack without removing it.
 * @param stack The stack to peek at.
 * @return The value of the top element, or -1 if the stack is empty (and prints "Stack Underflow").
 */
int peek(const Stack stack)
{
    if (isEmpty(stack))
    {
        printf("Stack Underflow\n");
        return -1;
    }
    return stack.array[stack.top - 1];
}

/**
 * @brief Removes and returns the top element from the stack.
 * @param stack A pointer to the stack to pop the element from.
 * @return The value of the popped element, or -1 if the stack is empty (and prints "Stack Underflow").
 */
int pop(Stack *stack)
{
    if (isEmpty(*stack))
    {
        printf("Stack Underflow\n");
        return -1;
    }
    return stack->array[--stack->top];
}

/**
 * @brief Traverses and prints the elements of the stack from bottom to top.
 * @param stack The stack to traverse.
 */
void traverse(const Stack stack)
{
    printf("[");
    for (int i = 0; i < stack.top; i++)
    {
        printf(" %d", stack.array[i]);
    }
    printf(" ] : %d/%d\n", stack.top, stack.capacity);
}

#endif // STACK_H