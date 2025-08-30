# PriorityQueue: A Fixed-Capacity Priority Queue Abstract Data Type in C

## Overview

Priority Queue is a robust and efficient implementation of a priority queue as an Abstract Data Type (ADT) in C. Implemented using a binary heap, it provides a flexible structure for managing data where each element has an associated priority. This ADT can be configured for both ascending and descending priority, making it a versatile tool for various applications like scheduling tasks, implementing graph algorithms, or handling event-driven simulations. The implementation is in pure C, making it lightweight and easy to integrate.

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

- **Ascending and Descending Priority:** The queue can be configured to prioritize elements based on either ascending (smaller priority value is higher priority) or descending (larger priority value is higher priority) order.
- **Dynamic Sizing:** Implemented using a dynamic array (heap), allowing the queue to grow up to its allocated capacity.
- **Core Operations:** Includes essential priority queue operations such as initialization, enqueueing, dequeueing the highest-priority element, and peeking at the top element.
- **Priority Updates:** Allows for the priority of an existing element to be updated, with the queue automatically re-balancing to maintain the correct order.
- **Memory Management:** Explicitly handles memory allocation and deallocation to prevent memory leaks.
- **Clear Documentation:** Each function is documented with a clear description, parameters, and return values, enhancing readability and ease of use.

---

## Function Overview

The `adt_PriorityQueue.h` header file exposes the following public API functions:

### Core Management

- `PriorityQueue init(int capacity, Type type)`: Initializes a new `PriorityQueue` with a specified capacity and type (ASCENDING or DESCENDING).
- `PriorityQueue copy(const PriorityQueue *queue)`: Creates a deep copy of an existing priority queue.
- `void clear(PriorityQueue *queue)`: Clears the contents of the queue, resetting its length to zero.
- `void destroy(PriorityQueue *queue)`: Frees the memory allocated for the queue's array.

### Element Manipulation

- `int enqueue(PriorityQueue *queue, int value, int weight)`: Adds a new element with a given value and priority to the queue.
- `int update(PriorityQueue *queue, int value, int weight)`: Updates the priority of an existing element in the queue.
- `int peek(const PriorityQueue *queue)`: Retrieves the data of the highest-priority element without removing it.
- `int dequeue(PriorityQueue *queue)`: Removes and returns the data of the highest-priority element.

### Utility Functions

- `bool isEmpty(const PriorityQueue *queue)`: Checks if the priority queue is empty.
- `bool isFull(const PriorityQueue *queue)`: Checks if the priority queue is full.
- `int search(const PriorityQueue *queue, int value)`: Searches for a specific data value in the queue.
- `bool isAsc(const PriorityQueue *queue)`: Verifies if the queue satisfies the ascending priority property.
- `bool isDesc(const PriorityQueue *queue)`: Verifies if the queue satisfies the descending priority property.
- `void traverse(const PriorityQueue queue)`: Prints the contents (data and priority) of the queue level by level.

---

## How to Compile and Run

1.  **Download the Library**

    Ensure you have the `adt_PriorityQueue.h` header file and the `test_PriorityQueue.c` test file in your project directory.

2.  **Include the Header File**

    In your C program (e.g., `test_PriorityQueue.c`), include the header:

    ```c
    #include "adt_PriorityQueue.h"
    ```

3.  **Compile the Code**

    Since `adt_PriorityQueue.h` is a header-only library, you just need to compile your main application file (e.g., `test_PriorityQueue.c`). For example, if you're using GCC, compile your program like this:

    ```bash
    gcc -o test_PriorityQueue test_PriorityQueue.c
    ```

4.  **Run the Executable**

    After successful compilation, run the program from the terminal:

    ```bash
    ./test_PriorityQueue
    ```

5.  **Example Program**

    Here's a simple example demonstrating the use of an ascending priority queue:

    ```c
    #include "adt_PriorityQueue.h"
    #include <stdio.h>

    int main() {
        PriorityQueue ascQueue = init(10, ASCENDING);

        enqueue(&ascQueue, 100, 5);
        enqueue(&ascQueue, 200, 3);
        enqueue(&ascQueue, 300, 8);
        enqueue(&ascQueue, 400, 1);

        printf("Highest priority element (peek): %d (priority %d)\n", peek(&ascQueue), 1); // Should be 400

        printf("Dequeuing elements:\n");
        while (!isEmpty(&ascQueue)) {
            printf("%d ", dequeue(&ascQueue));
        }
        // Output: 400 200 100 300

        printf("\n");
        destroy(&ascQueue);
        return 0;
    }
    ```

---

## Limitations

- **Fixed Data Type (Integers Only):** This implementation stores `int` for both data and priority. To use other data types, you would need to modify the `Tuple` struct and adjust the comparison logic in the internal heapify functions.
- **Basic Error Handling:** Error handling for invalid operations (e.g., dequeuing from an empty queue) is done via `return -1`. Critical memory allocation failures use `perror` and `exit(EXIT_FAILURE)`.
- **Search Efficiency:** The `search` function performs a linear scan (`O(n)` complexity), as the heap structure is optimized for priority-based access, not for finding arbitrary elements.

---

## License

This project is licensed under the MIT License. See the `LICENSE` file for details.

---

## Author

- **Name:** Zulqarnain Chishti
- **Email:** thisiszulqarnain@gmail.com
- **LinkedIn:** [Zulqarnain Chishti](https://www.linkedin.com/in/zulqarnain-chishti-6731732a1/)
- **GitHub:** [zulqarnainchishti](https://www.google.com/search?q=https://github.com/zulqarnainchishti)
