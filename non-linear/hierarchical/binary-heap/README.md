# Heap: A Fixed-Capacity Binary Heap ADT for C

## Overview

Binary Heap is a versatile and efficient implementation of a heap data structure as an Abstract Data Type (ADT) in C. It serves as the foundation for priority queues and other heap-based algorithms. This ADT allows for the creation of both Min-Heaps and Max-Heaps, providing a flexible and robust solution for managing prioritized data. The implementation is in pure C, making it lightweight and easy to integrate into a variety of projects.

## Table of Contents

- [Features](#features)
- [Function Overview](#function-overview)
  - [Core Management](#core-management)
  - [Element Manipulation](#element-manipulation)
  - [Utility Functions](#utility-functions)
- [How to Compile and Run](#how-to-compile-and-run)
- [Limitations](#limitations)
- [License](#license)
- [Author](#author)

## Features

- **Min-Heap and Max-Heap Support:** A single ADT that can be configured to function as either a Min-Heap (smallest element at the root) or a Max-Heap (largest element at the root).
- **Dynamic Sizing:** Implemented using a dynamic array, allowing the heap to grow up to its allocated capacity.
- **Core Operations:** Includes essential heap operations such as initialization, insertion, extraction of the root element, and searching.
- **Heap Property Validation:** Provides functions to check if the heap currently satisfies the Min-Heap or Max-Heap properties.
- **Memory Management:** Explicitly handles memory allocation and deallocation, preventing memory leaks.
- **Clear Documentation:** Each function is documented with a clear description, parameters, and return values, enhancing readability and ease of use.

---

## Function Overview

The `adt_Heap.h` header file exposes the following public API functions:

### Core Management

- `Heap init(int capacity, Type type)`: Initializes a new `Heap` with a specified capacity and type (MIN_HEAP or MAX_HEAP).
- `Heap copy(const Heap *heap)`: Creates a deep copy of an existing heap.
- `void clear(Heap *heap)`: Clears the contents of a heap, resetting its length to zero.
- `void destroy(Heap *heap)`: Frees the memory allocated for the heap's array.

### Element Manipulation

- `int insert(Heap *heap, int value)`: Inserts a new value into the heap while maintaining the heap property.
- `int discard(Heap *heap, int value)`: Removes a specific value from the heap.
- `int peek(const Heap *heap)`: Retrieves the top element of the heap (the root) without removing it.
- `int extract(Heap *heap)`: Extracts and removes the top element from the heap.

### Utility Functions

- `bool isEmpty(const Heap *heap)`: Checks if the heap is empty.
- `bool isFull(const Heap *heap)`: Checks if the heap is full.
- `int search(const Heap *heap, int value)`: Searches for a specific value in the heap.
- `bool isMin(const Heap *heap)`: Verifies if the heap satisfies the min-heap property.
- `bool isMax(const Heap *heap)`: Verifies if the heap satisfies the max-heap property.
- `void traverse(const Heap *heap)`: Prints the contents of the heap level by level.

---

## How to Compile and Run

1.  **Download the Library**

    Ensure you have the `adt_Heap.h` header file and the `test_Heap.c` test file in your project directory.

2.  **Include the Header File**

    In your C program (e.g., `test_Heap.c`), include the header:

    ```c
    #include "adt_Heap.h"
    ```

3.  **Compile the Code**

    Since `adt_Heap.h` is a header-only library, you just need to compile your main application file (e.g., `test_Heap.c`). For example, if you're using GCC, compile your program like this:

    ```bash
    gcc -o test_Heap test_Heap.c
    ```

4.  **Run the Executable**

    After successful compilation, run the program from the terminal:

    ```bash
    ./test_Heap
    ```

5.  **Example Program**

    Here's a simple example demonstrating the use of a Min-Heap:

    ```c
    #include "adt_Heap.h"
    #include <stdio.h>

    int main() {
        Heap minHeap = init(10, MIN_HEAP);

        insert(&minHeap, 5);
        insert(&minHeap, 10);
        insert(&minHeap, 3);
        insert(&minHeap, 8);

        printf("Min-Heap elements (peek): %d\n", peek(&minHeap)); // Should be 3

        printf("Extracting elements:\n");
        while (!isEmpty(&minHeap)) {
            printf("%d ", extract(&minHeap));
        }
        // Output: 3 5 8 10

        printf("\n");
        destroy(&minHeap);
        return 0;
    }
    ```

---

## Limitations

- **Fixed Data Type (Integers Only):** This implementation is specifically designed to store `int` type elements. Modifying it for other data types (e.g., `float`, `char`, or custom structs) would require changes to the `typedef` and `sizeof` operations, as well as the comparison logic within the internal heapify functions.
- **Basic Error Handling:** Error handling for invalid operations (e.g., extracting from an empty heap) is done via `return -1`. Critical memory allocation failures use `perror` and `exit(EXIT_FAILURE)`. A more robust library might use a more sophisticated error reporting mechanism.
- **Single-Purpose:** While heaps are a general data structure, this implementation is tailored to integer values and basic heap properties. It does not include advanced features like `decrease-key` operations for graph algorithms, though a similar `discard` function is provided.

---

## License

This project is licensed under the MIT License. See the `LICENSE` file for details.

---

## Author

- **Name:** Zulqarnain Chishti
- **Email:** thisiszulqarnain@gmail.com
- **LinkedIn:** [Zulqarnain Chishti](https://www.linkedin.com/in/zulqarnain-chishti-6731732a1/)
- **GitHub:** [zulqarnainchishti](https://github.com/zulqarnainchishti)
