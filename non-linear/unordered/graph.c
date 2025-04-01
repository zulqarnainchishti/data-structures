#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// TREE         undirected, unweighted
// UNDIGRAPH    undirected, weighted
// DIGRAPH      directed, weighted

typedef struct Node {
    char vertex;
    int edge;
    struct Node *next;
} Node;

typedef struct {
    int size;
    int numOfVertex;
    int numOfEdges;
    char *vertexMapping;
    int **adjacencyMatrix;
    Node **adjacencyList;
} Undigraph;

void swap(int *a,int *b){
    int temp=*a;
    *a=*b;
    *b=temp;
}

Undigraph init(int n){
    Undigraph graph;
    graph.size=n;
    graph.numOfVertex=0;
    graph.numOfEdges=0;
    graph.vertexMapping=(char *)malloc(n*sizeof(char));
    for (int i = 0; i < n; i++)
    {
        graph.vertexMapping[i]='\0';
    }
    graph.adjacencyMatrix=(int **)malloc(n*sizeof(int *));
    for (int i = 0; i < n; i++)
    {
        graph.adjacencyMatrix[i]=(int *)malloc(n*sizeof(int));
        for (int j = 0; j < n; j++)
        {
            graph.adjacencyMatrix[i][j]=0;
        }
    }
    graph.adjacencyList=(Node **)malloc(n*sizeof(Node *));
    for (int i = 0; i < n; i++)
    {
        graph.adjacencyList[i]=NULL;
    }
    return graph;
}

Node *node(char label, int weight){
    Node *node=(Node *)malloc(sizeof(Node));
    node->vertex=label;
    node->edge=weight;
    node->next=NULL;
    return node;
}

int getIndex(Undigraph graph, char label){
    for (int i = 0; i < graph.numOfVertex; i++)
    {
        if(graph.vertexMapping[i]==label) return i;
    }
    return -1;
}

void addVertex(Undigraph *graph, char label){
    graph->vertexMapping[graph->numOfVertex++]=label;
}

void addEdge(Undigraph *graph, char label1, int weight, char label2){
    int index1=getIndex(*graph,label1);
    int index2=getIndex(*graph,label2);
    if(index1!=-1 && index2!=-1){
        graph->adjacencyMatrix[index1][index2]=weight;
        graph->adjacencyMatrix[index2][index1]=weight;

        Node *new1 = node(label2,weight);
        if (graph->adjacencyList[index1] == NULL) {
            graph->adjacencyList[index1] = new1;
        } else {
            Node *ptr1 = graph->adjacencyList[index1];
            while (ptr1->next != NULL) {
                ptr1 = ptr1->next;
            }
            ptr1->next = new1;
        }

        Node *new2 = node(label1,weight);
        if (graph->adjacencyList[index2] == NULL) {
            graph->adjacencyList[index2] = new2;
        } else {
            Node *ptr2 = graph->adjacencyList[index2];
            while (ptr2->next != NULL) {
                ptr2 = ptr2->next;
            }
            ptr2->next = new2;
        }

        graph->numOfEdges++;
    }
}

void removeEdge(Undigraph *graph, char label1, char label2){
    int index1=getIndex(*graph,label1);
    int index2=getIndex(*graph,label2);
    if(index1!=-1 && index2!=-1){
        graph->adjacencyMatrix[index1][index2]=0;
        graph->adjacencyMatrix[index2][index1]=0;

        Node *prev1 = NULL;
        Node *curr1 = graph->adjacencyList[index1];
        while (curr1->vertex!=label1) {
            prev1=curr1;
            curr1 = curr1->next;
        }
        if(prev1==NULL) graph->adjacencyList[index1]=curr1->next;
        else prev1->next=curr1->next;
        free(curr1); 
        
        Node *prev2 = NULL;
        Node *curr2 = graph->adjacencyList[index2];
        while (curr2->vertex!=label2) {
            prev2=curr2;
            curr2 = curr2->next;
        }
        if(prev2==NULL) graph->adjacencyList[index2]=curr2->next;
        else prev2->next=curr2->next;
        free(curr2);

        graph->numOfEdges--;
    }
}

void removeVertex(Undigraph *graph, char label){
    int index=getIndex(*graph,label);
    for (int i = 0; i < graph->numOfVertex; i++){
        graph->adjacencyMatrix[i][index]=graph->adjacencyMatrix[i][graph->numOfVertex-1];
    }
    for (int j = 0; j < graph->numOfVertex; j++){
        graph->adjacencyMatrix[index][j]=graph->adjacencyMatrix[graph->numOfVertex-1][j];
    }

    Node *ptr=graph->adjacencyList[index];
    graph->adjacencyList[index]=graph->adjacencyList[--graph->numOfVertex];
    graph->adjacencyList[graph->numOfVertex]=NULL;
    graph->vertexMapping[index]=graph->vertexMapping[graph->numOfVertex];
    int connected;
    Node *prev, *curr;
    while (ptr!=NULL)
    {
        connected=getIndex(*graph,ptr->vertex);
        prev=NULL;
        curr=graph->adjacencyList[connected];
        while (curr->vertex!=label)
        {
            prev=curr;
            curr=curr->next;
        }
        if(prev==NULL) graph->adjacencyList[connected]=curr->next;
        else prev->next=curr->next;
        free(curr);
        prev=ptr;
        ptr=ptr->next;
        free(prev);
        graph->numOfEdges--;
    }
}

void printAdjacencyMatrix(Undigraph graph){
    for (int i = -1; i < graph.numOfVertex; i++)
    {
        if(i==-1) printf("  ");
        else printf("%c ",graph.vertexMapping[i]);
        for (int j = 0; j < graph.numOfVertex; j++)
        {
            if(i==-1) printf("%c ",graph.vertexMapping[j]);
            else printf("%d ",graph.adjacencyMatrix[i][j]);
        }
        printf("\n");
    }
}

void printAdjacencyList(Undigraph graph){
    for (int i = 0; i < graph.numOfVertex; i++) {
        Node *ptr = graph.adjacencyList[i];
        printf("%c -> ", graph.vertexMapping[i]);
        while (ptr != NULL) {
            printf("(%c,%d) -> ", ptr->vertex, ptr->edge);
            ptr = ptr->next;
        }
        printf("NULL\n");
    }
}

bool isSelfloop(Undigraph graph, char label){
    int index=getIndex(graph,label);
    return graph.adjacencyMatrix[index][index]!=0;
}

bool isUnweighted(Undigraph graph){
    int wgt=-1;
    for (int i = 0; i < graph.numOfVertex; i++){
        for (int j = 0; j <= i; j++){
            if(graph.adjacencyMatrix[i][j]!=0){
                if(wgt==-1)
                    wgt=graph.adjacencyMatrix[i][j];
                else
                    if(graph.adjacencyMatrix[i][j]!=wgt)
                        return false;
            }
        }
    }
    return true;
}

bool isSparse(Undigraph graph){
    int leastPossibleEdges=graph.numOfVertex-1;
    int mostPossibleEdges=graph.numOfVertex*(graph.numOfVertex-1)/2;
    int mid=(leastPossibleEdges+mostPossibleEdges)/2;
    if(graph.numOfEdges<=mid && graph.numOfEdges>=leastPossibleEdges) return true;
    else return false;
}

bool isDense(Undigraph graph){
    int leastPossibleEdges=graph.numOfVertex-1;
    int mostPossibleEdges=graph.numOfVertex*(graph.numOfVertex-1)/2;
    int mid=(leastPossibleEdges+mostPossibleEdges)/2;
    if(graph.numOfEdges>mid && graph.numOfEdges<=mostPossibleEdges) return true;
    else return false;
}

bool isRegular(Undigraph graph){
    int prevDegree=-1;
    int currDegree;
    Node *ptr;
    for (int i = 0; i < graph.numOfVertex; i++){
        currDegree=0;
        ptr=graph.adjacencyList[i];
        while (ptr!=NULL)
        {
            currDegree++;
            ptr=ptr->next;
        }
        if(prevDegree==-1) prevDegree=currDegree;
        else if(prevDegree!=currDegree) return false;
    }
    return true;
}

bool isComplete(Undigraph graph){
    int maxDegree=graph.numOfVertex-1;
    int currDegree;
    Node *ptr;
    for (int i = 0; i < graph.numOfVertex; i++){
        currDegree=0;
        ptr=graph.adjacencyList[i];
        while (ptr!=NULL)
        {
            currDegree++;
            ptr=ptr->next;
        }
        if(maxDegree!=currDegree) return false;
    }
    return true;
}

void depthFirstSearchIterative(Undigraph graph, char label){
    int index=getIndex(graph,label);
    if(index==-1) return;

    int stack[graph.numOfVertex];
    int top=-1;
    
    bool visited[graph.numOfVertex];
    for (int i = 0; i < graph.numOfVertex; i++)
    {
        visited[i]=false;
    }
    
    stack[++top]=index;
    visited[index]=true;
    printf("DFSi: ");
    while (top>-1)
    {
        index=stack[top--];
        for (int j = graph.numOfVertex-1; j >=0; j--){
            if(graph.adjacencyMatrix[index][j]!=0 && visited[j]==false){
                stack[++top]=j;
                visited[j]=true;
            }
        }
        printf("%c",graph.vertexMapping[index]);
        if(top>-1) printf(" -> ");
    }
    printf("\n");
}

void dfsHelper(Undigraph graph, int index, bool visited[]){
    visited[index]=true;
    printf("%c -> ",graph.vertexMapping[index]);
    for (int j = 0; j < graph.numOfVertex ; j++){
        if(graph.adjacencyMatrix[index][j]!=0 && visited[j]==false){
            dfsHelper(graph,j,visited);
        }
    }
}

void depthFirstSearchRecursive(Undigraph graph, char label){
    int index=getIndex(graph,label);
    if(index==-1) return;
    
    bool visited[graph.numOfVertex];
    for (int i = 0; i < graph.numOfVertex; i++)
    {
        visited[i]=false;
    }

    printf("DFSr: ");
    dfsHelper(graph,index,visited);
    printf("NULL\n");
}

void breadthFirstSearchIterative(Undigraph graph, char label){
    int index=getIndex(graph,label);
    if(index==-1) return;

    int queue[graph.numOfVertex];
    int front=-1,rear=-1;
    
    bool visited[graph.numOfVertex];
    for (int i = 0; i < graph.numOfVertex; i++)
    {
        visited[i]=false;
    }
    
    queue[++rear]=index;
    visited[index]=true;
    printf("BFSi: ");
    while (front<rear)
    {
        index=queue[++front];
        for (int j = 0; j < graph.numOfVertex; j++){
            if(graph.adjacencyMatrix[index][j]!=0 && visited[j]==false){
                queue[++rear]=j;
                visited[j]=true;
            }
        }
        printf("%c",graph.vertexMapping[index]);
        if(front<rear) printf(" -> ");
    }
    printf("\n");
}

void bfsHelper(Undigraph graph, int currLevel[], int currCount, bool visited[]){
    int nextLevel[graph.numOfVertex];
    int nextCount=0;

    for (int i = 0; i < currCount; i++)
    {
        printf("%c -> ",graph.vertexMapping[currLevel[i]]);
        for (int j = 0; j < graph.numOfVertex ; j++){
            if(graph.adjacencyMatrix[currLevel[i]][j]!=0 && visited[j]==false){
                visited[j]=true;
                nextLevel[nextCount++]=j;
            }
        }
    }

    if (nextCount>0) {
        bfsHelper(graph,nextLevel,nextCount,visited);
    }
}

void breadthFirstSearchRecursive(Undigraph graph, char label){
    int index=getIndex(graph,label);
    if(index==-1) return;
    
    bool visited[graph.numOfVertex];
    for (int i = 0; i < graph.numOfVertex; i++)
    {
        visited[i]=false;
    }

    int currLevel[graph.numOfVertex];
    int currCount=0;
    visited[index]=true;
    currLevel[currCount++]=index;

    printf("BFSr: ");
    bfsHelper(graph,currLevel,currCount,visited);
    printf("NULL\n");
}

bool isConnected(Undigraph graph){
    bool all(bool array[], int n){
        for (int i = 0; i < n; i++)
        {
            if(array[i]==false) return false;
        }
        return true;
    }
    
    int stack[graph.numOfVertex];
    int top=-1;
    
    bool visited[graph.numOfVertex];
    for (int i = 0; i < graph.numOfVertex; i++)
    {
        visited[i]=false;
    }
    
    int index=0;
    stack[++top]=index;
    visited[index]=true;
    while (top>-1)
    {
        index=stack[top--];
        for (int j = graph.numOfVertex-1; j >=0; j--){
            if(graph.adjacencyMatrix[index][j]!=0 && visited[j]==false){
                stack[++top]=j;
                visited[j]=true;
            }
        }
    }

    return all(visited,graph.numOfVertex);
}

bool isCyclic(Undigraph graph){}

void shortestPath(Undigraph graph, char labelStart, char labelEnd){
    // DIJKSTRA'S
    // BELLMAN-FORD
    // A*
    // FLOYD-WARSHALL
    // JOHNSON
}

void minimumSpanningTree(Undigraph graph){
    // BORUVKA'S
    // PRIM'S
    // KRUSKAL'S
}

int main(){
    
    Undigraph grf1=init(10);
    
    addVertex(&grf1,'A');
    addVertex(&grf1,'B');
    addVertex(&grf1,'C');
    addVertex(&grf1,'D');
    addVertex(&grf1,'E');
    
    addEdge(&grf1,'A',1,'B');
    addEdge(&grf1,'A',1,'C');
    addEdge(&grf1,'A',1,'D');
    addEdge(&grf1,'C',1,'D');
    addEdge(&grf1,'C',1,'E');
    
    printAdjacencyMatrix(grf1);
    printAdjacencyList(grf1);

    printf("unweighted? %d\n",isUnweighted(grf1));
    printf("sparse? %d\n",isSparse(grf1));
    printf("dense? %d\n",isDense(grf1));
    printf("connected? %d\n",isConnected(grf1));
    printf("V=%d E=%d\n",grf1.numOfVertex,grf1.numOfEdges);

    char v='E';
    depthFirstSearchIterative(grf1,v);
    depthFirstSearchRecursive(grf1,v);
    breadthFirstSearchIterative(grf1,v);
    breadthFirstSearchRecursive(grf1,v);

    return 0;
}