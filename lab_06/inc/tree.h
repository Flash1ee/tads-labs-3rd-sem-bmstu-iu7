#ifndef __TREE_H__
#define __TREE_H__


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#ifndef NDEBUG
#define DBG_PRINT(s, ...) printf(s, __VA_ARGS__)
#else
#define DBG_PRINT(s, ...) ((void) 0)
#endif


#define NO_UNIQ -1
#define EMPTY -2

struct tree_node {
    int data;
    struct tree_node *left;
    struct tree_node *right;
};

typedef struct tree_node tree_node_t;

typedef void (*action_t)(void *node, void *arg);

tree_node_t *readTreeFromFile(const char *name);
tree_node_t *readTree(FILE *in);

tree_node_t *createNode(int num);
void printTree(tree_node_t *tree);
void freeNode(tree_node_t *node, void *arg);
void freeTree(tree_node_t *tree);
void printNode(tree_node_t *node, void *arg);

tree_node_t *bTreeInsert(tree_node_t *tree, tree_node_t *node);
void inTreeLook(tree_node_t *tree, action_t visit, void *arg);

tree_node_t *bTreeRemove(tree_node_t *tree, int value);
tree_node_t *bTreeSearch(tree_node_t *tree, int value);



#endif  // __TREE_H__