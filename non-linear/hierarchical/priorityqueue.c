#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct
{
    int data;
    int priority;
} Tuple;

typedef struct
{
    int type;
    Tuple *array;
    int length;
    int size;
} PriorityQueue;

void swap(Tuple *a, Tuple *b)
{
    Tuple temp = *a;
    *a = *b;
    *b = temp;
}

PriorityQueue init(int N, int T)
{
    // type = -1 : ascending priority queue
    // type =  1 : descending priority queue
    PriorityQueue queue;
    queue.type = (T < 0) ? -1 : 1;
    queue.array = (Tuple *)malloc(N * sizeof(Tuple));
    queue.length = 0;
    queue.size = N;
    return queue;
}

PriorityQueue copy(PriorityQueue queue)
{
    PriorityQueue copy = init(queue.size, queue.type);
    for (int i = 0; i < queue.length; i++)
    {
        copy.array[copy.length].data = queue.array[i].data;
        copy.array[copy.length++].priority = queue.array[i].priority;
    }
    return copy;
}

void clear(PriorityQueue *queue)
{
    queue->length = 0;
}

void delete(PriorityQueue *queue)
{
    queue->type = 0;
    free(queue->array);
    queue->array = NULL;
    queue->length = 0;
    queue->size = 0;
}

bool isEmpty(PriorityQueue queue)
{
    return queue.length == 0;
}

bool isFull(PriorityQueue queue)
{
    return queue.length == queue.size;
}

int search(PriorityQueue queue, int value)
{
    for (int i = 0; i < queue.length; i++)
    {
        if (queue.array[i].data == value)
            return i;
    }
    return -1;
}

int parent(int index)
{
    return (index - 1) / 2;
}

int leftChild(int index)
{
    return 2 * index + 1;
}

int rightChild(int index)
{
    return 2 * index + 2;
}

bool isAscending(PriorityQueue queue)
{
    for (int chld = 1; chld < queue.length; chld++)
    {
        int prnt = parent(chld);
        if (queue.array[prnt].priority > queue.array[chld].priority)
            return false;
    }
    return true;
}

bool isDescending(PriorityQueue queue)
{
    for (int chld = 1; chld < queue.length; chld++)
    {
        int prnt = parent(chld);
        if (queue.array[prnt].priority < queue.array[chld].priority)
            return false;
    }
    return true;
}

void heapifyUp(PriorityQueue *queue, int chld)
{
    if (chld == 0)
        return;
    int prnt = parent(chld);
    if (queue->type == -1)
    {
        if (queue->array[prnt].priority > queue->array[chld].priority)
        {
            swap(queue->array + prnt, queue->array + chld);
            heapifyUp(queue, prnt);
        }
    }
    else if (queue->type == 1)
    {
        if (queue->array[prnt].priority < queue->array[chld].priority)
        {
            swap(queue->array + prnt, queue->array + chld);
            heapifyUp(queue, prnt);
        }
    }
}

int heapifyDown(PriorityQueue *queue, int index)
{
    int prnt = index;
    int lchld = leftChild(prnt);
    int rchld = rightChild(prnt);
    int chld;
    if (lchld > queue->length - 1)
        return -1;
    else if (rchld > queue->length - 1)
        chld = lchld;
    else
    {
        if (queue->type == -1)
            chld = (queue->array[lchld].priority < queue->array[rchld].priority) ? lchld : rchld;
        else if (queue->type == 1)
            chld = (queue->array[lchld].priority > queue->array[rchld].priority) ? lchld : rchld;
    }
    if (queue->type == -1 && !(queue->array[prnt].priority <= queue->array[chld].priority))
        swap(queue->array + prnt, queue->array + chld);
    else if (queue->type == 1 && !(queue->array[prnt].priority >= queue->array[chld].priority))
        swap(queue->array + prnt, queue->array + chld);
    return chld;
}

void heapifyDown(PriorityQueue *queue, int prnt)
{
    int left = leftChild(prnt);
    int right = rightChild(prnt);
    int selected = prnt;
    if (queue->type == -1)
    {
        if (left < queue->length && queue->array[selected].priority > queue->array[left].priority)
            selected = left;
        if (right < queue->length && queue->array[selected].priority > queue->array[right].priority)
            selected = right;
    }
    else if (queue->type == 1)
    {
        if (left < queue->length && queue->array[selected].priority < queue->array[left].priority)
            selected = left;
        if (right < queue->length && queue->array[selected].priority < queue->array[right].priority)
            selected = right;
    }
    if (selected != prnt)
    {
        swap(&queue->array[selected], &queue->array[prnt]);
        heapifyDown(queue, selected);
    }
}

void enqueue(PriorityQueue *queue, int value, int weight)
{
    if (isFull(*queue))
        return;
    queue->array[queue->length].data = value;
    queue->array[queue->length++].priority = weight;
    int chld = queue->length - 1;
    heapifyUp(queue, chld);
}

void update(PriorityQueue *queue, int value, int weight)
{
    int index = -1;
    for (int i = 0; i < queue->length; i++)
    {
        if (queue->array[i].data == value)
        {
            index = i;
            break;
        }
    }
    if (index == -1)
        return;
    queue->array[index].priority = weight;
    int prnt = parent(index);
    if (queue->type == -1)
    {
        if (index > 0 && queue->array[prnt].priority > weight)
            heapifyUp(queue, index);
        else
            heapifyDown(queue, index);
    }
    else if (queue->type == 1)
    {
        if (index > 0 && queue->array[prnt].priority < weight)
            heapifyUp(queue, index);
        else
            heapifyDown(queue, index);
    }
}

int peek(PriorityQueue queue)
{
    if (isEmpty(queue))
        return -1;
    return queue.array[0].data;
}

int dequeue(PriorityQueue *queue)
{
    if (isEmpty(*queue))
        return -1;
    int value = queue->array[0].data;
    queue->array[0].data = queue->array[--queue->length].data;
    queue->array[0].priority = queue->array[queue->length].priority;
    int prnt = 0;
    heapifyDown(queue, prnt);
    return value;
}

void traverse(PriorityQueue queue)
{
    int levelEnd = 0;
    for (int i = 0; i < queue.length; i++)
    {
        printf("(%d,%d) ", queue.array[i].data, queue.array[i].priority);
        if (i == levelEnd || i == queue.length - 1)
        {
            printf("\n");
            levelEnd = 2 * levelEnd + 2;
        }
    }
}

int main()
{
    PriorityQueue qyu1 = init(10, 1);
    enqueue(&qyu1, 11, 50);
    enqueue(&qyu1, 22, 10);
    enqueue(&qyu1, 33, 30);
    enqueue(&qyu1, 44, 40);
    enqueue(&qyu1, 55, 70);
    enqueue(&qyu1, 66, 20);
    enqueue(&qyu1, 77, 60);
    traverse(qyu1);
    printf("peeked = %d\n", peek(qyu1));
    printf("dequeued = %d\n", dequeue(&qyu1));
    change(&qyu1, 33, 100);
    traverse(qyu1);

    return 0;
}