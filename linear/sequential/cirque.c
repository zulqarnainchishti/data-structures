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
}Cirque;

Cirque init(int n){
    Cirque queue;
    queue.size=n+1;
    queue.array=(int *)malloc((n+1)*sizeof(int));
    queue.front=0;
    queue.rear=0;
    return queue; 
}

Cirque copy(Cirque queue){
    Cirque result=init(queue.size);
    while (queue.front!=queue.rear)
    {
        result.array[result.rear++]=queue.array[queue.front];
        queue.front=(queue.front+1)%queue.size;
    }
    return result;
}

void clear(Cirque *queue){
    queue->front=0;
    queue->rear=0;
}

void delete(Cirque *vector){
    vector->size=0;
    free(vector->array);
    vector->array=NULL;
    vector->front=0;
    vector->rear=0;
}

bool isEmpty(Cirque queue){
    return queue.front==queue.rear;
}

bool isFull(Cirque queue){
    return (queue.rear+1)%queue.size==queue.front;
}

int length(Cirque queue){
    if(queue.rear<queue.front) queue.rear+=queue.size;
    return queue.rear-queue.front;
}

void enqueue(Cirque *queue, int value){
    if(isFull(*queue)){
        printf("Cirque Overflow\n");
        return;
    }
    queue->array[queue->rear]=value;
    queue->rear=(queue->rear+1)%queue->size;
}

int peek(Cirque queue){
    if(isEmpty(queue)){
        printf("Cirque Underflow\n");
        return -1;
    }
    return queue.array[queue.front];
}

int dequeue(Cirque *queue){
    if(isEmpty(*queue)==1){
        printf("Cirque underflow\n");
        return -1;
    }
    int value=queue->array[queue->front];
    queue->front=(queue->front+1)%queue->size;
    return value;
}

void traverse(Cirque queue){
    int len=length(queue);
    printf("[");
    while (queue.front!=queue.rear)
    {
        printf(" %d",queue.array[queue.front]);
        queue.front=(queue.front+1)%queue.size;
    }
    printf(" ] %d/%d\n",len,queue.size-1);
}

void main()
{
    Cirque qyu1=init(5);
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