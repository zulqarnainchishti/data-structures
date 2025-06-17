#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct{
    int *array;
    int front;
    int rear;
    int size;
}Queue;

Queue init(int n){
    Queue queue;
    queue.array=(int *)malloc(n*sizeof(int));
    queue.front=0;
    queue.rear=0;
    queue.size=n;
    return queue; 
}

Queue copy(Queue queue){
    Queue result=init(queue.size);
    for (int i = queue.front; i < queue.rear; i++)
    {
        result.array[result.rear++]=queue.array[i];
    }
    return result;
}

void clear(Queue *queue){
    queue->front=0;
    queue->rear=0;
}

void delete(Queue *vector){
    free(vector->array);
    vector->array=NULL;
    vector->front=0;
    vector->rear=0;
    vector->size=0;
}

bool isEmpty(Queue queue){
    return queue.front==queue.rear;
}

bool isFull(Queue queue){
    return queue.rear==queue.size;
}

int length(Queue queue){
    return queue.rear-queue.front;
}

void enqueue(Queue *queue, int value){
    if(isFull(*queue)){
        printf("Queue Overflow\n");
        return;
    }
    queue->array[queue->rear++]=value;
}

int peek(Queue queue){
    if(isEmpty(queue)){
        printf("Queue Underflow\n");
        return -1;
    }
    return queue.array[queue.front];
}

int dequeue(Queue *queue){
    if(isEmpty(*queue)){
        printf("Queue Underflow\n");
        return -1;
    }
    return queue->array[queue->front++];
}

void traverse(Queue queue){
    int len=length(queue);
    printf("[");
    for (int i = queue.front; i < queue.rear; i++)
    {
        printf(" %d",queue.array[i]);
    }
    printf(" ] : %d/%d\n",len,queue.size);
}

void main()
{
    Queue qyu1=init(5);
    enqueue(&qyu1,11);   
    enqueue(&qyu1,22);
    enqueue(&qyu1,33);
    enqueue(&qyu1,44);   
    traverse(qyu1);

    printf("%d\n",dequeue(&qyu1));
    printf("%d\n",dequeue(&qyu1));
    traverse(qyu1);
    Queue qyu2=copy(qyu1);
    traverse(qyu2);
}