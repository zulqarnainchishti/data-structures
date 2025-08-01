#ifndef HEAP_H
#define HEAP_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * @brief Represents the type of heap.
 */
typedef enum
{
    MIN_HEAP, /**< A min-heap, where the parent is always less than or equal to its children. */
    MAX_HEAP  /**< A max-heap, where the parent is always greater than or equal to its children. */
} Type;

/**
 * @brief Represents a heap data structure.
 */
typedef struct
{
    Type type;    /**< The type of the heap (MIN_HEAP or MAX_HEAP). */
    int *array;   /**< The array that stores the heap elements. */
    int length;   /**< The current number of elements in the heap. */
    int capacity; /**< The maximum number of elements the heap can hold. */
} Heap;

/**
 * @brief Swaps the values of two integers.
 * This is an internal helper function.
 * @param a A pointer to the first integer.
 * @param b A pointer to the second integer.
 */
void __swap__(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

/**
 * @brief Initializes a new heap with a specified capacity and type.
 * @param capacity The maximum number of elements the heap can hold.
 * @param type The type of heap (MIN_HEAP or MAX_HEAP).
 * @return A new Heap struct.
 */
Heap init(int capacity, Type type)
{
    if (capacity <= 0)
    {
        perror("Invalid capacity for Heap");
        exit(EXIT_FAILURE);
    }
    Heap heap;
    heap.type = type;
    heap.array = (int *)malloc(capacity * sizeof(int));
    if (heap.array == NULL)
    {
        perror("Failed to initialize Heap");
        exit(EXIT_FAILURE);
    }
    heap.length = 0;
    heap.capacity = capacity;
    return heap;
}

/**
 * @brief Creates a deep copy of an existing heap.
 * @param heap A pointer to the heap to be copied.
 * @return A new Heap struct that is a copy of the original.
 */
Heap copy(const Heap *heap)
{
    Heap copied = init(heap->capacity, heap->type);
    for (int i = 0; i < heap->length; i++)
        copied.array[copied.length++] = heap->array[i];
    return copied;
}

/**
 * @brief Clears the contents of a heap, resetting its length to zero.
 * @param heap A pointer to the heap to be cleared.
 */
void clear(Heap *heap)
{
    if (heap == NULL)
        return;
    heap->length = 0;
}

/**
 * @brief Frees the memory allocated for the heap's array.
 * @param heap A pointer to the heap to be destroyed.
 */
void destroy(Heap *heap)
{
    if (heap == NULL)
        return;
    free(heap->array);
    heap->array = NULL;
    heap->capacity = 0;
    heap->length = 0;
}

/**
 * @brief Checks if a heap is empty.
 * @param heap A pointer to the heap.
 * @return true if the heap is empty, false otherwise.
 */
bool isEmpty(const Heap *heap)
{
    if (heap == NULL)
        return true;
    return heap->length == 0;
}

/**
 * @brief Checks if a heap is full.
 * @param heap A pointer to the heap.
 * @return true if the heap is full, false otherwise.
 */
bool isFull(const Heap *heap)
{
    if (heap == NULL)
        return true;
    return heap->length == heap->capacity;
}

/**
 * @brief Searches for a value in the heap.
 * @param heap A pointer to the heap.
 * @param value The value to search for.
 * @return The index of the value if found, otherwise -1.
 */
int search(const Heap *heap, int value)
{
    if (heap == NULL)
        return -1;
    for (int i = 0; i < heap->length; i++)
    {
        if (heap->array[i] == value)
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
 * @brief Verifies if the heap satisfies the min-heap property.
 * @param heap A pointer to the heap.
 * @return true if it's a valid min-heap, false otherwise.
 */
bool isMin(const Heap *heap)
{
    if (heap == NULL)
        return false;
    for (int chld = 1; chld < heap->length; chld++)
    {
        int prnt = __parent__(chld);
        if (heap->array[prnt] > heap->array[chld])
            return false;
    }
    return true;
}

/**
 * @brief Verifies if the heap satisfies the max-heap property.
 * @param heap A pointer to the heap.
 * @return true if it's a valid max-heap, false otherwise.
 */
bool isMax(const Heap *heap)
{
    if (heap == NULL)
        return false;
    for (int chld = 1; chld < heap->length; chld++)
    {
        int prnt = __parent__(chld);
        if (heap->array[prnt] < heap->array[chld])
            return false;
    }
    return true;
}

/**
 * @brief Corrects the heap property by moving a node up the tree.
 * This is an internal helper function.
 * @param heap A pointer to the heap.
 * @param chld The index of the child node to start from.
 */
void __heapifyUp__(Heap *heap, int chld)
{
    if (heap == NULL || chld == 0)
        return;
    int prnt = __parent__(chld);
    if ((heap->type == MIN_HEAP && heap->array[prnt] > heap->array[chld]) || (heap->type == MAX_HEAP && heap->array[prnt] < heap->array[chld]))
    {
        __swap__(heap->array + prnt, heap->array + chld);
        __heapifyUp__(heap, prnt);
    }
}

/**
 * @brief Corrects the heap property by moving a node down the tree.
 * This is an internal helper function.
 * @param heap A pointer to the heap.
 * @param prnt The index of the parent node to start from.
 */
void __heapifyDown__(Heap *heap, int prnt)
{
    if (heap == NULL)
        return;
    int left = __leftChild__(prnt);
    int right = __rightChild__(prnt);
    int selected = prnt;
    if (heap->type == MIN_HEAP)
    {
        if (left < heap->length && heap->array[selected] > heap->array[left])
            selected = left;
        if (right < heap->length && heap->array[selected] > heap->array[right])
            selected = right;
    }
    else if (heap->type == MAX_HEAP)
    {
        if (left < heap->length && heap->array[selected] < heap->array[left])
            selected = left;
        if (right < heap->length && heap->array[selected] < heap->array[right])
            selected = right;
    }
    if (selected != prnt)
    {
        __swap__(heap->array + selected, heap->array + prnt);
        __heapifyDown__(heap, selected);
    }
}

/**
 * @brief Inserts a new value into the heap.
 * @param heap A pointer to the heap.
 * @param value The value to insert.
 * @return 0 on success, -1 if the heap is full.
 */
int insert(Heap *heap, int value)
{
    if (isFull(heap))
        return -1;
    heap->array[heap->length++] = value;
    __heapifyUp__(heap, heap->length - 1);
    return 0;
}

/**
 * @brief Removes a specific value from the heap.
 * @param heap A pointer to the heap.
 * @param value The value to remove.
 * @return 0 on success, -1 if the value is not found.
 */
int discard(Heap *heap, int value)
{
    int index = search(heap, value);
    if (index == -1)
        return -1;
    heap->array[index] = heap->array[--heap->length];
    __heapifyDown__(heap, index);
    return 0;
}

/**
 * @brief Peeks at the top element of the heap (the root).
 * @param heap A pointer to the heap.
 * @return The top value, or -1 if the heap is empty.
 */
int peek(const Heap *heap)
{
    if (isEmpty(heap))
        return -1;
    return heap->array[0];
}

/**
 * @brief Extracts and removes the top element from the heap.
 * @param heap A pointer to the heap.
 * @return The extracted value, or -1 if the heap is empty.
 */
int extract(Heap *heap)
{
    if (isEmpty(heap))
        return -1;
    int value = heap->array[0];
    heap->array[0] = heap->array[--heap->length];
    __heapifyDown__(heap, 0);
    return value;
}

/**
 * @brief Prints the contents of the heap level by level.
 * @param heap A pointer to the heap to traverse.
 */
void traverse(const Heap *heap)
{
    int levelEnd = 0;
    for (int i = 0; i < heap->length; i++)
    {
        printf("(%d) ", heap->array[i]);
        if (i == levelEnd || i == heap->length - 1)
        {
            printf("\n");
            levelEnd = 2 * (levelEnd + 1);
        }
    }
}

#endif // HEAP_H