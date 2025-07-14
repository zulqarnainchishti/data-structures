#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define ALPHABET_SIZE 26

typedef struct Trie
{
    struct Trie *children[ALPHABET_SIZE];
    bool terminal;
} Trie;

int numericEquivalent(char c)
{
    if (c >= 'a' && c <= 'z')
        return c - 'a';
    if (c >= 'A' && c <= 'Z')
        return c - 'A';
    return -1;
}

int alphabeticEquivalent(int n)
{
    return 'a' + n;
}

Trie *init()
{
    Trie *node = (Trie *)malloc(sizeof(Trie));
    for (int i = 0; i < ALPHABET_SIZE; i++)
        node->children[i] = NULL;
    node->terminal = false;
    return node;
}

Trie *copy(Trie *root)
{
    Trie *node = init();
    for (int i = 0; i < ALPHABET_SIZE; i++)
        if (root->children[i] != NULL)
            node->children[i] = copy(root->children[i]);
    node->terminal = root->terminal;
    return node;
}

void clear(Trie *root)
{
    if (root == NULL)
        return;
    for (int i = 0; i < ALPHABET_SIZE; i++)
        if (root->children[i] != NULL)
            clear(root->children[i]);
    free(root);
}

bool isEmpty(Trie *node)
{
    if (node == NULL)
        return true;
    for (int i = 0; i < ALPHABET_SIZE; i++)
        if (node->children[i] != NULL)
            return false;
    return true;
}

Trie *insert(Trie *root, char *word)
{
    if (root == NULL)
        root = init();
    Trie *curr = root;
    for (int i = 0; word[i] != '\0'; i++)
    {
        int index = numericEquivalent(word[i]);
        if (index == -1)
            continue;
        if (curr->children[index] == NULL)
            curr->children[index] = init();
        curr = curr->children[index];
    }
    curr->terminal = true;
    return root;
}

bool search(Trie *root, char *word)
{
    if (root == NULL)
        return false;
    Trie *curr = root;
    int len = strlen(word);
    for (int i = 0; word[i] != '\0'; i++)
    {
        int index = numericEquivalent(word[i]);
        if (index == -1)
            continue;
        if (curr->children[index] == NULL)
            return false;
        curr = curr->children[index];
    }
    return curr->terminal;
}

bool startsWith(Trie *root, char *prefix)
{
    if (root == NULL)
        return false;
    Trie *curr = root;
    for (int i = 0; prefix[i] != '\0'; i++)
    {
        int index = numericEquivalent(prefix[i]);
        if (index == -1)
            continue;
        if (curr->children[index] == NULL)
            return false;
        curr = curr->children[index];
    }
    return true;
}

Trie *discardHelper(Trie *root, const char *word, int depth)
{
    if (root == NULL)
        return NULL;

    if (word[depth] == '\0')
    {
        if (!root->terminal)
            return root;
        root->terminal = false;
        if (isEmpty(root))
        {
            clear(root);
            return NULL;
        }
        return root;
    }

    int index = numericEquivalent(word[depth]);
    if (index == -1 || root->children[index] == NULL)
        return root;

    root->children[index] = discardHelper(root->children[index], word, depth + 1);
    if (!root->terminal && isEmpty(root))
    {
        clear(root);
        return NULL;
    }
    return root;
}

Trie *discard(Trie *root, const char *word)
{
    return discardHelper(root, word, 0);
}

int countAll(Trie *root)
{
    if (root == NULL)
        return 0;
    int words = root->terminal ? 1 : 0;
    for (int i = 0; i < ALPHABET_SIZE; i++)
        if (root->children[i] != NULL)
            words += countAll(root->children[i]);
    return words;
}

int countWithPrefix(Trie *root, char *prefix)
{
    if (root == NULL)
        return 0;
    Trie *curr = root;
    for (int i = 0; prefix[i] != '\0'; i++)
    {
        int index = numericEquivalent(prefix[i]);
        if (index == -1)
            continue;
        if (curr->children[index] == NULL)
            return 0;
        curr = curr->children[index];
    }
    return countAll(curr);
}

int length(Trie *root)
{
    if (isEmpty(root))
        return 0;
    int maxDepth = 0;
    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        if (root->children[i] != NULL)
        {
            int childDepth = length(root->children[i]);
            if (maxDepth < childDepth)
                maxDepth = childDepth;
        }
    }
    return 1 + maxDepth;
}

void printHelper(Trie *root, char *buffer, int depth)
{
    if (root == NULL)
        return;
    if (root->terminal)
    {
        buffer[depth] = '\0';
        printf("%s\n", buffer);
    }
    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        if (root->children[i] != NULL)
        {
            buffer[depth] = alphabeticEquivalent(i);
            printHelper(root->children[i], buffer, depth + 1);
        }
    }
}

void printAll(Trie *root)
{
    int len = length(root);
    char buffer[len + 1];
    printHelper(root, buffer, 0);
}

void printWithPrefix(Trie *root, char *prefix)
{
    if (root == NULL)
        return;
    Trie *curr = root;
    int i;
    for (i = 0; prefix[i] != '\0'; i++)
    {
        int index = numericEquivalent(prefix[i]);
        if (index == -1)
            continue;
        if (curr->children[index] == NULL)
            return;
        curr = curr->children[index];
    }
    int len = length(curr);
    char buffer[i + len + 1];
    strcpy(buffer, prefix);
    printHelper(curr, buffer, i);
}

int main()
{
    Trie *root = NULL;

    printf("=== Insertion ===\n");
    root = insert(root, "apple");
    root = insert(root, "app");
    root = insert(root, "apex");
    root = insert(root, "bat");
    root = insert(root, "batch");
    root = insert(root, "baton");
    root = insert(root, "cat");

    printf("\n=== Search ===\n");
    printf("Search 'apple': %s\n", search(root, "apple") ? "Found" : "Not Found");
    printf("Search 'app': %s\n", search(root, "app") ? "Found" : "Not Found");
    printf("Search 'bat': %s\n", search(root, "bat") ? "Found" : "Not Found");
    printf("Search 'batch': %s\n", search(root, "batch") ? "Found" : "Not Found");
    printf("Search 'batman': %s\n", search(root, "batman") ? "Found" : "Not Found");
    printf("Search 'cat': %s\n", search(root, "cat") ? "Found" : "Not Found");

    printf("\n=== startsWith ===\n");
    printf("Prefix 'ap': %s\n", startsWith(root, "ap") ? "Yes" : "No");
    printf("Prefix 'bat': %s\n", startsWith(root, "bat") ? "Yes" : "No");
    printf("Prefix 'dog': %s\n", startsWith(root, "dog") ? "Yes" : "No");

    printf("\n=== countAll ===\n");
    printf("Total words in trie: %d\n", countAll(root));

    printf("\n=== countWithPrefix ===\n");
    printf("Words with prefix 'ba': %d\n", countWithPrefix(root, "ba"));
    printf("Words with prefix 'bat': %d\n", countWithPrefix(root, "bat"));
    printf("Words with prefix 'ap': %d\n", countWithPrefix(root, "ap"));
    printf("Words with prefix 'z': %d\n", countWithPrefix(root, "z"));

    printf("\n=== length ===\n");
    printf("Length of longest word path: %d\n", length(root));

    printf("\n=== printAll ===\n");
    printAll(root);

    printf("\n=== printWithPrefix 'bat' ===\n");
    printWithPrefix(root, "bat");

    printf("\n=== printWithPrefix 'ap' ===\n");
    printWithPrefix(root, "ap");

    printf("\n=== Deletion ===\n");
    root = discard(root, "baton");
    printf("Deleted 'baton'\n");
    root = discard(root, "apple");
    printf("Deleted 'apple'\n");
    root = discard(root, "bat");
    printf("Deleted 'bat'\n");
    root = discard(root, "cat");
    printf("Deleted 'cat'\n");

    printf("\n=== printAll After Deletions ===\n");
    printAll(root);

    printf("\n=== countAll After Deletions ===\n");
    printf("Total words in trie: %d\n", countAll(root));

    printf("\n=== Final Trie Length ===\n");
    printf("Length of longest word path: %d\n", length(root));

    printf("\n=== Clear Trie ===\n");
    clear(root);
    printf("Trie cleared.\n");

    return 0;
}