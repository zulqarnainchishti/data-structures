#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * @struct AVLTree
 * @brief Represents a node in the AVL Tree.
 * @var data The integer value stored in the node.
 * @var left Pointer to the left child.
 * @var right Pointer to the right child.
 * @var height The height of the node's subtree.
 */
typedef struct AVLTree
{
    int data;
    struct AVLTree *left;
    struct AVLTree *right;
    int height;
} AVLTree;

// ------------------------- Core Operations -------------------------------

/**
 * @brief Initializes a new AVL tree node.
 * @param value The integer value to store in the new node.
 * @return A pointer to the newly created node, or NULL if memory allocation fails.
 */
AVLTree *init(const int value)
{
    AVLTree *node = (AVLTree *)malloc(sizeof(AVLTree));
    if (node == NULL)
    {
        perror("Failed to initialize Node.");
        exit(EXIT_FAILURE);
    }
    node->data = value;
    node->left = NULL;
    node->right = NULL;
    node->height = 0;
    return node;
}

/**
 * @brief Destroys the entire AVL tree and deallocates memory.
 * @param root A pointer to the root of the tree.
 * @return Always NULL after destruction.
 */
void destroy(AVLTree *root)
{
    if (root == NULL)
        return;
    destroy(root->left);
    destroy(root->right);
    free(root);
}

/**
 * @brief Creates a deep copy of an AVL tree.
 * @param root A pointer to the root of the tree to be copied.
 * @return A pointer to the root of the new, copied tree.
 */
AVLTree *copy(const AVLTree *root)
{
    if (root == NULL)
        return NULL;
    AVLTree *copied = init(root->data);
    copied->left = copy(root->left);
    copied->right = copy(root->right);
    copied->height = root->height;
    return copied;
}

/**
 * @brief Calculates the height of a node.
 * @note This is a helper function to avoid direct access to the height member.
 * @param root A pointer to the node.
 * @return The height of the node's subtree. Returns -1 for a NULL node.
 */
int height(const AVLTree *root)
{
    if (root == NULL)
        return -1;
    return root->height;
}

/**
 * @brief Calculates the balance factor of a node.
 * @note A positive value indicates a left-heavy subtree, and a negative value indicates a right-heavy subtree.
 * @param root A pointer to the node.
 * @return The balance factor.
 */
int balance(const AVLTree *root)
{
    if (root == NULL)
        return 0;
    return height(root->left) - height(root->right);
}

/**
 * @brief Performs a left rotation on a subtree rooted with x.
 * @param x A pointer to the root of the subtree to be rotated.
 * @return The new root of the rotated subtree.
 */
AVLTree *__leftRotate__(AVLTree *x)
{
    AVLTree *y = x->right;
    x->right = y->left;
    y->left = x;
    x->height = 1 + ((height(x->left) > height(x->right)) ? height(x->left) : height(x->right));
    y->height = 1 + ((height(y->left) > height(y->right)) ? height(y->left) : height(y->right));
    return y;
}

/**
 * @brief Performs a right rotation on a subtree rooted with x.
 * @param x A pointer to the root of the subtree to be rotated.
 * @return The new root of the rotated subtree.
 */
AVLTree *__rightRotate__(AVLTree *x)
{
    AVLTree *y = x->left;
    x->left = y->right;
    y->right = x;
    x->height = 1 + ((height(x->left) > height(x->right)) ? height(x->left) : height(x->right));
    y->height = 1 + ((height(y->left) > height(y->right)) ? height(y->left) : height(y->right));
    return y;
}

/**
 * @brief Rebalances the AVL tree after insertion or deletion.
 * @param root A pointer to the root of the subtree to be rebalanced.
 * @return The new root of the rebalanced subtree.
 */
AVLTree *__rebalance__(AVLTree *root)
{
    int balanceFactor = balance(root);
    if (balanceFactor > 1) // Left Heavy
    {
        if (balance(root->left) >= 0)
            return __rightRotate__(root); // Left Left Case
        else
        {
            root->left = __leftRotate__(root->left);
            return __rightRotate__(root); // Left Right Case
        }
    }
    if (balanceFactor < -1) // Right Heavy
    {
        if (balance(root->right) <= 0)
            return __leftRotate__(root); // Right Right Case
        else
        {
            root->right = __rightRotate__(root->right);
            return __leftRotate__(root); // Right Left Case
        }
    }
    return root;
}

/**
 * @brief Inserts a new value into the AVL tree.
 * @param root A pointer to the root of the tree.
 * @param value The value to insert.
 * @return A pointer to the root of the modified tree.
 */
AVLTree *insert(AVLTree *root, const int value)
{
    if (root == NULL)
        return init(value);
    if (value < root->data)
        root->left = insert(root->left, value);
    else if (value > root->data)
        root->right = insert(root->right, value);
    else
        return root;
    root->height = 1 + ((height(root->left) > height(root->right)) ? height(root->left) : height(root->right));
    return __rebalance__(root);
}

/**
 * @brief Deletes a node with the specified value from the AVL tree.
 * @param root A pointer to the root of the tree.
 * @param value The value of the node to be deleted.
 * @return A pointer to the root of the modified tree.
 */
AVLTree *discard(AVLTree *root, const int value)
{
    if (root == NULL)
        return NULL;
    if (value < root->data)
        root->left = discard(root->left, value);
    else if (value > root->data)
        root->right = discard(root->right, value);
    else
    {
        if (root->left == NULL && root->right == NULL)
        {
            free(root);
            return NULL;
        }
        else if (root->left == NULL || root->right == NULL)
        {
            AVLTree *temp = (root->left == NULL) ? root->right : root->left;
            free(root);
            return temp;
        }
        else
        {
            AVLTree *succ = root->right;
            while (succ->left != NULL)
                succ = succ->left;
            root->data = succ->data;
            root->right = discard(root->right, succ->data);
        }
    }
    if (root == NULL)
        return root; // Case where the root becomes NULL after deletion
    root->height = 1 + ((height(root->left) > height(root->right)) ? height(root->left) : height(root->right));
    return __rebalance__(root);
}

// ------------------------- Search Operations -----------------------------

/**
 * @brief Searches for a node with a specific value.
 * @param root A pointer to the root of the tree.
 * @param value The value to search for.
 * @return A pointer to the found node, or NULL if not found.
 */
AVLTree *search(AVLTree *root, const int value)
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
AVLTree *parent(AVLTree *root, const int value)
{
    AVLTree *prev = NULL;
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
AVLTree *sibling(AVLTree *root, const int value)
{
    AVLTree *prev = NULL;
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
AVLTree *minimum(AVLTree *root)
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
AVLTree *maximum(AVLTree *root)
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
AVLTree *successor(AVLTree *root, const int value)
{
    AVLTree *succ = NULL;
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
AVLTree *predecessor(AVLTree *root, const int value)
{
    AVLTree *pred = NULL;
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
AVLTree *ceiling(AVLTree *root, const double value)
{
    AVLTree *next = NULL;
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
AVLTree *floored(AVLTree *root, const double value)
{
    AVLTree *prev = NULL;
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
int degree(const AVLTree *root)
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
int size(const AVLTree *root)
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
int external(const AVLTree *root)
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
int internal(const AVLTree *root)
{
    if (root == NULL)
        return 0;
    if (root->left == NULL && root->right == NULL)
        return 0;
    return 1 + internal(root->left) + internal(root->right);
}

/**
 * @brief Calculates the depth of a node.
 * @param root A pointer to the root of the tree.
 * @param value The value of the node whose depth is to be found.
 * @return The depth of the node. Returns -1 if the node is not found.
 */
int depth(const AVLTree *root, int value)
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
void preorder(const AVLTree *root)
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
void inorder(const AVLTree *root)
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
void postorder(const AVLTree *root)
{
    if (root == NULL)
        return;
    postorder(root->left);
    postorder(root->right);
    printf("<%d> ", root->data);
}

/**
 * @brief Prints the tree nodes in level-order traversal (Breadth-First Search).
 * @param root A pointer to the root of the tree.
 */
void levelorder(AVLTree *root)
{
    if (root == NULL)
        return;
    AVLTree **queue = (AVLTree **)malloc(size(root) * sizeof(AVLTree *));
    if (queue == NULL)
        return;
    int front = 0, rear = 0;
    queue[rear++] = root;
    while (front < rear)
    {
        root = queue[front++];
        if (root->left != NULL)
            queue[rear++] = root->left;
        if (root->right != NULL)
            queue[rear++] = root->right;
        printf("<%d> ", root->data);
    }
    free(queue);
}

// ------------------------- Type Checks ------------------------------

/**
 * @brief Checks if the tree is a perfect binary tree.
 * @param root A pointer to the root of the tree.
 * @return true if the tree is perfect, false otherwise.
 */
bool isPerfect(const AVLTree *root)
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
bool isComplete(AVLTree *root)
{
    if (root == NULL)
        return true;

    AVLTree **queue = (AVLTree **)malloc(size(root) * sizeof(AVLTree *));
    if (queue == NULL)
        return false;

    int front = 0, rear = 0;
    bool nullFound = false;
    queue[rear++] = root;

    while (front < rear)
    {
        AVLTree *curr = queue[front++];

        if (curr->left == NULL)
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

        if (curr->right == NULL)
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
bool isFull(const AVLTree *root)
{
    if (root == NULL)
        return true;
    if ((root->left == NULL) ^ (root->right == NULL))
        return false;
    return (isFull(root->left) && isFull(root->right));
}

/**
 * @brief Helper function to check if two trees are mirror images of each other.
 * @param root1 A pointer to the root of the first tree.
 * @param root2 A pointer to the root of the second tree.
 * @return true if they are mirrors, false otherwise.
 */
bool __mirror__(AVLTree *root1, AVLTree *root2)
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
bool isSymmetric(AVLTree *root)
{
    if (root == NULL)
        return true;
    return __mirror__(root->left, root->right);
}

#endif // AVL_TREE_H