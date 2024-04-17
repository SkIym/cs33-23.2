#include <stdio.h>
#include <stdlib.h>

#define infty 2147483647 // int is 4 bytes. (2^31-1)

typedef struct priorityqueue PriorityQueue;
struct priorityqueue
{
    int size;
    int capacity;
    int *heap;
    int *index;
    int *key;
};

typedef struct edge Edge;
struct edge
{
    int VRTX;
    int COST;
    Edge *NEXT;
};

typedef struct graph Graph;
struct graph
{
    int N;
    Edge **LIST;
    int *pred;
};

Graph *read_input();
void PRIM(Graph *, int);
void InitPQ (Graph *, PriorityQueue *, int);
int IsEmptyPQ(PriorityQueue *);
int EXTRACT_MIN (PriorityQueue *);
void HEAPIFY (PriorityQueue *, int);
void DECREASE_KEY (PriorityQueue *, int, int);
void DISPLAY_MST (Graph *);

// Utilities
void PRINT_LIST (Edge *);
void FREE_LIST (Edge *);
void FREE_GRAPH (Graph *);
void FREE_PQ (PriorityQueue *);
void PQ_UNDERFLOW();


int main()
{
    Graph *G;
    G = read_input();
    PRIM (G, 1);
    // Assume that Vertex 1 is always start vertex/root of MST
    DISPLAY_MST(G);
    
    FREE_GRAPH (G);
    
    return(0);
}

Edge *create_edge (int vrtx, int cost)
{
    Edge *temp = (Edge *) malloc (sizeof(Edge));
    temp->VRTX = vrtx;
    temp->COST = cost;
    temp->NEXT = NULL;
    return temp;
}

Graph *read_input()
{
    Graph *G = (Graph *) malloc (sizeof(Graph));
    scanf("%d", &(G->N));
    
    G->LIST = (Edge **) malloc ((G->N + 1)*sizeof(Edge *));
    
    int edge;
    Edge *end;
    int i, j;
    for (i = 1; i <= G->N; i++)
    {
        G->LIST[i] = NULL;
        for (j = 1; j <= G->N; j++)
        {
            scanf("%d", &edge);
            if (edge > 0)
            {
                if (G->LIST[i] == NULL)
                {
                    end = create_edge(j, edge);
                    G->LIST[i] = end;
                }
                else
                {
                    end->NEXT = create_edge(j, edge);
                    end = end->NEXT;
                }
            }
        }
    }
    
    G->pred = (int *) malloc ((G->N + 1)*sizeof(int));
    
    return G;
}

// Sources: Quiwa - Data Structures

void PRIM (Graph *G, int s)
{    
    PriorityQueue *PQ = (PriorityQueue *) malloc (sizeof(PriorityQueue));
    PQ->capacity = G->N;
    PQ->heap = (int *) malloc ((G->N + 1)*sizeof(int));
    PQ->index = (int *) malloc ((G->N + 1)*sizeof(int));
    PQ->key = (int *) malloc ((G->N + 1)*sizeof(int));
    
	InitPQ(G, PQ, s);
    G->pred[s] = 0;
    while (!IsEmptyPQ(PQ))
    {
        int min = EXTRACT_MIN(PQ); // min is vertex u here
        PQ->key[min] = -infty; 
        Edge *alpha = G->LIST[min];
        while (alpha != NULL)
        {
            int v = alpha->VRTX;
            int w = alpha->COST;
            if (w < PQ->key[v])
            {
                G->pred[v] = min;
                DECREASE_KEY(PQ, v, w);
            }
            alpha = alpha->NEXT;
        }
    }

    return;
}

void InitPQ (Graph *G, PriorityQueue *PQ, int s)
{
	int i = 1;
    for (int j = 1; j <= G->N; j++)
    {
        if (j == s) {
            PQ->heap[1] = s;
            PQ->index[s] = 1;
            PQ->key[s] = -infty;
        } else
        {
            i++;
            PQ->heap[i] = j;
            PQ->index[j] = i;
            PQ->key[j] = infty;
        }
    }
    PQ->size = G->N;
    return;
}

int IsEmptyPQ (PriorityQueue *PQ)
{
    return PQ->size == 0;
}

int EXTRACT_MIN (PriorityQueue *PQ)
{
	if(PQ->size == 0) PQ_UNDERFLOW();
    int j = PQ->heap[1];
    PQ->heap[1] = PQ->heap[PQ->size];
    PQ->index[PQ->heap[1]] = 1;
    PQ->size--;
    HEAPIFY(PQ, 1);
    return j;
}

void HEAPIFY (PriorityQueue *PQ, int r)
{
	int k = PQ->key[PQ->heap[r]];
    int l = PQ->heap[r];
    int i = r;
    int j = 2 * i;
    while (j <= PQ->size)
    {
        if (j < PQ->size && PQ->key[PQ->heap[j+1]] < PQ->key[PQ->heap[j]]) j++;
        if (PQ->key[PQ->heap[j]] < k) {
            PQ->heap[i] = PQ->heap[j];
            PQ->index[PQ->heap[j]] = i;
            i = j;
            j = 2 * i;
        }
        else break;
    }
    PQ->heap[i] = l;
    PQ->index[l] = i;
    return;
}

void DECREASE_KEY (PriorityQueue *PQ, int l, int newkey)
{
	PQ->key[l] = newkey;
    int i = PQ->index[l];
    int j = i / 2;
    while (i > 1 && PQ->key[PQ->heap[j]] > PQ->key[l])
    {
        PQ->heap[i] = PQ->heap[j];
        PQ->index[PQ->heap[j]] = i;
        i = j;
        j = i / 2;
    }
    PQ->heap[i] = l;
    PQ->index[l] = i;
    return;
}

void DISPLAY_MST (Graph *G)
{
    int TCost = 0;
    int l, j;
    for (l = 1; l <= G->N; l++)
    {
        if (G->pred[l] != 0)
        {
            Edge *alpha = G->LIST[l];
            while (alpha != NULL)
            {
                j = alpha->VRTX;
                if (G->pred[l] == j)
                {
                    printf ("(%d, %d) -- %d\n", l, j, alpha->COST);
                    TCost = TCost + alpha->COST;                    
                }
                alpha = alpha->NEXT;
            }
        }
    }
    printf("%d\n", TCost);
    return;
}


////
// Utilities Section
////

void PRINT_LIST (Edge *start)
{
    Edge *current = start;
    while (current != NULL)
    {
        printf("(%d:%d) -> ", current->VRTX, current->COST);
        current = current->NEXT;
    }
    
    return;
}

/**
Deallocates a linked list
**/
void FREE_LIST (Edge *start)
{
    Edge *current, *obsolete;
    current = start;
    while (current != NULL)
    {
        obsolete = current;
        current = current->NEXT;
        free(obsolete);
    }
    return;
}

void FREE_GRAPH (Graph *G)
{
    int i;
    for (i = 1; i <= G->N; i++) FREE_LIST (G->LIST[i]);
    free (G->pred);
    free (G);
}

void FREE_PQ (PriorityQueue *PQ)
{
    free(PQ->heap);
    free(PQ->index);
    free(PQ->key);
    free (PQ);
}

void PQ_UNDERFLOW(void)
{
    printf("Priority Queue underflow detected.\n");
    exit(1);
}