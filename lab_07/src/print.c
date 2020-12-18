#include <limits.h>
#include "print.h"

void export_graph_to_dot(FILE *f, const char *name, int **matrix, int n) {

    fprintf(f, "digraph %s {\n", name);
    fprintf(f, "%s\n","rankdir=LR");
    int flag = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (matrix[i][j] != INT_MIN && i != j) {
                flag = 1;
                fprintf(f, "%d -> %d [label = %d]\n", i, j, matrix[i][j]);
            }
        }
        
        flag = 0;
    }
    fprintf(f, "}\n");
}