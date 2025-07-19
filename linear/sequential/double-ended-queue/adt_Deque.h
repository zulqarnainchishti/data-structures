#ifndef DEQUE_H
#define DEQUE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * @brief Represents a Double-Ended Queue (Deque) data structure.
 */
typedef struct
{
    int *array;   /**< Pointer to the dynamically allocated array. */
    int front;    /**< Index of the front element. */
    int rear;     /**< Index where the next element will be inserted at the rear. */
    int capacity; /**< Maximum number of elements the deque can hold. */
    int length;   /**< Current number of elements in the deque. */
} Deque;

/**
 * @brief Initializes a new Deque.
 * @param capacity The maximum number of elements the deque can hold.
 * @return A new Deque instance.
 */
Deque init(int capacity)
{
    if (capacity <= 0)
    {
        perror("Invalid capacity for Deque");
        exit(EXIT_FAILURE);
    }
    Deque queue;
    queue.array = (int *)malloc(capacity * sizeof(int));
    if (!queue.array)
    {
        perror("Failed to initialize Deque");
        exit(EXIT_FAILURE);
    }
    queue.front = capacity / 2;
    queue.rear = capacity / 2;
    queue.capacity = capacity;
    queue.length = 0;
    return queue;
}

/**
 * @brief Creates a deep copy of an existing Deque.
 * @param queue The Deque to be copied.
 * @return A new Deque instance.
 */
Deque copy(const Deque queue)
{
    Deque result = init(queue.capacity);
    result.front = queue.front;
    result.rear = queue.rear;
    result.length = queue.length;
    for (int i = queue.front; i < queue.rear; i++)
    {
        result.array[i] = queue.array[i];
    }
    return result;
}

/**
 * @brief Clears all elements from the Deque.
 * @param queue A pointer to the Deque to be cleared.
 */
void clear(Deque *queue)
{
    queue->front = queue->capacity / 2;
    queue->rear = queue->capacity / 2;
    queue->length = 0;
}

/**
 * @brief Deallocates the memory used by the Deque.
 * @param vector A pointer to the Deque to be destroyed.
 */
void destroy(Deque *vector)
{
    free(vector->array);
    vector->array = NULL;
    vector->front = 0;
    vector->rear = 0;
    vector->capacity = 0;
    vector->length = 0;
}

/**
 * @brief Checks if the Deque is empty.
 * @param queue The Deque to check.
 * @return true if the deque is empty, false otherwise.
 */
bool isEmpty(const Deque queue)
{
    return queue.length == 0;
}

/**
 * @brief Checks if the front end of the Deque is full.
 * @param queue The Deque to check.
 * @return true if the front end is full, false otherwise.
 */
bool isFrontFull(const Deque queue)
{
    return queue.front == 0;
}

/**
 * @brief Checks if the rear end of the Deque is full.
 * @param queue The Deque to check.
 * @return true if the rear end is full, false otherwise.
 */
bool isRearFull(const Deque queue)
{
    return queue.rear == queue.capacity;
}

/**
 * @brief Adds an element to the front of the Deque.
 * @param queue A pointer to the Deque.
 * @param value The integer value to be added.
 */
void enqueueFront(Deque *queue, int value)
{
    if (isFrontFull(*queue))
    {
        printf("Deque Overflow\n");
        return;
    }
    queue->array[--queue->front] = value;
    queue->length++;
}

/**
 * @brief Returns the element at the front of the Deque without removing it.
 * @param queue The Deque to peek at.
 * @return The value of the front element, or -1 if the deque is empty.
 */
int peekFront(const Deque queue)
{
    if (isEmpty(queue))
    {
        printf("Deque Underflow\n");
        return -1;
    }
    return queue.array[queue.front];
}

/**
 * @brief Removes and returns the element from the front of the Deque.
 * @param queue A pointer to the Deque.
 * @return The value of the dequeued element, or -1 if the deque is empty.
 */
int dequeueFront(Deque *queue)
{
    if (isEmpty(*queue))
    {
        printf("Deque Underflow\n");
        return -1;
    }
    int value = queue->array[queue->front++];
    queue->length--;
    if (queue->length == 0)
    {
        queue->front = queue->capacity / 2;
        queue->rear = queue->capacity / 2;
    }
    return value;
}

/**
 * @brief Adds an element to the rear of the Deque.
 * @param queue A pointer to the Deque.
 * @param value The integer value to be added.
 */
void enqueueRear(Deque *queue, int value)
{
    if (isRearFull(*queue))
    {
        printf("Deque Overflow\n");
        return;
    }
    queue->array[queue->rear++] = value;
    queue->length++;
}

/**
 * @brief Returns the element at the rear of the Deque without removing it.
 * @param queue The Deque to peek at.
 * @return The value of the rear element, or -1 if the deque is empty.
 */
int peekRear(const Deque queue)
{
    if (isEmpty(queue))
    {
        printf("Deque Underflow\n");
        return -1;
    }
    return queue.array[queue.rear - 1];
}

/**
 * @brief Removes and returns the element from the rear of the Deque.
 * @param queue A pointer to the Deque.
 * @return The value of the dequeued element, or -1 if the deque is empty.
 */
int dequeueRear(Deque *queue)
{
    if (isEmpty(*queue))
    {
        printf("Deque Underflow\n");
        return -1;
    }
    int value = queue->array[--queue->rear];
    queue->length--;
    if (queue->length == 0)
    {
        queue->front = queue->capacity / 2;
        queue->rear = queue->capacity / 2;
    }
    return value;
}

/**
 * @brief Prints the elements of the Deque from front to rear.
 * @param queue The Deque to traverse and print.
 */
void traverse(const Deque queue)
{
    printf("[");
    for (int i = queue.front; i < queue.rear; i++)
    {
        printf(" %d", queue.array[i]);
    }
    printf(" ] : %d/%d\n", queue.length, queue.capacity);
}

#endif // DEQUE_H