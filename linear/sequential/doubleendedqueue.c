#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct{
    int *array;
    int front;
    int rear;
    int size;
}DoubleEndedQueue;

DoubleEndedQueue init(int n){
    DoubleEndedQueue queue;
    queue.array=(int *)malloc(n*sizeof(int));
    queue.front=0;
    queue.rear=0;
    queue.size=n;
    return queue; 
}

DoubleEndedQueue copy(DoubleEndedQueue queue){
    DoubleEndedQueue result=init(queue.size);
    for (int i = queue.front; i < queue.rear; i++)
    {
        result.array[result.rear++]=queue.array[i];
    }
    return result;
}

void clear(DoubleEndedQueue *queue){
    queue->front=0;
    queue->rear=0;
}

void delete(DoubleEndedQueue *vector){
    free(vector->array);
    vector->array=NULL;
    vector->front=0;
    vector->rear=0;
    vector->size=0;
}

bool isEmpty(DoubleEndedQueue queue){
    return queue.front==queue.rear;
}

bool isFrontFull(DoubleEndedQueue queue){
    return queue.front==0;
}

bool isRearFull(DoubleEndedQueue queue){
    return queue.rear==queue.size;
}

int length(DoubleEndedQueue queue){
    return queue.rear-queue.front;
}

void enqueueFront(DoubleEndedQueue *queue, int value){
    if(isFrontFull(*queue)){
        printf("DoubleEndedQueue Overflow\n");
        return;
    }
    queue->array[--queue->front]=value;
}

int peekFront(DoubleEndedQueue queue){
    if(isEmpty(queue)){
        printf("DoubleEndedQueue Underflow\n");
        return -1;
    }
    return queue.array[queue.front];
}

int dequeueFront(DoubleEndedQueue *queue){
    if(isEmpty(*queue)){
        printf("DoubleEndedQueue Underflow\n");
        return -1;
    }
    return queue->array[queue->front++];
}

void enqueueRear(DoubleEndedQueue *queue, int value){
    if(isRearFull(*queue)){
        printf("DoubleEndedQueue Overflow\n");
        return;
    }
    queue->array[queue->rear++]=value;
}

int peekRear(DoubleEndedQueue queue){
    if(isEmpty(queue)){
        printf("DoubleEndedQueue Underflow\n");
        return -1;
    }
    return queue.array[queue.rear-1];
}

int dequeueRear(DoubleEndedQueue *queue){
    if(isEmpty(*queue)){
        printf("DoubleEndedQueue Underflow\n");
        return -1;
    }
    return queue->array[--queue->rear];
}

void traverse(DoubleEndedQueue queue){
    printf("[");
    for (int i = queue.front; i < queue.rear; i++)
    {
        printf(" %d",queue.array[i]);
    }
    printf(" ] : %d/%d\n",length(queue),queue.size);
}

int main()
{
    DoubleEndedQueue qyu1=init(5);
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