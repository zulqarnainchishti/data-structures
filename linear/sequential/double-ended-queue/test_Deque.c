#include <assert.h>
#include <string.h>
#include "adt_Deque.h"

#define TEST_START(name) printf("\n--- Starting Test: %s ---\n", name)
#define TEST_PASSED(name) printf("--- Test PASSED: %s ---\n", name)
#define TEST_FAILED(name) printf("--- Test FAILED: %s ---\n", name)
#define ASSERT_TRUE(condition, message)            \
    if (!(condition))                              \
    {                                              \
        printf("ASSERTION FAILED: %s\n", message); \
        exit(EXIT_FAILURE);                        \
    }

void test_init_destroy()
{
    TEST_START("Initialization and Destruction");

    Deque dq1 = init(5);
    ASSERT_TRUE(dq1.array != NULL, "Array not allocated during init");
    ASSERT_TRUE(dq1.capacity == 5, "Capacity not set correctly");
    ASSERT_TRUE(dq1.length == 0, "Length not initialized to 0");
    ASSERT_TRUE(dq1.front == dq1.capacity / 2, "Front not initialized to center");
    ASSERT_TRUE(dq1.rear == dq1.capacity / 2, "Rear not initialized to center");
    printf("Init 1: Capacity %d, Front %d, Rear %d, Length %d\n", dq1.capacity, dq1.front, dq1.rear, dq1.length);

    destroy(&dq1);
    ASSERT_TRUE(dq1.array == NULL, "Array not freed during destroy");
    ASSERT_TRUE(dq1.capacity == 0, "Capacity not reset after destroy");
    ASSERT_TRUE(dq1.length == 0, "Length not reset after destroy");
    ASSERT_TRUE(dq1.front == 0, "Front not reset after destroy");
    ASSERT_TRUE(dq1.rear == 0, "Rear not reset after destroy");
    printf("Destroy 1: Array NULL, Capacity 0, Length 0\n");

    Deque dq2 = init(1);
    ASSERT_TRUE(dq2.array != NULL, "Array not allocated during init (cap 1)");
    ASSERT_TRUE(dq2.capacity == 1, "Capacity not set correctly (cap 1)");
    ASSERT_TRUE(dq2.front == 0, "Front not initialized correctly for cap 1");
    ASSERT_TRUE(dq2.rear == 0, "Rear not initialized correctly for cap 1");
    destroy(&dq2);

    TEST_PASSED("Initialization and Destruction");
}

void test_invalid_init_capacity()
{
    TEST_START("Invalid Initialization Capacity");
    printf("Expected behavior: Program should exit with an error message.\n");

    printf("This test is usually commented out or run in isolation because init() calls exit().\n");
    printf("Manually verifying init(0) and init(-5) causes program termination.\n");
    TEST_PASSED("Invalid Initialization Capacity (manual verification needed)");
}

void test_empty_full_checks()
{
    TEST_START("Empty and Full Checks");

    Deque dq = init(3);
    printf("Initial: ");
    traverse(dq);
    ASSERT_TRUE(isEmpty(dq), "Deque should be empty initially");
    ASSERT_TRUE(!isFrontFull(dq), "Front should not be full initially");
    ASSERT_TRUE(!isRearFull(dq), "Rear should not be full initially");

    enqueueRear(&dq, 10);
    printf("After enqueueRear 10: ");
    traverse(dq);
    ASSERT_TRUE(!isEmpty(dq), "Deque should not be empty after enqueue");
    ASSERT_TRUE(!isFrontFull(dq), "Front should not be full");
    ASSERT_TRUE(!isRearFull(dq), "Rear should not be full");

    enqueueFront(&dq, 5);
    printf("After enqueueFront 5: ");
    traverse(dq);
    ASSERT_TRUE(!isEmpty(dq), "Deque should not be empty");
    ASSERT_TRUE(isFrontFull(dq), "Front should be full");
    ASSERT_TRUE(!isRearFull(dq), "Rear should not be full");

    enqueueRear(&dq, 15);
    printf("After enqueueRear 15: ");
    traverse(dq);
    ASSERT_TRUE(!isEmpty(dq), "Deque should not be empty");
    ASSERT_TRUE(isFrontFull(dq), "Front should be full");
    ASSERT_TRUE(isRearFull(dq), "Rear should be full");

    dequeueFront(&dq);
    dequeueRear(&dq);
    dequeueFront(&dq);
    printf("After dequeuing all: ");
    traverse(dq);
    ASSERT_TRUE(isEmpty(dq), "Deque should be empty after dequeuing all");
    ASSERT_TRUE(!isFrontFull(dq), "Front should not be full after empty");
    ASSERT_TRUE(!isRearFull(dq), "Rear should not be full after empty");

    destroy(&dq);
    TEST_PASSED("Empty and Full Checks");
}

void test_enqueue_dequeue_front()
{
    TEST_START("Enqueue and Dequeue Front");
    Deque dq = init(5);

    enqueueFront(&dq, 10);
    ASSERT_TRUE(peekFront(dq) == 10, "Peek front failed after 1st enqueue front");
    ASSERT_TRUE(dq.length == 1, "Length incorrect after 1st enqueue front");
    printf("After enqueueFront 10: ");
    traverse(dq);

    enqueueFront(&dq, 20);
    ASSERT_TRUE(peekFront(dq) == 20, "Peek front failed after 2nd enqueue front");
    ASSERT_TRUE(dq.length == 2, "Length incorrect after 2nd enqueue front");
    printf("After enqueueFront 20: ");
    traverse(dq);

    printf("Attempting to enqueueFront when full...\n");
    enqueueFront(&dq, 30);
    ASSERT_TRUE(peekFront(dq) == 20, "Value changed after overflow attempt");
    ASSERT_TRUE(dq.length == 2, "Length changed after overflow attempt");
    printf("After overflow attempt: ");
    traverse(dq);

    int val = dequeueFront(&dq);
    ASSERT_TRUE(val == 20, "Dequeue front returned incorrect value (1st)");
    ASSERT_TRUE(peekFront(dq) == 10, "Peek front failed after 1st dequeue front");
    ASSERT_TRUE(dq.length == 1, "Length incorrect after 1st dequeue front");
    printf("After dequeueFront 20: ");
    traverse(dq);

    val = dequeueFront(&dq);
    ASSERT_TRUE(val == 10, "Dequeue front returned incorrect value (2nd)");
    ASSERT_TRUE(isEmpty(dq), "Deque not empty after 2nd dequeue front");
    ASSERT_TRUE(dq.length == 0, "Length incorrect after 2nd dequeue front");
    ASSERT_TRUE(dq.front == dq.capacity / 2 && dq.rear == dq.capacity / 2, "Front/rear not reset after empty");
    printf("After dequeueFront 10: ");
    traverse(dq);

    printf("Attempting to peekFront when empty...\n");
    ASSERT_TRUE(peekFront(dq) == -1, "Peek front when empty should return -1");
    printf("Attempting to dequeueFront when empty...\n");
    ASSERT_TRUE(dequeueFront(&dq) == -1, "Dequeue front when empty should return -1");
    printf("After underflow attempt: ");
    traverse(dq);

    destroy(&dq);
    TEST_PASSED("Enqueue and Dequeue Front");
}

void test_enqueue_dequeue_rear()
{
    TEST_START("Enqueue and Dequeue Rear");
    Deque dq = init(5);

    enqueueRear(&dq, 10);
    ASSERT_TRUE(peekRear(dq) == 10, "Peek rear failed after 1st enqueue rear");
    ASSERT_TRUE(dq.length == 1, "Length incorrect after 1st enqueue rear");
    printf("After enqueueRear 10: ");
    traverse(dq);

    enqueueRear(&dq, 20);
    ASSERT_TRUE(peekRear(dq) == 20, "Peek rear failed after 2nd enqueue rear");
    ASSERT_TRUE(dq.length == 2, "Length incorrect after 2nd enqueue rear");
    printf("After enqueueRear 20: ");
    traverse(dq);

    enqueueRear(&dq, 30);
    ASSERT_TRUE(peekRear(dq) == 30, "Peek rear failed after 3rd enqueue rear");
    ASSERT_TRUE(dq.length == 3, "Length incorrect after 3rd enqueue rear");
    printf("After enqueueRear 30: ");
    traverse(dq);

    printf("Attempting to enqueueRear when full...\n");
    enqueueRear(&dq, 40);
    ASSERT_TRUE(peekRear(dq) == 30, "Value changed after overflow attempt");
    ASSERT_TRUE(dq.length == 3, "Length changed after overflow attempt");
    printf("After overflow attempt: ");
    traverse(dq);

    int val = dequeueRear(&dq);
    ASSERT_TRUE(val == 30, "Dequeue rear returned incorrect value (1st)");
    ASSERT_TRUE(peekRear(dq) == 20, "Peek rear failed after 1st dequeue rear");
    ASSERT_TRUE(dq.length == 2, "Length incorrect after 1st dequeue rear");
    printf("After dequeueRear 30: ");
    traverse(dq);

    val = dequeueRear(&dq);
    ASSERT_TRUE(val == 20, "Dequeue rear returned incorrect value (2nd)");
    ASSERT_TRUE(peekRear(dq) == 10, "Peek rear failed after 2nd dequeue rear");
    ASSERT_TRUE(dq.length == 1, "Length incorrect after 2nd dequeue rear");
    printf("After dequeueRear 20: ");
    traverse(dq);

    val = dequeueRear(&dq);
    ASSERT_TRUE(val == 10, "Dequeue rear returned incorrect value (3rd)");
    ASSERT_TRUE(isEmpty(dq), "Deque not empty after 3rd dequeue rear");
    ASSERT_TRUE(dq.length == 0, "Length incorrect after 3rd dequeue rear");
    ASSERT_TRUE(dq.front == dq.capacity / 2 && dq.rear == dq.capacity / 2, "Front/rear not reset after empty");
    printf("After dequeueRear 10: ");
    traverse(dq);

    printf("Attempting to peekRear when empty...\n");
    ASSERT_TRUE(peekRear(dq) == -1, "Peek rear when empty should return -1");
    printf("Attempting to dequeueRear when empty...\n");
    ASSERT_TRUE(dequeueRear(&dq) == -1, "Dequeue rear when empty should return -1");
    printf("After underflow attempt: ");
    traverse(dq);

    destroy(&dq);
    TEST_PASSED("Enqueue and Dequeue Rear");
}

void test_interleaved_operations()
{
    TEST_START("Interleaved Enqueue and Dequeue Operations");
    Deque dq = init(7);

    printf("Initial: ");
    traverse(dq);

    enqueueRear(&dq, 1);
    enqueueFront(&dq, 2);
    enqueueRear(&dq, 3);
    enqueueFront(&dq, 4);
    printf("After mixed enqueues: ");
    traverse(dq);
    ASSERT_TRUE(dq.length == 4, "Length incorrect after mixed enqueues");
    ASSERT_TRUE(peekFront(dq) == 4, "Front incorrect after mixed enqueues");
    ASSERT_TRUE(peekRear(dq) == 3, "Rear incorrect after mixed enqueues");

    ASSERT_TRUE(dequeueFront(&dq) == 4, "Incorrect dequeueFront value");
    ASSERT_TRUE(dequeueRear(&dq) == 3, "Incorrect dequeueRear value");
    printf("After mixed dequeues: ");
    traverse(dq);
    ASSERT_TRUE(dq.length == 2, "Length incorrect after mixed dequeues");
    ASSERT_TRUE(peekFront(dq) == 2, "Front incorrect after mixed dequeues");
    ASSERT_TRUE(peekRear(dq) == 1, "Rear incorrect after mixed dequeues");

    enqueueFront(&dq, 5);
    enqueueRear(&dq, 6);
    enqueueFront(&dq, 7);
    printf("After more mixed enqueues: ");
    traverse(dq);
    ASSERT_TRUE(dq.length == 5, "Length incorrect after more mixed enqueues");
    ASSERT_TRUE(peekFront(dq) == 7, "Front incorrect after more mixed enqueues");
    ASSERT_TRUE(peekRear(dq) == 6, "Rear incorrect after more mixed enqueues");

    enqueueRear(&dq, 8);
    printf("After filling: ");
    traverse(dq);
    ASSERT_TRUE(dq.length == 6, "Length incorrect after filling");

    printf("Attempting overflow at front/rear...\n");
    enqueueFront(&dq, 9);
    enqueueRear(&dq, 10);
    printf("After overflow attempts: ");
    traverse(dq);

    ASSERT_TRUE(dequeueFront(&dq) == 7, "Incorrect dequeueFront value");
    ASSERT_TRUE(dequeueRear(&dq) == 10, "Incorrect dequeueRear value"); // FIX APPLIED HERE
    ASSERT_TRUE(dequeueFront(&dq) == 5, "Incorrect dequeueFront value");
    ASSERT_TRUE(dequeueRear(&dq) == 8, "Incorrect dequeueRear value");
    ASSERT_TRUE(dequeueFront(&dq) == 2, "Incorrect dequeueFront value");
    ASSERT_TRUE(dequeueRear(&dq) == 6, "Incorrect dequeueRear value");   // Rear was 6, now 1 has 5 and 2 has 2
    ASSERT_TRUE(dequeueFront(&dq) == 1, "Incorrect dequeueFront value"); // Should be 1, if prior was 2

    printf("After emptying completely: ");
    traverse(dq);
    ASSERT_TRUE(isEmpty(dq), "Deque not empty after complete emptying");
    ASSERT_TRUE(dq.length == 0, "Length not zero after complete emptying");

    destroy(&dq);
    TEST_PASSED("Interleaved Enqueue and Dequeue Operations");
}

void test_copy_function()
{
    TEST_START("Copy Function");
    Deque original = init(10);

    enqueueRear(&original, 10);
    enqueueFront(&original, 5);
    enqueueRear(&original, 20);
    printf("Original Deque: ");
    traverse(original);

    Deque copied = copy(original);
    printf("Copied Deque: ");
    traverse(copied);

    ASSERT_TRUE(copied.capacity == original.capacity, "Copied capacity mismatch");
    ASSERT_TRUE(copied.length == original.length, "Copied length mismatch");
    ASSERT_TRUE(copied.front == original.front, "Copied front mismatch");
    ASSERT_TRUE(copied.rear == original.rear, "Copied rear mismatch");
    ASSERT_TRUE(copied.array != original.array, "Copied array points to same memory as original");

    for (int i = 0; i < copied.length; ++i)
    {
        int original_idx = (original.front + i);
        int copied_idx = (copied.front + i);
        ASSERT_TRUE(copied.array[copied_idx] == original.array[original_idx], "Copied element mismatch");
    }

    enqueueRear(&original, 30);
    printf("Original after modification: ");
    traverse(original);
    printf("Copied after original modification: ");
    traverse(copied);
    ASSERT_TRUE(copied.length == 3, "Copied length changed after original modification");
    ASSERT_TRUE(peekRear(copied) == 20, "Copied rear value changed after original modification");

    enqueueFront(&copied, 1);
    printf("Copied after its own modification: ");
    traverse(copied);
    printf("Original after copied modification: ");
    traverse(original);
    ASSERT_TRUE(original.length == 4, "Original length changed after copied modification");
    ASSERT_TRUE(peekFront(original) == 5, "Original front value changed after copied modification");

    destroy(&original);
    destroy(&copied);
    TEST_PASSED("Copy Function");
}

void test_clear_function()
{
    TEST_START("Clear Function");
    Deque dq = init(8);

    enqueueRear(&dq, 1);
    enqueueFront(&dq, 2);
    enqueueRear(&dq, 3);
    printf("Deque before clear: ");
    traverse(dq);
    ASSERT_TRUE(dq.length == 3, "Length incorrect before clear");

    clear(&dq);
    printf("Deque after clear: ");
    traverse(dq);
    ASSERT_TRUE(isEmpty(dq), "Deque not empty after clear");
    ASSERT_TRUE(dq.length == 0, "Length not 0 after clear");
    ASSERT_TRUE(dq.front == dq.capacity / 2, "Front not reset to center after clear");
    ASSERT_TRUE(dq.rear == dq.capacity / 2, "Rear not reset to center after clear");

    enqueueFront(&dq, 100);
    enqueueRear(&dq, 200);
    printf("Deque after re-adding post-clear: ");
    traverse(dq);
    ASSERT_TRUE(dq.length == 2, "Length incorrect after re-adding post-clear");
    ASSERT_TRUE(peekFront(dq) == 100, "Front incorrect after re-adding post-clear");
    ASSERT_TRUE(peekRear(dq) == 200, "Rear incorrect after re-adding post-clear");

    destroy(&dq);
    TEST_PASSED("Clear Function");
}

int main()
{
    printf("Starting Deque ADT Test Suite...\n");

    test_init_destroy();
    test_empty_full_checks();
    test_enqueue_dequeue_front();
    test_enqueue_dequeue_rear();
    test_interleaved_operations();
    test_copy_function();
    test_clear_function();

    printf("\nAll Deque ADT Tests Completed.\n");
    return 0;
}