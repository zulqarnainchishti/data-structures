#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct BinarySearchTree
{
    int data;
    struct BinarySearchTree *left;
    struct BinarySearchTree *right;
} BinarySearchTree;

BinarySearchTree *init(int value)
{
    BinarySearchTree *node = (BinarySearchTree *)malloc(sizeof(BinarySearchTree));
    node->data = value;
    node->left = NULL;
    node->right = NULL;
    return node;
}

BinarySearchTree *copy(BinarySearchTree *root)
{
    if (root == NULL)
        return NULL;
    BinarySearchTree *new = init(root->data);
    new->left = copy(root->left);
    new->right = copy(root->right);
    return new;
}

BinarySearchTree *clear(BinarySearchTree *root)
{
    if (root == NULL)
        return NULL;
    root->left = clear(root->left);
    root->right = clear(root->right);
    free(root);
    return NULL;
}

BinarySearchTree *mirror(BinarySearchTree *root)
{
    if (root == NULL)
        return NULL;
    BinarySearchTree *new = init(root->data);
    new->left = mirror(root->right);
    new->right = mirror(root->left);
    return new; // no longer a valid BST
}

BinarySearchTree *insert(BinarySearchTree *root, int value)
{
    if (root == NULL)
    {
        BinarySearchTree *node = init(value);
        return node;
    }
    if (value < root->data)
        root->left = insert(root->left, value);
    else if (value > root->data)
        root->right = insert(root->right, value);
    return root;
}

BinarySearchTree *discard(BinarySearchTree *root, int value)
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
            BinarySearchTree *temp = (root->left == NULL) ? root->right : root->left;
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

int degree(BinarySearchTree *root)
{
    if (root == NULL)
        return 0;
    return (root->left != NULL) + (root->right != NULL);
}

int size(BinarySearchTree *root)
{
    if (root == NULL)
        return 0;
    return 1 + size(root->left) + size(root->right);
}

int external(BinarySearchTree *root)
{
    if (root == NULL)
        return 0;
    if (root->left == NULL && root->right == NULL)
        return 1;
    return external(root->left) + external(root->right);
}

int internal(BinarySearchTree *root)
{
    if (root == NULL)
        return 0;
    if (root->left == NULL && root->right == NULL)
        return 0;
    return 1 + internal(root->left) + internal(root->right);
}

int height(BinarySearchTree *root)
{
    if (root == NULL)
        return -1;
    int leftHeight = height(root->left);
    int rightHeight = height(root->right);
    return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
}

int depth(BinarySearchTree *root, int value)
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

BinarySearchTree *search(BinarySearchTree *root, int value)
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

BinarySearchTree *parent(BinarySearchTree *root, int value)
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

BinarySearchTree *sibling(BinarySearchTree *root, int value)
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

BinarySearchTree *minimum(BinarySearchTree *root)
{
    if (root == NULL)
        return NULL;
    while (root->left != NULL)
        root = root->left;
    return root;
}

BinarySearchTree *maximum(BinarySearchTree *root)
{
    if (root == NULL)
        return NULL;
    while (root->right != NULL)
        root = root->right;
    return root;
}

BinarySearchTree *successor(BinarySearchTree *root, int value)
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

BinarySearchTree *predecessor(BinarySearchTree *root, int value)
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

BinarySearchTree *ceiling(BinarySearchTree *root, float value)
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

BinarySearchTree *floored(BinarySearchTree *root, float value)
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

BinarySearchTree *lowestCommonAncestor(BinarySearchTree *root, int value1, int value2)
{
    if (root == NULL)
        return NULL;
    if (value1 < root->data && value2 < root->data)
        return lowestCommonAncestor(root->left, value1, value2);
    else if (value1 > root->data && value2 > root->data)
        return lowestCommonAncestor(root->right, value1, value2);
    else
        return root;
}

void path(BinarySearchTree *root, int value1, int value2)
{
    if (search(root, value1) == NULL || search(root, value2) == NULL)
        return;
    BinarySearchTree *lca = lowestCommonAncestor(root, value1, value2);
    int dist1 = depth(lca, value1);
    int dist2 = depth(lca, value2);
    int length = dist1 + dist2 + 1;
    int sequence[length];
    BinarySearchTree *temp1 = lca;
    for (int i = dist1 - 1; i >= 0; i--)
    {
        if (value1 < temp1->data)
            temp1 = temp1->left;
        else
            temp1 = temp1->right;
        sequence[i] = temp1->data;
    }
    sequence[dist1] = lca->data;
    BinarySearchTree *temp2 = lca;
    for (int i = 1; i <= dist2; i++)
    {
        if (value2 < temp2->data)
            temp2 = temp2->left;
        else
            temp2 = temp2->right;
        sequence[dist1 + i] = temp2->data;
    }
    for (int i = 0; i < length; i++)
        printf("<%d> ", sequence[i]);
}

void preorder(BinarySearchTree *root)
{
    if (root == NULL)
        return;
    printf("<%d> ", root->data);
    preorder(root->left);
    preorder(root->right);
}

void inorder(BinarySearchTree *root)
{
    if (root == NULL)
        return;
    inorder(root->left);
    printf("<%d> ", root->data);
    inorder(root->right);
}

void postorder(BinarySearchTree *root)
{
    if (root == NULL)
        return;
    postorder(root->left);
    postorder(root->right);
    printf("<%d> ", root->data);
}

// void printLevel(BinarySearchTree *root, int level)
// {
//     if (root == NULL)
//         return;
//     if (level == 0)
//         printf("<%d> ", root->data);
//     else
//     {
//         printLevel(root->left, level - 1);
//         printLevel(root->right, level - 1);
//     }
// }

// void levelorder(BinarySearchTree *root)
// {
//     int levels = height(root);
//     for (int i = 0; i <= levels; i++)
//         printLevel(root, i);
// }

// void preorder(BinarySearchTree *root)
// {
//     if (root == NULL)
//         return;
//     int nodes = size(root);
//     BinarySearchTree *stack[nodes];
//     int top = 0;
//     stack[top++] = root;
//     while (top > 0)
//     {
//         root = stack[--top];
//         printf("<%d> ", root->data);
//         if (root->right != NULL)
//             stack[top++] = root->right;
//         if (root->left != NULL)
//             stack[top++] = root->left;
//     }
// }

// void inorder(BinarySearchTree *root)
// {
//     if (root == NULL)
//         return;
//     int nodes = size(root);
//     BinarySearchTree *stack[nodes];
//     int top = 0;
//     while (root != NULL || top > 0)
//     {
//         while (root != NULL)
//         {
//             stack[top++] = root;
//             root = root->left;
//         }
//         root = stack[--top];
//         printf("<%d> ", root->data);
//         root = root->right;
//     }
// }

// void postorder(BinarySearchTree *root)
// {
//     if (root == NULL)
//         return;
//     int nodes = size(root);
//     BinarySearchTree *stack1[nodes];
//     BinarySearchTree *stack2[nodes];
//     int top1 = 0, top2 = 0;
//     stack1[top1++] = root;
//     while (top1 > 0)
//     {
//         root = stack1[--top1];
//         stack2[top2++] = root;
//         if (root->left != NULL)
//             stack1[top1++] = root->left;
//         if (root->right != NULL)
//             stack1[top1++] = root->right;
//     }
//     while (top2 > 0)
//     {
//         root = stack2[--top2];
//         printf("<%d> ", root->data);
//     }
// }

void levelorder(BinarySearchTree *root)
{
    if (root == NULL)
        return;
    int nodes = size(root);
    BinarySearchTree *queue[nodes];
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
}

bool isPerfect(BinarySearchTree *root)
{
    int levels = height(root);
    int nodes = size(root);
    return (nodes == (int)pow(2, levels + 1) - 1);
}

bool isComplete(BinarySearchTree *root)
{
    if (root == NULL)
        return true;
    int nodes = size(root);
    BinarySearchTree *queue[nodes];
    int front = 0, rear = 0;
    bool nullFound = false;
    queue[rear++] = root;
    while (front < rear)
    {
        root = queue[front++];
        if (root->left != NULL)
        {
            if (nullFound)
            {
                free(queue);
                return false;
            }
            queue[rear++] = root->left;
        }
        else
            nullFound = true;
        if (root->right != NULL)
        {
            if (nullFound)
            {
                free(queue);
                return false;
            }
            queue[rear++] = root->right;
        }
        else
            nullFound = true;
    }
    return true;
}

bool isFull(BinarySearchTree *root)
{
    if (root == NULL)
        return true;
    if ((root->left == NULL) ^ (root->right == NULL))
        return false;
    return (isFull(root->left) && isFull(root->right));
}

bool isDegenerate(BinarySearchTree *root)
{
    if (root == NULL)
        return true;
    if (root->left != NULL && root->right != NULL)
        return false;
    return (isDegenerate(root->left) && isDegenerate(root->right));
}

bool isSkewed(BinarySearchTree *root)
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

bool areMirrored(BinarySearchTree *root1, BinarySearchTree *root2)
{
    if (root1 == NULL && root2 == NULL)
        return true;
    else if (root1 == NULL || root2 == NULL)
        return false;
    else
        return (areMirrored(root1->left, root2->right) && areMirrored(root1->right, root2->left));
}

bool isSymmetric(BinarySearchTree *root)
{
    if (root == NULL)
        return true;
    return areMirrored(root->left, root->right);
}

bool isBalanced(BinarySearchTree *root, int *currHeight)
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

int main()
{
    BinarySearchTree *root = NULL;

    // Insertion
    root = insert(root, 5);
    root = insert(root, 2);
    root = insert(root, 7);
    root = insert(root, 1);
    root = insert(root, 4);
    root = insert(root, 6);
    root = insert(root, 3);

    printf("Inorder: ");
    inorder(root);
    printf("\n");

    printf("Preorder: ");
    preorder(root);
    printf("\n");

    printf("Postorder: ");
    postorder(root);
    printf("\n");

    printf("Levelorder: ");
    levelorder(root);
    printf("\n");

    // Size, Height, Degree
    printf("Size: %d\n", size(root));
    printf("Height: %d\n", height(root));
    printf("Degree of root: %d\n", degree(root));

    // External/Internal nodes
    printf("External nodes: %d\n", external(root));
    printf("Internal nodes: %d\n", internal(root));

    // Search, Parent, Sibling
    printf("Search 4: %d\n", 4 == search(root, 4)->data);
    printf("Parent of 4: %d\n", parent(root, 4)->data);
    printf("Sibling of 4: %d\n", sibling(root, 4)->data);

    // Min/Max
    printf("Minimum: %d\n", minimum(root)->data);
    printf("Maximum: %d\n", maximum(root)->data);

    // Successor/Predecessor
    printf("Successor of 4: %d\n", successor(root, 4)->data);
    printf("Predecessor of 4: %d\n", predecessor(root, 4)->data);

    // Ceiling / Floor
    printf("Ceiling of 4: %d\n", ceiling(root, 4.5)->data);
    printf("Floor of 4: %d\n", floored(root, 4.5)->data);

    // LCA
    printf("LCA of 1 and 3: %d\n", lowestCommonAncestor(root, 1, 3)->data);

    // Path between nodes
    printf("Path between 6 and 3: ");
    path(root, 6, 3);
    printf("\n");

    // Mirror
    BinarySearchTree *mirrored = mirror(root);
    printf("Inorder of mirrored tree: ");
    inorder(mirrored);
    printf("\n");

    // Tree Properties
    printf("Is Perfect? %d\n", isPerfect(root));
    printf("Is Complete? %d\n", isComplete(root));
    printf("Is Full? %d\n", isFull(root));
    printf("Is Degenerate? %d\n", isDegenerate(root));
    printf("Is Skewed? %d\n", isSkewed(root));
    printf("Is Symmetric? %d\n", isSymmetric(root));

    int currHeight;
    printf("Is Balanced? %d\n", isBalanced(root, &currHeight));

    // Copy
    BinarySearchTree *copied = copy(root);
    printf("Inorder of copied tree: ");
    inorder(copied);
    printf("\n");

    // Clear
    root = clear(root);
    mirrored = clear(mirrored);
    copied = clear(copied);
    printf("After clearing, size of original tree: %d\n", size(root));

    return 0;
}