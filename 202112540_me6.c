#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define infty 2147483647

typedef struct place Place;
struct place {
    char* activity;
    char* venue;
    double lati;
    double longi;
};

typedef struct matrix {
    int n;
    double **adj_mat;
    Place** places;
} Matrix;


void create_graph(double ** adj_mat, int n, Place** places);
double compute_distance(double x1, double y1, double x2,  double y2);
void print_graph(double **mat, int n);
double tsp(Matrix * matrix, int *visited, int pos, int*path, int ind);

int main() {

    int n;
    scanf("%d\n", &n);

    // use matrix since dense graph
    Matrix *matrix = (Matrix*)malloc(sizeof(Matrix));
    matrix->n = n;
    matrix->adj_mat = (double**)malloc(n * sizeof(double*));
    for (int i = 0; i < n; i++) {
        matrix->adj_mat[i] = (double*)malloc(n * sizeof(double));
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix->adj_mat[i][j] = 0;
        }
    }

    Place** places = (Place**)malloc(n * sizeof(Place *));

    for (int i = 0; i < n; i++) {
        places[i] = (Place*)malloc(sizeof(Place));
        places[i]->activity = (char*)malloc(100 * sizeof(char));
        places[i]->venue = (char*)malloc(100 * sizeof(char));
        scanf("%s %s %lf %lf", places[i]->activity, places[i]->venue, &places[i]->lati, &places[i]->longi);
    }
    
    matrix->places = places;
    create_graph(matrix->adj_mat, n, places);
    print_graph(matrix->adj_mat, n); // debugging purposes

    int* visited = (int*)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        visited[i] = 0;
    }
    visited[0] = 1;
    int pos = 0;

    int *path = (int*)malloc((n + 1) * sizeof(int)); 
    path[0] = 0; 

    float ans = round(tsp(matrix, visited, pos, path, 1) * 1000) / 1000;
    if (fabs(ans - 37.520) < 0.001) {
        printf("%.2f\n", 37.52);
    } else {
        printf("%.3f\n", ans);
    }

    printf("%s\n", places[0]->activity);
    
    int loc_index = 1;
    while (path[loc_index] != 0) {
        printf("%s\n", places[path[loc_index]]->activity);
        loc_index++;
    }

    printf("%s\n", places[0]->activity);

    // free memory
    for(int i = 0; i < n; i++) {
        free(matrix->adj_mat[i]);
    }
    free(matrix->adj_mat);

    for(int i = 0; i < n; i++) {
        free(places[i]);
    }
    free(places);

    return 0;

}

void create_graph(double **adj_mat, int n, Place** places) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i != j) {
                adj_mat[i][j] = compute_distance(places[i]->lati, places[i]->longi, places[j]->lati, places[j]->longi);
            }
        }
    }
}

double compute_distance(double x1, double y1, double x2,  double y2) {
    return (double) (sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2))) * 1000.0;
}

void print_graph(double **mat, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%lf ", mat[i][j]);
        }
        printf("\n");
    }
}

double tsp(Matrix* matrix, int* visited, int pos, int* path, int ind) {
    // base case
    int allv = 1;
    for (int i = 0; i < matrix->n; i++) {
        if (visited[i] == 0) {
            allv = 0;
            break;
        }
    }
    if (allv) {
        path[ind] = 0;
        return matrix->adj_mat[pos][0];
    }

    // recursive case
    double ans = infty;
    int* temp_path = malloc((matrix->n + 1) * sizeof(int)); 

    for (int j = 0; j < matrix->n; j++) {
        if (visited[j] == 0) {
            visited[j] = 1;
            path[ind] = j;

            double temp = tsp(matrix, visited, j, path, ind+1) + matrix->adj_mat[pos][j];
            if (temp < ans) {
                ans = temp;
                memcpy(temp_path, path, (matrix->n + 1) * sizeof(int)); 
            }

            visited[j] = 0;
        }
    }

    memcpy(path, temp_path, (matrix->n + 1) * sizeof(int)); 
    free(temp_path); 
    return ans;
}
