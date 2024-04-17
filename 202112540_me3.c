#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define newline printf("\n")

typedef struct binarytreenode BinaryTreeNode;
struct binarytreenode
{
    BinaryTreeNode *LSON;
    char DATA;
    int FREQ;
    BinaryTreeNode *RSON;
};

typedef struct priorityqueue
{
    int size;
    int capacity;
    BinaryTreeNode **array;
} PriorityQueue;

void PQ_INSERT (PriorityQueue *, BinaryTreeNode *);
BinaryTreeNode *PQ_EXTRACT (PriorityQueue *);
void HEAPIFY (PriorityQueue*, int);
void PQ_OVERFLOW(void);
void PQ_UNDERFLOW(void);

void CREATE_PQ(PriorityQueue **);
BinaryTreeNode *BUILD_HUFFMAN_TREE (PriorityQueue *);
void PRINT_HUFFMAN_CODE (BinaryTreeNode *, int *, int);
BinaryTreeNode **read_input(int *);
void FREE_PQ(PriorityQueue *);
void FREE_TREE (BinaryTreeNode *);

/*
[FREEBIE]
*/
int main()
{
    PriorityQueue *HuffmanCode;
    BinaryTreeNode *HuffmanRoot;
    
    CREATE_PQ(&HuffmanCode);
    HuffmanRoot = BUILD_HUFFMAN_TREE (HuffmanCode);
    int *code = (int *) malloc (HuffmanCode->capacity*sizeof(int));
    PRINT_HUFFMAN_CODE (HuffmanRoot, code, 0);
    FREE_TREE (HuffmanRoot);
    FREE_PQ(HuffmanCode);
    free (code);
    return 0;
}

/*
[FREEBIE]
*/
BinaryTreeNode *create_node(char data)
{
    BinaryTreeNode *node = NULL;
    
    node = (BinaryTreeNode *) malloc (sizeof(BinaryTreeNode));
    node->DATA = data;
    node->FREQ = 1;
    node->LSON = NULL;
    node->RSON = NULL;
    
    return node;
}

/*
[FREEBIE]
*/
void free_node (BinaryTreeNode *node)
{
    free(node);
    return;
}

/*
[FREEBIE]
*/
void free_node_array (BinaryTreeNode **array, int size)
{
    int i;
    
    for (i = 0; i < size; i++)
        free_node(array[i]);
    
    return;
}

/*
[FREEBIE]
*/
BinaryTreeNode *search_node_array(BinaryTreeNode **array, int size, char data)
{
    int i;
    
    for (i = 0; i < size; i++)
        if (array[i]->DATA == data) return array[i];
    
    return NULL;
}

/**
Returns an unsorted array of BinaryTreeNodes.
[FREEBIE]
**/
BinaryTreeNode **read_input(int *size)
{
    int n = 0, i;
    char symbol;
    BinaryTreeNode **array = NULL, **temp;
    BinaryTreeNode *node = NULL;
    
    while(scanf("%c", &symbol) != EOF)
    {
        if(symbol == ';') break; //  check for termination char
        if (n == 0)
        {
            array = (BinaryTreeNode **) malloc (sizeof(BinaryTreeNode *));
            array[0] = create_node(symbol);
            n++;
        }
        else
        {
            node = search_node_array(array, n, symbol);
            
            if (node == NULL)
            {
                temp = array;
                array = (BinaryTreeNode **) malloc ((n+1)*sizeof(BinaryTreeNode *));
                for (i = 0; i < n; i++) array[i] = temp[i];
                array[n] = create_node(symbol);
                free(temp);
                n++;
            }
            else
            {
                node->FREQ++;
            }
        }
    }
    
    *size = n;
    
    return array;
}

/*
[FREEBIE]
*/
void CREATE_PQ (PriorityQueue **P)
{
    BinaryTreeNode **array = NULL;
    BinaryTreeNode *x;
    int i, n;
    
    *P = (PriorityQueue *) malloc (sizeof(PriorityQueue));
    array = read_input(&(*P)->capacity);
    
    (*P)->array = array;
    (*P)->size = (*P)->capacity;
    
    n = (*P)->size - 1;
    for (i = n-1/2; i >= 0; i--)
        HEAPIFY (*P, i);
    
    return;
}

void FREE_PQ (PriorityQueue *P)
{
    // free_node_array(P->array, P->capacity);
    free(P->array);
    free(P);
    return;
}

void FREE_TREE (BinaryTreeNode *alpha)
{
    if (alpha != NULL)
    {
        FREE_TREE (alpha->LSON);
        FREE_TREE (alpha->RSON);
        free(alpha);
    }
}

/*
PQ_INSERT for heapified priority queue.
[IMPLEMENT THIS]
*/
void PQ_INSERT (PriorityQueue *P, BinaryTreeNode *x)
{
    if (P->size == P->capacity) PQ_OVERFLOW();
    
    /*
    Write code here.
    */
    P->size = P->size + 1;
    int i = P->size - 1;
    int j = i/2;
    while (i > 0 && P->array[j]->FREQ > x->FREQ) // changed comparison to > since min heap
    {
        P->array[i] = P->array[j];
        i = j;
        j = i/2;
    }
    P->array[i] = x;

    /*
    
    Pseudocode: Assumed indexing is 1 to n (= P->size)
    P->size <- P->size+1 
    i <- P->size
    j <- floor(i/2)
    while i > 1 and P->array[j] < x do
        P->array[i] <- P->array[j]
        i <- j
        j <- floor (i/2)
    endwhile
    P->array[i] <- x
    */
    
    return;
}

/*
PQ_EXTRACT for heapified priority queue.
[IMPLEMENT THIS]
*/
BinaryTreeNode *PQ_EXTRACT (PriorityQueue *P)
{
    BinaryTreeNode *x;
    
    if (P->size == 0) PQ_UNDERFLOW();
    
    /*
    Write code here
    */
    x = P->array[0];
    P->array[0] = P->array[P->size - 1];
    P->size = P->size - 1;
    HEAPIFY(P, 0);
    /*
    Pseudocode: Assumed indexing is 1 to n (= P->size)
    x <- P->array[1]
    P->array[1] <- P->array[n]
    n <- n - 1
    call HEAPIFY (P, 1)
    */
    
    return x;
}

/*
HEAPIFY for array implementation.
[IMPLEMENT THIS]
*/
void HEAPIFY (PriorityQueue *P, int r)
{
    /*
    Write code here
    */
    BinaryTreeNode *temp = P->array[r];

    int k = temp->FREQ;
    int i = r;
    int j = 2*i;
    while (j <= P->size - 1)
    {   // build min heap so build huffman tree always extract the minimum
        if (j < P->size - 1 && P->array[j+1]->FREQ < P->array[j]->FREQ) j = j + 1;
        if (P->array[j]->FREQ < k)
        {
            P->array[i] = P->array[j]; // move jth node up
            i = j;
            j = 2*i;
        }
        else break;
    }
    P->array[i] = temp;
    return;
    /*
    Pseudocode: Assumed indexing is 1 to n (= P->size)
    procedure HEAPIFY (P, r)
    k <- KEY(r)
    i <- r
    j <- 2*i
    while j <= P->size
        if j < P->size and KEY(j+1) > KEY(j) then j <- j + 1
        if KEY(j) > k then
            KEY(i) <- KEY(j)
            i <- j
            j <- 2*i
        else break loop
    endwhile
    KEY(i) <- k
    end HEAPIFY
    */
}

/*
[IMPLEMENT THIS]
*/
BinaryTreeNode *BUILD_HUFFMAN_TREE (PriorityQueue *P)
{
    /*
    Write code here
    */
    int n = P->size;
    for (int i = 1; i <= n-1; i++)
    {
        BinaryTreeNode *z = (BinaryTreeNode *) malloc (sizeof(BinaryTreeNode));
        z->LSON = PQ_EXTRACT(P);
        z->RSON = PQ_EXTRACT(P);
        z->FREQ = z->LSON->FREQ + z->RSON->FREQ;
        PQ_INSERT(P, z);
    }
    return PQ_EXTRACT(P);

    /*
    n = |P|
    for i = 1 to n-1
        allocate new node z
        z.left = x = EXTRACT-MIN(P)
        z.right = y = EXTRACT-MIN(P)
        z->FREQ = x->FREQ + y->FREQ
        INSERT(P, z)
    endfor    
    return EXTRACT-MIN(P)
    */
}

/*
[FREEBIE]
*/
void PRINT_HUFFMAN_CODE (BinaryTreeNode *root, int *code, int top)
{
    int i;
    
    if (root->LSON != NULL)
    {
        code[top] = 0;
        PRINT_HUFFMAN_CODE (root->LSON, code, top+1);
    }
    
    if (root->RSON != NULL)
    {
        code[top] = 1;
        PRINT_HUFFMAN_CODE (root->RSON, code, top+1);
    }
    
    if (root->LSON == NULL && root->RSON == NULL)
    {
        printf("%c :: ", root->DATA);
        for (i = 0; i < top; i++)
            printf("%d", code[i]);
        newline;
    }
}

void PQ_OVERFLOW(void)
{
    printf("Priority Queue overflow detected.\n");
    exit(1);
}

void PQ_UNDERFLOW(void)
{
    printf("Priority Queue underflow detected.\n");
    exit(1);
}