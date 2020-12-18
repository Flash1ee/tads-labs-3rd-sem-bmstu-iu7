#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>

#include "graph.h"

int **create_matrix_adjacency(int n, int m) {
    int **matrix = calloc(n, sizeof(int *));
    assert(matrix);
    for (int i = 0; i < n; i++) {
        matrix[i] = malloc(sizeof(int) * m);
        for (int j = 0; j < m; j++) {
            if (i == j) {
                matrix[i][j] = 0;
            }
            else {
                matrix[i][j] = INT_MIN;
            }
        }
    }
    return matrix;
}
void delete_matrix_adjacency(int **matrix, int n) {
    for (int i = 0; i < n; i++) {
        free(matrix[i]);
    }
    free(matrix);
} 

void stack_init(stack_t *stack, int capacity) {
    stack->size = 0;
    stack->capacity = capacity;

    stack->data = malloc(sizeof(int) * capacity);
    assert(stack->data);
}


int stack_is_empty(stack_t *stack) {
    return stack->size == 0;
}
int stack_is_full(stack_t *stack) {
    return stack->size == stack->capacity;
}

int stack_pop(stack_t *stack) {
    if (stack_is_empty(stack)) {
        return INT_MIN;
    }
    stack->size--;
    return stack->data[stack->size];
}
int stack_push(stack_t *stack, int elem) {
    if (stack_is_full(stack)) {
        return 0;
    }

    stack->data[stack->size++] = elem;

    return 1;
}
// adj_vertex *create_adj_vertex(int v, int weight) {
//     adj_vertex *adj = malloc(sizeof(adj_vertex));
//     if (!adj) {
//         return NULL;
//     }
//     adj->v = v;
//     adj->weight = weight;
//     return adj;
// }

// void destroy_adj_vertex(adj_vertex *adj) {
//     free(adj);
// }

// graph *create_graph(int cnt) {
//     graph *gr = calloc(1, sizeof(graph));
//     if (!gr) {
//         return NULL;
//     }
//     gr->cnt_v = cnt;
//     gr->size = cnt;
//     gr->adj_list = calloc(cnt, sizeof(adj_vertex*));
//     if (!gr->adj_list) {
//         free(gr);
//         return NULL;
//     }
//     return gr;
// }

// void destroy_graph(graph *graph) {
//     for (int i = 0; i < graph->cnt_v; ++i) {
//         destroy_adj_vertex(graph->adj_list[i]);
//     }
//     free(graph);
// }
// add edge for u->v with weight
void add_edge(int **arr, int u, int v, int weight) {
    arr[u][v] = weight;
}
void topological_sort(int v, int *visited, stack_t *stack, int **arr, int n) {

    visited[v] = 1;

    for (int i = 0; i < n; ++i) {
        if (arr[v][i] && visited[i] == -1) {
            topological_sort(i, visited, stack, arr, n);
        }

    }
    stack_push(stack, v);
}

void longest_path(int **arr,int n, int s) {

    stack_t stack;
    stack_init(&stack, n);
    
    int dist[n];
    int parent[n];
    int visited[n];


    for (int i = 0; i < n; ++i) {
        visited[i] = -1;
        parent[i] = -1;
    }

    for (int i = 0; i < n; ++i) {
        if (visited[i] == -1) {
            topological_sort(i, visited, &stack, arr, n);
        }
    }

    for (int i = 0; i < n; ++i) {
        dist[i] = INT_MIN;
    }

    dist[s] = 0;

    while (!stack_is_empty(&stack)) {
        int u = stack_pop(&stack);

        if (dist[u] != INT_MIN) {
            for (int i = 0; i < n; ++i) {
                if (arr[u][i] && arr[u][i] != INT_MIN) {
                    if (dist[i] < dist[u] + arr[u][i]) {
                        dist[i] = dist[u] + arr[u][i];
                        parent[i] = u;
                    }
                }
                }
            }
        }
 
    for (int i = 0; i < n; ++i) {
        if (dist[i] == INT_MIN) {
            printf("Вершина %d : INF\tПуть: ", i);
        }
        else {
            printf("Вершина %d : %d\tПуть: ", i,dist[i]);
        }
        stack_t st;
        stack_init(&st, n);
        int j = i;

        while (parent[j] != -1) {
            stack_push(&st, j);
            j = parent[j];
        }
        if (dist[i] != INT_MIN) {
            stack_push(&st, s);
        }
        while (!stack_is_empty(&st)) {
            printf("%d ", stack_pop(&st));
        }
        puts("");

    }
    puts("");

}
int **read_from_file(FILE *f, int *size) {
    if (!f || !size) {
        return NULL;
    }
    int cnt_v = -1;
    int edges = -1;

    if (fscanf(f,"%d%d", &cnt_v, &edges) != 2 || cnt_v < 0 || edges < 0) {
        return NULL;
    }
    int **arr = create_matrix_adjacency(cnt_v, cnt_v);
    if (!arr) {
        return NULL;
    }
    int u = -1;
    int v = -1;
    int weight = -1;

    for (int str = 0; str < edges; str++) {
        if (fscanf(f, "%d%d%d", &u, &v, &weight) != 3 || u < 0 || v < 0) {
            delete_matrix_adjacency(arr, cnt_v);
            return NULL;
        }
        add_edge(arr, u, v, weight);
    }
    
    *size = cnt_v;
    return arr;
}