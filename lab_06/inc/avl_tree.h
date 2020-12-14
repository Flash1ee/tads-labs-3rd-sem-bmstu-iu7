#ifndef __AVL_TREE__
#define __AVL_TREE__

#include <inttypes.h>
#include <stdio.h>

typedef struct avl_node avl_node_t;

struct avl_node {
    int data;
    int8_t height;
    avl_node_t *left;
    avl_node_t *right;
};

int8_t avlHeight(avl_node_t *node);

void avlFixHeight(avl_node_t *node);

int8_t avlHeightFactor(avl_node_t *node);

avl_node_t *avlLeftRotate(avl_node_t *node);

avl_node_t *avlRightRotate(avl_node_t *node);

avl_node_t *avlBalance(avl_node_t *node);

avl_node_t *avlInsert(avl_node_t *node, int value);

avl_node_t *avlSearch(avl_node_t *tree, int value);

avl_node_t *avlRemoveMin(avl_node_t *node);

avl_node_t *avlRemove(avl_node_t *node, int value);

avl_node_t *avlFindMin(avl_node_t *node);

void avlInOrder(avl_node_t *node, void (*visit)(avl_node_t *, void *arg), void *arg);

void *avlPrintNode(avl_node_t *node, void *arg);

void freeAvlNode(avl_node_t *node, void *arg);

void freeAvl(avl_node_t *tree);

avl_node_t *readAvlFromFile(const char *name);

avl_node_t *avlRead(FILE *in);

#endif  // __AVL_TREE__