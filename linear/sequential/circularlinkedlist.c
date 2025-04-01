#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void swap(int *a, int *b){
    int temp = *a;
    *a = *b;
    *b = temp;
}

typedef struct Node{
    int data;
    struct Node *next;
}Node;

Node *init(int value){
    Node *node=(Node *)malloc(sizeof(Node));
    node->data=value;
    node->next=NULL;
    return node; 
}

Node *find(Node *head, int value){
    Node *temp1=NULL;
    Node *temp2=head;
    while (temp2->data!=value)
    {
        temp2=temp2->next;
        if(temp2==head) break;
    }
    if(temp2->data==value) temp1=temp2;
    return temp1;
}

void clear(Node **head){
    Node *temp1=NULL;
    Node *temp2=(*head)->next;
    (*head)->next=NULL;
    *head=NULL;
    while(temp2!=NULL){
        temp1=temp2;
        temp2=temp2->next;
        free(temp1);
    }
}

int length(Node *head){
    int count=0;
    Node *temp1=head;
    while (temp1!=NULL)
    {
        count++;
        temp1=temp1->next;
        if(temp1==head) break;
    }
    return count;
}

void traverse(Node *head){
    Node *temp1=head;
    while (temp1!=NULL)
    {
        printf("[%d] ",temp1->data);
        temp1=temp1->next;
        if(temp1!=head) printf("-> ");
        else break;
    }
    printf("\n");
}

void insertAtStart(Node **head, int value){
    Node *node=init(value);
    if(*head==NULL) {
        node->next=node;
    }
    else{
        Node *temp=*head;
        while (temp->next!=*head){
            temp=temp->next;
        }
        temp->next=node;
        node->next=*head;
    }
    *head=node;
}

void insertAtEnd(Node **head, int value){
    Node *node=init(value);
    if(*head==NULL) {
        node->next=node;
        *head=node;
    }
    else{
        Node *temp=*head;
        while (temp->next!=*head){
            temp=temp->next;
        }
        temp->next=node;
        node->next=*head;
    }
}

void insertAtIndex(Node **head, int value, int index){
    if(index<=0){
        insertAtStart(head,value);
        return;
    }
    if(index>=length(*head)){
        insertAtEnd(head,value);
        return;
    }
    Node *node=init(value);
    Node *temp1=NULL;
    Node *temp2=*head;
    for (int i = 0; i < index; i++)
    {
        temp1=temp2;
        temp2=temp2->next;
    }
    temp1->next=node;
    node->next=temp2;
}

void insertAfterValue(Node **head, int new, int old){
    Node *temp=find(*head,old);
    if(temp==NULL) return;
    if(temp->next==*head){
        insertAtEnd(head,new);
        return;
    }
    Node *node=init(new);
    node->next=temp->next;
    temp->next=node;
}

int deleteStart(Node **head){
    Node *temp1=*head;
    if(temp1==NULL) return -1;
    else if(temp1->next==temp1){
        *head=NULL;
    }
    else{
        Node *temp2=*head;
        while (temp2->next!=*head)
        {
            temp2=temp2->next;
        }
        temp2->next=temp1->next;
        *head=(*head)->next;
    }
    int value=temp1->data;
    free(temp1);
    return value;
}

int deleteEnd(Node **head){
    Node *temp2=*head;
    if(temp2==NULL) return -1;
    else if(temp2->next==temp2){
        *head=NULL;
    }
    else{
        Node *temp1=NULL;
        while (temp2->next!=*head)
        {
            temp1=temp2;
            temp2=temp2->next;
        }
        temp1->next=temp2->next;
    }
    int value=temp2->data;
    free(temp2);
    return value;
}

int deleteIndex(Node **head, int index){
    if(index<=0) return deleteStart(head);
    if(index>=length(*head)-1) return deleteEnd(head);
    Node *temp1=NULL;
    Node *temp2=*head;
    for (int i = 0; i < index; i++)
    {
        temp1=temp2;
        temp2=temp2->next;
    }
    temp1->next=temp2->next;
    int value=temp2->data;
    free(temp2);
    return value;
}

int deleteValue(Node **head, int value){
    Node *temp2=find(*head,value);
    if(temp2==NULL) return -1;
    if(temp2==*head) return deleteStart(head);
    if(temp2->next==*head) return deleteEnd(head);
    Node *temp1=*head;
    while(temp1->next!=temp2)
    {
        temp1=temp1->next;
    }
    temp1->next=temp2->next;
    value=temp2->data;
    free(temp2);
    return value;
}

void main(){
    Node *head=NULL;
    insertAtStart(&head,44);
    insertAtStart(&head,11);
    insertAtEnd(&head,22);
    insertAtEnd(&head,66);
    insertAtIndex(&head,55,2);
    insertAtIndex(&head,33,4);
    insertAfterValue(&head,77,11);
    insertAfterValue(&head,88,66);
    traverse(head);
    deleteValue(&head,11);
    traverse(head);
    deleteValue(&head,22);
    traverse(head);
    deleteValue(&head,33);
    traverse(head);
    deleteValue(&head,44);
    traverse(head);
    deleteValue(&head,55);
    traverse(head);
    deleteValue(&head,66);
    traverse(head);
    deleteValue(&head,77);
    traverse(head);
    deleteValue(&head,88);
    traverse(head);
}