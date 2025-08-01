#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * @brief Represents the ordering type of the priority queue.
 */
typedef enum
{
    ASCENDING, /**< The highest priority is the smallest integer value. */
    DESCENDING /**< The highest priority is the largest integer value. */
} Type;

/**
 * @brief Represents an element in the priority queue.
 */
typedef struct
{
    int data;     /**< The value of the element. */
    int priority; /**< The priority of the element. */
} Tuple;

/**
 * @brief Represents a priority queue data structure, implemented as a heap.
 */
typedef struct
{
    Type type;    /**< The type of the priority queue (ASCENDING or DESCENDING). */
    Tuple *array; /**< The array that stores the queue elements. */
    int length;   /**< The current number of elements in the queue. */
    int capacity; /**< The maximum number of elements the queue can hold. */
} PriorityQueue;

/**
 * @brief Swaps the values of two Tuple structs.
 * This is an internal helper function.
 * @param a A pointer to the first Tuple.
 * @param b A pointer to the second Tuple.
 */
void __swap__(Tuple *a, Tuple *b)
{
    Tuple temp = *a;
    *a = *b;
    *b = temp;
}

/**
 * @brief Initializes a new priority queue with a specified capacity and type.
 * @param capacity The maximum number of elements the queue can hold.
 * @param type The type of priority queue (ASCENDING or DESCENDING).
 * @return A new PriorityQueue struct.
 */
PriorityQueue init(int capacity, Type type)
{
    if (capacity <= 0)
    {
        perror("Invalid capacity for PriorityQueue");
        exit(EXIT_FAILURE);
    }
    PriorityQueue queue;
    queue.type = type;
    queue.array = (Tuple *)malloc(capacity * sizeof(Tuple));
    if (queue.array == NULL)
    {
        perror("Failed to initialize PriorityQueue");
        exit(EXIT_FAILURE);
    }
    queue.length = 0;
    queue.capacity = capacity;
    return queue;
}

/**
 * @brief Creates a deep copy of an existing priority queue.
 * @param queue A pointer to the queue to be copied.
 * @return A new PriorityQueue struct that is a copy of the original.
 */
PriorityQueue copy(const PriorityQueue *queue)
{
    PriorityQueue copied = init(queue->capacity, queue->type);
    for (int i = 0; i < queue->length; i++)
    {
        copied.array[copied.length].data = queue->array[i].data;
        copied.array[copied.length++].priority = queue->array[i].priority;
    }
    return copied;
}

/**
 * @brief Clears the contents of a priority queue, resetting its length to zero.
 * @param queue A pointer to the queue to be cleared.
 */
void clear(PriorityQueue *queue)
{
    if (queue == NULL)
        return;
    queue->length = 0;
}

/**
 * @brief Frees the memory allocated for the priority queue's array.
 * @param queue A pointer to the queue to be destroyed.
 */
void destroy(PriorityQueue *queue)
{
    if (queue == NULL)
        return;
    free(queue->array);
    queue->array = NULL;
    queue->length = 0;
    queue->capacity = 0;
}

/**
 * @brief Checks if a priority queue is empty.
 * @param queue A pointer to the queue.
 * @return true if the queue is empty, false otherwise.
 */
bool isEmpty(const PriorityQueue *queue)
{
    if (queue == NULL)
        return true;
    return queue->length == 0;
}

/**
 * @brief Checks if a priority queue is full.
 * @param queue A pointer to the queue.
 * @return true if the queue is full, false otherwise.
 */
bool isFull(const PriorityQueue *queue)
{
    if (queue == NULL)
        return true;
    return queue->length == queue->capacity;
}

/**
 * @brief Searches for a value in the priority queue.
 * @param queue A pointer to the queue.
 * @param value The data value to search for.
 * @return The index of the value if found, otherwise -1.
 */
int search(const PriorityQueue *queue, int value)
{
    if (queue == NULL)
        return -1;
    for (int i = 0; i < queue->length; i++)
    {
        if (queue->array[i].data == value)
            return i;
    }
    return -1;
}

/**
 * @brief Calculates the parent index of a given index.
 * This is an internal helper function.
 * @param index The child's index.
 * @return The index of the parent node.
 */
int __parent__(int index)
{
    return (index - 1) / 2;
}

/**
 * @brief Calculates the left child index of a given index.
 * This is an internal helper function.
 * @param index The parent's index.
 * @return The index of the left child.
 */
int __leftChild__(int index)
{
    return 2 * index + 1;
}

/**
 * @brief Calculates the right child index of a given index.
 * This is an internal helper function.
 * @param index The parent's index.
 * @return The index of the right child.
 */
int __rightChild__(int index)
{
    return 2 * index + 2;
}

/**
 * @brief Verifies if the queue satisfies the ascending priority property.
 * @param queue A pointer to the queue.
 * @return true if it's a valid ascending priority queue, false otherwise.
 */
bool isAsc(const PriorityQueue *queue)
{
    if (queue == NULL)
        return false;
    for (int chld = 1; chld < queue->length; chld++)
    {
        int prnt = __parent__(chld);
        if (queue->array[prnt].priority > queue->array[chld].priority)
            return false;
    }
    return true;
}

/**
 * @brief Verifies if the queue satisfies the descending priority property.
 * @param queue A pointer to the queue.
 * @return true if it's a valid descending priority queue, false otherwise.
 */
bool isDesc(const PriorityQueue *queue)
{
    if (queue == NULL)
        return false;
    for (int chld = 1; chld < queue->length; chld++)
    {
        int prnt = __parent__(chld);
        if (queue->array[prnt].priority < queue->array[chld].priority)
            return false;
    }
    return true;
}

/**
 * @brief Corrects the heap property by moving a node up the tree.
 * This is an internal helper function.
 * @param queue A pointer to the queue.
 * @param chld The index of the child node to start from.
 */
void __heapifyUp__(PriorityQueue *queue, int chld)
{
    if (queue == NULL || chld == 0)
        return;
    int prnt = __parent__(chld);
    if ((queue->type == ASCENDING && queue->array[prnt].priority > queue->array[chld].priority) || (queue->type == DESCENDING && queue->array[prnt].priority < queue->array[chld].priority))
    {
        __swap__(queue->array + prnt, queue->array + chld);
        __heapifyUp__(queue, prnt);
    }
}

/**
 * @brief Corrects the heap property by moving a node down the tree.
 * This is an internal helper function.
 * @param queue A pointer to the queue.
 * @param prnt The index of the parent node to start from.
 */
void __heapifyDown__(PriorityQueue *queue, int prnt)
{
    if (queue == NULL)
        return;
    int left = __leftChild__(prnt);
    int right = __rightChild__(prnt);
    int selected = prnt;
    if (queue->type == ASCENDING)
    {
        if (left < queue->length && queue->array[selected].priority > queue->array[left].priority)
            selected = left;
        if (right < queue->length && queue->array[selected].priority > queue->array[right].priority)
            selected = right;
    }
    else if (queue->type == DESCENDING)
    {
        if (left < queue->length && queue->array[selected].priority < queue->array[left].priority)
            selected = left;
        if (right < queue->length && queue->array[selected].priority < queue->array[right].priority)
            selected = right;
    }
    if (selected != prnt)
    {
        __swap__(queue->array + selected, queue->array + prnt);
        __heapifyDown__(queue, selected);
    }
}

/**
 * @brief Enqueues a new element with a given priority.
 * @param queue A pointer to the priority queue.
 * @param value The value to enqueue.
 * @param weight The priority of the value.
 * @return 0 on success, -1 if the queue is full.
 */
int enqueue(PriorityQueue *queue, int value, int weight)
{
    if (isFull(queue))
        return -1;
    queue->array[queue->length].data = value;
    queue->array[queue->length++].priority = weight;
    __heapifyUp__(queue, queue->length - 1);
    return 0;
}

/**
 * @brief Updates the priority of an element in the queue.
 * @param queue A pointer to the queue.
 * @param value The data value of the element to update.
 * @param weight The new priority.
 * @return 0 on success, -1 if the value is not found.
 */
int update(PriorityQueue *queue, int value, int weight)
{
    int index = search(queue, value);
    if (index == -1)
        return -1;
    queue->array[index].priority = weight;
    int prnt = __parent__(index);
    if (queue->type == ASCENDING)
    {
        if (queue->array[prnt].priority > weight)
            __heapifyUp__(queue, index);
        else
            __heapifyDown__(queue, index);
    }
    else if (queue->type == DESCENDING)
    {
        if (queue->array[prnt].priority < weight)
            __heapifyUp__(queue, index);
        else
            __heapifyDown__(queue, index);
    }
    return 0;
}

/**
 * @brief Peeks at the element with the highest priority (the root).
 * @param queue A pointer to the priority queue.
 * @return The data value of the highest priority element, or -1 if the queue is empty.
 */
int peek(const PriorityQueue *queue)
{
    if (isEmpty(queue))
        return -1;
    return queue->array[0].data;
}

/**
 * @brief Dequeues and removes the element with the highest priority.
 * @param queue A pointer to the priority queue.
 * @return The data value of the dequeued element, or -1 if the queue is empty.
 */
int dequeue(PriorityQueue *queue)
{
    if (isEmpty(queue))
        return -1;
    int value = queue->array[0].data;
    queue->array[0].data = queue->array[--queue->length].data;
    queue->array[0].priority = queue->array[queue->length].priority;
    __heapifyDown__(queue, 0);
    return value;
}

/**
 * @brief Prints the contents of the priority queue level by level.
 * @param queue The priority queue to traverse.
 */
void traverse(const PriorityQueue queue)
{
    int levelEnd = 0;
    for (int i = 0; i < queue.length; i++)
    {
        printf("(%d,%d) ", queue.array[i].data, queue.array[i].priority);
        if (i == levelEnd || i == queue.length - 1)
        {
            printf("\n");
            levelEnd = 2 * (levelEnd + 1);
        }
    }
}

#endif // PRIORITY_QUEUE_H