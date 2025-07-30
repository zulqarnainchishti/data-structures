#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * @brief Represents a single node in the linked list.
 */
typedef struct LLNode
{
    int data;            /**< The integer data stored in the node. */
    struct LLNode *next; /**< Pointer to the next node in the list. */
} LLNode;

/**
 * @brief Represents a singly linked list.
 */
typedef struct LinkedList
{
    struct LLNode *head; /**< Pointer to the head (first) node of the list. */
    int length;          /**< The number of nodes in the list. */
} LinkedList;

/**
 * @brief Creates a new linked list node with the given value.
 * @param value The integer value to store in the new node.
 * @return A pointer to the newly created LLNode.
 */
LLNode *create(int value)
{
    LLNode *node = (LLNode *)malloc(sizeof(LLNode));
    if (node == NULL)
    {
        perror("Failed to create a Node");
        exit(EXIT_FAILURE);
    }
    node->data = value;
    node->next = NULL;
    return node;
}

/**
 * @brief Initializes an empty linked list.
 * @return An initialized LinkedList structure with head set to NULL and length to 0.
 */
LinkedList init()
{
    LinkedList list;
    list.head = NULL;
    list.length = 0;
    return list;
}

/**
 * @brief Recursively frees all nodes in a linked list.
 * This is a helper function typically used by `clear`.
 * @param node The current node to free.
 */
void __recClear__(LLNode *node)
{
    if (node == NULL)
        return;
    __recClear__(node->next);
    free(node);
}

/**
 * @brief Clears all elements from the linked list, freeing allocated memory.
 * @param list A pointer to the LinkedList to clear.
 */
void clear(LinkedList *list)
{
    if (list == NULL)
        return;
    __recClear__(list->head);
    list->head = NULL;
    list->length = 0;
}

/**
 * @brief Recursively copies a linked list from a given node.
 * This is a helper function typically used by `copy`.
 * @param node The current node to copy.
 * @return A pointer to the head of the newly copied sub-list.
 */
LLNode *__recCopy__(const LLNode *node)
{
    if (node == NULL)
        return NULL;
    LLNode *newLLNode = create(node->data);
    newLLNode->next = __recCopy__(node->next);
    return newLLNode;
}

/**
 * @brief Creates a deep copy of an existing linked list.
 * @param list A pointer to the LinkedList to copy.
 * @return A new LinkedList that is a duplicate of the original.
 */
LinkedList copy(const LinkedList *list)
{
    LinkedList newList = init();
    newList.head = __recCopy__(list->head);
    newList.length = list->length;
    return newList;
}

/**
 * @brief Reverses the order of elements in the linked list.
 * @param list A pointer to the LinkedList to reverse.
 */
void reverse(LinkedList *list)
{
    if (list == NULL || list->length <= 1)
        return;
    LLNode *prevLLNode = NULL;
    LLNode *currLLNode = list->head;
    LLNode *nextLLNode;
    while (currLLNode != NULL)
    {
        nextLLNode = currLLNode->next;
        currLLNode->next = prevLLNode;
        prevLLNode = currLLNode;
        currLLNode = nextLLNode;
    }
    list->head = prevLLNode;
}

/**
 * @brief Searches for a specific value within the linked list.
 * @param list A pointer to the LinkedList to search.
 * @param value The integer value to search for.
 * @return A pointer to the LLNode containing the value if found, otherwise NULL.
 */
LLNode *search(const LinkedList *list, int value)
{
    LLNode *temp = list->head;
    while (temp != NULL)
    {
        if (temp->data == value)
            return temp;
        temp = temp->next;
    }
    return NULL;
}

/**
 * @brief Traverses and prints all elements of the linked list.
 * @param list A pointer to the LinkedList to traverse.
 */
void traverse(const LinkedList *list)
{
    LLNode *temp = list->head;
    while (temp != NULL)
    {
        printf("[%d]", temp->data);
        temp = temp->next;
        printf((temp == NULL) ? "\n" : " -> ");
    }
}

/**
 * @brief Inserts a new node with the given value at the beginning of the list.
 * @param list A pointer to the LinkedList.
 * @param value The integer value to insert.
 */
void insertAtStart(LinkedList *list, int value)
{
    if (list == NULL)
        return;
    LLNode *node = create(value);
    list->length++;
    node->next = list->head;
    list->head = node;
}

/**
 * @brief Inserts a new node with the given value at the end of the list.
 * @param list A pointer to the LinkedList.
 * @param value The integer value to insert.
 */
void insertAtEnd(LinkedList *list, int value)
{
    if (list == NULL)
        return;
    LLNode *node = create(value);
    list->length++;
    if (list->head == NULL)
    {
        list->head = node;
        return;
    }
    LLNode *temp = list->head;
    while (temp->next != NULL)
        temp = temp->next;
    temp->next = node;
}

/**
 * @brief Inserts a new node with the given value at a specified index in the list.
 * If index is <= 0, inserts at the start. If index is >= length, inserts at the end.
 * @param list A pointer to the LinkedList.
 * @param value The integer value to insert.
 * @param index The zero-based index at which to insert the new node.
 */
void insertAtIndex(LinkedList *list, int value, int index)
{
    if (list == NULL)
        return;
    if (index <= 0)
    {
        insertAtStart(list, value);
        return;
    }
    if (index >= list->length)
    {
        insertAtEnd(list, value);
        return;
    }
    LLNode *node = create(value);
    list->length++;
    LLNode *temp = list->head;
    for (int i = 0; i < index - 1; i++)
        temp = temp->next;
    node->next = temp->next;
    temp->next = node;
}

/**
 * @brief Inserts a new node with a given value after the first occurrence of another specified value.
 * @param list A pointer to the LinkedList.
 * @param newVal The integer value to insert.
 * @param oldVal The integer value after which to insert the new node.
 * @return 0 if insertion was successful, -1 if `oldVal` was not found or list is NULL.
 */
int insertAfterValue(LinkedList *list, int newVal, int oldVal)
{
    if (list == NULL)
        return -1;
    LLNode *temp = search(list, oldVal);
    if (temp == NULL)
        return -1;
    LLNode *node = create(newVal);
    list->length++;
    node->next = temp->next;
    temp->next = node;
    return 0;
}

/**
 * @brief Deletes the first node from the linked list.
 * @param list A pointer to the LinkedList.
 * @return The integer data of the deleted node, or -1 if the list is empty or NULL.
 */
int deleteStart(LinkedList *list)
{
    if (list == NULL || list->head == NULL)
        return -1;
    LLNode *target = list->head;
    int value = target->data;
    list->head = target->next;
    free(target);
    list->length--;
    return value;
}

/**
 * @brief Deletes the last node from the linked list.
 * @param list A pointer to the LinkedList.
 * @return The integer data of the deleted node, or -1 if the list is empty or NULL.
 */
int deleteEnd(LinkedList *list)
{
    if (list == NULL || list->head == NULL)
        return -1;
    if (list->head->next == NULL)
    {
        int value = list->head->data;
        free(list->head);
        list->head = NULL;
        list->length--;
        return value;
    }
    LLNode *temp = list->head;
    while (temp->next->next != NULL)
        temp = temp->next;
    LLNode *target = temp->next;
    int value = target->data;
    temp->next = NULL;
    free(target);
    list->length--;
    return value;
}

/**
 * @brief Deletes the node at a specified index from the linked list.
 * If index is <= 0, deletes from the start. If index is >= length - 1, deletes from the end.
 * @param list A pointer to the LinkedList.
 * @param index The zero-based index of the node to delete.
 * @return The integer data of the deleted node, or -1 if the list is empty or NULL.
 */
int deleteIndex(LinkedList *list, int index)
{
    if (list == NULL || list->head == NULL)
        return -1;
    if (index <= 0)
        return deleteStart(list);
    if (index >= list->length - 1)
        return deleteEnd(list);
    LLNode *temp = list->head;
    for (int i = 0; i < index - 1; i++)
        temp = temp->next;
    LLNode *target = temp->next;
    int value = target->data;
    temp->next = target->next;
    free(target);
    list->length--;
    return value;
}

/**
 * @brief Deletes the first occurrence of a node with a specific value from the linked list.
 * @param list A pointer to the LinkedList.
 * @param value The integer value of the node to delete.
 * @return 0 if deletion was successful, -1 if the value was not found, list is empty or NULL.
 */
int deleteValue(LinkedList *list, int value)
{
    if (list == NULL || list->head == NULL)
        return -1;
    if (list->head->data == value)
    {
        LLNode *target = list->head;
        list->head = list->head->next;
        free(target);
        list->length--;
        return 0;
    }
    LLNode *prevLLNode = list->head;
    LLNode *currLLNode = list->head->next;
    while (currLLNode != NULL)
    {
        if (currLLNode->data == value)
        {
            prevLLNode->next = currLLNode->next;
            free(currLLNode);
            list->length--;
            return 0;
        }
        prevLLNode = currLLNode;
        currLLNode = currLLNode->next;
    }
    return -1;
}

#endif // LINKEDLIST_H