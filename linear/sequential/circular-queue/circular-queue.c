#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct{
    int *array;
    int front;
    int rear;
    int size;
}CircularQueue;

CircularQueue init(int n){
    CircularQueue queue;
    queue.array=(int *)malloc((n+1)*sizeof(int));
    queue.front=0;
    queue.rear=0;
    queue.size=n+1;
    return queue; 
}

CircularQueue copy(CircularQueue queue){
    CircularQueue result=init(queue.size);
    while (queue.front!=queue.rear)
    {
        result.array[result.rear++]=queue.array[queue.front];
        queue.front=(queue.front+1)%queue.size;
    }
    return result;
}

void clear(CircularQueue *queue){
    queue->front=0;
    queue->rear=0;
}

void delete(CircularQueue *queue){
    free(queue->array);
    queue->array=NULL;
    queue->front=0;
    queue->rear=0;
    queue->size=0;
}

bool isEmpty(CircularQueue queue){
    return queue.front==queue.rear;
}

bool isFull(CircularQueue queue){
    return (queue.rear+1)%queue.size==queue.front;
}

int length(CircularQueue queue){
    if(queue.rear<queue.front) queue.rear+=queue.size;
    return queue.rear-queue.front;
}

void enqueue(CircularQueue *queue, int value){
    if(isFull(*queue)){
        printf("CircularQueue Overflow\n");
        return;
    }
    queue->array[queue->rear]=value;
    queue->rear=(queue->rear+1)%queue->size;
}

int peek(CircularQueue queue){
    if(isEmpty(queue)){
        printf("CircularQueue Underflow\n");
        return -1;
    }
    return queue.array[queue.front];
}

int dequeue(CircularQueue *queue){
    if(isEmpty(*queue)==1){
        printf("CircularQueue underflow\n");
        return -1;
    }
    int value=queue->array[queue->front];
    queue->front=(queue->front+1)%queue->size;
    return value;
}

void traverse(CircularQueue queue){
    int len=length(queue);
    printf("[");
    while (queue.front!=queue.rear)
    {
        printf(" %d",queue.array[queue.front]);
        queue.front=(queue.front+1)%queue.size;
    }
    printf(" ] : %d/%d\n",len,queue.size-1);
}

void main()
{
    CircularQueue qyu1=init(5);
    enqueue(&qyu1,11);   
    enqueue(&qyu1,22);
    enqueue(&qyu1,33);
    enqueue(&qyu1,44);
    enqueue(&qyu1,55);
    traverse(qyu1);
    printf("%d\n",dequeue(&qyu1));
    printf("%d\n",dequeue(&qyu1));
    traverse(qyu1);
    enqueue(&qyu1,66);
    enqueue(&qyu1,77);
    enqueue(&qyu1,88);
    traverse(qyu1);
    printf("%d\n",dequeue(&qyu1));
    printf("%d\n",dequeue(&qyu1));
    printf("%d\n",dequeue(&qyu1));
    printf("%d\n",dequeue(&qyu1));
    printf("%d\n",dequeue(&qyu1));
    printf("%d\n",dequeue(&qyu1));
    traverse(qyu1);
    enqueue(&qyu1,12);
    enqueue(&qyu1,23);
    enqueue(&qyu1,34);
    enqueue(&qyu1,45);
    traverse(qyu1);
}