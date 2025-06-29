#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Node
{
    int data;
    struct Node *prev;
    struct Node *next;
} Node;

typedef struct
{
    struct Node *head;
    int length;
} CircularDoublyLinkedList;

Node *create(int value)
{
    Node *node = (Node *)malloc(sizeof(Node));
    node->data = value;
    node->prev = NULL;
    node->next = NULL;
    return node;
}

CircularDoublyLinkedList init()
{
    CircularDoublyLinkedList list;
    list.head = NULL;
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

void clear(CircularDoublyLinkedList *list)
{
    if (list->head == NULL)
        return;
    Node *tail = list->head->prev;
    clearRecursive(list->head, tail);
    list->head = NULL;
    list->length = 0;
}

CircularDoublyLinkedList copy(CircularDoublyLinkedList list)
{
    CircularDoublyLinkedList newList = init();
    if (list.head == NULL)
        return newList;
    Node *temp = list.head;
    do
    {
        Node *node = create(temp->data);
        if (newList.head == NULL)
        {
            newList.head = node;
            node->next = node;
            node->prev = node;
        }
        else
        {
            Node *tail = newList.head->prev;
            node->next = newList.head;
            node->prev = tail;
            tail->next = node;
            newList.head->prev = node;
        }
        newList.length++;
        temp = temp->next;
    } while (temp != list.head);
    return newList;
}

void reverse(CircularDoublyLinkedList *list)
{
    if (list->head == NULL || list->head == list->head->next)
        return;
    Node *prevNode = list->head->prev;
    Node *currNode = list->head;
    Node *nextNode;
    do
    {
        nextNode = currNode->next;
        currNode->next = prevNode;
        currNode->prev = nextNode;
        prevNode = currNode;
        currNode = nextNode;
    } while (currNode != list->head);
    list->head = list->head->prev;
}

Node *search(CircularDoublyLinkedList list, int value)
{
    if (list.head == NULL)
        return NULL;
    Node *temp = list.head;
    do
    {
        if (temp->data == value)
            return temp;
        temp = temp->next;
    } while (temp != list.head);
    return NULL;
}

void traverse(CircularDoublyLinkedList list)
{
    if (list.head == NULL)
        return;
    Node *temp = list.head;
    do
    {
        printf("[%d]", temp->data);
        temp = temp->next;
        printf(temp == list.head ? "\n" : " <=> ");
    } while (temp != list.head);
}

void insertAtStart(CircularDoublyLinkedList *list, int value)
{
    Node *node = create(value);
    list->length++;
    if (list->head == NULL)
    {
        list->head = node;
        node->prev = node;
        node->next = node;
        return;
    }
    Node *tail = list->head->prev;
    node->prev = tail;
    node->next = list->head;
    list->head->prev = node;
    tail->next = node;
    list->head = node;
}

void insertAtEnd(CircularDoublyLinkedList *list, int value)
{
    Node *node = create(value);
    list->length++;
    if (list->head == NULL)
    {
        list->head = node;
        node->prev = node;
        node->next = node;
        return;
    }
    Node *tail = list->head->prev;
    node->prev = tail;
    node->next = list->head;
    list->head->prev = node;
    tail->next = node;
}

void insertAtIndex(CircularDoublyLinkedList *list, int value, int index)
{
    if (index <= 0)
        return insertAtStart(list, value);
    if (index >= list->length)
        return insertAtEnd(list, value);
    Node *node = create(value);
    list->length++;
    Node *tempPrev = list->head;
    for (int i = 0; i < index - 1; i++)
        tempPrev = tempPrev->next;
    Node *tempNext = tempPrev->next;
    node->prev = tempPrev;
    node->next = tempNext;
    tempNext->prev = node;
    tempPrev->next = node;
}

int insertAfterValue(CircularDoublyLinkedList *list, int new, int old)
{
    Node *tempPrev = search(*list, old);
    if (tempPrev == NULL)
        return -1;
    Node *node = create(new);
    list->length++;
    Node *tempNext = tempPrev->next;
    node->prev = tempPrev;
    node->next = tempNext;
    tempNext->prev = node;
    tempPrev->next = node;
    return 0;
}

int deleteStart(CircularDoublyLinkedList *list)
{
    if (list->head == NULL)
        return -1;
    list->length--;
    if (list->head == list->head->next)
    {
        int value = list->head->data;
        free(list->head);
        list->head = NULL;
        return value;
    }
    Node *target = list->head;
    int value = target->data;
    Node *tail = target->prev;
    list->head = target->next;
    list->head->prev = tail;
    tail->next = list->head;
    free(target);
    return value;
}

int deleteEnd(CircularDoublyLinkedList *list)
{
    if (list->head == NULL)
        return -1;
    list->length--;
    if (list->head == list->head->next)
    {
        int value = list->head->data;
        free(list->head);
        list->head = NULL;
        return value;
    }
    Node *target = list->head->prev;
    int value = target->data;
    Node *tail = target->prev;
    list->head->prev = tail;
    tail->next = list->head;
    free(target);
    return value;
}

int deleteIndex(CircularDoublyLinkedList *list, int index)
{
    if (list->head == NULL)
        return -1;
    if (index <= 0)
        return deleteStart(list);
    if (index >= list->length - 1)
        return deleteEnd(list);
    list->length--;
    Node *target = list->head;
    for (int i = 0; i < index; i++)
        target = target->next;
    Node *tempPrev = target->prev;
    Node *tempNext = target->next;
    tempPrev->next = tempNext;
    tempNext->prev = tempPrev;
    int value = target->data;
    free(target);
    return value;
}

int deleteValue(CircularDoublyLinkedList *list, int value)
{
    Node *target = search(*list, value);
    if (target == NULL)
        return -1;
    list->length--;
    if (target->next == target)
    {
        free(target);
        list->head = NULL;
        return 0;
    }
    if (target == list->head)
        list->head = target->next;
    Node *tempPrev = target->prev;
    Node *tempNext = target->next;
    tempPrev->next = tempNext;
    tempNext->prev = tempPrev;
    free(target);
    return 0;
}

int main()
{
    CircularDoublyLinkedList head = init();

    insertAtStart(&head, 44);
    insertAtStart(&head, 11);
    insertAtEnd(&head, 55);
    insertAtEnd(&head, 77);
    insertAfterValue(&head, 22, 11);
    insertAfterValue(&head, 66, 55);
    insertAtIndex(&head, 33, 2);
    insertAtIndex(&head, 88, 10);
    traverse(head);

    reverse(&head);
    traverse(head);

    deleteStart(&head);
    deleteEnd(&head);
    deleteIndex(&head, 0);
    deleteValue(&head, 22);
    traverse(head);

    return 0;
}