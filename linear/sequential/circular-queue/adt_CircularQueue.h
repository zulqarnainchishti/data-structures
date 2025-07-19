#ifndef CIRCULAR_QUEUE_H
#define CIRCULAR_QUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * @brief Represents a Circular Queue data structure.
 * @details This structure holds the array, the front and rear indices, and the effective capacity of the queue.
 * The capacity is internally stored as `user_capacity + 1` to differentiate between a full and an empty queue.
 */
typedef struct
{
    int *array;   /**< Pointer to the dynamically allocated array that stores queue elements. */
    int front;    /**< Index of the front element in the queue. */
    int rear;     /**< Index where the next element will be inserted (one past the last element in logical order). */
    int capacity; /**< The internal storage capacity (actual_capacity + 1). */
} CircularQueue;

/**
 * @brief Initializes a new CircularQueue with a specified logical capacity.
 * @param capacity The maximum number of elements the circular queue can logically hold.
 * @return A new CircularQueue object.
 * @note Exits the program if `capacity` is non-positive or if memory allocation fails.
 */
CircularQueue init(int capacity)
{
    if (capacity <= 0)
    {
        perror("Invalid capacity for CircularQueue");
        exit(EXIT_FAILURE);
    }
    CircularQueue queue;
    // Allocate one extra space to differentiate between full and empty states
    queue.array = (int *)malloc((capacity + 1) * sizeof(int));
    if (!queue.array)
    {
        perror("Failed to initialize CircularQueue");
        exit(EXIT_FAILURE);
    }
    queue.front = 0;
    queue.rear = 0;
    queue.capacity = capacity + 1; // Store internal capacity (user_capacity + 1)
    return queue;
}

/**
 * @brief Creates a deep copy of an existing CircularQueue.
 * @param queue The CircularQueue to be copied.
 * @return A new CircularQueue containing the same elements as the original.
 */
CircularQueue copy(const CircularQueue queue)
{
    // Initialize with the logical capacity (queue.capacity - 1)
    CircularQueue copied = init(queue.capacity - 1);
    int curr = queue.front;
    while (curr != queue.rear)
    {
        copied.array[copied.rear++] = queue.array[curr];
        curr = (curr + 1) % queue.capacity;
    }
    return copied;
}

/**
 * @brief Clears all elements from the CircularQueue, making it empty.
 * @param queue A pointer to the CircularQueue to be cleared.
 * @note This function does not free the allocated memory; it just resets the front and rear pointers.
 */
void clear(CircularQueue *queue)
{
    queue->front = 0;
    queue->rear = 0;
}

/**
 * @brief Deallocates the memory used by the CircularQueue.
 * @param queue A pointer to the CircularQueue to be destroyed.
 * @note After calling this function, the queue pointer should not be used.
 */
void destroy(CircularQueue *queue)
{
    free(queue->array);
    queue->array = NULL;
    queue->front = 0;
    queue->rear = 0;
    queue->capacity = 0;
}

/**
 * @brief Checks if the CircularQueue is empty.
 * @param queue The CircularQueue to check.
 * @return True if the queue is empty, false otherwise.
 */
bool isEmpty(const CircularQueue queue)
{
    return queue.front == queue.rear;
}

/**
 * @brief Checks if the CircularQueue is full.
 * @param queue The CircularQueue to check.
 * @return True if the queue is full, false otherwise.
 */
bool isFull(const CircularQueue queue)
{
    return (queue.rear + 1) % queue.capacity == queue.front;
}

/**
 * @brief Returns the current number of elements in the CircularQueue.
 * @param queue The CircularQueue to query.
 * @return The number of elements currently in the queue.
 */
int length(const CircularQueue queue)
{
    if (queue.rear >= queue.front)
        return queue.rear - queue.front;
    return queue.capacity - queue.front + queue.rear;
}

/**
 * @brief Adds a new element to the rear of the CircularQueue.
 * @param queue A pointer to the CircularQueue to enqueue the element into.
 * @param value The integer value to be enqueued.
 * @note Prints "CircularQueue Overflow" if the queue is full.
 */
void enqueue(CircularQueue *queue, const int value)
{
    if (isFull(*queue))
    {
        printf("CircularQueue Overflow\n");
        return;
    }
    queue->array[queue->rear] = value;
    queue->rear = (queue->rear + 1) % queue->capacity;
}

/**
 * @brief Returns the front element of the CircularQueue without removing it.
 * @param queue The CircularQueue to peek at.
 * @return The value of the front element, or -1 if the queue is empty (and prints "CircularQueue Underflow").
 */
int peek(const CircularQueue queue)
{
    if (isEmpty(queue))
    {
        printf("CircularQueue Underflow\n");
        return -1;
    }
    return queue.array[queue.front];
}

/**
 * @brief Removes and returns the front element from the CircularQueue.
 * @param queue A pointer to the CircularQueue to dequeue the element from.
 * @return The value of the dequeued element, or -1 if the queue is empty (and prints "CircularQueue Underflow").
 */
int dequeue(CircularQueue *queue)
{
    if (isEmpty(*queue))
    {
        printf("CircularQueue Underflow\n");
        return -1;
    }
    int value = queue->array[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    return value;
}

/**
 * @brief Traverses and prints the elements of the CircularQueue from front to rear.
 * @param queue The CircularQueue to traverse.
 * @details Elements are printed in the format "[ el1 el2 ... ] : current_length/logical_capacity".
 */
void traverse(const CircularQueue queue)
{
    int len = length(queue);
    printf("[");
    int i = queue.front;
    while (i != queue.rear)
    {
        printf(" %d", queue.array[i]);
        i = (i + 1) % queue.capacity;
    }
    printf(" ] : %d/%d\n", len, queue.capacity - 1); // Display logical capacity
}

#endif // CIRCULAR_QUEUE_H