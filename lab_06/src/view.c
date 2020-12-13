#include <assert.h>
#include "view.h"
#include "avl_tree.h"

static int cnt = 0;

void treeExportToDot(FILE *f, const char *tree_name, void *tree, int flag)
{
    assert(f && tree && tree_name);
    fprintf(f, "digraph %s {\n", tree_name);

    switch (flag) {
        case BST:
            inTreeLook(tree, nodeToDot, f);
            break;
        case AVL:
            avlInOrder(tree, nodeToDot, f);
            break;
    }
    fprintf(f, "}\n");
    cnt = 0;
}
void nodeToDot(void *node, void *param, int flag)
{
    tree_node_t *node_n = (tree_node_t *) node;
    if (flag == AVL) {
        avl_node_t *node_n = (avl_node_t *) node;
    }
    FILE *out = param;
    if (node_n->left)
    {
        fprintf(out, "%d -> %d;\n", node_n->data, node_n->left->data);
    }
    else
    {
        fprintf(out, "null%d [shape=point];\n", cnt);
        fprintf(out, "%d -> null%d;\n", node_n->data, cnt++);
    }
    
    if (node_n->right)
    {
        fprintf(out, "%d -> %d;\n", node_n->data, node_n->right->data);
    }
    else
    {
        fprintf(out, "null%d [shape=point];\n", cnt);
        fprintf(out, "%d -> null%d;\n", node_n->data, cnt++);
        
    }
    
}