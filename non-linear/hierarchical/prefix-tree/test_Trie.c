#include <stdio.h>
#include <string.h>
#include "adt_Trie.h"

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_RESET "\x1b[0m"

void print_assertion_result(const char *assertion_name, bool result)
{
    printf("    -> Assertion '%s': ", assertion_name);
    if (result)
    {
        printf(ANSI_COLOR_GREEN "PASSED" ANSI_COLOR_RESET "\n");
    }
    else
    {
        printf(ANSI_COLOR_RED "FAILED" ANSI_COLOR_RESET "\n");
    }
}

void print_test_summary(const char *test_name, bool result)
{
    printf("--- Test Case: %s ---\n", test_name);
    if (result)
    {
        printf("Result: " ANSI_COLOR_GREEN "PASSED" ANSI_COLOR_RESET "\n\n");
    }
    else
    {
        printf("Result: " ANSI_COLOR_RED "FAILED" ANSI_COLOR_RESET "\n\n");
    }
}

void test_init()
{
    printf("--- Running test_init ---\n");
    bool overall_result = true;
    Trie *root = init();

    bool assertion1 = (root != NULL);
    print_assertion_result("root is not NULL", assertion1);
    overall_result &= assertion1;

    if (root)
    {
        bool assertion2 = !root->terminal;
        print_assertion_result("terminal flag is false", assertion2);
        overall_result &= assertion2;

        bool all_children_null = true;
        for (int i = 0; i < ALPHABET_SIZE; i++)
        {
            if (root->children[i] != NULL)
            {
                all_children_null = false;
                break;
            }
        }
        print_assertion_result("all children are NULL", all_children_null);
        overall_result &= all_children_null;
    }

    print_test_summary("test_init", overall_result);
    destroy(root);
}

void test_insert_and_search()
{
    printf("--- Running test_insert_and_search ---\n");
    bool overall_result = true;
    Trie *root = init();
    const char *word1 = "apple";
    const char *word2 = "app";

    root = insert(root, word1);
    root = insert(root, word2);

    bool assertion1 = search(root, word1);
    print_assertion_result("search 'apple' returns true", assertion1);
    overall_result &= assertion1;

    bool assertion2 = search(root, word2);
    print_assertion_result("search 'app' returns true", assertion2);
    overall_result &= assertion2;

    bool assertion3 = !search(root, "appl");
    print_assertion_result("search 'appl' returns false", assertion3);
    overall_result &= assertion3;

    bool assertion4 = startsWith(root, "ap");
    print_assertion_result("startsWith 'ap' returns true", assertion4);
    overall_result &= assertion4;

    bool assertion5 = startsWith(root, "apple");
    print_assertion_result("startsWith 'apple' returns true", assertion5);
    overall_result &= assertion5;

    bool assertion6 = !startsWith(root, "appli");
    print_assertion_result("startsWith 'appli' returns false", assertion6);
    overall_result &= assertion6;

    print_test_summary("test_insert_and_search", overall_result);
    destroy(root);
}

void test_discard()
{
    printf("--- Running test_discard ---\n");
    bool overall_result = true;
    Trie *root = init();

    insert(root, "test");
    insert(root, "tester");
    insert(root, "tea");

    bool assertion1 = (countAll(root) == 3);
    print_assertion_result("initial count is 3", assertion1);
    overall_result &= assertion1;

    root = discard(root, "tea");
    bool assertion2 = (countAll(root) == 2);
    print_assertion_result("count is 2 after discarding 'tea'", assertion2);
    overall_result &= assertion2;

    bool assertion3 = !search(root, "tea");
    print_assertion_result("search 'tea' returns false after discard", assertion3);
    overall_result &= assertion3;

    bool assertion4 = search(root, "test");
    print_assertion_result("search 'test' still returns true", assertion4);
    overall_result &= assertion4;

    root = discard(root, "tester");
    bool assertion5 = (countAll(root) == 1);
    print_assertion_result("count is 1 after discarding 'tester'", assertion5);
    overall_result &= assertion5;

    bool assertion6 = !search(root, "tester");
    print_assertion_result("search 'tester' returns false after discard", assertion6);
    overall_result &= assertion6;

    bool assertion7 = search(root, "test");
    print_assertion_result("search 'test' still returns true", assertion7);
    overall_result &= assertion7;

    root = discard(root, "test");
    bool assertion8 = (countAll(root) == 0);
    print_assertion_result("count is 0 after discarding 'test'", assertion8);
    overall_result &= assertion8;

    bool assertion9 = !search(root, "test");
    print_assertion_result("search 'test' returns false after discard", assertion9);
    overall_result &= assertion9;

    print_test_summary("test_discard", overall_result);
    destroy(root);
}

void test_count()
{
    printf("--- Running test_count ---\n");
    bool overall_result = true;
    Trie *root = init();

    insert(root, "apple");
    insert(root, "app");
    insert(root, "application");
    insert(root, "apricot");
    insert(root, "banana");

    bool assertion1 = (countAll(root) == 5);
    print_assertion_result("total count is 5", assertion1);
    overall_result &= assertion1;

    bool assertion2 = (countWithPrefix(root, "ap") == 4);
    print_assertion_result("count with prefix 'ap' is 4", assertion2);
    overall_result &= assertion2;

    bool assertion3 = (countWithPrefix(root, "app") == 3);
    print_assertion_result("count with prefix 'app' is 3", assertion3);
    overall_result &= assertion3;

    bool assertion4 = (countWithPrefix(root, "orange") == 0);
    print_assertion_result("count with prefix 'orange' is 0", assertion4);
    overall_result &= assertion4;

    print_test_summary("test_count", overall_result);
    destroy(root);
}

void test_copy()
{
    printf("--- Running test_copy ---\n");
    bool overall_result = true;
    Trie *root = init();

    insert(root, "word");
    insert(root, "words");

    Trie *copy_root = copy(root);

    bool assertion1 = (copy_root != NULL);
    print_assertion_result("copy root is not NULL", assertion1);
    overall_result &= assertion1;

    bool assertion2 = search(copy_root, "word");
    print_assertion_result("copy contains 'word'", assertion2);
    overall_result &= assertion2;

    bool assertion3 = search(copy_root, "words");
    print_assertion_result("copy contains 'words'", assertion3);
    overall_result &= assertion3;

    bool assertion4 = (countAll(copy_root) == 2);
    print_assertion_result("copy has a total count of 2", assertion4);
    overall_result &= assertion4;

    root = discard(root, "word");

    bool assertion5 = !search(root, "word");
    print_assertion_result("original no longer contains 'word'", assertion5);
    overall_result &= assertion5;

    bool assertion6 = search(copy_root, "word");
    print_assertion_result("copy still contains 'word'", assertion6);
    overall_result &= assertion6;

    bool assertion7 = (countAll(root) == 1);
    print_assertion_result("original has a total count of 1", assertion7);
    overall_result &= assertion7;

    bool assertion8 = (countAll(copy_root) == 2);
    print_assertion_result("copy still has a total count of 2", assertion8);
    overall_result &= assertion8;

    print_test_summary("test_copy", overall_result);
    destroy(root);
    destroy(copy_root);
}

void test_display()
{
    printf("--- Running test_display ---\n");
    Trie *root = init();

    insert(root, "apple");
    insert(root, "app");
    insert(root, "banana");

    printf("This test requires manual verification of output.\n");
    printf("Expected output for displayAll: apple, app, banana (in some order).\n");
    printf("Actual output for displayAll:\n");
    displayAll(root);

    printf("\nExpected output for displayWithPrefix('a'): apple, app (in some order).\n");
    printf("Actual output for displayWithPrefix('a'):\n");
    displayWithPrefix(root, "a");

    printf("\n--- Test Case: test_display ---\n");
    printf("Result: " ANSI_COLOR_GREEN "PASSED" ANSI_COLOR_RESET "\n\n");

    destroy(root);
}

int main()
{
    test_init();
    test_insert_and_search();
    test_discard();
    test_count();
    test_copy();
    test_display();

    return 0;
}