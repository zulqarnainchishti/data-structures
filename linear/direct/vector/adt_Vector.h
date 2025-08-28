#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

/**
 * @brief Represents a dynamic array (vector) of integers.
 * @details This struct holds a pointer to the array, the number of elements
 * currently in the vector (length), and the total allocated memory capacity.
 */
typedef struct
{
    int *array;
    int length;
    int capacity;
} Vector;

/**
 * @brief Initializes a new Vector with a given capacity.
 * @param capacity The initial capacity of the vector. Must be > 0.
 * @return A new Vector instance.
 * @note Exits the program if memory allocation fails or capacity is invalid.
 */
Vector init(const int capacity)
{
    if (capacity <= 0)
    {
        perror("Invalid capacity for Vector");
        exit(EXIT_FAILURE);
    }
    Vector result;
    result.array = (int *)malloc(capacity * sizeof(int));
    if (result.array == NULL)
    {
        perror("Failed to initialize Vector");
        exit(EXIT_FAILURE);
    }
    result.length = 0;
    result.capacity = capacity;
    return result;
}

/**
 * @brief Creates a deep copy of an existing vector.
 * @param vector A pointer to the vector to copy.
 * @return A new Vector instance that is a copy of the original.
 * @note Returns a new vector with capacity 1 if the input vector is NULL.
 */
Vector copy(const Vector *vector)
{
    if (vector == NULL)
        return init(1);
    Vector copied = init(vector->capacity);
    for (int i = 0; i < vector->length; i++)
        copied.array[i] = vector->array[i];
    copied.length = vector->length;
    return copied;
}

/**
 * @brief Clears the contents of a vector by resetting its length to 0.
 * @param vector A pointer to the vector to clear.
 * @note This function does not free the allocated memory.
 */
void clear(Vector *vector)
{
    if (vector == NULL)
        return;
    vector->length = 0;
}

/**
 * @brief Destroys a vector and frees its allocated memory.
 * @param vector A pointer to the vector to destroy.
 * @note The vector pointer's internal members are set to NULL or 0 after freeing.
 */
void destroy(Vector *vector)
{
    if (vector == NULL)
        return;
    free(vector->array);
    vector->array = NULL;
    vector->length = 0;
    vector->capacity = 0;
}

/**
 * @brief Creates a new vector from a slice of an existing vector.
 * @param vector A pointer to the original vector.
 * @param start The starting index of the slice (inclusive).
 * @param end The ending index of the slice (exclusive).
 * @return A new Vector instance containing the sliced elements.
 * @note Returns a new vector with capacity 1 if the input is invalid.
 */
Vector slice(const Vector *vector, const int start, const int end)
{
    if (vector == NULL || start < 0 || end > vector->length || start > end)
        return init(1);
    Vector sliced = init(end - start);
    for (int i = start; i < end; i++)
        sliced.array[sliced.length++] = vector->array[i];
    return sliced;
}

/**
 * @brief Joins two vectors into a new vector.
 * @param front A pointer to the first vector.
 * @param rear A pointer to the second vector.
 * @return A new Vector instance containing the elements of both vectors.
 * @note Returns a new vector with capacity 1 if either input vector is NULL.
 */
Vector join(const Vector *front, const Vector *rear)
{
    if (front == NULL || rear == NULL)
        return init(1);
    Vector joined = init(front->length + rear->length);
    for (int i = 0; i < front->length; i++)
        joined.array[joined.length++] = front->array[i];
    for (int i = 0; i < rear->length; i++)
        joined.array[joined.length++] = rear->array[i];
    return joined;
}

/**
 * @brief Checks if a vector is empty.
 * @param vector A pointer to the vector.
 * @return true if the vector's length is 0, false otherwise.
 */
bool isEmpty(const Vector *vector)
{
    if (vector == NULL)
        return true;
    return vector->length == 0;
}

/**
 * @brief Prints the contents of a vector to the console.
 * @param vector A pointer to the vector to print.
 */
void traverse(const Vector *vector)
{
    if (vector == NULL)
    {
        printf("Vector is empty\n");
        return;
    }
    printf("[ ");
    for (int i = 0; i < vector->length; i++)
        printf("%2d ", vector->array[i]);
    printf("] : %d/%d\n", vector->length, vector->capacity);
}

/**
 * @brief Gets the value at a specific index.
 * @param vector A pointer to the vector.
 * @param index The index of the element to retrieve.
 * @return The value at the specified index, or -1 if the index is invalid.
 */
int get(const Vector *vector, const int index)
{
    if (vector == NULL || index < 0 || index >= vector->length)
        return -1;
    return vector->array[index];
}

/**
 * @brief Sets the value at a specific index.
 * @param vector A pointer to the vector.
 * @param value The new value to set.
 * @param index The index at which to set the new value.
 * @note This function does nothing if the index is out of bounds.
 */
void set(Vector *vector, const int value, const int index)
{
    if (vector == NULL || index < 0 || index >= vector->length)
        return;
    vector->array[index] = value;
}

/**
 * @brief Replaces the first occurrence of an old value with a new value from a given starting index.
 * @param vector A pointer to the vector.
 * @param newVal The new value to insert.
 * @param oldVal The old value to be replaced.
 * @param index The starting index for the search.
 */
void replace(Vector *vector, const int newVal, const int oldVal, const int index)
{
    if (vector == NULL || index < 0 || index >= vector->length)
        return;
    for (int i = index; i < vector->length; i++)
    {
        if (vector->array[i] == oldVal)
        {
            vector->array[i] = newVal;
            return;
        }
    }
}

/**
 * @brief Checks if the vector's length has reached its capacity.
 * @param vector A pointer to the vector.
 * @return true if the vector is full, false otherwise.
 */
bool __isFull__(const Vector *vector)
{
    return vector->length == vector->capacity;
}

/**
 * @brief Doubles the vector's capacity and reallocates memory.
 * @param vector A pointer to the vector to expand.
 * @note This is a private helper function.
 * @note Exits the program if realloc fails.
 */
void __expand__(Vector *vector)
{
    int newCapacity = vector->capacity == 0 ? 1 : vector->capacity * 2;
    int *newArray = (int *)realloc(vector->array, newCapacity * sizeof(int));
    if (newArray == NULL)
    {
        perror("Failed to expand Vector");
        exit(EXIT_FAILURE);
    }
    vector->array = newArray;
    vector->capacity = newCapacity;
}

/**
 * @brief Appends a value to the end of the vector.
 * @param vector A pointer to the vector.
 * @param value The value to append.
 */
void append(Vector *vector, const int value)
{
    if (vector == NULL)
        return;
    if (__isFull__(vector))
        __expand__(vector);
    vector->array[vector->length++] = value;
}

/**
 * @brief Inserts a value at a specific index, shifting existing elements.
 * @param vector A pointer to the vector.
 * @param value The value to insert.
 * @param index The index at which to insert the value.
 */
void insert(Vector *vector, const int value, const int index)
{
    if (vector == NULL || index < 0 || index > vector->length)
        return;
    if (__isFull__(vector))
        __expand__(vector);
    for (int i = vector->length; i > index; i--)
        vector->array[i] = vector->array[i - 1];
    vector->array[index] = value;
    vector->length++;
}

/**
 * @brief Removes the first occurrence of a specific value from the vector.
 * @param vector A pointer to the vector.
 * @param value The value to remove.
 */
void pop(Vector *vector, const int value)
{
    if (isEmpty(vector))
        return;
    for (int i = 0; i < vector->length; i++)
    {
        if (vector->array[i] == value)
        {
            for (int j = i; j < vector->length - 1; j++)
                vector->array[j] = vector->array[j + 1];
            vector->length--;
            return;
        }
    }
}

/**
 * @brief Removes and returns the element at a specific index.
 * @param vector A pointer to the vector.
 * @param index The index of the element to discard.
 * @return The discarded value, or -1 if the index is invalid.
 */
int discard(Vector *vector, const int index)
{
    if (isEmpty(vector) || index < 0 || index >= vector->length)
        return -1;
    int value = vector->array[index];
    for (int i = index; i < vector->length - 1; i++)
        vector->array[i] = vector->array[i + 1];
    vector->length--;
    return value;
}

/**
 * @brief Creates a new vector populated with elements from a given C-style array.
 * @param array A pointer to the source array.
 * @param length The number of elements in the source array.
 * @return A new Vector instance.
 * @note Exits the program if the input array or length is invalid.
 */
Vector populate(const int *array, const int length)
{
    if (array == NULL || length <= 0)
    {
        perror("Invalid input for populate");
        exit(EXIT_FAILURE);
    }
    Vector result = init(length);
    for (int i = 0; i < length; i++)
        append(&result, array[i]);
    return result;
}

/**
 * @brief Fills a vector with a specified number of a single value.
 * @param vector A pointer to the vector.
 * @param quantity The number of elements to add.
 * @param value The value to fill the vector with.
 * @note Clears the vector before adding new elements.
 */
void fill(Vector *vector, const int quantity, const int value)
{
    if (vector == NULL || quantity < 0)
        return;
    clear(vector);
    for (int i = 0; i < quantity; i++)
        append(vector, value);
}

/**
 * @brief Seeds the random number generator.
 * @note This is a private helper function to ensure `srand` is called only once.
 */
void __seeding__()
{
    static int seeded = 0;
    if (!seeded)
    {
        srand((unsigned int)time(NULL));
        seeded = 1;
    }
}

/**
 * @brief Populates a vector with a specified number of random integers within a range.
 * @param vector A pointer to the vector.
 * @param quantity The number of random elements to generate.
 * @param min The minimum value (inclusive).
 * @param max The maximum value (exclusive).
 * @note Clears the vector before adding new elements.
 */
void random(Vector *vector, const int quantity, const int min, const int max)
{
    if (vector == NULL || quantity < 0 || min >= max)
        return;
    __seeding__();
    clear(vector);
    for (int i = 0; i < quantity; i++)
        append(vector, rand() % (max - min) + min);
}

/**
 * @brief Swaps the values of two integer pointers.
 * @param a A pointer to the first integer.
 * @param b A pointer to the second integer.
 * @note This is a private helper function.
 */
void __swap__(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

/**
 * @brief Reverses the order of elements in a vector.
 * @param vector A pointer to the vector to reverse.
 */
void reverse(Vector *vector)
{
    if (vector == NULL || vector->length <= 1)
        return;
    for (int i = 0; i < vector->length / 2; i++)
        __swap__(&vector->array[i], &vector->array[vector->length - i - 1]);
}

/**
 * @brief Shuffles the elements of a vector using the Fisher-Yates algorithm.
 * @param vector A pointer to the vector to shuffle.
 */
void shuffle(Vector *vector)
{
    if (vector == NULL || vector->length <= 1)
        return;
    __seeding__();
    for (int i = vector->length - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        __swap__(&vector->array[i], &vector->array[j]);
    }
}

/**
 * @brief Performs a linear search for a value starting from a given index.
 * @param vector A pointer to the vector to search.
 * @param value The value to search for.
 * @param index The starting index for the search.
 * @return The index of the first occurrence of the value, or -1 if not found.
 */
int linearSearch(const Vector *vector, const int value, const int index)
{
    if (vector == NULL || index < 0 || index >= vector->length)
        return -1;

    for (int i = index; i < vector->length; i++)
    {
        if (vector->array[i] == value)
            return i;
    }
    return -1;
}

/**
 * @brief Performs a binary search for a value in a sorted vector starting from a given index.
 * @param vector A pointer to the vector to search.
 * @param value The value to search for.
 * @param index The starting index for the search.
 * @return The index of the value, or -1 if not found.
 */
int binarySearch(const Vector *vector, const int value, const int index)
{
    if (vector == NULL || index < 0 || index >= vector->length)
        return -1;

    int start = index;
    int end = vector->length - 1;
    while (start <= end)
    {
        int mid = start + (end - start) / 2;
        if (vector->array[mid] == value)
            return mid;
        else if (vector->array[mid] < value)
            start = mid + 1;
        else
            end = mid - 1;
    }
    return -1;
}

/**
 * @brief Performs a ternary search for a value in a sorted vector.
 * @param vector A pointer to the vector to search.
 * @param value The value to search for.
 * @param index The starting index for the search.
 * @return The index of the value, or -1 if not found.
 */
int ternarySearch(const Vector *vector, const int value, const int index)
{
    if (vector == NULL || index < 0 || index >= vector->length)
        return -1;

    int start = index;
    int end = vector->length - 1;
    while (start <= end)
    {
        int mid1 = start + (end - start) / 3;
        int mid2 = end - (end - start) / 3;

        if (vector->array[mid1] == value)
            return mid1;
        if (vector->array[mid2] == value)
            return mid2;

        if (value < vector->array[mid1])
            end = mid1 - 1;
        else if (value > vector->array[mid2])
            start = mid2 + 1;
        else
        {
            start = mid1 + 1;
            end = mid2 - 1;
        }
    }
    return -1;
}

/**
 * @brief Performs an interpolation search for a value in a sorted, uniformly distributed vector.
 * @param vector A pointer to the vector to search.
 * @param value The value to search for.
 * @param index The starting index for the search.
 * @return The index of the value, or -1 if not found.
 */
int interpolationSearch(const Vector *vector, const int value, const int index)
{
    if (vector == NULL || index < 0 || index >= vector->length)
        return -1;

    int start = index;
    int end = vector->length - 1;
    while (start <= end && value >= vector->array[start] && value <= vector->array[end])
    {
        if (vector->array[start] == vector->array[end])
            break;

        int pos = start + (int)(((double)(end - start) * (value - vector->array[start])) / (vector->array[end] - vector->array[start]));

        if (pos < start || pos > end)
            break;

        if (vector->array[pos] == value)
            return pos;
        else if (vector->array[pos] < value)
            start = pos + 1;
        else
            end = pos - 1;
    }
    if (start <= end && vector->array[start] == value)
        return start;
    return -1;
}

/**
 * @brief Performs a jump search for a value in a sorted vector.
 * @param vector A pointer to the vector to search.
 * @param value The value to search for.
 * @param index The starting index for the search.
 * @return The index of the value, or -1 if not found.
 */
int jumpSearch(const Vector *vector, const int value, const int index)
{
    if (vector == NULL || index < 0 || index >= vector->length)
        return -1;

    int jump = (int)sqrt(vector->length);
    int start = index;
    int end = start + jump;

    while (end < vector->length && vector->array[end] <= value)
    {
        start = end;
        end += jump;
    }

    if (end > vector->length)
        end = vector->length;

    for (int i = start; i < end; i++)
        if (vector->array[i] == value)
            return i;
    return -1;
}

/**
 * @brief Sorts the vector using the bubble sort algorithm.
 * @param vector A pointer to the vector to sort.
 */
void bubbleSort(Vector *vector)
{
    if (vector == NULL || vector->length <= 1)
        return;
    for (int i = 1; i < vector->length; i++)
    {
        bool isSorted = true;
        for (int j = 0; j < vector->length - i; j++)
        {
            if (vector->array[j] > vector->array[j + 1])
            {
                __swap__(&vector->array[j], &vector->array[j + 1]);
                isSorted = false;
            }
        }
        if (isSorted)
            break;
    }
}

/**
 * @brief Sorts the vector using the selection sort algorithm.
 * @param vector A pointer to the vector to sort.
 */
void selectionSort(Vector *vector)
{
    if (vector == NULL || vector->length <= 1)
        return;
    for (int i = 0; i < vector->length - 1; i++)
    {
        int minIndex = i;
        for (int j = i + 1; j < vector->length; j++)
        {
            if (vector->array[j] < vector->array[minIndex])
                minIndex = j;
        }
        if (minIndex != i)
            __swap__(&vector->array[i], &vector->array[minIndex]);
    }
}

/**
 * @brief Sorts the vector using the insertion sort algorithm.
 * @param vector A pointer to the vector to sort.
 */
void insertionSort(Vector *vector)
{
    if (vector == NULL || vector->length <= 1)
        return;
    for (int i = 1; i < vector->length; i++)
    {
        int curr = vector->array[i];
        int j = i - 1;
        while (j >= 0 && vector->array[j] > curr)
        {
            vector->array[j + 1] = vector->array[j];
            j--;
        }
        vector->array[j + 1] = curr;
    }
}

/**
 * @brief Sorts the vector using the counting sort algorithm.
 * @param vector A pointer to the vector to sort.
 * @note This algorithm is stable and efficient for a small range of values.
 */
void countSort(Vector *vector)
{
    if (vector == NULL || vector->length <= 1)
        return;

    int min = vector->array[0];
    int max = vector->array[0];
    for (int i = 1; i < vector->length; i++)
    {
        if (vector->array[i] < min)
            min = vector->array[i];
        if (vector->array[i] > max)
            max = vector->array[i];
    }

    int range = max - min + 1;

    int *counter = (int *)calloc(range, sizeof(int));
    if (counter == NULL)
    {
        perror("Failed to allocate memory for Counter Array in countSort");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < vector->length; i++)
        counter[vector->array[i] - min]++;

    for (int i = 1; i < range; i++)
        counter[i] += counter[i - 1];

    int *result = (int *)malloc(vector->length * sizeof(int));
    if (result == NULL)
    {
        perror("Failed to allocate memory for Result Array in countSort");
        free(counter);
        exit(EXIT_FAILURE);
    }

    for (int i = vector->length - 1; i >= 0; i--)
    {
        int val = vector->array[i];
        result[--counter[val - min]] = val;
    }

    for (int i = 0; i < vector->length; i++)
        vector->array[i] = result[i];

    free(counter);
    free(result);
}

/**
 * @brief Sorts the vector using the radix sort algorithm.
 * @param vector A pointer to the vector to sort.
 * @note This algorithm is stable and efficient for large numbers with few digits.
 */
void radixSort(Vector *vector)
{
    if (vector == NULL || vector->length <= 1)
        return;

    int max = vector->array[0];
    for (int i = 1; i < vector->length; i++)
        if (vector->array[i] > max)
            max = vector->array[i];

    for (int place = 1; max / place > 0; place *= 10)
    {
        int counter[10] = {0};

        for (int i = 0; i < vector->length; i++)
            counter[(vector->array[i] / place) % 10]++;

        for (int i = 1; i < 10; i++)
            counter[i] += counter[i - 1];

        int *result = (int *)malloc(vector->length * sizeof(int));
        if (result == NULL)
        {
            perror("Failed to allocate memory for Result Array in radixSort");
            exit(EXIT_FAILURE);
        }

        for (int i = vector->length - 1; i >= 0; i--)
        {
            int digit = (vector->array[i] / place) % 10;
            result[--counter[digit]] = vector->array[i];
        }

        for (int i = 0; i < vector->length; i++)
            vector->array[i] = result[i];

        free(result);
    }
}

/**
 * @brief Merges two sorted vectors into a single sorted vector.
 * @param vector A pointer to the destination vector.
 * @param left A pointer to the first sorted vector (subarray).
 * @param right A pointer to the second sorted vector (subarray).
 * @note This is a private helper function for mergeSort.
 */
void __merge__(Vector *vector, const Vector *left, const Vector *right)
{
    int l = 0, r = 0, i = 0;

    while (l < left->length && r < right->length)
    {
        if (left->array[l] <= right->array[r])
            vector->array[i++] = left->array[l++];
        else
            vector->array[i++] = right->array[r++];
    }

    while (l < left->length)
        vector->array[i++] = left->array[l++];
    while (r < right->length)
        vector->array[i++] = right->array[r++];
}

/**
 * @brief Sorts the vector using the merge sort algorithm.
 * @param vector A pointer to the vector to sort.
 * @note This is a recursive, top-down implementation.
 */
void mergeSort(Vector *vector)
{
    if (vector == NULL || vector->length <= 1)
        return;

    int mid = vector->length / 2;
    Vector left = slice(vector, 0, mid);
    Vector right = slice(vector, mid, vector->length);

    mergeSort(&left);
    mergeSort(&right);

    __merge__(vector, &left, &right);

    destroy(&left);
    destroy(&right);
}

/**
 * @brief Partitions the vector for the quick sort algorithm.
 * @param vector A pointer to the vector.
 * @param start The starting index of the partition.
 * @param end The ending index of the partition.
 * @note This is a private helper function.
 */
void __partition__(Vector *vector, const int start, const int end)
{
    if (end - start <= 0)
        return;

    int pivot = vector->array[end];
    int i = start - 1;

    for (int j = start; j <= end; j++)
        if (vector->array[j] <= pivot)
            __swap__(&vector->array[++i], &vector->array[j]);

    __partition__(vector, start, i - 1);
    __partition__(vector, i + 1, end);
}

/**
 * @brief Sorts the vector using the quick sort algorithm.
 * @param vector A pointer to the vector to sort.
 */
void quickSort(Vector *const vector)
{
    if (vector == NULL || vector->length <= 1)
        return;
    __partition__(vector, 0, vector->length - 1);
}

/**
 * @brief Helper function to build a max-heap.
 * @param vector A pointer to the vector.
 * @param size The current size of the heap.
 * @param index The root of the subtree to heapify.
 * @note This is a private helper function for heapSort.
 */
void __heapify__(Vector *vector, const int size, const int index)
{
    int largest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (left < size && vector->array[left] > vector->array[largest])
        largest = left;

    if (right < size && vector->array[right] > vector->array[largest])
        largest = right;

    if (largest != index)
    {
        __swap__(&vector->array[index], &vector->array[largest]);
        __heapify__(vector, size, largest);
    }
}

/**
 * @brief Sorts the vector using the heap sort algorithm.
 * @param vector A pointer to the vector to sort.
 */
void heapSort(Vector *vector)
{
    if (vector == NULL || vector->length <= 1)
        return;

    for (int i = vector->length / 2 - 1; i >= 0; i--)
        __heapify__(vector, vector->length, i);

    for (int i = vector->length - 1; i > 0; i--)
    {
        __swap__(&vector->array[0], &vector->array[i]);
        __heapify__(vector, i, 0);
    }
}

/**
 * @brief Applies a function to each element of the vector.
 * @param vector A pointer to the vector.
 * @param func A function pointer that takes an integer and returns an integer.
 */
void map(Vector *vector, int (*func)(int))
{
    for (int i = 0; i < vector->length; i++)
        vector->array[i] = func(vector->array[i]);
}

/**
 * @brief Filters a vector in-place, keeping only elements for which the function returns true.
 * @param vector A pointer to the vector.
 * @param func A function pointer that takes an integer and returns a boolean.
 */
void filter(Vector *vector, bool (*func)(int))
{
    int index = 0;
    for (int i = 0; i < vector->length; i++)
        if (func(vector->array[i]))
            vector->array[index++] = vector->array[i];
    vector->length = index;
}

/**
 * @brief Checks if any element in the vector is non-zero (true).
 * @param vector A pointer to the vector.
 * @return true if any element is non-zero, false otherwise.
 */
bool any(const Vector *vector)
{
    if (isEmpty(vector))
        return false;
    for (int i = 0; i < vector->length; i++)
        if (vector->array[i])
            return true;
    return false;
}

/**
 * @brief Checks if all elements in the vector are non-zero (true).
 * @param vector A pointer to the vector.
 * @return true if all elements are non-zero, false otherwise.
 */
bool all(const Vector *vector)
{
    if (isEmpty(vector))
        return true;
    for (int i = 0; i < vector->length; i++)
        if (!vector->array[i])
            return false;
    return true;
}

/**
 * @brief Finds the maximum value in the vector.
 * @param vector A pointer to the vector.
 * @return The maximum value, or -1 if the vector is empty.
 */
int max(const Vector *vector)
{
    if (isEmpty(vector))
        return -1;
    int maximum = vector->array[0];
    for (int i = 1; i < vector->length; i++)
        if (maximum < vector->array[i])
            maximum = vector->array[i];
    return maximum;
}

/**
 * @brief Finds the minimum value in the vector.
 * @param vector A pointer to the vector.
 * @return The minimum value, or -1 if the vector is empty.
 */
int min(const Vector *vector)
{
    if (isEmpty(vector))
        return -1;
    int minimum = vector->array[0];
    for (int i = 1; i < vector->length; i++)
        if (minimum > vector->array[i])
            minimum = vector->array[i];
    return minimum;
}

/**
 * @brief Calculates the sum of all elements in the vector.
 * @param vector A pointer to the vector.
 * @return The sum of the elements, or 0 if the vector is empty.
 */
int sum(const Vector *vector)
{
    if (isEmpty(vector))
        return 0;
    int total = 0;
    for (int i = 0; i < vector->length; i++)
        total += vector->array[i];
    return total;
}

/**
 * @brief Calculates the product of all elements in the vector.
 * @param vector A pointer to the vector.
 * @return The product of the elements, or 1 if the vector is empty.
 */
int prod(const Vector *vector)
{
    if (isEmpty(vector))
        return 1;
    int product = 1;
    for (int i = 0; i < vector->length; i++)
        product *= vector->array[i];
    return product;
}

/**
 * @brief Counts the occurrences of a specific value in the vector.
 * @param vector A pointer to the vector.
 * @param value The value to count.
 * @return The number of times the value appears in the vector.
 */
int count(const Vector *vector, const int value)
{
    if (isEmpty(vector))
        return 0;
    int freq = 0;
    for (int i = 0; i < vector->length; i++)
        if (vector->array[i] == value)
            freq++;
    return freq;
}

/**
 * @brief Checks if a specific value exists in the vector.
 * @param vector A pointer to the vector.
 * @param value The value to search for.
 * @return true if the value is found, false otherwise.
 */
bool contains(const Vector *vector, const int value)
{
    if (isEmpty(vector))
        return false;
    for (int i = 0; i < vector->length; i++)
        if (vector->array[i] == value)
            return true;
    return false;
}

#endif // VECTOR_H