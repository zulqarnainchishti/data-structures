#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Node
{
    int data;
    struct Node *next;
} Node;

typedef struct CircularLinkedList
{
    struct Node *tail;
    int length;
} CircularLinkedList;

Node *create(int value)
{
    Node *node = (Node *)malloc(sizeof(Node));
    node->data = value;
    node->next = NULL;
    return node;
}

CircularLinkedList init()
{
    CircularLinkedList list;
    list.tail = NULL;
    list.length = 0;
    return list;
}

void clearRecursive(Node *curr, Node *tail)
{
    if (curr == tail)
    {
        free(curr);
        return;
    }
    clearRecursive(curr->next, tail);
    free(curr);
}

void clear(CircularLinkedList *list)
{
    if (list->tail == NULL)
        return;
    Node *head = list->tail->next;
    clearRecursive(head, list->tail);
    list->tail = NULL;
    list->length = 0;
}

CircularLinkedList copy(CircularLinkedList list)
{
    CircularLinkedList newList = init();
    if (list.tail == NULL)
        return newList;
    Node *temp = list.tail->next;
    do
    {
        Node *node = create(temp->data);
        if (newList.tail == NULL)
        {
            newList.tail = node;
            node->next = node;
        }
        else
        {
            node->next = newList.tail->next;
            newList.tail->next = node;
            newList.tail = node;
        }
        newList.length++;
        temp = temp->next;
    } while (temp != list.tail->next);
    return newList;
}

void reverse(CircularLinkedList *list)
{
    if (list->tail == NULL || list->tail == list->tail->next)
        return;
    Node *prevNode = list->tail;
    Node *currNode = list->tail->next;
    Node *nextNode;
    do
    {
        nextNode = currNode->next;
        currNode->next = prevNode;
        prevNode = currNode;
        currNode = nextNode;
    } while (currNode != list->tail->next);
    list->tail = list->tail->next;
}

Node *search(CircularLinkedList list, int value)
{
    if (list.tail == NULL)
        return NULL;
    Node *temp = list.tail;
    do
    {
        if (temp->data == value)
            return temp;
        temp = temp->next;
    } while (temp != list.tail);
    return NULL;
}

void traverse(CircularLinkedList list)
{
    if (list.tail == NULL)
        return;
    Node *temp = list.tail->next;
    do
    {
        printf("[%d]", temp->data);
        printf(temp == list.tail ? "\n" : " -> ");
        temp = temp->next;
    } while (temp != list.tail->next);
}

void insertAtStart(CircularLinkedList *list, int value)
{
    Node *node = create(value);
    list->length++;
    if (list->tail == NULL)
    {
        list->tail = node;
        node->next = node;
        return;
    }
    node->next = list->tail->next;
    list->tail->next = node;
}

void insertAtEnd(CircularLinkedList *list, int value)
{
    Node *node = create(value);
    list->length++;
    if (list->tail == NULL)
    {
        list->tail = node;
        node->next = node;
        return;
    }
    node->next = list->tail->next;
    list->tail->next = node;
    list->tail = node;
}

void insertAtIndex(CircularLinkedList *list, int value, int index)
{
    if (index <= 0)
        return insertAtStart(list, value);
    if (index >= list->length)
        return insertAtEnd(list, value);
    Node *node = create(value);
    list->length++;
    Node *temp = list->tail->next;
    for (int i = 0; i < index - 1; i++)
        temp = temp->next;
    node->next = temp->next;
    temp->next = node;
}

int insertAfterValue(CircularLinkedList *list, int new, int old)
{
    Node *temp = search(*list, old);
    if (temp == NULL)
        return -1;
    Node *node = create(new);
    list->length++;
    node->next = temp->next;
    temp->next = node;
    if (list->tail == temp)
        list->tail = node;
    return 0;
}

int deleteStart(CircularLinkedList *list)
{
    if (list->tail == NULL)
        return -1;
    list->length--;
    if (list->tail->next == list->tail)
    {
        int value = list->tail->data;
        free(list->tail);
        list->tail = NULL;
        return value;
    }
    Node *target = list->tail->next;
    int value = target->data;
    list->tail->next = target->next;
    free(target);
    return value;
}

int deleteEnd(CircularLinkedList *list)
{
    if (list->tail == NULL)
        return -1;
    list->length--;
    if (list->tail->next == list->tail)
    {
        int value = list->tail->data;
        free(list->tail);
        list->tail = NULL;
        return value;
    }
    Node *temp = list->tail->next;
    while (temp->next != list->tail)
        temp = temp->next;
    Node *target = list->tail;
    int value = target->data;
    temp->next = target->next;
    list->tail = temp;
    free(target);
    return value;
}

int deleteIndex(CircularLinkedList *list, int index)
{
    if (list->tail == NULL)
        return -1;
    if (index <= 0)
        return deleteStart(list);
    if (index >= list->length - 1)
        return deleteEnd(list);
    list->length--;
    Node *temp = list->tail->next;
    for (int i = 0; i < index - 1; i++)
        temp = temp->next;
    Node *target = temp->next;
    int value = target->data;
    temp->next = target->next;
    free(target);
    return value;
}

int deleteValue(CircularLinkedList *list, int value)
{
    if (list->tail == NULL)
        return -1;
    if (list->tail->next == list->tail)
    {
        if (list->tail->data == value)
        {
            free(list->tail);
            list->tail = NULL;
            list->length--;
            return 0;
        }
        return -1;
    }
    Node *prevNode = list->tail;
    Node *currNode = list->tail->next;
    do
    {
        if (currNode->data == value)
        {
            prevNode->next = currNode->next;
            if (currNode == list->tail)
                list->tail = prevNode;
            free(currNode);
            list->length--;
            return 0;
        }
        prevNode = currNode;
        currNode = currNode->next;
    } while (prevNode != list->tail);
    return -1;
}

void main()
{
    CircularLinkedList head = init();
    insertAtStart(&head, 44);
    insertAtStart(&head, 11);
    insertAtEnd(&head, 22);
    insertAtEnd(&head, 66);
    insertAtIndex(&head, 55, 2);
    insertAtIndex(&head, 33, 4);
    insertAfterValue(&head, 77, 11);
    insertAfterValue(&head, 88, 66);
    printf("len %d\n", head.length);
    traverse(head);

    printf("%d\n", deleteValue(&head, 11));
    printf("len %d\n", head.length);
    traverse(head);
    printf("%d\n", deleteValue(&head, 22));
    printf("len %d\n", head.length);
    traverse(head);
    printf("%d\n", deleteValue(&head, 33));
    printf("len %d\n", head.length);
    traverse(head);
    printf("%d\n", deleteValue(&head, 44));
    printf("len %d\n", head.length);
    traverse(head);
    printf("%d\n", deleteValue(&head, 55));
    printf("len %d\n", head.length);
    traverse(head);
    printf("%d\n", deleteValue(&head, 66));
    printf("len %d\n", head.length);
    traverse(head);
    printf("%d\n", deleteValue(&head, 77));
    printf("len %d\n", head.length);
    traverse(head);
    printf("%d\n", deleteValue(&head, 88));
    printf("len %d\n", head.length);
    traverse(head);
}