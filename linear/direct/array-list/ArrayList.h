#ifndef ARRAYLIST_H
#define ARRAYLIST_H

#include <stdlib.h>  // For malloc, realloc, free, exit
#include <stdio.h>   // For perror, printf
#include <stdbool.h> // For bool type
#include <math.h>    // For sqrt in jumpSearch
#include <time.h>    // For time in randomize

/**
 * @brief Defines the structure for an ArrayList.
 * It holds a pointer to an integer array,
 * the current number of elements (length),
 * and the total allocated memory (capacity).
 */
typedef struct
{
    int *array;
    int length;
    int capacity;
} ArrayList;

/**
 * @brief Swaps the values of two integers.
 * @param a A pointer to the first integer.
 * @param b A pointer to the second integer.
 */
void __swap__(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

/**
 * @brief Initializes a new ArrayList with a specified capacity.
 * Allocates memory for the underlying array.
 * @param capacity The initial capacity of the ArrayList.
 * @return An initialized ArrayList.
 * @note Exits with an error if capacity is invalid or memory allocation fails.
 */
ArrayList init(const int capacity)
{
    if (capacity < 0)
    {
        perror("Invalid capacity for ArrayList");
        exit(EXIT_FAILURE);
    }
    ArrayList result;
    result.array = (int *)malloc(capacity * sizeof(int));
    if (result.array == NULL)
    {
        perror("Failed to initialize ArrayList");
        exit(EXIT_FAILURE);
    }
    result.length = 0;
    result.capacity = capacity;
    return result;
}

/**
 * @brief Checks if the ArrayList is empty.
 * @param list The ArrayList to check.
 * @return True if the list's length is 0, false otherwise.
 */
bool isEmpty(const ArrayList list)
{
    return list.length == 0;
}

/**
 * @brief Checks if the ArrayList is full (reached its current capacity).
 * @param list The ArrayList to check.
 * @return True if the list's length equals its capacity, false otherwise.
 */
bool isFull(const ArrayList list)
{
    return list.length == list.capacity;
}

/**
 * @brief Creates a deep copy of an existing ArrayList.
 * @param list The ArrayList to copy.
 * @return A new ArrayList containing the same elements and capacity.
 */
ArrayList copy(const ArrayList list)
{
    ArrayList copied = init(list.capacity);
    for (int i = 0; i < list.length; i++)
        copied.array[i] = list.array[i];
    copied.length = list.length;
    return copied;
}

/**
 * @brief Creates a new ArrayList containing a slice of an existing list.
 * @param list The original ArrayList.
 * @param start The starting index (inclusive).
 * @param end The ending index (exclusive).
 * @return A new ArrayList representing the slice. Returns an empty list if indices are invalid.
 */
ArrayList slice(ArrayList list, int start, int end)
{
    if (start < 0 || end > list.length || start > end)
        return init(0); // Return an empty list for invalid slice parameters
    ArrayList sliced = init(end - start);
    for (int i = start; i < end; i++)
    {
        sliced.array[sliced.length++] = list.array[i];
    }
    return sliced;
}

/**
 * @brief Joins two ArrayLists into a new single ArrayList.
 * @param front The first ArrayList.
 * @param rear The second ArrayList.
 * @return A new ArrayList containing elements from both `front` and `rear`.
 */
ArrayList join(const ArrayList front, const ArrayList rear)
{
    ArrayList joined = init(front.length + rear.length);
    for (int i = 0; i < front.length; i++)
        joined.array[joined.length++] = front.array[i];
    for (int i = 0; i < rear.length; i++)
        joined.array[joined.length++] = rear.array[i];
    return joined;
}

/**
 * @brief Clears all elements from the ArrayList, setting its length to 0.
 * Does not deallocate the underlying array memory.
 * @param list A pointer to the ArrayList to clear.
 */
void clear(ArrayList *list)
{
    if (list == NULL)
        return;
    list->length = 0;
}

/**
 * @brief Deallocates the memory associated with the ArrayList.
 * Sets the array pointer to NULL and resets length and capacity.
 * @param list A pointer to the ArrayList to destroy.
 */
void destroy(ArrayList *list)
{
    if (list == NULL)
        return;
    free(list->array);
    list->array = NULL;
    list->length = 0;
    list->capacity = 0;
}

/**
 * @brief Prints the elements of the ArrayList to the console.
 * Also shows the current length and capacity.
 * @param list The ArrayList to traverse (print).
 */
void traverse(const ArrayList list)
{
    printf("[ ");
    for (int i = 0; i < list.length; i++)
        printf("%2d ", list.array[i]);
    printf("] : %d/%d\n", list.length, list.capacity);
}

/**
 * @brief Retrieves the value at a specific index in the ArrayList.
 * @param list The ArrayList to query.
 * @param index The index of the element to retrieve.
 * @return The value at the specified index, or -1 if the index is invalid.
 */
int get(ArrayList list, int index)
{
    if (index < 0 || index >= list.length)
        return -1; // Indicate error/invalid index
    return list.array[index];
}

/**
 * @brief Sets the value of an element at a specific index in the ArrayList.
 * @param list A pointer to the ArrayList to modify.
 * @param value The new value to set.
 * @param index The index at which to set the value.
 */
void set(ArrayList *list, int value, int index)
{
    if (list == NULL || index < 0 || index >= list->length)
        return;
    list->array[index] = value;
}

/**
 * @brief Replaces the first occurrence of 'old' with 'new', starting from a given index.
 * @param list A pointer to the ArrayList to modify.
 * @param new The new value to insert.
 * @param old The value to be replaced.
 * @param index The starting index to begin the search.
 */
void replace(ArrayList *list, int new, int old, int index)
{
    if (list == NULL || index >= list->length)
        return;

    for (int i = index; i < list->length; i++)
    {
        if (list->array[i] == old)
        {
            list->array[i] = new;
            return; // Replace only the first occurrence
        }
    }
}

/**
 * @brief Expands the capacity of the ArrayList when it becomes full.
 * Doubles the current capacity or sets it to 1 if it was 0.
 * @param list A pointer to the ArrayList to expand.
 * @note Exits with an error if memory reallocation fails.
 */
void __expand__(ArrayList *list)
{
    int newCapacity = (list->capacity == 0) ? 1 : list->capacity * 2;
    int *newArray = (int *)realloc(list->array, newCapacity * sizeof(int));
    if (newArray == NULL)
    {
        perror("Failed to expand ArrayList");
        exit(EXIT_FAILURE);
    }
    list->array = newArray;
    list->capacity = newCapacity;
}

/**
 * @brief Appends a value to the end of the ArrayList.
 * Expands the list's capacity if it's full.
 * @param list A pointer to the ArrayList to modify.
 * @param value The value to append.
 */
void append(ArrayList *list, int value)
{
    if (list == NULL)
        return;
    if (isFull(*list))
        __expand__(list);
    list->array[list->length++] = value;
}

/**
 * @brief Inserts a value at a specified index in the ArrayList.
 * Elements from the index onwards are shifted to the right.
 * Expands the list's capacity if it's full.
 * @param list A pointer to the ArrayList to modify.
 * @param value The value to insert.
 * @param index The index at which to insert the value.
 */
void insert(ArrayList *list, int value, int index)
{
    if (list == NULL || index < 0 || index > list->length)
        return;
    if (isFull(*list))
        __expand__(list);
    for (int i = list->length; i > index; i--)
        list->array[i] = list->array[i - 1];
    list->array[index] = value;
    list->length++;
}

/**
 * @brief Removes and returns the last element from the ArrayList.
 * @param list A pointer to the ArrayList to modify.
 * @return The value of the popped element, or -1 if the list is empty.
 */
int pop(ArrayList *list)
{
    if (list == NULL || isEmpty(*list))
        return -1;
    return list->array[--list->length];
}

/**
 * @brief Removes and returns the element at a specified index.
 * Elements after the removed element are shifted to the left.
 * @param list A pointer to the ArrayList to modify.
 * @param index The index of the element to discard.
 * @return The value of the discarded element, or -1 if the index is invalid or list is empty.
 */
int discard(ArrayList *list, int index)
{
    if (list == NULL || isEmpty(*list) || index < 0 || index >= list->length)
        return -1;
    int value = list->array[index];
    for (int i = index; i < list->length - 1; i++)
        list->array[i] = list->array[i + 1];
    list->length--;
    return value;
}

/**
 * @brief Fills the ArrayList with a specified quantity of a given value.
 * Clears any existing elements before filling.
 * @param list A pointer to the ArrayList to modify.
 * @param quantity The number of elements to fill.
 * @param value The value to fill the list with.
 */
void fill(ArrayList *list, const int quantity, const int value)
{
    if (list == NULL || quantity < 0)
        return;
    clear(list); // Clear existing elements
    for (int i = 0; i < quantity; i++)
        append(list, value);
}

/**
 * @brief Reverses the order of elements in the ArrayList in-place.
 * @param list A pointer to the ArrayList to modify.
 */
void reverse(ArrayList *list)
{
    if (list == NULL || list->length <= 1)
        return;
    for (int i = 0; i < list->length / 2; i++)
        __swap__(&list->array[i], &list->array[list->length - i - 1]);
}

/**
 * @brief Seeds the random number generator once.
 * Internal helper function to ensure randomness.
 */
void __seeding__()
{
    static int seeded = 0; // Static variable to ensure seeding happens only once
    if (!seeded)
    {
        srand((unsigned int)time(NULL));
        seeded = 1;
    }
}

/**
 * @brief Fills the ArrayList with a specified quantity of random integers
 * within a given range [min, max).
 * Clears any existing elements before randomizing.
 * @param list A pointer to the ArrayList to modify.
 * @param quantity The number of random elements to generate.
 * @param min The minimum value (inclusive) for random numbers.
 * @param max The maximum value (exclusive) for random numbers.
 */
void randomize(ArrayList *list, const int quantity, const int min, const int max)
{
    if (list == NULL || quantity < 0 || min >= max)
        return;
    __seeding__(); // Ensure srand is called
    clear(list);   // Clear existing elements
    for (int i = 0; i < quantity; i++)
        append(list, rand() % (max - min) + min);
}

/**
 * @brief Randomly shuffles the elements in the ArrayList.
 * Uses the Fisher-Yates (Knuth) shuffle algorithm.
 * @param list A pointer to the ArrayList to shuffle.
 */
void shuffle(ArrayList *list)
{
    if (list == NULL || list->length <= 1)
        return;
    __seeding__(); // Ensure srand is called
    for (int i = list->length - 1; i > 0; i--)
    {
        int j = rand() % (i + 1); // Generate a random index from 0 to i
        __swap__(&list->array[i], &list->array[j]);
    }
}

// --- Search Algorithms ---

/**
 * @brief Performs a linear search for a value starting from a given index.
 * @param list The ArrayList to search.
 * @param value The value to find.
 * @param index The starting index for the search (inclusive).
 * @return The index of the first occurrence of the value, or -1 if not found or invalid index.
 */
int linearSearch(ArrayList list, int value, int index)
{
    if (index < 0 || index >= list.length)
        return -1;

    for (int i = index; i < list.length; i++)
    {
        if (list.array[i] == value)
            return i;
    }
    return -1;
}

/**
 * @brief Performs a binary search for a value in a sorted ArrayList.
 * The search is conducted within the range [index, list.length - 1].
 * @param list The ArrayList to search (must be sorted for correct results).
 * @param value The value to find.
 * @param index The starting index for the search (inclusive).
 * @return The index of the value, or -1 if not found or invalid index.
 */
int binarySearch(ArrayList list, int value, int index)
{
    if (index < 0 || index >= list.length)
        return -1;

    int start = index;
    int end = list.length - 1;
    while (start <= end)
    {
        int mid = start + (end - start) / 2; // Prevents potential overflow compared to (start + end) / 2
        if (list.array[mid] == value)
            return mid;
        else if (list.array[mid] < value)
            start = mid + 1;
        else
            end = mid - 1;
    }
    return -1;
}

/**
 * @brief Performs a ternary search for a value in a sorted ArrayList.
 * The search is conducted within the range [index, list.length - 1].
 * @param list The ArrayList to search (must be sorted for correct results).
 * @param value The value to find.
 * @param index The starting index for the search (inclusive).
 * @return The index of the value, or -1 if not found or invalid index.
 */
int ternarySearch(ArrayList list, int value, int index)
{
    if (index < 0 || index >= list.length)
        return -1;

    int start = index;
    int end = list.length - 1;
    while (start <= end)
    {
        int mid1 = start + (end - start) / 3;
        int mid2 = end - (end - start) / 3;

        if (list.array[mid1] == value)
            return mid1;
        if (list.array[mid2] == value)
            return mid2;

        if (value < list.array[mid1])
            end = mid1 - 1;
        else if (value > list.array[mid2])
            start = mid2 + 1;
        else
        { // value is between mid1 and mid2
            start = mid1 + 1;
            end = mid2 - 1;
        }
    }
    return -1;
}

/**
 * @brief Performs an interpolation search for a value in a uniformly distributed, sorted ArrayList.
 * The search is conducted within the range [index, list.length - 1].
 * @param list The ArrayList to search (must be sorted and preferably uniformly distributed).
 * @param value The value to find.
 * @param index The starting index for the search (inclusive).
 * @return The index of the value, or -1 if not found or invalid index.
 */
int interpolationSearch(ArrayList list, int value, int index)
{
    if (index < 0 || index >= list.length)
        return -1;

    int start = index;
    int end = list.length - 1;
    while (start <= end && value >= list.array[start] && value <= list.array[end])
    {
        // Avoid division by zero when start and end elements are the same
        if (list.array[start] == list.array[end])
            break;

        // Calculate the probe position
        int pos = start + (int)(((double)(end - start) * (value - list.array[start])) / (list.array[end] - list.array[start]));

        // Check if the calculated position is within bounds
        if (pos < start || pos > end)
            break;

        if (list.array[pos] == value)
            return pos;
        else if (list.array[pos] < value)
            start = pos + 1;
        else
            end = pos - 1;
    }
    // If the value is at start and loop breaks early due to start/end equality
    if (start <= end && list.array[start] == value)
    {
        return start;
    }
    return -1;
}

/**
 * @brief Performs a jump search for a value in a sorted ArrayList.
 * The search is conducted within the range [index, list.length - 1].
 * @param list The ArrayList to search (must be sorted).
 * @param value The value to find.
 * @param index The starting index for the search (inclusive).
 * @return The index of the value, or -1 if not found or invalid index.
 */
int jumpSearch(ArrayList list, int value, int index)
{
    if (index < 0 || index >= list.length)
        return -1;

    int jump = (int)sqrt(list.length); // Optimal block size
    int start = index;
    int end = start + jump;

    // Find the block where the element might be present
    while (end < list.length && list.array[end] <= value)
    {
        start = end;
        end += jump;
    }

    // Adjust 'end' if it overshoots the list length
    if (end > list.length)
        end = list.length;

    // Perform linear search in the identified block
    for (int i = start; i < end; i++)
        if (list.array[i] == value)
            return i;
    return -1;
}

// --- Sorting Algorithms ---

/**
 * @brief Sorts the ArrayList using the Bubble Sort algorithm.
 * Compares adjacent elements and swaps them if they are in the wrong order.
 * Repeats until no swaps are needed.
 * @param list A pointer to the ArrayList to sort.
 */
void bubbleSort(ArrayList *list)
{
    if (list == NULL || list->length <= 1)
        return;
    for (int i = 1; i < list->length; i++)
    {
        bool isSorted = true; // Optimization: if no swaps, array is sorted
        for (int j = 0; j < list->length - i; j++)
        {
            if (list->array[j] > list->array[j + 1])
            {
                __swap__(&list->array[j], &list->array[j + 1]);
                isSorted = false;
            }
        }
        if (isSorted)
            break;
    }
}

/**
 * @brief Sorts the ArrayList using the Selection Sort algorithm.
 * Repeatedly finds the minimum element from the unsorted part
 * and puts it at the beginning of the sorted part.
 * @param list A pointer to the ArrayList to sort.
 */
void selectionSort(ArrayList *list)
{
    if (list == NULL || list->length <= 1)
        return;
    for (int i = 0; i < list->length - 1; i++)
    {
        int minIndex = i;
        for (int j = i + 1; j < list->length; j++)
        {
            if (list->array[j] < list->array[minIndex])
                minIndex = j;
        }
        if (minIndex != i) // Swap if the minimum element is not already in place
            __swap__(&list->array[i], &list->array[minIndex]);
    }
}

/**
 * @brief Sorts the ArrayList using the Insertion Sort algorithm.
 * Builds the final sorted array one item at a time.
 * It iterates through the input elements and inserts each element
 * into its correct position in the already sorted part.
 * @param list A pointer to the ArrayList to sort.
 */
void insertionSort(ArrayList *list)
{
    if (list == NULL || list->length <= 1)
        return;
    for (int i = 1; i < list->length; i++)
    {
        int curr = list->array[i];
        int j = i - 1;
        // Move elements of list->array[0..i-1], that are greater than curr,
        // to one position ahead of their current position
        while (j >= 0 && list->array[j] > curr)
        {
            list->array[j + 1] = list->array[j];
            j--;
        }
        list->array[j + 1] = curr;
    }
}

/**
 * @brief Sorts the ArrayList using the Counting Sort algorithm.
 * It is an efficient sorting algorithm for integers when the range
 * of input numbers is not significantly larger than the number of items.
 * @param list A pointer to the ArrayList to sort.
 * @note This algorithm is stable and non-comparison based.
 * @note Exits with an error if memory allocation fails for counting or result arrays.
 */
void countingSort(ArrayList *list)
{
    if (list == NULL || list->length <= 1)
        return;

    // Find min and max elements to determine the range
    int min = list->array[0];
    int max = list->array[0];
    for (int i = 1; i < list->length; i++)
    {
        if (list->array[i] < min)
            min = list->array[i];
        if (list->array[i] > max)
            max = list->array[i];
    }

    int range = max - min + 1;
    // Create a counting array, initialized to zeros
    int *counting = (int *)calloc(range, sizeof(int));
    if (counting == NULL)
    {
        perror("Failed to allocate memory for Counting Array");
        exit(EXIT_FAILURE);
    }

    // Store count of each character
    for (int i = 0; i < list->length; i++)
        counting[list->array[i] - min]++;

    // Change counting[i] so that counting[i] now contains actual
    // position of this character in output array
    for (int i = 1; i < range; i++)
        counting[i] += counting[i - 1];

    // Build the output array
    int *result = (int *)malloc(list->length * sizeof(int));
    if (result == NULL)
    {
        perror("Failed to allocate memory for Result Array");
        free(counting); // Clean up allocated memory before exiting
        exit(EXIT_FAILURE);
    }

    // Iterate from the end to maintain stability
    for (int i = list->length - 1; i >= 0; i--)
    {
        int val = list->array[i];
        result[--counting[val - min]] = val;
    }

    // Copy the sorted elements back to the original list
    for (int i = 0; i < list->length; i++)
        list->array[i] = result[i];

    // Free the auxiliary arrays
    free(counting);
    free(result);
}

/**
 * @brief Merges two sorted ArrayLists into a single sorted ArrayList.
 * This is a helper function for mergeSort.
 * @param list A pointer to the destination ArrayList where merged elements will be stored.
 * @param left A pointer to the left sorted ArrayList.
 * @param right A pointer to the right sorted ArrayList.
 */
void __merge__(ArrayList *list, const ArrayList *left, const ArrayList *right)
{
    int l = 0, r = 0, i = 0;

    // Compare elements from left and right lists and place the smaller one into 'list'
    while (l < left->length && r < right->length)
    {
        if (left->array[l] <= right->array[r])
            list->array[i++] = left->array[l++];
        else
            list->array[i++] = right->array[r++];
    }

    // Copy any remaining elements from the left list
    while (l < left->length)
        list->array[i++] = left->array[l++];
    // Copy any remaining elements from the right list
    while (r < right->length)
        list->array[i++] = right->array[r++];
}

/**
 * @brief Sorts the ArrayList using the Merge Sort algorithm.
 * A divide-and-conquer algorithm that recursively divides the list
 * into halves, sorts them, and then merges the sorted halves.
 * @param list A pointer to the ArrayList to sort.
 */
void mergeSort(ArrayList *list)
{
    if (list == NULL || list->length <= 1)
        return;

    int mid = list->length / 2;
    // Create left and right sub-lists (slices)
    ArrayList left = slice(*list, 0, mid);
    ArrayList right = slice(*list, mid, list->length);

    // Recursively sort the sub-lists
    mergeSort(&left);
    mergeSort(&right);

    // Merge the sorted sub-lists back into the original list
    __merge__(list, &left, &right);

    // Free the memory allocated for the temporary left and right sub-lists
    destroy(&left);
    destroy(&right);
}

/**
 * @brief Partitions the ArrayList for Quick Sort.
 * This is a recursive helper function for quickSort.
 * It selects a pivot element and partitions the array around it.
 * @param list A pointer to the ArrayList to partition.
 * @param start The starting index of the sub-array to partition.
 * @param end The ending index of the sub-array to partition.
 */
void __partition__(ArrayList *list, int start, int end)
{
    // Base case: if the sub-array has 0 or 1 element, it's already sorted
    if (end - start <= 0)
        return;

    int pivot = list->array[end]; // Choosing the last element as pivot
    int i = start - 1;            // Index of smaller element

    // Iterate through the sub-array
    for (int j = start; j <= end; j++)
        if (list->array[j] <= pivot)
            __swap__(&list->array[++i], &list->array[j]); // Move smaller elements to the left of 'i'

    // Recursively sort the sub-arrays to the left and right of the pivot
    __partition__(list, start, i - 1);
    __partition__(list, i + 1, end);
}

/**
 * @brief Sorts the ArrayList using the Quick Sort algorithm.
 * A divide-and-conquer algorithm that selects a 'pivot' element
 * and partitions the array around the picked pivot.
 * @param list A pointer to the ArrayList to sort.
 */
void quickSort(ArrayList *list)
{
    if (list == NULL || list->length <= 1)
        return;
    __partition__(list, 0, list->length - 1);
}

/**
 * @brief Helper function for Heap Sort: maintains the heap property.
 * Performs heapify operation on a sub-tree rooted with given index.
 * @param list A pointer to the ArrayList.
 * @param size The current size of the heap (portion of the array).
 * @param index The root index of the sub-tree to heapify.
 */
void __heapify__(ArrayList *list, int size, int index)
{
    int largest = index;       // Initialize largest as root
    int left = 2 * index + 1;  // Left child
    int right = 2 * index + 2; // Right child

    // If left child is larger than root
    if (left < size && list->array[left] > list->array[largest])
        largest = left;

    // If right child is larger than largest so far
    if (right < size && list->array[right] > list->array[largest])
        largest = right;

    // If largest is not root
    if (largest != index)
    {
        __swap__(&list->array[index], &list->array[largest]);
        // Recursively heapify the affected sub-tree
        __heapify__(list, size, largest);
    }
}

/**
 * @brief Sorts the ArrayList using the Heap Sort algorithm.
 * First, it builds a max-heap from the input data.
 * Then, it repeatedly extracts the maximum element from the heap
 * and places it at the end of the array.
 * @param list A pointer to the ArrayList to sort.
 */
void heapSort(ArrayList *list)
{
    if (list == NULL || list->length <= 1)
        return;

    // Build max heap (rearrange array)
    for (int i = list->length / 2 - 1; i >= 0; i--)
        __heapify__(list, list->length, i);

    // One by one extract elements from heap
    for (int i = list->length - 1; i > 0; i--)
    {
        // Move current root to end
        __swap__(&list->array[0], &list->array[i]);
        // Call max heapify on the reduced heap
        __heapify__(list, i, 0);
    }
}

// --- Functional Programming Helpers ---

/**
 * @brief Applies a given function to each element in the ArrayList,
 * modifying the elements in-place.
 * @param list A pointer to the ArrayList to modify.
 * @param func A pointer to a function that takes an int and returns an int.
 */
void map(ArrayList *list, int (*func)(int))
{
    for (int i = 0; i < list->length; i++)
        list->array[i] = func(list->array[i]);
}

/**
 * @brief Filters the elements of the ArrayList based on a given predicate function.
 * Only elements for which the function returns true are kept.
 * @param list A pointer to the ArrayList to modify.
 * @param func A pointer to a function that takes an int and returns a bool.
 */
void filter(ArrayList *list, bool (*func)(int))
{
    int index = 0; // New index for filtered elements
    for (int i = 0; i < list->length; i++)
        if (func(list->array[i]))
            list->array[index++] = list->array[i]; // Keep element if func returns true
    list->length = index;                          // Update length to the number of filtered elements
}

/**
 * @brief Checks if any element in the ArrayList is non-zero (truthy).
 * @param list The ArrayList to check.
 * @return True if at least one element is non-zero, false otherwise.
 */
bool any(ArrayList list)
{
    for (int i = 0; i < list.length; i++)
        if (list.array[i]) // If any element is non-zero
            return true;
    return false;
}

/**
 * @brief Checks if all elements in the ArrayList are non-zero (truthy).
 * @param list The ArrayList to check.
 * @return True if all elements are non-zero, false otherwise.
 */
bool all(ArrayList list)
{
    for (int i = 0; i < list.length; i++)
        if (!list.array[i]) // If any element is zero
            return false;
    return true;
}

/**
 * @brief Finds the maximum value in the ArrayList.
 * @param list The ArrayList to search.
 * @return The maximum value, or -1 if the list is empty.
 */
int max(ArrayList list)
{
    if (isEmpty(list))
        return -1; // Or throw an error, depending on desired behavior for empty list
    int maximum = list.array[0];
    for (int i = 1; i < list.length; i++)
        if (maximum < list.array[i])
            maximum = list.array[i];
    return maximum;
}

/**
 * @brief Finds the minimum value in the ArrayList.
 * @param list The ArrayList to search.
 * @return The minimum value, or -1 if the list is empty.
 */
int min(ArrayList list)
{
    if (isEmpty(list))
        return -1; // Or throw an error, depending on desired behavior for empty list
    int minimum = list.array[0];
    for (int i = 1; i < list.length; i++)
        if (minimum > list.array[i])
            minimum = list.array[i];
    return minimum;
}

/**
 * @brief Calculates the sum of all elements in the ArrayList.
 * @param list The ArrayList to sum.
 * @return The sum of the elements.
 */
int sum(ArrayList list)
{
    int total = 0;
    for (int i = 0; i < list.length; i++)
        total += list.array[i];
    return total;
}

/**
 * @brief Calculates the product of all elements in the ArrayList.
 * @param list The ArrayList to process.
 * @return The product of the elements.
 */
int prod(ArrayList list)
{
    int product = 1;
    for (int i = 0; i < list.length; i++)
        product *= list.array[i];
    return product;
}

/**
 * @brief Counts the occurrences of a specific value in the ArrayList.
 * @param list The ArrayList to search.
 * @param value The value to count.
 * @return The number of times the value appears in the list.
 */
int count(ArrayList list, int value)
{
    int freq = 0;
    for (int i = 0; i < list.length; i++)
        if (list.array[i] == value)
            freq++;
    return freq;
}

/**
 * @brief Checks if the ArrayList contains a specific value.
 * @param list The ArrayList to search.
 * @param value The value to check for.
 * @return True if the value is found, false otherwise.
 */
bool contains(ArrayList list, int value)
{
    for (int i = 0; i < list.length; i++)
        if (list.array[i] == value)
            return true;
    return false;
}

#endif // ARRAYLIST_H