#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // Required for dup, dup2, close
#include "adt_Queue.h"

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

// Fixed ASSERT_INT: Now takes 'msg', 'expected', 'actual'. The comparison 'actual == expected' is done inside.
#define ASSERT_INT(msg, expected, actual)                            \
    do                                                               \
    {                                                                \
        char exp_str[20], act_str[20];                               \
        sprintf(exp_str, "%d", expected);                            \
        sprintf(act_str, "%d", actual);                              \
        ASSERT_VERBOSE((actual == expected), msg, exp_str, act_str); \
    } while (0)

// Fixed ASSERT_BOOL: Now takes 'expr_result', 'msg', 'expected_bool'.
// The comparison '(expr_result) == (expected_bool)' is done inside.
#define ASSERT_BOOL(expr_result, msg, expected_bool)                                     \
    do                                                                                   \
    {                                                                                    \
        const char *expected_str = (expected_bool) ? "true" : "false";                   \
        const char *actual_str = (expr_result) ? "true" : "false";                       \
        ASSERT_VERBOSE((expr_result) == (expected_bool), msg, expected_str, actual_str); \
    } while (0)

char *capture_stdout(void (*func)(const Queue), const Queue q)
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
    Queue q = init(5);

    ASSERT_BOOL(isEmpty(q), "New queue should be empty", true);
    ASSERT_BOOL(isFull(q), "New queue should not be full", false);
    ASSERT_INT("Capacity should be 5", 5, q.capacity);
    ASSERT_INT("Length of new queue should be 0", 0, length(q));

    enqueue(&q, 10);
    ASSERT_INT("Peek after one enqueue should return 10", 10, peek(q));
    ASSERT_BOOL(isEmpty(q), "Queue should not be empty after one enqueue", false);
    ASSERT_INT("Length after one enqueue should be 1", 1, length(q));

    enqueue(&q, 20);
    ASSERT_INT("Peek after two enqueues should still return 10", 10, peek(q));
    ASSERT_INT("Length after two enqueues should be 2", 2, length(q));

    int dequeued = dequeue(&q);
    ASSERT_INT("Dequeue should return 10", 10, dequeued);
    ASSERT_INT("Peek after one dequeue should return 20", 20, peek(q));
    ASSERT_INT("Length after one dequeue should be 1", 1, length(q));

    clear(&q);
    ASSERT_BOOL(isEmpty(q), "Queue should be empty after clear", true);
    ASSERT_INT("Length after clear should be 0", 0, length(q));

    destroy(&q);
}

void test_queue_overflow_underflow()
{
    printf(CYAN "--- Test: Queue Overflow and Underflow ---\n" RESET);
    Queue q = init(2);
    enqueue(&q, 1);
    enqueue(&q, 2);

    // This enqueue should cause an overflow and print "Queue Overflow"
    printf("Attempting to enqueue into a full queue (expecting 'Queue Overflow' message):\n");
    enqueue(&q, 3);

    ASSERT_BOOL(isFull(q), "Queue should be full after 2 enqueues", true);
    ASSERT_INT("Length should be 2 when full", 2, length(q));

    // Dequeue the first element
    int val1 = dequeue(&q);
    ASSERT_INT("Dequeue should return 1", 1, val1);

    // Dequeue the second element
    int val2 = dequeue(&q);
    ASSERT_INT("Dequeue should return 2", 2, val2);

    // Now the queue is empty. Subsequent operations should cause underflow.
    printf("Attempting to dequeue from an empty queue (expecting 'Queue Underflow' message):\n");
    ASSERT_INT("Dequeue from empty should return -1", -1, dequeue(&q));

    printf("Attempting to peek from an empty queue (expecting 'Queue Underflow' message):\n");
    ASSERT_INT("Peek from empty should return -1", -1, peek(q));

    ASSERT_BOOL(isEmpty(q), "Queue should be empty after all valid dequeues", true);

    destroy(&q);
}

void test_copy_queue()
{
    printf(CYAN "--- Test: Copy Queue ---\n" RESET);
    Queue q1 = init(3);
    enqueue(&q1, 5);
    enqueue(&q1, 6);

    Queue q2 = copy(q1);

    ASSERT_INT("Copied queue has correct capacity", q1.capacity, q2.capacity);
    ASSERT_INT("Copied queue has correct length", length(q1), length(q2));
    ASSERT_INT("Copied queue front element matches", peek(q1), peek(q2));

    dequeue(&q1); // Modify original
    ASSERT_INT("Modifying original does not affect copy length", 2, length(q2));
    ASSERT_INT("Modifying original does not affect copy front", 5, peek(q2));

    enqueue(&q2, 7); // Modify copy
    ASSERT_INT("Modifying copy does not affect original length", 1, length(q1));

    destroy(&q1);
    destroy(&q2);
}

void test_stress_queue()
{
    printf(CYAN "--- Test: Stress Test ---\n" RESET);
    const int N = 10000;
    Queue q = init(N);

    for (int i = 0; i < N; i++)
    {
        enqueue(&q, i);
    }
    ASSERT_BOOL(isFull(q), "Queue should be full after N enqueues", true);
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
    ASSERT_BOOL(isEmpty(q), "Queue should be empty after full dequeue", true);
    ASSERT_INT("Length should be 0 after full dequeue", 0, length(q));

    destroy(&q);
}

void test_reinit_queue()
{
    printf(CYAN "--- Test: Reinitialization ---\n" RESET);
    Queue q = init(2);
    enqueue(&q, 7);
    destroy(&q);

    q = init(3);
    ASSERT_BOOL(isEmpty(q), "Reinitialized queue should be empty", true);
    ASSERT_INT("Length of reinitialized queue should be 0", 0, length(q));
    enqueue(&q, 99);
    ASSERT_INT("Peek after reinit enqueue should return 99", 99, peek(q));

    destroy(&q);
}

void test_traverse()
{
    printf(CYAN "--- Test: Traverse Function ---\n" RESET);
    Queue q = init(5);

    char *output_empty = capture_stdout(traverse, q);
    ASSERT_VERBOSE(strcmp(output_empty, "[ ] : 0/5\n") == 0, "Traverse empty queue", "[ ] : 0/5\n", output_empty);
    free(output_empty);

    enqueue(&q, 10);
    enqueue(&q, 20);
    char *output_two_elements = capture_stdout(traverse, q);
    ASSERT_VERBOSE(strcmp(output_two_elements, "[ 10 20 ] : 2/5\n") == 0, "Traverse queue with two elements", "[ 10 20 ] : 2/5\n", output_two_elements);
    free(output_two_elements);

    dequeue(&q);
    char *output_after_dequeue = capture_stdout(traverse, q);
    ASSERT_VERBOSE(strcmp(output_after_dequeue, "[ 20 ] : 1/5\n") == 0, "Traverse queue after one dequeue", "[ 20 ] : 1/5\n", output_after_dequeue);
    free(output_after_dequeue);

    destroy(&q);
}

int main()
{
    printf(CYAN "\n===== QUEUE TEST SUITE =====\n\n" RESET);

    test_init_and_basic_ops();
    test_queue_overflow_underflow();
    test_copy_queue();
    test_stress_queue();
    test_reinit_queue();
    test_traverse();

    printf(CYAN "\n===== TEST SUMMARY =====\n" RESET);
    printf("Total tests run    : %d\n", tests_run);
    printf(GREEN "Total tests passed : %d\n" RESET, tests_passed);
    printf(RED "Total tests failed : %d\n\n" RESET, tests_run - tests_passed);

    return 0;
}