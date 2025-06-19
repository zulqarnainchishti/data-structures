#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

typedef struct
{
    int type;
    int *array;
    int length;
    int size;
} Heap;

Heap init(int N, int T)
{
    // type = -1 : min heap
    // type =  1 : max heap
    Heap heap;
    heap.type = (T < 0) ? -1 : 1;
    heap.array = (int *)malloc(N * sizeof(int));
    heap.size = N;
    heap.length = 0;
    return heap;
}

void clear(Heap *heap)
{
    heap->length = 0;
}

void delete(Heap *heap)
{
    heap->type = 0;
    free(heap->array);
    heap->array = NULL;
    heap->size = 0;
    heap->length = 0;
}

bool isEmpty(Heap heap)
{
    return heap.length == 0;
}

bool isFull(Heap heap)
{
    return heap.length == heap.size;
}

int search(Heap heap, int value)
{
    for (int i = 0; i < heap.length; i++)
    {
        if (heap.array[i] == value)
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

bool isMinHeap(Heap heap)
{
    for (int chld = 1; chld < heap.length; chld++)
    {
        int prnt = parent(chld);
        if (!(heap.array[prnt] <= heap.array[chld]))
            return false;
    }
    return true;
}

bool isMaxHeap(Heap heap)
{
    for (int chld = 1; chld < heap.length; chld++)
    {
        int prnt = parent(chld);
        if (!(heap.array[prnt] >= heap.array[chld]))
            return false;
    }
    return true;
}

bool isHeap(Heap heap)
{
    // flag = -1 : min  heap
    // flag =  0 : both heap
    // flag =  1 : max  heap
    int flag = 0;
    if (isMinHeap(heap))
        flag += -1;
    if (isMaxHeap(heap))
        flag += 1;
    return flag;
}

void heapifyUp(Heap *heap, int index)
{
    if (index == 0)
        return;
    int prnt = parent(index);
    if (heap->type == -1)
    {
        if (heap->array[prnt] > heap->array[index])
        {
            swap(heap->array + prnt, heap->array + index);
            heapifyUp(heap, prnt);
        }
    }
    else if (heap->type == 1)
    {
        if (heap->array[prnt] < heap->array[index])
        {
            swap(heap->array + prnt, heap->array + index);
            heapifyUp(heap, prnt);
        }
    }
}

void heapifyDown(Heap *heap, int index)
{
    int left = leftChild(index);
    int right = rightChild(index);
    int selected = index;
    if (heap->type == -1)
    {
        if (left < heap->length && heap->array[selected] > heap->array[left])
            selected = left;
        if (right < heap->length && heap->array[selected] > heap->array[right])
            selected = right;
    }
    else if (heap->type == 1)
    {
        if (left < heap->length && heap->array[selected] < heap->array[left])
            selected = left;
        if (right < heap->length && heap->array[selected] < heap->array[right])
            selected = right;
    }
    if (selected != index)
    {
        swap(&heap->array[selected], &heap->array[index]);
        heapifyDown(heap, selected);
    }
}

void insert(Heap *heap, int value)
{
    if (isFull(*heap))
        return;
    heap->array[heap->length++] = value;
    int chld = heap->length - 1;
    heapifyUp(heap, chld);
}

void discard(Heap *heap, int value)
{
    if (isEmpty(*heap))
        return;
    int index = search(*heap, value);
    if (index == -1)
        return;
    heap->array[index] = heap->array[--heap->length];
    int prnt = index;
    heapifyDown(heap, prnt);
}

int peek(Heap heap)
{
    if (isEmpty(heap))
        return -1;
    return heap.array[0];
}

int extract(Heap *heap)
{
    if (isEmpty(*heap))
        return -1;
    int value = heap->array[0];
    heap->array[0] = heap->array[--heap->length];
    int prnt = 0;
    heapifyDown(heap, prnt);
    return value;
}

void traverse(Heap heap)
{
    int levelEnd = 0;
    for (int i = 0; i < heap.length; i++)
    {
        printf("<%d> ", heap.array[i]);
        if (i == levelEnd || i==heap.length-1)
        {
            printf("\n");
            levelEnd = 2 * levelEnd + 2;
        }
    }
}

Heap heapify(int *array, int size, int type)
{
    Heap heap = init(size, type);
    for (int i = 0; i < heap.size; i++)
        heap.array[heap.length++] = array[i];
    for (int prnt = heap.length / 2 - 1; prnt >= 0; prnt--)
        heapifyDown(&heap, prnt);
    return heap;
}

void heapSort(int *array, int size, int order)
{
    // order = -1 : ascending  using min heap
    // order =  1 : descending using max heap
    Heap heap = heapify(array, size, order);
    for (int i = 0; i < size; i++)
        array[i] = extract(&heap);
    delete(&heap);
}

int main()
{
    printf("===== HEAP TEST CASES =====\n");

    // Initialize a max-heap of size 10
    Heap maxHeap = init(10, 1);
    printf("\nMax-Heap initialized (size 10)\n");

    // Insert elements into max-heap
    int elements[] = {20, 5, 15, 30, 10};
    printf("\nInserting elements into Max-Heap: ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", elements[i]);
        insert(&maxHeap, elements[i]);
    }
    printf("\nHeap after insertions:\n");
    traverse(maxHeap);

    // Check peek
    printf("\nTop element (peek): %d\n", peek(maxHeap));

    // Check isEmpty and isFull
    printf("Is heap empty? %s\n", isEmpty(maxHeap) ? "Yes" : "No");
    printf("Is heap full?  %s\n", isFull(maxHeap) ? "Yes" : "No");

    // Check isHeap validation
    printf("Is valid Max-Heap? %s\n", isMaxHeap(maxHeap) ? "Yes" : "No");
    printf("Heap type detected by isHeap(): %d\n", isHeap(maxHeap));

    // Extract top element
    printf("\nExtracting elements from Max-Heap:\n");
    while (!isEmpty(maxHeap)) {
        int top = extract(&maxHeap);
        printf("%d ", top);
    }
    printf("\nIs heap empty after extracts? %s\n", isEmpty(maxHeap) ? "Yes" : "No");

    // Insert new elements
    printf("\nRe-inserting elements for discard test: 50 40 30 20 10\n");
    int newElems[] = {50, 40, 30, 20, 10};
    for (int i = 0; i < 5; i++) insert(&maxHeap, newElems[i]);
    traverse(maxHeap);

    // Discard a middle element
    printf("\nDiscarding value 30\n");
    discard(&maxHeap, 30);
    traverse(maxHeap);

    // Build Min-Heap from array
    int array[] = {45, 35, 25, 15, 5};
    int size = 5;
    Heap minHeap = heapify(array, size, -1);
    printf("\nMin-Heap built from array:\n");
    traverse(minHeap);
    printf("Is valid Min-Heap? %s\n", isMinHeap(minHeap) ? "Yes" : "No");

    // Test heapSort ascending
    int sortArray[] = {9, 4, 7, 1, 5, 3};
    int sortSize = 6;
    printf("\nOriginal array for ascending heapSort: ");
    for (int i = 0; i < sortSize; i++) printf("%d ", sortArray[i]);
    printf("\n");

    heapSort(sortArray, sortSize, -1);
    printf("Sorted array (ascending): ");
    for (int i = 0; i < sortSize; i++) printf("%d ", sortArray[i]);
    printf("\n");

    // Test heapSort descending
    int sortArray2[] = {9, 4, 7, 1, 5, 3};
    printf("\nOriginal array for descending heapSort: ");
    for (int i = 0; i < sortSize; i++) printf("%d ", sortArray2[i]);
    printf("\n");

    heapSort(sortArray2, sortSize, 1);
    printf("Sorted array (descending): ");
    for (int i = 0; i < sortSize; i++) printf("%d ", sortArray2[i]);
    printf("\n");

    // Clean up
    delete(&maxHeap);
    delete(&minHeap);
    printf("\nHeaps deleted and memory freed.\n");

    printf("\n===== END OF TESTS =====\n");
    return 0;
}
