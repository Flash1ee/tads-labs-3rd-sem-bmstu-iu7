#ifndef __PRINT_H__
#define __PRINT_H__

#include <stdio.h>
#include "graph.h"

#define graph_file "../data/graph.gv"

void export_graph_to_dot(FILE *f, const char *name, int **matrix, int n);

#endif //__PRINT_H__