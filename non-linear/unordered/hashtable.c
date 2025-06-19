#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct Node
{
    char *key;
    int value;
    struct Node *next;
} Node;

typedef struct HashTable
{
    Node **buckets;
    int size;
    int length;
} HashTable;

HashTable init(int capacity)
{
    HashTable table;
    table.buckets = (Node **)calloc(capacity, sizeof(Node *));
    table.size = capacity;
    table.length = 0;
    return table;
}

HashTable copy(HashTable table)
{
    HashTable new;
    new.size = table.size;
    new.length = table.length;
    new.buckets = (Node **)calloc(new.size, sizeof(Node *));
    for (int i = 0; i < table.size; i++)
    {
        Node *src = table.buckets[i];
        Node *dst = new.buckets[i];
        while (src != NULL)
        {
            Node *node = (Node *)malloc(sizeof(Node));
            node->key = strdup(src->key);
            node->value = src->value;
            node->next = NULL;
            if (dst == NULL)
                new.buckets[i] = node;
            else
                dst->next = node;
            dst = node;
            src = src->next;
        }
    }
    return new;
}

void clear(HashTable *table)
{
    Node *curr, *next;
    for (int i = 0; i < table->size; i++)
    {
        curr = table->buckets[i];
        while (curr != NULL)
        {
            next = curr->next;
            free(curr->key);
            free(curr);
            curr = next;
        }
        table->buckets[i] = NULL;
    }
    table->length = 0;
}

void delete(HashTable *table)
{
    Node *curr, *next;
    for (int i = 0; i < table->size; i++)
    {
        curr = table->buckets[i];
        while (curr != NULL)
        {
            next = curr->next;
            free(curr->key);
            free(curr);
            curr = next;
        }
        table->buckets[i] = NULL;
    }
    free(table->buckets);
    table->buckets = NULL;
    table->size = 0;
    table->length = 0;
}

int hashFunction(char *key, int size)
{
    // FNV-1a algorithm
    unsigned long hash = 2166136261u;
    // FNV 32-bit offset basis, 'u' enforces unsigned arithmetic with defined modular overflow (wrap-around modulo 2^32)
    while (*key)
    {
        hash ^= (unsigned char)(*key++);
        // cast enforces unsigned 8-bit interpretation (0–255)
        hash *= 16777619u;
        // FNV 32-bit prime, unsigned multiplication with well-defined modular overflow (mod 2^32)
    }
    int index = hash % size;
    // index in [0, size-1]
    return index;
}

float loadFactor(HashTable table)
{
    return (float)table.length / table.size;
}

void resize(HashTable *table)
{
    if (loadFactor(*table) > 0.75)
    {
        int newSize = table->size * 2;
        Node **newBuckets = (Node **)calloc(newSize, sizeof(Node *));
        for (int i = 0; i < table->size; i++)
        {
            Node *curr = table->buckets[i];
            while (curr != NULL)
            {
                Node *next = curr->next;
                int index = hashFunction(curr->key, newSize);
                curr->next = newBuckets[index];
                newBuckets[index] = curr;
                curr = next;
            }
        }
        free(table->buckets);
        table->buckets = newBuckets;
        table->size = newSize;
    }
}

bool contains(HashTable table, char *key)
{
    int index = hashFunction(key, table.size);
    Node *curr = table.buckets[index];
    while (curr != NULL)
    {
        if (strcmp(curr->key, key) == 0)
            return true;
        curr = curr->next;
    }
    return false;
}

int get(HashTable table, char *key)
{
    int index = hashFunction(key, table.size);
    Node *curr = table.buckets[index];
    while (curr != NULL)
    {
        if (strcmp(curr->key, key) == 0)
            return curr->value;
        curr = curr->next;
    }
    return -1;
}

int pop(HashTable *table, char *key)
{
    int index = hashFunction(key, table->size);
    Node *prev = NULL;
    Node *curr = table->buckets[index];
    while (curr != NULL)
    {
        if (strcmp(curr->key, key) == 0)
        {
            if (prev == NULL)
                table->buckets[index] = curr->next;
            else
                prev->next = curr->next;
            int value = curr->value;
            free(curr->key);
            free(curr);
            table->length--;
            return value;
        }
        prev = curr;
        curr = curr->next;
    }
    return -1;
}

void put(HashTable *table, char *key, int value)
{
    int index = hashFunction(key, table->size);
    Node *curr = table->buckets[index];
    while (curr != NULL)
    {
        if (strcmp(curr->key, key) == 0)
        {
            curr->value = value;
            return;
        }
        curr = curr->next;
    }
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->key = strdup(key);
    newNode->value = value;
    newNode->next = table->buckets[index];
    table->buckets[index] = newNode;
    table->length++;
    resize(table);
}

char **keys(HashTable table)
{
    char **keyList = (char **)malloc(table.length * sizeof(char *));
    int count = 0;
    Node *curr;
    for (int i = 0; i < table.size; i++)
    {
        curr = table.buckets[i];
        while (curr != NULL)
        {
            keyList[count++] = strdup(curr->key);
            curr = curr->next;
        }
    }
    return keyList;
}

int *values(HashTable table)
{
    int *valueList = (int *)malloc(table.length * sizeof(int));
    int count = 0;
    Node *curr;
    for (int i = 0; i < table.size; i++)
    {
        curr = table.buckets[i];
        while (curr != NULL)
        {
            valueList[count++] = curr->value;
            curr = curr->next;
        }
    }
    return valueList;
}

Node *items(HashTable table)
{
    Node *itemList = (Node *)malloc(table.length * sizeof(Node));
    int count = 0;
    Node *curr;
    for (int i = 0; i < table.size; i++)
    {
        curr = table.buckets[i];
        while (curr != NULL)
        {
            itemList[count].key = strdup(curr->key);
            itemList[count].value = curr->value;
            itemList[count].next = NULL;
            count++;
            curr = curr->next;
        }
    }
    return itemList;
}

void traverse(HashTable table)
{
    printf("{ ");
    for (int i = 0; i < table.size; i++)
    {
        Node *curr = table.buckets[i];
        while (curr != NULL)
        {
            printf("%s:%d ", curr->key, curr->value);
            curr = curr->next;
        }
    }
    printf("} : %.2f\n", loadFactor(table));
}

void describe(HashTable table)
{
    Node *curr;
    for (int i = 0; i < table.size; i++)
    {
        printf("%2d | ", i);
        curr = table.buckets[i];
        while (curr != NULL)
        {
            printf("{%s,%d}", curr->key, curr->value);
            curr = curr->next;
            if (curr != NULL)
                printf(" -> ");
        }
        printf("\n");
    }
}

int main()
{
    printf("Initializing hash table...\n");
    HashTable table = init(5);
    describe(table);

    // Test put()
    printf("\nInserting elements...\n");
    put(&table, "apple", 10);
    put(&table, "banana", 20);
    put(&table, "cherry", 30);
    put(&table, "date", 40);
    put(&table, "elderberry", 50); // should trigger resize
    describe(table);

    // Test get()
    printf("\nRetrieving 'banana': %d\n", get(table, "banana"));
    printf("Retrieving 'date': %d\n", get(table, "date"));
    printf("Retrieving 'fig' (not present): %d\n", get(table, "fig"));

    // Test contains()
    printf("\nContains 'apple': %d\n", contains(table, "apple"));
    printf("Contains 'fig': %d\n", contains(table, "fig"));

    // Test pop()
    printf("\nPopping 'banana': %d\n", pop(&table, "banana"));
    printf("Popping 'fig' (not present): %d\n", pop(&table, "fig"));
    describe(table);

    // Test keys()
    printf("\nKeys in table:\n");
    char **keysArray = keys(table);
    for (int i = 0; i < table.length; i++)
    {
        printf("Key: %s\n", keysArray[i]);
        free(keysArray[i]);
    }
    free(keysArray);

    // Test values()
    printf("\nValues in table:\n");
    int *valuesArray = values(table);
    for (int i = 0; i < table.length; i++)
        printf("Value: %d\n", valuesArray[i]);
    free(valuesArray);

    // Test items()
    printf("\nItems in table:\n");
    Node *itemsArray = items(table);
    for (int i = 0; i < table.length; i++)
    {
        printf("{%s, %d}\n", itemsArray[i].key, itemsArray[i].value);
        free(itemsArray[i].key);
    }
    free(itemsArray);

    // Test traverse()
    printf("\nTraversing table:\n");
    traverse(table);

    // Test clear()
    printf("\nClearing table...\n");
    clear(&table);
    describe(table);

    // Reinsert after clear
    printf("\nReinserting after clear...\n");
    put(&table, "grape", 60);
    put(&table, "kiwi", 70);
    describe(table);

    // Test copy()
    printf("\nCopying table...\n");
    HashTable copyTable = copy(table);
    describe(copyTable);

    // Test delete() on original
    printf("\nDeleting original table...\n");
    delete(&table);
    describe(table);

    // Test describe() on copy to confirm it remains
    printf("\nCopy table after original deletion:\n");
    describe(copyTable);

    // Final cleanup
    delete(&copyTable);
}