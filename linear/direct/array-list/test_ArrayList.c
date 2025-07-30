#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h> // For assert() macro
#include <time.h>   // For clock()
#include <limits.h> // For INT_MAX, INT_MIN

#include "adt_ArrayList.h" // Include your header file

// --- Helper Functions for Testing ---

void print_test_status(const char *test_name, bool success)
{
    if (success)
    {
        printf("[PASS] %s\n", test_name);
    }
    else
    {
        printf("[FAIL] %s\n", test_name);
    }
}

// Function to check if an ArrayList is sorted in ascending order
bool is_sorted(ArrayList list)
{
    for (int i = 0; i < list.length - 1; i++)
    {
        if (list.array[i] > list.array[i + 1])
        {
            return false;
        }
    }
    return true;
}

// Custom function for map test: double the value
int double_value(int val)
{
    return val * 2;
}

// Custom function for filter test: check if even
bool is_even(int val)
{
    return val % 2 == 0;
}

// Custom function for filter test: check if positive
bool is_positive(int val)
{
    return val > 0;
}

// --- Test Cases ---

void test_init_and_destroy()
{
    printf("\n--- Test: init() and destroy() ---\n");
    ArrayList list = init(5);
    print_test_status("init(5) - length should be 0", list.length == 0);
    print_test_status("init(5) - capacity should be 5", list.capacity == 5);
    print_test_status("init(5) - array pointer should not be NULL", list.array != NULL);
    print_test_status("isEmpty() on new list", isEmpty(list) == true);
    print_test_status("isFull() on new list", isFull(list) == false);

    destroy(&list);
    print_test_status("destroy() - array pointer should be NULL", list.array == NULL);
    print_test_status("destroy() - length should be 0", list.length == 0);
    print_test_status("destroy() - capacity should be 0", list.capacity == 0);

    // Test init with capacity 0
    ArrayList empty_list = init(0);
    print_test_status("init(0) - length should be 0", empty_list.length == 0);
    print_test_status("init(0) - capacity should be 0", empty_list.capacity == 0);
    destroy(&empty_list);
}

void test_append_and_expand()
{
    printf("\n--- Test: append() and automatic expansion ---\n"); // Updated description
    ArrayList list = init(2);
    append(&list, 10);
    print_test_status("append(10) - length should be 1", list.length == 1);
    print_test_status("append(10) - value at index 0 should be 10", list.array[0] == 10);

    append(&list, 20);
    print_test_status("append(20) - length should be 2", list.length == 2);
    print_test_status("append(20) - value at index 1 should be 20", list.array[1] == 20);
    print_test_status("isFull() should be true", isFull(list) == true);

    append(&list, 30); // This should trigger expansion
    print_test_status("append(30) - length should be 3 (after expansion)", list.length == 3);
    print_test_status("append(30) - capacity should be 4 (after expansion)", list.capacity == 4);
    print_test_status("append(30) - value at index 2 should be 30", list.array[2] == 30);
    traverse(list);

    // Append many items to test multiple expansions
    int initial_cap = list.capacity;
    int initial_len = list.length;
    for (int i = 0; i < 100; i++)
    {
        append(&list, i);
    }
    print_test_status("append() 100 items - length should be 103", list.length == initial_len + 100);
    print_test_status("append() 100 items - capacity should be expanded", list.capacity > initial_cap);
    traverse(list);

    destroy(&list);
}

void test_insert_and_discard()
{
    printf("\n--- Test: insert() and discard() ---\n");
    ArrayList list = init(5);
    append(&list, 10);
    append(&list, 20);
    append(&list, 30); // List: [10, 20, 30]

    insert(&list, 15, 1); // List: [10, 15, 20, 30]
    print_test_status("insert(15, 1) - length should be 4", list.length == 4);
    print_test_status("insert(15, 1) - array[0] == 10", list.array[0] == 10);
    print_test_status("insert(15, 1) - array[1] == 15", list.array[1] == 15);
    print_test_status("insert(15, 1) - array[2] == 20", list.array[2] == 20);
    traverse(list);

    insert(&list, 5, 0); // List: [5, 10, 15, 20, 30]
    print_test_status("insert(5, 0) - length should be 5", list.length == 5);
    print_test_status("insert(5, 0) - array[0] == 5", list.array[0] == 5);
    traverse(list);

    insert(&list, 35, 5); // Insert at end: [5, 10, 15, 20, 30, 35] (triggers expansion)
    print_test_status("insert(35, 5) - length should be 6", list.length == 6);
    print_test_status("insert(35, 5) - array[5] == 35", list.array[5] == 35);
    print_test_status("insert(35, 5) - capacity should have expanded", list.capacity > 5);
    traverse(list);

    int discarded_val = discard(&list, 2); // Discard 15: [5, 10, 20, 30, 35]
    print_test_status("discard(2) - returned value should be 15", discarded_val == 15);
    print_test_status("discard(2) - length should be 5", list.length == 5);
    print_test_status("discard(2) - array[2] should now be 20", list.array[2] == 20);
    traverse(list);

    discarded_val = discard(&list, 0); // Discard 5: [10, 20, 30, 35]
    print_test_status("discard(0) - returned value should be 5", discarded_val == 5);
    print_test_status("discard(0) - length should be 4", list.length == 4);
    print_test_status("discard(0) - array[0] should now be 10", list.array[0] == 10);
    traverse(list);

    discarded_val = pop(&list); // Pop 35: [10, 20, 30]
    print_test_status("pop() - returned value should be 35", discarded_val == 35);
    print_test_status("pop() - length should be 3", list.length == 3);
    traverse(list);

    pop(&list); // Pop 30: [10, 20]
    pop(&list);
    pop(&list); // Pop 20, then 10, list should be empty
    print_test_status("pop() on empty list should return -1", pop(&list) == -1);
    print_test_status("discard() on empty list should return -1", discard(&list, 0) == -1);
    traverse(list);

    destroy(&list);
}

void test_get_set_replace()
{
    printf("\n--- Test: get(), set(), replace() ---\n");
    ArrayList list = init(5); // Increased capacity for more robust replace tests
    append(&list, 10);
    append(&list, 20);
    append(&list, 30);
    append(&list, 20); // Add another 20 to test starting index
    append(&list, 40); // List: [10, 20, 30, 20, 40]

    printf("Initial List for replace tests: ");
    traverse(list);

    print_test_status("get(0) should be 10", get(list, 0) == 10);
    print_test_status("get(1) should be 20", get(list, 1) == 20);
    print_test_status("get(2) should be 30", get(list, 2) == 30);
    print_test_status("get(-1) should be -1", get(list, -1) == -1);
    print_test_status("get(list.length) should be -1", get(list, list.length) == -1);

    set(&list, 25, 1); // List: [10, 25, 30, 20, 40]
    print_test_status("set(25, 1) - get(1) should be 25", get(list, 1) == 25);
    printf("List after set(25, 1): ");
    traverse(list);

    replace(&list, 50, 10, 0); // List: [50, 25, 30, 20, 40]
    print_test_status("replace(50, 10, 0) - array[0] should be 50", get(list, 0) == 50);
    printf("List after replace(50, 10, 0): ");
    traverse(list);

    // Reset the list for clearer testing of the `index` parameter.
    destroy(&list);
    list = init(5);
    append(&list, 10);
    append(&list, 20);
    append(&list, 30);
    append(&list, 20);
    append(&list, 40); // List: [10, 20, 30, 20, 40]
    printf("Reset List for specific replace tests: ");
    traverse(list);

    replace(&list, 99, 20, 0); // Replaces 20 at index 1. List: [10, 99, 30, 20, 40]
    print_test_status("replace(99, 20, 0) - array[1] should be 99", get(list, 1) == 99);
    print_test_status("replace(99, 20, 0) - array[3] should still be 20", get(list, 3) == 20); // Verify only first was replaced
    printf("List after replace(99, 20, 0): ");
    traverse(list);

    replace(&list, 88, 20, 2); // Replaces 20 at index 3 (starting search from index 2). List: [10, 99, 30, 88, 40]
    print_test_status("replace(88, 20, 2) - array[3] should be 88", get(list, 3) == 88);
    printf("List after replace(88, 20, 2): ");
    traverse(list);

    replace(&list, 77, 10, 1); // No 10 from index 1 onwards. List should be unchanged.
    print_test_status("replace(77, 10, 1) - list unchanged", get(list, 0) == 10 && get(list, 1) == 99);
    printf("List after replace(77, 10, 1) (no change): ");
    traverse(list);

    replace(&list, 66, 999, 0); // Value not present
    print_test_status("replace(66, 999, 0) - list unchanged", get(list, 0) == 10 && get(list, 1) == 99);
    printf("List after replace(66, 999, 0) (not found): ");
    traverse(list);

    replace(&list, 55, 10, -1); // Invalid index
    print_test_status("replace(55, 10, -1) - list unchanged", get(list, 0) == 10);
    printf("List after replace(55, 10, -1) (invalid index): ");
    traverse(list);

    replace(&list, 55, 10, list.length); // Invalid index
    print_test_status("replace(55, 10, list.length) - list unchanged", get(list, 0) == 10);
    printf("List after replace(55, 10, list.length) (invalid index): ");
    traverse(list);

    destroy(&list);
}

void test_copy_slice_join()
{
    printf("\n--- Test: copy(), slice(), join() ---\n");
    ArrayList original = init(5);
    append(&original, 1);
    append(&original, 2);
    append(&original, 3);
    append(&original, 4);
    append(&original, 5);
    printf("Original List: ");
    traverse(original);

    ArrayList copied = copy(original);
    print_test_status("copy() - copied length matches original", copied.length == original.length);
    print_test_status("copy() - copied capacity matches original", copied.capacity == original.capacity);
    bool copy_content_match = true;
    for (int i = 0; i < original.length; i++)
    {
        if (copied.array[i] != original.array[i])
        {
            copy_content_match = false;
            break;
        }
    }
    print_test_status("copy() - copied content matches original", copy_content_match);
    // Ensure it's a deep copy
    set(&copied, 99, 0);
    print_test_status("copy() - modifying copy does not affect original", get(original, 0) == 1 && get(copied, 0) == 99);
    destroy(&copied);

    ArrayList sliced = slice(original, 1, 4); // [2, 3, 4]
    print_test_status("slice(1, 4) - length should be 3", sliced.length == 3);
    print_test_status("slice(1, 4) - array[0] should be 2", sliced.array[0] == 2);
    print_test_status("slice(1, 4) - array[1] should be 3", sliced.array[1] == 3);
    print_test_status("slice(1, 4) - array[2] should be 4", sliced.array[2] == 4);
    printf("Sliced List (1,4): ");
    traverse(sliced);
    destroy(&sliced);

    ArrayList invalid_slice_1 = slice(original, -1, 3);
    print_test_status("slice(-1, 3) - should be empty", isEmpty(invalid_slice_1) == true);
    destroy(&invalid_slice_1);

    ArrayList invalid_slice_2 = slice(original, 2, original.length + 1);
    print_test_status("slice(2, length+1) - should be empty", isEmpty(invalid_slice_2) == true);
    destroy(&invalid_slice_2);

    ArrayList invalid_slice_3 = slice(original, 3, 1);
    print_test_status("slice(3, 1) - should be empty", isEmpty(invalid_slice_3) == true);
    destroy(&invalid_slice_3);

    ArrayList slice_single = slice(original, 2, 3); // [3]
    print_test_status("slice(2, 3) - length should be 1", slice_single.length == 1);
    print_test_status("slice(2, 3) - array[0] should be 3", slice_single.array[0] == 3);
    destroy(&slice_single);

    ArrayList slice_empty_range = slice(original, 2, 2); // []
    print_test_status("slice(2, 2) - should be empty", isEmpty(slice_empty_range) == true);
    destroy(&slice_empty_range);

    ArrayList front_list = init(2);
    append(&front_list, 100);
    append(&front_list, 200);

    ArrayList rear_list = init(3);
    append(&rear_list, 300);
    append(&rear_list, 400);
    append(&rear_list, 500);

    ArrayList joined_list = join(front_list, rear_list);
    print_test_status("join() - length should be 5", joined_list.length == 5);
    print_test_status("join() - first element 100", get(joined_list, 0) == 100);
    print_test_status("join() - last element 500", get(joined_list, 4) == 500);
    print_test_status("join() - middle element 300", get(joined_list, 2) == 300);
    printf("Joined List: ");
    traverse(joined_list);

    destroy(&front_list);
    destroy(&rear_list);
    destroy(&joined_list);
    destroy(&original);
}

void test_clear_reverse_fill_randomize_shuffle()
{
    printf("\n--- Test: clear(), reverse(), fill(), randomize(), shuffle() ---\n");
    ArrayList list = init(10);
    append(&list, 1);
    append(&list, 2);
    append(&list, 3);
    printf("Initial List: ");
    traverse(list);

    clear(&list);
    print_test_status("clear() - length should be 0", list.length == 0);
    print_test_status("clear() - isEmpty() should be true", isEmpty(list) == true);
    traverse(list);

    fill(&list, 5, 77); // List: [77, 77, 77, 77, 77]
    print_test_status("fill(5, 77) - length should be 5", list.length == 5);
    print_test_status("fill(5, 77) - all elements should be 77", get(list, 0) == 77 && get(list, 4) == 77);
    traverse(list);

    reverse(&list); // List: [77, 77, 77, 77, 77] (no change for identical elements)
    print_test_status("reverse() on all same elements - length unchanged", list.length == 5);
    traverse(list);

    append(&list, 1); // Add a different element
    append(&list, 2);
    append(&list, 3);
    printf("List before reverse: ");
    traverse(list); // [77, 77, 77, 77, 77, 1, 2, 3]
    reverse(&list); // List: [3, 2, 1, 77, 77, 77, 77, 77]
    print_test_status("reverse() on mixed elements - first element 3", get(list, 0) == 3);
    print_test_status("reverse() on mixed elements - second element 2", get(list, 1) == 2);
    print_test_status("reverse() on mixed elements - last element 77", get(list, list.length - 1) == 77);
    traverse(list);

    randomize(&list, 10, 0, 100);
    print_test_status("randomize(10, 0, 100) - length should be 10", list.length == 10);
    bool all_in_range = true;
    for (int i = 0; i < list.length; i++)
    {
        if (list.array[i] < 0 || list.array[i] >= 100)
        {
            all_in_range = false;
            break;
        }
    }
    print_test_status("randomize(10, 0, 100) - all elements in range [0, 99)", all_in_range);
    printf("Randomized List: ");
    traverse(list);

    // Shuffle test (hard to assert exact order, but can check length and contents)
    ArrayList original_for_shuffle = copy(list);
    shuffle(&list);
    print_test_status("shuffle() - length unchanged", list.length == original_for_shuffle.length);
    // Check if contents are the same, just order changed
    // This is a weak check; a stronger one would compare frequency of each number
    bool same_elements_after_shuffle = true;
    for (int i = 0; i < original_for_shuffle.length; i++)
    {
        if (!contains(list, original_for_shuffle.array[i]) ||
            count(list, original_for_shuffle.array[i]) != count(original_for_shuffle, original_for_shuffle.array[i]))
        {
            same_elements_after_shuffle = false;
            break;
        }
    }
    print_test_status("shuffle() - contains same elements (frequency check)", same_elements_after_shuffle);
    printf("Shuffled List: ");
    traverse(list);
    destroy(&original_for_shuffle);

    destroy(&list);
}

void test_search_algorithms()
{
    printf("\n--- Test: Search Algorithms ---\n");
    ArrayList sorted_list = init(10);
    for (int i = 0; i < 10; i++)
    {
        append(&sorted_list, i * 10); // 0, 10, 20, ..., 90
    }
    printf("Sorted List: ");
    traverse(sorted_list);

    // Linear Search
    print_test_status("linearSearch(30, 0) == 3", linearSearch(sorted_list, 30, 0) == 3);
    print_test_status("linearSearch(0, 0) == 0", linearSearch(sorted_list, 0, 0) == 0);
    print_test_status("linearSearch(90, 0) == 9", linearSearch(sorted_list, 90, 0) == 9);
    print_test_status("linearSearch(50, 5) == 5", linearSearch(sorted_list, 50, 5) == 5);
    print_test_status("linearSearch(50, 6) == -1", linearSearch(sorted_list, 50, 6) == -1); // Start index too high
    print_test_status("linearSearch(100, 0) == -1 (not found)", linearSearch(sorted_list, 100, 0) == -1);
    print_test_status("linearSearch(-10, 0) == -1 (not found)", linearSearch(sorted_list, -10, 0) == -1);
    print_test_status("linearSearch(30, -1) == -1 (invalid index)", linearSearch(sorted_list, 30, -1) == -1);
    print_test_status("linearSearch(30, 10) == -1 (invalid index)", linearSearch(sorted_list, 30, 10) == -1);

    // Binary Search
    print_test_status("binarySearch(30, 0) == 3", binarySearch(sorted_list, 30, 0) == 3);
    print_test_status("binarySearch(0, 0) == 0", binarySearch(sorted_list, 0, 0) == 0);
    print_test_status("binarySearch(90, 0) == 9", binarySearch(sorted_list, 90, 0) == 9);
    print_test_status("binarySearch(50, 0) == 5", binarySearch(sorted_list, 50, 0) == 5); // Index 5, not 0
    print_test_status("binarySearch(55, 0) == -1 (not found)", binarySearch(sorted_list, 55, 0) == -1);
    print_test_status("binarySearch(-10, 0) == -1 (not found)", binarySearch(sorted_list, -10, 0) == -1);
    print_test_status("binarySearch(30, -1) == -1 (invalid index)", binarySearch(sorted_list, 30, -1) == -1);

    // Ternary Search
    print_test_status("ternarySearch(30, 0) == 3", ternarySearch(sorted_list, 30, 0) == 3);
    print_test_status("ternarySearch(0, 0) == 0", ternarySearch(sorted_list, 0, 0) == 0);
    print_test_status("ternarySearch(90, 0) == 9", ternarySearch(sorted_list, 90, 0) == 9);
    print_test_status("ternarySearch(50, 0) == 5", ternarySearch(sorted_list, 50, 0) == 5);
    print_test_status("ternarySearch(55, 0) == -1 (not found)", ternarySearch(sorted_list, 55, 0) == -1);
    print_test_status("ternarySearch(-10, 0) == -1 (not found)", ternarySearch(sorted_list, -10, 0) == -1);
    print_test_status("ternarySearch(30, -1) == -1 (invalid index)", ternarySearch(sorted_list, 30, -1) == -1);

    // Interpolation Search
    print_test_status("interpolationSearch(30, 0) == 3", interpolationSearch(sorted_list, 30, 0) == 3);
    print_test_status("interpolationSearch(0, 0) == 0", interpolationSearch(sorted_list, 0, 0) == 0);
    print_test_status("interpolationSearch(90, 0) == 9", interpolationSearch(sorted_list, 90, 0) == 9);
    print_test_status("interpolationSearch(50, 0) == 5", interpolationSearch(sorted_list, 50, 0) == 5);
    print_test_status("interpolationSearch(55, 0) == -1 (not found)", interpolationSearch(sorted_list, 55, 0) == -1);
    print_test_status("interpolationSearch(-10, 0) == -1 (not found)", interpolationSearch(sorted_list, -10, 0) == -1);
    print_test_status("interpolationSearch(30, -1) == -1 (invalid index)", interpolationSearch(sorted_list, 30, -1) == -1);
    // Test with all same elements for interpolation search
    ArrayList same_elements_list = init(5);
    fill(&same_elements_list, 5, 42);
    print_test_status("interpolationSearch(42) on [42,42,42,42,42] == 0 (first index)", interpolationSearch(same_elements_list, 42, 0) == 0);
    print_test_status("interpolationSearch(10) on [42,42,42,42,42] == -1 (not found)", interpolationSearch(same_elements_list, 10, 0) == -1);
    destroy(&same_elements_list);

    // Jump Search
    print_test_status("jumpSearch(30, 0) == 3", jumpSearch(sorted_list, 30, 0) == 3);
    print_test_status("jumpSearch(0, 0) == 0", jumpSearch(sorted_list, 0, 0) == 0);
    print_test_status("jumpSearch(90, 0) == 9", jumpSearch(sorted_list, 90, 0) == 9);
    print_test_status("jumpSearch(50, 0) == 5", jumpSearch(sorted_list, 50, 0) == 5);
    print_test_status("jumpSearch(55, 0) == -1 (not found)", jumpSearch(sorted_list, 55, 0) == -1);
    print_test_status("jumpSearch(-10, 0) == -1 (not found)", jumpSearch(sorted_list, -10, 0) == -1);
    print_test_status("jumpSearch(30, -1) == -1 (invalid index)", jumpSearch(sorted_list, 30, -1) == -1);

    destroy(&sorted_list);

    // Test search on empty list
    ArrayList empty_search_list = init(0);
    print_test_status("binarySearch(10, 0) on empty list == -1", binarySearch(empty_search_list, 10, 0) == -1);
    print_test_status("ternarySearch(10, 0) on empty list == -1", ternarySearch(empty_search_list, 10, 0) == -1);
    print_test_status("interpolationSearch(10, 0) on empty list == -1", interpolationSearch(empty_search_list, 10, 0) == -1);
    print_test_status("jumpSearch(10, 0) on empty list == -1", jumpSearch(empty_search_list, 10, 0) == -1);
    print_test_status("linearSearch(10, 0) on empty list == -1", linearSearch(empty_search_list, 10, 0) == -1);
    destroy(&empty_search_list);
}

void test_sorting_algorithms()
{
    printf("\n--- Test: Sorting Algorithms ---\n");

    const int TEST_SIZE_SMALL = 10;
    const int TEST_SIZE_MEDIUM = 1000;
    const int TEST_SIZE_LARGE = 10000; // Warning: Bubble/Selection can be slow here

    clock_t start_time, end_time;
    double cpu_time_used;

    // --- Test Bubble Sort ---
    ArrayList bubble_list = init(TEST_SIZE_SMALL);
    randomize(&bubble_list, TEST_SIZE_SMALL, 0, 100);
    printf("Bubble Sort - Original Small: ");
    traverse(bubble_list);
    bubbleSort(&bubble_list);
    printf("Bubble Sort - Sorted Small:   ");
    traverse(bubble_list);
    print_test_status("bubbleSort (small) - is sorted", is_sorted(bubble_list));
    destroy(&bubble_list);

    bubble_list = init(TEST_SIZE_MEDIUM);
    randomize(&bubble_list, TEST_SIZE_MEDIUM, 0, 1000);
    start_time = clock();
    bubbleSort(&bubble_list);
    end_time = clock();
    cpu_time_used = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    print_test_status("bubbleSort (medium) - is sorted", is_sorted(bubble_list));
    printf("  -> Bubble Sort (medium) took %f seconds\n", cpu_time_used);
    destroy(&bubble_list);

    // --- Test Selection Sort ---
    ArrayList selection_list = init(TEST_SIZE_SMALL);
    randomize(&selection_list, TEST_SIZE_SMALL, 0, 100);
    printf("Selection Sort - Original Small: ");
    traverse(selection_list);
    selectionSort(&selection_list);
    printf("Selection Sort - Sorted Small:   ");
    traverse(selection_list);
    print_test_status("selectionSort (small) - is sorted", is_sorted(selection_list));
    destroy(&selection_list);

    selection_list = init(TEST_SIZE_MEDIUM);
    randomize(&selection_list, TEST_SIZE_MEDIUM, 0, 1000);
    start_time = clock();
    selectionSort(&selection_list);
    end_time = clock();
    cpu_time_used = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    print_test_status("selectionSort (medium) - is sorted", is_sorted(selection_list));
    printf("  -> Selection Sort (medium) took %f seconds\n", cpu_time_used);
    destroy(&selection_list);

    // --- Test Insertion Sort ---
    ArrayList insertion_list = init(TEST_SIZE_SMALL);
    randomize(&insertion_list, TEST_SIZE_SMALL, 0, 100);
    printf("Insertion Sort - Original Small: ");
    traverse(insertion_list);
    insertionSort(&insertion_list);
    printf("Insertion Sort - Sorted Small:   ");
    traverse(insertion_list);
    print_test_status("insertionSort (small) - is sorted", is_sorted(insertion_list));
    destroy(&insertion_list);

    insertion_list = init(TEST_SIZE_MEDIUM);
    randomize(&insertion_list, TEST_SIZE_MEDIUM, 0, 1000);
    start_time = clock();
    insertionSort(&insertion_list);
    end_time = clock();
    cpu_time_used = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    print_test_status("insertionSort (medium) - is sorted", is_sorted(insertion_list));
    printf("  -> Insertion Sort (medium) took %f seconds\n", cpu_time_used);
    destroy(&insertion_list);

    // --- Test Counting Sort ---
    ArrayList counting_list = init(TEST_SIZE_SMALL);
    randomize(&counting_list, TEST_SIZE_SMALL, 0, 100); // Small range needed for efficiency
    printf("Counting Sort - Original Small: ");
    traverse(counting_list);
    countSort(&counting_list);
    printf("Counting Sort - Sorted Small:   ");
    traverse(counting_list);
    print_test_status("countingSort (small) - is sorted", is_sorted(counting_list));
    destroy(&counting_list);

    counting_list = init(TEST_SIZE_LARGE);                 // Counting sort excels with fixed range
    randomize(&counting_list, TEST_SIZE_LARGE, -500, 500); // Test negative numbers
    start_time = clock();
    countSort(&counting_list);
    end_time = clock();
    cpu_time_used = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    print_test_status("countingSort (large) - is sorted", is_sorted(counting_list));
    printf("  -> Counting Sort (large) took %f seconds\n", cpu_time_used);
    destroy(&counting_list);

    // --- Test Radix Sort ---
    ArrayList radix_list = init(TEST_SIZE_SMALL);
    randomize(&radix_list, TEST_SIZE_SMALL, 0, 1000); // Radix sort works well with larger ranges of non-negative ints
    printf("Radix Sort - Original Small: ");
    traverse(radix_list);
    radixSort(&radix_list);
    printf("Radix Sort - Sorted Small:   ");
    traverse(radix_list);
    print_test_status("radixSort (small) - is sorted", is_sorted(radix_list));
    destroy(&radix_list);

    radix_list = init(TEST_SIZE_LARGE);
    randomize(&radix_list, TEST_SIZE_LARGE, 0, 100000); // Test large range with non-negative numbers
    start_time = clock();
    radixSort(&radix_list);
    end_time = clock();
    cpu_time_used = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    print_test_status("radixSort (large) - is sorted", is_sorted(radix_list));
    printf("  -> Radix Sort (large) took %f seconds\n", cpu_time_used);
    destroy(&radix_list);

    // --- Test Merge Sort ---
    ArrayList merge_list = init(TEST_SIZE_SMALL);
    randomize(&merge_list, TEST_SIZE_SMALL, 0, 100);
    printf("Merge Sort - Original Small: ");
    traverse(merge_list);
    mergeSort(&merge_list);
    printf("Merge Sort - Sorted Small:   ");
    traverse(merge_list);
    print_test_status("mergeSort (small) - is sorted", is_sorted(merge_list));
    destroy(&merge_list);

    ArrayList merge_list_large = init(TEST_SIZE_LARGE);
    randomize(&merge_list_large, TEST_SIZE_LARGE, INT_MIN / 2, INT_MAX / 2); // Test large range
    start_time = clock();
    mergeSort(&merge_list_large);
    end_time = clock();
    cpu_time_used = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    print_test_status("mergeSort (large) - is sorted", is_sorted(merge_list_large));
    printf("  -> Merge Sort (large) took %f seconds\n", cpu_time_used);
    destroy(&merge_list_large);

    // --- Test Quick Sort ---
    ArrayList quick_list = init(TEST_SIZE_SMALL);
    randomize(&quick_list, TEST_SIZE_SMALL, 0, 100);
    printf("Quick Sort - Original Small: ");
    traverse(quick_list);
    quickSort(&quick_list);
    printf("Quick Sort - Sorted Small:   ");
    traverse(quick_list);
    print_test_status("quickSort (small) - is sorted", is_sorted(quick_list));
    destroy(&quick_list);

    ArrayList quick_list_large = init(TEST_SIZE_LARGE);
    randomize(&quick_list_large, TEST_SIZE_LARGE, INT_MIN / 2, INT_MAX / 2); // Test large range
    start_time = clock();
    quickSort(&quick_list_large);
    end_time = clock();
    cpu_time_used = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    print_test_status("quickSort (large) - is sorted", is_sorted(quick_list_large));
    printf("  -> Quick Sort (large) took %f seconds\n", cpu_time_used);
    destroy(&quick_list_large);

    // --- Test Heap Sort ---
    ArrayList heap_list = init(TEST_SIZE_SMALL);
    randomize(&heap_list, TEST_SIZE_SMALL, 0, 100);
    printf("Heap Sort - Original Small: ");
    traverse(heap_list);
    heapSort(&heap_list);
    printf("Heap Sort - Sorted Small:   ");
    traverse(heap_list);
    print_test_status("heapSort (small) - is sorted", is_sorted(heap_list));
    destroy(&heap_list);

    ArrayList heap_list_large = init(TEST_SIZE_LARGE);
    randomize(&heap_list_large, TEST_SIZE_LARGE, INT_MIN / 2, INT_MAX / 2); // Test large range
    start_time = clock();
    heapSort(&heap_list_large);
    end_time = clock();
    cpu_time_used = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    print_test_status("heapSort (large) - is sorted", is_sorted(heap_list_large));
    printf("  -> Heap Sort (large) took %f seconds\n", cpu_time_used);
    destroy(&heap_list_large);
}

void test_functional_helpers()
{
    printf("\n--- Test: map(), filter(), any(), all(), max(), min(), sum(), prod(), count(), contains() ---\n");

    ArrayList list = init(5);
    append(&list, 1);
    append(&list, 2);
    append(&list, 3);
    append(&list, 4);
    append(&list, 5);
    printf("Initial List: ");
    traverse(list);

    // map
    map(&list, double_value); // Doubles each element
    print_test_status("map(double_value) - first element is 2", get(list, 0) == 2);
    print_test_status("map(double_value) - last element is 10", get(list, 4) == 10);
    printf("After map(double_value): ");
    traverse(list); // [2, 4, 6, 8, 10]

    // filter
    ArrayList filter_test_list = init(10);
    append(&filter_test_list, -2);
    append(&filter_test_list, -1);
    append(&filter_test_list, 0);
    append(&filter_test_list, 1);
    append(&filter_test_list, 2);
    append(&filter_test_list, 3);
    append(&filter_test_list, 4);
    append(&filter_test_list, 5);
    append(&filter_test_list, 6);
    append(&filter_test_list, 7);
    printf("Filter Test List: ");
    traverse(filter_test_list);

    filter(&filter_test_list, is_even);
    print_test_status("filter(is_even) - length should be 5", filter_test_list.length == 5);
    print_test_status("filter(is_even) - first element is -2", get(filter_test_list, 0) == -2);
    print_test_status("filter(is_even) - last element is 6", get(filter_test_list, 4) == 6);
    printf("After filter(is_even): ");
    traverse(filter_test_list); // [-2, 0, 2, 4, 6]

    filter(&filter_test_list, is_positive);
    print_test_status("filter(is_positive) - length should be 3", filter_test_list.length == 3);
    print_test_status("filter(is_positive) - first element is 2", get(filter_test_list, 0) == 2);
    print_test_status("filter(is_positive) - last element is 6", get(filter_test_list, 2) == 6);
    printf("After filter(is_positive): ");
    traverse(filter_test_list); // [2, 4, 6]
    destroy(&filter_test_list);

    // any, all
    ArrayList bool_list1 = init(3);
    append(&bool_list1, 0);
    append(&bool_list1, 0);
    append(&bool_list1, 0);
    print_test_status("any() on [0,0,0] is false", any(bool_list1) == false);
    print_test_status("all() on [0,0,0] is false", all(bool_list1) == false);
    destroy(&bool_list1);

    ArrayList bool_list2 = init(3);
    append(&bool_list2, 1);
    append(&bool_list2, 0);
    append(&bool_list2, 1);
    print_test_status("any() on [1,0,1] is true", any(bool_list2) == true);
    print_test_status("all() on [1,0,1] is false", all(bool_list2) == false);
    destroy(&bool_list2);

    ArrayList bool_list3 = init(3);
    append(&bool_list3, 1);
    append(&bool_list3, 2);
    append(&bool_list3, 3);
    print_test_status("any() on [1,2,3] is true", any(bool_list3) == true);
    print_test_status("all() on [1,2,3] is true", all(bool_list3) == true);
    destroy(&bool_list3);

    ArrayList empty_bool_list = init(0);
    print_test_status("any() on empty list is false", any(empty_bool_list) == false);
    print_test_status("all() on empty list is true (vacuously true)", all(empty_bool_list) == true);
    destroy(&empty_bool_list);

    // max, min, sum, prod
    ArrayList stats_list = init(5);
    append(&stats_list, 5);
    append(&stats_list, 10);
    append(&stats_list, 2);
    append(&stats_list, 8);
    append(&stats_list, 15); // [5, 10, 2, 8, 15]
    printf("Stats List: ");
    traverse(stats_list);

    print_test_status("max() should be 15", max(stats_list) == 15);
    print_test_status("min() should be 2", min(stats_list) == 2);
    print_test_status("sum() should be 40", sum(stats_list) == (5 + 10 + 2 + 8 + 15));
    print_test_status("prod() should be 12000", prod(stats_list) == (5 * 10 * 2 * 8 * 15));
    destroy(&stats_list);

    ArrayList empty_stats_list = init(0);
    print_test_status("max() on empty list should be -1", max(empty_stats_list) == -1);
    print_test_status("min() on empty list should be -1", min(empty_stats_list) == -1);
    print_test_status("sum() on empty list should be 0", sum(empty_stats_list) == 0);
    print_test_status("prod() on empty list should be 1", prod(empty_stats_list) == 1);
    destroy(&empty_stats_list);

    // count, contains
    ArrayList freq_list = init(8);
    append(&freq_list, 1);
    append(&freq_list, 2);
    append(&freq_list, 1);
    append(&freq_list, 3);
    append(&freq_list, 1);
    append(&freq_list, 2);
    append(&freq_list, 4);
    append(&freq_list, 5); // [1, 2, 1, 3, 1, 2, 4, 5]
    printf("Frequency List: ");
    traverse(freq_list);

    print_test_status("count(1) should be 3", count(freq_list, 1) == 3);
    print_test_status("count(2) should be 2", count(freq_list, 2) == 2);
    print_test_status("count(5) should be 1", count(freq_list, 5) == 1);
    print_test_status("count(99) should be 0", count(freq_list, 99) == 0);

    print_test_status("contains(3) should be true", contains(freq_list, 3) == true);
    print_test_status("contains(99) should be false", contains(freq_list, 99) == false);
    print_test_status("contains(1) should be true", contains(freq_list, 1) == true);
    destroy(&freq_list);

    destroy(&list);
}

// Main Test Runner
int main()
{
    printf("--- Running ArrayList Tests ---\n");

    test_init_and_destroy();
    test_append_and_expand();
    test_insert_and_discard();
    test_get_set_replace();
    test_copy_slice_join();
    test_clear_reverse_fill_randomize_shuffle();
    test_search_algorithms();
    test_sorting_algorithms();
    test_functional_helpers();

    printf("\n--- All ArrayList Tests Completed ---\n");

    return 0;
}