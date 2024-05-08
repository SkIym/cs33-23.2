import math

# right rotation (when a node is inserted into the left subtree of the left son)
# left rotation (when a node is inserted into the right subtree of the right son)
# left-right rotation (when a node is inserted into the right subtree of the left son)
# right-left rotation (when a node is inserted into the left subtree of the right son)

class Node:
    def __init__(self, data):
        self.data = data
        self.left = None
        self.right = None
        self.bf = 0
    
    def rotate_right(self, beta):
        self.left = beta.right
        beta.right = self
        self.bf = 0
        beta.bf = 0

    def rotate_left(self, beta):
        self.right = beta.left
        beta.left = self
        self.bf = 0
        beta.bf = 0

    def rotate_left_right(self, p):
        beta = self.left
        beta.right = p.left
        p.left = beta
        self.left = p.right
        p.right = self

        if p.bf == 0:
            self.bf = 0
            beta.bf = 0
        elif p.bf == 1:
            self.bf = 0
            beta.bf = -1
        elif p.bf == -1:
            self.bf = 1
            beta.bf = 0
        
        p.bf = 0

    def rotate_right_left(self, p):
        beta = self.right
        beta.left = p.right
        p.right = beta
        self.right = p.left
        p.left = self

        if p.bf == 0:
            self.bf = 0
            beta.bf = 0
        elif p.bf == 1:
            self.bf = -1
            beta.bf = 0
        elif p.bf == -1:
            self.bf = 0
            beta.bf = 1
        
        p.bf = 0

class AVL:
    def __init__(self):
        self.root = None

# Reference: Quiwa Session 14.3
def insert_node(node, tree):
    if tree.root == None:
        tree.root = node
        tree.root.left = None
        tree.root.right = None
        tree.root.bf = 0
        # print('first node inserted')
        return

    alpha = tree.root  # alpha is the node where rebalancing may have to be done
    gamma = tree.root  # root node
    phi = None         # parent of node alpha
                       # tau is the node where the new node is to be inserted

    # insert node

    while 1:
        if node.data == gamma.data:
            break
        if node.data < gamma.data:
            tau = gamma.left
            if tau == None:
                gamma.left = node
                break
        if node.data > gamma.data:
            tau = gamma.right
            if tau == None:
                gamma.right = node
                break
        if tau.bf != 0: 
            alpha = tau
            phi = gamma

        gamma = tau

    # fill the node with data
    node.left = None
    node.right = None
    node.bf = 0

    # adjust balanc factors
    if node.data < alpha.data:
        gamma = alpha.left
        beta = alpha.left
    else:
        gamma = alpha.right
        beta = alpha.right
    
    while gamma != node:
        if node.data < gamma.data:
            gamma.bf = -1
            gamma = gamma.left
        else:
            gamma.bf = 1
            gamma = gamma.right

    # rebalance subtree at node alpha

    if (node.data < alpha.data):  # if w = -1, node is inserted at left subtree of alpha, if 1, right subtree!
        w = -1
    else: 
        w = 1
    
    # print(w, alpha.bf)

    if alpha.bf == 0:
        alpha.bf = w
    elif alpha.bf == -w:
        alpha.bf = 0
        # print('rebalanced!')
    elif alpha.bf == w:
        # print('rebalancing...')
        if node.data < alpha.data and beta.bf == w:
            ANGDAMI = alpha.left
            alpha.rotate_right(alpha.left)
        elif node.data < alpha.data and beta.bf == -w:
            ANGDAMI = alpha.left.right
            alpha.rotate_left_right(alpha.left.right)
        elif node.data > alpha.data and beta.bf == w:
            ANGDAMI = alpha.right
            alpha.rotate_left(alpha.right)
        elif node.data > alpha.data and beta.bf == -w:
            ANGDAMI = alpha.right.left
            alpha.rotate_right_left(alpha.right.left)   
        
        if alpha == tree.root:
            tree.root = ANGDAMI
        elif alpha == phi.left:
            phi.left = ANGDAMI
        elif alpha == phi.right:
            phi.right = ANGDAMI
           

def pre_order_traversal(root):
    if root == None:
        return

    print('-', end='')
    print(root.data, end='')
    # print(f'({root.bf})', end='')
    print('-', end='')
    pre_order_traversal(root.left)
    pre_order_traversal(root.right)

    return



if __name__ == '__main__':
    tree = AVL()
    key = int(input())
    while True:
        node = Node(key)
        insert_node(node, tree)
        pre_order_traversal(tree.root)
        print()
        key = int(input())
        if key == 0:
            break
    