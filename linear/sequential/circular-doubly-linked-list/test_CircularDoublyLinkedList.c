#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "adt_CircularDoublyLinkedList.h" // Assuming your CDLL header is named this

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

char *cdll_to_string(const CircularDoublyLinkedList *list)
{
    if (list == NULL || list->head == NULL)
    {
        char *s = (char *)malloc(8 * sizeof(char));
        strcpy(s, "[empty]");
        return s;
    }
    char *buffer = (char *)malloc((list->length * 20 + 10) * sizeof(char));
    if (!buffer)
        return NULL;
    buffer[0] = '\0';
    CDLLNode *curr = list->head;
    do
    {
        char node_str[20];
        sprintf(node_str, "[%d]", curr->data);
        strcat(buffer, node_str);
        curr = curr->next;
        if (curr != list->head)
        {
            strcat(buffer, " <=> ");
        }
    } while (curr != list->head);
    return buffer;
}

void test_init_and_clear()
{
    printf("--- Test: Initialization and Clear ---\n");
    CircularDoublyLinkedList list = init();
    ASSERT_PTR_NULL(list.head, "init() should create a list with a NULL head.");
    ASSERT_INT(list.length, 0, "init() should create a list with length 0.");

    char *s = cdll_to_string(&list);
    ASSERT_STRING(s, "[empty]", "Empty list string representation.");
    free(s);

    clear(&list);
    ASSERT_PTR_NULL(list.head, "clear() on empty list should keep head NULL.");
    ASSERT_INT(list.length, 0, "clear() on empty list should keep length 0.");

    insertAtEnd(&list, 1);
    insertAtEnd(&list, 2);
    insertAtStart(&list, 0);

    ASSERT_INT(list.length, 3, "Length is 3 before clear.");
    s = cdll_to_string(&list);
    ASSERT_STRING(s, "[0] <=> [1] <=> [2]", "List content before clear.");
    free(s);

    clear(&list);
    ASSERT_PTR_NULL(list.head, "clear() on populated list should set head to NULL.");
    ASSERT_INT(list.length, 0, "clear() on populated list should set length to 0.");
    s = cdll_to_string(&list);
    ASSERT_STRING(s, "[empty]", "List content after clear.");
    free(s);
}

void test_insert_at_start()
{
    printf("--- Test: insertAtStart ---\n");
    CircularDoublyLinkedList list = init();
    char *s = NULL;

    insertAtStart(&list, 10);
    ASSERT_INT(list.length, 1, "Length should be 1 after first insertAtStart.");
    s = cdll_to_string(&list);
    ASSERT_STRING(s, "[10]", "List should be '[10]' after one insertAtStart.");
    free(s);
    ASSERT_INT(list.head->data, 10, "Head should be 10 for single node list.");
    ASSERT_INT(list.head->prev->data, 10, "Tail should be 10 for single node list.");

    insertAtStart(&list, 5);
    ASSERT_INT(list.length, 2, "Length should be 2 after second insertAtStart.");
    s = cdll_to_string(&list);
    ASSERT_STRING(s, "[5] <=> [10]", "List should be '[5] <=> [10]'.");
    free(s);
    ASSERT_INT(list.head->data, 5, "Head should be 5.");
    ASSERT_INT(list.head->prev->data, 10, "Tail should be 10.");

    insertAtStart(&list, 1);
    ASSERT_INT(list.length, 3, "Length should be 3 after third insertAtStart.");
    s = cdll_to_string(&list);
    ASSERT_STRING(s, "[1] <=> [5] <=> [10]", "List should be '[1] <=> [5] <=> [10]'.");
    free(s);
    ASSERT_INT(list.head->data, 1, "Head should be 1.");
    ASSERT_INT(list.head->prev->data, 10, "Tail should be 10.");

    clear(&list);
}

void test_insert_at_end()
{
    printf("--- Test: insertAtEnd ---\n");
    CircularDoublyLinkedList list = init();
    char *s = NULL;

    insertAtEnd(&list, 10);
    ASSERT_INT(list.length, 1, "Length should be 1 after first insertAtEnd.");
    s = cdll_to_string(&list);
    ASSERT_STRING(s, "[10]", "List should be '[10]' after one insertAtEnd.");
    free(s);
    ASSERT_INT(list.head->data, 10, "Head should be 10 for single node list.");
    ASSERT_INT(list.head->prev->data, 10, "Tail should be 10 for single node list.");

    insertAtEnd(&list, 20);
    ASSERT_INT(list.length, 2, "Length should be 2 after second insertAtEnd.");
    s = cdll_to_string(&list);
    ASSERT_STRING(s, "[10] <=> [20]", "List should be '[10] <=> [20]'.");
    free(s);
    ASSERT_INT(list.head->data, 10, "Head should be 10.");
    ASSERT_INT(list.head->prev->data, 20, "Tail should be 20.");

    insertAtEnd(&list, 30);
    ASSERT_INT(list.length, 3, "Length should be 3 after third insertAtEnd.");
    s = cdll_to_string(&list);
    ASSERT_STRING(s, "[10] <=> [20] <=> [30]", "List should be '[10] <=> [20] <=> [30]'.");
    free(s);
    ASSERT_INT(list.head->data, 10, "Head should be 10.");
    ASSERT_INT(list.head->prev->data, 30, "Tail should be 30.");

    clear(&list);
}

void test_insert_at_index()
{
    printf("--- Test: insertAtIndex ---\n");
    CircularDoublyLinkedList list = init();
    char *s = NULL;

    insertAtIndex(&list, 50, 0);
    ASSERT_INT(list.length, 1, "Length should be 1 after insertAtIndex(0) on empty.");
    s = cdll_to_string(&list);
    ASSERT_STRING(s, "[50]", "List should be '[50]' after insertAtIndex(0) on empty.");
    free(s);
    ASSERT_INT(list.head->data, 50, "Head should be 50.");
    ASSERT_INT(list.head->prev->data, 50, "Tail should be 50.");

    insertAtIndex(&list, 10, 0);
    ASSERT_INT(list.length, 2, "Length should be 2 after insertAtIndex(0).");
    s = cdll_to_string(&list);
    ASSERT_STRING(s, "[10] <=> [50]", "List should be '[10] <=> [50]' after insertAtIndex(0).");
    free(s);
    ASSERT_INT(list.head->data, 10, "Head should be 10.");
    ASSERT_INT(list.head->prev->data, 50, "Tail should be 50.");

    insertAtIndex(&list, 100, 5);
    ASSERT_INT(list.length, 3, "Length should be 3 after insertAtIndex(end).");
    s = cdll_to_string(&list);
    ASSERT_STRING(s, "[10] <=> [50] <=> [100]", "List should be '[10] <=> [50] <=> [100]'.");
    free(s);
    ASSERT_INT(list.head->prev->data, 100, "Tail should be 100.");

    insertAtIndex(&list, 75, 2);
    ASSERT_INT(list.length, 4, "Length should be 4 after insertAtIndex(middle).");
    s = cdll_to_string(&list);
    ASSERT_STRING(s, "[10] <=> [50] <=> [75] <=> [100]", "List should be '[10] <=> [50] <=> [75] <=> [100]'.");
    free(s);
    ASSERT_INT(list.head->prev->data, 100, "Tail should still be 100.");

    clear(&list);
}

void test_insert_after_value()
{
    printf("--- Test: insertAfterValue ---\n");
    CircularDoublyLinkedList list = init();
    char *s = NULL;

    insertAtEnd(&list, 10);
    insertAtEnd(&list, 20);
    insertAtEnd(&list, 30);
    insertAtEnd(&list, 40);
    s = cdll_to_string(&list);
    ASSERT_STRING(s, "[10] <=> [20] <=> [30] <=> [40]", "Initial list for insertAfterValue.");
    free(s);

    int status = insertAfterValue(&list, 25, 20);
    ASSERT_INT(status, 0, "insertAfterValue should return 0 on success.");
    ASSERT_INT(list.length, 5, "Length should increase after insertAfterValue.");
    s = cdll_to_string(&list);
    ASSERT_STRING(s, "[10] <=> [20] <=> [25] <=> [30] <=> [40]", "List after inserting 25 after 20.");
    free(s);
    ASSERT_INT(list.head->prev->data, 40, "Tail should remain 40.");

    status = insertAfterValue(&list, 45, 40);
    ASSERT_INT(status, 0, "insertAfterValue should return 0 on success after tail.");
    ASSERT_INT(list.length, 6, "Length should increase after insertAfterValue after tail.");
    s = cdll_to_string(&list);
    ASSERT_STRING(s, "[10] <=> [20] <=> [25] <=> [30] <=> [40] <=> [45]", "List after inserting 45 after 40 (new tail).");
    free(s);
    ASSERT_INT(list.head->prev->data, 45, "Tail should now be 45.");

    status = insertAfterValue(&list, 5, 10);
    ASSERT_INT(status, 0, "insertAfterValue should return 0 on success after head.");
    ASSERT_INT(list.length, 7, "Length should increase after insertAfterValue after head.");
    s = cdll_to_string(&list);
    ASSERT_STRING(s, "[10] <=> [5] <=> [20] <=> [25] <=> [30] <=> [40] <=> [45]", "List after inserting 5 after 10.");
    free(s);
    ASSERT_INT(list.head->data, 10, "Head should remain 10.");

    status = insertAfterValue(&list, 99, 999);
    ASSERT_INT(status, -1, "insertAfterValue should return -1 for non-existent value.");
    ASSERT_INT(list.length, 7, "Length should not change after failed insertAfterValue.");
    s = cdll_to_string(&list);
    ASSERT_STRING(s, "[10] <=> [5] <=> [20] <=> [25] <=> [30] <=> [40] <=> [45]", "List should be unchanged after failed insert.");
    free(s);

    clear(&list);
}

void test_delete_start()
{
    printf("--- Test: deleteStart ---\n");
    CircularDoublyLinkedList list = init();
    char *s = NULL;

    int deleted_val = deleteStart(&list);
    ASSERT_INT(deleted_val, -1, "deleteStart on empty list should return -1.");
    ASSERT_INT(list.length, 0, "Length should remain 0 for empty list.");
    ASSERT_PTR_NULL(list.head, "Head should remain NULL for empty list.");

    insertAtEnd(&list, 100);
    deleted_val = deleteStart(&list);
    ASSERT_INT(deleted_val, 100, "deleteStart on single-node list should return its value.");
    ASSERT_INT(list.length, 0, "Length should be 0 after deleting last node.");
    ASSERT_PTR_NULL(list.head, "Head should be NULL after deleting last node.");
    s = cdll_to_string(&list);
    ASSERT_STRING(s, "[empty]", "List should be empty after deleting last node.");
    free(s);

    insertAtEnd(&list, 10);
    insertAtEnd(&list, 20);
    insertAtEnd(&list, 30);
    s = cdll_to_string(&list);
    ASSERT_STRING(s, "[10] <=> [20] <=> [30]", "Initial multi-node list for deleteStart.");
    free(s);

    deleted_val = deleteStart(&list);
    ASSERT_INT(deleted_val, 10, "deleteStart should remove the first element (10).");
    ASSERT_INT(list.length, 2, "Length should be 2 after deleteStart.");
    s = cdll_to_string(&list);
    ASSERT_STRING(s, "[20] <=> [30]", "List after deleteStart (10).");
    free(s);
    ASSERT_INT(list.head->data, 20, "New head should be 20.");
    ASSERT_INT(list.head->prev->data, 30, "Tail should remain 30.");

    deleted_val = deleteStart(&list);
    ASSERT_INT(deleted_val, 20, "deleteStart should remove the new first element (20).");
    ASSERT_INT(list.length, 1, "Length should be 1 after second deleteStart.");
    s = cdll_to_string(&list);
    ASSERT_STRING(s, "[30]", "List after deleteStart (20).");
    free(s);
    ASSERT_INT(list.head->data, 30, "New head should be 30.");
    ASSERT_INT(list.head->prev->data, 30, "Tail should be 30.");

    clear(&list);
}

void test_delete_end()
{
    printf("--- Test: deleteEnd ---\n");
    CircularDoublyLinkedList list = init();
    char *s = NULL;

    int deleted_val = deleteEnd(&list);
    ASSERT_INT(deleted_val, -1, "deleteEnd on empty list should return -1.");
    ASSERT_INT(list.length, 0, "Length should remain 0 for empty list.");
    ASSERT_PTR_NULL(list.head, "Head should remain NULL for empty list.");

    insertAtEnd(&list, 100);
    deleted_val = deleteEnd(&list);
    ASSERT_INT(deleted_val, 100, "deleteEnd on single-node list should return its value.");
    ASSERT_INT(list.length, 0, "Length should be 0 after deleting last node.");
    ASSERT_PTR_NULL(list.head, "Head should be NULL after deleting last node.");
    s = cdll_to_string(&list);
    ASSERT_STRING(s, "[empty]", "List should be empty after deleting last node.");
    free(s);

    insertAtEnd(&list, 10);
    insertAtEnd(&list, 20);
    insertAtEnd(&list, 30);
    s = cdll_to_string(&list);
    ASSERT_STRING(s, "[10] <=> [20] <=> [30]", "Initial multi-node list for deleteEnd.");
    free(s);

    deleted_val = deleteEnd(&list);
    ASSERT_INT(deleted_val, 30, "deleteEnd should remove the last element (30).");
    ASSERT_INT(list.length, 2, "Length should be 2 after deleteEnd.");
    s = cdll_to_string(&list);
    ASSERT_STRING(s, "[10] <=> [20]", "List after deleteEnd (30).");
    free(s);
    ASSERT_INT(list.head->data, 10, "Head should remain 10.");
    ASSERT_INT(list.head->prev->data, 20, "New tail should be 20.");

    deleted_val = deleteEnd(&list);
    ASSERT_INT(deleted_val, 20, "deleteEnd should remove the new last element (20).");
    ASSERT_INT(list.length, 1, "Length should be 1 after second deleteEnd.");
    s = cdll_to_string(&list);
    ASSERT_STRING(s, "[10]", "List after deleteEnd (20).");
    free(s);
    ASSERT_INT(list.head->data, 10, "Head should be 10.");
    ASSERT_INT(list.head->prev->data, 10, "Tail should be 10.");

    clear(&list);
}

void test_delete_index()
{
    printf("--- Test: deleteIndex ---\n");
    CircularDoublyLinkedList list = init();
    char *s = NULL;

    int deleted_val = deleteIndex(&list, 0);
    ASSERT_INT(deleted_val, -1, "deleteIndex on empty list should return -1.");

    insertAtEnd(&list, 10);
    insertAtEnd(&list, 20);
    insertAtEnd(&list, 30);
    insertAtEnd(&list, 40);

    s = cdll_to_string(&list);
    ASSERT_STRING(s, "[10] <=> [20] <=> [30] <=> [40]", "Initial list for deleteIndex.");
    free(s);

    deleted_val = deleteIndex(&list, 0);
    ASSERT_INT(deleted_val, 10, "deleteIndex(0) should remove 10.");
    ASSERT_INT(list.length, 3, "Length should be 3.");
    s = cdll_to_string(&list);
    ASSERT_STRING(s, "[20] <=> [30] <=> [40]", "List after deleteIndex(0).");
    free(s);
    ASSERT_INT(list.head->data, 20, "Head should be 20.");

    deleted_val = deleteIndex(&list, 2);
    ASSERT_INT(deleted_val, 40, "deleteIndex(2) should remove 40.");
    ASSERT_INT(list.length, 2, "Length should be 2.");
    s = cdll_to_string(&list);
    ASSERT_STRING(s, "[20] <=> [30]", "List after deleteIndex(2).");
    free(s);
    ASSERT_INT(list.head->prev->data, 30, "Tail should be 30.");

    deleted_val = deleteIndex(&list, 0);
    ASSERT_INT(deleted_val, 20, "deleteIndex(0) on [20]<=>[30] should remove 20.");
    ASSERT_INT(list.length, 1, "Length should be 1.");
    s = cdll_to_string(&list);
    ASSERT_STRING(s, "[30]", "List after deleteIndex(0).");
    free(s);
    ASSERT_INT(list.head->data, 30, "Head should be 30.");

    deleted_val = deleteIndex(&list, 0);
    ASSERT_INT(deleted_val, 30, "deleteIndex(0) on single element list should remove 30.");
    ASSERT_INT(list.length, 0, "Length should be 0.");
    s = cdll_to_string(&list);
    ASSERT_STRING(s, "[empty]", "List should be empty.");
    free(s);

    insertAtEnd(&list, 10);
    insertAtEnd(&list, 20);
    insertAtEnd(&list, 30);
    deleted_val = deleteIndex(&list, 100);
    ASSERT_INT(deleted_val, 30, "deleteIndex(out_of_bounds) should remove end (30).");
    ASSERT_INT(list.length, 2, "Length should be 2.");
    s = cdll_to_string(&list);
    ASSERT_STRING(s, "[10] <=> [20]", "List after deleteIndex(out_of_bounds).");
    free(s);

    clear(&list);
}

void test_delete_value()
{
    printf("--- Test: deleteValue ---\n");
    CircularDoublyLinkedList list = init();
    char *s = NULL;

    int status = deleteValue(&list, 10);
    ASSERT_INT(status, -1, "deleteValue on empty list should return -1.");

    insertAtEnd(&list, 50);
    status = deleteValue(&list, 50);
    ASSERT_INT(status, 0, "deleteValue for the only element should return 0.");
    ASSERT_INT(list.length, 0, "Length should be 0 after deleting only element.");
    ASSERT_PTR_NULL(list.head, "Head should be NULL after deleting only element.");
    s = cdll_to_string(&list);
    ASSERT_STRING(s, "[empty]", "List should be empty after deleting only element.");
    free(s);

    insertAtEnd(&list, 10);
    insertAtEnd(&list, 20);
    insertAtEnd(&list, 30);
    insertAtEnd(&list, 20);
    insertAtEnd(&list, 40);

    s = cdll_to_string(&list);
    ASSERT_STRING(s, "[10] <=> [20] <=> [30] <=> [20] <=> [40]", "Initial list for deleteValue.");
    free(s);

    status = deleteValue(&list, 20);
    ASSERT_INT(status, 0, "deleteValue for middle element (first 20) should return 0.");
    ASSERT_INT(list.length, 4, "Length should be 4.");
    s = cdll_to_string(&list);
    ASSERT_STRING(s, "[10] <=> [30] <=> [20] <=> [40]", "List after deleting first 20.");
    free(s);

    status = deleteValue(&list, 10);
    ASSERT_INT(status, 0, "deleteValue for head (10) should return 0.");
    ASSERT_INT(list.length, 3, "Length should be 3.");
    s = cdll_to_string(&list);
    ASSERT_STRING(s, "[30] <=> [20] <=> [40]", "List after deleting 10 (new head 30).");
    free(s);
    ASSERT_INT(list.head->data, 30, "Head should be 30.");

    status = deleteValue(&list, 40);
    ASSERT_INT(status, 0, "deleteValue for tail (40) should return 0.");
    ASSERT_INT(list.length, 2, "Length should be 2.");
    s = cdll_to_string(&list);
    ASSERT_STRING(s, "[30] <=> [20]", "List after deleting 40 (new tail 20).");
    free(s);
    ASSERT_INT(list.head->prev->data, 20, "Tail should be 20.");

    status = deleteValue(&list, 999);
    ASSERT_INT(status, -1, "deleteValue for non-existent value should return -1.");
    ASSERT_INT(list.length, 2, "Length should be unchanged after failed delete.");

    clear(&list);
}

void test_search()
{
    printf("--- Test: search ---\n");
    CircularDoublyLinkedList list = init();

    CDLLNode *found_node = search(&list, 10);
    ASSERT_PTR_NULL(found_node, "Search on empty list should return NULL.");

    insertAtEnd(&list, 10);
    insertAtEnd(&list, 20);
    insertAtEnd(&list, 30);

    found_node = search(&list, 20);
    ASSERT_PTR_NOT_NULL(found_node, "Search for existing middle value (20) should return NOT NULL.");
    ASSERT_INT(found_node->data, 20, "Found node data should be 20.");

    found_node = search(&list, 10);
    ASSERT_PTR_NOT_NULL(found_node, "Search for existing head value (10) should return NOT NULL.");
    ASSERT_INT(found_node->data, 10, "Found node data should be 10.");

    found_node = search(&list, 30);
    ASSERT_PTR_NOT_NULL(found_node, "Search for existing tail value (30) should return NOT NULL.");
    ASSERT_INT(found_node->data, 30, "Found node data should be 30.");

    found_node = search(&list, 99);
    ASSERT_PTR_NULL(found_node, "Search for non-existent value (99) should return NULL.");

    clear(&list);
}

void test_reverse()
{
    printf("--- Test: reverse ---\n");
    CircularDoublyLinkedList list = init();
    char *s = NULL;

    reverse(&list);
    ASSERT_INT(list.length, 0, "Length should be 0 after reversing empty list.");
    ASSERT_PTR_NULL(list.head, "Head should be NULL after reversing empty list.");
    s = cdll_to_string(&list);
    ASSERT_STRING(s, "[empty]", "Empty list remains empty after reverse.");
    free(s);

    insertAtEnd(&list, 100);
    reverse(&list);
    ASSERT_INT(list.length, 1, "Length should be 1 after reversing single-node list.");
    s = cdll_to_string(&list);
    ASSERT_STRING(s, "[100]", "Single-node list remains unchanged after reverse.");
    free(s);
    ASSERT_INT(list.head->data, 100, "Head should still be 100.");
    ASSERT_INT(list.head->prev->data, 100, "Tail should still be 100.");
    clear(&list);

    insertAtEnd(&list, 10);
    insertAtEnd(&list, 20);
    insertAtEnd(&list, 30);
    insertAtEnd(&list, 40);

    s = cdll_to_string(&list);
    ASSERT_STRING(s, "[10] <=> [20] <=> [30] <=> [40]", "Original list before reverse.");
    free(s);

    reverse(&list);
    ASSERT_INT(list.length, 4, "Length should remain 4 after reverse.");
    s = cdll_to_string(&list);
    ASSERT_STRING(s, "[40] <=> [30] <=> [20] <=> [10]", "List should be reversed.");
    free(s);
    ASSERT_INT(list.head->prev->data, 10, "New tail should be original head (10).");
    ASSERT_INT(list.head->data, 40, "New head should be original tail (40).");

    reverse(&list);
    s = cdll_to_string(&list);
    ASSERT_STRING(s, "[10] <=> [20] <=> [30] <=> [40]", "List should be back to original after double reverse.");
    free(s);
    ASSERT_INT(list.head->prev->data, 40, "Tail should be 40.");
    ASSERT_INT(list.head->data, 10, "Head should be 10.");

    clear(&list);
}

void test_copy()
{
    printf("--- Test: copy ---\n");
    CircularDoublyLinkedList list1 = init();
    char *s1 = NULL, *s2 = NULL;

    CircularDoublyLinkedList list_empty_copy = copy(&list1);
    ASSERT_INT(list_empty_copy.length, 0, "Copied empty list should have length 0.");
    ASSERT_PTR_NULL(list_empty_copy.head, "Copied empty list should have NULL head.");
    s1 = cdll_to_string(&list_empty_copy);
    ASSERT_STRING(s1, "[empty]", "Copied empty list should be empty string.");
    free(s1);

    insertAtEnd(&list1, 10);
    insertAtEnd(&list1, 20);
    insertAtEnd(&list1, 30);

    s1 = cdll_to_string(&list1);
    ASSERT_STRING(s1, "[10] <=> [20] <=> [30]", "Original list1 before copy.");
    free(s1);

    CircularDoublyLinkedList list2 = copy(&list1);
    ASSERT_INT(list2.length, 3, "Copied list2 should have length 3.");
    s2 = cdll_to_string(&list2);
    ASSERT_STRING(s2, "[10] <=> [20] <=> [30]", "Copied list2 should have same content as list1.");
    free(s2);

    deleteStart(&list1);
    s1 = cdll_to_string(&list1);
    ASSERT_STRING(s1, "[20] <=> [30]", "Original list1 after modification.");
    free(s1);

    s2 = cdll_to_string(&list2);
    ASSERT_STRING(s2, "[10] <=> [20] <=> [30]", "Copied list2 should be unaffected by original's modification.");
    free(s2);

    if (list1.head && list2.head)
    {
        ASSERT_VERBOSE(list1.head != list2.head, "Head pointers should be different (deep copy).", "Different", "list1.head == list2.head ? \"Same\" : \"Different\"");
        ASSERT_VERBOSE(list1.head->prev != list2.head->prev, "Tail pointers should be different (deep copy).", "Different", "list1.head->prev == list2.head->prev ? \"Same\" : \"Different\"");

        CDLLNode *curr1 = list1.head;
        CDLLNode *curr2 = list2.head;
        bool all_nodes_different = true;
        do
        {
            if (curr1 == curr2)
            {
                all_nodes_different = false;
                break;
            }
            curr1 = curr1->next;
            curr2 = curr2->next;
        } while (curr1 != list1.head && curr2 != list2.head);

        ASSERT_VERBOSE(all_nodes_different, "All copied nodes should be new objects (deep copy).", "True", all_nodes_different ? "True" : "False");
    }

    clear(&list1);
    clear(&list2);
    clear(&list_empty_copy);
}

int main()
{
    printf(CYAN "\n===== CIRCULAR DOUBLY LINKED LIST TEST SUITE =====\n\n" RESET);

    test_init_and_clear();
    test_insert_at_start();
    test_insert_at_end();
    test_insert_at_index();
    test_insert_after_value();
    test_delete_start();
    test_delete_end();
    test_delete_index();
    test_delete_value();
    test_search();
    test_reverse();
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