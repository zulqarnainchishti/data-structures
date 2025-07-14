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

void clearRecursive(Node *node)
{
    if (node == NULL)
        return;
    clearRecursive(node->next);
    free(node);
}

void clear(DoublyLinkedList *list)
{
    clearRecursive(list->head);
    list->head = NULL;
    list->tail = NULL;
    list->length = 0;
}

Node *copyRecursive(Node *node, Node *prevNode, Node **tailNode)
{
    if (node == NULL)
    {
        *tailNode = prevNode;
        return NULL;
    }
    Node *newNode = create(node->data);
    newNode->prev = prevNode;
    newNode->next = copyRecursive(node->next, newNode, tailNode);
    return newNode;
}

DoublyLinkedList copy(DoublyLinkedList list)
{
    DoublyLinkedList newList = init();
    newList.head = copyRecursive(list.head, NULL, &newList.tail);
    newList.length = list.length;
    return newList;
}

void reverse(DoublyLinkedList *list)
{
    if (list->head == NULL || list->head == list->tail)
        return;
    Node *temp = NULL;
    Node *currNode = list->head;
    while (currNode != NULL)
    {
        temp = currNode->prev;
        currNode->prev = currNode->next;
        currNode->next = temp;
        currNode = currNode->prev;
    }
    temp = list->head;
    list->head = list->tail;
    list->tail = temp;
}

Node *search(DoublyLinkedList list, int value)
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

void traverse(DoublyLinkedList list)
{
    Node *temp = list.head;
    while (temp != NULL)
    {
        printf("[%d]", temp->data);
        temp = temp->next;
        printf((temp == NULL) ? "\n" : " <=> ");
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
    Node *temp = list->head;
    for (int i = 0; i < index - 1; i++)
        temp = temp->next;
    node->next = temp->next;
    temp->next->prev = node;
    node->prev = temp;
    temp->next = node;
}

int insertAfterValue(DoublyLinkedList *list, int new, int old)
{
    if (list->head == NULL)
        return -1;
    Node *temp;
    if (list->tail->data == old)
        temp = list->tail;
    else
        temp = search(*list, old);
    if (temp == NULL)
        return -1;
    Node *node = create(new);
    list->length++;
    node->next = temp->next;
    if (temp == list->tail)
        list->tail = node;
    else
        temp->next->prev = node;
    node->prev = temp;
    temp->next = node;
    return 0;
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
    Node *target = list->head;
    int value = target->data;
    list->head = list->head->next;
    list->head->prev = NULL;
    free(target);
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
    Node *target = list->tail;
    int value = target->data;
    list->tail = list->tail->prev;
    list->tail->next = NULL;
    free(target);
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
    Node *temp = list->head;
    for (int i = 0; i < index; i++)
        temp = temp->next;
    temp->prev->next = temp->next;
    temp->next->prev = temp->prev;
    int value = temp->data;
    free(temp);
    return value;
}

int deleteValue(DoublyLinkedList *list, int value)
{
    if (list->head == NULL)
        return -1;
    if (list->head->data == value)
        return (deleteStart(list) == -1) ? -1 : 0;
    if (list->tail->data == value)
        return (deleteEnd(list) == -1) ? -1 : 0;
    Node *temp = search(*list, value);
    if (temp == NULL)
        return -1;
    temp->prev->next = temp->next;
    temp->next->prev = temp->prev;
    free(temp);
    return 0;
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
    deleteIndex(&head, 0);
    deleteValue(&head, 22);
    traverse(head);

    return 0;
}