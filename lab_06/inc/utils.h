#ifndef __UTILS_H__
#define __UTILS_H__

#include "tree.h"
#include "avl_tree.h"

avl_node_t *preTreeLook(tree_node_t *tree, void *(*act)(avl_node_t *avl, int value), avl_node_t *avl);

avl_node_t *bstToAvl(tree_node_t *bst, avl_node_t *avl);

void treeOperations(tree_node_t **bTree, avl_node_t **avlTree, char *f_name);

void timeOperations();


#endif  // __UTILS_H__