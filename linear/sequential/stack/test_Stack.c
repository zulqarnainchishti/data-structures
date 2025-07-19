#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "adt_Stack.h" // Assuming stack.h contains the Stack struct and all functions

int tests_run = 0;
int tests_passed = 0;

#define GREEN "\033[1;32m"
#define RED "\033[1;31m"
#define CYAN "\033[1;36m"
#define RESET "\033[0m"

#define ASSERT_VERBOSE(expr, msg, expected_str, actual_str) \
    do                                                      \
    {                                                       \
        tests_run++;                                        \
        printf(CYAN "TEST: %s\n" RESET, msg);               \
        printf("  Expected: \"%s\"\n", expected_str);       \
        printf("  Actual  : \"%s\"\n", actual_str);         \
        if (expr)                                           \
        {                                                   \
            printf(GREEN "  Result  : PASS\n\n" RESET);     \
            tests_passed++;                                 \
        }                                                   \
        else                                                \
        {                                                   \
            printf(RED "  Result  : FAIL\n\n" RESET);       \
        }                                                   \
    } while (0)

#define ASSERT_INT(expr, msg, expected, actual)      \
    do                                               \
    {                                                \
        char exp_str[20], act_str[20];               \
        sprintf(exp_str, "%d", expected);            \
        sprintf(act_str, "%d", actual);              \
        ASSERT_VERBOSE(expr, msg, exp_str, act_str); \
    } while (0)

#define ASSERT_BOOL(expr, msg, expected_bool)                                     \
    do                                                                            \
    {                                                                             \
        const char *expected_str = (expected_bool) ? "true" : "false";            \
        const char *actual_str = (expr) ? "true" : "false";                       \
        ASSERT_VERBOSE((expr) == (expected_bool), msg, expected_str, actual_str); \
    } while (0)

void test_init_and_basic_ops()
{
    Stack s = init(5);
    ASSERT_BOOL(isEmpty(s), "New stack should be empty", true);
    ASSERT_BOOL(isFull(s), "New stack should not be full", false);
    ASSERT_INT(s.capacity, "Capacity should be 5", 5, s.capacity);

    push(&s, 10);
    ASSERT_INT(peek(s), "Peek after one push should return 10", 10, peek(s));
    ASSERT_BOOL(isEmpty(s), "Stack should not be empty after one push", false);

    push(&s, 20);
    ASSERT_INT(peek(s), "Peek after two pushes should return 20", 20, peek(s));

    int popped = pop(&s);
    ASSERT_INT(popped, "Pop should return 20", 20, popped);
    ASSERT_INT(peek(s), "Peek after one pop should return 10", 10, peek(s));

    clear(&s);
    ASSERT_BOOL(isEmpty(s), "Stack should be empty after clear", true);

    destroy(&s);
}

void test_stack_overflow_underflow()
{
    Stack s = init(2);
    push(&s, 1);
    push(&s, 2);
    push(&s, 3); // Should trigger overflow (non-fatal)

    ASSERT_BOOL(isFull(s), "Stack should be full after 2 pushes", true);

    ASSERT_INT(pop(&s), "Pop should return 2", 2, pop(&s));
    ASSERT_INT(pop(&s), "Pop should return 1", 1, pop(&s));
    ASSERT_INT(pop(&s), "Pop from empty should return -1", -1, pop(&s));
    ASSERT_INT(peek(s), "Peek from empty should return -1", -1, peek(s));

    destroy(&s);
}

void test_copy_stack()
{
    Stack s1 = init(3);
    push(&s1, 5);
    push(&s1, 6);

    Stack s2 = copy(s1);
    ASSERT_INT(s2.capacity, "Copied stack has correct capacity", s1.capacity, s2.capacity);
    ASSERT_INT(s2.top, "Copied stack has correct top index", s1.top, s2.top);
    ASSERT_INT(s2.array[0], "Copied stack element 0 matches", s1.array[0], s2.array[0]);
    ASSERT_INT(s2.array[1], "Copied stack element 1 matches", s1.array[1], s2.array[1]);

    pop(&s1);
    ASSERT_INT(s2.top, "Modifying original does not affect copy", 2, s2.top);

    destroy(&s1);
    destroy(&s2);
}

void test_stress_stack()
{
    const int N = 10000;
    Stack s = init(N);
    for (int i = 0; i < N; i++)
    {
        push(&s, i);
    }
    ASSERT_BOOL(isFull(s), "Stack should be full after N pushes", true);

    int valid = 1;
    for (int i = N - 1; i >= 0; i--)
    {
        if (pop(&s) != i)
        {
            valid = 0;
            break;
        }
    }
    ASSERT_BOOL(valid, "Pop sequence should be reverse of push", true);
    ASSERT_BOOL(isEmpty(s), "Stack should be empty after full pop", true);

    destroy(&s);
}

void test_reinit_stack()
{
    Stack s = init(2);
    push(&s, 7);
    destroy(&s);

    s = init(3);
    ASSERT_BOOL(isEmpty(s), "Reinitialized stack should be empty", true);
    push(&s, 99);
    ASSERT_INT(peek(s), "Peek after reinit push should return 99", 99, peek(s));
    destroy(&s);
}

int main()
{
    printf(CYAN "\n===== STACK TEST SUITE =====\n\n" RESET);
    test_init_and_basic_ops();
    test_stack_overflow_underflow();
    test_copy_stack();
    test_stress_stack();
    test_reinit_stack();

    printf(CYAN "\n===== TEST SUMMARY =====\n" RESET);
    printf("Total tests run   : %d\n", tests_run);
    printf(GREEN "Total tests passed: %d\n" RESET, tests_passed);
    printf(RED "Total tests failed: %d\n\n" RESET, tests_run - tests_passed);

    return 0;
}
