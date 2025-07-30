#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "adt_LinkedList.h"

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

#define ASSERT_INT(actual, expected, msg)                              \
    do                                                                 \
    {                                                                  \
        char exp_str[20], act_str[20];                                 \
        sprintf(exp_str, "%d", expected);                              \
        sprintf(act_str, "%d", actual);                                \
        ASSERT_VERBOSE((actual) == (expected), msg, exp_str, act_str); \
    } while (0)

#define ASSERT_STRING(actual, expected, msg)                                  \
    do                                                                        \
    {                                                                         \
        ASSERT_VERBOSE(strcmp(actual, expected) == 0, msg, expected, actual); \
    } while (0)

#define ASSERT_PTR_NULL(ptr, msg)                                                \
    do                                                                           \
    {                                                                            \
        ASSERT_VERBOSE((ptr) == NULL, msg, "NULL", (ptr) ? "NOT NULL" : "NULL"); \
    } while (0)

#define ASSERT_PTR_NOT_NULL(ptr, msg)                                                \
    do                                                                               \
    {                                                                                \
        ASSERT_VERBOSE((ptr) != NULL, msg, "NOT NULL", (ptr) ? "NOT NULL" : "NULL"); \
    } while (0)

char *list_to_string(const LinkedList *list)
{
    if (list == NULL || list->head == NULL)
    {
        char *s = (char *)malloc(8 * sizeof(char));
        strcpy(s, "[empty]");
        return s;
    }
    char *buffer = (char *)malloc((list->length * 15 + 10) * sizeof(char));
    if (!buffer)
        return NULL;
    buffer[0] = '\0';
    LLNode *temp = list->head;
    while (temp != NULL)
    {
        char node_str[20];
        sprintf(node_str, "[%d]", temp->data);
        strcat(buffer, node_str);
        temp = temp->next;
        if (temp != NULL)
        {
            strcat(buffer, " -> ");
        }
    }
    return buffer;
}

void test_initialization_and_clear()
{
    printf("--- Testing Initialization and Clear ---\n");
    LinkedList list = init();
    ASSERT_PTR_NULL(list.head, "init() should create a list with a NULL head.");
    ASSERT_INT(list.length, 0, "init() should create a list with length 0.");

    clear(&list);
    ASSERT_PTR_NULL(list.head, "clear() on an empty list should result in a NULL head.");
    ASSERT_INT(list.length, 0, "clear() on an empty list should result in length 0.");

    insertAtStart(&list, 10);
    insertAtStart(&list, 20);
    clear(&list);
    ASSERT_PTR_NULL(list.head, "clear() on a populated list should result in a NULL head.");
    ASSERT_INT(list.length, 0, "clear() on a populated list should result in length 0.");
}

void test_insertion()
{
    printf("--- Testing Insertion Operations ---\n");
    LinkedList list = init();
    char *s = NULL;

    insertAtEnd(&list, 10);
    ASSERT_INT(list.length, 1, "Length should be 1 after one insertAtEnd.");
    s = list_to_string(&list);
    ASSERT_STRING(s, "[10]", "List should be '[10]' after one insertAtEnd.");
    free(s);

    insertAtEnd(&list, 20);
    ASSERT_INT(list.length, 2, "Length should be 2 after second insertAtEnd.");
    s = list_to_string(&list);
    ASSERT_STRING(s, "[10] -> [20]", "List should be '[10] -> [20]'.");
    free(s);

    insertAtStart(&list, 5);
    ASSERT_INT(list.length, 3, "Length should be 3 after insertAtStart.");
    s = list_to_string(&list);
    ASSERT_STRING(s, "[5] -> [10] -> [20]", "List should be '[5] -> [10] -> [20]'.");
    free(s);

    insertAtIndex(&list, 99, 0);
    s = list_to_string(&list);
    ASSERT_STRING(s, "[99] -> [5] -> [10] -> [20]", "insertAtIndex(0) should insert at start.");
    free(s);

    insertAtIndex(&list, 44, 4);
    s = list_to_string(&list);
    ASSERT_STRING(s, "[99] -> [5] -> [10] -> [20] -> [44]", "insertAtIndex(length) should insert at end.");
    free(s);

    insertAtIndex(&list, 7, 2);
    s = list_to_string(&list);
    ASSERT_STRING(s, "[99] -> [5] -> [7] -> [10] -> [20] -> [44]", "insertAtIndex(middle) should insert correctly.");
    free(s);
    ASSERT_INT(list.length, 6, "Length should be 6 after all insertions.");

    insertAfterValue(&list, 8, 7);
    s = list_to_string(&list);
    ASSERT_STRING(s, "[99] -> [5] -> [7] -> [8] -> [10] -> [20] -> [44]", "insertAfterValue should insert correctly.");
    free(s);

    int status = insertAfterValue(&list, 100, 1000);
    ASSERT_INT(status, -1, "insertAfterValue should return -1 for non-existent value.");
    ASSERT_INT(list.length, 7, "Length should not change after failed insertion.");

    clear(&list);
}

void test_deletion()
{
    printf("--- Testing Deletion Operations ---\n");
    LinkedList list = init();
    char *s = NULL;
    insertAtEnd(&list, 10);
    insertAtEnd(&list, 20);
    insertAtEnd(&list, 30);
    insertAtEnd(&list, 40);
    insertAtEnd(&list, 50);

    int val1 = deleteStart(&list);
    ASSERT_INT(val1, 10, "deleteStart should return the first element's value.");
    s = list_to_string(&list);
    ASSERT_STRING(s, "[20] -> [30] -> [40] -> [50]", "List state after deleteStart.");
    free(s);

    int val2 = deleteEnd(&list);
    ASSERT_INT(val2, 50, "deleteEnd should return the last element's value.");
    s = list_to_string(&list);
    ASSERT_STRING(s, "[20] -> [30] -> [40]", "List state after deleteEnd.");
    free(s);

    int val3 = deleteIndex(&list, 1);
    ASSERT_INT(val3, 30, "deleteIndex(1) should return the correct value.");
    ASSERT_INT(list.length, 2, "Length should decrease after deleteIndex.");
    s = list_to_string(&list);
    ASSERT_STRING(s, "[20] -> [40]", "List state after deleteIndex(1).");
    free(s);

    deleteValue(&list, 20);
    s = list_to_string(&list);
    ASSERT_STRING(s, "[40]", "List state after deleting head by value.");
    free(s);

    int status2 = deleteValue(&list, 999);
    ASSERT_INT(status2, -1, "deleteValue for non-existent value should return -1.");

    deleteEnd(&list);
    ASSERT_PTR_NULL(list.head, "deleteEnd on single-element list makes head NULL.");

    ASSERT_INT(deleteStart(&list), -1, "deleteStart on empty list should return -1.");
    ASSERT_INT(deleteEnd(&list), -1, "deleteEnd on empty list should return -1.");

    clear(&list);
}

void test_search_and_reverse()
{
    printf("--- Testing Search and Reverse ---\n");
    LinkedList list = init();
    char *s = NULL;
    insertAtEnd(&list, 10);
    insertAtEnd(&list, 20);
    insertAtEnd(&list, 30);

    LLNode *foundNode1 = search(&list, 20);
    ASSERT_PTR_NOT_NULL(foundNode1, "Search for existing value (20) should not return NULL.");
    ASSERT_INT(foundNode1->data, 20, "Found node should contain the correct data.");

    LLNode *foundNode2 = search(&list, 99);
    ASSERT_PTR_NULL(foundNode2, "Search for non-existent value (99) should return NULL.");

    reverse(&list);
    s = list_to_string(&list);
    ASSERT_STRING(s, "[30] -> [20] -> [10]", "List should be reversed.");
    free(s);
    ASSERT_INT(list.head->data, 30, "Head should be 30 after reverse.");

    clear(&list);
    reverse(&list);
    ASSERT_PTR_NULL(list.head, "Reversing an empty list should not cause issues.");

    clear(&list);
}

void test_copy()
{
    printf("--- Testing Copy Operation ---\n");
    LinkedList list1 = init();
    insertAtEnd(&list1, 1);
    insertAtEnd(&list1, 2);
    insertAtEnd(&list1, 3);

    LinkedList list2 = copy(&list1);
    char *s1 = NULL, *s2 = NULL, *s3 = NULL;

    ASSERT_INT(list2.length, list1.length, "Copied list should have the same length.");
    s1 = list_to_string(&list1);
    s2 = list_to_string(&list2);
    ASSERT_STRING(s2, s1, "Copied list should have the same contents.");

    if (list1.head && list2.head)
    {
        ASSERT_PTR_NOT_NULL(list1.head, "Original list head should not be NULL.");
        ASSERT_PTR_NOT_NULL(list2.head, "Copied list head should not be NULL.");
        ASSERT_VERBOSE(list1.head != list2.head, "Heads of original and copy should be different objects (deep copy).", "true", (list1.head != list2.head) ? "true" : "false");
    }

    deleteStart(&list1);
    s3 = list_to_string(&list2);
    ASSERT_STRING(s3, "[1] -> [2] -> [3]", "Copied list should not be affected by changes to original.");

    free(s1);
    free(s2);
    free(s3);
    clear(&list1);
    clear(&list2);
}

int main()
{
    printf(CYAN "\n===== LINKED LIST TEST SUITE =====\n\n" RESET);

    test_initialization_and_clear();
    test_insertion();
    test_deletion();
    test_search_and_reverse();
    test_copy();

    printf(CYAN "\n===== TEST SUMMARY =====\n" RESET);
    printf("Total tests run   : %d\n", tests_run);

    if (tests_run == tests_passed)
    {
        printf(GREEN "All tests passed! \n\n" RESET);
    }
    else
    {
        printf(GREEN "Total tests passed: %d\n" RESET, tests_passed);
        printf(RED "Total tests failed: %d\n\n" RESET, tests_run - tests_passed);
    }

    return tests_run - tests_passed;
}