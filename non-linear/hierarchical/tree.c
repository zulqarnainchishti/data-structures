#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

void swap(int *a,int *b){
    int temp=*a;
    *a=*b;
    *b=temp;
}

typedef struct Tree {
    int data;
    struct Tree *left;
    struct Tree *right;
} Tree;

Tree *init(int value){
    Tree *node=(Tree *)malloc(sizeof(Tree));
    node->data=value;
    node->left=NULL;
    node->right=NULL;
    return node;
}

void clear(Tree **head){
    if(*head==NULL) return;
    clear(&(*head)->left);
    clear(&(*head)->right);
    free(*head);
    *head=NULL;
}

int size(Tree *head){
    int count=0;
    if(head!=NULL) count=1+size(head->left)+size(head->right);
    return count;
}

int degree(Tree *head){
    int count=0;
    if(head->left!=NULL) count++;
    if(head->right!=NULL) count++;
    return count;
}

Tree *max(Tree *head){
    while (head->right!=NULL)
    {
        head=head->right;
    }
    return head;
}

Tree *min(Tree *head){
    while (head->left!=NULL)
    {
        head=head->left;
    }
    return head;
}

Tree *successor(Tree *head, int value){
    Tree **stack=(Tree **)malloc(size(head)*sizeof(Tree *));
    int top=-1;
    Tree *temp=NULL;
    while (head!=NULL || top>=0)
    {
        while (head!=NULL){
            stack[++top]=head;
            head=head->left;
        }
        head=stack[top--];
        if(temp!=NULL && temp->data==value) break;
        temp=head;
        head=head->right;
    }
    free(stack);
    return head;
}

Tree *predecessor(Tree *head, int value){
    Tree **stack=(Tree **)malloc(size(head)*sizeof(Tree *));
    int top=-1;
    Tree *temp=NULL;
    while (head!=NULL || top>=0)
    {
        while (head!=NULL){
            stack[++top]=head;
            head=head->left;
        }
        head=stack[top--];
        if(head->data==value) break;
        temp=head;
        head=head->right;
    }
    free(stack);
    return temp;
}

Tree *ceiling(Tree *head, int value){
    Tree **stack=(Tree **)malloc(size(head)*sizeof(Tree *));
    int top=-1;
    Tree *temp=NULL;
    while (head!=NULL || top>=0)
    {
        while (head!=NULL){
            stack[++top]=head;
            head=head->right;
        }
        head=stack[top--];
        if(head->data<value) break;
        temp=head;
        head=head->left;
    }
    free(stack);
    return temp;
}

Tree *floored(Tree *head, int value){
    Tree **stack=(Tree **)malloc(size(head)*sizeof(Tree *));
    int top=-1;
    Tree *temp=NULL;
    while (head!=NULL || top>=0)
    {
        while (head!=NULL){
            stack[++top]=head;
            head=head->left;
        }
        head=stack[top--];
        if(head->data>value) break;
        temp=head;
        head=head->right;
    }
    free(stack);
    return temp;
}

void insert(Tree **head, int value){
    if(*head==NULL){
        *head=init(value);
        return;
    }
    if(value<(*head)->data) insert(&(*head)->left,value);
    else if(value>(*head)->data) insert(&(*head)->right,value);
}

Tree *parent(Tree *head, int value){
    Tree *temp=NULL;
    while (head->data!=value)
    {
        temp=head;
        if(value<head->data) head=head->left;
        else if(value>head->data) head=head->right;
    }
    return temp;
}

Tree *search(Tree *head, int value){
    if(head==NULL || value==head->data) return head;
    else if(value<head->data) return search(head->left,value);
    else return search(head->right,value);
}

Tree *sibling(Tree *head, int value){
    head=parent(head,value);
    if(head->left->data!=value) return head->left;
    if(head->right->data!=value) return head->right;
}

int height(Tree *head, int value){
    head=search(head,value);
    int getHeight(Tree *head){
        if(head==NULL) return -1;
        return 1+((getHeight(head->left)>=getHeight(head->right))?getHeight(head->left):getHeight(head->right));
    }
    return getHeight(head);
}

int depth(Tree *head, int value){
    int count=0;
    while (head!=NULL && head->data!=value)
    {
        count++;
        if(value<head->data) head=head->left;
        else if(head->data<value) head=head->right;
    }
    return count;
}

void delete(Tree **head, int value){
    Tree *temp2=search(*head,value);
    if(temp2==NULL) return;
    Tree *temp1=parent(*head,value);
    if(temp2->left==NULL && temp2->right==NULL){
        if(temp1->left==temp2) temp1->left=NULL;
        else if(temp1->right==temp2) temp1->right=NULL;
        free(temp2);
    }
    if(temp2->left==NULL || temp2->right==NULL){
        Tree *temp3=NULL;
        if(temp2->left!=NULL) temp3=temp2->left;
        else if(temp1->right!=NULL) temp3=temp2->right;
        if(temp1->left==temp2) temp1->left=temp3;
        else if(temp1->right==temp2) temp1->right=temp3;
        free(temp2);
    }
    else{
        int prev=max(temp2->left)->data;
        delete(&temp2,prev);
        temp2->data=prev;
    }
}

void preorderIterative(Tree *head){
    if(head==NULL) return;
    Tree **stack=(Tree **)malloc(size(head)*sizeof(Tree *));
    int top=-1;
    stack[++top]=head;
    while (top>=0)
    {
        head=stack[top--];
        printf("<%d> ",head->data);
        if(head->right!=NULL) stack[++top]=head->right;
        if(head->left!=NULL) stack[++top]=head->left;
    }
    free(stack);
}

void preorderRecursive(Tree *head){
    if(head==NULL) return;
    printf("<%d> ",head->data);
    preorderRecursive(head->left);
    preorderRecursive(head->right);
}

void inorderIterative(Tree *head){
    Tree **stack=(Tree **)malloc(size(head)*sizeof(Tree *));
    int top=-1;
    while (head!=NULL || top>=0)
    {
        while (head!=NULL){
            stack[++top]=head;
            head=head->left;
        }
        head=stack[top--];
        printf("<%d> ",head->data);
        head=head->right;
    }
    free(stack);
}

void inorderRecursive(Tree *head){
    if(head==NULL) return;
    inorderRecursive(head->left);
    printf("<%d> ",head->data);
    inorderRecursive(head->right);
}

void postorderIterative(Tree *head){
    if(head==NULL) return;
    Tree **stack1=(Tree **)malloc(size(head)*sizeof(Tree *));
    Tree **stack2=(Tree **)malloc(size(head)*sizeof(Tree *));
    int top1=-1, top2=-1;
    stack1[++top1]=head;
    while (top1>=0)
    {
        head=stack1[top1--];
        stack2[++top2]=head;
        if(head->left!=NULL) stack1[++top1]=head->left;
        if(head->right!=NULL) stack1[++top1]=head->right;
    }
    while (top2>=0)
    {
        head=stack2[top2--];
        printf("<%d> ",head->data);
    }
    free(stack1);
    free(stack2);
}

void postorderRecursive(Tree *head){
    if(head==NULL) return;
    postorderRecursive(head->left);
    postorderRecursive(head->right);
    printf("<%d> ",head->data);
}

void levelorderIterative(Tree *head){
    if(head==NULL) return;
    Tree **queue=(Tree **)malloc(size(head)*sizeof(Tree *));
    int front=-1,rear=-1; 
    queue[++rear]=head;
    while(front<rear)
    {
        Tree *node=queue[++front];
        if(node->left!=NULL) queue[++rear]=node->left;
        if(node->right!=NULL) queue[++rear]=node->right;
        printf("<%d> ",node->data);
    }
    free(queue);
}

void levelorderRecursive(Tree *head){
    void printLevel(Tree *head, int level){
        if(head==NULL) return;
        if(level==0) printf("<%d> ",head->data);
        else{
            printLevel(head->left,level-1);
            printLevel(head->right,level-1);
        }
    }
    int levels=height(head,head->data);
    for (int i = 0; i <= levels; i++)
    {
        printLevel(head,i);
    }
}

bool isin(Tree *head, int value){
    if(head==NULL) return false;
    if(value==head->data) return true;
    if(value<head->data) return isin(head->left,value);
    else return isin(head->right,value);
}

bool isFull(Tree *head){
    if(head==NULL) return false;
    if(head->left==NULL && head->right==NULL) return true;
    return (isFull(head->left) && isFull(head->right));
}

bool isSkewed(Tree *head){
    if(head==NULL) return true;
    if(head->left!=NULL && head->right!=NULL) return false;
    return (isSkewed(head->left) && isSkewed(head->right));
}

bool isPerfect(Tree *head){
    int l=1+height(head,head->data);
    int s=size(head);
    return s==(int)pow(2,l)-1;
}

bool isComplete(Tree *head){
    bool areLevelPerfect(Tree *head, int level){
        if(head==NULL) return false;
        if(level==0) return true;
        return areLevelPerfect(head->left,level-1) && areLevelPerfect(head->right,level-1);
    }
    int level=height(head,head->data);
    if(level==0) return true;
    if(!areLevelPerfect(head,level-1)) return false;
    bool flag=true;
    int prevDegree=3;
    void areNodesLeft(Tree *head, int level){
        if(flag==false) return;
        if(level==0){
            int currDegree=degree(head);
            if(prevDegree<currDegree) flag=false;
            if(currDegree==1 && prevDegree==1) flag=false;
            if(currDegree==1 && head->left==NULL) flag=false;
            prevDegree=currDegree;
        }else{
            areNodesLeft(head->left,level-1);
            areNodesLeft(head->right,level-1);
        }
    }
    areNodesLeft(head,level-1);
    return flag;
}

int balanceFactor(Tree *head){
    if(head==NULL) return 0;
    int getHeight(Tree *head){
        if(head==NULL) return -1;
        return 1+((getHeight(head->left)>=getHeight(head->right))?getHeight(head->left):getHeight(head->right));
    }
    return getHeight(head->left)-getHeight(head->right);
}

bool isBalanced(Tree *head){
    if(head==NULL) return true;
    bool flag=true;
    Tree **stack=(Tree **)malloc(size(head)*sizeof(Tree *));
    int top=-1;
    stack[++top]=head;
    while (top>=0)
    {
        head=stack[top--];
        int temp=abs(balanceFactor(head));
        if(temp>1){
            flag=false;
            break;
        }
        if(head->right!=NULL) stack[++top]=head->right;
        if(head->left!=NULL) stack[++top]=head->left;
    }
    free(stack);
    return flag;
}

Tree *rotateLeft(Tree *a){
    Tree *b=a->right;
    Tree *c=b->right;
    Tree *d=b->left;
    a->right=d;
    b->left=a;
    return b;
}

Tree *rotateRight(Tree *a){
    Tree *b=a->left;
    Tree *c=b->left;
    Tree *d=b->right;
    a->left=d;
    b->right=a;
    return b;
}

Tree *rotateRightLeft(Tree *a){
    Tree *b=a->right;
    Tree *c=b->right;
    Tree *d=b->left;
    a->right=rotateRight(b);
    d=rotateLeft(a);
    return d;
}

Tree *rotateLeftRight(Tree *a){
    Tree *b=a->left;
    Tree *c=b->left;
    Tree *d=b->right;
    a->left=rotateLeft(b);
    d=rotateRight(a);
    return d;
}

void balancedInsert(Tree **head, int value){
    Tree *prev=NULL;
    Tree *curr=*head;
    while (curr!=NULL)
    {
        prev=curr;
        if (value<curr->data){
            curr=curr->left;
        }else if (value>curr->data){
            curr=curr->right;
        }else break;
    }
    if(prev==NULL) *head=init(value);
    else if(value<prev->data) prev->left=init(value);
    else if(value>prev->data) prev->right=init(value);
    else return;

    if(!isBalanced(*head)){
        Tree *temp0=NULL;
        Tree *temp1=NULL;
        Tree *temp2=*head;
        int n,m;
        m=abs(balanceFactor(temp2));
        while (temp1==NULL || n<2 || m>1)
        {
            if (value<temp2->data){
                temp0=temp1;
                temp1=temp2;
                temp2=temp2->left;
                n=m;
                m=abs(balanceFactor(temp2));
            }else if (value>temp2->data){
                temp0=temp1;
                temp1=temp2;
                temp2=temp2->right;
                n=m;
                m=abs(balanceFactor(temp2));
            }else break;
        }
        if(temp1->data<value && temp2->data<value){
            if(temp0==NULL){
                *head=rotateLeft(temp1);
            }else if(temp0->left==temp1){
                temp0->left=rotateLeft(temp1);
            }else if(temp0->right==temp1){
                temp0->right=rotateLeft(temp1);
            }
        }
        else if(value<temp1->data && value<temp2->data){
            if(temp0==NULL){
                *head=rotateRight(temp1);
            }else if(temp0->left==temp1){
                temp0->left=rotateRight(temp1);
            }else if(temp0->right==temp1){
                temp0->right=rotateRight(temp1);
            }
        }
        else if(temp1->data<value && value<temp2->data){
            if(temp0==NULL){
                *head=rotateRightLeft(temp1);
            }else if(temp0->left==temp1){
                temp0->left=rotateRightLeft(temp1);
            }else if(temp0->right==temp1){
                temp0->right=rotateRightLeft(temp1);
            }
        }
        else if(value<temp1->data && temp2->data<value){
            if(temp0==NULL){
                *head=rotateLeftRight(temp1);
            }else if(temp0->left==temp1){
                temp0->left=rotateLeftRight(temp1);
            }else if(temp0->right==temp1){
                temp0->right=rotateLeftRight(temp1);
            }
        }
    }
}

void balancedDelete(Tree **head, int value){
    delete(head,value);

    if(!isBalanced(*head)){
        Tree *temp0=NULL;
        Tree *temp1=NULL;
        Tree *temp2=*head;
        int n,m;
        m=balanceFactor(temp2);
        while (temp1==NULL || abs(n)<2 || abs(m)>1)
        {
            if (m>0){
                temp0=temp1;
                temp1=temp2;
                temp2=temp2->left;
                n=m;
                m=balanceFactor(temp2);
            }else if (m<0){
                temp0=temp1;
                temp1=temp2;
                temp2=temp2->right;
                n=m;
                m=balanceFactor(temp2);
            }else break;
        }
        if(n<-1 && m<=0){
            if(temp0==NULL){
                *head=rotateLeft(temp1);
            }else if(temp0->left==temp1){
                temp0->left=rotateLeft(temp1);
            }else if(temp0->right==temp1){  
                temp0->right=rotateLeft(temp1);
            }
        }
        else if(n>1 && m>=0){
            if(temp0==NULL){
                *head=rotateRight(temp1);
            }else if(temp0->left==temp1){
                temp0->left=rotateRight(temp1);
            }else if(temp0->right==temp1){
                temp0->right=rotateRight(temp1);
            }
        }
        else if(n<-1 && m>0){
            if(temp0==NULL){
                *head=rotateRightLeft(temp1);
            }else if(temp0->left==temp1){
                temp0->left=rotateRightLeft(temp1);
            }else if(temp0->right==temp1){
                temp0->right=rotateRightLeft(temp1);
            }
        }
        else if(n>1 && m<0){
            if(temp0==NULL){
                *head=rotateLeftRight(temp1);
            }else if(temp0->left==temp1){
                temp0->left=rotateLeftRight(temp1);
            }else if(temp0->right==temp1){
                temp0->right=rotateLeftRight(temp1);
            }
        }
    }
}

Tree *avlTree(Tree *head){
    Tree *remakeTree(Tree *unbalanced, Tree *balanced){
        if(unbalanced!=NULL){
            balanced=remakeTree(unbalanced->left,balanced);
            balancedInsert(&balanced,unbalanced->data);
            balanced=remakeTree(unbalanced->right,balanced);
        }
        return balanced;
    }
    return remakeTree(head,NULL);
}

int main()
{
    Tree *head=NULL;

    balancedInsert(&head,3);
    balancedInsert(&head,5);
    balancedInsert(&head,2);
    balancedInsert(&head,4);

    balancedDelete(&head,2);
    // head=avlTree(head);
    
    preorderIterative(head);
    printf("\n");
    inorderIterative(head);
    printf("\n");
    postorderIterative(head);
    printf("\n");
    levelorderIterative(head);
    printf("\n");

    // for (int i = 0; i <= 6; i++)
    // {
    //     printf("%2d => %2d\n",i,balanceFactor(search(head,i)));
    // }
    
    printf("%d isin? %d\n",7,isin(head,7));
    printf("full? %d\n",isFull(head));
    printf("skewed? %d\n",isSkewed(head));
    printf("perfect? %d\n",isPerfect(head));
    printf("complete? %d\n",isComplete(head));
    printf("balanced? %d\n",isBalanced(head));
    return 0;
}