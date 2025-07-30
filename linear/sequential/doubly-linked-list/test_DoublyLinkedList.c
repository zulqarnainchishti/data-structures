#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "adt_DoublyLinkedList.h"

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
        printf("   Expected: \"%s\"\n", expected_str);      \
        printf("   Actual   : \"%s\"\n", actual_str);       \
        if (expr)                                           \
        {                                                   \
            printf(GREEN "   Result   : PASS\n\n" RESET);   \
            tests_passed++;                                 \
        }                                                   \
        else                                                \
        {                                                   \
            printf(RED "   Result   : FAIL\n\n" RESET);     \
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

// Helper function to convert DoublyLinkedList to string for assertions
char *list_to_string(const DoublyLinkedList *list)
{
    if (list == NULL || list->head == NULL)
    {
        char *s = (char *)malloc(8 * sizeof(char)); // For "[empty]"
        if (!s)
            return NULL;
        strcpy(s, "[empty]");
        return s;
    }
    // Allocate buffer: length * (data_str_len + " <=> ") + null_terminator + safety
    // Max data str len for int is about 11 (e.g., -2147483648)
    // "[%d]" + " <=> " = ~15 characters per node
    char *buffer = (char *)malloc((list->length * 15 + 10) * sizeof(char));
    if (!buffer)
        return NULL;
    buffer[0] = '\0';
    DLLNode *temp = list->head;
    while (temp != NULL)
    {
        char node_str[20];
        sprintf(node_str, "[%d]", temp->data);
        strcat(buffer, node_str);
        temp = temp->next;
        if (temp != NULL)
        {
            strcat(buffer, " <=> ");
        }
    }
    return buffer;
}

void test_initialization_and_clear()
{
    printf("--- Testing Initialization and Clear ---\n");
    DoublyLinkedList list = init();
    ASSERT_PTR_NULL(list.head, "init() should create a list with a NULL head.");
    ASSERT_PTR_NULL(list.tail, "init() should create a list with a NULL tail.");
    ASSERT_INT(list.length, 0, "init() should create a list with length 0.");
    char *s = list_to_string(&list);
    ASSERT_STRING(s, "[empty]", "Empty list string representation.");
    free(s);

    clear(&list);
    ASSERT_PTR_NULL(list.head, "clear() on an empty list should result in a NULL head.");
    ASSERT_PTR_NULL(list.tail, "clear() on an empty list should result in a NULL tail.");
    ASSERT_INT(list.length, 0, "clear() on an empty list should result in length 0.");
    s = list_to_string(&list);
    ASSERT_STRING(s, "[empty]", "Empty list string representation after clear.");
    free(s);

    insertAtStart(&list, 10);
    insertAtStart(&list, 20);
    clear(&list);
    ASSERT_PTR_NULL(list.head, "clear() on a populated list should result in a NULL head.");
    ASSERT_PTR_NULL(list.tail, "clear() on a populated list should result in a NULL tail.");
    ASSERT_INT(list.length, 0, "clear() on a populated list should result in length 0.");
}

void test_insertion()
{
    printf("--- Testing Insertion Operations ---\n");
    DoublyLinkedList list = init();
    char *s = NULL;

    // Test insertAtEnd
    insertAtEnd(&list, 10);
    ASSERT_INT(list.length, 1, "Length should be 1 after one insertAtEnd.");
    s = list_to_string(&list);
    ASSERT_STRING(s, "[10]", "List should be '[10]' after one insertAtEnd.");
    free(s);
    ASSERT_PTR_NOT_NULL(list.head, "Head should not be NULL.");
    ASSERT_PTR_NOT_NULL(list.tail, "Tail should not be NULL.");
    ASSERT_INT(list.head->data, 10, "Head data should be 10.");
    ASSERT_INT(list.tail->data, 10, "Tail data should be 10.");
    ASSERT_PTR_NULL(list.head->prev, "Head prev should be NULL.");
    ASSERT_PTR_NULL(list.tail->next, "Tail next should be NULL.");

    insertAtEnd(&list, 20);
    ASSERT_INT(list.length, 2, "Length should be 2 after second insertAtEnd.");
    s = list_to_string(&list);
    ASSERT_STRING(s, "[10] <=> [20]", "List should be '[10] <=> [20]'.");
    free(s);
    ASSERT_INT(list.tail->data, 20, "Tail data should be 20.");
    ASSERT_PTR_NULL(list.tail->next, "Tail next should be NULL.");
    ASSERT_INT(list.head->next->data, 20, "Head next should be 20.");
    ASSERT_INT(list.tail->prev->data, 10, "Tail prev should be 10.");

    // Test insertAtStart
    insertAtStart(&list, 5);
    ASSERT_INT(list.length, 3, "Length should be 3 after insertAtStart.");
    s = list_to_string(&list);
    ASSERT_STRING(s, "[5] <=> [10] <=> [20]", "List should be '[5] <=> [10] <=> [20]'.");
    free(s);
    ASSERT_INT(list.head->data, 5, "Head data should be 5.");
    ASSERT_PTR_NULL(list.head->prev, "Head prev should be NULL.");
    ASSERT_INT(list.head->next->data, 10, "Head next should be 10.");
    ASSERT_INT(list.tail->data, 20, "Tail data should be 20.");

    // Test insertAtIndex
    insertAtIndex(&list, 99, 0); // Insert at start
    s = list_to_string(&list);
    ASSERT_STRING(s, "[99] <=> [5] <=> [10] <=> [20]", "insertAtIndex(0) should insert at start.");
    free(s);
    ASSERT_INT(list.head->data, 99, "Head data should be 99 after insertAtIndex(0).");
    ASSERT_PTR_NULL(list.head->prev, "New head prev should be NULL.");
    ASSERT_INT(list.head->next->data, 5, "New head next should be 5.");

    insertAtIndex(&list, 44, list.length); // Insert at end (index >= length)
    s = list_to_string(&list);
    ASSERT_STRING(s, "[99] <=> [5] <=> [10] <=> [20] <=> [44]", "insertAtIndex(length) should insert at end.");
    free(s);
    ASSERT_INT(list.tail->data, 44, "Tail data should be 44 after insertAtIndex(length).");
    ASSERT_PTR_NULL(list.tail->next, "New tail next should be NULL.");
    ASSERT_INT(list.tail->prev->data, 20, "New tail prev should be 20.");

    insertAtIndex(&list, 7, 2); // Insert in middle
    s = list_to_string(&list);
    ASSERT_STRING(s, "[99] <=> [5] <=> [7] <=> [10] <=> [20] <=> [44]", "insertAtIndex(middle) should insert correctly.");
    free(s);
    ASSERT_INT(list.length, 6, "Length should be 6 after all insertions.");
    // Verify prev/next links around the inserted node (7)
    DLLNode *node7 = search(&list, 7);
    ASSERT_PTR_NOT_NULL(node7, "Node 7 should exist.");
    ASSERT_INT(node7->prev->data, 5, "Node 7 prev should be 5.");
    ASSERT_INT(node7->next->data, 10, "Node 7 next should be 10.");
    ASSERT_INT(node7->next->prev->data, 7, "Node 10 prev should be 7.");
    ASSERT_INT(node7->prev->next->data, 7, "Node 5 next should be 7.");

    // Test insertAfterValue
    insertAfterValue(&list, 8, 7);
    s = list_to_string(&list);
    ASSERT_STRING(s, "[99] <=> [5] <=> [7] <=> [8] <=> [10] <=> [20] <=> [44]", "insertAfterValue should insert correctly.");
    free(s);
    ASSERT_INT(list.length, 7, "Length should be 7 after insertAfterValue.");
    // Verify prev/next links around inserted node (8)
    DLLNode *node8 = search(&list, 8);
    ASSERT_PTR_NOT_NULL(node8, "Node 8 should exist.");
    ASSERT_INT(node8->prev->data, 7, "Node 8 prev should be 7.");
    ASSERT_INT(node8->next->data, 10, "Node 8 next should be 10.");
    ASSERT_INT(node8->next->prev->data, 8, "Node 10 prev should be 8.");
    ASSERT_INT(node8->prev->next->data, 8, "Node 7 next should be 8.");

    insertAfterValue(&list, 45, 44); // Insert after tail
    s = list_to_string(&list);
    ASSERT_STRING(s, "[99] <=> [5] <=> [7] <=> [8] <=> [10] <=> [20] <=> [44] <=> [45]", "insertAfterValue after tail should work.");
    free(s);
    ASSERT_INT(list.tail->data, 45, "New tail data should be 45.");
    ASSERT_PTR_NULL(list.tail->next, "New tail next should be NULL.");
    ASSERT_INT(list.tail->prev->data, 44, "New tail prev should be 44.");

    int status = insertAfterValue(&list, 100, 1000);
    ASSERT_INT(status, -1, "insertAfterValue should return -1 for non-existent value.");
    ASSERT_INT(list.length, 8, "Length should not change after failed insertion."); // Length remains 8 from previous test

    clear(&list);
}

void test_deletion()
{
    printf("--- Testing Deletion Operations ---\n");
    DoublyLinkedList list = init();
    char *s = NULL;

    // Test delete from empty list
    ASSERT_INT(deleteStart(&list), -1, "deleteStart on empty list should return -1.");
    ASSERT_INT(deleteEnd(&list), -1, "deleteEnd on empty list should return -1.");
    ASSERT_INT(deleteIndex(&list, 0), -1, "deleteIndex on empty list should return -1.");
    ASSERT_INT(deleteValue(&list, 1), -1, "deleteValue on empty list should return -1.");

    insertAtEnd(&list, 10); // List: [10]
    insertAtEnd(&list, 20); // List: [10] <=> [20]
    insertAtEnd(&list, 30); // List: [10] <=> [20] <=> [30]
    insertAtEnd(&list, 40); // List: [10] <=> [20] <=> [30] <=> [40]
    insertAtEnd(&list, 50); // List: [10] <=> [20] <=> [30] <=> [40] <=> [50]

    // Test deleteStart
    int val1 = deleteStart(&list);
    ASSERT_INT(val1, 10, "deleteStart should return the first element's value.");
    s = list_to_string(&list);
    ASSERT_STRING(s, "[20] <=> [30] <=> [40] <=> [50]", "List state after deleteStart.");
    free(s);
    ASSERT_INT(list.length, 4, "Length should be 4 after deleteStart.");
    ASSERT_INT(list.head->data, 20, "New head data should be 20.");
    ASSERT_PTR_NULL(list.head->prev, "New head prev should be NULL.");

    // Test deleteEnd
    int val2 = deleteEnd(&list);
    ASSERT_INT(val2, 50, "deleteEnd should return the last element's value.");
    s = list_to_string(&list);
    ASSERT_STRING(s, "[20] <=> [30] <=> [40]", "List state after deleteEnd.");
    free(s);
    ASSERT_INT(list.length, 3, "Length should be 3 after deleteEnd.");
    ASSERT_INT(list.tail->data, 40, "New tail data should be 40.");
    ASSERT_PTR_NULL(list.tail->next, "New tail next should be NULL.");

    // Test deleteIndex
    int val3 = deleteIndex(&list, 1); // Delete 30
    ASSERT_INT(val3, 30, "deleteIndex(1) should return the correct value.");
    ASSERT_INT(list.length, 2, "Length should decrease after deleteIndex.");
    s = list_to_string(&list);
    ASSERT_STRING(s, "[20] <=> [40]", "List state after deleteIndex(1).");
    free(s);
    // Verify prev/next links after deleting 30 (node 20's next, node 40's prev)
    ASSERT_INT(list.head->next->data, 40, "20's next should be 40.");
    ASSERT_INT(list.tail->prev->data, 20, "40's prev should be 20.");

    // Test deleteValue
    int status_del = deleteValue(&list, 20); // Delete head by value
    ASSERT_INT(status_del, 0, "deleteValue for existing head should return 0.");
    s = list_to_string(&list);
    ASSERT_STRING(s, "[40]", "List state after deleting head by value (20).");
    free(s);
    ASSERT_INT(list.length, 1, "Length should be 1 after deleting head by value.");
    ASSERT_INT(list.head->data, 40, "Head should be 40.");
    ASSERT_PTR_NULL(list.head->prev, "Head prev should be NULL.");
    ASSERT_PTR_NULL(list.head->next, "Head next should be NULL.");
    ASSERT_INT(list.tail->data, 40, "Tail should be 40.");

    status_del = deleteValue(&list, 999);
    ASSERT_INT(status_del, -1, "deleteValue for non-existent value should return -1.");
    ASSERT_INT(list.length, 1, "Length should not change after failed deletion.");

    // Test deleting the last element
    deleteEnd(&list); // List becomes empty
    s = list_to_string(&list);
    ASSERT_STRING(s, "[empty]", "List should be empty after deleting last element.");
    free(s);
    ASSERT_PTR_NULL(list.head, "deleteEnd on single-element list makes head NULL.");
    ASSERT_PTR_NULL(list.tail, "deleteEnd on single-element list makes tail NULL.");
    ASSERT_INT(list.length, 0, "Length should be 0 after deleting last element.");

    clear(&list);
}

void test_search_and_reverse()
{
    printf("--- Testing Search and Reverse ---\n");
    DoublyLinkedList list = init();
    char *s = NULL;

    // Test search
    insertAtEnd(&list, 10);
    insertAtEnd(&list, 20);
    insertAtEnd(&list, 30); // List: [10] <=> [20] <=> [30]

    DLLNode *foundNode1 = search(&list, 20);
    ASSERT_PTR_NOT_NULL(foundNode1, "Search for existing value (20) should not return NULL.");
    ASSERT_INT(foundNode1->data, 20, "Found node should contain the correct data.");

    DLLNode *foundNode2 = search(&list, 99);
    ASSERT_PTR_NULL(foundNode2, "Search for non-existent value (99) should return NULL.");

    // Test reverse
    reverse(&list);
    s = list_to_string(&list);
    ASSERT_STRING(s, "[30] <=> [20] <=> [10]", "List should be reversed.");
    free(s);
    ASSERT_INT(list.head->data, 30, "Head should be 30 after reverse.");
    ASSERT_INT(list.tail->data, 10, "Tail should be 10 after reverse.");
    // Verify prev/next links after reversal
    ASSERT_PTR_NULL(list.head->prev, "New head prev should be NULL.");
    ASSERT_INT(list.head->next->data, 20, "New head next should be 20.");
    ASSERT_PTR_NULL(list.tail->next, "New tail next should be NULL.");
    ASSERT_INT(list.tail->prev->data, 20, "New tail prev should be 20.");
    DLLNode *middle_node = list.head->next; // Node 20
    ASSERT_INT(middle_node->data, 20, "Middle node should be 20.");
    ASSERT_INT(middle_node->prev->data, 30, "Middle node's prev should be 30.");
    ASSERT_INT(middle_node->next->data, 10, "Middle node's next should be 10.");

    // Test reverse on empty list
    clear(&list);
    reverse(&list);
    ASSERT_PTR_NULL(list.head, "Reversing an empty list should result in NULL head.");
    ASSERT_PTR_NULL(list.tail, "Reversing an empty list should result in NULL tail.");
    ASSERT_INT(list.length, 0, "Reversing an empty list should maintain length 0.");

    // Test reverse on single element list
    insertAtStart(&list, 5); // List: [5]
    reverse(&list);
    s = list_to_string(&list);
    ASSERT_STRING(s, "[5]", "Reversing a single-element list should not change it.");
    free(s);
    ASSERT_INT(list.head->data, 5, "Head should remain 5.");
    ASSERT_INT(list.tail->data, 5, "Tail should remain 5.");
    ASSERT_PTR_NULL(list.head->prev, "Head prev should be NULL.");
    ASSERT_PTR_NULL(list.tail->next, "Tail next should be NULL.");

    clear(&list);
}

void test_copy()
{
    printf("--- Testing Copy Operation ---\n");
    DoublyLinkedList list1 = init();
    insertAtEnd(&list1, 1);
    insertAtEnd(&list1, 2);
    insertAtEnd(&list1, 3); // list1: [1] <=> [2] <=> [3]

    DoublyLinkedList list2 = copy(&list1);
    char *s1 = NULL, *s2 = NULL, *s3 = NULL;

    ASSERT_INT(list2.length, list1.length, "Copied list should have the same length.");
    s1 = list_to_string(&list1);
    s2 = list_to_string(&list2);
    ASSERT_STRING(s2, s1, "Copied list should have the same contents.");
    free(s1);
    free(s2);

    // Deep copy verification
    if (list1.head && list2.head)
    {
        ASSERT_PTR_NOT_NULL(list1.head, "Original list head should not be NULL.");
        ASSERT_PTR_NOT_NULL(list2.head, "Copied list head should not be NULL.");
        ASSERT_VERBOSE(list1.head != list2.head, "Heads of original and copy should be different objects (deep copy).", "true", (list1.head != list2.head) ? "true" : "false");
        ASSERT_VERBOSE(list1.head->next != list2.head->next, "Next pointers of original and copy heads should be different objects (deep copy).", "true", (list1.head->next != list2.head->next) ? "true" : "false");
        ASSERT_VERBOSE(list1.tail != list2.tail, "Tails of original and copy should be different objects (deep copy).", "true", (list1.tail != list2.tail) ? "true" : "false");

        // Verify prev pointers in copied list
        DLLNode *curr1 = list1.head;
        DLLNode *curr2 = list2.head;
        while (curr1 != NULL)
        {
            if (curr1->prev != NULL)
            {
                ASSERT_PTR_NOT_NULL(curr2->prev, "Copied node's prev should not be NULL if original's isn't.");
                ASSERT_INT(curr2->prev->data, curr1->prev->data, "Copied node's prev data should match original.");
            }
            else
            {
                ASSERT_PTR_NULL(curr2->prev, "Copied node's prev should be NULL if original's is.");
            }
            curr1 = curr1->next;
            curr2 = curr2->next;
        }
    }

    deleteStart(&list1);         // Modify original list
    s3 = list_to_string(&list2); // Check copied list
    ASSERT_STRING(s3, "[1] <=> [2] <=> [3]", "Copied list should not be affected by changes to original.");
    free(s3);

    // Test copying an empty list
    DoublyLinkedList empty_list = init();
    DoublyLinkedList copied_empty_list = copy(&empty_list);
    ASSERT_PTR_NULL(copied_empty_list.head, "Copied empty list should have NULL head.");
    ASSERT_PTR_NULL(copied_empty_list.tail, "Copied empty list should have NULL tail.");
    ASSERT_INT(copied_empty_list.length, 0, "Copied empty list should have length 0.");

    clear(&list1);
    clear(&list2);
    clear(&empty_list);
    clear(&copied_empty_list);
}

int main()
{
    printf(CYAN "\n===== DOUBLY LINKED LIST TEST SUITE =====\n\n" RESET);

    test_initialization_and_clear();
    test_insertion();
    test_deletion();
    test_search_and_reverse();
    test_copy();

    printf(CYAN "\n===== TEST SUMMARY =====\n" RESET);
    printf("Total tests run    : %d\n", tests_run);

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