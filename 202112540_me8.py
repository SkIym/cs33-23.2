
def dp_knapsack(w, v, W, n):
    V = [[0 for i in range(W + 1)] for j in range(n+1)] # W columns, n rows
    P = [[0 for i in range(W + 1)] for j in range(n+1)]
    for i in range(1, n+1):
        for j in range(1, W+1):
            if w[i] <= j and (v[i] + V[i-1][j-w[i]]) > V[i-1][j]:
                V[i][j] = v[i] + V[i-1][j-w[i]]
                P[i][j] = j-w[i]
            else:
                V[i][j] = V[i-1][j]
                P[i][j] = j
    return V


if __name__ == '__main__':
    W = int(input())
    n = int(input())
    w = [0]
    v = [0]
    for i in range(n):
        # record - > id weight value
        record = input().split()
        w.append(int(record[1]))
        v.append(int(record[2]))
        # print(w, v)
    
    matrix = dp_knapsack(w, v, W, n)
    print(matrix[n][W])
    for i in range(n, 0, -1):
        if matrix[i][W] != matrix[i-1][W]:
            print(i)
            W -= w[i]
        
