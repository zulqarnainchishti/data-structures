#ifndef TRIE_H
#define TRIE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define ALPHABET_SIZE 26

/**
 * @struct Trie
 * @brief Represents a node in the trie data structure.
 * @note Each node contains an array of pointers to its children and a boolean flag to indicate if the node marks the end of a word.
 */
typedef struct Trie
{
    struct Trie *children[ALPHABET_SIZE];
    bool terminal;
} Trie;

/**
 * @brief Initializes a new trie node.
 * @return A pointer to the newly created node, or NULL on failure.
 */
Trie *init()
{
    Trie *node = (Trie *)malloc(sizeof(Trie));
    if (node == NULL)
        return NULL;
    for (int i = 0; i < ALPHABET_SIZE; i++)
        node->children[i] = NULL;
    node->terminal = false;
    return node;
}

/**
 * @brief Creates a deep copy of a trie.
 * @param root The root of the trie to copy.
 * @return A pointer to the new, copied trie, or NULL on failure.
 */
Trie *copy(const Trie *root)
{
    if (root == NULL)
        return NULL;
    Trie *node = init();
    if (node == NULL)
        return NULL;
    for (int i = 0; i < ALPHABET_SIZE; i++)
        if (root->children[i] != NULL)
            node->children[i] = copy(root->children[i]);
    node->terminal = root->terminal;
    return node;
}

/**
 * @brief Frees all memory allocated for a trie.
 * @param root The root of the trie to destroy.
 */
void destroy(Trie *root)
{
    if (root == NULL)
        return;
    for (int i = 0; i < ALPHABET_SIZE; i++)
        if (root->children[i] != NULL)
            destroy(root->children[i]);
    free(root);
}

/**
 * @brief Converts a character to its corresponding alphabet index (0-25).
 * @param c The character to convert.
 * @return The index (0-25) or -1 if the character is not a letter.
 */
int __number__(const char c)
{
    if (c >= 'a' && c <= 'z')
        return c - 'a';
    if (c >= 'A' && c <= 'Z')
        return c - 'A';
    return -1;
}

/**
 * @brief Inserts a word into the trie.
 * @param root The root of the trie.
 * @param word The word to insert.
 * @return The root of the trie, or NULL if insertion fails.
 */
Trie *insert(Trie *root, const char *word)
{
    if (root == NULL || word == NULL)
        return NULL;
    Trie *curr = root;
    for (int i = 0; word[i] != '\0'; i++)
    {
        int index = __number__(word[i]);
        if (index == -1)
            continue;
        if (curr->children[index] == NULL)
        {
            curr->children[index] = init();
            if (curr->children[index] == NULL)
                return root;
        }
        curr = curr->children[index];
    }
    curr->terminal = true;
    return root;
}

/**
 * @brief Checks if a trie node has any children.
 * @param node The trie node to check.
 * @return True if the node is a leaf (has no children), otherwise false.
 */
bool __isEmpty__(const Trie *node)
{
    if (node == NULL)
        return true;
    for (int i = 0; i < ALPHABET_SIZE; i++)
        if (node->children[i] != NULL)
            return false;
    return true;
}

/**
 * @brief Recursively removes a word from the trie.
 * @param root The current trie node.
 * @param word The word to remove.
 * @param depth The current depth in the trie.
 * @return The modified trie node, or NULL if the node is removed.
 */
Trie *__discard__(Trie *root, const char *word, const int depth)
{
    if (root == NULL || word == NULL)
        return NULL;

    if (word[depth] == '\0')
    {
        root->terminal = false;
        if (__isEmpty__(root))
        {
            destroy(root);
            return NULL;
        }
        return root;
    }

    int index = __number__(word[depth]);
    if (index == -1 || root->children[index] == NULL)
        return root;

    root->children[index] = __discard__(root->children[index], word, depth + 1);
    if (!root->terminal && __isEmpty__(root))
    {
        destroy(root);
        return NULL;
    }
    return root;
}

/**
 * @brief Removes a word from the trie.
 * @param root The root of the trie.
 * @param word The word to remove.
 * @return The root of the trie, or NULL if the trie becomes empty.
 */
Trie *discard(Trie *root, const char *word)
{
    if (root == NULL || word == NULL)
        return NULL;
    return __discard__(root, word, 0);
}

/**
 * @brief Searches for a complete word in the trie.
 * @param root The root of the trie.
 * @param word The word to search for.
 * @return True if the word exists, otherwise false.
 */
bool search(const Trie *root, const char *word)
{
    if (root == NULL || word == NULL)
        return false;
    const Trie *curr = root;
    for (int i = 0; word[i] != '\0'; i++)
    {
        int index = __number__(word[i]);
        if (index == -1)
            continue;
        if (curr->children[index] == NULL)
            return false;
        curr = curr->children[index];
    }
    return curr->terminal;
}

/**
 * @brief Checks if any word in the trie starts with a given prefix.
 * @param root The root of the trie.
 * @param prefix The prefix to check.
 * @return True if at least one word starts with the prefix, otherwise false.
 */
bool startsWith(const Trie *root, const char *prefix)
{
    if (root == NULL || prefix == NULL)
        return false;
    const Trie *curr = root;
    for (int i = 0; prefix[i] != '\0'; i++)
    {
        int index = __number__(prefix[i]);
        if (index == -1)
            continue;
        if (curr->children[index] == NULL)
            return false;
        curr = curr->children[index];
    }
    return true;
}

/**
 * @brief Counts the total number of words in the trie.
 * @param root The root of the trie.
 * @return The total number of words.
 */
int countAll(const Trie *root)
{
    if (root == NULL)
        return 0;
    int words = root->terminal ? 1 : 0;
    for (int i = 0; i < ALPHABET_SIZE; i++)
        if (root->children[i] != NULL)
            words += countAll(root->children[i]);
    return words;
}

/**
 * @brief Counts the number of words in the trie with a given prefix.
 * @param root The root of the trie.
 * @param prefix The prefix to count words for.
 * @return The number of words that start with the prefix.
 */
int countWithPrefix(const Trie *root, const char *prefix)
{
    if (root == NULL || prefix == NULL)
        return 0;
    const Trie *curr = root;
    for (int i = 0; prefix[i] != '\0'; i++)
    {
        int index = __number__(prefix[i]);
        if (index == -1)
            continue;
        if (curr->children[index] == NULL)
            return 0;
        curr = curr->children[index];
    }
    return countAll(curr);
}

/**
 * @brief Recursively finds the length of the longest word in the trie.
 * @param root The current trie node.
 * @return The length of the longest word.
 */
int __depth__(const Trie *root)
{
    if (__isEmpty__(root))
        return 0;
    int maxDepth = 0;
    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        if (root->children[i] != NULL)
        {
            int childDepth = __depth__(root->children[i]);
            if (maxDepth < childDepth)
                maxDepth = childDepth;
        }
    }
    return 1 + maxDepth;
}

/**
 * @brief Converts an alphabet index back to a lowercase character.
 * @param n The index (0-25).
 * @return The character or '\0' if the index is out of range.
 */
int __alphabet__(const int n)
{
    if (n >= 0 && n < ALPHABET_SIZE)
        return 'a' + n;
    return '\0';
}

/**
 * @brief Recursively traverses the trie to display all words.
 * @param root The current trie node.
 * @param buffer A character buffer to build the word.
 * @param depth The current depth in the trie.
 */
void __display__(const Trie *root, char *buffer, const int depth)
{
    if (root == NULL || buffer == NULL)
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
            buffer[depth] = __alphabet__(i);
            __display__(root->children[i], buffer, depth + 1);
        }
    }
}

/**
 * @brief Displays all words stored in the trie.
 * @param root The root of the trie.
 */
void displayAll(const Trie *root)
{
    int len = __depth__(root);
    char *buffer = (char *)malloc((len + 1) * sizeof(char));
    if (buffer == NULL)
        return;
    __display__(root, buffer, 0);
    free(buffer);
}

/**
 * @brief Displays all words in the trie that begin with a given prefix.
 * @param root The root of the trie.
 * @param prefix The prefix to filter words by.
 */
void displayWithPrefix(const Trie *root, const char *prefix)
{
    if (root == NULL || prefix == NULL)
        return;
    const Trie *curr = root;
    int depth;
    for (depth = 0; prefix[depth] != '\0'; depth++)
    {
        int index = __number__(prefix[depth]);
        if (index == -1)
            continue;
        if (curr->children[index] == NULL)
            return;
        curr = curr->children[index];
    }
    int len = __depth__(curr);
    char *buffer = (char *)malloc((len + depth + 1) * sizeof(char));
    if (buffer == NULL)
        return;
    for (int i = 0; i < depth; i++)
        buffer[i] = prefix[i];
    __display__(curr, buffer, depth);
    free(buffer);
}

#endif // TRIE_H