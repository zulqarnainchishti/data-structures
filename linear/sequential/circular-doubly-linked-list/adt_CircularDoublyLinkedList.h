#ifndef CIRCULAR_DOUBLY_LINKED_LIST_H
#define CIRCULAR_DOUBLY_LINKED_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * @brief Represents a node in the Circular Doubly Linked List.
 */
typedef struct Node
{
    int data;          ///< The integer data stored in the node.
    struct Node *prev; ///< Pointer to the previous node in the list.
    struct Node *next; ///< Pointer to the next node in the list.
} CDLLNode;

/**
 * @brief Represents the Circular Doubly Linked List ADT.
 */
typedef struct
{
    CDLLNode *head; ///< Pointer to the first node in the list.
    int length;     ///< Current number of nodes in the list.
} CircularDoublyLinkedList;

/**
 * @brief Creates and initializes a new CDLLNode.
 * @param value The integer data for the node.
 * @return A pointer to the newly created CDLLNode. Exits on memory allocation failure.
 */
CDLLNode *create(int value)
{
    CDLLNode *node = (CDLLNode *)malloc(sizeof(CDLLNode));
    if (!node)
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
 * @brief Initializes an empty CircularDoublyLinkedList.
 * @return An initialized CircularDoublyLinkedList structure.
 */
CircularDoublyLinkedList init()
{
    CircularDoublyLinkedList list;
    list.head = NULL;
    list.length = 0;
    return list;
}

/**
 * @brief Clears all nodes from the list, freeing memory.
 * @param list Pointer to the CircularDoublyLinkedList to clear.
 */
void clear(CircularDoublyLinkedList *list)
{
    if (!list || !list->head)
        return;
    CDLLNode *curr = list->head;
    CDLLNode *tail = curr->prev;
    tail->next = NULL; // Break circular link to enable linear traversal
    while (curr != NULL)
    {
        CDLLNode *temp = curr;
        curr = curr->next;
        free(temp);
    }
    list->head = NULL;
    list->length = 0;
}

/**
 * @brief Creates a deep copy of a CircularDoublyLinkedList.
 * @param list Pointer to the source CircularDoublyLinkedList.
 * @return A new CircularDoublyLinkedList that is a deep copy.
 */
CircularDoublyLinkedList copy(const CircularDoublyLinkedList *list)
{
    CircularDoublyLinkedList newList = init();
    if (!list || !list->head)
        return newList;
    CDLLNode *temp = list->head;
    CDLLNode *newHead = NULL;
    CDLLNode *newTail = NULL;
    do
    {
        CDLLNode *newNode = create(temp->data);
        if (newHead == NULL)
        {
            newHead = newNode;
            newTail = newNode;
        }
        else
        {
            newTail->next = newNode;
            newNode->prev = newTail;
            newTail = newNode;
        }
        newList.length++;
        temp = temp->next;
    } while (temp != list->head);
    newHead->prev = newTail; // Establish circularity for new list
    newTail->next = newHead;
    newList.head = newHead;
    return newList;
}

/**
 * @brief Reverses the order of nodes in the list in-place.
 * @param list Pointer to the CircularDoublyLinkedList to reverse.
 */
void reverse(CircularDoublyLinkedList *list)
{
    if (!list || list->length <= 1)
        return;
    CDLLNode *currNode = list->head;
    CDLLNode *nextNode;
    do
    {
        nextNode = currNode->next;       // Save next
        currNode->next = currNode->prev; // Reverse next pointer
        currNode->prev = nextNode;       // Reverse prev pointer
        currNode = nextNode;             // Move to next node (original next)
    } while (currNode != list->head);
    list->head = list->head->next; // Update head to original tail (new head)
}

/**
 * @brief Searches for a specific value in the list.
 * @param list Pointer to the CircularDoublyLinkedList to search.
 * @param value The integer value to search for.
 * @return A pointer to the node if found, otherwise NULL.
 */
CDLLNode *search(const CircularDoublyLinkedList *list, int value)
{
    if (!list || !list->head)
        return NULL;
    CDLLNode *temp = list->head;
    do
    {
        if (temp->data == value)
            return temp;
        temp = temp->next;
    } while (temp != list->head);
    return NULL;
}

/**
 * @brief Traverses and prints all elements in the list.
 * @param list Pointer to the CircularDoublyLinkedList to traverse.
 */
void traverse(CircularDoublyLinkedList *list)
{
    if (!list || !list->head)
    {
        printf("[empty]\n"); // Indicate empty list
        return;
    }
    CDLLNode *temp = list->head;
    do
    {
        printf("[%d]", temp->data);
        temp = temp->next;
        printf(temp == list->head ? "\n" : " <=> ");
    } while (temp != list->head);
}

/**
 * @brief Inserts a new node at the beginning of the list.
 * @param list Pointer to the CircularDoublyLinkedList.
 * @param value The integer data to insert.
 */
void insertAtStart(CircularDoublyLinkedList *list, int value)
{
    CDLLNode *node = create(value);
    list->length++;
    if (list->head == NULL) // Empty list case
    {
        list->head = node;
        node->prev = node;
        node->next = node;
        return;
    }
    CDLLNode *tail = list->head->prev;
    node->prev = tail;
    node->next = list->head;
    list->head->prev = node;
    tail->next = node;
    list->head = node; // New node becomes the head
}

/**
 * @brief Inserts a new node at the end of the list.
 * @param list Pointer to the CircularDoublyLinkedList.
 * @param value The integer data to insert.
 */
void insertAtEnd(CircularDoublyLinkedList *list, int value)
{
    CDLLNode *node = create(value);
    list->length++;
    if (list->head == NULL) // Empty list case
    {
        list->head = node;
        node->prev = node;
        node->next = node;
        return;
    }
    CDLLNode *tail = list->head->prev;
    node->prev = tail;
    node->next = list->head;
    list->head->prev = node;
    tail->next = node;
    // Head remains unchanged
}

/**
 * @brief Inserts a new node at a specified index.
 * Delegates to insertAtStart or insertAtEnd for boundary cases.
 * @param list Pointer to the CircularDoublyLinkedList.
 * @param value The integer data to insert.
 * @param index The zero-based index for insertion.
 */
void insertAtIndex(CircularDoublyLinkedList *list, int value, int index)
{
    if (index <= 0)
        return insertAtStart(list, value);
    if (index >= list->length)
        return insertAtEnd(list, value);
    CDLLNode *node = create(value);
    list->length++;
    CDLLNode *tempPrev = list->head;
    for (int i = 0; i < index - 1; i++) // Traverse to node before insertion point
        tempPrev = tempPrev->next;
    CDLLNode *tempNext = tempPrev->next;
    node->prev = tempPrev;
    node->next = tempNext;
    tempNext->prev = node;
    tempPrev->next = node;
}

/**
 * @brief Inserts a new node after the first occurrence of a given value.
 * @param list Pointer to the CircularDoublyLinkedList.
 * @param newVal The integer data for the new node.
 * @param oldVal The value after which to insert.
 * @return 0 on success, -1 if oldVal is not found.
 */
int insertAfterValue(CircularDoublyLinkedList *list, int newVal, int oldVal)
{
    CDLLNode *tempPrev = search(list, oldVal); // Find the node
    if (tempPrev == NULL)
        return -1; // Value not found
    CDLLNode *node = create(newVal);
    list->length++;
    CDLLNode *tempNext = tempPrev->next;
    node->prev = tempPrev;
    node->next = tempNext;
    tempPrev->next = node;
    tempNext->prev = node;
    return 0;
}

/**
 * @brief Deletes the first node from the list.
 * @param list Pointer to the CircularDoublyLinkedList.
 * @return The data of the deleted node, or -1 if the list is empty.
 */
int deleteStart(CircularDoublyLinkedList *list)
{
    if (!list || !list->head)
        return -1;
    CDLLNode *target = list->head;
    int value = target->data;
    list->length--;
    if (list->head == list->head->next) // Single node case
    {
        free(target);
        list->head = NULL;
        return value;
    }
    CDLLNode *tail = target->prev; // Get current tail
    list->head = target->next;     // New head
    list->head->prev = tail;       // Update new head's prev
    tail->next = list->head;       // Update tail's next
    free(target);
    return value;
}

/**
 * @brief Deletes the last node from the list.
 * @param list Pointer to the CircularDoublyLinkedList.
 * @return The data of the deleted node, or -1 if the list is empty.
 */
int deleteEnd(CircularDoublyLinkedList *list)
{
    if (!list || !list->head)
        return -1;
    CDLLNode *target = list->head->prev; // Target is the current tail
    int value = target->data;
    list->length--;
    if (list->head == list->head->next) // Single node case
    {
        free(target);
        list->head = NULL;
        return value;
    }
    CDLLNode *newTail = target->prev; // Node before the current tail
    list->head->prev = newTail;       // Head points to new tail
    newTail->next = list->head;       // New tail points to head
    free(target);
    return value;
}

/**
 * @brief Deletes the node at a specified index.
 * Delegates to deleteStart or deleteEnd for boundary cases.
 * @param list Pointer to the CircularDoublyLinkedList.
 * @param index The zero-based index of the node to delete.
 * @return The data of the deleted node, or -1 if the list is empty or index is invalid.
 */
int deleteIndex(CircularDoublyLinkedList *list, int index)
{
    if (!list || !list->head)
        return -1;
    if (index <= 0)
        return deleteStart(list);
    if (index >= list->length - 1)
        return deleteEnd(list);
    CDLLNode *target = list->head;
    for (int i = 0; i < index; i++) // Traverse to the node at the index
        target = target->next;
    CDLLNode *prevNode = target->prev;
    CDLLNode *nextNode = target->next;
    prevNode->next = nextNode; // Re-link prev node
    nextNode->prev = prevNode; // Re-link next node
    int value = target->data;
    free(target);
    list->length--;
    return value;
}

/**
 * @brief Deletes the first occurrence of a specified value from the list.
 * @param list Pointer to the CircularDoublyLinkedList.
 * @param value The integer value to delete.
 * @return 0 on success, -1 if the value is not found or the list is empty.
 */
int deleteValue(CircularDoublyLinkedList *list, int value)
{
    CDLLNode *target = search(list, value); // Find the node
    if (target == NULL)
        return -1; // Value not found
    list->length--;
    if (target->next == target) // Single node case (target is the only node)
    {
        free(target);
        list->head = NULL;
        return 0;
    }
    CDLLNode *prevNode = target->prev;
    CDLLNode *nextNode = target->next;
    prevNode->next = nextNode; // Re-link prev node
    nextNode->prev = prevNode; // Re-link next node
    if (target == list->head)  // If head was deleted, update head
        list->head = nextNode;
    free(target);
    return 0;
}

#endif // CIRCULAR_DOUBLY_LINKED_LIST_H