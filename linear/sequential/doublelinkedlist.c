#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void swap(int *a, int *b){
    int temp = *a;
    *a = *b;
    *b = temp;
}

typedef struct Node{
    struct Node *prev;
    int data;
    struct Node *next;
}Node;

Node *init(int value){
    Node *node=(Node *)malloc(sizeof(Node));
    node->prev=NULL;
    node->data=value;
    node->next=NULL;
    return node; 
}

int lengthIterative(Node *head){
    int count=0;
    while (head!=NULL)
    {
        count++;
        head=head->next;
    }
    return count;
}

int lengthRecursive(Node *head){
    if(head==NULL) return 0;
    return 1+lengthRecursive(head->next);
}

void clearIterative(Node **head){
    Node *tempPrev=NULL;
    Node *tempCurr=*head;
    *head=NULL;
    while(tempCurr!=NULL){
        tempPrev=tempCurr;
        tempCurr=tempCurr->next;
        free(tempPrev);
    }
}

void clearRecursive(Node **head){
    if(*head==NULL) return;
    clearRecursive(&(*head)->next);
    free(*head);
    *head=NULL;
}

Node *searchIterative(Node *head, int value){
    while (head!=NULL){
        if(head->data==value) return head;
        head=head->next;
    }
    return NULL;
}

Node *searchRecursive(Node *head, int value){
    if(head==NULL || head->data==value) return head;
    return searchRecursive(head->next,value);
}

void traverseIterative(Node *head){
    while (head!=NULL){
        printf("[%d] ",head->data);
        head=head->next;
        printf((head!=NULL)?"<=> ":"\n");
    }
}

void traverseRecursive(Node *head){
    if(head==NULL) return;
    printf("[%d] ",head->data);
    head=head->next;
    printf((head!=NULL)?"<=> ":"\n");
    traverseRecursive(head);
}

Node *reverseIterative(Node *head){
    Node *tempPrev=NULL;
    Node *tempCurr=head;
    Node *tempNext;
    while (tempCurr!=NULL){
        tempNext=tempCurr->next;
        tempCurr->prev=tempNext;
        tempCurr->next=tempPrev;
        tempPrev=tempCurr;
        tempCurr=tempNext;
    }
    return tempPrev;
}

Node *reverseRecursive(Node *head){
    if(head==NULL) return head;
    if(head->next==NULL){
        head->prev=NULL;
        return head;
    }
    Node *temp=reverseRecursive(head->next);
    head->next->next=head;
    head->prev=head->next;
    head->next=NULL;
    return temp;
}

Node *insertAtStart(Node *head, int value){
    Node *node=init(value);
    if(head==NULL) return node;
    node->next=head;
    head->prev=node;
    return node;
}

Node *insertAtEnd(Node *head, int value){
    Node *node=init(value);
    if(head==NULL) return node;
    Node *tempPrev=head;
    while (tempPrev->next!=NULL) tempPrev=tempPrev->next;
    tempPrev->next=node;
    node->prev=tempPrev;
    return head;
}

Node *insertAtIndex(Node *head, int value, int index){
    if(index<=0) return insertAtStart(head,value);
    if(index>=lengthIterative(head)) return insertAtEnd(head,value);
    Node *node=init(value);
    Node *tempNext=head;
    for (int i = 0; i < index; i++) tempNext=tempNext->next;
    Node *tempPrev=tempNext->prev;
    tempPrev->next=node;
    node->prev=tempPrev;
    node->next=tempNext;
    tempNext->prev=node;
    return head;
}

Node *insertAfterValue(Node *head, int new, int old){
    Node *tempPrev=head;
    while (tempPrev!=NULL && tempPrev->data!=old) tempPrev=tempPrev->next;
    if(tempPrev!=NULL){
        Node *node=init(new);
        Node *tempNext=tempPrev->next;
        tempPrev->next=node;
        node->prev=tempPrev;
        if(tempNext!=NULL){
            node->next=tempNext;
            tempNext->prev=node;
        }
    }
    return head;
}

Node *deleteStart(Node *head){
    if(head!=NULL){
        Node *tempCurr=head;
        head=head->next;
        if(head!=NULL) head->prev=NULL;
        free(tempCurr);
    }    
    return head;
}

Node *deleteEnd(Node *head){
    if(head!=NULL){
        Node *tempCurr=head;
        while (tempCurr->next!=NULL) tempCurr=tempCurr->next;
        Node *tempPrev=tempCurr->prev;
        if(tempPrev==NULL) head=NULL;
        else tempPrev->next=NULL;
        free(tempCurr);
    }    
    return head;
}

Node *deleteIndex(Node *head, int index){
    if(head!=NULL){
        if(index<=0) return deleteStart(head);
        if(index>=lengthIterative(head)-1) return deleteEnd(head);
        Node *tempCurr=head;
        for (int i = 0; i < index; i++) tempCurr=tempCurr->next;
        Node *tempPrev=tempCurr->prev;
        tempPrev->next=tempCurr->next;
        free(tempCurr);
    }
    return head;
}

Node *deleteValue(Node *head, int value){
    if(head!=NULL){
        Node *tempCurr=head;
        while (tempCurr!=NULL && tempCurr->data!=value) tempCurr=tempCurr->next;
        if(tempCurr!=NULL){
            Node *tempPrev=tempCurr->prev;
            if(tempPrev==NULL) head=head->next;
            else tempPrev->next=tempCurr->next;
            free(tempCurr);
        }
    }
    return head;
}

int main(){
    Node *head=NULL;

    head=insertAtStart(head,44);
    head=insertAtStart(head,11);
    head=insertAtEnd(head,55);
    head=insertAtEnd(head,77);
    head=insertAfterValue(head,22,11);
    head=insertAfterValue(head,66,55);
    head=insertAtIndex(head,33,2);
    head=insertAtIndex(head,88,10);
    
    traverseIterative(head);
    head=reverseRecursive(head);
    traverseIterative(head);

    return 0;
}