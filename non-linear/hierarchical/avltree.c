#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct AVLTree
{
    int data;
    struct AVLTree *left;
    struct AVLTree *right;
    int height;
} AVLTree;

AVLTree *init(int value)
{
    AVLTree *node = (AVLTree *)malloc(sizeof(AVLTree));
    node->data = value;
    node->left = NULL;
    node->right = NULL;
    node->height = 0;
    return node;
}

AVLTree *copy(AVLTree *root)
{
    if (root == NULL)
        return NULL;
    AVLTree *new = init(root->data);
    new->left = copy(root->left);
    new->right = copy(root->right);
    new->height = root->height;
    return new;
}

AVLTree *clear(AVLTree *root)
{
    if (root == NULL)
        return NULL;
    root->left = clear(root->left);
    root->right = clear(root->right);
    free(root);
    return NULL;
}

AVLTree *mirror(AVLTree *root)
{
    if (root == NULL)
        return NULL;
    AVLTree *new = init(root->data);
    new->left = mirror(root->right);
    new->right = mirror(root->left);
    new->height = root->height;
    return new; // no longer a valid BST
}

int degree(AVLTree *root)
{
    if (root == NULL)
        return 0;
    return (root->left != NULL) + (root->right != NULL);
}

int size(AVLTree *root)
{
    if (root == NULL)
        return 0;
    return 1 + size(root->left) + size(root->right);
}

int external(AVLTree *root)
{
    if (root == NULL)
        return 0;
    if (root->left == NULL && root->right == NULL)
        return 1;
    return external(root->left) + external(root->right);
}

int internal(AVLTree *root)
{
    if (root == NULL)
        return 0;
    if (root->left == NULL && root->right == NULL)
        return 0;
    return 1 + internal(root->left) + internal(root->right);
}

int height(AVLTree *root)
{
    if (root == NULL)
        return -1;
    return root->height;
}

int depth(AVLTree *root, int value)
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

int balance(AVLTree *root)
{
    if (root == NULL)
        return 0;
    return height(root->left) - height(root->right);
}

AVLTree *search(AVLTree *root, int value)
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

AVLTree *parent(AVLTree *root, int value)
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

AVLTree *sibling(AVLTree *root, int value)
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

AVLTree *minimum(AVLTree *root)
{
    if (root == NULL)
        return NULL;
    while (root->left != NULL)
        root = root->left;
    return root;
}

AVLTree *maximum(AVLTree *root)
{
    if (root == NULL)
        return NULL;
    while (root->right != NULL)
        root = root->right;
    return root;
}

AVLTree *successor(AVLTree *root, int value)
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

AVLTree *predecessor(AVLTree *root, int value)
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

AVLTree *ceiling(AVLTree *root, float value)
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

AVLTree *floored(AVLTree *root, float value)
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

AVLTree *lowestCommonAncestor(AVLTree *root, int value1, int value2)
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

void path(AVLTree *root, int value1, int value2)
{
    if (search(root, value1) == NULL || search(root, value2) == NULL)
        return;
    AVLTree *lca = lowestCommonAncestor(root, value1, value2);
    int dist1 = depth(lca, value1);
    int dist2 = depth(lca, value2);
    int length = dist1 + dist2 + 1;
    int sequence[length];
    AVLTree *temp1 = lca;
    for (int i = dist1 - 1; i >= 0; i--)
    {
        if (value1 < temp1->data)
            temp1 = temp1->left;
        else
            temp1 = temp1->right;
        sequence[i] = temp1->data;
    }
    sequence[dist1] = lca->data;
    AVLTree *temp2 = lca;
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

void preorder(AVLTree *root)
{
    if (root == NULL)
        return;
    printf("<%d> ", root->data);
    preorder(root->left);
    preorder(root->right);
}

void inorder(AVLTree *root)
{
    if (root == NULL)
        return;
    inorder(root->left);
    printf("<%d> ", root->data);
    inorder(root->right);
}

void postorder(AVLTree *root)
{
    if (root == NULL)
        return;
    postorder(root->left);
    postorder(root->right);
    printf("<%d> ", root->data);
}

// void printLevel(AVLTree *root, int level)
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

// void levelorder(AVLTree *root)
// {
//     int levels = height(root);
//     for (int i = 0; i <= levels; i++)
//         printLevel(root, i);
// }

// void preorder(AVLTree *root)
// {
//     if (root == NULL)
//         return;
//     int nodes = size(root);
//     AVLTree *stack[nodes];
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

// void inorder(AVLTree *root)
// {
//     if (root == NULL)
//         return;
//     int nodes = size(root);
//     AVLTree *stack[nodes];
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

// void postorder(AVLTree *root)
// {
//     if (root == NULL)
//         return;
//     int nodes = size(root);
//     AVLTree *stack1[nodes];
//     AVLTree *stack2[nodes];
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

void levelorder(AVLTree *root)
{
    if (root == NULL)
        return;
    int nodes = size(root);
    AVLTree *queue[nodes];
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

bool isPerfect(AVLTree *root)
{
    int levels = height(root);
    int nodes = size(root);
    return (nodes == (int)pow(2, levels + 1) - 1);
}

bool isComplete(AVLTree *root)
{
    if (root == NULL)
        return true;
    int nodes = size(root);
    AVLTree *queue[nodes];
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
    free(queue);
    return true;
}

bool isFull(AVLTree *root)
{
    if (root == NULL)
        return true;
    if ((root->left == NULL) ^ (root->right == NULL))
        return false;
    return (isFull(root->left) && isFull(root->right));
}

bool areMirrored(AVLTree *root1, AVLTree *root2)
{
    if (root1 == NULL && root2 == NULL)
        return true;
    else if (root1 == NULL || root2 == NULL)
        return false;
    else
        return (areMirrored(root1->left, root2->right) && areMirrored(root1->right, root2->left));
}

bool isSymmetric(AVLTree *root)
{
    if (root == NULL)
        return true;
    return areMirrored(root->left, root->right);
}

AVLTree *leftRotate(AVLTree *x)
{
    AVLTree *y = x->right;
    x->right = y->left;
    y->left = x;
    x->height = 1 + ((height(x->left) > height(x->right)) ? height(x->left) : height(x->right));
    y->height = 1 + ((height(y->left) > height(y->right)) ? height(y->left) : height(y->right));
    return y;
}

AVLTree *rightRotate(AVLTree *x)
{
    AVLTree *y = x->left;
    x->left = y->right;
    y->right = x;
    x->height = 1 + ((height(x->left) > height(x->right)) ? height(x->left) : height(x->right));
    y->height = 1 + ((height(y->left) > height(y->right)) ? height(y->left) : height(y->right));
    return y;
}

AVLTree *insert(AVLTree *root, int value)
{
    if (root == NULL)
    {
        AVLTree *node = init(value);
        return node;
    }
    if (value < root->data)
        root->left = insert(root->left, value);
    else if (value > root->data)
        root->right = insert(root->right, value);
    else
        return root;
    root->height = 1 + ((height(root->left) > height(root->right)) ? height(root->left) : height(root->right));
    int balanceFactor = balance(root);
    if (balanceFactor > 1) // Left Heavy
    {
        if (balance(root->left) >= 0)
            return rightRotate(root); // Left Left
        else
        {
            root->left = leftRotate(root->left);
            return rightRotate(root); // Left Right
        }
    }
    if (balanceFactor < -1) // Right Heavy
    {
        if (balance(root->right) <= 0)
            return leftRotate(root); // Right Right
        else
        {
            root->right = rightRotate(root->right);
            return leftRotate(root); // Right Left
        }
    }
    return root;
}

AVLTree *discard(AVLTree *root, int value)
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
    root->height = 1 + ((height(root->left) > height(root->right)) ? height(root->left) : height(root->right));
    int balanceFactor = balance(root);
    if (balanceFactor > 1) // Left Heavy
    {
        if (balance(root->left) >= 0)
            return rightRotate(root); // Left Left
        else
        {
            root->left = leftRotate(root->left);
            return rightRotate(root); // Left Right
        }
    }
    if (balanceFactor < -1) // Right Heavy
    {
        if (balance(root->right) <= 0)
            return leftRotate(root); // Right Right
        else
        {
            root->right = rightRotate(root->right);
            return leftRotate(root); // Right Left
        }
    }
    return root;
}

int main()
{
    AVLTree *tree = NULL;

    printf("Inserting values into AVL Tree:\n");
    int values[] = {10, 20, 30, 40, 50, 60};
    int n = sizeof(values) / sizeof(values[0]);
    for (int i = 0; i < n; i++)
    {
        tree = insert(tree, values[i]);
    }

    printf("Inorder traversal:\n");
    inorder(tree);
    printf("\n");

    printf("Preorder traversal:\n");
    preorder(tree);
    printf("\n");

    printf("Postorder traversal:\n");
    postorder(tree);
    printf("\n");

    printf("Level order traversal:\n");
    levelorder(tree);
    printf("\n");

    printf("Size of tree: %d\n", size(tree));
    printf("Height of tree: %d\n", height(tree));
    printf("Internal nodes: %d\n", internal(tree));
    printf("External nodes (leaves): %d\n", external(tree));

    printf("Minimum value: %d\n", minimum(tree)->data);
    printf("Maximum value: %d\n", maximum(tree)->data);

    int key = 30;
    printf("Depth of %d: %d\n", key, depth(tree, key));
    printf("Parent of %d: %d\n", key, parent(tree, key)->data);
    printf("Successor of %d: %d\n", key, successor(tree, key)->data);
    printf("Predecessor of %d: %d\n", key, predecessor(tree, key)->data);
    printf("Ceiling of 37: %d\n", ceiling(tree, 37)->data);
    printf("Floor of 37: %d\n", floored(tree, 37)->data);

    printf("Is tree perfect? %s\n", isPerfect(tree) ? "Yes" : "No");
    printf("Is tree complete? %s\n", isComplete(tree) ? "Yes" : "No");
    printf("Is tree full? %s\n", isFull(tree) ? "Yes" : "No");
    printf("Is tree symmetric? %s\n", isSymmetric(tree) ? "Yes" : "No");

    AVLTree *copyTree = copy(tree);
    printf("Copied tree inorder:\n");
    inorder(copyTree);
    printf("\n");

    AVLTree *mirrorTree = mirror(tree);
    printf("Mirrored tree inorder:\n");
    inorder(mirrorTree);
    printf("\n");

    printf("Path between 30 and 40:\n");
    path(tree, 30, 40);
    printf("\n");

    printf("Deleting 50 and 60 from tree:\n");
    tree = discard(tree, 50);
    tree = discard(tree, 60);
    levelorder(tree);
    printf("\n");

    printf("Clearing tree:\n");
    tree = clear(tree);
    printf("Tree cleared.\n");

    return 0;
}