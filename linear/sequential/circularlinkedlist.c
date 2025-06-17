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

void clear(CircularLinkedList *list)
{
    if (list->tail == NULL)
        return;
    Node *tempPrev = NULL;
    Node *tempNext = list->tail->next;
    list->tail->next = NULL;
    while (tempNext != NULL)
    {
        tempPrev = tempNext;
        tempNext = tempNext->next;
        free(tempPrev);
    }
    list->tail = NULL;
    list->length = 0;
}

CircularLinkedList copy(CircularLinkedList list)
{
    CircularLinkedList new = init();
    if (list.tail == NULL)
        return new;
    new.length = list.length;
    Node *head = list.tail->next, *temp;
    while (true)
    {
        if (new.tail == NULL)
        {
            new.tail = create(head->data);
            temp = new.tail;
        }
        else
        {
            new.tail->next = create(head->data);
            new.tail = new.tail->next;
        }
        if (head == list.tail)
        {
            new.tail->next = temp;
            break;
        }
        head = head->next;
    }
    return new;
}

Node *search(CircularLinkedList list, int value)
{
    if (list.tail == NULL)
        return NULL;
    Node *temp = list.tail;
    while (true)
    {
        if (temp->data == value)
            return temp;
        temp = temp->next;
        if (temp == list.tail)
            break;
    }
    return NULL;
}

void reverse(CircularLinkedList *list)
{
    if (list->tail == NULL || list->tail == list->tail->next)
        return;
    Node *tempPrev = NULL;
    Node *tempCurr = list->tail->next;
    Node *tempNext = tempCurr->next;
    while (true)
    {
        tempCurr->next = tempPrev;
        if (tempCurr == list->tail)
            break;
        tempPrev = tempCurr;
        tempCurr = tempNext;
        tempNext = tempNext->next;
    }
    list->tail->next = tempNext;
}

void traverse(CircularLinkedList list)
{
    if (list.tail == NULL)
        return;
    Node *head = list.tail->next;
    while (true)
    {
        printf("[%d]", head->data);
        if (head == list.tail)
        {
            printf("\n");
            break;
        }
        else
        {
            printf(" -> ");
        }
        head = head->next;
    }
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
    Node *temp1 = NULL;
    Node *temp2 = list->tail->next;
    for (int i = 0; i < index; i++)
    {
        temp1 = temp2;
        temp2 = temp2->next;
    }
    temp1->next = node;
    node->next = temp2;
}

void insertAfterValue(CircularLinkedList *list, int new, int old)
{
    Node *temp = search(*list, old);
    if (temp == NULL)
        return;
    Node *node = create(new);
    list->length++;
    node->next = temp->next;
    temp->next = node;
    if (temp == list->tail)
        list->tail = node;
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
    Node *head = list->tail->next;
    list->tail->next = head->next;
    int value = head->data;
    free(head);
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
    Node *temp = list->tail;
    while (temp->next != list->tail)
        temp = temp->next;
    temp->next = list->tail->next;
    int value = list->tail->data;
    free(list->tail);
    list->tail = temp;
    return value;
}

int deleteIndex(CircularLinkedList *list, int index)
{
    if (index <= 0)
        return deleteStart(list);
    if (index >= list->length - 1)
        return deleteEnd(list);
    Node *temp1 = NULL;
    Node *temp2 = list->tail->next;
    for (int i = 0; i < index; i++)
    {
        temp1 = temp2;
        temp2 = temp2->next;
    }
    temp1->next = temp2->next;
    int value = temp2->data;
    free(temp2);
    list->length--;
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
            int value = list->tail->data;
            free(list->tail);
            list->tail = NULL;
            list->length--;
            return value;
        }
        return -1;
    }
    Node *tempPrev = list->tail;
    Node *tempCurr = list->tail->next;
    while (tempCurr->data != value)
    {
        tempPrev = tempCurr;
        tempCurr = tempCurr->next;
        if (tempPrev == list->tail)
            return -1;
    }
    tempPrev->next = tempCurr->next;
    if (tempCurr == list->tail)
        list->tail = tempPrev;
    free(tempCurr);
    list->length--;
    return value;
}

void main()
{
    CircularLinkedList head=init();
    insertAtStart(&head, 44);
    insertAtStart(&head, 11);
    insertAtEnd(&head, 22);
    insertAtEnd(&head, 66);
    insertAtIndex(&head, 55, 2);
    insertAtIndex(&head, 33, 4);
    insertAfterValue(&head, 77, 11);
    insertAfterValue(&head, 88, 66);
    printf("len %d\n",head.length);
    traverse(head);
    
    printf("%d\n",deleteValue(&head, 11));
    printf("len %d\n",head.length);
    traverse(head);
    printf("%d\n",deleteValue(&head, 22));
    printf("len %d\n",head.length);
    traverse(head);
    printf("%d\n",deleteValue(&head, 33));
    printf("len %d\n",head.length);
    traverse(head);
    printf("%d\n",deleteValue(&head, 44));
    printf("len %d\n",head.length);
    traverse(head);
    printf("%d\n",deleteValue(&head, 55));
    printf("len %d\n",head.length);
    traverse(head);
    printf("%d\n",deleteValue(&head, 66));
    printf("len %d\n",head.length);
    traverse(head);
    printf("%d\n",deleteValue(&head, 77));
    printf("len %d\n",head.length);
    traverse(head);
    printf("%d\n",deleteValue(&head, 88));
    printf("len %d\n",head.length);
    traverse(head);
}