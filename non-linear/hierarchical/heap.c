#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void swap(int *a,int *b){
    int temp=*a;
    *a=*b;
    *b=temp;
}

typedef struct {
    int type;
    int *array;
    int size;
    int length;
}Heap;

Heap init(int N, int T){
    // type = -1 : min heap
    // type =  1 : max heap
    Heap heap;
    heap.type=(T<0)?-1:1;
    heap.array=(int *)malloc(N*sizeof(int));
    heap.size=N;
    heap.length=0;
    return heap;
}

void clear(Heap *heap){
    heap->length=0;
}

void delete(Heap *heap){
    heap->type=0;
    free(heap->array);
    heap->array=NULL;
    heap->size=0;
    heap->length=0;
}

bool isEmpty(Heap heap){
    return heap.length<=0;
}

bool isFull(Heap heap){
    return heap.length>=heap.size;
}

int search(Heap heap, int value){
    for (int i = 0; i < heap.length; i++)
    {
        if(heap.array[i]==value) return i;
    }
    return -1;
}

int parent(int index){
    return (index-1)/2;
}

int leftChild(int index){
    return 2*index+1;
}

int rightChild(int index){
    return 2*index+2;
}

bool isMinHeap(Heap heap){
    for (int chld=1; chld<heap.length; chld++)
    {
        int prnt=parent(chld);
        if(!(heap.array[prnt]<=heap.array[chld])) return false;
    }
    return true;
}

bool isMaxHeap(Heap heap){
    for (int chld=1; chld<heap.length; chld++)
    {
        int prnt=parent(chld);
        if(!(heap.array[prnt]>=heap.array[chld])) return false;
    }
    return true;
}

int heapifyUp(Heap *heap, int index){
    int chld=index;
    int prnt=parent(chld);
    if(heap->type==-1 && !(heap->array[prnt]<=heap->array[chld]))
        swap(heap->array+prnt,heap->array+chld);
    else if(heap->type==1 && !(heap->array[prnt]>=heap->array[chld]))
        swap(heap->array+prnt,heap->array+chld);
    return prnt;
}

int heapifyDown(Heap *heap, int index){
    int prnt=index;
    int lchld=leftChild(prnt);
    int rchld=rightChild(prnt);
    int chld;
    if(lchld>heap->length-1) return -1;
    else if(rchld>heap->length-1) chld=lchld;
    else{
        if(heap->type==-1) chld=(heap->array[lchld]<heap->array[rchld])?lchld:rchld;
        else if(heap->type==1) chld=(heap->array[lchld]>heap->array[rchld])?lchld:rchld;
    }
    if(heap->type==-1 && !(heap->array[prnt]<=heap->array[chld]))
        swap(heap->array+prnt,heap->array+chld);
    else if(heap->type==1 && !(heap->array[prnt]>=heap->array[chld]))
        swap(heap->array+prnt,heap->array+chld);
    return chld;
}

void insert(Heap *heap, int value){
    if(isFull(*heap)) return;
    heap->array[heap->length++]=value;
    int chld=heap->length-1;
    while (chld>0)
    {
        chld=heapifyUp(heap,chld);
    }
}

void discard(Heap *heap, int value){
    int index=search(*heap,value);
    if(isEmpty(*heap) || index==-1) return;
    heap->array[index]=heap->array[--heap->length];
    int prnt=index;
    while (prnt<heap->length/2)
    { 
        prnt=heapifyDown(heap,prnt);
    }
}

int peek(Heap heap){
    if(isEmpty(heap)) return -1;
    return heap.array[0];
}

int extract(Heap *heap){
    if(isEmpty(*heap)) return -1;
    int value=heap->array[0];
    heap->array[0]=heap->array[--heap->length];
    int prnt=0;
    while (prnt<heap->length/2)
    { 
        prnt=heapifyDown(heap,prnt);
    }
    return value;
}

void traverse(Heap heap){
    int temp=2;
    for (int i = 0; i < heap.length; i++)
    {
        printf("<%d> ",heap.array[i]);
        if(i==temp-2 || i==heap.length-1){
            printf("\n");
            temp*=2;
        }
    }
}

Heap heapify(int *array, int size, int type){
    Heap heap=init(size,type);
    for (int i = 0; i < heap.size; i++)
    {
        heap.array[heap.length++]=array[i];
    }
    for (int index = heap.length/2-1; index >= 0; index--)
    {
        int prnt=index;
        while (prnt<heap.length/2 && heap.type>0)
        { 
            prnt=heapifyDown(&heap,prnt);
        }
    }
    return heap;
}

void heapSort(int *array, int size, int order){
    // order = -1 : ascending  using min heap
    // order =  1 : descending using max heap
    Heap heap=heapify(array,size,order);
    for (int i = 0; i < size; i++)
    {
        array[i]=extract(&heap);
    }
    delete(&heap);
}

int main()
{
    Heap qyu=init(10,2);
    insert(&qyu,0);
    insert(&qyu,19);
    insert(&qyu,34);
    insert(&qyu,47);
    insert(&qyu,11);
    insert(&qyu,63);
    insert(&qyu,87);
    traverse(qyu);
    while (!isEmpty(qyu))
    {
        printf("%d > ",extract(&qyu));
    }
    return 0;
}