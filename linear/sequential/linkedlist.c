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

void clear(LinkedList *list)
{
    Node *tempPrev = NULL;
    Node *tempNext = list->head;
    while (tempNext != NULL)
    {
        tempPrev = tempNext;
        tempNext = tempNext->next;
        free(tempPrev);
    }
    list->head = NULL;
    list->length = 0;
}

LinkedList copy(LinkedList list)
{
    LinkedList new = init();
    new.length=list.length;
    Node *temp;
    while (list.head != NULL)
    {
        if (new.head == NULL)
        {
            new.head = create(list.head->data);
            temp = new.head;
        }
        else
        {
            temp->next = create(list.head->data);
            temp = temp->next;
        }
        list.head = list.head->next;
    }
    return new;
}

Node *search(LinkedList list, int value)
{
    while (list.head != NULL)
    {
        if (list.head->data == value)
            return list.head;
        list.head = list.head->next;
    }
    return NULL;
}

void reverse(LinkedList *list)
{
    Node *tempPrev = NULL;
    Node *tempCurr = list->head;
    Node *tempNext;
    while (tempCurr != NULL)
    {
        tempNext = tempCurr->next;
        tempCurr->next = tempPrev;
        tempPrev = tempCurr;
        tempCurr = tempNext;
    }
    list->head = tempPrev;
}

void traverse(LinkedList list)
{
    while (list.head != NULL)
    {
        printf("[%d]", list.head->data);
        list.head = list.head->next;
        printf((list.head != NULL) ? " -> " : "\n");
    }
}

void insertAtStart(LinkedList *list, int value)
{
    Node *node = create(value);
    node->next = list->head;
    list->head = node;
    list->length++;
}

void insertAtEnd(LinkedList *list, int value)
{
    Node *node = create(value);
    if (list->head == NULL)
    {
        list->head = node;
        return;
    }
    Node *temp = list->head;
    while (temp->next != NULL)
        temp = temp->next;
    temp->next = node;
    list->length++;
}

void insertAtIndex(LinkedList *list, int value, int index)
{
    if (index <= 0)
        return insertAtStart(list, value);
    if (index >= list->length)
        return insertAtEnd(list, value);
    Node *node = create(value);
    Node *temp = list->head;
    for (int i = 1; i < index; i++)
        temp = temp->next;
    node->next = temp->next;
    temp->next = node;
    list->length++;
}

void insertAfterValue(LinkedList *list, int new, int old)
{
    Node *temp = search(*list, old);
    if (temp == NULL)
        return;
    Node *node = create(new);
    node->next = temp->next;
    temp->next = node;
    list->length++;
}

int deleteStart(LinkedList *list)
{
    if (list->head == NULL)
        return -1;
    Node *tempCurr = list->head;
    list->head = list->head->next;
    list->length--;
    int value = tempCurr->data;
    free(tempCurr);
    return value;
}

int deleteEnd(LinkedList *list)
{
    if (list->head == NULL)
        return -1;
    Node *tempPrev = NULL;
    Node *tempCurr = list->head;
    while (tempCurr->next != NULL)
    {
        tempPrev = tempCurr;
        tempCurr = tempCurr->next;
    }
    if (tempPrev == NULL)
        list->head = NULL;
    else
        tempPrev->next = NULL;
    int value = tempCurr->data;
    free(tempCurr);
    list->length--;
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
    Node *tempPrev = NULL;
    Node *tempCurr = list->head;
    for (int i = 0; i < index; i++)
    {
        tempPrev = tempCurr;
        tempCurr = tempCurr->next;
    }
    tempPrev->next = tempCurr->next;
    int value = tempCurr->data;
    free(tempCurr);
    list->length--;
    return value;
}

int deleteValue(LinkedList *list, int value)
{
    if (list->head == NULL)
        return -1;
    Node *tempPrev = NULL;
    Node *tempCurr = list->head;
    while (tempCurr != NULL && tempCurr->data != value)
    {
        tempPrev = tempCurr;
        tempCurr = tempCurr->next;
    }
    if (tempCurr == NULL)
        return -1;
    if (tempPrev == NULL)
        list->head = tempCurr->next;
    else
        tempPrev->next = tempCurr->next;
    free(tempCurr);
    list->length--;
    return value;
}

int main()
{
    LinkedList list = init();

    insertAtEnd(&list, 33);
    insertAtEnd(&list, 44);
    insertAtStart(&list, 22);
    insertAtStart(&list, 11);
    insertAtIndex(&list, 55, 0);
    insertAtIndex(&list, 66, 10);
    insertAfterValue(&list, 77, 55);
    insertAfterValue(&list, 88, 22);
    insertAfterValue(&list, 99, 66);
    traverse(list);

    deleteValue(&list, 99);
    traverse(list);
    deleteStart(&list);
    traverse(list);
    deleteEnd(&list);
    traverse(list);
    deleteIndex(&list,3);
    traverse(list);
    reverse(&list);
    traverse(list);

    LinkedList new=copy(list);
    traverse(new);

    return 0;
}