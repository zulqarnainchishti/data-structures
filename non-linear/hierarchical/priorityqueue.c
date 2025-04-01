#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    int data;
    int priority;
}Tuple;

void swap(Tuple *a, Tuple *b){
    Tuple temp = *a;
    *a = *b;
    *b = temp;
}

typedef struct {
    int type;
    Tuple *array;
    int size;
    int length;
}Priorque;

Priorque init(int N, int T){
    // type = -1 : ascending priority
    // type =  1 : descending priority
    Priorque queue;
    queue.type=(T<0)?-1:1;
    queue.array=(Tuple *)malloc(N*sizeof(Tuple));
    queue.size=N;
    queue.length=0;
    return queue;
}

Priorque copy(Priorque queue){
    Priorque copy=init(queue.size,queue.type);
    for (int i = 0; i < queue.length; i++)
    {
        copy.array[copy.length].data=queue.array[i].data;
        copy.array[copy.length++].priority=queue.array[i].priority;
    }
    return copy;
}

void clear(Priorque *queue){
    queue->length=0;
}

void delete(Priorque *queue){
    queue->type=0;
    free(queue->array);
    queue->array=NULL;
    queue->size=0;
    queue->length=0;
}

bool isEmpty(Priorque queue){
    return queue.length==0;
}

bool isFull(Priorque queue){
    return queue.length==queue.size;
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

int heapifyUp(Priorque *queue, int index){
    int chld=index;
    int prnt=parent(chld);
    if(queue->type==-1 && !(queue->array[prnt].priority<=queue->array[chld].priority))
        swap(queue->array+prnt,queue->array+chld);
    else if(queue->type==1 && !(queue->array[prnt].priority>=queue->array[chld].priority))
        swap(queue->array+prnt,queue->array+chld);
    return prnt;
}

int heapifyDown(Priorque *queue, int index){
    int prnt=index;
    int lchld=leftChild(prnt);
    int rchld=rightChild(prnt);
    int chld;
    if(lchld>queue->length-1) return -1;
    else if(rchld>queue->length-1) chld=lchld;
    else{
        if(queue->type==-1) chld=(queue->array[lchld].priority<queue->array[rchld].priority)?lchld:rchld;
        else if(queue->type==1) chld=(queue->array[lchld].priority>queue->array[rchld].priority)?lchld:rchld;
    }
    if(queue->type==-1 && !(queue->array[prnt].priority<=queue->array[chld].priority))
        swap(queue->array+prnt,queue->array+chld);
    else if(queue->type==1 && !(queue->array[prnt].priority>=queue->array[chld].priority))
        swap(queue->array+prnt,queue->array+chld);
    return chld;
}

void enqueue(Priorque *queue, int value, int weight){
    if(isFull(*queue)) return;
    queue->array[queue->length].data=value;
    queue->array[queue->length++].priority=weight;
    int chld=queue->length-1;
    while (chld>0)
    {
        chld=heapifyUp(queue,chld);
    }
}

void change(Priorque *queue, int value, int weight){
    int index=-1;
    for (int i = 0; i < queue->length; i++)
    {
        if(queue->array[i].data==value){
            index=i;
            break;
        }
    }
    if(index==-1) return;
    queue->array[index].priority=weight;
    for (int index = queue->length/2-1; index >= 0; index--)
    {
        int prnt=index;
        while (prnt<queue->length/2)
        { 
            prnt=heapifyDown(queue,prnt);
        }
    }
}

Tuple peek(Priorque queue){
    if(isEmpty(queue)){
        Tuple temp;
        temp.data=-1;
        temp.priority=-1;
        return temp;
    }
    return queue.array[0];
}

Tuple dequeue(Priorque *queue){
        if(isEmpty(*queue)){
        Tuple temp;
        temp.data=-1;
        temp.priority=-1;
        return temp;
    }
    Tuple temp=queue->array[0];
    queue->array[0].data=queue->array[--queue->length].data;
    queue->array[0].priority=queue->array[queue->length].priority;
    int prnt=0;
    while (prnt<queue->length/2)
    { 
        prnt=heapifyDown(queue,prnt);
    }
    return temp;
}

void traverse(Priorque queue){
    queue=copy(queue);
    Tuple temp;
    printf("[ ");
    while (!isEmpty(queue)){
        temp=dequeue(&queue);
        printf("<%d,%d> ",temp.data,temp.priority);
    }
    printf("]\n");
    delete(&queue);
}

int main()
{
    Priorque qyu1=init(10,1);
    enqueue(&qyu1,11,50);  
    enqueue(&qyu1,22,10);
    enqueue(&qyu1,33,30);
    enqueue(&qyu1,44,40);   
    enqueue(&qyu1,55,70);   
    enqueue(&qyu1,66,20);   
    enqueue(&qyu1,77,60);   
    traverse(qyu1);
    printf("peeked = %d\n",peek(qyu1));
    printf("dequeued = %d\n",dequeue(&qyu1));
    change(&qyu1,33,100);
    traverse(qyu1);

    return 0;
}