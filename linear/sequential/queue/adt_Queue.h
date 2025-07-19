#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * @brief Represents a Queue data structure.
 * @details This structure holds the array, the front and rear indices, and the maximum capacity of the queue.
 */
typedef struct
{
    int *array;   /**< Pointer to the dynamically allocated array that stores queue elements. */
    int front;    /**< Index of the front element in the queue. */
    int rear;     /**< Index where the next element will be inserted (one past the last element). */
    int capacity; /**< Maximum number of elements the queue can hold. */
} Queue;

/**
 * @brief Initializes a new Queue with a specified capacity.
 * @param capacity The maximum number of elements the queue can hold.
 * @return A new Queue object.
 * @note Exits the program if `capacity` is non-positive or if memory allocation fails.
 */
Queue init(int capacity)
{
    if (capacity <= 0)
    {
        perror("Invalid capacity for Queue");
        exit(EXIT_FAILURE);
    }
    Queue queue;
    queue.array = (int *)malloc(capacity * sizeof(int));
    if (!queue.array)
    {
        perror("Failed to initialize Queue");
        exit(EXIT_FAILURE);
    }
    queue.front = 0;
    queue.rear = 0;
    queue.capacity = capacity;
    return queue;
}

/**
 * @brief Creates a deep copy of an existing Queue.
 * @param queue The Queue to be copied.
 * @return A new Queue containing the same elements as the original.
 */
Queue copy(const Queue queue)
{
    Queue copied = init(queue.capacity);
    for (int i = queue.front; i < queue.rear; i++)
        copied.array[copied.rear++] = queue.array[i];
    return copied;
}

/**
 * @brief Clears all elements from the Queue, making it empty.
 * @param queue A pointer to the Queue to be cleared.
 * @note This function does not free the allocated memory; it just resets the front and rear pointers.
 */
void clear(Queue *queue)
{
    queue->front = 0;
    queue->rear = 0;
}

/**
 * @brief Deallocates the memory used by the Queue.
 * @param vector A pointer to the Queue to be destroyed.
 * @note After calling this function, the queue pointer should not be used.
 */
void destroy(Queue *vector)
{
    free(vector->array);
    vector->array = NULL;
    vector->front = 0;
    vector->rear = 0;
    vector->capacity = 0;
}

/**
 * @brief Checks if the Queue is empty.
 * @param queue The Queue to check.
 * @return True if the queue is empty, false otherwise.
 */
bool isEmpty(const Queue queue)
{
    return queue.front == queue.rear;
}

/**
 * @brief Checks if the Queue is full.
 * @param queue The Queue to check.
 * @return True if the queue is full, false otherwise.
 */
bool isFull(const Queue queue)
{
    return queue.rear == queue.capacity;
}

/**
 * @brief Returns the current number of elements in the Queue.
 * @param queue The Queue to query.
 * @return The number of elements currently in the queue.
 */
int length(const Queue queue)
{
    return queue.rear - queue.front;
}

/**
 * @brief Adds a new element to the rear of the Queue.
 * @param queue A pointer to the Queue to enqueue the element into.
 * @param value The integer value to be enqueued.
 * @note Prints "Queue Overflow" if the queue is full.
 */
void enqueue(Queue *queue, int value)
{
    if (isFull(*queue))
    {
        printf("Queue Overflow\n");
        return;
    }
    queue->array[queue->rear++] = value;
}

/**
 * @brief Returns the front element of the Queue without removing it.
 * @param queue The Queue to peek at.
 * @return The value of the front element, or -1 if the queue is empty (and prints "Queue Underflow").
 */
int peek(const Queue queue)
{
    if (isEmpty(queue))
    {
        printf("Queue Underflow\n");
        return -1;
    }
    return queue.array[queue.front];
}

/**
 * @brief Removes and returns the front element from the Queue.
 * @param queue A pointer to the Queue to dequeue the element from.
 * @return The value of the dequeued element, or -1 if the queue is empty (and prints "Queue Underflow").
 */
int dequeue(Queue *queue)
{
    if (isEmpty(*queue))
    {
        printf("Queue Underflow\n");
        return -1;
    }
    return queue->array[queue->front++];
}

/**
 * @brief Traverses and prints the elements of the Queue from front to rear.
 * @param queue The Queue to traverse.
 * @details Elements are printed in the format "[ el1 el2 ... ] : current_length/capacity".
 */
void traverse(const Queue queue)
{
    int len = length(queue);
    printf("[");
    for (int i = queue.front; i < queue.rear; i++)
    {
        printf(" %d", queue.array[i]);
    }
    printf(" ] : %d/%d\n", len, queue.capacity);
}

#endif // QUEUE_H