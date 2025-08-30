#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include "adt_BinarySearchTree.h"

// ANSI Escape Codes for coloring output
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_RESET "\x1b[0m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_BLUE "\x1b[34m"

#define TEST_PASS() printf(ANSI_COLOR_GREEN "-> PASS\n\n" ANSI_COLOR_RESET)
#define TEST_FAIL()                                            \
    {                                                          \
        printf(ANSI_COLOR_RED "-> FAIL\n\n" ANSI_COLOR_RESET); \
        exit(EXIT_FAILURE);                                    \
    }

void test_init_and_destroy()
{
    printf(ANSI_COLOR_CYAN "--- Testing init() and destroy() ---\n" ANSI_COLOR_RESET);
    BinarySearchTree *root = init(10);
    printf("Test 1.1: Is new node not NULL?\n");
    printf("Expected: Not NULL\n");
    printf("Actual:   %s\n", (root != NULL) ? "Not NULL" : "NULL");
    if (root == NULL)
        TEST_FAIL();

    printf("Test 1.2: Is new node data correct?\n");
    printf("Expected: 10\n");
    printf("Actual:   %d\n", root->data);
    if (root->data != 10)
        TEST_FAIL();

    printf("Test 1.3: Are child pointers NULL?\n");
    printf("Expected: NULL, NULL\n");
    printf("Actual:   %s, %s\n", (root->left == NULL) ? "NULL" : "Not NULL", (root->right == NULL) ? "NULL" : "Not NULL");
    if (root->left != NULL || root->right != NULL)
        TEST_FAIL();

    root = destroy(root);
    printf("Test 1.4: Is root NULL after destroy?\n");
    printf("Expected: NULL\n");
    printf("Actual:   %s\n", (root == NULL) ? "NULL" : "Not NULL");
    if (root != NULL)
        TEST_FAIL();

    TEST_PASS();
}

void test_insert()
{
    printf(ANSI_COLOR_CYAN "--- Testing insert() ---\n" ANSI_COLOR_RESET);
    BinarySearchTree *root = NULL;
    root = insert(root, 50);
    root = insert(root, 30);
    root = insert(root, 70);
    root = insert(root, 20);
    root = insert(root, 40);
    root = insert(root, 60);
    root = insert(root, 80);

    printf("Test 2.1: In-order traversal of the tree after insertions\n");
    printf("Expected: <20> <30> <40> <50> <60> <70> <80>\n");
    printf("Actual:   ");
    inorder(root);
    printf("\n");

    printf("Test 2.2: Insert duplicate value (50) and check size\n");
    int initial_size = size(root);
    root = insert(root, 50);
    int final_size = size(root);
    printf("Expected size: %d\n", initial_size);
    printf("Actual size:   %d\n", final_size);
    if (initial_size != final_size)
        TEST_FAIL();

    root = destroy(root);
    TEST_PASS();
}

void test_search_and_relationships()
{
    printf(ANSI_COLOR_CYAN "--- Testing search(), parent(), and sibling() ---\n" ANSI_COLOR_RESET);
    BinarySearchTree *root = NULL;
    root = insert(root, 50);
    root = insert(root, 30);
    root = insert(root, 70);
    root = insert(root, 20);
    root = insert(root, 40);
    root = insert(root, 60);
    root = insert(root, 80);

    // Search tests
    printf("Test 3.1: Search for 50\n");
    printf("Expected: Found\n");
    printf("Actual:   %s\n", (search(root, 50) != NULL) ? "Found" : "Not Found");
    if (search(root, 50) == NULL)
        TEST_FAIL();

    printf("Test 3.2: Search for 100\n");
    printf("Expected: Not Found\n");
    printf("Actual:   %s\n", (search(root, 100) == NULL) ? "Not Found" : "Found");
    if (search(root, 100) != NULL)
        TEST_FAIL();

    // Parent tests
    printf("Test 3.3: Parent of 30\n");
    printf("Expected: 50\n");
    BinarySearchTree *p = parent(root, 30);
    printf("Actual:   %d\n", (p != NULL) ? p->data : -1);
    if (p == NULL || p->data != 50)
        TEST_FAIL();

    printf("Test 3.4: Parent of 50 (root)\n");
    printf("Expected: NULL\n");
    printf("Actual:   %s\n", (parent(root, 50) == NULL) ? "NULL" : "Not NULL");
    if (parent(root, 50) != NULL)
        TEST_FAIL();

    // Sibling tests
    printf("Test 3.5: Sibling of 30\n");
    printf("Expected: 70\n");
    BinarySearchTree *s = sibling(root, 30);
    printf("Actual:   %d\n", (s != NULL) ? s->data : -1);
    if (s == NULL || s->data != 70)
        TEST_FAIL();

    printf("Test 3.6: Sibling of 50 (root)\n");
    printf("Expected: NULL\n");
    printf("Actual:   %s\n", (sibling(root, 50) == NULL) ? "NULL" : "Not NULL");
    if (sibling(root, 50) != NULL)
        TEST_FAIL();

    root = destroy(root);
    TEST_PASS();
}

void test_discard()
{
    printf(ANSI_COLOR_CYAN "--- Testing discard() ---\n" ANSI_COLOR_RESET);
    BinarySearchTree *root = NULL;
    root = insert(root, 50);
    root = insert(root, 30);
    root = insert(root, 70);
    root = insert(root, 20);
    root = insert(root, 40);
    root = insert(root, 60);
    root = insert(root, 80);

    // Case 1: Discard leaf node (20)
    root = discard(root, 20);
    printf("Test 4.1: Discard leaf node 20\n");
    printf("Expected: Size=6, 20 not found\n");
    printf("Actual:   Size=%d, Found=%s\n", size(root), (search(root, 20) == NULL) ? "No" : "Yes");
    if (size(root) != 6 || search(root, 20) != NULL)
        TEST_FAIL();

    // Case 2: Discard node with one child (70)
    root = discard(root, 70);
    printf("Test 4.2: Discard one-child node 70\n");
    printf("Expected: Size=5, 70 not found, parent 50 points to 80\n");
    BinarySearchTree *p70 = parent(root, 80);
    printf("Actual:   Size=%d, Found=%s, Parent->data=%d\n", size(root), (search(root, 70) == NULL) ? "No" : "Yes", (p70 != NULL) ? p70->data : -1);
    if (size(root) != 5 || search(root, 70) != NULL || p70->data != 50)
        TEST_FAIL();

    // Case 3: Discard node with two children (30)
    root = discard(root, 30);
    printf("Test 4.3: Discard two-child node 30\n");
    printf("Expected: Size=4, 30 not found, parent 50 points to 40\n");
    BinarySearchTree *p30 = parent(root, 40);
    printf("Actual:   Size=%d, Found=%s, Parent->data=%d\n", size(root), (search(root, 30) == NULL) ? "No" : "Yes", (p30 != NULL) ? p30->data : -1);
    if (size(root) != 4 || search(root, 30) != NULL || p30->data != 50)
        TEST_FAIL();

    // Discard non-existent value
    printf("Test 4.4: Discard non-existent value (100)\n");
    int initial_size = size(root);
    root = discard(root, 100);
    printf("Expected: Size remains %d\n", initial_size);
    printf("Actual:   Size is %d\n", size(root));
    if (size(root) != initial_size)
        TEST_FAIL();

    root = destroy(root);
    TEST_PASS();
}

void test_min_max_successor_predecessor()
{
    printf(ANSI_COLOR_CYAN "--- Testing min/max and successor/predecessor ---\n" ANSI_COLOR_RESET);
    BinarySearchTree *root = NULL;
    root = insert(root, 50);
    root = insert(root, 30);
    root = insert(root, 70);
    root = insert(root, 20);
    root = insert(root, 40);
    root = insert(root, 60);
    root = insert(root, 80);

    printf("Test 5.1: Minimum value\n");
    printf("Expected: 20\n");
    BinarySearchTree *min_node = minimum(root);
    printf("Actual:   %d\n", (min_node != NULL) ? min_node->data : -1);
    if (min_node == NULL || min_node->data != 20)
        TEST_FAIL();

    printf("Test 5.2: Maximum value\n");
    printf("Expected: 80\n");
    BinarySearchTree *max_node = maximum(root);
    printf("Actual:   %d\n", (max_node != NULL) ? max_node->data : -1);
    if (max_node == NULL || max_node->data != 80)
        TEST_FAIL();

    printf("Test 5.3: Successor of 40\n");
    printf("Expected: 50\n");
    BinarySearchTree *succ_40 = successor(root, 40);
    printf("Actual:   %d\n", (succ_40 != NULL) ? succ_40->data : -1);
    if (succ_40 == NULL || succ_40->data != 50)
        TEST_FAIL();

    printf("Test 5.4: Successor of 80 (maximum value)\n");
    printf("Expected: NULL\n");
    BinarySearchTree *succ_80 = successor(root, 80);
    printf("Actual:   %s\n", (succ_80 == NULL) ? "NULL" : "Not NULL");
    if (succ_80 != NULL)
        TEST_FAIL();

    printf("Test 5.5: Predecessor of 60\n");
    printf("Expected: 50\n");
    BinarySearchTree *pred_60 = predecessor(root, 60);
    printf("Actual:   %d\n", (pred_60 != NULL) ? pred_60->data : -1);
    if (pred_60 == NULL || pred_60->data != 50)
        TEST_FAIL();

    printf("Test 5.6: Predecessor of 20 (minimum value)\n");
    printf("Expected: NULL\n");
    BinarySearchTree *pred_20 = predecessor(root, 20);
    printf("Actual:   %s\n", (pred_20 == NULL) ? "NULL" : "Not NULL");
    if (pred_20 != NULL)
        TEST_FAIL();

    root = destroy(root);
    TEST_PASS();
}

void test_traversals()
{
    printf(ANSI_COLOR_CYAN "--- Testing Traversal Functions ---\n" ANSI_COLOR_RESET);
    BinarySearchTree *root = NULL;
    root = insert(root, 50);
    root = insert(root, 30);
    root = insert(root, 70);
    root = insert(root, 20);
    root = insert(root, 40);
    root = insert(root, 60);
    root = insert(root, 80);

    printf("Test 6.1: In-order traversal\n");
    printf("Expected: " ANSI_COLOR_YELLOW "<20> <30> <40> <50> <60> <70> <80>\n" ANSI_COLOR_RESET);
    printf("Actual:   ");
    inorder(root);
    printf("\n");

    printf("Test 6.2: Pre-order traversal\n");
    printf("Expected: " ANSI_COLOR_YELLOW "<50> <30> <20> <40> <70> <60> <80>\n" ANSI_COLOR_RESET);
    printf("Actual:   ");
    preorder(root);
    printf("\n");

    printf("Test 6.3: Post-order traversal\n");
    printf("Expected: " ANSI_COLOR_YELLOW "<20> <40> <30> <60> <80> <70> <50>\n" ANSI_COLOR_RESET);
    printf("Actual:   ");
    postorder(root);
    printf("\n");

    printf("Test 6.4: Level-order traversal\n");
    printf("Expected: " ANSI_COLOR_YELLOW "<50> <30> <70> <20> <40> <60> <80>\n" ANSI_COLOR_RESET);
    printf("Actual:   ");
    levelorder(root);
    printf("\n");

    root = destroy(root);
    TEST_PASS();
}

void test_height_depth_size_degree_metrics()
{
    printf(ANSI_COLOR_CYAN "--- Testing height(), depth(), size(), degree(), external(), internal() ---\n" ANSI_COLOR_RESET);
    BinarySearchTree *root = NULL;
    printf("Test 7.1: Size of empty tree\n");
    printf("Expected: 0\n");
    printf("Actual:   %d\n", size(root));
    if (size(root) != 0)
        TEST_FAIL();

    printf("Test 7.2: Height of empty tree\n");
    printf("Expected: -1\n");
    printf("Actual:   %d\n", height(root));
    if (height(root) != -1)
        TEST_FAIL();

    root = insert(root, 50);
    root = insert(root, 30);
    root = insert(root, 70);
    root = insert(root, 20);
    root = insert(root, 40);
    root = insert(root, 60);
    root = insert(root, 80);

    printf("Test 7.3: Size of populated tree\n");
    printf("Expected: 7\n");
    printf("Actual:   %d\n", size(root));
    if (size(root) != 7)
        TEST_FAIL();

    printf("Test 7.4: Height of populated tree\n");
    printf("Expected: 2\n");
    printf("Actual:   %d\n", height(root));
    if (height(root) != 2)
        TEST_FAIL();

    printf("Test 7.5: Depth of node 20\n");
    printf("Expected: 2\n");
    printf("Actual:   %d\n", depth(root, 20));
    if (depth(root, 20) != 2)
        TEST_FAIL();

    printf("Test 7.6: Degree of node 30\n");
    printf("Expected: 2\n");
    printf("Actual:   %d\n", degree(search(root, 30)));
    if (degree(search(root, 30)) != 2)
        TEST_FAIL();

    printf("Test 7.7: Number of external (leaf) nodes\n");
    printf("Expected: 4\n");
    printf("Actual:   %d\n", external(root));
    if (external(root) != 4)
        TEST_FAIL();

    printf("Test 7.8: Number of internal nodes\n");
    printf("Expected: 3\n");
    printf("Actual:   %d\n", internal(root));
    if (internal(root) != 3)
        TEST_FAIL();

    root = destroy(root);
    TEST_PASS();
}

void test_tree_properties()
{
    printf(ANSI_COLOR_CYAN "--- Testing tree properties (isPerfect, isComplete, etc.) ---\n" ANSI_COLOR_RESET);

    // Perfect Tree
    BinarySearchTree *perfect_tree = NULL;
    perfect_tree = insert(perfect_tree, 4);
    perfect_tree = insert(perfect_tree, 2);
    perfect_tree = insert(perfect_tree, 6);
    perfect_tree = insert(perfect_tree, 1);
    perfect_tree = insert(perfect_tree, 3);
    perfect_tree = insert(perfect_tree, 5);
    perfect_tree = insert(perfect_tree, 7);
    printf("Test 8.1: Perfect tree properties\n");
    printf("Expected: isPerfect=true, isComplete=true, isFull=true, isBalanced=true\n");
    int h;
    printf("Actual:   isPerfect=%s, isComplete=%s, isFull=%s, isBalanced=%s\n", isPerfect(perfect_tree) ? "true" : "false", isComplete(perfect_tree) ? "true" : "false", isFull(perfect_tree) ? "true" : "false", isBalanced(perfect_tree, &h) ? "true" : "false");
    if (!isPerfect(perfect_tree) || !isComplete(perfect_tree) || !isFull(perfect_tree) || !isBalanced(perfect_tree, &h))
        TEST_FAIL();
    destroy(perfect_tree);

    // This tree is FULL and COMPLETE, but not PERFECT.
    // The previous test case had a correct `isFull` function but an incorrect expectation.
    BinarySearchTree *full_and_complete_tree = NULL;
    full_and_complete_tree = insert(full_and_complete_tree, 50);
    full_and_complete_tree = insert(full_and_complete_tree, 30);
    full_and_complete_tree = insert(full_and_complete_tree, 70);
    full_and_complete_tree = insert(full_and_complete_tree, 20);
    full_and_complete_tree = insert(full_and_complete_tree, 40);
    printf("Test 8.2: Complete and Full but not Perfect tree\n");
    printf("Expected: isPerfect=false, isComplete=true, isFull=true\n");
    printf("Actual:   isPerfect=%s, isComplete=%s, isFull=%s\n", isPerfect(full_and_complete_tree) ? "true" : "false", isComplete(full_and_complete_tree) ? "true" : "false", isFull(full_and_complete_tree) ? "true" : "false");
    if (isPerfect(full_and_complete_tree) || !isComplete(full_and_complete_tree) || !isFull(full_and_complete_tree))
        TEST_FAIL();
    destroy(full_and_complete_tree);

    // New Test Case: A tree that is COMPLETE but NOT FULL.
    // A tree like this will cause the `isFull` test to fail correctly.
    BinarySearchTree *complete_but_not_full_tree = NULL;
    complete_but_not_full_tree = insert(complete_but_not_full_tree, 50);
    complete_but_not_full_tree = insert(complete_but_not_full_tree, 30);
    complete_but_not_full_tree = insert(complete_but_not_full_tree, 70);
    complete_but_not_full_tree = insert(complete_but_not_full_tree, 20);
    printf("Test 8.3: Complete but not Full tree\n");
    printf("Expected: isPerfect=false, isComplete=true, isFull=false\n");
    printf("Actual:   isPerfect=%s, isComplete=%s, isFull=%s\n", isPerfect(complete_but_not_full_tree) ? "true" : "false", isComplete(complete_but_not_full_tree) ? "true" : "false", isFull(complete_but_not_full_tree) ? "true" : "false");
    if (isPerfect(complete_but_not_full_tree) || !isComplete(complete_but_not_full_tree) || isFull(complete_but_not_full_tree))
        TEST_FAIL();
    destroy(complete_but_not_full_tree);

    // Skewed Tree
    BinarySearchTree *skewed_tree = NULL;
    skewed_tree = insert(skewed_tree, 10);
    skewed_tree = insert(skewed_tree, 20);
    skewed_tree = insert(skewed_tree, 30);
    printf("Test 8.4: Skewed tree properties\n");
    printf("Expected: isDegenerate=true, isSkewed=true, isBalanced=false\n");
    printf("Actual:   isDegenerate=%s, isSkewed=%s, isBalanced=%s\n", isDegenerate(skewed_tree) ? "true" : "false", isSkewed(skewed_tree) ? "true" : "false", isBalanced(skewed_tree, &h) ? "true" : "false");
    if (!isDegenerate(skewed_tree) || !isSkewed(skewed_tree) || isBalanced(skewed_tree, &h))
        TEST_FAIL();
    destroy(skewed_tree);

    // Symmetric but not perfect
    BinarySearchTree *symmetric_tree = NULL;
    symmetric_tree = insert(symmetric_tree, 50);
    symmetric_tree = insert(symmetric_tree, 30);
    symmetric_tree = insert(symmetric_tree, 70);
    symmetric_tree = insert(symmetric_tree, 20);
    symmetric_tree = insert(symmetric_tree, 80);
    printf("Test 8.5: Symmetric tree properties\n");
    printf("Expected: isSymmetric=true\n");
    printf("Actual:   isSymmetric=%s\n", isSymmetric(symmetric_tree) ? "true" : "false");
    if (!isSymmetric(symmetric_tree))
        TEST_FAIL();
    destroy(symmetric_tree);

    // Unbalanced tree
    BinarySearchTree *unbalanced_tree = NULL;
    unbalanced_tree = insert(unbalanced_tree, 10);
    unbalanced_tree = insert(unbalanced_tree, 8);
    unbalanced_tree = insert(unbalanced_tree, 15);
    unbalanced_tree = insert(unbalanced_tree, 16);
    unbalanced_tree = insert(unbalanced_tree, 17);
    printf("Test 8.6: Unbalanced tree properties\n");
    printf("Expected: isBalanced=false\n");
    printf("Actual:   isBalanced=%s\n", isBalanced(unbalanced_tree, &h) ? "true" : "false");
    if (isBalanced(unbalanced_tree, &h))
        TEST_FAIL();
    destroy(unbalanced_tree);

    TEST_PASS();
}

void test_copy()
{
    printf(ANSI_COLOR_CYAN "--- Testing copy() ---\n" ANSI_COLOR_RESET);
    BinarySearchTree *root = NULL;
    root = insert(root, 50);
    root = insert(root, 30);
    root = insert(root, 70);
    BinarySearchTree *copied_root = copy(root);

    printf("Test 9.1: Is copied tree a new object?\n");
    printf("Expected: Pointers are different\n");
    printf("Actual:   %s\n", (copied_root != root) ? "Pointers are different" : "Pointers are same");
    if (copied_root == root)
        TEST_FAIL();

    printf("Test 9.2: Is copied tree data correct?\n");
    printf("Expected: 50, 30, 70\n");
    printf("Actual:   %d, %d, %d\n", copied_root->data, copied_root->left->data, copied_root->right->data);
    if (copied_root->data != 50 || copied_root->left->data != 30 || copied_root->right->data != 70)
        TEST_FAIL();

    destroy(root);
    destroy(copied_root);
    TEST_PASS();
}

void test_floor_ceiling()
{
    printf(ANSI_COLOR_CYAN "--- Testing floored() and ceiling() ---\n" ANSI_COLOR_RESET);
    BinarySearchTree *root = NULL;
    root = insert(root, 50);
    root = insert(root, 30);
    root = insert(root, 70);
    root = insert(root, 20);
    root = insert(root, 40);
    root = insert(root, 60);
    root = insert(root, 80);

    printf("Test 10.1: Ceiling of 35\n");
    printf("Expected: 40\n");
    BinarySearchTree *ceil_node = ceiling(root, 35);
    printf("Actual:   %d\n", (ceil_node != NULL) ? ceil_node->data : -1);
    if (ceil_node == NULL || ceil_node->data != 40)
        TEST_FAIL();

    printf("Test 10.2: Floor of 35\n");
    printf("Expected: 30\n");
    BinarySearchTree *floor_node = floored(root, 35);
    printf("Actual:   %d\n", (floor_node != NULL) ? floor_node->data : -1);
    if (floor_node == NULL || floor_node->data != 30)
        TEST_FAIL();

    printf("Test 10.3: Ceiling for a value greater than all nodes (90)\n");
    printf("Expected: NULL\n");
    BinarySearchTree *ceil_90 = ceiling(root, 90);
    printf("Actual:   %s\n", (ceil_90 == NULL) ? "NULL" : "Not NULL");
    if (ceil_90 != NULL)
        TEST_FAIL();

    printf("Test 10.4: Floor for a value less than all nodes (10)\n");
    printf("Expected: NULL\n");
    BinarySearchTree *floor_10 = floored(root, 10);
    printf("Actual:   %s\n", (floor_10 == NULL) ? "NULL" : "Not NULL");
    if (floor_10 != NULL)
        TEST_FAIL();

    destroy(root);
    TEST_PASS();
}

int main()
{
    printf(ANSI_COLOR_BLUE "\n*** Starting extensive BST test suite ***\n\n" ANSI_COLOR_RESET);

    test_init_and_destroy();
    test_insert();
    test_search_and_relationships();
    test_discard();
    test_min_max_successor_predecessor();
    test_traversals();
    test_height_depth_size_degree_metrics();
    test_tree_properties();
    test_copy();
    test_floor_ceiling();

    printf(ANSI_COLOR_BLUE "\n*** All tests completed successfully! ***\n" ANSI_COLOR_RESET);
    return 0;
}