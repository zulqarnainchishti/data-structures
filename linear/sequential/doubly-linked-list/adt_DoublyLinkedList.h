#ifndef DOUBLYLINKEDLIST_H
#define DOUBLYLINKEDLIST_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * @brief Represents a node in the Doubly Linked List.
 */
typedef struct DLLNode
{
    int data;             /**< The integer data stored in the node. */
    struct DLLNode *prev; /**< Pointer to the previous node in the list. */
    struct DLLNode *next; /**< Pointer to the next node in the list. */
} DLLNode;

/**
 * @brief Represents a Doubly Linked List.
 */
typedef struct
{
    DLLNode *head; /**< Pointer to the first node of the list. */
    DLLNode *tail; /**< Pointer to the last node of the list. */
    int length;    /**< The number of nodes currently in the list. */
} DoublyLinkedList;

/**
 * @brief Creates a new DLLNode with the given value.
 * @param value The integer data to be stored in the new node.
 * @return A pointer to the newly created DLLNode. Exits if memory allocation fails.
 */
static inline DLLNode *create(const int value)
{
    DLLNode *node = (DLLNode *)malloc(sizeof(DLLNode));
    if (node == NULL)
    {
        perror("Failed to create a Node");
        exit(EXIT_FAILURE);
    }
    node->data = value;
    node->prev = NULL;
    node->next = NULL;
    return node;
}

/**
 * @brief Initializes an empty Doubly Linked List.
 * @return An initialized DoublyLinkedList structure with head and tail set to NULL and length to 0.
 */
static inline DoublyLinkedList init()
{
    DoublyLinkedList list;
    list.head = NULL;
    list.tail = NULL;
    list.length = 0;
    return list;
}

/**
 * @brief Recursively frees all nodes starting from the given node.
 * @param node The starting node from which to clear.
 */
static inline void __recClear__(DLLNode *node)
{
    if (node == NULL)
        return;
    __recClear__(node->next);
    free(node);
}

/**
 * @brief Clears all nodes from the Doubly Linked List and resets its state.
 * @param list A pointer to the DoublyLinkedList to be cleared.
 */
static inline void clear(DoublyLinkedList *list)
{
    if (list == NULL)
        return;
    __recClear__(list->head);
    list->head = NULL;
    list->tail = NULL;
    list->length = 0;
}

/**
 * @brief Recursively copies a Doubly Linked List from a source node.
 * @param node The current node being copied from the source list.
 * @param prev The pointer to the previously created node in the new list.
 * @param tail A double pointer to the tail of the new list, updated during recursion.
 * @return A pointer to the newly created node in the copied list.
 */
static inline DLLNode *__recCopy__(const DLLNode *node, DLLNode *prev, DLLNode **tail)
{
    if (node == NULL)
    {
        *tail = prev;
        return NULL;
    }
    DLLNode *newNode = create(node->data);
    newNode->prev = prev;
    newNode->next = __recCopy__(node->next, newNode, tail);
    return newNode;
}

/**
 * @brief Creates a deep copy of an existing Doubly Linked List.
 * @param list A pointer to the DoublyLinkedList to be copied.
 * @return A new DoublyLinkedList that is an exact copy of the source list.
 */
static inline DoublyLinkedList copy(const DoublyLinkedList *list)
{
    DoublyLinkedList newList = init();
    if (list == NULL)
        return newList;
    newList.head = __recCopy__(list->head, NULL, &newList.tail);
    newList.length = list->length;
    return newList;
}

/**
 * @brief Reverses the order of elements in the Doubly Linked List.
 * @param list A pointer to the DoublyLinkedList to be reversed.
 */
static inline void reverse(DoublyLinkedList *list)
{
    if (list == NULL || list->length <= 1)
        return;
    DLLNode *currNode = list->head;
    DLLNode *nextNode;
    while (currNode != NULL)
    {
        nextNode = currNode->prev;
        currNode->prev = currNode->next;
        currNode->next = nextNode;
        currNode = currNode->prev;
    }
    nextNode = list->head;
    list->head = list->tail;
    list->tail = nextNode;
}

/**
 * @brief Searches for a specific value within the Doubly Linked List.
 * @param list A pointer to the DoublyLinkedList to search within.
 * @param value The integer value to search for.
 * @return A pointer to the first DLLNode containing the value, or NULL if not found or list is empty.
 */
static inline DLLNode *search(const DoublyLinkedList *list, const int value)
{
    if (list == NULL)
        return NULL;
    DLLNode *curr = list->head;
    while (curr != NULL)
    {
        if (curr->data == value)
            return curr;
        curr = curr->next;
    }
    return NULL;
}

/**
 * @brief Traverses and prints the elements of the Doubly Linked List.
 * @param list A pointer to the DoublyLinkedList to traverse.
 */
static inline void traverse(const DoublyLinkedList *list)
{
    if (list == NULL)
        return;
    const DLLNode *curr = list->head;
    while (curr != NULL)
    {
        printf("[%d]", curr->data);
        curr = curr->next;
        printf((curr == NULL) ? "\n" : " <=> ");
    }
}

/**
 * @brief Inserts a new node with the given value at the beginning of the list.
 * @param list A pointer to the DoublyLinkedList to modify.
 * @param value The integer data to insert.
 */
static inline void insertAtStart(DoublyLinkedList *list, const int value)
{
    if (list == NULL)
        return;
    DLLNode *node = create(value);
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

/**
 * @brief Inserts a new node with the given value at the end of the list.
 * @param list A pointer to the DoublyLinkedList to modify.
 * @param value The integer data to insert.
 */
static inline void insertAtEnd(DoublyLinkedList *list, const int value)
{
    if (list == NULL)
        return;
    DLLNode *node = create(value);
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

/**
 * @brief Inserts a new node with the given value at a specific index in the list.
 * If index is <= 0, it inserts at the start. If index is >= length, it inserts at the end.
 * @param list A pointer to the DoublyLinkedList to modify.
 * @param value The integer data to insert.
 * @param index The zero-based index at which to insert the new node.
 */
static inline void insertAtIndex(DoublyLinkedList *list, const int value, const int index)
{
    if (list == NULL)
        return;
    if (index <= 0)
        return insertAtStart(list, value);
    if (index >= list->length)
        return insertAtEnd(list, value);
    DLLNode *node = create(value);
    list->length++;
    DLLNode *temp = list->head;
    for (int i = 0; i < index - 1; i++)
        temp = temp->next;
    node->next = temp->next;
    temp->next->prev = node;
    node->prev = temp;
    temp->next = node;
}

/**
 * @brief Inserts a new node with a given value after the first occurrence of an old value.
 * @param list A pointer to the DoublyLinkedList to modify.
 * @param newVal The integer data of the new node to insert.
 * @param oldVal The integer value after which the new node should be inserted.
 * @return 0 if successful, -1 if oldVal is not found in the list.
 */
static inline int insertAfterValue(DoublyLinkedList *list, const int newVal, const int oldVal)
{
    DLLNode *target = search(list, oldVal);
    if (target == NULL)
        return -1;
    DLLNode *node = create(newVal);
    list->length++;
    node->prev = target;
    node->next = target->next;
    if (target->next != NULL)
        target->next->prev = node;
    else
        list->tail = node;
    target->next = node;
    return 0;
}

/**
 * @brief Deletes the first node from the Doubly Linked List.
 * @param list A pointer to the DoublyLinkedList to modify.
 * @return The data of the deleted node, or -1 if the list is empty or invalid.
 */
static inline int deleteStart(DoublyLinkedList *list)
{
    if (list == NULL || list->head == NULL)
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
    DLLNode *target = list->head;
    int value = target->data;
    list->head = list->head->next;
    list->head->prev = NULL;
    free(target);
    return value;
}

/**
 * @brief Deletes the last node from the Doubly Linked List.
 * @param list A pointer to the DoublyLinkedList to modify.
 * @return The data of the deleted node, or -1 if the list is empty or invalid.
 */
static inline int deleteEnd(DoublyLinkedList *list)
{
    if (list == NULL || list->head == NULL)
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
    DLLNode *target = list->tail;
    int value = target->data;
    list->tail = list->tail->prev;
    list->tail->next = NULL;
    free(target);
    return value;
}

/**
 * @brief Deletes the node at a specific index from the Doubly Linked List.
 * If index is <= 0, it deletes from the start. If index is >= length - 1, it deletes from the end.
 * @param list A pointer to the DoublyLinkedList to modify.
 * @param index The zero-based index of the node to delete.
 * @return The data of the deleted node, or -1 if the list is empty or invalid index.
 */
static inline int deleteIndex(DoublyLinkedList *list, const int index)
{
    if (list == NULL || list->head == NULL)
        return -1;
    if (index <= 0)
        return deleteStart(list);
    if (index >= list->length - 1)
        return deleteEnd(list);
    list->length--;
    DLLNode *temp = list->head;
    for (int i = 0; i < index; i++)
        temp = temp->next;
    temp->prev->next = temp->next;
    temp->next->prev = temp->prev;
    int value = temp->data;
    free(temp);
    return value;
}

/**
 * @brief Deletes the first occurrence of a node with a specific value from the Doubly Linked List.
 * @param list A pointer to the DoublyLinkedList to modify.
 * @param value The integer value of the node to delete.
 * @return 0 if successful, -1 if the value is not found or the list is empty/invalid.
 */
static inline int deleteValue(DoublyLinkedList *list, const int value)
{
    if (list == NULL || list->head == NULL)
        return -1;
    if (list->head->data == value)
        return (deleteStart(list) == -1) ? -1 : 0;
    if (list->tail->data == value)
        return (deleteEnd(list) == -1) ? -1 : 0;
    DLLNode *temp = search(list, value);
    if (temp == NULL)
        return -1;
    temp->prev->next = temp->next;
    temp->next->prev = temp->prev;
    free(temp);
    list->length--;
    return 0;
}

#endif // DOUBLYLINKEDLIST_H