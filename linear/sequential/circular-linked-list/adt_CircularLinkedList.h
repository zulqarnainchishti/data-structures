#ifndef CIRCULAR_LINKED_LIST_H
#define CIRCULAR_LINKED_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * @brief Node structure for circular singly linked list.
 */
typedef struct CLLNode
{
    int data;             /**< Data held by the node */
    struct CLLNode *next; /**< Pointer to the next node */
} CLLNode;

/**
 * @brief Metadata structure for the circular linked list.
 */
typedef struct CircularLinkedList
{
    struct CLLNode *tail; /**< Pointer to the tail node */
    int length;           /**< Number of elements in the list */
} CircularLinkedList;

/**
 * @brief Allocates and initializes a new node with the given value.
 * @param value Integer to store in the node.
 * @return Pointer to the newly created node.
 */
CLLNode *create(int value)
{
    CLLNode *node = (CLLNode *)malloc(sizeof(CLLNode));
    if (!node)
    {
        perror("Failed to create a Node");
        exit(EXIT_FAILURE);
    }
    node->data = value;
    node->next = NULL;
    return node;
}

/**
 * @brief Initializes an empty circular linked list.
 * @return A CircularLinkedList with tail set to NULL and length zero.
 */
CircularLinkedList init()
{
    CircularLinkedList list;
    list.tail = NULL;
    list.length = 0;
    return list;
}

/**
 * @brief Clears the list and deallocates all nodes iteratively.
 * @param list Pointer to the list to clear.
 */
void clear(CircularLinkedList *list)
{
    if (!list || !list->tail)
        return;
    CLLNode *head = list->tail->next;
    list->tail->next = NULL;
    while (head != NULL)
    {
        CLLNode *temp = head;
        head = head->next;
        free(temp);
    }
    list->tail = NULL;
    list->length = 0;
}

/**
 * @brief Creates a deep copy of the circular linked list.
 * @param list Pointer to the source list.
 * @return A new list that is a deep copy of the source.
 */
CircularLinkedList copy(const CircularLinkedList *list)
{
    CircularLinkedList newList = init();
    if (!list || !list->tail)
        return newList;
    CLLNode *curr = list->tail->next;
    do
    {
        CLLNode *node = create(curr->data);
        if (!newList.tail)
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
        curr = curr->next;
    } while (curr != list->tail->next);
    return newList;
}

/**
 * @brief Reverses the list in-place.
 * @param list Pointer to the list to reverse.
 */
void reverse(CircularLinkedList *list)
{
    if (!list || list->length <= 1)
        return;
    CLLNode *head = list->tail->next;
    CLLNode *prevNode = list->tail;
    CLLNode *currNode = head;
    CLLNode *nextNode;
    do
    {
        nextNode = currNode->next;
        currNode->next = prevNode;
        prevNode = currNode;
        currNode = nextNode;
    } while (currNode != head);
    list->tail = head;
    list->tail->next = prevNode;
}

/**
 * @brief Searches for the first node with the given value.
 * @param list Pointer to the list.
 * @param value Value to search for.
 * @return Pointer to the node if found, NULL otherwise.
 */
CLLNode *search(const CircularLinkedList *list, int value)
{
    if (!list || !list->tail)
        return NULL;
    CLLNode *curr = list->tail->next;
    do
    {
        if (curr->data == value)
            return curr;
        curr = curr->next;
    } while (curr != list->tail->next);
    return NULL;
}

/**
 * @brief Prints the list elements to stdout.
 * @param list Pointer to the list.
 */
void traverse(const CircularLinkedList *list)
{
    if (!list || !list->tail)
        return;
    CLLNode *curr = list->tail->next;
    do
    {
        printf("[%d]", curr->data);
        printf(curr == list->tail ? "\n" : " -> ");
        curr = curr->next;
    } while (curr != list->tail->next);
}

/**
 * @brief Inserts a new node at the beginning of the list.
 * @param list Pointer to the list.
 * @param value Value to insert.
 */
void insertAtStart(CircularLinkedList *list, int value)
{
    CLLNode *node = create(value);
    list->length++;
    if (!list->tail)
    {
        node->next = node;
        list->tail = node;
    }
    else
    {
        node->next = list->tail->next;
        list->tail->next = node;
    }
}

/**
 * @brief Inserts a new node at the end of the list.
 * @param list Pointer to the list.
 * @param value Value to insert.
 */
void insertAtEnd(CircularLinkedList *list, int value)
{
    CLLNode *node = create(value);
    list->length++;
    if (!list->tail)
    {
        node->next = node;
        list->tail = node;
    }
    else
    {
        node->next = list->tail->next;
        list->tail->next = node;
        list->tail = node;
    }
}

/**
 * @brief Inserts a new node at a specified index.
 * @param list Pointer to the list.
 * @param value Value to insert.
 * @param index Zero-based index at which to insert.
 */
void insertAtIndex(CircularLinkedList *list, int value, int index)
{
    if (index <= 0)
        return insertAtStart(list, value);
    if (index >= list->length)
        return insertAtEnd(list, value);
    CLLNode *node = create(value);
    list->length++;
    CLLNode *curr = list->tail->next;
    for (int i = 0; i < index - 1; i++)
        curr = curr->next;
    node->next = curr->next;
    curr->next = node;
}

/**
 * @brief Inserts a new value after the first occurrence of a given value.
 * @param list Pointer to the list.
 * @param newVal Value to insert.
 * @param oldVal Value to search and insert after.
 * @return 0 on success, -1 if oldVal not found.
 */
int insertAfterValue(CircularLinkedList *list, int newVal, int oldVal)
{
    CLLNode *target = search(list, oldVal);
    if (target == NULL)
        return -1;
    CLLNode *node = create(newVal);
    list->length++;
    node->next = target->next;
    target->next = node;
    if (list->tail == target)
        list->tail = node;
    return 0;
}

/**
 * @brief Deletes the first node of the list.
 * @param list Pointer to the list.
 * @return Value of the deleted node, or -1 if list is empty.
 */
int deleteStart(CircularLinkedList *list)
{
    if (!list || !list->tail)
        return -1;
    CLLNode *head = list->tail->next;
    int value = head->data;
    list->length--;
    if (head == list->tail)
    {
        free(head);
        list->tail = NULL;
    }
    else
    {
        list->tail->next = head->next;
        free(head);
    }
    return value;
}

/**
 * @brief Deletes the last node of the list.
 * @param list Pointer to the list.
 * @return Value of the deleted node, or -1 if list is empty.
 */
int deleteEnd(CircularLinkedList *list)
{
    if (!list || !list->tail)
        return -1;
    list->length--;
    if (list->tail->next == list->tail)
    {
        int value = list->tail->data;
        free(list->tail);
        list->tail = NULL;
        return value;
    }
    CLLNode *curr = list->tail;
    while (curr->next != list->tail)
        curr = curr->next;
    int value = list->tail->data;
    curr->next = list->tail->next;
    free(list->tail);
    list->tail = curr;
    return value;
}

/**
 * @brief Deletes the node at a given index.
 * @param list Pointer to the list.
 * @param index Zero-based index of the node to delete.
 * @return Value of the deleted node, or -1 on error.
 */
int deleteIndex(CircularLinkedList *list, int index)
{
    if (!list || !list->tail)
        return -1;
    if (index <= 0)
        return deleteStart(list);
    if (index >= list->length - 1)
        return deleteEnd(list);
    CLLNode *curr = list->tail->next;
    for (int i = 0; i < index - 1; i++)
        curr = curr->next;
    CLLNode *target = curr->next;
    int value = target->data;
    curr->next = target->next;
    free(target);
    list->length--;
    return value;
}

/**
 * @brief Deletes the first node with the specified value.
 * @param list Pointer to the list.
 * @param value Value to delete.
 * @return 0 on success, -1 if value not found.
 */
int deleteValue(CircularLinkedList *list, int value)
{
    if (!list || !list->tail)
        return -1;
    CLLNode *prevNode = list->tail;
    CLLNode *currNode = list->tail->next;
    do
    {
        if (currNode->data == value)
        {
            prevNode->next = currNode->next;
            if (currNode == list->tail)
                list->tail = (currNode == prevNode) ? NULL : prevNode;
            free(currNode);
            list->length--;
            return 0;
        }
        prevNode = currNode;
        currNode = currNode->next;
    } while (currNode != list->tail->next);
    return -1;
}

#endif // CIRCULAR_LINKED_LISTH