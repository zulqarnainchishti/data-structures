#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

typedef enum
{
    EMPTY,
    OCCUPIED,
    DELETED
} Status;

typedef struct
{
    int key;
    Status status;
} Node;

typedef struct Set
{
    Node *buckets;
    int size;
    int length;
} Set;

Set init(int capacity)
{
    Set set;
    set.buckets = (Node *)malloc(capacity * sizeof(Node));
    for (int i = 0; i < capacity; i++)
        set.buckets[i].status = EMPTY;
    set.size = capacity;
    set.length = 0;
    return set;
}

Set copy(Set set)
{
    Set new;
    new.buckets = (Node *)malloc(set.size * sizeof(Node));
    for (int i = 0; i < set.size; i++)
    {
        new.buckets[i].status = set.buckets[i].status;
        if (set.buckets[i].status == OCCUPIED)
            new.buckets[i].key = set.buckets[i].key;
    }
    new.size = set.size;
    new.length = set.length;
    return new;
}

void clear(Set *set)
{
    for (int i = 0; i < set->size; i++)
        set->buckets[i].status = EMPTY;
    set->length = 0;
}

void delete(Set *set)
{
    free(set->buckets);
    set->buckets = NULL;
    set->size = 0;
    set->length = 0;
}

float loadFactor(Set set)
{
    return (float)set.length / set.size;
}

int h1(int key, int size)
{
    // primary hash function: multiplication method
    double A = (sqrt(5.0) - 1) / 2.0;
    double frac = fmod(key * A, 1.0);
    return (int)(size * frac);
    // range [0, size-1]
}
int h2(int key, int size)
{
    // secondary hash function: division method
    return (key % (size - 1)) + 1;
    // range adjusted to avoid zero [1, size-1]
}

int hashFunction(int key, int size, int attempt)
{
    // Unified double hashing probe function
    return (h1(key, size) + attempt * h2(key, size)) % size;
}

void resize(Set *set)
{
    if (loadFactor(*set) > 0.75)
    {
        int newSize = set->size * 2;
        Node *newBuckets = (Node *)malloc(newSize * sizeof(Node));
        for (int i = 0; i < newSize; i++)
            newBuckets[i].status = EMPTY;
        for (int i = 0; i < set->size; i++)
        {
            if (set->buckets[i].status == OCCUPIED)
            {
                int key = set->buckets[i].key;
                for (int j = 0; j < newSize; j++)
                {
                    int index = hashFunction(key, newSize, j);
                    if (newBuckets[index].status == EMPTY)
                    {
                        newBuckets[index].key = key;
                        newBuckets[index].status = OCCUPIED;
                        break;
                    }
                }
            }
        }
        free(set->buckets);
        set->buckets = newBuckets;
        set->size = newSize;
    }
}

bool contains(Set set, int key)
{
    for (int i = 0; i < set.size; i++)
    {
        int index = hashFunction(key, set.size, i);
        if (set.buckets[index].status == EMPTY)
            return false;
        else if (set.buckets[index].status == OCCUPIED)
            if (set.buckets[index].key == key)
                return true;
    }
    return false;
}

void insert(Set *set, int key)
{
    for (int i = 0; i < set->size; i++)
    {
        int index = hashFunction(key, set->size, i);
        if (set->buckets[index].status == OCCUPIED && set->buckets[index].key == key)
            return;
        if (set->buckets[index].status == EMPTY || set->buckets[index].status == DELETED)
        {
            set->buckets[index].status = OCCUPIED;
            set->buckets[index].key = key;
            set->length++;
            resize(set);
            return;
        }
    }
}

void discard(Set *set, int key)
{
    for (int i = 0; i < set->size; i++)
    {
        int index = hashFunction(key, set->size, i);
        if (set->buckets[index].status == EMPTY)
            return;
        if (set->buckets[index].status == OCCUPIED && set->buckets[index].key == key)
        {
            set->buckets[index].status = DELETED;
            set->length--;
            return;
        }
    }
}

void traverse(Set set)
{
    printf("{");
    for (int i = 0; i < set.size; i++)
    {
        if (set.buckets[i].status == OCCUPIED)
            printf(" %d", set.buckets[i].key);
    }
    printf(" } : %.2f\n", loadFactor(set));
}

Set unionOf(Set set1, Set set2)
{
    Set result = init(set1.size + set2.size);
    for (int i = 0; i < set1.size; i++)
        if (set1.buckets[i].status == OCCUPIED)
            insert(&result, set1.buckets[i].key);
    for (int i = 0; i < set2.size; i++)
        if (set2.buckets[i].status == OCCUPIED)
            insert(&result, set2.buckets[i].key);
    return result;
}

Set intersectionOf(Set set1, Set set2)
{
    Set result = init(set1.size < set2.size ? set1.size : set2.size);
    for (int i = 0; i < set1.size; i++)
        if (set1.buckets[i].status == OCCUPIED && contains(set2, set1.buckets[i].key))
            insert(&result, set1.buckets[i].key);
    return result;
}

Set differenceOf(Set set1, Set set2)
{
    Set result = init(set1.size);
    for (int i = 0; i < set1.size; i++)
        if (set1.buckets[i].status == OCCUPIED && !contains(set2, set1.buckets[i].key))
            insert(&result, set1.buckets[i].key);
    return result;
}

Set symmetricDifferenceOf(Set set1, Set set2)
{
    Set result = init(set1.size + set2.size);
    for (int i = 0; i < set1.size; i++)
        if (set1.buckets[i].status == OCCUPIED && !contains(set2, set1.buckets[i].key))
            insert(&result, set1.buckets[i].key);
    for (int i = 0; i < set2.size; i++)
        if (set2.buckets[i].status == OCCUPIED && !contains(set1, set2.buckets[i].key))
            insert(&result, set2.buckets[i].key);
    return result;
}

bool isEqual(Set set1, Set set2)
{
    if (set1.length != set2.length)
        return false;
    for (int i = 0; i < set1.size; i++)
        if (set1.buckets[i].status == OCCUPIED && !contains(set2, set1.buckets[i].key))
            return false;
    return true;
}

bool isSubset(Set subset, Set set)
{
    if (subset.length > set.length)
        return false;
    for (int i = 0; i < subset.size; i++)
        if (subset.buckets[i].status == OCCUPIED && !contains(set, subset.buckets[i].key))
            return false;
    return true;
}

bool isProperSubset(Set subset, Set set)
{
    if (subset.length >= set.length)
        return false;
    for (int i = 0; i < subset.size; i++)
        if (subset.buckets[i].status == OCCUPIED && !contains(set, subset.buckets[i].key))
            return false;
    return true;
}

bool isSuperset(Set superset, Set set)
{
    return isSubset(set, superset);
}

bool isProperSuperset(Set superset, Set set)
{
    return isProperSubset(set, superset);
}

bool isDisjoint(Set set1, Set set2)
{
    for (int i = 0; i < set1.size; i++)
        if (set1.buckets[i].status == OCCUPIED && contains(set2, set1.buckets[i].key))
            return false;
    return true;
}

void describe(Set set)
{
    for (int i = 0; i < set.size; i++)
    {
        printf("%2d | ", i);
        if (set.buckets[i].status == EMPTY)
            printf("EMP\n");
        else if (set.buckets[i].status == OCCUPIED)
            printf("%d\n", set.buckets[i].key);
        else if (set.buckets[i].status == DELETED)
            printf("DEL\n");
    }
}

int main()
{
    // Initialize two sets
    Set A = init(5);
    Set B = init(5);

    // Insert elements into Set A
    insert(&A, 10);
    insert(&A, 20);
    insert(&A, 30);
    printf("Set A: ");
    traverse(A);

    // Insert elements into Set B
    insert(&B, 20);
    insert(&B, 40);
    printf("Set B: ");
    traverse(B);

    // Test contains
    printf("Does A contain 10? %s\n", contains(A, 10) ? "Yes" : "No");
    printf("Does B contain 10? %s\n", contains(B, 10) ? "Yes" : "No");

    // Test discard
    discard(&A, 20);
    printf("Set A after removing 20: ");
    traverse(A);

    // Test union
    Set U = unionOf(A, B);
    printf("Union of A and B: ");
    traverse(U);

    // Test intersection
    Set I = intersectionOf(A, B);
    printf("Intersection of A and B: ");
    traverse(I);

    // Test difference
    Set D = differenceOf(A, B);
    printf("Difference A - B: ");
    traverse(D);

    // Test symmetric difference
    Set SD = symmetricDifferenceOf(A, B);
    printf("Symmetric difference of A and B: ");
    traverse(SD);

    // Test equality
    printf("Are A and B equal? %s\n", isEqual(A, B) ? "Yes" : "No");

    // Test subset relations
    printf("Is A a subset of U? %s\n", isSubset(A, U) ? "Yes" : "No");
    printf("Is A a proper subset of U? %s\n", isProperSubset(A, U) ? "Yes" : "No");

    // Test superset relations
    printf("Is U a superset of A? %s\n", isSuperset(U, A) ? "Yes" : "No");
    printf("Is U a proper superset of A? %s\n", isProperSuperset(U, A) ? "Yes" : "No");

    // Test disjoint
    Set C = init(5);
    insert(&C, 50);
    insert(&C, 60);
    printf("Are A and C disjoint? %s\n", isDisjoint(A, C) ? "Yes" : "No");

    // Test copy
    Set AC = copy(A);
    printf("Copy of A: ");
    traverse(AC);

    // Test clear
    clear(&AC);
    printf("Cleared copy of A: ");
    traverse(AC);

    // Test describe
    printf("\nInternal structure of Set A:\n");
    describe(A);

    // Free all allocated memory
    delete(&A);
    delete(&B);
    delete(&U);
    delete(&I);
    delete(&D);
    delete(&SD);
    delete(&C);
    delete(&AC);

    return 0;
}