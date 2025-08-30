#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include "adt_RedBlackTree.h"

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

bool is_rbt_valid(RedBlackTree *node);

int get_black_height(const RedBlackTree *node)
{
    if (node == NIL)
        return 1;
    int left_bh = get_black_height(node->left);
    int right_bh = get_black_height(node->right);
    if (left_bh == 0 || right_bh == 0 || left_bh != right_bh)
        return 0;
    return left_bh + (node->color == BLACK ? 1 : 0);
}

bool is_rbt_valid(RedBlackTree *root)
{
    if (root == NIL)
        return true;
    if (root->color != BLACK)
    {
        printf("RBT Validation Failed: Root is not BLACK.\n");
        return false;
    }
    if (root->color == RED && (root->left->color == RED || root->right->color == RED))
    {
        printf("RBT Validation Failed: Red node's child is also Red.\n");
        return false;
    }
    int bh = get_black_height(root);
    if (bh == 0)
    {
        printf("RBT Validation Failed: Black heights are not consistent.\n");
        return false;
    }
    return true;
}

void test_init_and_destroy()
{
    printf(ANSI_COLOR_CYAN "--- Testing init() and destroy() ---\n" ANSI_COLOR_RESET);
    printf("Test 1.1: Insert a single node (10) into an empty tree\n");
    RedBlackTree *root = NIL;
    root = insert(root, 10);
    printf("Expected Output: Not NULL, Data: 10, Color: BLACK\n");
    printf("Actual Output:   %s, Data: %d, Color: %s\n",
           (root != NIL) ? "Not NULL" : "NULL",
           (root != NIL) ? root->data : -1,
           (root != NIL && root->color == BLACK) ? "BLACK" : "RED");
    if (root == NIL || root->data != 10 || root->color != BLACK)
        TEST_FAIL();
    destroy(root);
    TEST_PASS();
}

void test_insert_and_properties()
{
    printf(ANSI_COLOR_CYAN "--- Testing insert() and RBT properties ---\n" ANSI_COLOR_RESET);
    RedBlackTree *root = NIL;
    int values[] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 5, 15, 25, 35, 45};
    printf("Test 2.1: Inserting a sequence of values\n");
    for (int i = 0; i < sizeof(values) / sizeof(values[0]); i++)
    {
        root = insert(root, values[i]);
    }
    printf("Test 2.2: Verifying RBT properties after insertions\n");
    printf("Expected Output: RBT properties hold\n");
    if (!is_rbt_valid(root))
    {
        printf("Actual Output:   RBT properties do NOT hold\n");
        TEST_FAIL();
    }
    printf("Actual Output:   RBT properties hold\n");
    printf("Test 2.3: In-order traversal after insertions\n");
    printf("Expected Output: A sorted sequence of numbers\n");
    printf("Actual Output:   ");
    inorder(root);
    printf("\n");
    printf("Test 2.4: Check size\n");
    printf("Expected Output: %zu\n", sizeof(values) / sizeof(values[0]));
    printf("Actual Output:   %d\n", size(root));
    if (size(root) != sizeof(values) / sizeof(values[0]))
        TEST_FAIL();
    destroy(root);
    TEST_PASS();
}

void test_search_and_relationships()
{
    printf(ANSI_COLOR_CYAN "--- Testing search() and relationships ---\n" ANSI_COLOR_RESET);
    RedBlackTree *root = NIL;
    int values[] = {50, 30, 70, 20, 40, 60, 80};
    for (int i = 0; i < sizeof(values) / sizeof(values[0]); i++)
    {
        root = insert(root, values[i]);
    }
    printf("Test 3.1: Search for 40\n");
    printf("Expected Output: Found\n");
    if (search(root, 40) == NIL)
    {
        printf("Actual Output:   Not Found\n");
        TEST_FAIL();
    }
    printf("Actual Output:   Found\n");
    printf("Test 3.2: Search for non-existent value (99)\n");
    printf("Expected Output: Not Found\n");
    if (search(root, 99) != NIL)
    {
        printf("Actual Output:   Found\n");
        TEST_FAIL();
    }
    printf("Actual Output:   Not Found\n");
    printf("Test 3.3: Parent of 40\n");
    RedBlackTree *node = search(root, 40);
    if (node == NIL)
        TEST_FAIL();
    printf("Expected Output: 30\n");
    printf("Actual Output:   %d\n", node->parent->data);
    if (node->parent->data != 30)
        TEST_FAIL();
    destroy(root);
    TEST_PASS();
}

void test_discard()
{
    printf(ANSI_COLOR_CYAN "--- Testing discard() ---\n" ANSI_COLOR_RESET);
    RedBlackTree *root = NIL;
    int values[] = {50, 30, 70, 20, 40, 60, 80};
    for (int i = 0; i < sizeof(values) / sizeof(values[0]); i++)
    {
        root = insert(root, values[i]);
    }
    printf("Test 4.1: Discard leaf node (40)\n");
    root = discard(root, 40);
    if (search(root, 40) != NIL || size(root) != 6 || !is_rbt_valid(root))
        TEST_FAIL();
    printf("Test 4.2: Discard node with one child (20)\n");
    root = discard(root, 20);
    if (search(root, 20) != NIL || size(root) != 5 || !is_rbt_valid(root))
        TEST_FAIL();
    printf("Test 4.3: Discard node with two children (50)\n");
    root = discard(root, 50);
    if (search(root, 50) != NIL || size(root) != 4 || !is_rbt_valid(root))
        TEST_FAIL();
    printf("Test 4.4: Discard non-existent value (99)\n");
    int initial_size = size(root);
    root = discard(root, 99);
    if (size(root) != initial_size)
        TEST_FAIL();
    destroy(root);
    TEST_PASS();
}

void test_min_max_predecessor_successor()
{
    printf(ANSI_COLOR_CYAN "--- Testing min/max and predecessor/successor ---\n" ANSI_COLOR_RESET);
    RedBlackTree *root = NIL;
    int values[] = {50, 30, 70, 20, 40, 60, 80};
    for (int i = 0; i < sizeof(values) / sizeof(values[0]); i++)
    {
        root = insert(root, values[i]);
    }
    printf("Test 5.1: Minimum value\n");
    printf("Expected Output: 20\n");
    printf("Actual Output:   %d\n", minimum(root)->data);
    if (minimum(root)->data != 20)
        TEST_FAIL();
    printf("Test 5.2: Maximum value\n");
    printf("Expected Output: 80\n");
    printf("Actual Output:   %d\n", maximum(root)->data);
    if (maximum(root)->data != 80)
        TEST_FAIL();
    printf("Test 5.3: Successor of 40\n");
    printf("Expected Output: 50\n");
    printf("Actual Output:   %d\n", successor(root, 40)->data);
    if (successor(root, 40)->data != 50)
        TEST_FAIL();
    printf("Test 5.4: Successor of 80 (max)\n");
    printf("Expected Output: NIL (0x0)\n");
    printf("Actual Output:   %p\n", (void *)successor(root, 80));
    if (successor(root, 80) != NIL)
        TEST_FAIL();
    printf("Test 5.5: Predecessor of 60\n");
    printf("Expected Output: 50\n");
    printf("Actual Output:   %d\n", predecessor(root, 60)->data);
    if (predecessor(root, 60)->data != 50)
        TEST_FAIL();
    printf("Test 5.6: Predecessor of 20 (min)\n");
    printf("Expected Output: NIL (0x0)\n");
    printf("Actual Output:   %p\n", (void *)predecessor(root, 20));
    if (predecessor(root, 20) != NIL)
        TEST_FAIL();
    destroy(root);
    TEST_PASS();
}

void test_traversals()
{
    printf(ANSI_COLOR_CYAN "--- Testing Traversal Functions ---\n" ANSI_COLOR_RESET);
    RedBlackTree *root = NIL;
    int values[] = {50, 30, 70, 20, 40, 60, 80};
    for (int i = 0; i < sizeof(values) / sizeof(values[0]); i++)
    {
        root = insert(root, values[i]);
    }
    printf("Test 6.1: In-order traversal\n");
    printf("Expected Output: <20> <30> <40> <50> <60> <70> <80>\n");
    printf("Actual Output:   ");
    inorder(root);
    printf("\n");
    printf("Test 6.2: Pre-order traversal\n");
    printf("Expected Output: <50> <30> <20> <40> <70> <60> <80>\n");
    printf("Actual Output:   ");
    preorder(root);
    printf("\n");
    printf("Test 6.3: Post-order traversal\n");
    printf("Expected Output: <20> <40> <30> <60> <80> <70> <50>\n");
    printf("Actual Output:   ");
    postorder(root);
    printf("\n");
    printf("Test 6.4: Level-order traversal\n");
    printf("Expected Output: <50> <30> <70> <20> <40> <60> <80>\n");
    printf("Actual Output:   ");
    levelorder(root);
    printf("\n");
    destroy(root);
    TEST_PASS();
}

void test_metrics()
{
    printf(ANSI_COLOR_CYAN "--- Testing size(), height(), depth() ---\n" ANSI_COLOR_RESET);
    RedBlackTree *root = NIL;
    int values[] = {50, 30, 70, 20, 40, 60, 80};
    for (int i = 0; i < sizeof(values) / sizeof(values[0]); i++)
    {
        root = insert(root, values[i]);
    }
    printf("Test 7.1: Size of tree\n");
    printf("Expected Output: 7\n");
    printf("Actual Output:   %d\n", size(root));
    if (size(root) != 7)
        TEST_FAIL();
    printf("Test 7.2: Height of tree\n");
    printf("Expected Output: 2, 3, or 4. Actual: %d\n", height(root));
    if (height(root) < 2 || height(root) > 4)
        TEST_FAIL();
    printf("Test 7.3: Depth of 20\n");
    printf("Expected Output: 2\n");
    printf("Actual Output:   %d\n", depth(root, 20));
    if (depth(root, 20) != 2)
        TEST_FAIL();
    printf("Test 7.4: Internal nodes\n");
    printf("Expected Output: 3\n");
    printf("Actual Output:   %d\n", internal(root));
    if (internal(root) != 3)
        TEST_FAIL();
    printf("Test 7.5: External nodes\n");
    printf("Expected Output: 4\n");
    printf("Actual Output:   %d\n", external(root));
    if (external(root) != 4)
        TEST_FAIL();
    destroy(root);
    TEST_PASS();
}

void test_copy()
{
    printf(ANSI_COLOR_CYAN "--- Testing copy() ---\n" ANSI_COLOR_RESET);
    RedBlackTree *root = NIL;
    int values[] = {50, 30, 70, 20, 40, 60, 80};
    for (int i = 0; i < sizeof(values) / sizeof(values[0]); i++)
    {
        root = insert(root, values[i]);
    }
    RedBlackTree *copied_root = copy(root);
    printf("Test 8.1: Check if copied tree exists and is a deep copy.\n");
    printf("Expected Output: Not NULL and a different memory address.\n");
    printf("Actual Output:   %s and a %s memory address.\n",
           (copied_root != NIL) ? "Not NULL" : "NULL",
           (copied_root != root) ? "different" : "same");
    if (copied_root == NIL || copied_root == root)
    {
        TEST_FAIL();
    }
    printf("Test 8.2: Check if copied tree has same size.\n");
    printf("Expected Output: %d\n", size(root));
    printf("Actual Output:   %d\n", size(copied_root));
    if (size(root) != size(copied_root))
    {
        TEST_FAIL();
    }
    printf("Test 8.3: Check if copied tree is a valid RBT.\n");
    printf("Expected Output: Valid\n");
    if (!is_rbt_valid(copied_root))
    {
        printf("Actual Output:   Invalid\n");
        TEST_FAIL();
    }
    printf("Actual Output:   Valid\n");
    root = discard(root, 40);
    printf("Test 8.4: Modifying original tree does not affect the copy.\n");
    printf("Expected Output: Value 40 still exists in copied tree.\n");
    if (search(copied_root, 40) == NIL)
    {
        printf("Actual Output:   Value 40 not found in copied tree.\n");
        TEST_FAIL();
    }
    printf("Actual Output:   Value 40 found in copied tree.\n");
    destroy(root);
    destroy(copied_root);
    TEST_PASS();
}

void test_ceiling_floor()
{
    printf(ANSI_COLOR_CYAN "--- Testing ceiling() and floored() ---\n" ANSI_COLOR_RESET);
    RedBlackTree *root = NIL;
    int values[] = {10, 20, 30, 40, 50, 60};
    for (int i = 0; i < sizeof(values) / sizeof(values[0]); i++)
    {
        root = insert(root, values[i]);
    }
    printf("Test 9.1: Ceiling of 25\n");
    printf("Expected Output: 30\n");
    RedBlackTree *ceil_node = ceiling(root, 25);
    printf("Actual Output:   %d\n", (ceil_node != NIL) ? ceil_node->data : -1);
    if (ceil_node == NIL || ceil_node->data != 30)
    {
        TEST_FAIL();
    }
    printf("Test 9.2: Floor of 25\n");
    printf("Expected Output: 20\n");
    RedBlackTree *floor_node = floored(root, 25);
    printf("Actual Output:   %d\n", (floor_node != NIL) ? floor_node->data : -1);
    if (floor_node == NIL || floor_node->data != 20)
    {
        TEST_FAIL();
    }
    printf("Test 9.3: Ceiling of an exact value (40)\n");
    printf("Expected Output: 40\n");
    ceil_node = ceiling(root, 40);
    printf("Actual Output:   %d\n", (ceil_node != NIL) ? ceil_node->data : -1);
    if (ceil_node == NIL || ceil_node->data != 40)
    {
        TEST_FAIL();
    }
    printf("Test 9.4: Floor of an exact value (40)\n");
    printf("Expected Output: 40\n");
    floor_node = floored(root, 40);
    printf("Actual Output:   %d\n", (floor_node != NIL) ? floor_node->data : -1);
    if (floor_node == NIL || floor_node->data != 40)
    {
        TEST_FAIL();
    }
    printf("Test 9.5: Ceiling of a value greater than max (70)\n");
    printf("Expected Output: NIL (0x0)\n");
    printf("Actual Output:   %p\n", (void *)ceiling(root, 70));
    if (ceiling(root, 70) != NIL)
    {
        TEST_FAIL();
    }
    printf("Test 9.6: Floor of a value smaller than min (5)\n");
    printf("Expected Output: NIL (0x0)\n");
    printf("Actual Output:   %p\n", (void *)floored(root, 5));
    if (floored(root, 5) != NIL)
    {
        TEST_FAIL();
    }
    destroy(root);
    TEST_PASS();
}

void test_structural_properties()
{
    printf(ANSI_COLOR_CYAN "--- Testing structural properties ---\n" ANSI_COLOR_RESET);
    RedBlackTree *perfect_root = NIL;
    int perfect_values[] = {4, 2, 6, 1, 3, 5, 7};
    for (int i = 0; i < sizeof(perfect_values) / sizeof(perfect_values[0]); i++)
    {
        perfect_root = insert(perfect_root, perfect_values[i]);
    }
    printf("Test 10.1: isPerfect on a perfect tree-like structure.\n");
    printf("Expected Output: true\n");
    printf("Actual Output:   %s\n", isPerfect(perfect_root) ? "true" : "false");
    if (!isPerfect(perfect_root))
    {
        TEST_FAIL();
    }
    printf("Test 10.2: isComplete on a perfect tree.\n");
    printf("Expected Output: true\n");
    printf("Actual Output:   %s\n", isComplete(perfect_root) ? "true" : "false");
    if (!isComplete(perfect_root))
    {
        TEST_FAIL();
    }
    printf("Test 10.3: isFull on a perfect tree.\n");
    printf("Expected Output: true\n");
    printf("Actual Output:   %s\n", isFull(perfect_root) ? "true" : "false");
    if (!isFull(perfect_root))
    {
        TEST_FAIL();
    }
    printf("Test 10.4: isSymmetric on a symmetric tree.\n");
    printf("Expected Output: true\n");
    printf("Actual Output:   %s\n", isSymmetric(perfect_root) ? "true" : "false");
    if (!isSymmetric(perfect_root))
    {
        TEST_FAIL();
    }
    destroy(perfect_root);
    RedBlackTree *complete_root = NIL;
    int complete_values[] = {10, 5, 15, 2};
    for (int i = 0; i < sizeof(complete_values) / sizeof(complete_values[0]); i++)
    {
        complete_root = insert(complete_root, complete_values[i]);
    }
    printf("Test 10.5: isPerfect on a non-perfect tree.\n");
    printf("Expected Output: false\n");
    printf("Actual Output:   %s\n", isPerfect(complete_root) ? "true" : "false");
    if (isPerfect(complete_root))
    {
        TEST_FAIL();
    }
    printf("Test 10.6: isComplete on a complete tree.\n");
    printf("Expected Output: true\n");
    printf("Actual Output:   %s\n", isComplete(complete_root) ? "true" : "false");
    if (!isComplete(complete_root))
    {
        TEST_FAIL();
    }
    printf("Test 10.7: isFull on a non-full tree.\n");
    printf("Expected Output: false\n");
    printf("Actual Output:   %s\n", isFull(complete_root) ? "true" : "false");
    if (isFull(complete_root))
    {
        TEST_FAIL();
    }
    printf("Test 10.8: isSymmetric on an asymmetric tree.\n");
    printf("Expected Output: false\n");
    printf("Actual Output:   %s\n", isSymmetric(complete_root) ? "true" : "false");
    if (isSymmetric(complete_root))
    {
        TEST_FAIL();
    }
    destroy(complete_root);
    TEST_PASS();
}

int main()
{
    __sentinel__();
    printf(ANSI_COLOR_BLUE "\n*** Starting extensive Red-Black Tree test suite ***\n\n" ANSI_COLOR_RESET);
    test_init_and_destroy();
    test_insert_and_properties();
    test_search_and_relationships();
    test_discard();
    test_min_max_predecessor_successor();
    test_traversals();
    test_metrics();
    test_copy();
    test_ceiling_floor();
    test_structural_properties();
    if (NIL != NULL)
    {
        free(NIL);
        NIL = NULL;
    }
    printf(ANSI_COLOR_BLUE "\n*** All tests completed successfully! ***\n" ANSI_COLOR_RESET);
    return 0;
}