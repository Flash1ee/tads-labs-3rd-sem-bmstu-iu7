#ifndef __VIEW_H__
#define __VIEW_H__

#include "tree.h"

#define BST 1
#define AVL 2

void treeExportToDot(FILE *f, const char *tree_name, void *tree, int flag);
void nodeToDot(void *node, void *param, int flag);

#endif  // __VIEW_H__