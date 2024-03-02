#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define newline printf("\n")

typedef int BinaryTreeDataType;
typedef struct binarytreenode BinaryTreeNode;
struct binarytreenode
{
    int LTAG;
    BinaryTreeNode *LSON;
    BinaryTreeDataType DATA;
    BinaryTreeNode *RSON;
    int RTAG;
};


struct edge
{
	int name;
    int lson;
    int rson;
};
typedef struct edge Edge;

void CREATE(BinaryTreeNode **);
void POSTORDER_T (BinaryTreeNode *);
void PREORDER_T (BinaryTreeNode *);
void INORDER_T (BinaryTreeNode *);
BinaryTreeNode *INSUC(BinaryTreeNode *);
BinaryTreeNode *INPRED(BinaryTreeNode *);
BinaryTreeNode *PRESUC(BinaryTreeNode *);
BinaryTreeNode *POSTSUC(BinaryTreeNode *);
void VISIT (BinaryTreeNode *);
void FREE_TREE (BinaryTreeNode *);
int read_tree_input(Edge **, int **);
int LOOK (int *, int, int);

BinaryTreeNode *create_node(BinaryTreeDataType);

int main()
{
    BinaryTreeNode *H;
    
    CREATE(&H);
    INORDER_T(H);
    newline;
    PREORDER_T(H);
    newline;
    POSTORDER_T(H);
    newline;

    FREE_TREE(H->LSON);
    free(H);
    return 0;
}

int read_tree_input (Edge **edges, int **inorder_list)
{
	int i, n;
    int lson, rson, name;
	
	scanf("%d\n", &n);
    *edges = (Edge *) malloc (n*sizeof(Edge));
    for (i = 0; i < n; i++)
    {
        (*edges)[i].name = i+1;
        
        scanf("%d %d\n", &lson, &rson);
        (*edges)[i].lson = lson;
        (*edges)[i].rson = rson;
    }
    *inorder_list = (int *) malloc (n*sizeof(int));
    for (i = 0; i < n; i++)
    {
        scanf("-%d-", &name);
        (*inorder_list)[i] = name;
    }
	
	return n;
}

void CREATE (BinaryTreeNode **H)
{
	Edge *edges;
    int *inorder_list;
    BinaryTreeNode **nodes;
    BinaryTreeNode *head;
	int n;
	int i = 0, index;
	
	n = read_tree_input(&edges, &inorder_list);
    
    /**
    Head node of the threaded binary tree.
    **/
    head = (BinaryTreeNode *) malloc (sizeof(BinaryTreeNode));
    head->DATA = -1;
    head->RSON = head;
    head->RTAG = 1;
	
    nodes = (BinaryTreeNode **) malloc (n*sizeof(BinaryTreeNode*));
	for (i = 0; i < n; i++)
    {
        nodes[i] = (BinaryTreeNode *) malloc (sizeof(BinaryTreeNode));
        nodes[i]->DATA = edges[i].name;
        nodes[i]->LSON = NULL;
        nodes[i]->LTAG = 0;
        nodes[i]->RSON = NULL;
        nodes[i]->RTAG = 0;
    }
    
    head->LSON = nodes[0];
    head->LTAG = 1;
    
    /*
    Creates the regular and thread links
    */
    for (i = 0; i < n; i++)
    {
        
        if (edges[i].lson > 0) 
        {
            nodes[i]->LSON = nodes[edges[i].lson-1];
            nodes[i]->LTAG = 1;
        }
        else
        {
            index = LOOK (inorder_list, n, edges[i].name);
            if (index == 0) nodes[i]->LSON = head;
            else nodes[i]->LSON = nodes[inorder_list[index-1]-1];
        }
        
        if (edges[i].rson > 0) 
        {
            nodes[i]->RSON = nodes[edges[i].rson-1];
            nodes[i]->RTAG = 1;
        }
        else
        {
            index = LOOK (inorder_list, n, edges[i].name);
            if (index == n-1) nodes[i]->RSON = head;
            else nodes[i]->RSON = nodes[inorder_list[index+1]-1];
        }
        
    }
    
    *H = head;
	
	free(edges);
    free(inorder_list);
}

void VISIT(BinaryTreeNode *T)
{
    printf("-%d-", T->DATA);
}

/**
Deallocating a Binary Tree should be
done in POSTORDER. Suggested is iterative
to avoid stack overflow.
**/
void FREE_TREE (BinaryTreeNode *H)
{
    if (H->LTAG == 1) FREE_TREE(H->LSON);
    if (H->RTAG == 1) FREE_TREE(H->RSON);    
    free(H);
    
    return;
}

/*
Standard linear search
*/
int LOOK (int *list, int size, int key)
{
    int i;
    for (i = 0; i < size; i++)
        if (list[i] == key) return i;
    
    return -1;
}

// Traversals Onward
BinaryTreeNode *INSUC(BinaryTreeNode *alpha)
{
    BinaryTreeNode *insuc = alpha->RSON;
    if (alpha->RTAG == 1) {
        while (insuc->LTAG == 1) insuc = insuc->LSON;
    }
    return insuc;
}

BinaryTreeNode *INPRED (BinaryTreeNode *alpha)
{
    BinaryTreeNode *inpred = alpha->LSON;
    if (alpha->LTAG == 1) {
        while (inpred->RTAG == 1) inpred = inpred->RSON;
    }
    return inpred;
}

void INORDER_T (BinaryTreeNode *H)
{
    BinaryTreeNode *alpha = H;
    while(1) {
        alpha = INSUC(alpha);
        if (alpha == H) return;
        VISIT(alpha);
    }
}

BinaryTreeNode *PRESUC (BinaryTreeNode *alpha)
{
    if (alpha->LTAG == 1) return alpha->LSON;
    else {
      BinaryTreeNode *presuc = alpha;
      while (presuc->RTAG == 0) presuc = presuc->RSON;
      return presuc->RSON;
    }
}

void PREORDER_T (BinaryTreeNode *H)
{
    BinaryTreeNode *alpha = H;
    while (1)
    {
        alpha = PRESUC(alpha);
        if(alpha == H) return;
        VISIT(alpha);
    }

}

BinaryTreeNode *POSTSUC (BinaryTreeNode *alpha)
{
    // observations:
    // if alpha is the right child, return parent
    // if alpha is the left child, 
        // if it has no sibling, return parent
        // else return leftmost node of the right siblings' tree

    int is_left_child = 0;
    int is_right_child = 0;

    // Find parent to determine if alpha is left or right child
    
    BinaryTreeNode *parent = alpha;
    while(parent->RTAG == 1) parent = parent->RSON;
    parent = parent->RSON;

    if(parent->LSON == alpha) {
        is_left_child = 1;
    }
    else {
        parent = parent->LSON;
        while(parent->RSON != alpha) parent = parent->RSON;
        is_right_child = 1;
    }

    // Find successor

    if (is_right_child) return parent;
    else {
        if (parent->RTAG == 0) return parent;
        else {
            BinaryTreeNode *postsuc = parent->RSON;
            while (postsuc->RTAG == 1 || postsuc->LTAG == 1) {
                if (postsuc->LTAG == 1) postsuc = postsuc->LSON;
                else postsuc = postsuc->RSON;
            }
            return postsuc;
        }
    }
}

void POSTORDER_T (BinaryTreeNode *H)
{
    BinaryTreeNode *alpha = H->LSON;

    // Find leftmost/rightmost node of left subtree

    while (alpha->RTAG == 1 || alpha->LTAG == 1) {
        if (alpha->LTAG == 1) alpha = alpha->LSON;
        else alpha = alpha->RSON;
    }
    VISIT(alpha);

    while (1)
    {
        alpha = POSTSUC(alpha);
        VISIT(alpha);
        if(alpha == H->LSON) return;
    }
}