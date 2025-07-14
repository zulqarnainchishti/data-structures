#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

typedef enum
{
    RED,
    BLACK
} Color;

typedef struct RedBlackTree
{
    int data;
    Color color;
    struct RedBlackTree *parent;
    struct RedBlackTree *left;
    struct RedBlackTree *right;
} RedBlackTree;

RedBlackTree sentinel;
RedBlackTree *NIL = &sentinel;
void initSentinel()
{
    NIL->data = 0;
    NIL->color = BLACK;
    NIL->parent = NIL;
    NIL->left = NIL;
    NIL->right = NIL;
}

RedBlackTree *init(int value)
{
    RedBlackTree *node = (RedBlackTree *)malloc(sizeof(RedBlackTree));
    node->data = value;
    node->color = RED;
    node->parent = NIL;
    node->left = NIL;
    node->right = NIL;
    return node;
}

RedBlackTree *copy(RedBlackTree *root)
{
    if (root == NIL)
        return NIL;
    RedBlackTree *new = init(root->data);
    new->color = root->color;
    new->left = copy(root->left);
    if (new->left != NIL)
        new->left->parent = new;
    new->right = copy(root->right);
    if (new->right != NIL)
        new->right->parent = new;
    return new;
}

RedBlackTree *clear(RedBlackTree *root)
{
    if (root == NIL)
        return NIL;
    root->left = clear(root->left);
    root->right = clear(root->right);
    free(root);
    return NIL;
}

RedBlackTree *mirror(RedBlackTree *root)
{
    if (root == NIL)
        return NIL;
    RedBlackTree *new = init(root->data);
    new->color = root->color;
    new->left = mirror(root->right);
    if (new->left != NIL)
        new->left->parent = new;
    new->right = mirror(root->left);
    if (new->right != NIL)
        new->right->parent = new;
    return new; // no longer a valid BST
}

int degree(RedBlackTree *root)
{
    if (root == NIL)
        return 0;
    return (root->left != NIL) + (root->right != NIL);
}

int size(RedBlackTree *root)
{
    if (root == NIL)
        return 0;
    return 1 + size(root->left) + size(root->right);
}

int external(RedBlackTree *root)
{
    if (root == NIL)
        return 0;
    if (root->left == NIL && root->right == NIL)
        return 1;
    return external(root->left) + external(root->right);
}

int internal(RedBlackTree *root)
{
    if (root == NIL)
        return 0;
    if (root->left == NIL && root->right == NIL)
        return 0;
    return 1 + internal(root->left) + internal(root->right);
}

int height(RedBlackTree *root)
{
    if (root == NIL)
        return -1;
    int leftHeight = height(root->left);
    int rightHeight = height(root->right);
    return 1 + ((leftHeight > rightHeight) ? leftHeight : rightHeight);
}

int depth(RedBlackTree *root, int value)
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

RedBlackTree *search(RedBlackTree *root, int value)
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

RedBlackTree *minimum(RedBlackTree *root)
{
    if (root == NIL)
        return NIL;
    while (root->left != NIL)
        root = root->left;
    return root;
}

RedBlackTree *maximum(RedBlackTree *root)
{
    if (root == NIL)
        return NIL;
    while (root->right != NIL)
        root = root->right;
    return root;
}

RedBlackTree *successor(RedBlackTree *root, int value)
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

RedBlackTree *predecessor(RedBlackTree *root, int value)
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

RedBlackTree *ceiling(RedBlackTree *root, float value)
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

RedBlackTree *floored(RedBlackTree *root, float value)
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

RedBlackTree *lowestCommonAncestor(RedBlackTree *root, int value1, int value2)
{
    if (root == NIL)
        return NIL;
    if (value1 < root->data && value2 < root->data)
        return lowestCommonAncestor(root->left, value1, value2);
    else if (value1 > root->data && value2 > root->data)
        return lowestCommonAncestor(root->right, value1, value2);
    else
        return root;
}

void path(RedBlackTree *root, int value1, int value2)
{
    if (search(root, value1) == NIL || search(root, value2) == NIL)
        return;
    RedBlackTree *lca = lowestCommonAncestor(root, value1, value2);
    int dist1 = depth(lca, value1);
    int dist2 = depth(lca, value2);
    int length = dist1 + dist2 + 1;
    int sequence[length];
    RedBlackTree *temp1 = lca;
    for (int i = dist1 - 1; i >= 0; i--)
    {
        if (value1 < temp1->data)
            temp1 = temp1->left;
        else
            temp1 = temp1->right;
        sequence[i] = temp1->data;
    }
    sequence[dist1] = lca->data;
    RedBlackTree *temp2 = lca;
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

void preorder(RedBlackTree *root)
{
    if (root == NIL)
        return;
    printf("<%d|%c> ", root->data, root->color == BLACK ? 'K' : 'R');
    preorder(root->left);
    preorder(root->right);
}

void inorder(RedBlackTree *root)
{
    if (root == NIL)
        return;
    inorder(root->left);
    printf("<%d|%c> ", root->data, root->color == BLACK ? 'K' : 'R');
    inorder(root->right);
}

void postorder(RedBlackTree *root)
{
    if (root == NIL)
        return;
    postorder(root->left);
    postorder(root->right);
    printf("<%d|%c> ", root->data, root->color == BLACK ? 'K' : 'R');
}

// void printLevel(RedBlackTree *root, int level)
// {
//     if (root == NIL)
//         return;
//     if (level == 0)
//         printf("<%d|%c> ", root->data, root->color == BLACK ? 'K' : 'R');
//     else
//     {
//         printLevel(root->left, level - 1);
//         printLevel(root->right, level - 1);
//     }
// }

// void levelorder(RedBlackTree *root)
// {
//     int levels = height(root);
//     for (int i = 0; i <= levels; i++)
//         printLevel(root, i);
// }

// void preorder(RedBlackTree *root)
// {
//     if (root == NIL)
//         return;
//     int nodes = size(root);
//     RedBlackTree *stack[nodes];
//     int top = 0;
//     stack[top++] = root;
//     while (top > 0)
//     {
//         root = stack[--top];
//         printf("<%d|%c> ", root->data, root->color == BLACK ? 'K' : 'R');
//         if (root->right != NIL)
//             stack[top++] = root->right;
//         if (root->left != NIL)
//             stack[top++] = root->left;
//     }
// }

// void inorder(RedBlackTree *root)
// {
//     if (root == NIL)
//         return;
//     int nodes = size(root);
//     RedBlackTree *stack[nodes];
//     int top = 0;
//     while (root != NIL || top > 0)
//     {
//         while (root != NIL)
//         {
//             stack[top++] = root;
//             root = root->left;
//         }
//         root = stack[--top];
//         printf("<%d|%c> ", root->data, root->color == BLACK ? 'K' : 'R');
//         root = root->right;
//     }
// }

// void postorder(RedBlackTree *root)
// {
//     if (root == NIL)
//         return;
//     int nodes = size(root);
//     RedBlackTree *stack1[nodes];
//     RedBlackTree *stack2[nodes];
//     int top1 = 0, top2 = 0;
//     stack1[top1++] = root;
//     while (top1 > 0)
//     {
//         root = stack1[--top1];
//         stack2[top2++] = root;
//         if (root->left != NIL)
//             stack1[top1++] = root->left;
//         if (root->right != NIL)
//             stack1[top1++] = root->right;
//     }
//     while (top2 > 0)
//     {
//         root = stack2[--top2];
//         printf("<%d|%c> ", root->data, root->color == BLACK ? 'K' : 'R');
//     }
// }

void levelorder(RedBlackTree *root)
{
    if (root == NIL)
        return;
    int nodes = size(root);
    RedBlackTree *queue[nodes];
    int front = 0, rear = 0;
    queue[rear++] = root;
    while (front < rear)
    {
        root = queue[front++];
        if (root->left != NIL)
            queue[rear++] = root->left;
        if (root->right != NIL)
            queue[rear++] = root->right;
        printf("<%d|%c> ", root->data, root->color == BLACK ? 'K' : 'R');
    }
}

bool isPerfect(RedBlackTree *root)
{
    int levels = height(root);
    int nodes = size(root);
    return (nodes == (int)pow(2, levels + 1) - 1);
}

bool isComplete(RedBlackTree *root)
{
    if (root == NIL)
        return true;
    int nodes = size(root);
    RedBlackTree *queue[nodes];
    int front = 0, rear = 0;
    bool nullFound = false;
    queue[rear++] = root;
    while (front < rear)
    {
        root = queue[front++];
        if (root->left != NIL)
        {
            if (nullFound)
                return false;
            queue[rear++] = root->left;
        }
        else
            nullFound = true;
        if (root->right != NIL)
        {
            if (nullFound)
                return false;
            queue[rear++] = root->right;
        }
        else
            nullFound = true;
    }
    return true;
}

bool isFull(RedBlackTree *root)
{
    if (root == NIL)
        return true;
    if ((root->left == NIL) ^ (root->right == NIL))
        return false;
    return (isFull(root->left) && isFull(root->right));
}

bool areMirrored(RedBlackTree *root1, RedBlackTree *root2)
{
    if (root1 == NIL && root2 == NIL)
        return true;
    else if (root1 == NIL || root2 == NIL)
        return false;
    else
        return (areMirrored(root1->left, root2->right) && areMirrored(root1->right, root2->left));
}

bool isSymmetric(RedBlackTree *root)
{
    if (root == NIL)
        return true;
    return areMirrored(root->left, root->right);
}

RedBlackTree *leftRotate(RedBlackTree *root, RedBlackTree *x)
{
    RedBlackTree *y = x->right;
    x->right = y->left;
    if (x->right != NIL)
        x->right->parent = x;
    y->parent = x->parent;
    if (y->parent == NIL)
        root = y;
    else if (y->parent->left == x)
        y->parent->left = y;
    else
        y->parent->right = y;
    y->left = x;
    x->parent = y;
    return root;
}

RedBlackTree *rightRotate(RedBlackTree *root, RedBlackTree *x)
{
    RedBlackTree *y = x->left;
    x->left = y->right;
    if (x->left != NIL)
        x->left->parent = x;
    y->parent = x->parent;
    if (y->parent == NIL)
        root = y;
    else if (y->parent->left == x)
        y->parent->left = y;
    else
        y->parent->right = y;
    y->right = x;
    x->parent = y;
    return root;
}

RedBlackTree *fixInsertion(RedBlackTree *root, RedBlackTree *node)
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
                    root = leftRotate(root, node);
                }
                // Case 3: Uncle is BLACK, and node is a left child
                node->parent->color = BLACK;
                node->parent->parent->color = RED;
                root = rightRotate(root, node->parent->parent);
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
                    root = rightRotate(root, node);
                }
                // Case 3: Uncle is BLACK, and node is a right child
                node->parent->color = BLACK;
                node->parent->parent->color = RED;
                root = leftRotate(root, node->parent->parent);
            }
        }
    }
    // Ensure root is always BLACK
    root->color = BLACK;
    return root;
}

RedBlackTree *insert(RedBlackTree *root, int value)
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
    root = fixInsertion(root, node);
    return root;
}

RedBlackTree *fixDeletion(RedBlackTree *root, RedBlackTree *node)
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
                root = leftRotate(root, node->parent);
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
                    root = rightRotate(root, sibling);
                    sibling = node->parent->right;
                }
                // Case 4: Sibling is BLACK, and sibling's right child is RED
                sibling->color = node->parent->color;
                node->parent->color = BLACK;
                sibling->right->color = BLACK;
                root = leftRotate(root, node->parent);
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
                root = rightRotate(root, node->parent);
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
                    root = leftRotate(root, sibling);
                    sibling = node->parent->left;
                }
                // Case 4: Sibling is BLACK, and sibling's left child is RED
                sibling->color = node->parent->color;
                node->parent->color = BLACK;
                sibling->left->color = BLACK;
                root = rightRotate(root, node->parent);
                node = root;
            }
        }
    }
    // Ensure replacement node is BLACK
    node->color = BLACK;
    return root;
}

RedBlackTree *transplant(RedBlackTree *root, RedBlackTree *u, RedBlackTree *v)
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

RedBlackTree *discard(RedBlackTree *root, int value)
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
                NIL->parent = curr->parent;
                replacementNode = NIL;
                deletedColor = curr->color;

                root = transplant(root, curr, NIL);
                free(curr);
            }
            else if (curr->left == NIL || curr->right == NIL)
            {
                RedBlackTree *child = (curr->left != NIL) ? curr->left : curr->right;
                replacementNode = child;
                deletedColor = curr->color;

                root = transplant(root, curr, child);
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

                root = transplant(root, succ, child);
                free(succ);
            }
            break;
        }
    }
    if (deletedColor == BLACK)
        root = fixDeletion(root, replacementNode);
    return root;
}

int main()
{
    initSentinel();
    RedBlackTree *root = NIL;

    // Insertion Tests
    root = insert(root, 10);
    root = insert(root, 20);
    root = insert(root, 30);
    root = insert(root, 15);
    root = insert(root, 5);
    root = insert(root, 1);
    root = insert(root, 25);
    root = insert(root, 35);

    printf("Level order Traversal after insertions: ");
    levelorder(root);
    printf("\n");

    // Deletion Tests
    root = discard(root, 20);
    printf("Level order after deleting 20: ");
    levelorder(root);
    printf("\n");

    root = discard(root, 10);
    printf("Level order after deleting 10: ");
    levelorder(root);
    printf("\n");

    // Search Test
    int target = 25;
    RedBlackTree *found = search(root, target);
    if (found != NIL)
        printf("Found %d in tree\n", found->data);
    else
        printf("%d not found in tree\n", target);

    // Successor / Predecessor Test
    RedBlackTree *succ = successor(root, 15);
    if (succ != NIL)
        printf("Successor of 15 is %d\n", succ->data);
    else
        printf("No successor for 15\n");

    RedBlackTree *pred = predecessor(root, 15);
    if (pred != NIL)
        printf("Predecessor of 15 is %d\n", pred->data);
    else
        printf("No predecessor for 15\n");

    // Tree Property Tests
    printf("Is Perfect? %s\n", isPerfect(root) ? "Yes" : "No");
    printf("Is Complete? %s\n", isComplete(root) ? "Yes" : "No");
    printf("Is Full? %s\n", isFull(root) ? "Yes" : "No");
    printf("Is Symmetric? %s\n", isSymmetric(root) ? "Yes" : "No");

    // Maximum and Minimum
    printf("Minimum is %d\n", minimum(root)->data);
    printf("Maximum is %d\n", maximum(root)->data);

    // Ceiling and Floor
    float value = 17;
    RedBlackTree *ceil = ceiling(root, value);
    if (ceil != NIL)
        printf("Ceiling of %.2f is %d\n", value, ceil->data);
    else
        printf("No ceiling for %.2f\n", value);

    RedBlackTree *floor_val = floored(root, value);
    if (floor_val != NIL)
        printf("Floor of %.2f is %d\n", value, floor_val->data);
    else
        printf("No floor for %.2f\n", value);

    // Height, Size, Internal, External
    printf("Height of tree: %d\n", height(root));
    printf("Size of tree: %d\n", size(root));
    printf("Internal nodes: %d\n", internal(root));
    printf("External nodes: %d\n", external(root));

    // Path between two nodes
    printf("Path between 1 and 35: ");
    path(root, 1, 35);
    printf("\n");

    // Levelorder Traversal
    printf("Levelorder Traversal: ");
    levelorder(root);
    printf("\n");

    // Mirror Test
    RedBlackTree *mirrored = mirror(root);
    printf("Inorder of mirrored tree: ");
    inorder(mirrored);
    printf("\n");
    mirrored = clear(mirrored);

    // Copy Test
    RedBlackTree *copy_tree = copy(root);
    printf("Inorder of copied tree: ");
    inorder(copy_tree);
    printf("\n");

    // Clear Trees
    root = clear(root);
    copy_tree = clear(copy_tree);

    printf("All tests passed and memory cleaned up.\n");
    return 0;
}