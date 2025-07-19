#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Edge
{
    char label;
    int weight;
    struct Edge *next;
} Edge;

typedef struct
{
    int V;
    int E;
    char *labelMap;
    Edge **adjacencyList;
} Digraph;

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

int mapIndex(Digraph graph, char vertex)
{
    for (int i = 0; i < graph.V; i++)
        if (graph.labelMap[i] == vertex)
            return i;
    return -1;
}

Edge *create(char vertex, int edge)
{
    Edge *e = (Edge *)malloc(sizeof(Edge));
    e->label = vertex;
    e->weight = edge;
    e->next = NULL;
    return e;
}

Digraph init()
{
    Digraph graph;
    graph.V = 0;
    graph.E = 0;
    graph.labelMap = NULL;
    graph.adjacencyList = NULL;
    return graph;
}

Digraph copy(Digraph graph)
{
    Digraph newDigraph = init();
    newDigraph.V = graph.V;
    newDigraph.E = graph.E;
    newDigraph.labelMap = (char *)realloc(newDigraph.labelMap, newDigraph.V * sizeof(char));
    newDigraph.adjacencyList = (Edge **)realloc(newDigraph.adjacencyList, newDigraph.V * sizeof(Edge *));

    for (int i = 0; i < graph.V; i++)
    {
        newDigraph.labelMap[i] = graph.labelMap[i];
        newDigraph.adjacencyList[i] = NULL;
        Edge *prev = NULL;
        Edge *curr = graph.adjacencyList[i];
        while (curr != NULL)
        {
            Edge *e = create(curr->label, curr->weight);
            if (prev == NULL)
                newDigraph.adjacencyList[i] = e;
            else
                prev->next = e;
            prev = e;
            curr = curr->next;
        }
    }

    return newDigraph;
}

void clear(Digraph *graph)
{
    for (int i = 0; i < graph->V; i++)
    {
        Edge *curr = graph->adjacencyList[i];
        while (curr != NULL)
        {
            Edge *next = curr->next;
            free(curr);
            curr = next;
        }
    }
    free(graph->adjacencyList);
    graph->adjacencyList = NULL;

    free(graph->labelMap);
    graph->labelMap = NULL;

    graph->E = 0;
    graph->V = 0;
}

void addVertex(Digraph *graph, char vertex)
{
    graph->V++;
    graph->labelMap = (char *)realloc(graph->labelMap, graph->V * sizeof(char));
    graph->labelMap[graph->V - 1] = vertex;
    graph->adjacencyList = (Edge **)realloc(graph->adjacencyList, graph->V * sizeof(Edge *));
    graph->adjacencyList[graph->V - 1] = NULL;
}

void removeVertex(Digraph *graph, char vertex)
{
    int idx = index(*graph, vertex);
    if (idx == -1)
        return;

    for (int i = 0; i < graph->V; i++)
    {
        if (i == idx)
            continue;

        Edge *prev = NULL;
        Edge *curr = graph->adjacencyList[i];
        while (curr != NULL)
        {
            if (curr->label == vertex)
            {
                if (prev == NULL)
                    graph->adjacencyList[i] = curr->next;
                else
                    prev->next = curr->next;
                free(curr);
                graph->E--;
                break;
            }
            prev = curr;
            curr = curr->next;
        }
    }

    Edge *prev = NULL;
    Edge *curr = graph->adjacencyList[idx];
    while (curr != NULL)
    {
        prev = curr;
        curr = curr->next;
        free(prev);
        graph->E--;
    }

    graph->V--;

    graph->labelMap[idx] = graph->labelMap[graph->V];
    graph->adjacencyList[idx] = graph->adjacencyList[graph->V];

    graph->labelMap = (char *)realloc(graph->labelMap, graph->V * sizeof(char));
    graph->adjacencyList = (Edge **)realloc(graph->adjacencyList, graph->V * sizeof(Edge *));
}

void addEdge(Digraph *graph, char vertex1, int edge, char vertex2)
{
    int idx1 = index(*graph, vertex1);
    int idx2 = index(*graph, vertex2);
    if (idx1 == -1 || idx2 == -1)
        return;

    bool exists = false;

    Edge *curr = graph->adjacencyList[idx1];
    while (curr != NULL)
    {
        if (curr->label == vertex2)
        {
            curr->weight = edge;
            exists = true;
            break;
        }
        curr = curr->next;
    }

    curr = graph->adjacencyList[idx2];
    while (curr != NULL)
    {
        if (curr->label == vertex1)
        {
            curr->weight = edge;
            break;
        }
        curr = curr->next;
    }

    if (!exists)
    {
        curr = graph->adjacencyList[idx1];
        graph->adjacencyList[idx1] = create(vertex2, edge);
        graph->adjacencyList[idx1]->next = curr;

        curr = graph->adjacencyList[idx2];
        graph->adjacencyList[idx2] = create(vertex1, edge);
        graph->adjacencyList[idx2]->next = curr;

        graph->E++;
    }
}

void removeEdge(Digraph *graph, char vertex1, char vertex2)
{
    int idx1 = index(*graph, vertex1);
    int idx2 = index(*graph, vertex2);
    if (idx1 == -1 || idx2 == -1)
        return;

    bool removed = false;

    Edge *prev = NULL;
    Edge *curr = graph->adjacencyList[idx1];
    while (curr != NULL)
    {
        if (curr->label == vertex2)
        {
            if (prev == NULL)
                graph->adjacencyList[idx1] = curr->next;
            else
                prev->next = curr->next;
            free(curr);
            removed = true;
            break;
        }
        prev = curr;
        curr = curr->next;
    }

    prev = NULL;
    curr = graph->adjacencyList[idx2];
    while (curr != NULL)
    {
        if (curr->label == vertex1)
        {
            if (prev == NULL)
                graph->adjacencyList[idx2] = curr->next;
            else
                prev->next = curr->next;
            free(curr);
            break;
        }
        prev = curr;
        curr = curr->next;
    }

    if (removed)
        graph->E--;
}

bool hasSelfloop(Digraph graph, char vertex)
{
    int idx = index(graph, vertex);
    if (idx == -1)
        return false;
    Edge *curr = graph.adjacencyList[idx];
    while (curr != NULL)
    {
        if (curr->label == vertex)
            return true;
        curr = curr->next;
    }
    return false;
}

bool areAdjacent(Digraph graph, char vertex1, char vertex2)
{
    int idx = index(graph, vertex1);
    if (idx == -1)
        return false;
    Edge *curr = graph.adjacencyList[idx];
    while (curr != NULL)
    {
        if (curr->label == vertex2)
            return true;
        curr = curr->next;
    }
    return false;
}

int vertexDegree(Digraph graph, char vertex)
{
    int idx = index(graph, vertex);
    if (idx == -1)
        return -1;
    int degree = 0;
    Edge *curr = graph.adjacencyList[idx];
    while (curr != NULL)
    {
        if (curr->label == vertex)
            degree += 2;
        else
            degree++;
        curr = curr->next;
    }
    return degree;
}

int edgeWeight(Digraph graph, char vertex1, char vertex2)
{
    int idx = index(graph, vertex1);
    if (idx == -1)
        return -1;
    Edge *curr = graph.adjacencyList[idx];
    while (curr != NULL)
    {
        if (curr->label == vertex2)
            return curr->weight;
        curr = curr->next;
    }
    return -1;
}

float graphDensity(Digraph graph)
{
    return (float)(2 * graph.E) / (graph.V * (graph.V + 1));
}

void DFSIterative(Digraph graph, char vertex)
{
    int index = mapIndex(graph, vertex);
    if (index == -1)
        return;

    bool visited[graph.V];
    for (int i = 0; i < graph.V; i++)
        visited[i] = false;

    int stack[graph.V];
    int top = 0;

    visited[index] = true;
    stack[top++] = index;
    printf("DFSi: ");
    while (top > 0)
    {
        index = stack[--top];
        printf("%c ", graph.labelMap[index]);
        Edge *curr = graph.adjacencyList[index];
        while (curr != NULL)
        {
            int neighbour = mapIndex(graph, curr->label);
            if (!visited[neighbour])
            {
                visited[neighbour] = true;
                stack[top++] = neighbour;
            }
            curr = curr->next;
        }
    }
    printf("\n");
}

void dfsHelper(Digraph graph, int index, bool visited[])
{
    visited[index] = true;
    printf("%c ", graph.labelMap[index]);
    Edge *curr = graph.adjacencyList[index];
    while (curr != NULL)
    {
        int neighbour = mapIndex(graph, curr->label);
        if (!visited[neighbour])
            dfsHelper(graph, neighbour, visited);
        curr = curr->next;
    }
}

void DFSRecursive(Digraph graph, char label)
{
    int index = mapIndex(graph, label);
    if (index == -1)
        return;

    bool visited[graph.V];
    for (int i = 0; i < graph.V; i++)
        visited[i] = false;

    printf("DFSr: ");
    dfsHelper(graph, index, visited);
    printf("\n");
}

void BFSIterative(Digraph graph, char label)
{
    int index = mapIndex(graph, label);
    if (index == -1)
        return;

    bool visited[graph.V];
    for (int i = 0; i < graph.V; i++)
        visited[i] = false;

    int queue[graph.V];
    int front = 0, rear = 0;

    visited[index] = true;
    queue[rear++] = index;
    printf("BFSi: ");
    while (front < rear)
    {
        index = queue[front++];
        printf("%c ", graph.labelMap[index]);

        Edge *curr = graph.adjacencyList[index];
        while (curr != NULL)
        {
            int neighbour = mapIndex(graph, curr->label);
            if (!visited[neighbour])
            {
                visited[neighbour] = true;
                queue[rear++] = neighbour;
            }
            curr = curr->next;
        }
    }
    printf("\n");
}

void bfsHelper(Digraph graph, int currLevel[], int currCount, bool visited[])
{
    int nextLevel[graph.V];
    int nextCount = 0;

    for (int i = 0; i < currCount; i++)
    {
        int index = currLevel[i];
        printf("%c ", graph.labelMap[index]);

        Edge *curr = graph.adjacencyList[index];
        while (curr != NULL)
        {
            int neighbour = mapIndex(graph, curr->label);
            if (!visited[neighbour])
            {
                visited[neighbour] = true;
                nextLevel[nextCount++] = neighbour;
            }
            curr = curr->next;
        }
    }

    if (nextCount > 0)
    {
        bfsHelper(graph, nextLevel, nextCount, visited);
    }
}

void BFSRecursive(Digraph graph, char label)
{
    int index = mapIndex(graph, label);
    if (index == -1)
        return;

    bool visited[graph.V];
    for (int i = 0; i < graph.V; i++)
        visited[i] = false;

    int currLevel[graph.V];
    int currCount = 0;
    visited[index] = true;
    currLevel[currCount++] = index;

    printf("BFSr: ");
    bfsHelper(graph, currLevel, currCount, visited);
    printf("\n");
}

// algos

bool isUnweighted(Digraph graph)
{
    if (graph.E == 0)
        return true;
    int commonWeight = -1;
    for (int i = 0; i < graph.V; i++)
    {
        Edge *curr = graph.adjacencyList[i];
        while (curr != NULL)
        {
            if (commonWeight == -1)
                commonWeight = curr->weight;
            else if (curr->weight != commonWeight)
                return false;
            curr = curr->next;
        }
    }
    return true;
}

bool isRegular(Digraph graph)
{
    if (graph.E == 0)
        return true;
    int neighbours = -1;
    for (int i = 0; i < graph.V; i++)
    {
        int count = 0;
        Edge *curr = graph.adjacencyList[i];
        while (curr != NULL)
        {
            count++;
            curr = curr->next;
        }
        if (neighbours == -1)
            neighbours = count;
        else if (count != neighbours)
            return false;
    }
    return true;
}

bool isComplete(Digraph graph)
{
    if (graph.E == 0)
        return true;
    for (int i = 0; i < graph.V; i++)
    {
        int degree = 0;
        Edge *curr = graph.adjacencyList[i];
        while (curr != NULL)
        {
            if (curr->label == graph.labelMap[i])
                degree += 2;
            else
                degree++;
            curr = curr->next;
        }
        if (degree != graph.V + 1)
            return false;
    }
    return true;
}

bool isSparse(Digraph graph)
{
    return density(graph) <= 0.5;
}

bool isDense(Digraph graph)
{
    return density(graph) > 0.5;
}

bool isConnected(Digraph graph)
{
    if (graph.V <= 1)
        return true;

    bool visited[graph.V];
    for (int i = 0; i < graph.V; i++)
        visited[i] = false;

    int stack[graph.V];
    int top = 0;

    visited[0] = true;
    stack[top++] = 0;
    while (top > 0)
    {
        int index = stack[--top];
        Edge *curr = graph.adjacencyList[index];
        while (curr != NULL)
        {
            int neighbour = mapIndex(graph, curr->label);
            if (!visited[neighbour])
            {
                visited[neighbour] = true;
                stack[top++] = neighbour;
            }
            curr = curr->next;
        }
    }

    for (int i = 0; i < graph.V; i++)
        if (!visited[i])
            return false;

    return true;
}

bool isDisconnected(Digraph graph)
{
    return !isConnected(graph);
}

bool isCyclic(Digraph graph)
{
    if (graph.V == 0 && graph.E == 0)
        return false;

    bool visited[graph.V];
    int parent[graph.V];
    for (int i = 0; i < graph.V; i++)
    {
        visited[i] = false;
        parent[i] = -1;
    }

    int stack[graph.V];
    int top = 0;

    for (int start = 0; start < graph.V; start++)
    {
        if (visited[start])
            continue;

        visited[start] = true;
        stack[top++] = start;

        while (top > 0)
        {
            int index = stack[--top];
            Edge *curr = graph.adjacencyList[index];
            while (curr != NULL)
            {
                int neighbour = mapIndex(graph, curr->label);
                if (!visited[neighbour])
                {
                    visited[neighbour] = true;
                    parent[neighbour] = index;
                    stack[top++] = neighbour;
                }
                else if (neighbour != parent[index])
                    return true;
                curr = curr->next;
            }
        }
    }

    return false;
}

bool isAcyclic(Digraph graph)
{
    return !isCyclic(graph);
}

bool isTree(Digraph graph)
{
    return graph.E == graph.V - 1 && isConnected(graph) && isAcyclic(graph);
}

void describe(Digraph graph)
{
    for (int i = 0; i < graph.V; i++)
    {
        printf("%c |", graph.labelMap[i]);
        Edge *curr = graph.adjacencyList[i];
        while (curr != NULL)
        {
            printf(" (%c,%d)", curr->label, curr->weight);
            curr = curr->next;
        }
        printf("\n");
    }
}

int main()
{

    Digraph grf1 = init(10);

    addVertex(&grf1, 'A');
    addVertex(&grf1, 'B');
    addVertex(&grf1, 'C');
    addVertex(&grf1, 'D');
    addVertex(&grf1, 'E');

    addEdge(&grf1, 'A', 1, 'B');
    addEdge(&grf1, 'A', 1, 'C');
    addEdge(&grf1, 'A', 1, 'D');
    addEdge(&grf1, 'C', 1, 'D');
    addEdge(&grf1, 'C', 1, 'E');

    printAdjacencyMatrix(grf1);
    printAdjacencyList(grf1);

    printf("unweighted? %d\n", isUnweighted(grf1));
    printf("sparse? %d\n", isSparse(grf1));
    printf("dense? %d\n", isDense(grf1));
    printf("connected? %d\n", isConnected(grf1));
    printf("V=%d E=%d\n", grf1.numOfVertex, grf1.numOfEdges);

    char v = 'E';
    depthFirstSearchIterative(grf1, v);
    depthFirstSearchRecursive(grf1, v);
    breadthFirstSearchIterative(grf1, v);
    breadthFirstSearchRecursive(grf1, v);

    return 0;
}