#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Node
{
    int data;
    struct Node *prev;
    struct Node *next;
} Node;

typedef struct CircularDoublyLinkedList
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

void clear(CircularDoublyLinkedList *list)
{
    if (list->head == NULL)
        return;
    Node *tempPrev = list->head->prev;
    Node *tempCurr = list->head;
    tempPrev->next = NULL;
    tempCurr->prev = NULL;
    while (tempCurr != NULL)
    {
        tempPrev = tempCurr;
        tempCurr = tempCurr->next;
        free(tempPrev);
    }
    list->head = NULL;
    list->length = 0;
}

CircularDoublyLinkedList copy(CircularDoublyLinkedList list)
{
    CircularDoublyLinkedList new = init();
    if (list.head == NULL)
        return new;
    Node *temp1 = list.head, *temp2;
    do
    {
        Node *node = create(temp1->data);
        if (new.head == NULL)
        {
            new.head = node;
            temp2 = new.head;
        }
        else
        {
            temp2->next = node;
            node->prev = temp2;
            temp2 = node;
        }
        temp1 = temp1->next;
    } while (temp1 != list.head);
    temp2->next = new.head;
    new.head->prev = temp2;
    return new;
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

void reverse(CircularDoublyLinkedList *list)
{
    if (list->head == NULL || list->head == list->head->next)
        return;
    Node *tempCurr = list->head;
    Node *tempNext;
    do
    {
        tempNext = tempCurr->next;
        tempCurr->next = tempCurr->prev;
        tempCurr->prev = tempNext;
        tempCurr = tempNext;
    } while (tempCurr != list->head);
    list->head = list->head->next;
}

void traverse(CircularDoublyLinkedList list)
{
    if (list.head == NULL)
        return;
    Node *temp = list.head;
    do
    {
        printf("[%d] ", temp->data);
        temp = temp->next;
        printf((temp != list.head) ? "<=> " : "\n");
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
    node->prev = list->head->prev;
    node->next = list->head;
    list->head->prev = node;
    node->prev->next = node;
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
    node->next = list->head;
    node->prev = list->head->prev;
    list->head->prev = node;
    node->prev->next = node;
}

void insertAtIndex(CircularDoublyLinkedList *list, int value, int index)
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

void insertAfterValue(CircularDoublyLinkedList *list, int new, int old)
{
    Node *tempPrev = search(*list, old);
    if (tempPrev == NULL)
        return;
    Node *node = create(new);
    list->length++;
    Node *tempNext = tempPrev->next;
    tempPrev->next = node;
    node->prev = tempPrev;
    node->next = tempNext;
    tempNext->prev = node;
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
    Node *tempCurr = list->head;
    Node *tempPrev = tempCurr->prev;
    Node *tempNext = tempCurr->next;
    tempPrev->next = tempNext;
    tempNext->prev = tempPrev;
    list->head = tempNext;
    int value = tempCurr->data;
    free(tempCurr);
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
    Node *tempCurr = list->head->prev;
    Node *tempPrev = tempCurr->prev;
    Node *tempNext = tempCurr->next;
    tempPrev->next = tempNext;
    tempNext->prev = tempPrev;
    int value = tempCurr->data;
    free(tempCurr);
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

int deleteValue(CircularDoublyLinkedList *list, int value)
{
    if (list->head == NULL)
        return -1;
    if (list->head == list->head->next)
    {
        if (list->head->data == value)
        {
            free(list->head);
            list->length--;
            list->head = NULL;
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
    tempPrev->next = tempNext;
    tempNext->prev = tempPrev;
    if (tempCurr == list->head)
        list->head = tempNext;
    free(tempCurr);
    return value;
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