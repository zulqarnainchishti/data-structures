#ifndef RED_BLACK_TREE_H
#define RED_BLACK_TREE_H

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * @enum Color
 * @brief Represents the color of a Red-Black Tree node.
 */
typedef enum
{
    RED,
    BLACK
} Color;

/**
 * @struct RedBlackTree
 * @brief Represents a node in the Red-Black Tree.
 * @var data The integer value stored in the node.
 * @var color The color of the node (RED or BLACK).
 * @var parent Pointer to the parent node.
 * @var left Pointer to the left child.
 * @var right Pointer to the right child.
 */
typedef struct RedBlackTree
{
    int data;
    Color color;
    struct RedBlackTree *parent;
    struct RedBlackTree *left;
    struct RedBlackTree *right;
} RedBlackTree;

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GRAY "\x1b[90m"
#define ANSI_COLOR_RESET "\x1b[0m"
static RedBlackTree *NIL = NULL;

// ------------------------- Core Operations -------------------------------

/**
 * @brief Initializes the sentinel NIL node.
 */
void __sentinel__()
{
    NIL = (RedBlackTree *)malloc(sizeof(RedBlackTree));
    if (NIL == NULL)
    {
        perror("Failed to initialize Sentinel.");
        exit(EXIT_FAILURE);
    }
    NIL->data = 0;
    NIL->color = BLACK;
    NIL->parent = NIL;
    NIL->left = NIL;
    NIL->right = NIL;
}

/**
 * @brief Initializes a new RBT node.
 * @param value The integer value to store in the new node.
 * @return A pointer to the newly created node, or NULL if memory allocation fails.
 */
RedBlackTree *init(const int value)
{
    if (NIL == NULL)
        __sentinel__();

    RedBlackTree *node = (RedBlackTree *)malloc(sizeof(RedBlackTree));
    if (node == NULL)
    {
        perror("Failed to initialize Node.");
        exit(EXIT_FAILURE);
    }
    node->data = value;
    node->color = RED;
    node->parent = NIL;
    node->left = NIL;
    node->right = NIL;
    return node;
}

/**
 * @brief Destroys the entire RBT and deallocates memory.
 * @param root A pointer to the root of the tree.
 * @return Always NIL after destruction.
 */
void destroy(RedBlackTree *root)
{
    if (root == NIL)
        return;
    destroy(root->left);
    destroy(root->right);
    free(root);
}

/**
 * @brief Creates a deep copy of a RBT.
 * @param root A pointer to the root of the tree to be copied.
 * @return A pointer to the root of the new, copied tree.
 */
RedBlackTree *copy(const RedBlackTree *root)
{
    if (root == NULL || root == NIL)
        return NIL;
    RedBlackTree *copied = init(root->data);
    copied->color = root->color;
    copied->left = copy(root->left);
    if (copied->left != NIL)
        copied->left->parent = copied;
    copied->right = copy(root->right);
    if (copied->right != NIL)
        copied->right->parent = copied;
    return copied;
}

/**
 * @brief Performs a left rotation on the Red-Black Tree.
 * @param root A pointer to the root of the tree.
 * @param x The node to be rotated.
 * @return A pointer to the new root of the modified subtree.
 */
RedBlackTree *__leftRotate__(RedBlackTree *root, RedBlackTree *x)
{
    RedBlackTree *y = x->right;
    x->right = y->left;
    if (x->right != NIL)
        x->right->parent = x;
    y->parent = x->parent;
    if (x->parent == NIL)
        root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    y->left = x;
    x->parent = y;
    return root;
}

/**
 * @brief Performs a right rotation on the Red-Black Tree.
 * @param root A pointer to the root of the tree.
 * @param x The node to be rotated.
 * @return A pointer to the new root of the modified subtree.
 */
RedBlackTree *__rightRotate__(RedBlackTree *root, RedBlackTree *x)
{
    RedBlackTree *y = x->left;
    x->left = y->right;
    if (x->left != NIL)
        x->left->parent = x;
    y->parent = x->parent;
    if (x->parent == NIL)
        root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    y->right = x;
    x->parent = y;
    return root;
}

/**
 * @brief Fixes the Red-Black Tree properties after an insertion.
 * @param root A pointer to the root of the tree.
 * @param node The newly inserted node.
 * @return A pointer to the root of the modified tree.
 */
RedBlackTree *__fixInsertion__(RedBlackTree *root, RedBlackTree *node)
{
    while (node->parent->color == RED)
    {
        // CASE: parent is a left child
        if (node->parent == node->parent->parent->left)
        {
            RedBlackTree *uncle = node->parent->parent->right;
            // Case 1: Uncle is RED
            if (uncle->color == RED)
            {
                node->parent->color = BLACK;
                uncle->color = BLACK;
                node->parent->parent->color = RED;
                node = node->parent->parent;
            }
            else
            {
                // Case 2: Uncle is BLACK, and node is a right child
                if (node == node->parent->right)
                {
                    node = node->parent;
                    root = __leftRotate__(root, node);
                }
                // Case 3: Uncle is BLACK, and node is a left child
                node->parent->color = BLACK;
                node->parent->parent->color = RED;
                root = __rightRotate__(root, node->parent->parent);
            }
        }
        // Mirror CASE: parent is a right child
        else
        {
            RedBlackTree *uncle = node->parent->parent->left;
            // Case 1: Uncle is RED
            if (uncle->color == RED)
            {
                node->parent->color = BLACK;
                uncle->color = BLACK;
                node->parent->parent->color = RED;
                node = node->parent->parent;
            }
            else
            {
                // Case 2: Uncle is BLACK, and node is a left child
                if (node == node->parent->left)
                {
                    node = node->parent;
                    root = __rightRotate__(root, node);
                }
                // Case 3: Uncle is BLACK, and node is a right child
                node->parent->color = BLACK;
                node->parent->parent->color = RED;
                root = __leftRotate__(root, node->parent->parent);
            }
        }
    }
    // Ensure root is always BLACK
    root->color = BLACK;
    return root;
}

/**
 * @brief Inserts a new value into the RBT.
 * @param root A pointer to the root of the tree.
 * @param value The value to insert.
 * @return A pointer to the root of the modified tree.
 */
RedBlackTree *insert(RedBlackTree *root, const int value)
{
    if (root == NIL)
    {
        RedBlackTree *node = init(value);
        node->color = BLACK;
        return node;
    }
    RedBlackTree *prnt = NIL;
    RedBlackTree *curr = root;
    while (curr != NIL)
    {
        prnt = curr;
        if (value < curr->data)
            curr = curr->left;
        else if (value > curr->data)
            curr = curr->right;
        else
            return root;
    }
    RedBlackTree *node = init(value);
    node->parent = prnt;
    if (value < prnt->data)
        prnt->left = node;
    else
        prnt->right = node;
    root = __fixInsertion__(root, node);
    return root;
}

/**
 * @brief Helper function for deletion to move subtrees.
 * @param root A pointer to the root of the tree.
 * @param u The node to be replaced.
 * @param v The node replacing u.
 * @return A pointer to the root of the modified tree.
 */
RedBlackTree *__transplant__(RedBlackTree *root, RedBlackTree *u, RedBlackTree *v)
{
    if (u->parent == NIL)
        root = v;
    else if (u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;
    v->parent = u->parent;
    return root;
}

/**
 * @brief Fixes the Red-Black Tree properties after a deletion.
 * @param root A pointer to the root of the tree.
 * @param node The node that replaced the deleted node.
 * @return A pointer to the root of the modified tree.
 */
RedBlackTree *__fixDeletion__(RedBlackTree *root, RedBlackTree *node)
{
    while (node != root && node->color == BLACK)
    {
        // CASE: node is a left child
        if (node == node->parent->left)
        {
            RedBlackTree *sibling = node->parent->right;
            // Case 1: Sibling is RED
            if (sibling->color == RED)
            {
                sibling->color = BLACK;
                node->parent->color = RED;
                root = __leftRotate__(root, node->parent);
                sibling = node->parent->right;
            }
            // Case 2: Sibling and both its children are BLACK
            if (sibling->left->color == BLACK && sibling->right->color == BLACK)
            {
                sibling->color = RED;
                node = node->parent;
            }
            else
            {
                // Case 3: Sibling is BLACK, sibling's right child is BLACK, left child is RED
                if (sibling->right->color == BLACK)
                {
                    sibling->left->color = BLACK;
                    sibling->color = RED;
                    root = __rightRotate__(root, sibling);
                    sibling = node->parent->right;
                }
                // Case 4: Sibling is BLACK, and sibling's right child is RED
                sibling->color = node->parent->color;
                node->parent->color = BLACK;
                sibling->right->color = BLACK;
                root = __leftRotate__(root, node->parent);
                node = root;
            }
        }
        // Mirror CASE: node is a right child
        else
        {
            RedBlackTree *sibling = node->parent->left;
            // Case 1: Sibling is RED
            if (sibling->color == RED)
            {
                sibling->color = BLACK;
                node->parent->color = RED;
                root = __rightRotate__(root, node->parent);
                sibling = node->parent->left;
            }
            // Case 2: Sibling and both its children are BLACK
            if (sibling->left->color == BLACK && sibling->right->color == BLACK)
            {
                sibling->color = RED;
                node = node->parent;
            }
            else
            {
                // Case 3: Sibling is BLACK, sibling's left child is BLACK, right child is RED
                if (sibling->left->color == BLACK)
                {
                    sibling->right->color = BLACK;
                    sibling->color = RED;
                    root = __leftRotate__(root, sibling);
                    sibling = node->parent->left;
                }
                // Case 4: Sibling is BLACK, and sibling's left child is RED
                sibling->color = node->parent->color;
                node->parent->color = BLACK;
                sibling->left->color = BLACK;
                root = __rightRotate__(root, node->parent);
                node = root;
            }
        }
    }
    // Ensure replacement node is BLACK
    node->color = BLACK;
    return root;
}

/**
 * @brief Deletes a node with the specified value from the RBT.
 * @param root A pointer to the root of the tree.
 * @param value The value of the node to be deleted.
 * @return A pointer to the root of the modified tree.
 */
RedBlackTree *discard(RedBlackTree *root, const int value)
{
    Color deletedColor = RED;
    RedBlackTree *replacementNode = NIL;
    RedBlackTree *curr = root;
    while (curr != NIL)
    {
        if (value < curr->data)
            curr = curr->left;
        else if (value > curr->data)
            curr = curr->right;
        else
        {
            if (curr->left == NIL && curr->right == NIL)
            {
                replacementNode = NIL;
                deletedColor = curr->color;

                root = __transplant__(root, curr, NIL);
                free(curr);
            }
            else if (curr->left == NIL || curr->right == NIL)
            {
                RedBlackTree *child = (curr->left != NIL) ? curr->left : curr->right;
                replacementNode = child;
                deletedColor = curr->color;

                root = __transplant__(root, curr, child);
                free(curr);
            }
            else
            {
                RedBlackTree *succ = curr->right;
                while (succ->left != NIL)
                    succ = succ->left;
                curr->data = succ->data;

                RedBlackTree *child = succ->right;
                replacementNode = child;
                deletedColor = succ->color;

                root = __transplant__(root, succ, child);
                free(succ);
            }
            break;
        }
    }
    if (deletedColor == BLACK)
        root = __fixDeletion__(root, replacementNode);
    return root;
}

// ------------------------- Search Operations -----------------------------

/**
 * @brief Searches for a node with a specific value.
 * @param root A pointer to the root of the tree.
 * @param value The value to search for.
 * @return A pointer to the found node, or NIL if not found.
 */
RedBlackTree *search(RedBlackTree *root, const int value)
{
    while (root != NIL)
    {
        if (value == root->data)
            return root;
        else if (value < root->data)
            root = root->left;
        else
            root = root->right;
    }
    return NIL;
}

/**
 * @brief Finds the node with the minimum value in the tree.
 * @param root A pointer to the root of the tree.
 * @return A pointer to the minimum node, or NIL if the tree is empty.
 */
RedBlackTree *minimum(RedBlackTree *root)
{
    if (root == NIL)
        return NIL;
    while (root->left != NIL)
        root = root->left;
    return root;
}

/**
 * @brief Finds the node with the maximum value in the tree.
 * @param root A pointer to the root of the tree.
 * @return A pointer to the maximum node, or NIL if the tree is empty.
 */
RedBlackTree *maximum(RedBlackTree *root)
{
    if (root == NIL)
        return NIL;
    while (root->right != NIL)
        root = root->right;
    return root;
}

/**
 * @brief Finds the in-order successor of a node.
 * @param root A pointer to the root of the tree.
 * @param value The value of the node whose successor is to be found.
 * @return A pointer to the successor node, or NIL if no successor exists.
 */
RedBlackTree *successor(RedBlackTree *root, const int value)
{
    RedBlackTree *node = search(root, value);
    if (node == NIL)
        return NIL;
    if (node->right != NIL)
        return minimum(node->right);
    RedBlackTree *p = node->parent;
    while (p != NIL && p->right == node)
    {
        node = p;
        p = p->parent;
    }
    return p;
}

/**
 * @brief Finds the in-order predecessor of a node.
 * @param root A pointer to the root of the tree.
 * @param value The value of the node whose predecessor is to be found.
 * @return A pointer to the predecessor node, or NIL if no predecessor exists.
 */
RedBlackTree *predecessor(RedBlackTree *root, const int value)
{
    RedBlackTree *node = search(root, value);
    if (node == NIL)
        return NIL;
    if (node->left != NIL)
        return maximum(node->left);
    RedBlackTree *p = node->parent;
    while (p != NIL && p->left == node)
    {
        node = p;
        p = p->parent;
    }
    return p;
}

/**
 * @brief Finds the smallest value greater than or equal to the given value.
 * @param root A pointer to the root of the tree.
 * @param value The value to find the ceiling for.
 * @return A pointer to the ceiling node, or NIL if no such value exists.
 */
RedBlackTree *ceiling(RedBlackTree *root, const double value)
{
    RedBlackTree *next = NIL;
    while (root != NIL)
    {
        if (value == root->data)
            return root;
        else if (value < root->data)
        {
            next = root;
            root = root->left;
        }
        else
            root = root->right;
    }
    return next;
}

/**
 * @brief Finds the largest value less than or equal to the given value.
 * @param root A pointer to the root of the tree.
 * @param value The value to find the floor for.
 * @return A pointer to the floor node, or NIL if no such value exists.
 */
RedBlackTree *floored(RedBlackTree *root, const double value)
{
    RedBlackTree *prev = NIL;
    while (root != NIL)
    {
        if (value == root->data)
            return root;
        else if (value > root->data)
        {
            prev = root;
            root = root->right;
        }
        else
            root = root->left;
    }
    return prev;
}

// ------------------------- Tree Properties -------------------------------

/**
 * @brief Calculates the number of children of a node.
 * @param root A pointer to the node.
 * @return The number of children (0, 1, or 2).
 */
int degree(const RedBlackTree *root)
{
    if (root == NIL)
        return 0;
    return (root->left != NIL) + (root->right != NIL);
}

/**
 * @brief Counts the total number of nodes in the tree.
 * @param root A pointer to the root of the tree.
 * @return The total number of nodes.
 */
int size(const RedBlackTree *root)
{
    if (root == NIL)
        return 0;
    return 1 + size(root->left) + size(root->right);
}

/**
 * @brief Counts the number of external (leaf) nodes.
 * @param root A pointer to the root of the tree.
 * @return The number of external nodes.
 */
int external(const RedBlackTree *root)
{
    if (root == NIL)
        return 0;
    if (root->left == NIL && root->right == NIL)
        return 1;
    return external(root->left) + external(root->right);
}

/**
 * @brief Counts the number of internal nodes (nodes with at least one child).
 * @param root A pointer to the root of the tree.
 * @return The number of internal nodes.
 */
int internal(const RedBlackTree *root)
{
    if (root == NIL)
        return 0;
    if (root->left == NIL && root->right == NIL)
        return 0;
    return 1 + internal(root->left) + internal(root->right);
}

/**
 * @brief Calculates the height of the tree.
 * @param root A pointer to the root of the tree.
 * @return The height of the tree. Returns -1 for an empty tree.
 */
int height(const RedBlackTree *root)
{
    if (root == NIL)
        return -1;
    int leftHeight = height(root->left);
    int rightHeight = height(root->right);
    return 1 + ((leftHeight > rightHeight) ? leftHeight : rightHeight);
}

/**
 * @brief Calculates the depth of a node.
 * @param root A pointer to the root of the tree.
 * @param value The value of the node whose depth is to be found.
 * @return The depth of the node. Returns -1 if the node is not found.
 */
int depth(const RedBlackTree *root, const int value)
{
    int level = 0;
    while (root != NIL)
    {
        if (value == root->data)
            return level;
        else if (value < root->data)
            root = root->left;
        else
            root = root->right;
        level++;
    }
    return -1;
}

// ------------------------- Traversal Operations --------------------------

/**
 * @brief Prints the tree nodes in pre-order traversal (Root -> Left -> Right).
 * @param root A pointer to the root of the tree.
 */
void preorder(const RedBlackTree *root)
{
    if (root == NIL)
        return;
    if (root->color == RED)
        printf(ANSI_COLOR_RED "<%d> " ANSI_COLOR_RESET, root->data);
    else
        printf(ANSI_COLOR_GRAY "<%d> " ANSI_COLOR_RESET, root->data);
    preorder(root->left);
    preorder(root->right);
}

/**
 * @brief Prints the tree nodes in in-order traversal (Left -> Root -> Right).
 * @param root A pointer to the root of the tree.
 */
void inorder(const RedBlackTree *root)
{
    if (root == NIL)
        return;
    inorder(root->left);
    if (root->color == RED)
        printf(ANSI_COLOR_RED "<%d> " ANSI_COLOR_RESET, root->data);
    else
        printf(ANSI_COLOR_GRAY "<%d> " ANSI_COLOR_RESET, root->data);
    inorder(root->right);
}

/**
 * @brief Prints the tree nodes in post-order traversal (Left -> Right -> Root).
 * @param root A pointer to the root of the tree.
 */
void postorder(const RedBlackTree *root)
{
    if (root == NIL)
        return;
    postorder(root->left);
    postorder(root->right);
    if (root->color == RED)
        printf(ANSI_COLOR_RED "<%d> " ANSI_COLOR_RESET, root->data);
    else
        printf(ANSI_COLOR_GRAY "<%d> " ANSI_COLOR_RESET, root->data);
}

/**
 * @brief Prints the tree nodes in level-order traversal (Breadth-First Search).
 * @param root A pointer to the root of the tree.
 */
void levelorder(RedBlackTree *root)
{
    if (root == NIL)
        return;
    int nodes = size(root);
    RedBlackTree **queue = (RedBlackTree **)malloc(nodes * sizeof(RedBlackTree *));
    if (queue == NULL)
        return;
    int front = 0, rear = 0;
    queue[rear++] = root;
    while (front < rear)
    {
        root = queue[front++];
        if (root->left != NIL)
            queue[rear++] = root->left;
        if (root->right != NIL)
            queue[rear++] = root->right;
        if (root->color == RED)
            printf(ANSI_COLOR_RED "<%d> " ANSI_COLOR_RESET, root->data);
        else
            printf(ANSI_COLOR_GRAY "<%d> " ANSI_COLOR_RESET, root->data);
    }
}

// ------------------------- Type Checks ------------------------------

/**
 * @brief Checks if the tree is a perfect binary tree.
 * @param root A pointer to the root of the tree.
 * @return true if the tree is perfect, false otherwise.
 */
bool isPerfect(const RedBlackTree *root)
{
    if (root == NIL)
        return true;
    int levels = height(root);
    int nodes = size(root);
    return (nodes == (int)pow(2, levels + 1) - 1);
}

/**
 * @brief Checks if the tree is a complete binary tree.
 * @param root A pointer to the root of the tree.
 * @return true if the tree is complete, false otherwise.
 */
bool isComplete(RedBlackTree *root)
{
    if (root == NIL)
        return true;

    RedBlackTree **queue = (RedBlackTree **)malloc(size(root) * sizeof(RedBlackTree *));
    if (queue == NULL)
        return false;

    int front = 0, rear = 0;
    bool nullFound = false;
    queue[rear++] = root;

    while (front < rear)
    {
        RedBlackTree *curr = queue[front++];

        if (curr->left == NIL)
            nullFound = true;
        else
        {
            if (nullFound)
            {
                free(queue);
                return false;
            }
            queue[rear++] = curr->left;
        }

        if (curr->right == NIL)
            nullFound = true;
        else
        {
            if (nullFound)
            {
                free(queue);
                return false;
            }
            queue[rear++] = curr->right;
        }
    }

    free(queue);
    return true;
}

/**
 * @brief Checks if the tree is a full binary tree.
 * @param root A pointer to the root of the tree.
 * @return true if the tree is full, false otherwise.
 */
bool isFull(const RedBlackTree *root)
{
    if (root == NIL)
        return true;
    if ((root->left == NIL) ^ (root->right == NIL))
        return false;
    return (isFull(root->left) && isFull(root->right));
}

/**
 * @brief Helper function to check if two trees are mirror images of each other.
 * @param root1 A pointer to the root of the first tree.
 * @param root2 A pointer to the root of the second tree.
 * @return true if they are mirrors, false otherwise.
 */
bool __mirror__(const RedBlackTree *root1, const RedBlackTree *root2)
{
    if (root1 == NIL && root2 == NIL)
        return true;
    else if (root1 == NIL || root2 == NIL)
        return false;
    else
        return (__mirror__(root1->left, root2->right) && __mirror__(root1->right, root2->left));
}

/**
 * @brief Checks if the tree is symmetric.
 * @param root A pointer to the root of the tree.
 * @return true if the tree is symmetric, false otherwise.
 */
bool isSymmetric(const RedBlackTree *root)
{
    if (root == NIL)
        return true;
    return __mirror__(root->left, root->right);
}

#endif // RED_BLACK_TREE_H