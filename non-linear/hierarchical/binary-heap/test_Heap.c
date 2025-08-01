#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

// Include the header file for the heap implementation
#include "adt_Heap.h"

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

// Helper function to print the contents of the heap for debugging
void printHeap(const char *testName, const Heap *heap)
{
    printf("--- %s: Current Heap ---\n", testName);
    if (isEmpty(heap))
    {
        printf("Heap is empty.\n");
    }
    else
    {
        traverse(heap);
    }
    printf("------------------------\n");
}

// Test case for initializing and destroying a heap
void test_init_destroy()
{
    printf("--- Running test_init_destroy ---\n");
    Heap minHeap = init(10, MIN_HEAP);
    TEST_ASSERT(minHeap.capacity == 10, "Initial capacity should be 10");
    TEST_ASSERT(minHeap.length == 0, "Initial length should be 0");
    TEST_ASSERT(minHeap.type == MIN_HEAP, "Heap type should be MIN_HEAP");
    TEST_ASSERT(minHeap.array != NULL, "Heap array should not be NULL");

    Heap maxHeap = init(5, MAX_HEAP);
    TEST_ASSERT(maxHeap.capacity == 5, "Initial capacity should be 5");
    TEST_ASSERT(maxHeap.length == 0, "Initial length should be 0");
    TEST_ASSERT(maxHeap.type == MAX_HEAP, "Heap type should be MAX_HEAP");
    TEST_ASSERT(maxHeap.array != NULL, "Heap array should not be NULL");

    destroy(&minHeap);
    TEST_ASSERT(minHeap.array == NULL, "Array should be freed after destroy");
    TEST_ASSERT(minHeap.capacity == 0, "Capacity should be 0 after destroy");

    destroy(&maxHeap);
    TEST_ASSERT(maxHeap.array == NULL, "Array should be freed after destroy");
    TEST_ASSERT(maxHeap.capacity == 0, "Capacity should be 0 after destroy");
    printf("--- Finished test_init_destroy ---\n\n");
}

// Test case for insertion and validation of min-heap property
void test_min_heap_insert()
{
    printf("--- Running test_min_heap_insert ---\n");
    Heap heap = init(10, MIN_HEAP);

    TEST_ASSERT(insert(&heap, 5) == 0, "Insert should succeed");
    TEST_ASSERT(heap.length == 1, "Length should be 1 after one insert");
    TEST_ASSERT(isMin(&heap), "Heap should be a valid min-heap");

    TEST_ASSERT(insert(&heap, 3) == 0, "Insert should succeed");
    TEST_ASSERT(heap.array[0] == 3, "Root should be the smallest element");
    TEST_ASSERT(isMin(&heap), "Heap should be a valid min-heap");

    TEST_ASSERT(insert(&heap, 8) == 0, "Insert should succeed");
    TEST_ASSERT(heap.array[0] == 3, "Root should remain the smallest");
    TEST_ASSERT(isMin(&heap), "Heap should be a valid min-heap");

    TEST_ASSERT(insert(&heap, 1) == 0, "Insert should succeed");
    TEST_ASSERT(heap.array[0] == 1, "Root should be the new smallest element");
    TEST_ASSERT(isMin(&heap), "Heap should be a valid min-heap");

    TEST_ASSERT(insert(&heap, 10) == 0, "Insert should succeed");
    TEST_ASSERT(insert(&heap, 2) == 0, "Insert should succeed");
    TEST_ASSERT(insert(&heap, 7) == 0, "Insert should succeed");
    TEST_ASSERT(heap.length == 7, "Length should be 7");
    TEST_ASSERT(isMin(&heap), "Heap should be a valid min-heap");

    printHeap("test_min_heap_insert", &heap);

    // Fill the heap to test isFull
    insert(&heap, 6);
    insert(&heap, 9);
    insert(&heap, 4);
    TEST_ASSERT(isFull(&heap), "Heap should be full");
    TEST_ASSERT(insert(&heap, 11) == -1, "Insert should fail on a full heap");

    destroy(&heap);
    printf("--- Finished test_min_heap_insert ---\n\n");
}

// Test case for insertion and validation of max-heap property
void test_max_heap_insert()
{
    printf("--- Running test_max_heap_insert ---\n");
    Heap heap = init(10, MAX_HEAP);

    TEST_ASSERT(insert(&heap, 5) == 0, "Insert should succeed");
    TEST_ASSERT(heap.length == 1, "Length should be 1 after one insert");
    TEST_ASSERT(isMax(&heap), "Heap should be a valid max-heap");

    TEST_ASSERT(insert(&heap, 8) == 0, "Insert should succeed");
    TEST_ASSERT(heap.array[0] == 8, "Root should be the largest element");
    TEST_ASSERT(isMax(&heap), "Heap should be a valid max-heap");

    TEST_ASSERT(insert(&heap, 3) == 0, "Insert should succeed");
    TEST_ASSERT(heap.array[0] == 8, "Root should remain the largest");
    TEST_ASSERT(isMax(&heap), "Heap should be a valid max-heap");

    TEST_ASSERT(insert(&heap, 10) == 0, "Insert should succeed");
    TEST_ASSERT(heap.array[0] == 10, "Root should be the new largest element");
    TEST_ASSERT(isMax(&heap), "Heap should be a valid max-heap");

    TEST_ASSERT(insert(&heap, 1) == 0, "Insert should succeed");
    TEST_ASSERT(insert(&heap, 9) == 0, "Insert should succeed");
    TEST_ASSERT(insert(&heap, 4) == 0, "Insert should succeed");
    TEST_ASSERT(heap.length == 7, "Length should be 7");
    TEST_ASSERT(isMax(&heap), "Heap should be a valid max-heap");

    printHeap("test_max_heap_insert", &heap);

    destroy(&heap);
    printf("--- Finished test_max_heap_insert ---\n\n");
}

// Test case for peeking and extracting elements
void test_peek_extract()
{
    printf("--- Running test_peek_extract ---\n");
    Heap minHeap = init(10, MIN_HEAP);
    int values[] = {10, 5, 8, 3, 1, 9, 2, 7};
    for (int i = 0; i < 8; ++i)
    {
        insert(&minHeap, values[i]);
    }

    printHeap("test_peek_extract (Min-Heap before extraction)", &minHeap);

    TEST_ASSERT(peek(&minHeap) == 1, "Peek on min-heap should return the smallest element");
    TEST_ASSERT(extract(&minHeap) == 1, "Extract should return the smallest element");
    TEST_ASSERT(minHeap.length == 7, "Length should be 7 after extraction");
    TEST_ASSERT(peek(&minHeap) == 2, "Peek should return the new smallest element");
    TEST_ASSERT(isMin(&minHeap), "Heap should maintain min-heap property after extraction");

    TEST_ASSERT(extract(&minHeap) == 2, "Extract should return the smallest element");
    TEST_ASSERT(peek(&minHeap) == 3, "Peek should return the new smallest element");
    TEST_ASSERT(isMin(&minHeap), "Heap should maintain min-heap property after extraction");

    destroy(&minHeap);

    Heap maxHeap = init(10, MAX_HEAP);
    for (int i = 0; i < 8; ++i)
    {
        insert(&maxHeap, values[i]);
    }

    printHeap("test_peek_extract (Max-Heap before extraction)", &maxHeap);

    TEST_ASSERT(peek(&maxHeap) == 10, "Peek on max-heap should return the largest element");
    TEST_ASSERT(extract(&maxHeap) == 10, "Extract should return the largest element");
    TEST_ASSERT(maxHeap.length == 7, "Length should be 7 after extraction");
    TEST_ASSERT(peek(&maxHeap) == 9, "Peek should return the new largest element");
    TEST_ASSERT(isMax(&maxHeap), "Heap should maintain max-heap property after extraction");

    TEST_ASSERT(extract(&maxHeap) == 9, "Extract should return the largest element");
    TEST_ASSERT(peek(&maxHeap) == 8, "Peek should return the new largest element");
    TEST_ASSERT(isMax(&maxHeap), "Heap should maintain max-heap property after extraction");

    destroy(&maxHeap);
    printf("--- Finished test_peek_extract ---\n\n");
}

// Test case for searching and discarding elements
void test_search_discard()
{
    printf("--- Running test_search_discard ---\n");
    Heap heap = init(10, MIN_HEAP);
    int values[] = {10, 5, 8, 3, 1, 9, 2, 7};
    for (int i = 0; i < 8; ++i)
    {
        insert(&heap, values[i]);
    }

    printHeap("test_search_discard (Before discard)", &heap);

    TEST_ASSERT(search(&heap, 8) != -1, "Search should find value 8");
    TEST_ASSERT(search(&heap, 100) == -1, "Search should not find value 100");

    TEST_ASSERT(discard(&heap, 8) == 0, "Discard should succeed for an existing value");
    TEST_ASSERT(heap.length == 7, "Length should be 7 after discard");
    TEST_ASSERT(search(&heap, 8) == -1, "Search should no longer find value 8");
    TEST_ASSERT(isMin(&heap), "Heap should maintain min-heap property after discard");

    TEST_ASSERT(discard(&heap, 1) == 0, "Discard should succeed for root element");
    TEST_ASSERT(heap.length == 6, "Length should be 6 after discard");
    TEST_ASSERT(isMin(&heap), "Heap should maintain min-heap property after discard");
    TEST_ASSERT(peek(&heap) == 2, "New root should be 2");

    TEST_ASSERT(discard(&heap, 100) == -1, "Discard should fail for a non-existing value");
    TEST_ASSERT(heap.length == 6, "Length should not change for failed discard");

    printHeap("test_search_discard (After discard)", &heap);

    destroy(&heap);
    printf("--- Finished test_search_discard ---\n\n");
}

// Test case for clearing and copying a heap
void test_clear_copy()
{
    printf("--- Running test_clear_copy ---\n");
    Heap original = init(10, MIN_HEAP);
    int values[] = {10, 5, 8, 3, 1};
    for (int i = 0; i < 5; ++i)
    {
        insert(&original, values[i]);
    }

    printHeap("test_clear_copy (Original Heap)", &original);
    TEST_ASSERT(original.length == 5, "Original heap should have 5 elements");

    Heap copied = copy(&original);
    TEST_ASSERT(copied.length == 5, "Copied heap should have 5 elements");
    TEST_ASSERT(copied.capacity == original.capacity, "Copied heap should have same capacity");
    TEST_ASSERT(copied.type == original.type, "Copied heap should have same type");
    TEST_ASSERT(copied.array != original.array, "Copied array should be a deep copy");

    clear(&original);
    TEST_ASSERT(original.length == 0, "Original heap should be empty after clear");
    TEST_ASSERT(copied.length == 5, "Copied heap should be unaffected by clearing the original");

    printHeap("test_clear_copy (Copied Heap after original is cleared)", &copied);

    destroy(&original);
    destroy(&copied);
    printf("--- Finished test_clear_copy ---\n\n");
}

int main()
{
    printf("Starting extensive test suite for Heap data structure...\n\n");

    test_init_destroy();
    test_min_heap_insert();
    test_max_heap_insert();
    test_peek_extract();
    test_search_discard();
    test_clear_copy();

    printf("All tests passed!\n");
    return 0;
}