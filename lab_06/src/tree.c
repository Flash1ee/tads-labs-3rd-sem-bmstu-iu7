#include <assert.h>

#include "tree.h"

tree_node_t *readTreeFromFile(const char *name)
{
    if (!name)
    {
        return NULL;
    }
    DBG_PRINT("filename = %s\n", name);
    FILE *in = fopen(name, "r");
    if (!in)
    {
        DBG_PRINT("%s", "OPEN FAILED\n");

        return NULL;
    }

    tree_node_t *tree = readTree(in);
    DBG_PRINT("%s", "SUCCESS READ\n");
    fclose(in);
    return tree;
}
tree_node_t *readTree(FILE *in)
{
    if (!in)
    {
        return NULL;
    }
    int temp;
    tree_node_t *root = NULL;
    tree_node_t *node = NULL;
    int i = 0;
    while (fscanf(in, "%d", &temp) == 1)
    {
        i++;
        DBG_PRINT("%s\n", "READ NUM");
        node = createNode(temp);
        if (!node)
        {
            freeTree(root);
            return NULL;
        }
        root = bTreeInsert(root, node);
    }
    if (ferror(in) || !feof(in))
    {
        freeTree(root);
        return NULL;
    }
    DBG_PRINT("Считано %d\n", i);
    return root;
}

tree_node_t *createNode(int num) 
{
    tree_node_t *node = NULL;
    node = calloc(1, sizeof(tree_node_t));
    if (!node) 
    {
        return NULL;
    }
    node->data = num;

    return node;
}
void printTree(tree_node_t *tree)
{
    inTreeLook(tree, printNode, NULL);
}
void printNode(tree_node_t *node, void *arg)
{
    printf("%d\n", node->data);
}
void freeNode(tree_node_t *node, void *arg)
{
    free(node);
}
void freeTree(tree_node_t *tree)
{
    inTreeLook(tree, freeNode, NULL);
}
static int i = 0;
void inTreeLook(tree_node_t *tree, action_t visit, void *arg)
{
    i++;
    if (tree == NULL)
    {
        return;
    }
    inTreeLook(tree->left, visit, arg);
    visit(tree, arg);
    inTreeLook(tree->right, visit, arg);
}
tree_node_t *bTreeInsert(tree_node_t *tree, tree_node_t *node)
{
    if (!tree)
    {
        return node;
    }
    if (!node)
    {
        return node;
    }
    if (node->data == tree->data) {
        return NULL;
    }
    if (node->data > tree->data)
    {
        tree->right = bTreeInsert(tree->right, node);
    }
    else
    {
        tree->left = bTreeInsert(tree->left, node);
    }
    return tree;
}
tree_node_t *bTreeSearch(tree_node_t *tree, int value) {
    if (!tree) {
        return NULL;
    }
    if (value == tree->data) {
        return tree;
    }
    if (value < tree->data) {
        return bTreeSearch(tree->left, value);
    }
    if (value > tree->data) {
        return bTreeSearch(tree->right, value);
    }
}
tree_node_t *bTreeSearchMin(tree_node_t *tree) {
    if (tree->left) {
        return bTreeSearchMin(tree->left);
    }
    return tree;
}
tree_node_t *bTreeRemoveMin(tree_node_t* tree) // удаление узла с минимальным ключом из дерева p
{
	if (!tree->left)
		return tree->right;
	tree->left = bTreeRemoveMin(tree->left);
	return tree;
}
tree_node_t *bTreeRemove(tree_node_t *tree, int value) {
    if (!tree) {
        return tree;
    }
    if (value < tree->data) {
        tree->left = bTreeRemove(tree->left, value);
    }
    else if (value > tree->data) {
        tree->right = bTreeRemove(tree->right, value);
    }
    else {
        tree_node_t *l = tree->left;
        tree_node_t *r = tree->right;
        if (!l && !r) {
            free(tree);
            return NULL;
        }
        if (!r) {
            free(tree);
            return l;
        }
        else {
            tree_node_t *min = bTreeSearchMin(tree->right);
            min->right = bTreeRemoveMin(r);
            min->left = l;
            return min;
        }
        return tree;
    }
    return tree;
}