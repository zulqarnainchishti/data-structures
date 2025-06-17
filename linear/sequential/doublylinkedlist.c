#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Node
{
    int data;
    struct Node *prev;
    struct Node *next;
} Node;

typedef struct DoublyLinkedList
{
    struct Node *head;
    struct Node *tail;
    int length;
} DoublyLinkedList;

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

Node *create(int value)
{
    Node *node = (Node *)malloc(sizeof(Node));
    node->data = value;
    node->prev = NULL;
    node->next = NULL;
    return node;
}

DoublyLinkedList init()
{
    DoublyLinkedList list;
    list.head = NULL;
    list.tail = NULL;
    list.length = 0;
    return list;
}

void clear(DoublyLinkedList *list)
{
    Node *tempPrev = NULL;
    Node *tempCurr = list->head;
    while (tempCurr != NULL)
    {
        tempPrev = tempCurr;
        tempCurr = tempCurr->next;
        free(tempPrev);
    }
    list->head = NULL;
    list->tail = NULL;
    list->length = 0;
}

DoublyLinkedList copy(DoublyLinkedList list)
{
    DoublyLinkedList new = init();
    if (list.head == NULL)
        return new;
    while (list.head != NULL)
    {
        Node *temp = create(list.head->data);
        if (new.head == NULL)
        {
            new.head = temp;
            new.tail = temp;
        }
        else
        {
            new.tail->next = temp;
            temp->prev = new.tail;
            new.tail = temp;
        }
        list.head = list.head->next;
    }
    new.length = list.length;
    return new;
}

Node *search(DoublyLinkedList list, int value)
{
    while (list.head != NULL)
    {
        if (list.head->data == value)
            return list.head;
        list.head = list.head->next;
    }
    return NULL;
}

void reverse(DoublyLinkedList *list)
{
    if (list->head == NULL || list->head == list->tail)
        return;
    Node *oldHead = list->head;
    Node *oldTail = list->tail;
    Node *tempPrev = NULL;
    Node *tempCurr = list->head;
    while (tempCurr != NULL)
    {
        Node *tempNext = tempCurr->next;
        tempCurr->prev = tempNext;
        tempCurr->next = tempPrev;
        tempPrev = tempCurr;
        tempCurr = tempNext;
    }
    list->head = oldTail;
    list->tail = oldHead;
}

void traverse(DoublyLinkedList list)
{
    while (list.head != NULL)
    {
        printf("[%d] ", list.head->data);
        list.head = list.head->next;
        printf((list.head != NULL) ? "<=> " : "\n");
    }
}

void insertAtStart(DoublyLinkedList *list, int value)
{
    Node *node = create(value);
    list->length++;
    if (list->head == NULL)
    {
        list->head = node;
        list->tail = node;
        return;
    }
    node->next = list->head;
    list->head->prev = node;
    list->head = node;
}

void insertAtEnd(DoublyLinkedList *list, int value)
{
    Node *node = create(value);
    list->length++;
    if (list->head == NULL)
    {
        list->head = node;
        list->tail = node;
        return;
    }
    list->tail->next = node;
    node->prev = list->tail;
    list->tail = node;
}

void insertAtIndex(DoublyLinkedList *list, int value, int index)
{
    if (index <= 0)
        return insertAtStart(list, value);
    if (index >= list->length)
        return insertAtEnd(list, value);
    Node *node = create(value);
    list->length++;
    Node *tempNext = list->head;
    for (int i = 0; i < index; i++)
        tempNext = tempNext->next;
    Node *tempPrev = tempNext->prev;
    tempPrev->next = node;
    node->prev = tempPrev;
    node->next = tempNext;
    tempNext->prev = node;
}

void insertAfterValue(DoublyLinkedList *list, int new, int old)
{
    Node *tempPrev = search(*list, old);
    if (tempPrev == NULL)
        return;
    Node *node = create(new);
    list->length++;
    Node *tempNext = tempPrev->next;
    node->prev = tempPrev;
    tempPrev->next = node;
    if (tempPrev != list->tail)
    {
        tempNext->prev = node;
        node->next = tempNext;
    }
    else
    {
        list->tail = node;
    }
}

int deleteStart(DoublyLinkedList *list)
{
    if (list->head == NULL)
        return -1;
    list->length--;
    if (list->head == list->tail)
    {
        int value = list->head->data;
        free(list->head);
        list->head = NULL;
        list->tail = NULL;
        return value;
    }
    Node *temp = list->head;
    list->head = list->head->next;
    list->head->prev = NULL;
    int value = temp->data;
    free(temp);
    return value;
}

int deleteEnd(DoublyLinkedList *list)
{
    if (list->head == NULL)
        return -1;
    list->length--;
    if (list->head == list->tail)
    {
        int value = list->head->data;
        free(list->head);
        list->head = NULL;
        list->tail = NULL;
        return value;
    }
    Node *temp = list->tail;
    list->tail = list->tail->prev;
    list->tail->next = NULL;
    int value = temp->data;
    free(temp);
    return value;
}

int deleteIndex(DoublyLinkedList *list, int index)
{
    if (list->head == NULL)
        return -1;
    if (index <= 0)
        return deleteStart(list);
    if (index >= list->length - 1)
        return deleteEnd(list);
    list->length--;
    Node *tempCurr = list->head;
    for (int i = 0; i < index; i++)
        tempCurr = tempCurr->next;
    Node *tempPrev = tempCurr->prev;
    Node *tempNext = tempCurr->next;
    tempPrev->next = tempNext;
    tempNext->prev = tempPrev;
    int value = tempCurr->data;
    free(tempCurr);
    return value;
}

int deleteValue(DoublyLinkedList *list, int value)
{
    if (list->head == NULL)
        return -1;
    if (list->head == list->tail)
    {
        if (list->head->data == value)
        {
            free(list->head);
            list->length--;
            list->head = NULL;
            list->tail = NULL;
            return value;
        }
        return -1;
    }
    Node *tempCurr = search(*list, value);
    if (tempCurr == NULL)
        return -1;
    list->length--;
    Node *tempPrev = tempCurr->prev;
    Node *tempNext = tempCurr->next;
    if (tempCurr == list->head)
    {
        list->head = tempNext;
        tempNext->prev = NULL;
    }
    else if (tempCurr == list->tail)
    {
        list->tail = tempPrev;
        tempPrev->next = NULL;
    }
    else
    {
        tempPrev->next = tempNext;
        tempNext->prev = tempPrev;
    }
    free(tempCurr);
    return value;
}

int main()
{
    DoublyLinkedList head = init();

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
    deleteIndex(&head,0);
    deleteValue(&head,22);
    traverse(head);

    return 0;
}