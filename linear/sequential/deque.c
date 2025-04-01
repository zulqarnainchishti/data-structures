#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void swap(int *a, int *b){
    int temp = *a;
    *a = *b;
    *b = temp;
}

typedef struct{
    int size;
    int *array;
    int front;
    int rear;
}Deque;

Deque init(int n){
    Deque queue;
    queue.size=n;
    queue.array=(int *)malloc(n*sizeof(int));
    queue.front=0;
    queue.rear=0;
    return queue; 
}

Deque copy(Deque queue){
    Deque result=init(queue.size);
    for (int i = queue.front; i < queue.rear; i++)
    {
        result.array[result.rear++]=queue.array[i];
    }
    return result;
}

void clear(Deque *queue){
    queue->front=0;
    queue->rear=0;
}

void delete(Deque *vector){
    vector->size=0;
    free(vector->array);
    vector->array=NULL;
    vector->front=0;
    vector->rear=0;
}

bool isEmpty(Deque queue){
    return queue.front==queue.rear;
}

bool isFrontFull(Deque queue){
    return queue.front==0;
}

bool isRearFull(Deque queue){
    return queue.rear==queue.size;
}

int length(Deque queue){
    return queue.rear-queue.front;
}

void enqueueFront(Deque *queue, int value){
    if(isFrontFull(*queue)){
        printf("Deque Overflow\n");
        return;
    }
    queue->array[--queue->front]=value;
}

int peekFront(Deque queue){
    if(isEmpty(queue)){
        printf("Deque Underflow\n");
        return -1;
    }
    return queue.array[queue.front];
}

int dequeueFront(Deque *queue){
    if(isEmpty(*queue)){
        printf("Deque Underflow\n");
        return -1;
    }
    return queue->array[queue->front++];
}

void enqueueRear(Deque *queue, int value){
    if(isRearFull(*queue)){
        printf("Deque Overflow\n");
        return;
    }
    queue->array[queue->rear++]=value;
}

int peekRear(Deque queue){
    if(isEmpty(queue)){
        printf("Deque Underflow\n");
        return -1;
    }
    return queue.array[queue.rear-1];
}

int dequeueRear(Deque *queue){
    if(isEmpty(*queue)){
        printf("Deque Underflow\n");
        return -1;
    }
    return queue->array[--queue->rear];
}

void traverse(Deque queue){
    printf("[");
    for (int i = queue.front; i < queue.rear; i++)
    {
        printf(" %d",queue.array[i]);
    }
    printf(" ] : %d/%d\n",length(queue),queue.size);
}

int main()
{
    Deque qyu1=init(5);
    enqueueRear(&qyu1,11);   
    enqueueRear(&qyu1,22);
    enqueueRear(&qyu1,33);
    enqueueRear(&qyu1,44);   
    enqueueRear(&qyu1,55);  
    enqueueRear(&qyu1,55);  
    enqueueFront(&qyu1,55);
    traverse(qyu1);

    printf("%d\n",peekFront(qyu1));
    printf("%d\n",peekRear(qyu1));
    printf("%d\n",dequeueFront(&qyu1));
    printf("%d\n",dequeueRear(&qyu1));
    traverse(qyu1);

    enqueueRear(&qyu1,66);   
    enqueueRear(&qyu1,77);
    enqueueFront(&qyu1,77);
    enqueueFront(&qyu1,66);   
    traverse(qyu1);

    printf("%d\n",dequeueFront(&qyu1));
    printf("%d\n",dequeueFront(&qyu1));
    printf("%d\n",dequeueFront(&qyu1));
    printf("%d\n",dequeueFront(&qyu1));
    printf("%d\n",dequeueFront(&qyu1));
    printf("%d\n",dequeueFront(&qyu1));
    printf("%d\n",dequeueRear(&qyu1));

    return 0;
}