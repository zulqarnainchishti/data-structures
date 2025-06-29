#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Node
{
    int data;
    struct Node *next;
} Node;

typedef struct LinkedList
{
    struct Node *head;
    int length;
} LinkedList;

Node *create(int value)
{
    Node *node = (Node *)malloc(sizeof(Node));
    node->data = value;
    node->next = NULL;
    return node;
}

LinkedList init()
{
    LinkedList list;
    list.head = NULL;
    list.length = 0;
    return list;
}

void clearRecursive(Node *node)
{
    if (node == NULL)
        return;
    clearRecursive(node->next);
    free(node);
}

void clear(LinkedList *list)
{
    clearRecursive(list->head);
    list->head = NULL;
    list->length = 0;
}

Node *copyRecursive(Node *node)
{
    if (node == NULL)
        return NULL;
    Node *newNode = create(node->data);
    newNode->next = copyRecursive(node->next);
    return newNode;
}

LinkedList copy(LinkedList list)
{
    LinkedList newList = init();
    newList.head = copyRecursive(list.head);
    newList.length = list.length;
    return newList;
}

void reverse(LinkedList *list)
{
    Node *prevNode = NULL;
    Node *currNode = list->head;
    Node *nextNode;
    while (currNode != NULL)
    {
        nextNode = currNode->next;
        currNode->next = prevNode;
        prevNode = currNode;
        currNode = nextNode;
    }
    list->head = prevNode;
}

Node *search(LinkedList list, int value)
{
    Node *temp = list.head;
    while (temp != NULL)
    {
        if (temp->data == value)
            return temp;
        temp = temp->next;
    }
    return NULL;
}

void traverse(LinkedList list)
{
    Node *temp = list.head;
    while (temp != NULL)
    {
        printf("[%d]", temp->data);
        temp = temp->next;
        printf((temp == NULL) ? "\n" : " -> ");
    }
}

void insertAtStart(LinkedList *list, int value)
{
    Node *node = create(value);
    list->length++;
    if (list->head == NULL)
    {
        list->head = node;
        return;
    }
    node->next = list->head;
    list->head = node;
}

void insertAtEnd(LinkedList *list, int value)
{
    Node *node = create(value);
    list->length++;
    if (list->head == NULL)
    {
        list->head = node;
        return;
    }
    Node *temp = list->head;
    while (temp->next != NULL)
        temp = temp->next;
    temp->next = node;
}

void insertAtIndex(LinkedList *list, int value, int index)
{
    if (index <= 0)
        return insertAtStart(list, value);
    if (index >= list->length)
        return insertAtEnd(list, value);
    Node *node = create(value);
    list->length++;
    Node *temp = list->head;
    for (int i = 0; i < index - 1; i++)
        temp = temp->next;
    node->next = temp->next;
    temp->next = node;
}

int insertAfterValue(LinkedList *list, int new, int old)
{
    Node *temp = search(*list, old);
    if (temp == NULL)
        return -1;
    Node *node = create(new);
    list->length++;
    node->next = temp->next;
    temp->next = node;
    return 0;
}

int deleteStart(LinkedList *list)
{
    if (list->head == NULL)
        return -1;
    list->length--;
    if (list->head->next == NULL)
    {
        int value = list->head->data;
        free(list->head);
        list->head = NULL;
        return value;
    }
    Node *target = list->head;
    int value = target->data;
    list->head = target->next;
    free(target);
    return value;
}

int deleteEnd(LinkedList *list)
{
    if (list->head == NULL)
        return -1;
    list->length--;
    if (list->head->next == NULL)
    {
        int value = list->head->data;
        free(list->head);
        list->head = NULL;
        return value;
    }
    Node *temp = list->head;
    while (temp->next->next != NULL)
        temp = temp->next;
    Node *target = temp->next;
    int value = target->data;
    temp->next = NULL;
    free(target);
    return value;
}

int deleteIndex(LinkedList *list, int index)
{
    if (list->head == NULL)
        return -1;
    if (index <= 0)
        return deleteStart(list);
    if (index >= list->length - 1)
        return deleteEnd(list);
    list->length--;
    Node *temp = list->head;
    for (int i = 0; i < index - 1; i++)
        temp = temp->next;
    Node *target = temp->next;
    int value = target->data;
    temp->next = target->next;
    free(target);
    return value;
}

int deleteValue(LinkedList *list, int value)
{
    if (list->head == NULL)
        return -1;
    if (list->head->data == value)
    {
        Node *target = list->head;
        list->head = list->head->next;
        free(target);
        list->length--;
        return 0;
    }
    Node *prevNode = list->head;
    Node *currNode = list->head->next;
    while (currNode != NULL)
    {
        if (currNode->data == value)
        {
            prevNode->next = currNode->next;
            free(currNode);
            list->length--;
            return 0;
        }
        prevNode = currNode;
        currNode = currNode->next;
    }
    return -1;
}

int main()
{
    LinkedList list = init();

    printf("Initial list:\n");
    traverse(list);

    // Insertion at start
    insertAtStart(&list, 30);
    insertAtStart(&list, 20);
    insertAtStart(&list, 10);
    printf("After insertAtStart 10, 20, 30:\n");
    traverse(list);

    // Insertion at end
    insertAtEnd(&list, 40);
    insertAtEnd(&list, 50);
    printf("After insertAtEnd 40, 50:\n");
    traverse(list);

    // Insertion at index
    insertAtIndex(&list, 25, 2); // Insert 25 at index 2
    printf("After insertAtIndex 25 at index 2:\n");
    traverse(list);

    // Insertion after value
    if (insertAfterValue(&list, 35, 30) == 0)
        printf("After insertAfterValue 35 after 30:\n");
    else
        printf("Value 30 not found\n");
    traverse(list);

    // Search existing and non-existing values
    Node *found = search(list, 40);
    printf("Search 40: %s\n", (found != NULL) ? "Found" : "Not found");
    found = search(list, 99);
    printf("Search 99: %s\n", (found != NULL) ? "Found" : "Not found");

    // Deletion at start
    deleteStart(&list);
    printf("After deleteStart:\n");
    traverse(list);

    // Deletion at end
    deleteEnd(&list);
    printf("After deleteEnd:\n");
    traverse(list);

    // Deletion at index
    deleteIndex(&list, 2); // Should remove element at index 2
    printf("After deleteIndex at 2:\n");
    traverse(list);

    // Deletion by value (existing)
    if (deleteValue(&list, 35) == 0)
        printf("After deleteValue 35:\n");
    else
        printf("Value 35 not found\n");
    traverse(list);

    // Deletion by value (non-existing)
    if (deleteValue(&list, 999) == 0)
        printf("After deleteValue 999:\n");
    else
        printf("Value 999 not found\n");
    traverse(list);

    // Copying the list
    LinkedList copiedList = copy(list);
    printf("Copied list:\n");
    traverse(copiedList);

    // Reversing the original list
    reverse(&list);
    printf("After reverse (original list):\n");
    traverse(list);

    // Clearing both lists
    clear(&list);
    clear(&copiedList);
    printf("After clearing both lists:\n");
    printf("Original: ");
    traverse(list);
    printf("Copied: ");
    traverse(copiedList);

    return 0;
}