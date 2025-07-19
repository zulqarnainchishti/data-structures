#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>            // Required for dup, dup2, close
#include "adt_CircularQueue.h" // Include your CircularQueue header

int tests_run = 0;
int tests_passed = 0;

#define GREEN "\033[1;32m"
#define RED "\033[1;31m"
#define CYAN "\033[1;36m"
#define RESET "\033[0m"

#define ASSERT_VERBOSE(expr_result, msg, expected_str, actual_str) \
    do                                                             \
    {                                                              \
        tests_run++;                                               \
        printf(CYAN "TEST: %s\n" RESET, msg);                      \
        printf("   Expected: \"%s\"\n", expected_str);             \
        printf("   Actual   : \"%s\"\n", actual_str);              \
        if (expr_result)                                           \
        {                                                          \
            printf(GREEN "   Result   : PASS\n\n" RESET);          \
            tests_passed++;                                        \
        }                                                          \
        else                                                       \
        {                                                          \
            printf(RED "   Result   : FAIL\n\n" RESET);            \
        }                                                          \
    } while (0)

#define ASSERT_INT(msg, expected, actual)                            \
    do                                                               \
    {                                                                \
        char exp_str[20], act_str[20];                               \
        sprintf(exp_str, "%d", expected);                            \
        sprintf(act_str, "%d", actual);                              \
        ASSERT_VERBOSE((actual == expected), msg, exp_str, act_str); \
    } while (0)

#define ASSERT_BOOL(expr_result, msg, expected_bool)                                     \
    do                                                                                   \
    {                                                                                    \
        const char *expected_str = (expected_bool) ? "true" : "false";                   \
        const char *actual_str = (expr_result) ? "true" : "false";                       \
        ASSERT_VERBOSE((expr_result) == (expected_bool), msg, expected_str, actual_str); \
    } while (0)

char *capture_stdout(void (*func)(const CircularQueue), const CircularQueue q)
{
    fflush(stdout);
    char buffer[256];
    size_t len = 0;

    int saved_stdout = dup(fileno(stdout));
    if (saved_stdout == -1)
    {
        perror("Failed to duplicate stdout");
        exit(EXIT_FAILURE);
    }

    FILE *temp_file = tmpfile();
    if (!temp_file)
    {
        perror("Failed to create temporary file");
        close(saved_stdout);
        exit(EXIT_FAILURE);
    }

    if (dup2(fileno(temp_file), fileno(stdout)) == -1)
    {
        perror("Failed to redirect stdout to temp file");
        fclose(temp_file);
        close(saved_stdout);
        exit(EXIT_FAILURE);
    }

    func(q);

    fflush(stdout);
    long current_pos = ftell(temp_file);
    rewind(temp_file);

    len = fread(buffer, 1, sizeof(buffer) - 1, temp_file);
    buffer[len] = '\0';

    if (dup2(saved_stdout, fileno(stdout)) == -1)
    {
        perror("Failed to restore stdout");
        fclose(temp_file);
        close(saved_stdout);
        exit(EXIT_FAILURE);
    }
    close(saved_stdout);
    fclose(temp_file);

    char *result = strdup(buffer);
    if (!result)
    {
        perror("Failed to allocate memory for captured output");
        exit(EXIT_FAILURE);
    }
    return result;
}

void test_init_and_basic_ops()
{
    printf(CYAN "--- Test: Initialization and Basic Operations ---\n" RESET);
    CircularQueue q = init(5); // Logical capacity 5

    ASSERT_BOOL(isEmpty(q), "New circular queue should be empty", true);
    ASSERT_BOOL(isFull(q), "New circular queue should not be full", false);
    ASSERT_INT("Logical capacity should be 5", 5, q.capacity - 1); // Internal capacity is +1
    ASSERT_INT("Length of new circular queue should be 0", 0, length(q));

    enqueue(&q, 10);
    ASSERT_INT("Peek after one enqueue should return 10", 10, peek(q));
    ASSERT_BOOL(isEmpty(q), "Circular queue should not be empty after one enqueue", false);
    ASSERT_INT("Length after one enqueue should be 1", 1, length(q));

    enqueue(&q, 20);
    ASSERT_INT("Peek after two enqueues should still return 10", 10, peek(q));
    ASSERT_INT("Length after two enqueues should be 2", 2, length(q));

    int dequeued = dequeue(&q);
    ASSERT_INT("Dequeue should return 10", 10, dequeued);
    ASSERT_INT("Peek after one dequeue should return 20", 20, peek(q));
    ASSERT_INT("Length after one dequeue should be 1", 1, length(q));

    clear(&q);
    ASSERT_BOOL(isEmpty(q), "Circular queue should be empty after clear", true);
    ASSERT_INT("Length after clear should be 0", 0, length(q));

    destroy(&q);
}

void test_circular_queue_overflow_underflow()
{
    printf(CYAN "--- Test: Circular Queue Overflow and Underflow ---\n" RESET);
    CircularQueue q = init(2); // Logical capacity 2
    enqueue(&q, 1);
    enqueue(&q, 2);

    // This enqueue should cause an overflow and print "CircularQueue Overflow"
    printf("Attempting to enqueue into a full circular queue (expecting 'CircularQueue Overflow' message):\n");
    enqueue(&q, 3);

    ASSERT_BOOL(isFull(q), "Circular queue should be full after 2 enqueues", true);
    ASSERT_INT("Length should be 2 when full", 2, length(q));

    // Dequeue the first element
    int val1 = dequeue(&q);
    ASSERT_INT("Dequeue should return 1", 1, val1);

    // Dequeue the second element
    int val2 = dequeue(&q);
    ASSERT_INT("Dequeue should return 2", 2, val2);

    // Now the queue is empty. Subsequent operations should cause underflow.
    printf("Attempting to dequeue from an empty circular queue (expecting 'CircularQueue Underflow' message):\n");
    ASSERT_INT("Dequeue from empty should return -1", -1, dequeue(&q));

    printf("Attempting to peek from an empty circular queue (expecting 'CircularQueue Underflow' message):\n");
    ASSERT_INT("Peek from empty should return -1", -1, peek(q));

    ASSERT_BOOL(isEmpty(q), "Circular queue should be empty after all valid dequeues", true);

    destroy(&q);
}

void test_copy_circular_queue()
{
    printf(CYAN "--- Test: Copy Circular Queue ---\n" RESET);
    CircularQueue q1 = init(3); // Logical capacity 3
    enqueue(&q1, 5);
    enqueue(&q1, 6);
    enqueue(&q1, 7); // Fill it up

    CircularQueue q2 = copy(q1);

    ASSERT_INT("Copied queue has correct logical capacity", q1.capacity - 1, q2.capacity - 1);
    ASSERT_INT("Copied queue has correct length", length(q1), length(q2));
    ASSERT_INT("Copied queue front element matches", peek(q1), peek(q2));

    dequeue(&q1);                                                                // Modify original
    ASSERT_INT("Modifying original does not affect copy length", 3, length(q2)); // Q2 still has 3 elements
    ASSERT_INT("Modifying original does not affect copy front", 5, peek(q2));

    enqueue(&q2, 8);                                                             // Modify copy (this will wrap around)
    ASSERT_INT("Modifying copy does not affect original length", 2, length(q1)); // Q1 has 2 elements

    destroy(&q1);
    destroy(&q2);
}

void test_stress_circular_queue()
{
    printf(CYAN "--- Test: Stress Test ---\n" RESET);
    const int N = 10000;
    CircularQueue q = init(N); // Logical capacity N

    for (int i = 0; i < N; i++)
    {
        enqueue(&q, i);
    }
    ASSERT_BOOL(isFull(q), "Circular queue should be full after N enqueues", true);
    ASSERT_INT("Length should be N", N, length(q));

    int valid = 1;
    for (int i = 0; i < N; i++)
    {
        if (dequeue(&q) != i)
        {
            valid = 0;
            break;
        }
    }
    ASSERT_BOOL(valid, "Dequeue sequence should be same as enqueue", true);
    ASSERT_BOOL(isEmpty(q), "Circular queue should be empty after full dequeue", true);
    ASSERT_INT("Length should be 0 after full dequeue", 0, length(q));

    // Test enqueuing and dequeuing in a circular manner
    clear(&q); // Ensure it's truly empty before the mixed operations test

    // Fill half
    for (int i = 0; i < N / 2; i++)
    {
        enqueue(&q, i);
    }
    // Dequeue a quarter
    for (int i = 0; i < N / 4; i++)
    {
        dequeue(&q);
    }
    // Enqueue another quarter (this will often involve wrapping)
    for (int i = N / 2; i < N * 3 / 4; i++)
    {
        enqueue(&q, i);
    }

    // Validate remaining elements in order and then empty the queue
    int expected_val = N / 4;
    int current_len = length(q); // Get current length before starting dequeue loop
    for (int i = 0; i < current_len; i++)
    {
        if (peek(q) != expected_val)
        {
            valid = 0;
            printf("Error in mixed ops: Expected %d, got %d\n", expected_val, peek(q)); // Debug print
            break;
        }
        dequeue(&q);
        expected_val++;
    }
    ASSERT_BOOL(valid, "Circular behavior and element order maintained after mixed operations", true);
    ASSERT_BOOL(isEmpty(q), "Circular queue should be empty after mixed operations and subsequent full dequeue", true);

    destroy(&q);
}

void test_reinit_circular_queue()
{
    printf(CYAN "--- Test: Reinitialization ---\n" RESET);
    CircularQueue q = init(2);
    enqueue(&q, 7);
    destroy(&q);

    q = init(3); // Reinitialize with a new capacity
    ASSERT_BOOL(isEmpty(q), "Reinitialized circular queue should be empty", true);
    ASSERT_INT("Length of reinitialized circular queue should be 0", 0, length(q));
    enqueue(&q, 99);
    ASSERT_INT("Peek after reinit enqueue should return 99", 99, peek(q));

    destroy(&q);
}

void test_traverse()
{
    printf(CYAN "--- Test: Traverse Function ---\n" RESET);
    CircularQueue q = init(5); // Logical capacity 5

    char *output_empty = capture_stdout(traverse, q);
    ASSERT_VERBOSE(strcmp(output_empty, "[ ] : 0/5\n") == 0, "Traverse empty circular queue", "[ ] : 0/5\n", output_empty);
    free(output_empty);

    enqueue(&q, 10);
    enqueue(&q, 20);
    char *output_two_elements = capture_stdout(traverse, q);
    ASSERT_VERBOSE(strcmp(output_two_elements, "[ 10 20 ] : 2/5\n") == 0, "Traverse circular queue with two elements", "[ 10 20 ] : 2/5\n", output_two_elements);
    free(output_two_elements);

    dequeue(&q);
    char *output_after_dequeue = capture_stdout(traverse, q);
    ASSERT_VERBOSE(strcmp(output_after_dequeue, "[ 20 ] : 1/5\n") == 0, "Traverse circular queue after one dequeue", "[ 20 ] : 1/5\n", output_after_dequeue);
    free(output_after_dequeue);

    // Test wrapping behavior with traverse
    clear(&q);
    enqueue(&q, 100);
    enqueue(&q, 101);
    enqueue(&q, 102);
    enqueue(&q, 103);
    enqueue(&q, 104); // Queue is full: [100 101 102 103 104]

    dequeue(&q); // Dequeue 100
    dequeue(&q); // Dequeue 101
    // Queue is now [102 103 104] with front at index 2 (assuming 0-indexed array)
    enqueue(&q, 105); // Enqueue 105 (wraps to index 0)
    enqueue(&q, 106); // Enqueue 106 (wraps to index 1)
    // Logical order: [102 103 104 105 106]

    char *output_wrapped = capture_stdout(traverse, q);
    ASSERT_VERBOSE(strcmp(output_wrapped, "[ 102 103 104 105 106 ] : 5/5\n") == 0, "Traverse wrapped circular queue", "[ 102 103 104 105 106 ] : 5/5\n", output_wrapped);
    free(output_wrapped);

    destroy(&q);
}

int main()
{
    printf(CYAN "\n===== CIRCULAR QUEUE TEST SUITE =====\n\n" RESET);

    test_init_and_basic_ops();
    test_circular_queue_overflow_underflow();
    test_copy_circular_queue();
    test_stress_circular_queue();
    test_reinit_circular_queue();
    test_traverse();

    printf(CYAN "\n===== TEST SUMMARY =====\n" RESET);
    printf("Total tests run     : %d\n", tests_run);
    printf(GREEN "Total tests passed  : %d\n" RESET, tests_passed);
    printf(RED "Total tests failed  : %d\n\n" RESET, tests_run - tests_passed);

    return 0;
}