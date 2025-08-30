#ifndef BST_H
#define BST_H

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * @struct BinarySearchTree
 * @brief Represents a node in the Binary Search Tree.
 */
typedef struct BinarySearchTree
{
    int data;
    struct BinarySearchTree *left;
    struct BinarySearchTree *right;
} BinarySearchTree;

// ------------------------- Core Operations -------------------------------

/**
 * @brief Initializes a new BST node.
 * @param value The integer value to store in the new node.
 * @return A pointer to the newly created node, or NULL if memory allocation fails.
 */
BinarySearchTree *init(const int value)
{
    BinarySearchTree *node = (BinarySearchTree *)malloc(sizeof(BinarySearchTree));
    if (node == NULL)
        return NULL;
    node->data = value;
    node->left = NULL;
    node->right = NULL;
    return node;
}

/**
 * @brief Destroys the entire BST and deallocates memory.
 * @param root A pointer to the root of the tree.
 * @return Always NULL after destruction.
 */
BinarySearchTree *destroy(BinarySearchTree *root)
{
    if (root == NULL)
        return NULL;
    root->left = destroy(root->left);
    root->right = destroy(root->right);
    free(root);
    return NULL;
}

/**
 * @brief Creates a deep copy of a BST.
 * @param root A pointer to the root of the tree to be copied.
 * @return A pointer to the root of the new, copied tree.
 */
BinarySearchTree *copy(const BinarySearchTree *root)
{
    if (root == NULL)
        return NULL;
    BinarySearchTree *copied = init(root->data);
    copied->left = copy(root->left);
    copied->right = copy(root->right);
    return copied;
}

/**
 * @brief Inserts a new value into the BST.
 * @param root A pointer to the root of the tree.
 * @param value The value to insert.
 * @return A pointer to the root of the modified tree.
 */
BinarySearchTree *insert(BinarySearchTree *root, const int value)
{
    if (root == NULL)
        return init(value);
    if (value < root->data)
        root->left = insert(root->left, value);
    else if (value > root->data)
        root->right = insert(root->right, value);
    return root;
}

/**
 * @brief Deletes a node with the specified value from the BST.
 * @param root A pointer to the root of the tree.
 * @param value The value of the node to be deleted.
 * @return A pointer to the root of the modified tree.
 */
BinarySearchTree *discard(BinarySearchTree *root, const int value)
{
    if (root == NULL)
        return NULL;
    if (value < root->data)
        root->left = discard(root->left, value);
    else if (value > root->data)
        root->right = discard(root->right, value);
    else
    {
        if (root->left == NULL)
        {
            BinarySearchTree *temp = root->right;
            free(root);
            return temp;
        }
        else if (root->right == NULL)
        {
            BinarySearchTree *temp = root->left;
            free(root);
            return temp;
        }
        else
        {
            BinarySearchTree *succ = root->right;
            while (succ->left != NULL)
                succ = succ->left;
            root->data = succ->data;
            root->right = discard(root->right, succ->data);
        }
    }
    return root;
}

// ------------------------- Search Operations -----------------------------

/**
 * @brief Searches for a node with a specific value.
 * @param root A pointer to the root of the tree.
 * @param value The value to search for.
 * @return A pointer to the found node, or NULL if not found.
 */
BinarySearchTree *search(BinarySearchTree *root, const int value)
{
    while (root != NULL)
    {
        if (value == root->data)
            return root;
        else if (value < root->data)
            root = root->left;
        else
            root = root->right;
    }
    return NULL;
}

/**
 * @brief Finds the parent of a node with a specific value.
 * @param root A pointer to the root of the tree.
 * @param value The value of the node whose parent is to be found.
 * @return A pointer to the parent node, or NULL if the node is the root or not found.
 */
BinarySearchTree *parent(BinarySearchTree *root, const int value)
{
    BinarySearchTree *prev = NULL;
    while (root != NULL)
    {
        if (value == root->data)
            return prev;
        prev = root;
        if (value < root->data)
            root = root->left;
        else
            root = root->right;
    }
    return NULL;
}

/**
 * @brief Finds the sibling of a node.
 * @param root A pointer to the root of the tree.
 * @param value The value of the node whose sibling is to be found.
 * @return A pointer to the sibling node, or NULL if the node is the root or has no sibling.
 */
BinarySearchTree *sibling(BinarySearchTree *root, const int value)
{
    BinarySearchTree *prev = NULL;
    while (root != NULL)
    {
        if (value == root->data)
        {
            if (prev == NULL)
                return NULL;
            if (prev->left == root)
                return prev->right;
            else
                return prev->left;
        }
        prev = root;
        if (value < root->data)
            root = root->left;
        else
            root = root->right;
    }
    return NULL;
}

/**
 * @brief Finds the node with the minimum value in the tree.
 * @param root A pointer to the root of the tree.
 * @return A pointer to the minimum node, or NULL if the tree is empty.
 */
BinarySearchTree *minimum(BinarySearchTree *root)
{
    if (root == NULL)
        return NULL;
    while (root->left != NULL)
        root = root->left;
    return root;
}

/**
 * @brief Finds the node with the maximum value in the tree.
 * @param root A pointer to the root of the tree.
 * @return A pointer to the maximum node, or NULL if the tree is empty.
 */
BinarySearchTree *maximum(BinarySearchTree *root)
{
    if (root == NULL)
        return NULL;
    while (root->right != NULL)
        root = root->right;
    return root;
}

/**
 * @brief Finds the in-order successor of a node.
 * @param root A pointer to the root of the tree.
 * @param value The value of the node whose successor is to be found.
 * @return A pointer to the successor node, or NULL if no successor exists.
 */
BinarySearchTree *successor(BinarySearchTree *root, const int value)
{
    BinarySearchTree *succ = NULL;
    while (root != NULL)
    {
        if (value == root->data)
        {
            if (root->right != NULL)
                return minimum(root->right);
            else
                return succ;
        }
        else if (value < root->data)
        {
            succ = root;
            root = root->left;
        }
        else
            root = root->right;
    }
    return NULL;
}

/**
 * @brief Finds the in-order predecessor of a node.
 * @param root A pointer to the root of the tree.
 * @param value The value of the node whose predecessor is to be found.
 * @return A pointer to the predecessor node, or NULL if no predecessor exists.
 */
BinarySearchTree *predecessor(BinarySearchTree *root, const int value)
{
    BinarySearchTree *pred = NULL;
    while (root != NULL)
    {
        if (value == root->data)
        {
            if (root->left != NULL)
                return maximum(root->left);
            else
                return pred;
        }
        else if (value > root->data)
        {
            pred = root;
            root = root->right;
        }
        else
            root = root->left;
    }
    return NULL;
}

/**
 * @brief Finds the smallest value greater than or equal to the given value.
 * @param root A pointer to the root of the tree.
 * @param value The value to find the ceiling for.
 * @return A pointer to the ceiling node, or NULL if no such value exists.
 */
BinarySearchTree *ceiling(BinarySearchTree *root, const double value)
{
    BinarySearchTree *next = NULL;
    while (root != NULL)
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
 * @return A pointer to the floor node, or NULL if no such value exists.
 */
BinarySearchTree *floored(BinarySearchTree *root, const double value)
{
    BinarySearchTree *prev = NULL;
    while (root != NULL)
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
int degree(const BinarySearchTree *root)
{
    if (root == NULL)
        return 0;
    return (root->left != NULL) + (root->right != NULL);
}

/**
 * @brief Counts the total number of nodes in the tree.
 * @param root A pointer to the root of the tree.
 * @return The total number of nodes.
 */
int size(const BinarySearchTree *root)
{
    if (root == NULL)
        return 0;
    return 1 + size(root->left) + size(root->right);
}

/**
 * @brief Counts the number of external (leaf) nodes.
 * @param root A pointer to the root of the tree.
 * @return The number of external nodes.
 */
int external(const BinarySearchTree *root)
{
    if (root == NULL)
        return 0;
    if (root->left == NULL && root->right == NULL)
        return 1;
    return external(root->left) + external(root->right);
}

/**
 * @brief Counts the number of internal nodes (nodes with at least one child).
 * @param root A pointer to the root of the tree.
 * @return The number of internal nodes.
 */
int internal(const BinarySearchTree *root)
{
    if (root == NULL)
        return 0;
    if (root->left == NULL && root->right == NULL)
        return 0;
    return 1 + internal(root->left) + internal(root->right);
}

/**
 * @brief Calculates the height of the tree.
 * @param root A pointer to the root of the tree.
 * @return The height of the tree. Returns -1 for an empty tree.
 */
int height(const BinarySearchTree *root)
{
    if (root == NULL)
        return -1;
    int leftHeight = height(root->left);
    int rightHeight = height(root->right);
    return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
}

/**
 * @brief Calculates the depth of a node.
 * @param root A pointer to the root of the tree.
 * @param value The value of the node whose depth is to be found.
 * @return The depth of the node. Returns -1 if the node is not found.
 */
int depth(const BinarySearchTree *root, const int value)
{
    int level = 0;
    while (root != NULL)
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
void preorder(const BinarySearchTree *root)
{
    if (root == NULL)
        return;
    printf("<%d> ", root->data);
    preorder(root->left);
    preorder(root->right);
}

/**
 * @brief Prints the tree nodes in in-order traversal (Left -> Root -> Right).
 * @param root A pointer to the root of the tree.
 */
void inorder(const BinarySearchTree *root)
{
    if (root == NULL)
        return;
    inorder(root->left);
    printf("<%d> ", root->data);
    inorder(root->right);
}

/**
 * @brief Prints the tree nodes in post-order traversal (Left -> Right -> Root).
 * @param root A pointer to the root of the tree.
 */
void postorder(const BinarySearchTree *root)
{
    if (root == NULL)
        return;
    postorder(root->left);
    postorder(root->right);
    printf("<%d> ", root->data);
}

/**
 * @brief Helper function for level-order traversal.
 * @param root A pointer to the root of the tree.
 * @param level The current level to print.
 */
void __level__(const BinarySearchTree *root, const int level)
{
    if (root == NULL)
        return;
    if (level == 0)
        printf("<%d> ", root->data);
    else
    {
        __level__(root->left, level - 1);
        __level__(root->right, level - 1);
    }
}

/**
 * @brief Prints the tree nodes in level-order traversal (Breadth-First Search).
 * @param root A pointer to the root of the tree.
 */
void levelorder(const BinarySearchTree *root)
{
    int levels = height(root);
    for (int i = 0; i <= levels; i++)
        __level__(root, i);
}

// ------------------------- Type Checks ------------------------------

/**
 * @brief Checks if the tree is a perfect binary tree.
 * @param root A pointer to the root of the tree.
 * @return true if the tree is perfect, false otherwise.
 */
bool isPerfect(const BinarySearchTree *root)
{
    if (root == NULL)
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
bool isComplete(BinarySearchTree *root)
{
    if (root == NULL)
        return true;

    BinarySearchTree **queue = (BinarySearchTree **)malloc(size(root) * sizeof(BinarySearchTree *));
    if (queue == NULL)
        return false;

    int front = 0, rear = 0;
    bool nullFound = false;
    queue[rear++] = root;

    while (front < rear)
    {
        BinarySearchTree *current = queue[front++];

        if (current->left == NULL)
            nullFound = true;
        else
        {
            if (nullFound)
            {
                free(queue);
                return false;
            }
            queue[rear++] = current->left;
        }

        if (current->right == NULL)
            nullFound = true;
        else
        {
            if (nullFound)
            {
                free(queue);
                return false;
            }
            queue[rear++] = current->right;
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
bool isFull(const BinarySearchTree *root)
{
    if (root == NULL)
        return true;
    if ((root->left == NULL) ^ (root->right == NULL))
        return false;
    return (isFull(root->left) && isFull(root->right));
}

/**
 * @brief Checks if the tree is a degenerate binary tree.
 * @param root A pointer to the root of the tree.
 * @return true if the tree is degenerate, false otherwise.
 */
bool isDegenerate(const BinarySearchTree *root)
{
    if (root == NULL)
        return true;
    if (root->left != NULL && root->right != NULL)
        return false;
    return (isDegenerate(root->left) && isDegenerate(root->right));
}

/**
 * @brief Checks if the tree is a skewed binary tree.
 * @param root A pointer to the root of the tree.
 * @return true if the tree is skewed, false otherwise.
 */
bool isSkewed(const BinarySearchTree *root)
{
    if (root == NULL)
        return true;
    if (root->left != NULL)
    {
        while (root != NULL)
        {
            if (root->right != NULL)
                return false;
            root = root->left;
        }
    }
    else
    {
        while (root != NULL)
        {
            if (root->left != NULL)
                return false;
            root = root->right;
        }
    }
    return true;
}

/**
 * @brief Helper function to check if two trees are mirror images of each other.
 * @param root1 A pointer to the root of the first tree.
 * @param root2 A pointer to the root of the second tree.
 * @return true if they are mirrors, false otherwise.
 */
bool __mirror__(const BinarySearchTree *root1, const BinarySearchTree *root2)
{
    if (root1 == NULL && root2 == NULL)
        return true;
    else if (root1 == NULL || root2 == NULL)
        return false;
    else
        return (__mirror__(root1->left, root2->right) && __mirror__(root1->right, root2->left));
}

/**
 * @brief Checks if the tree is symmetric.
 * @param root A pointer to the root of the tree.
 * @return true if the tree is symmetric, false otherwise.
 */
bool isSymmetric(const BinarySearchTree *root)
{
    if (root == NULL)
        return true;
    return __mirror__(root->left, root->right);
}

/**
 * @brief Checks if the tree is balanced.
 * @param root A pointer to the root of the tree.
 * @param currHeight A pointer to an integer to store the height of the current subtree.
 * @return true if the tree is balanced, false otherwise.
 */
bool isBalanced(const BinarySearchTree *root, int *currHeight)
{
    if (root == NULL)
    {
        *currHeight = -1;
        return true;
    }
    int leftHeight, rightHeight;
    if (!isBalanced(root->left, &leftHeight) || !isBalanced(root->right, &rightHeight))
        return false;
    int balanceFactor = leftHeight - rightHeight;
    if (abs(balanceFactor) > 1)
        return false;
    *currHeight = 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
    return true;
}

#endif // BST_H