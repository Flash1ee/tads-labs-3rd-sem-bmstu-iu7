#ifndef __GRAPH_H__
#define __GRAPH_H__

typedef struct {
    int *data;
    int size;
    int capacity;
} stack_t;

void delete_matrix_adjacency(int **matrix, int n);

int **create_matrix_adjacency(int n, int m);

void stack_init(stack_t *stack, int capacity);

int stack_is_empty(stack_t *stack);

int stack_pop(stack_t *stack);

int stack_push(stack_t *stack, int adj);

void add_edge(int **arr, int u, int v, int weight);

void topological_sort(int v, int *visited, stack_t *stack, int **arr, int n);

void longest_path(int **arr,int n, int s);

int **read_from_file(FILE *f, int *size);


#endif  //  __GRAPH_H__