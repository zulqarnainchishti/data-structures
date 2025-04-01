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

int lengthIterative(Node *head){
    int len=0;
    while (head!=NULL){
        len++;
        head=head->next;
    }
    return len;
}

int lengthRecursive(Node *head){
    if(head==NULL) return 0;
    return 1+lengthRecursive(head->next);
}

void clearIterative(Node **head){
    Node *tempPrev=NULL;
    Node *tempNext=*head;
    *head=NULL;
    while(tempNext!=NULL){
        tempPrev=tempNext;
        tempNext=tempNext->next;
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
        printf((head!=NULL)?"-> ":"\n");
    }
}

void traverseRecursive(Node *head){
    if(head==NULL) return;
    printf("[%d] ",head->data);
    head=head->next;
    printf((head!=NULL)?"-> ":"\n");
    traverseRecursive(head);
}

Node *reverseIterative(Node *head){
    Node *tempPrev=NULL;
    Node *tempCurr=head;
    Node *tempNext;
    while (tempCurr!=NULL)
    {
        tempNext=tempCurr->next;
        tempCurr->next=tempPrev;
        tempPrev=tempCurr;
        tempCurr=tempNext;
    }
    return tempPrev;
}

Node *reverseRecursive(Node *head){
    if(head==NULL || head->next==NULL) return head;
    Node *temp=reverseRecursive(head->next);
    head->next->next=head;
    head->next=NULL;
    return temp;
}

Node *insertAtStart(Node *head, int value){
    Node *node=init(value);
    node->next=head;
    return node;
}

Node *insertAtEnd(Node *head, int value){
    Node *node=init(value);
    if(head==NULL) return node;
    Node *tempPrev=head;
    while (tempPrev->next!=NULL) tempPrev=tempPrev->next;
    tempPrev->next=node;
    return head;
}

Node *insertAtIndex(Node *head, int value, int index){
    if(index<=0) return insertAtStart(head,value);
    if(index>=lengthIterative(head)) return insertAtEnd(head,value);
    Node *node=init(value);
    Node *tempPrev=head;
    for (int i = 0; i < index-1; i++) tempPrev=tempPrev->next;
    node->next=tempPrev->next;
    tempPrev->next=node;
    return head;
}

Node *insertAfterValue(Node *head, int new, int old){
    Node *tempPrev=head;
    while(tempPrev!=NULL && tempPrev->data!=old) tempPrev=tempPrev->next;
    if(tempPrev!=NULL){
        Node *node=init(new);
        node->next=tempPrev->next;
        tempPrev->next=node;
    }
    return head;
}

Node *deleteStart(Node *head){
    if(head!=NULL){
        Node *tempCurr=head;
        head=head->next;
        free(tempCurr);
    }
    return head;
}

Node *deleteEnd(Node *head){
    if(head!=NULL){
        Node *tempPrev=NULL;
        Node *tempCurr=head;
        while (tempCurr->next!=NULL)
        {
            tempPrev=tempCurr;
            tempCurr=tempCurr->next;
        }
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
        Node *tempPrev=NULL;
        Node *tempCurr=head;
        for (int i = 0; i < index; i++)
        {
            tempPrev=tempCurr;
            tempCurr=tempCurr->next;
        }
        tempPrev->next=tempCurr->next;
        free(tempCurr);
    }
    return head;
}

Node *deleteValue(Node *head, int value){
    if(head!=NULL){
        Node *tempPrev=NULL;
        Node *tempCurr=head;
        while (tempCurr!=NULL && tempCurr->data!=value){
            tempPrev=tempCurr;
            tempCurr=tempCurr->next;
        }
        if(tempCurr!=NULL){
            if(tempPrev==NULL) head=head->next;
            else tempPrev->next=tempCurr->next;
            free(tempCurr);
        }
    }
    return head;
}

int main(){
    Node *head=NULL;

    head=insertAtEnd(head,33);
    head=insertAtEnd(head,44);
    head=insertAtStart(head,22);
    head=insertAtStart(head,11);
    head=insertAtIndex(head,55,0);
    head=insertAtIndex(head,66,10);
    head=insertAfterValue(head,77,55);
    head=insertAfterValue(head,88,22);
    head=insertAfterValue(head,99,66);
    traverseIterative(head);

    head=deleteValue(head,99);
    traverseIterative(head);

    return 0;
}