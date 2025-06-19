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
    printf("=== Comprehensive PriorityQueue Manual Test Suite ===\n\n");

    // Phase 1: Initialization
    PriorityQueue q = init(6, -1);
    printf("Initialized queue with capacity 6 (min-heap)\n");

    // Phase 2: Insertion + HeapifyUp
    enqueue(&q, 10, 3);
    enqueue(&q, 20, 5);
    enqueue(&q, 30, 1);
    enqueue(&q, 40, 4);
    enqueue(&q, 50, 2);
    printf("Inserted 5 elements (data,priority): (10,3), (20,5), (30,1), (40,4), (50,2)\n");

    // Phase 3: Search + Peek
    int idx = search(q, 20);
    printf("Search for value 20: %s (index %d)\n", (idx != -1) ? "FOUND" : "NOT FOUND", idx);

    printf("Peek (root element data): %d\n", peek(q));

    // Phase 4: Traversal
    printf("Traversing queue (data,priority) by levels:\n");
    traverse(q);

    // Phase 5: isEmpty, isFull
    printf("Queue is %s\n", isEmpty(q) ? "EMPTY" : "NOT EMPTY");
    printf("Queue is %s\n", isFull(q) ? "FULL" : "NOT FULL");

    // Phase 6: Update (which triggers HeapifyUp/Down)
    printf("Updating value 20's priority to 0 (should move to root)\n");
    update(&q, 20, 0);
    printf("After update:\n");
    traverse(q);
    printf("Peek now: %d\n", peek(q));

    // Phase 7: isAscending / isDescending
    printf("Queue is %s an ascending priority queue\n", isAscending(q) ? "" : "NOT");
    printf("Queue is %s a descending priority queue\n", isDescending(q) ? "" : "NOT");

    // Phase 8: Dequeue (extract min) and test heapifyDown
    int removed = dequeue(&q);
    printf("Dequeued element (should be 20): %d\n", removed);
    printf("After dequeue:\n");
    traverse(q);

    // Phase 9: Copy Queue
    PriorityQueue q2 = copy(q);
    printf("Copied queue contents:\n");
    traverse(q2);

    // Phase 10: Clear
    printf("Clearing original queue...\n");
    clear(&q);
    printf("Original queue is now %s\n", isEmpty(q) ? "EMPTY" : "NOT EMPTY");

    // Phase 11: Delete both queues
    delete(&q);
    printf("Deleted original queue\n");
    delete(&q2);
    printf("Deleted copied queue\n");

    printf("\n=== All manual operations and checks executed successfully ===\n");

    return 0;
}