#include <stdio.h>
#include <stdlib.h>

#define newline printf("\n")

typedef struct node Node;
struct node
{
    int VRTX;
    Node *NEXT;
};

typedef struct graph Graph;
struct graph
{
    int N; // number of nodes
    Node **LIST; // adjacency list; dummy cell 0
    int *pred; // DFS tree father
    int *d; // discovery time array
    int *f; // finish time array
};

/*
GLOBAL
*/
int TIME = 0;
char *COLOR; // w = White, g = Gray, b = Black
/*
GLOBAL
*/

void DFS_DRIVER (Graph *);
void DFS (Graph *, int);
Graph *read_input();
void PRINT_GRAPH (Graph *);

// Utilities
void PRINT_LIST (Node *);
void FREE_LIST (Node *);
void FREE_GRAPH (Graph *);

int main()
{
    Graph *G;
    G = read_input();
    // printf("okay!\n");
    
    DFS_DRIVER (G);
    PRINT_GRAPH (G);
    FREE_GRAPH (G);
    
    return 0;
}

Node *create_node (int vrtx)
{
    Node *temp = (Node *) malloc (sizeof(Node));
    if (temp == NULL)
    {
        fprintf(stderr, "Out-of-Memory Error.\n");
        exit(1);
    }
    temp->VRTX = vrtx;
    temp->NEXT = NULL;

    return temp;
}

/**
Reads adjacency matrix of a directed graph
and returns a Graph G. Other attributes of
G are not initialized.
**/
Graph *read_input()
{
    Graph *G = (Graph *) malloc (sizeof(Graph));
    scanf("%d", &(G->N));
    
    G->LIST = (Node **) malloc ((G->N + 1) * sizeof(Node *));
    
    int edge;
    Node *end;
    int i, j;
    for (i = 1; i <= G->N; i++)
    {
        G->LIST[i] = NULL;
        for (j = 1; j <= G->N; j++)
        {
            scanf("%d", &edge);
            // printf("%d", j);
            if (edge == 1)
            {
                // printf("(%d, %d)", i, j);
                if (G->LIST[i] == NULL)
                {
                    end = create_node(j);
                    G->LIST[i] = end;
                }
                else
                {
                    end->NEXT = create_node(j);
                    end = end->NEXT;
                }
            }
        }
        // PRINT_LIST (G->LIST[i]);
        // FREE_LIST (G->LIST[i]);
    }
    
    G->pred = (int *) malloc ((G->N + 1)*sizeof(int));
    G->d = (int *) malloc ((G->N + 1)*sizeof(int));
    G->f = (int *) malloc ((G->N + 1)*sizeof(int));
    
    return G;
}

void DFS_DRIVER (Graph *G) // From Quiwa
{
    TIME = 0;
    COLOR = (char *) malloc ((G->N + 1)*sizeof(char));
    G->pred[1] = 0;
    for (int i = 1; i <= G->N; i++) COLOR[i] = 'w';
    for (int i = 1; i <= G->N; i++)
    {
        if (COLOR[i] == 'w') DFS (G, i);
    }
    return;
}

void DFS (Graph *G, int i) // From Quiwa
{
	COLOR[i] = 'g';
    TIME++;
    G->d[i] = TIME;
    Node *current = G->LIST[i];
    while (current != NULL)
    {
        int v = current->VRTX;
        if (COLOR[v] == 'w')
        {
            G->pred[v] = i;
            DFS (G, v);
        }
        current = current->NEXT;
    }
    COLOR[i] = 'b';
    TIME++;
    G->f[i] = TIME;

    return;
}

/**
Prints the predecessor array, discovery times,
and finish times.
**/
void PRINT_GRAPH (Graph *G)
{
    int i;
    for (i = 1; i <= G->N; i++) printf("-%d-", G->pred[i]);
    newline;
    for (i = 1; i <= G->N; i++) printf("-%d-", G->d[i]);
    newline;
    for (i = 1; i <= G->N; i++) printf("-%d-", G->f[i]);
    newline;
    
    return;
}

////
// Utilities Section
////

void PRINT_LIST (Node *start)
{
    Node *current = start;
    while (current != NULL)
    {
        printf("%d -> ", current->VRTX);
        current = current->NEXT;
    }
    printf("NULL\n");
    return;
}

/**
Deallocates a linked list
**/
void FREE_LIST (Node *start)
{
    Node *current, *obsolete;
    current = start;
    while (current != NULL)
    {
        obsolete = current;
        current = current->NEXT;
        free(obsolete);
    }
    // printf("Free Success\n");
    return;
}

void FREE_GRAPH (Graph *G)
{
    int i;
    for (i = 1; i <= G->N; i++) FREE_LIST (G->LIST[i]);
    free (G->pred);
    free (G->f);
    free (G->d);
    free (G);
}