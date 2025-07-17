# C-ArrayList: A Dynamic Array Abstract Data Type in C

## Overview

C-ArrayList is a robust and feature-rich implementation of a dynamic array (often called an ArrayList in other languages) as an Abstract Data Type (ADT) in C. Designed for educational clarity and practical utility, it provides a flexible, resizable array structure capable of storing integer elements. This project serves as a comprehensive example of data structure implementation in pure C, covering fundamental array operations, various search and sort algorithms, and functional programming paradigms.

## Table of Contents

- [Features](#features)
- [Function Overview](#function-overview)
  - [Core Management](#core-management)
  - [Element Manipulation](#element-manipulation)
  - [Data Transformation & Utilities](#data-transformation--utilities)
  - [Search Algorithms](#search-algorithms)
  - [Sorting Algorithms](#sorting-algorithms)
  - [Functional Programming Inspired](#functional-programming-inspired)
- [How to Compile and Run](#how-to-compile-and-run)
- [Limitations](#limitation)
- [License](#license)
- [Author](#author)

## Features

- **Dynamic Sizing:** Automatically expands its capacity as more elements are added.
- **Core Operations:** Includes essential array manipulations like initialization, appending, inserting, retrieving, setting, and removing elements.
- **Search Algorithms:** Implements various search techniques for both sorted and unsorted data.
- **Sorting Algorithms:** Provides implementations of common sorting algorithms, allowing for comparison and analysis.
- **Functional Helpers:** Incorporates functions inspired by functional programming (map, filter, any, all) for convenient data transformation and analysis.
- **Memory Management:** Explicitly handles memory allocation and deallocation to prevent leaks.
- **Header-Only Library:** Easy to integrate into other C projects by simply including the header file.

## Functions Overview

The `ArrayList.h` header file exposes the following public API functions:

### Core Management

- `ArrayList init(const int capacity)`: Initializes a new `ArrayList` with a specified initial capacity.
- `void destroy(ArrayList *list)`: Deallocates all memory associated with the `ArrayList`.
- `void clear(ArrayList *list)`: Resets the `ArrayList`'s length to 0, but retains its allocated capacity.
- `bool isEmpty(const ArrayList list)`: Checks if the `ArrayList` contains no elements.
- `bool isFull(const ArrayList list)`: Checks if the `ArrayList` has reached its current allocated capacity.
- `void traverse(const ArrayList list)`: Prints the elements, length, and capacity of the `ArrayList`.

### Element Manipulation

- `void append(ArrayList *list, int value)`: Adds an element to the end of the `ArrayList`.
- `void insert(ArrayList *list, int value, int index)`: Inserts an element at a specific index, shifting subsequent elements.
- `int pop(ArrayList *list)`: Removes and returns the last element.
- `int discard(ArrayList *list, int index)`: Removes and returns the element at a specific index.
- `int get(ArrayList list, int index)`: Retrieves the element at a specific index.
- `void set(ArrayList *list, int value, int index)`: Sets the element at a specific index to a new value.
- `void replace(ArrayList *list, int new, int old, int index)`: Replaces the first occurrence of `old` with `new`, starting the search from `index`.

### Data Transformation & Utilities

- `ArrayList copy(const ArrayList list)`: Creates a deep copy of the `ArrayList`.
- `ArrayList slice(ArrayList list, int start, int end)`: Creates a new `ArrayList` containing a portion (slice) of the original.
- `ArrayList join(const ArrayList front, const ArrayList rear)`: Concatenates two `ArrayLists` into a new one.
- `void reverse(ArrayList *list)`: Reverses the order of elements in-place.
- `void fill(ArrayList *list, const int quantity, const int value)`: Fills the list with a specified quantity of a given value.
- `void randomize(ArrayList *list, const int quantity, const int min, const int max)`: Fills the list with random integers within a specified range.
- `void shuffle(ArrayList *list)`: Randomly shuffles the elements in the `ArrayList` using Fisher-Yates algorithm.

### Search Algorithms

- `int linearSearch(ArrayList list, int value, int index)`: Finds an element by iterating sequentially from a starting index.
- `int binarySearch(ArrayList list, int value, int index)`: Efficiently finds an element in a sorted list by repeatedly dividing the search interval in half.
- `int ternarySearch(ArrayList list, int value, int index)`: Similar to binary search but divides the array into three parts.
- `int interpolationSearch(ArrayList list, int value, int index)`: An improvement over binary search for uniformly distributed sorted data.
- `int jumpSearch(ArrayList list, int value, int index)`: Reduces search time by jumping through fixed-size blocks.

### Sorting Algorithms

- `void bubbleSort(ArrayList *list)`: Simple comparison-based sort, repeatedly steps through the list.
- `void selectionSort(ArrayList *list)`: Finds the minimum element from the unsorted part and puts it at the beginning.
- `void insertionSort(ArrayList *list)`: Builds the final sorted array one item at a time.
- `void countingSort(ArrayList *list)`: Non-comparison sort, effective for integer data within a limited range.
- `void mergeSort(ArrayList *list)`: Divide and conquer algorithm, recursively divides and merges sorted halves.
- `void quickSort(ArrayList *list)`: Divide and conquer algorithm, picks a pivot and partitions the array around it.
- `void heapSort(ArrayList *list)`: Builds a max-heap and repeatedly extracts the maximum element.

### Functional Programming Inspired

- `void map(ArrayList *list, int (*func)(int))`: Applies a function to each element, transforming it in-place.
- `void filter(ArrayList *list, bool (*func)(int))`: Retains only elements for which a predicate function returns true.
- `bool any(ArrayList list)`: Returns true if at least one element is non-zero (truthy).
- `bool all(ArrayList list)`: Returns true if all elements are non-zero (truthy).
- `int max(ArrayList list)`: Returns the maximum value in the list.
- `int min(ArrayList list)`: Returns the minimum value in the list.
- `int sum(ArrayList list)`: Calculates the sum of all elements.
- `int prod(ArrayList list)`: Calculates the product of all elements.
- `int count(ArrayList list, int value)`: Counts occurrences of a specific value.
- `bool contains(ArrayList list, int value)`: Checks if the list contains a specific value.

## How to Compile and Run

1.  **Download the Library**

    To begin using the library, ensure you have the `ArrayList.h` header file and the `test_arraylist.c` test file in your project directory.

    _Note: `ArrayList.h` is a header-only library, meaning all function implementations are directly in the header._

2.  **Include the Header File**

    In your C program (e.g., `my_program.c` or `test_arraylist.c`), include the header:

    ```c
    #include "ArrayList.h"
    ```

3.  **Compile the Code**

    Since `ArrayList.h` is a header-only library, you just need to compile your main application file (e.g., `test_arraylist.c`) and link against the math library. For example, if you're using GCC, compile your program like this:

    ```bash
    gcc -o test_arraylist test_arraylist.c -lm
    ```

    _This command will compile your source file (`test_arraylist.c`) and link it with the necessary math functions to produce the final executable (`test_arraylist`)._

4.  **Run the Executable**

    After successful compilation, run the program from the terminal:

    ```bash
    ./test_arraylist
    ```

5.  **Example Program**

    Here's an example that demonstrates initialization, mutation, traversal, replacement, sorting, and destruction:

    ```c
    #include "ArrayList.h"
    #include <stdio.h>

    int main() {
        ArrayList myList = init(5);

        append(&myList, 10);
        append(&myList, 20);
        append(&myList, 5);
        append(&myList, 20);  // duplicate

        printf("Original List: ");
        traverse(myList);    // [ 10  20   5  20 ] : 4/5

        replace(&myList, 25, 20, 0);  // Replace first 20 with 25
        printf("After replacing first 20 with 25: ");
        traverse(myList);    // [ 10  25   5  20 ] : 4/5

        bubbleSort(&myList);
        printf("Sorted List: ");
        traverse(myList);    // [  5  10  20  25 ] : 4/5

        destroy(&myList);
        return 0;
    }
    ```

## Limitations

- **Fixed Data Type (Integers Only):** This implementation is specifically designed to store `int` type elements. Modifying it to store other data types (e.g., `float`, `char`, or custom structs) would require changing the `typedef` and `sizeof` operations throughout the code.
- **Basic Error Handling:** Error handling is primarily done via `return -1` for `get`/`pop`/`discard` on invalid operations, or `perror` and `exit(EXIT_FAILURE)` for critical memory allocation failures. A more robust production-grade library might use custom error codes, `errno`, or pass error information back to the caller for more flexible handling.
- **No Iterator Support:** The ADT does not expose explicit iterator mechanisms like those found in C++ STL containers. Traversal and manipulation are done via direct index access or the provided helper functions.
- **Complexity Trade-offs:** While various algorithms are provided, the implementation prioritizes clarity and directness, not necessarily the absolute most optimized version of each algorithm (e.g., in-place merge sort is more complex than one using auxiliary arrays).

## License

This project is licensed under the MIT License. See the `LICENSE` file for details.

## Author

- **Name:** Zulqarnain Chishti
- **Email:** thisiszulqarnain@gmail.com
- **LinkedIn:** [Zulqarnain Chishti](https://www.linkedin.com/in/zulqarnain-chishti-6731732a1/)
- **GitHub:** [zulqarnainchishti](https://github.com/zulqarnainchishti)
