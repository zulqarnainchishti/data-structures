#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

// Include the header file for the priority queue implementation
#include "adt_PriorityQueue.h"

// Macro for printing test results
#define TEST_ASSERT(condition, message)                                         \
    do                                                                          \
    {                                                                           \
        if (!(condition))                                                       \
        {                                                                       \
            fprintf(stderr, "[FAIL] %s:%d: %s\n", __FILE__, __LINE__, message); \
            exit(EXIT_FAILURE);                                                 \
        }                                                                       \
        else                                                                    \
        {                                                                       \
            printf("[PASS] %s\n", message);                                     \
        }                                                                       \
    } while (0)

// Helper function to print the contents of the queue for debugging
void printQueue(const char *testName, const PriorityQueue *queue)
{
    printf("--- %s: Current Priority Queue ---\n", testName);
    if (isEmpty(queue))
    {
        printf("Queue is empty.\n");
    }
    else
    {
        traverse(*queue);
    }
    printf("----------------------------------\n");
}

// Test case for initializing and destroying a priority queue
void test_init_destroy()
{
    printf("--- Running test_init_destroy ---\n");
    PriorityQueue ascQueue = init(10, ASCENDING);
    TEST_ASSERT(ascQueue.capacity == 10, "Initial capacity should be 10");
    TEST_ASSERT(ascQueue.length == 0, "Initial length should be 0");
    TEST_ASSERT(ascQueue.type == ASCENDING, "Queue type should be ASCENDING");
    TEST_ASSERT(ascQueue.array != NULL, "Queue array should not be NULL");

    PriorityQueue descQueue = init(5, DESCENDING);
    TEST_ASSERT(descQueue.capacity == 5, "Initial capacity should be 5");
    TEST_ASSERT(descQueue.length == 0, "Initial length should be 0");
    TEST_ASSERT(descQueue.type == DESCENDING, "Queue type should be DESCENDING");
    TEST_ASSERT(descQueue.array != NULL, "Queue array should not be NULL");

    destroy(&ascQueue);
    TEST_ASSERT(ascQueue.array == NULL, "Array should be freed after destroy");
    TEST_ASSERT(ascQueue.capacity == 0, "Capacity should be 0 after destroy");

    destroy(&descQueue);
    TEST_ASSERT(descQueue.array == NULL, "Array should be freed after destroy");
    TEST_ASSERT(descQueue.capacity == 0, "Capacity should be 0 after destroy");
    printf("--- Finished test_init_destroy ---\n\n");
}

// Test case for enqueueing elements into an ascending priority queue
void test_asc_enqueue()
{
    printf("--- Running test_asc_enqueue ---\n");
    PriorityQueue queue = init(10, ASCENDING);

    TEST_ASSERT(enqueue(&queue, 10, 5) == 0, "Enqueue should succeed");
    TEST_ASSERT(queue.length == 1, "Length should be 1 after one enqueue");
    TEST_ASSERT(isAsc(&queue), "Queue should maintain ascending priority property");

    TEST_ASSERT(enqueue(&queue, 20, 3) == 0, "Enqueue should succeed");
    TEST_ASSERT(peek(&queue) == 20, "Element with smallest priority should be at the front");
    TEST_ASSERT(isAsc(&queue), "Queue should maintain ascending priority property");

    TEST_ASSERT(enqueue(&queue, 30, 8) == 0, "Enqueue should succeed");
    TEST_ASSERT(peek(&queue) == 20, "Front element should remain unchanged");
    TEST_ASSERT(isAsc(&queue), "Queue should maintain ascending priority property");

    TEST_ASSERT(enqueue(&queue, 40, 1) == 0, "Enqueue should succeed");
    TEST_ASSERT(peek(&queue) == 40, "New element with highest priority should be at the front");
    TEST_ASSERT(isAsc(&queue), "Queue should maintain ascending priority property");

    printQueue("test_asc_enqueue", &queue);

    destroy(&queue);
    printf("--- Finished test_asc_enqueue ---\n\n");
}

// Test case for enqueueing elements into a descending priority queue
void test_desc_enqueue()
{
    printf("--- Running test_desc_enqueue ---\n");
    PriorityQueue queue = init(10, DESCENDING);

    TEST_ASSERT(enqueue(&queue, 10, 5) == 0, "Enqueue should succeed");
    TEST_ASSERT(queue.length == 1, "Length should be 1 after one enqueue");
    TEST_ASSERT(isDesc(&queue), "Queue should maintain descending priority property");

    TEST_ASSERT(enqueue(&queue, 20, 8) == 0, "Enqueue should succeed");
    TEST_ASSERT(peek(&queue) == 20, "Element with largest priority should be at the front");
    TEST_ASSERT(isDesc(&queue), "Queue should maintain descending priority property");

    TEST_ASSERT(enqueue(&queue, 30, 3) == 0, "Enqueue should succeed");
    TEST_ASSERT(peek(&queue) == 20, "Front element should remain unchanged");
    TEST_ASSERT(isDesc(&queue), "Queue should maintain descending priority property");

    TEST_ASSERT(enqueue(&queue, 40, 10) == 0, "Enqueue should succeed");
    TEST_ASSERT(peek(&queue) == 40, "New element with highest priority should be at the front");
    TEST_ASSERT(isDesc(&queue), "Queue should maintain descending priority property");

    printQueue("test_desc_enqueue", &queue);

    destroy(&queue);
    printf("--- Finished test_desc_enqueue ---\n\n");
}

// Test case for peeking and dequeueing elements
void test_peek_dequeue()
{
    printf("--- Running test_peek_dequeue ---\n");
    PriorityQueue ascQueue = init(10, ASCENDING);
    int values[] = {10, 20, 30, 40};
    int priorities[] = {5, 3, 8, 1};
    for (int i = 0; i < 4; ++i)
    {
        enqueue(&ascQueue, values[i], priorities[i]);
    }

    printQueue("test_peek_dequeue (ASCENDING before dequeue)", &ascQueue);
    TEST_ASSERT(peek(&ascQueue) == 40, "Peek on ascending queue should return data with smallest priority (1)");
    TEST_ASSERT(dequeue(&ascQueue) == 40, "Dequeue should return data with smallest priority");
    TEST_ASSERT(ascQueue.length == 3, "Length should be 3 after dequeue");
    TEST_ASSERT(peek(&ascQueue) == 20, "Peek should return the new highest priority element");
    TEST_ASSERT(isAsc(&ascQueue), "Queue should maintain ascending priority property after dequeue");
    destroy(&ascQueue);

    PriorityQueue descQueue = init(10, DESCENDING);
    for (int i = 0; i < 4; ++i)
    {
        enqueue(&descQueue, values[i], priorities[i]);
    }

    printQueue("test_peek_dequeue (DESCENDING before dequeue)", &descQueue);
    TEST_ASSERT(peek(&descQueue) == 30, "Peek on descending queue should return data with largest priority (8)");
    TEST_ASSERT(dequeue(&descQueue) == 30, "Dequeue should return data with largest priority");
    TEST_ASSERT(descQueue.length == 3, "Length should be 3 after dequeue");
    TEST_ASSERT(peek(&descQueue) == 10, "Peek should return the new highest priority element");
    TEST_ASSERT(isDesc(&descQueue), "Queue should maintain descending priority property after dequeue");
    destroy(&descQueue);

    printf("--- Finished test_peek_dequeue ---\n\n");
}

// Test case for updating element priority
void test_update()
{
    printf("--- Running test_update ---\n");
    PriorityQueue queue = init(10, ASCENDING);
    enqueue(&queue, 10, 5);
    enqueue(&queue, 20, 3);
    enqueue(&queue, 30, 8);
    enqueue(&queue, 40, 1);

    printQueue("test_update (Before update)", &queue);
    TEST_ASSERT(peek(&queue) == 40, "Initial peek should be 40");

    // Update an element to a higher priority (smaller value)
    TEST_ASSERT(update(&queue, 10, 0) == 0, "Update should succeed");
    printQueue("test_update (After updating 10 to priority 0)", &queue);
    TEST_ASSERT(peek(&queue) == 10, "Peek should now be 10, as its priority is the highest");
    TEST_ASSERT(isAsc(&queue), "Queue should maintain ascending priority property after update");

    // Update an element to a lower priority (larger value)
    TEST_ASSERT(update(&queue, 40, 10) == 0, "Update should succeed");
    printQueue("test_update (After updating 40 to priority 10)", &queue);
    TEST_ASSERT(peek(&queue) == 10, "Peek should still be 10, as it's the highest priority");
    TEST_ASSERT(isAsc(&queue), "Queue should maintain ascending priority property after update");

    TEST_ASSERT(update(&queue, 50, 100) == -1, "Update should fail for a non-existing value");
    destroy(&queue);
    printf("--- Finished test_update ---\n\n");
}

// Test case for clearing and copying a priority queue
void test_clear_copy()
{
    printf("--- Running test_clear_copy ---\n");
    PriorityQueue original = init(10, DESCENDING);
    enqueue(&original, 10, 5);
    enqueue(&original, 20, 8);
    enqueue(&original, 30, 3);

    printQueue("test_clear_copy (Original Queue)", &original);
    TEST_ASSERT(original.length == 3, "Original queue should have 3 elements");

    PriorityQueue copied = copy(&original);
    TEST_ASSERT(copied.length == 3, "Copied queue should have 3 elements");
    TEST_ASSERT(copied.capacity == original.capacity, "Copied queue should have same capacity");
    TEST_ASSERT(copied.type == original.type, "Copied queue should have same type");
    TEST_ASSERT(copied.array != original.array, "Copied array should be a deep copy");
    TEST_ASSERT(copied.array[0].data == original.array[0].data, "Copied element data should match original");
    TEST_ASSERT(copied.array[0].priority == original.array[0].priority, "Copied element priority should match original");

    clear(&original);
    TEST_ASSERT(original.length == 0, "Original queue should be empty after clear");
    TEST_ASSERT(copied.length == 3, "Copied queue should be unaffected by clearing the original");

    printQueue("test_clear_copy (Copied Queue after original is cleared)", &copied);

    destroy(&original);
    destroy(&copied);
    printf("--- Finished test_clear_copy ---\n\n");
}

int main()
{
    printf("Starting extensive test suite for Priority Queue data structure...\n\n");

    test_init_destroy();
    test_asc_enqueue();
    test_desc_enqueue();
    test_peek_dequeue();
    test_update();
    test_clear_copy();

    printf("All tests passed!\n");
    return 0;
}