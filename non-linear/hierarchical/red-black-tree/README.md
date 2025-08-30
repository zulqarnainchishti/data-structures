# RedBlackTree: A Red-Black Tree ADT for C

## Overview

RedBlackTree is an efficient, **self-balancing** implementation of a Red-Black Tree as an Abstract Data Type (ADT) in C. Unlike a basic Binary Search Tree (BST), this implementation guarantees logarithmic time complexity for search, insertion, and deletion operations, even in the worst-case scenario. It uses a single **NIL sentinel** to represent all external nodes (leaves), which simplifies the logic for tree rotations and color-balancing. This library provides a complete set of functions to manage the tree, analyze its properties, and perform various traversals. It's built in pure C, making it a robust and lightweight solution for applications requiring ordered data storage with guaranteed performance.

## Table of Contents

- [Features](#features)
- [Function Overview](#function-overview)
  - [Core Operations](#core-operations)
  - [Search Operations](#search-operations)
  - [Tree Properties](#tree-properties)
  - [Traversal Operations](#traversal-operations)
  - [Type Checks](#type-checks)
- [How to Compile and Run](#how-to-compile-and-run)
- [Limitations](#limitations)
- [License](#license)
- [Author](#author)

---

## Features

- **Guaranteed Logarithmic Performance:** As a self-balancing Red-Black Tree, this ADT ensures that all core operations—search, insert, and delete—are performed in $O(\\log n)$ time, where $n$ is the number of nodes. This prevents the degradation to a linear list that can occur with unbalanced BSTs.
- **Robust Node Management:** The implementation provides functions for creating, copying, and destroying the entire tree, ensuring proper memory management. It utilizes a special **NIL sentinel** to represent all leaves, simplifying the handling of tree boundaries and ensuring that all properties are maintained correctly.
- **Comprehensive Search and Traversal:** The library is optimized for fast data retrieval, allowing you to quickly find the **minimum** and **maximum** values, and locate a node's **predecessor** or **successor**. It also supports all standard traversal methods (**in-order, pre-order, post-order, and level-order**), as well as functions to find the **ceiling** or **floor** of a value.
- **Structural Analysis and Classification:** Beyond basic operations, the library includes functions to analyze the tree's structure. You can determine its **size**, **height**, and the **depth** of any given node. The ADT also includes functions to check for properties like **perfection**, **completeness**, **fullness**, and **symmetry**, which are vital for understanding the tree's structure and health.
- **Memory-Safe Design:** The ADT is built with a focus on memory safety. All functions that allocate memory explicitly return pointers that the user must manage, and a dedicated `destroy` function helps prevent memory leaks. This design promotes responsible resource management in C programming.

---

## Function Overview

The `adt_RedBlackTree.h` header file exposes a public API for interacting with the Red-Black Tree ADT.

### Core Operations

- `RedBlackTree *init(const int value)`: Initializes a new Red-Black Tree node.
- `RedBlackTree *destroy(RedBlackTree *root)`: Frees the memory for the entire tree.
- `RedBlackTree *copy(const RedBlackTree *root)`: Creates a deep copy of a tree.
- `RedBlackTree *insert(RedBlackTree *root, const int value)`: Inserts a new value.
- `RedBlackTree *discard(RedBlackTree *root, const int value)`: Deletes a node.

### Search Operations

- `RedBlackTree *search(RedBlackTree *root, const int value)`: Finds a node with a specific value.
- `RedBlackTree *minimum(RedBlackTree *root)`: Finds the node with the minimum value.
- `RedBlackTree *maximum(RedBlackTree *root)`: Finds the node with the maximum value.
- `RedBlackTree *successor(RedBlackTree *root, const int value)`: Finds the in-order successor.
- `RedBlackTree *predecessor(RedBlackTree *root, const int value)`: Finds the in-order predecessor.
- `RedBlackTree *ceiling(RedBlackTree *root, const double value)`: Finds the smallest value greater than or equal to the given value.
- `RedBlackTree *floored(RedBlackTree *root, const double value)`: Finds the largest value less than or equal to the given value.

### Tree Properties

- `int degree(const RedBlackTree *root)`: Calculates the number of children for a node.
- `int size(const RedBlackTree *root)`: Counts the total number of nodes.
- `int external(const RedBlackTree *root)`: Counts the number of external (leaf) nodes.
- `int internal(const RedBlackTree *root)`: Counts the number of internal nodes.
- `int height(const RedBlackTree *root)`: Calculates the height of the tree.
- `int depth(const RedBlackTree *root, const int value)`: Calculates the depth of a specific node.

### Traversal Operations

- `void preorder(const RedBlackTree *root)`: Prints nodes in a **Root → Left → Right** order.
- `void inorder(const RedBlackTree *root)`: Prints nodes in a **Left → Root → Right** order (sorted).
- `void postorder(const RedBlackTree *root)`: Prints nodes in a **Left → Right → Root** order.
- `void levelorder(const RedBlackTree *root)`: Prints nodes level by level (**Breadth-First Search**).

### Type Checks

- `bool isPerfect(const RedBlackTree *root)`: Checks for a perfect binary tree.
- `bool isComplete(RedBlackTree *root)`: Checks for a complete binary tree.
- `bool isFull(const RedBlackTree *root)`: Checks for a full binary tree.
- `bool isSymmetric(const RedBlackTree *root)`: Checks if the tree is symmetric.

---

## How to Compile and Run

1.  **Download the Library**

    Make sure you have the `adt_RedBlackTree.h` header file and the `test_RedBlackTree.c` test file in your project directory.

2.  **Include the Header File**

    In your C program (e.g., `test_RedBlackTree.c`), include the header file at the top:

    ```c
    #include "adt_RedBlackTree.h"
    ```

3.  **Compile the Code**

    Since `adt_RedBlackTree.h` is a header-only library, you just need to compile your main application file (e.g., `test_RedBlackTree.c`). For example, using GCC:

    ```bash
    gcc -o test_RedBlackTree test_RedBlackTree.c
    ```

4.  **Run the Executable**

    Execute the compiled program from your terminal:

    ```bash
    ./test_RedBlackTree
    ```

5.  **Example Program**

    Here's a simple example demonstrating how to build and traverse a Red-Black Tree:

    ```c
    #include "adt_RedBlackTree.h"
    #include <stdio.h>

    int main() {
        RedBlackTree *root = NULL;

        root = insert(root, 50);
        root = insert(root, 30);
        root = insert(root, 20);
        root = insert(root, 40);
        root = insert(root, 70);
        root = insert(root, 60);
        root = insert(root, 80);

        printf("In-order traversal: ");
        inorder(root);
        printf("\n");

        printf("Height of the tree: %d\n", height(root));

        root = discard(root, 20); // Discard a node
        root = discard(root, 70); // Discard with one child
        root = discard(root, 50); // Discard the root

        printf("In-order traversal after discard: ");
        inorder(root);
        printf("\n");

        destroy(root);
        return 0;
    }
    ```

---

## Limitations

- **Fixed Data Type (Integers Only):** The current implementation is designed to store integer data. To handle other data types, the `data` field in the `RedBlackTree` struct and comparison logic within the functions would need to be modified.
- **Basic Error Handling:** Error handling is limited. For example, `init` returns `NULL` on memory allocation failure, which the caller is responsible for checking.

---

## License

This project is licensed under the MIT License. See the `LICENSE` file for details.

---

## Author

- **Name:** Zulqarnain Chishti
- **Email:** thisiszulqarnain@gmail.com
- **LinkedIn:** [Zulqarnain Chishti](https://www.linkedin.com/in/zulqarnain-chishti-6731732a1/)
- **GitHub:** [zulqarnainchishti](https://www.google.com/search?q=https://github.com/zulqarnainchishti)
