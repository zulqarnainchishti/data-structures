# Trie: A Prefix Tree ADT in C

## Overview

The Trie, or Prefix Tree, is an efficient and flexible data structure implemented as an Abstract Data Type (ADT) in pure C. It is designed to store and retrieve a dynamic set of strings, where keys are typically words. Unlike hash tables, which can have collisions, a trie's search time depends on the length of the string, making it an ideal choice for applications like autocompletion, spell checkers, and IP routing. The implementation is lightweight, header-only, and provides a robust set of operations for managing a dictionary of words.

---

## Table of Contents

- [Features](#features)
- [Function Overview](#function-overview)
  - [Core Management](#core-management)
  - [Word Operations](#word-operations)
  - [Utility Functions](#utility-functions)
- [How to Compile and Run](#how-to-compile-and-run)
- [Limitations](#limitations)
- [License](#license)
- [Author](#author)

---

## Features

- **Efficient String Operations**: Provides fast insertion, deletion, and searching of words with a time complexity proportional to the length of the word ($O(L)$), not the number of words.
- **Prefix-Based Search**: The structure is inherently optimized for prefix-based searches, enabling functionalities like autocompletion and spell-checking.
- **Memory Management**: Explicitly handles dynamic memory allocation and deallocation for each node, ensuring efficient memory usage and preventing leaks.
- **Case-Insensitive Operations**: All string operations (`insert`, `search`, `discard`) are case-insensitive, handling both uppercase and lowercase English letters seamlessly.
- **Header-Only Design**: The entire ADT is contained within a single header file, simplifying integration into C projects.
- **Clear Documentation**: Each function is well-documented with a clear description, parameters, and return values.

---

## Function Overview

The `adt_Trie.h` header file exposes the following public API functions for managing a trie data structure:

### Core Management

- `Trie *init()`: Initializes a new, empty trie node. This function is typically used to create the root of the trie.
- `Trie *copy(const Trie *root)`: Creates a **deep copy** of an entire trie, including all its nodes and connections.
- `void destroy(Trie *root)`: Recursively frees all memory allocated for the trie, starting from the root.

### Word Operations

- `Trie *insert(Trie *root, const char *word)`: Inserts a word into the trie. The function traverses the trie, creating new nodes as needed.
- `Trie *discard(Trie *root, const char *word)`: Removes a word from the trie. It frees nodes that are no longer part of any other words.
- `bool search(const Trie *root, const char *word)`: Checks if a complete word exists in the trie.
- `bool startsWith(const Trie *root, const char *prefix)`: Checks if any word in the trie begins with a given prefix.

### Utility Functions

- `int countAll(const Trie *root)`: Counts the total number of words stored in the trie.
- `int countWithPrefix(const Trie *root, const char *prefix)`: Counts how many words in the trie start with a specific prefix.
- `void displayAll(const Trie *root)`: Prints all words in the trie to the console.
- `void displayWithPrefix(const Trie *root, const char *prefix)`: Prints all words that begin with a given prefix.

---

## How to Compile and Run

1.  **Download the Library**

    Ensure you have the `adt_Trie.h` header file and the `test_Trie.c` test file in your project directory.

2.  **Include the Header File**

    In your C program (e.g., `test_Trie.c`), include the header file:

    ```c
    #include "adt_Trie.h"
    ```

3.  **Compile the Code**

    Since `adt_Trie.h` is a header-only ADT, you can compile your main file (e.g., `test_Trie.c`) directly. For example, using GCC:

    ```bash
    gcc -o test_Trie test_Trie.c
    ```

4.  **Run the Executable**

    Execute the compiled program from your terminal:

    ```bash
    ./test_Trie
    ```

5.  **Example Program**

    Here's a simple example demonstrating the core operations of the trie:

    ```c
    #include "adt_Trie.h"
    #include <stdio.h>

    int main() {
        Trie *root = init();

        insert(root, "apple");
        insert(root, "app");
        insert(root, "apricot");

        printf("Is 'apple' in the trie? %s\n", search(root, "apple") ? "Yes" : "No");
        printf("Are there any words with prefix 'app'? %s\n", startsWith(root, "app") ? "Yes" : "No");
        printf("Total words in the trie: %d\n", countAll(root));

        printf("All words with prefix 'ap':\n");
        displayWithPrefix(root, "ap");

        discard(root, "app");
        printf("\nIs 'app' still in the trie? %s\n", search(root, "app") ? "Yes" : "No");
        printf("Total words after discarding 'app': %d\n", countAll(root));

        destroy(root);
        return 0;
    }
    ```

---

## Limitations

- **Fixed Alphabet Size**: The trie is hard-coded for the English alphabet (26 characters). The `ALPHABET_SIZE` macro must be changed to accommodate other character sets or a wider range of characters.
- **Basic Error Handling**: The functions handle `NULL` pointers gracefully to prevent crashes, but more advanced error reporting (e.g., using `errno`) is not included.
- **Case-Insensitive Only**: The current implementation is case-insensitive. To support case-sensitive words, the `__number__` and `__alphabet__` utility functions would need to be modified.
- **Memory Consumption**: Tries can consume more memory than hash tables, especially for sparse datasets, as each node has an array of child pointers.

---

## License

This project is licensed under the MIT License. See the `LICENSE` file for details.

---

## Author

- **Name**: Zulqarnain Chishti
- **Email**: thisiszulqarnain@gmail.com
- **LinkedIn**: [Zulqarnain Chishti](https://www.linkedin.com/in/zulqarnain-chishti-6731732a1/)
- **GitHub**: [zulqarnainchishti](https://www.google.com/search?q=https://github.com/zulqarnainchishti)
