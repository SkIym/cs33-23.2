
def create_matrix(activities, n):
    matrix = [[0 for i in range(n)] for j in range(n)]

    for i in range(n):
        for j in range(n):
            if i != j:
                matrix[i][j] = (((float(activities[i][2]) - float(activities[j][2]))**2 + (float(activities[i][3]) - float(activities[j][3]))**2)**0.5)*1000
            else:
                matrix[i][j] = float('inf')

    return matrix

def create_mst(mat, n): # Prim's Algorithm
    
    # U set = visited 
    visited = []
    visited.append(0)
    tree = []
    while len(visited) < n:
        min = float('inf')
        for i in visited:
            for j in range(n):
                if j not in visited and mat[i][j] < min:
                    min = mat[i][j]
                    u = i
                    v = j
        visited.append(v)
        tree.append([u, v, min])

    return tree

def dfs(tree, n):
    visited = [False for i in range(n)]
    stack = []
    stack.append(0)
    visited[0] = True
    tour = [0]
    while stack:
        v = stack[-1]
        for i in range(len(tree)): # guaranteed preordered because of line 24
            if tree[i][0] == v and not visited[tree[i][1]]:
                stack.append(tree[i][1])
                visited[tree[i][1]] = True
                tour.append(tree[i][1])
                #print(tree[i])
                break
        else:
            stack.pop()
    return tour


if __name__ == '__main__':
    n = int(input())
    activities = []
    for i in range(n):
        # <activity> <venue> <latitude> <longitude>
        activities.append(input().split())
    adj_matrix = create_matrix(activities, n)
    mst = create_mst(adj_matrix, n)

    tour = dfs(mst, n)

    #v = tour[-1]
    # backtrack
    # while True:
    #     for i in range(len(mst)):
    #         if mst[i][1] == v:
    #             cost += mst[i][2]
    #             v = mst[i][0]
    #             print(mst[i])
    #             for j in range(len(mst)):
    #                 if mst[j][0] == v and mst[j][1] != mst[i][1]:
    #                     cost += mst[j][2]
    #                     print(mst[j])
    #             break
    #     if v == 0:
    #         break

    cost = 0
    u, v = tour[0], tour[1]
    for i in range(0, len(tour)-1):
        u, v = tour[i], tour[i+1]
        cost += adj_matrix[u][v]
    cost += adj_matrix[tour[-1]][0]
    print(round(cost, 3))

    for i in range(len(tour)):
        print(activities[tour[i]][0])
    print(activities[0][0])

    # for i in range(len(adj_matrix[0])):
    #     print(adj_matrix[i])