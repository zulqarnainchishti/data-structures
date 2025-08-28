#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "adt_Vector.h"

#define GREEN "\x1b[32m"
#define RED "\x1b[31m"
#define YELLOW "\x1b[33m"
#define BLUE "\x1b[34m"
#define RESET "\x1b[0m"

// Function prototypes for test groups
void runAllTests();
void testCoreManagement();
void testElementManipulation();
void testDataTransformationAndUtilities();
void testSearchAlgorithms();
void testSortingAlgorithms();
void testFunctionalProgrammingInspired();

// Helper functions for testing
void printVectorForTest(const Vector *v)
{
    if (v == NULL)
    {
        printf("[ ] : 0/0\n");
        return;
    }
    printf("[ ");
    for (int i = 0; i < v->length; i++)
    {
        printf("%d ", v->array[i]);
    }
    printf("] : %d/%d\n", v->length, v->capacity);
}

bool areVectorsEqual(const Vector *v1, const Vector *v2)
{
    if (v1 == NULL || v2 == NULL)
    {
        return v1 == v2;
    }
    if (v1->length != v2->length)
    {
        return false;
    }
    for (int i = 0; i < v1->length; i++)
    {
        if (v1->array[i] != v2->array[i])
        {
            return false;
        }
    }
    return true;
}

// Helper functions for functional tests
int square(int x)
{
    return x * x;
}

bool isEven(int x)
{
    return x % 2 == 0;
}

// Main function to execute tests
int main()
{
    runAllTests();
    return 0;
}

void runAllTests()
{
    printf(BLUE "========================================\n" RESET);
    printf(BLUE "       Running Vector ADT Test Suite\n" RESET);
    printf(BLUE "========================================\n\n" RESET);

    testCoreManagement();
    testElementManipulation();
    testDataTransformationAndUtilities();
    testSearchAlgorithms();
    testSortingAlgorithms();
    testFunctionalProgrammingInspired();

    printf(BLUE "========================================\n" RESET);
    printf(BLUE "        All Tests Finished\n" RESET);
    printf(BLUE "========================================\n" RESET);
}

// ========================================
// Test Group: Core Management
// ========================================
void testCoreManagement()
{
    printf(YELLOW "--- Core Management Functions ---\n" RESET);

    // Test `init()` and `isEmpty()`
    Vector v1 = init(5);
    printf("Test: init() and isEmpty()\n");
    printf("  Expected: length=0, capacity=5, isEmpty()=true\n");
    printf("  Actual:   length=%d, capacity=%d, isEmpty()=%s\n", v1.length, v1.capacity, isEmpty(&v1) ? "true" : "false");
    if (v1.capacity == 5 && v1.length == 0 && isEmpty(&v1))
    {
        printf("  Result: " GREEN "Passed\n" RESET);
    }
    else
    {
        printf("  Result: " RED "Failed\n" RESET);
    }
    printf("----------------------------------------\n");

    // Test `copy()`
    append(&v1, 10);
    append(&v1, 20);
    Vector v_copy = copy(&v1);
    printf("Test: copy()\n");
    printf("  Original: ");
    printVectorForTest(&v1);
    printf("  Copied:   ");
    printVectorForTest(&v_copy);
    if (areVectorsEqual(&v1, &v_copy) && v1.array != v_copy.array)
    {
        printf("  Result: " GREEN "Passed\n" RESET);
    }
    else
    {
        printf("  Result: " RED "Failed\n" RESET);
    }
    destroy(&v_copy);
    printf("----------------------------------------\n");

    // Test `slice()`
    Vector v_slice = slice(&v1, 0, 1);
    int expected_slice_data[] = {10};
    Vector expected_slice = populate(expected_slice_data, 1);
    printf("Test: slice()\n");
    printf("  Expected slice: ");
    printVectorForTest(&expected_slice);
    printf("  Actual slice:   ");
    printVectorForTest(&v_slice);
    if (areVectorsEqual(&v_slice, &expected_slice))
    {
        printf("  Result: " GREEN "Passed\n" RESET);
    }
    else
    {
        printf("  Result: " RED "Failed\n" RESET);
    }
    destroy(&v_slice);
    destroy(&expected_slice);
    printf("----------------------------------------\n");

    // Test `join()`
    Vector v_front = init(2);
    append(&v_front, 1);
    append(&v_front, 2);
    Vector v_rear = init(2);
    append(&v_rear, 3);
    append(&v_rear, 4);
    Vector v_joined = join(&v_front, &v_rear);
    int expected_join_data[] = {1, 2, 3, 4};
    Vector expected_join = populate(expected_join_data, 4);
    printf("Test: join()\n");
    printf("  Expected joined vector: ");
    printVectorForTest(&expected_join);
    printf("  Actual joined vector:   ");
    printVectorForTest(&v_joined);
    if (areVectorsEqual(&v_joined, &expected_join))
    {
        printf("  Result: " GREEN "Passed\n" RESET);
    }
    else
    {
        printf("  Result: " RED "Failed\n" RESET);
    }
    destroy(&v_front);
    destroy(&v_rear);
    destroy(&v_joined);
    destroy(&expected_join);
    printf("----------------------------------------\n");

    // Test `clear()` and `destroy()`
    Vector v_to_clear = init(3);
    append(&v_to_clear, 1);
    clear(&v_to_clear);
    printf("Test: clear()\n");
    printf("  Expected: length=0\n");
    printf("  Actual:   length=%d\n", v_to_clear.length);
    if (v_to_clear.length == 0)
    {
        printf("  Result: " GREEN "Passed\n" RESET);
    }
    else
    {
        printf("  Result: " RED "Failed\n" RESET);
    }
    destroy(&v_to_clear);
    printf("----------------------------------------\n");
    printf("Test: destroy()\n");
    printf("  Expected: array=NULL, length=0, capacity=0\n");
    printf("  Actual:   array=%p, length=%d, capacity=%d\n", (void *)v_to_clear.array, v_to_clear.length, v_to_clear.capacity);
    if (v_to_clear.array == NULL && v_to_clear.length == 0 && v_to_clear.capacity == 0)
    {
        printf("  Result: " GREEN "Passed\n" RESET);
    }
    else
    {
        printf("  Result: " RED "Failed\n" RESET);
    }
    printf("----------------------------------------\n");

    destroy(&v1);
}

// ========================================
// Test Group: Element Manipulation
// ========================================
void testElementManipulation()
{
    printf(YELLOW "--- Element Manipulation Functions ---\n" RESET);

    int data[] = {10, 20, 30, 40, 50};
    Vector v = populate(data, 5);

    // Test `get()`
    printf("Test: get()\n");
    printf("  Expected: value at index 2 is 30\n");
    int actual_get = get(&v, 2);
    printf("  Actual:   value at index 2 is %d\n", actual_get);
    if (actual_get == 30)
    {
        printf("  Result: " GREEN "Passed\n" RESET);
    }
    else
    {
        printf("  Result: " RED "Failed\n" RESET);
    }
    printf("----------------------------------------\n");

    // Test `set()`
    printf("Test: set()\n");
    set(&v, 99, 2);
    printf("  Expected: [ 10 20 99 40 50 ]\n");
    printf("  Actual:   ");
    printVectorForTest(&v);
    if (get(&v, 2) == 99)
    {
        printf("  Result: " GREEN "Passed\n" RESET);
    }
    else
    {
        printf("  Result: " RED "Failed\n" RESET);
    }
    printf("----------------------------------------\n");

    // Test `replace()`
    printf("Test: replace()\n");
    Vector v_replace = populate((int[]){10, 20, 30, 20, 50}, 5);
    replace(&v_replace, 25, 20, 0); // Replace first 20 with 25
    int expected_replace_data[] = {10, 25, 30, 20, 50};
    Vector expected_replace = populate(expected_replace_data, 5);
    printf("  Expected: ");
    printVectorForTest(&expected_replace);
    printf("  Actual:   ");
    printVectorForTest(&v_replace);
    if (areVectorsEqual(&v_replace, &expected_replace))
    {
        printf("  Result: " GREEN "Passed\n" RESET);
    }
    else
    {
        printf("  Result: " RED "Failed\n" RESET);
    }
    destroy(&v_replace);
    destroy(&expected_replace);
    printf("----------------------------------------\n");

    // Test `insert()`
    printf("Test: insert()\n");
    insert(&v, 100, 3);
    printf("  Expected: [ 10 20 99 100 40 50 ] : 6/5 (expanded capacity)\n");
    printf("  Actual:   ");
    printVectorForTest(&v);
    if (v.array[3] == 100 && v.length == 6)
    {
        printf("  Result: " GREEN "Passed\n" RESET);
    }
    else
    {
        printf("  Result: " RED "Failed\n" RESET);
    }
    printf("----------------------------------------\n");

    // Test `pop()`
    printf("Test: pop()\n");
    pop(&v, 99); // Removes the value 99
    printf("  Expected: [ 10 20 100 40 50 ]\n");
    printf("  Actual:   ");
    printVectorForTest(&v);
    if (!contains(&v, 99) && v.length == 5)
    {
        printf("  Result: " GREEN "Passed\n" RESET);
    }
    else
    {
        printf("  Result: " RED "Failed\n" RESET);
    }
    printf("----------------------------------------\n");

    // Test `discard()`
    printf("Test: discard()\n");
    int discarded_val = discard(&v, 0);
    printf("  Expected: discarded value 10, new vector starting with 20\n");
    printf("  Actual:   discarded value %d, new vector starting with %d\n", discarded_val, v.array[0]);
    if (discarded_val == 10 && v.array[0] == 20 && v.length == 4)
    {
        printf("  Result: " GREEN "Passed\n" RESET);
    }
    else
    {
        printf("  Result: " RED "Failed\n" RESET);
    }
    printf("----------------------------------------\n");

    destroy(&v);
}

// ========================================
// Test Group: Data Transformation & Utilities
// ========================================
void testDataTransformationAndUtilities()
{
    printf(YELLOW "--- Data Transformation & Utilities ---\n" RESET);

    // Test `populate()`
    int data[] = {1, 2, 3, 4, 5};
    Vector v = populate(data, 5);
    printf("Test: populate()\n");
    printf("  Expected populated vector: [ 1 2 3 4 5 ] : 5/5\n");
    printf("  Actual populated vector:   ");
    printVectorForTest(&v);
    if (v.length == 5 && v.array[0] == 1 && v.array[4] == 5)
    {
        printf("  Result: " GREEN "Passed\n" RESET);
    }
    else
    {
        printf("  Result: " RED "Failed\n" RESET);
    }
    printf("----------------------------------------\n");

    // Test `fill()`
    fill(&v, 3, 7);
    printf("Test: fill()\n");
    printf("  Expected filled vector: [ 7 7 7 ] : 3/5\n");
    printf("  Actual filled vector:   ");
    printVectorForTest(&v);
    if (v.length == 3 && v.array[0] == 7 && v.array[2] == 7)
    {
        printf("  Result: " GREEN "Passed\n" RESET);
    }
    else
    {
        printf("  Result: " RED "Failed\n" RESET);
    }
    printf("----------------------------------------\n");

    // Test `reverse()`
    clear(&v);
    append(&v, 1);
    append(&v, 2);
    append(&v, 3);
    reverse(&v);
    printf("Test: reverse()\n");
    printf("  Expected reversed vector: [ 3 2 1 ] : 3/%d\n", v.capacity);
    printf("  Actual reversed vector:   ");
    printVectorForTest(&v);
    if (v.array[0] == 3 && v.array[1] == 2 && v.array[2] == 1)
    {
        printf("  Result: " GREEN "Passed\n" RESET);
    }
    else
    {
        printf("  Result: " RED "Failed\n" RESET);
    }
    printf("----------------------------------------\n");

    // Test `random()`
    Vector v_rand = init(10);
    random(&v_rand, 5, 0, 100);
    printf("Test: random()\n");
    printf("  Expected: A vector of length 5 with values between 0 and 100\n");
    printf("  Actual:   ");
    printVectorForTest(&v_rand);
    if (v_rand.length == 5 && v_rand.array[0] >= 0 && v_rand.array[0] <= 100)
    {
        printf("  Result: " GREEN "Passed\n" RESET);
    }
    else
    {
        printf("  Result: " RED "Failed\n" RESET);
    }
    destroy(&v_rand);
    printf("----------------------------------------\n");

    // Test `shuffle()`
    Vector v_shuffle = populate((int[]){1, 2, 3, 4, 5}, 5);
    shuffle(&v_shuffle);
    printf("Test: shuffle()\n");
    printf("  Expected: shuffled vector with same elements as [ 1 2 3 4 5 ]\n");
    printf("  Actual:   ");
    printVectorForTest(&v_shuffle);
    bool all_present = contains(&v_shuffle, 1) && contains(&v_shuffle, 2) && contains(&v_shuffle, 3) && contains(&v_shuffle, 4) && contains(&v_shuffle, 5);
    if (v_shuffle.length == 5 && all_present)
    {
        printf("  Result: " GREEN "Passed\n" RESET);
    }
    else
    {
        printf("  Result: " RED "Failed\n" RESET);
    }
    destroy(&v_shuffle);
    printf("----------------------------------------\n");

    destroy(&v);
}

// ========================================
// Test Group: Search Algorithms
// ========================================
void testSearchAlgorithms()
{
    printf(YELLOW "--- Search Algorithms ---\n" RESET);

    int sorted_data[] = {5, 10, 15, 20, 25, 30};
    Vector sorted_v = populate(sorted_data, 6);
    int unsorted_data[] = {10, 4, 30, 5, 20, 15};
    Vector unsorted_v = populate(unsorted_data, 6);

    // Test `linearSearch()` on an unsorted vector
    printf("Test: linearSearch()\n");
    printf("  Searching for 20 in unsorted vector. Expected index: 4\n");
    int actual_linear_search = linearSearch(&unsorted_v, 20, 0);
    printf("  Actual index: %d\n", actual_linear_search);
    if (actual_linear_search == 4)
    {
        printf("  Result: " GREEN "Passed\n" RESET);
    }
    else
    {
        printf("  Result: " RED "Failed\n" RESET);
    }
    printf("----------------------------------------\n");

    // Test `binarySearch()` on a sorted vector
    printf("Test: binarySearch()\n");
    printf("  Searching for 25 in sorted vector. Expected index: 4\n");
    int actual_binary_search = binarySearch(&sorted_v, 25, 0);
    printf("  Actual index: %d\n", actual_binary_search);
    if (actual_binary_search == 4)
    {
        printf("  Result: " GREEN "Passed\n" RESET);
    }
    else
    {
        printf("  Result: " RED "Failed\n" RESET);
    }
    printf("----------------------------------------\n");

    // Test `ternarySearch()` on a sorted vector
    printf("Test: ternarySearch()\n");
    printf("  Searching for 15 in sorted vector. Expected index: 2\n");
    int actual_ternary_search = ternarySearch(&sorted_v, 15, 0);
    printf("  Actual index: %d\n", actual_ternary_search);
    if (actual_ternary_search == 2)
    {
        printf("  Result: " GREEN "Passed\n" RESET);
    }
    else
    {
        printf("  Result: " RED "Failed\n" RESET);
    }
    printf("----------------------------------------\n");

    // Test `interpolationSearch()` on a sorted vector
    printf("Test: interpolationSearch()\n");
    printf("  Searching for 15 in sorted vector. Expected index: 2\n");
    int actual_interpolation_search = interpolationSearch(&sorted_v, 15, 0);
    printf("  Actual index: %d\n", actual_interpolation_search);
    if (actual_interpolation_search == 2)
    {
        printf("  Result: " GREEN "Passed\n" RESET);
    }
    else
    {
        printf("  Result: " RED "Failed\n" RESET);
    }
    printf("----------------------------------------\n");

    // Test `jumpSearch()` on a sorted vector
    printf("Test: jumpSearch()\n");
    printf("  Searching for 30 in sorted vector. Expected index: 5\n");
    int actual_jump_search = jumpSearch(&sorted_v, 30, 0);
    printf("  Actual index: %d\n", actual_jump_search);
    if (actual_jump_search == 5)
    {
        printf("  Result: " GREEN "Passed\n" RESET);
    }
    else
    {
        printf("  Result: " RED "Failed\n" RESET);
    }
    printf("----------------------------------------\n");

    destroy(&sorted_v);
    destroy(&unsorted_v);
}

// ========================================
// Test Group: Sorting Algorithms
// ========================================
void testSortingAlgorithms()
{
    printf(YELLOW "--- Sorting Algorithms ---\n" RESET);
    int unsorted_data[] = {40, 10, 30, 50, 20};
    int expected_sorted_data[] = {10, 20, 30, 40, 50};
    Vector expected_v = populate(expected_sorted_data, 5);

    // Test `bubbleSort()`
    Vector v_bubble = populate(unsorted_data, 5);
    bubbleSort(&v_bubble);
    printf("Test: bubbleSort()\n");
    printf("  Expected: ");
    printVectorForTest(&expected_v);
    printf("  Actual:   ");
    printVectorForTest(&v_bubble);
    if (areVectorsEqual(&v_bubble, &expected_v))
    {
        printf("  Result: " GREEN "Passed\n" RESET);
    }
    else
    {
        printf("  Result: " RED "Failed\n" RESET);
    }
    destroy(&v_bubble);
    printf("----------------------------------------\n");

    // Test `selectionSort()`
    Vector v_selection = populate(unsorted_data, 5);
    selectionSort(&v_selection);
    printf("Test: selectionSort()\n");
    printf("  Expected: ");
    printVectorForTest(&expected_v);
    printf("  Actual:   ");
    printVectorForTest(&v_selection);
    if (areVectorsEqual(&v_selection, &expected_v))
    {
        printf("  Result: " GREEN "Passed\n" RESET);
    }
    else
    {
        printf("  Result: " RED "Failed\n" RESET);
    }
    destroy(&v_selection);
    printf("----------------------------------------\n");

    // Test `insertionSort()`
    Vector v_insertion = populate(unsorted_data, 5);
    insertionSort(&v_insertion);
    printf("Test: insertionSort()\n");
    printf("  Expected: ");
    printVectorForTest(&expected_v);
    printf("  Actual:   ");
    printVectorForTest(&v_insertion);
    if (areVectorsEqual(&v_insertion, &expected_v))
    {
        printf("  Result: " GREEN "Passed\n" RESET);
    }
    else
    {
        printf("  Result: " RED "Failed\n" RESET);
    }
    destroy(&v_insertion);
    printf("----------------------------------------\n");

    // Test `countSort()`
    Vector v_count = populate(unsorted_data, 5);
    countSort(&v_count);
    printf("Test: countSort()\n");
    printf("  Expected: ");
    printVectorForTest(&expected_v);
    printf("  Actual:   ");
    printVectorForTest(&v_count);
    if (areVectorsEqual(&v_count, &expected_v))
    {
        printf("  Result: " GREEN "Passed\n" RESET);
    }
    else
    {
        printf("  Result: " RED "Failed\n" RESET);
    }
    destroy(&v_count);
    printf("----------------------------------------\n");

    // Test `radixSort()`
    Vector v_radix = populate(unsorted_data, 5);
    radixSort(&v_radix);
    printf("Test: radixSort()\n");
    printf("  Expected: ");
    printVectorForTest(&expected_v);
    printf("  Actual:   ");
    printVectorForTest(&v_radix);
    if (areVectorsEqual(&v_radix, &expected_v))
    {
        printf("  Result: " GREEN "Passed\n" RESET);
    }
    else
    {
        printf("  Result: " RED "Failed\n" RESET);
    }
    destroy(&v_radix);
    printf("----------------------------------------\n");

    // Test `mergeSort()`
    Vector v_merge = populate(unsorted_data, 5);
    mergeSort(&v_merge);
    printf("Test: mergeSort()\n");
    printf("  Expected: ");
    printVectorForTest(&expected_v);
    printf("  Actual:   ");
    printVectorForTest(&v_merge);
    if (areVectorsEqual(&v_merge, &expected_v))
    {
        printf("  Result: " GREEN "Passed\n" RESET);
    }
    else
    {
        printf("  Result: " RED "Failed\n" RESET);
    }
    destroy(&v_merge);
    printf("----------------------------------------\n");

    // Test `quickSort()`
    Vector v_quick = populate(unsorted_data, 5);
    quickSort(&v_quick);
    printf("Test: quickSort()\n");
    printf("  Expected: ");
    printVectorForTest(&expected_v);
    printf("  Actual:   ");
    printVectorForTest(&v_quick);
    if (areVectorsEqual(&v_quick, &expected_v))
    {
        printf("  Result: " GREEN "Passed\n" RESET);
    }
    else
    {
        printf("  Result: " RED "Failed\n" RESET);
    }
    destroy(&v_quick);
    printf("----------------------------------------\n");

    // Test `heapSort()`
    Vector v_heap = populate(unsorted_data, 5);
    heapSort(&v_heap);
    printf("Test: heapSort()\n");
    printf("  Expected: ");
    printVectorForTest(&expected_v);
    printf("  Actual:   ");
    printVectorForTest(&v_heap);
    if (areVectorsEqual(&v_heap, &expected_v))
    {
        printf("  Result: " GREEN "Passed\n" RESET);
    }
    else
    {
        printf("  Result: " RED "Failed\n" RESET);
    }
    destroy(&v_heap);
    printf("----------------------------------------\n");

    destroy(&expected_v);
}

// ========================================
// Test Group: Functional Programming
// ========================================
void testFunctionalProgrammingInspired()
{
    printf(YELLOW "--- Functional Programming Inspired ---\n" RESET);

    int data[] = {2, 3, 4, 5, 6};
    Vector v = populate(data, 5);

    // Test `map()`
    printf("Test: map()\n");
    map(&v, square);
    int expected_map_data[] = {4, 9, 16, 25, 36};
    Vector expected_map = populate(expected_map_data, 5);
    printf("  Expected: ");
    printVectorForTest(&expected_map);
    printf("  Actual:   ");
    printVectorForTest(&v);
    if (areVectorsEqual(&v, &expected_map))
    {
        printf("  Result: " GREEN "Passed\n" RESET);
    }
    else
    {
        printf("  Result: " RED "Failed\n" RESET);
    }
    destroy(&expected_map);
    printf("----------------------------------------\n");

    // Test `filter()`
    Vector v_filter = populate(data, 5); // reset with original data
    filter(&v_filter, isEven);
    int expected_filter_data[] = {2, 4, 6};
    Vector expected_filter = populate(expected_filter_data, 3);
    printf("Test: filter()\n");
    printf("  Expected: ");
    printVectorForTest(&expected_filter);
    printf("  Actual:   ");
    printVectorForTest(&v_filter);
    if (areVectorsEqual(&v_filter, &expected_filter))
    {
        printf("  Result: " GREEN "Passed\n" RESET);
    }
    else
    {
        printf("  Result: " RED "Failed\n" RESET);
    }
    destroy(&v_filter);
    destroy(&expected_filter);
    printf("----------------------------------------\n");

    // Test `max()` and `min()`
    printf("Test: max() and min()\n");
    int actual_max = max(&v);
    int actual_min = min(&v);
    printf("  Vector under test: ");
    printVectorForTest(&v);
    printf("  Expected: max=36, min=4\n");
    printf("  Actual:   max=%d, min=%d\n", actual_max, actual_min);
    if (actual_max == 36 && actual_min == 4)
    {
        printf("  Result: " GREEN "Passed\n" RESET);
    }
    else
    {
        printf("  Result: " RED "Failed\n" RESET);
    }
    printf("----------------------------------------\n");

    // Test `sum()` and `prod()`
    printf("Test: sum() and prod()\n");
    int actual_sum = sum(&v);
    int actual_prod = prod(&v);
    printf("  Vector under test: ");
    printVectorForTest(&v);
    printf("  Expected: sum=90, prod=518400\n");
    printf("  Actual:   sum=%d, prod=%d\n", actual_sum, actual_prod);
    if (actual_sum == 90 && actual_prod == 518400)
    {
        printf("  Result: " GREEN "Passed\n" RESET);
    }
    else
    {
        printf("  Result: " RED "Failed\n" RESET);
    }
    printf("----------------------------------------\n");

    // Test `count()` and `contains()`
    Vector v_count_contains = populate((int[]){10, 20, 10, 30}, 4);
    printf("Test: count() and contains()\n");
    int count_10 = count(&v_count_contains, 10);
    bool contains_20 = contains(&v_count_contains, 20);
    bool contains_99 = contains(&v_count_contains, 99);
    printf("  Vector under test: ");
    printVectorForTest(&v_count_contains);
    printf("  Expected: count(10)=2, contains(20)=true, contains(99)=false\n");
    printf("  Actual:   count(10)=%d, contains(20)=%s, contains(99)=%s\n", count_10, contains_20 ? "true" : "false", contains_99 ? "true" : "false");
    if (count_10 == 2 && contains_20 && !contains_99)
    {
        printf("  Result: " GREEN "Passed\n" RESET);
    }
    else
    {
        printf("  Result: " RED "Failed\n" RESET);
    }
    destroy(&v_count_contains);
    printf("----------------------------------------\n");

    destroy(&v);
}