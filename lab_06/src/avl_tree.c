#include "avl_tree.h"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

int8_t avlHeight(avl_node_t *node) {
    return node ? node->height : 0;
}

void avlFixHeight(avl_node_t *node) {
    assert(node != NULL);

    int8_t right = avlHeight(node->right);
    int8_t left = avlHeight(node->left);

    node->height = (right > left ? right : left) + 1;
}
int8_t avlHeightFactor(avl_node_t *node) {
    assert(node != NULL);

    return avlHeight(node->right) - avlHeight(node->left);
}
avl_node_t *avlLeftRotate(avl_node_t *node) {
    avl_node_t *new_node = node->right;
    node->right = new_node->left;
    new_node->left = node;
    avlFixHeight(node);
    avlFixHeight(new_node);

    return new_node;
}

avl_node_t *avlRightRotate(avl_node_t *node) {
    avl_node_t *new_node = node->left;
    node->left = new_node->right;
    new_node->right = node;
    avlFixHeight(node);
    avlFixHeight(new_node);

    return new_node;
}
avl_node_t *avlBalance(avl_node_t *node) {
    avlFixHeight(node);
    if (avlHeightFactor(node) >= 2) {
        if (avlHeightFactor(node->right) < 0) {
            node->right = avlRightRotate(node->right);
        }
        return avlLeftRotate(node);
    }
    if (avlHeightFactor(node) <= -2) {
        if (avlHeightFactor(node->left) > 0) {
            node->left = avlLeftRotate(node->left);
        }
        return avlRightRotate(node);
    }

    return node;  // Balance not required
}
avl_node_t *readAvlFromFile(const char *name) {
    if (!name) {
        return NULL;
    }
    FILE *in = fopen(name, "r");
    if (!in) {
        fprintf(stderr, "%s\n", "OPEN FAILED");
        return NULL;
    }
    avl_node_t *tree = avlRead(in);
    fclose(in);
    return tree;
}
avl_node_t *avlRead(FILE *in) {
    if (!in) {
        return NULL;
    }
    int temp;
    avl_node_t *root = NULL;
    int i = 0;
    while (fscanf(in, "%d", &temp) == 1) {
        i++;
        root = avlInsert(root, temp);
    }
    if (ferror(in) || !feof(in))
    {
        freeAvl(root);
        return NULL;
    }
    return root;
}
avl_node_t *avlInsert(avl_node_t *node, int value) {
    if (!node) {
        node = calloc(1, sizeof(avl_node_t));
        assert(node != NULL);
        node->data = value;
        return node;
    }
    if (node->data == value) {
        return NULL;
    }
    if (value < node->data) {
        node->left = avlInsert(node->left, value);
    } else {
        node->right = avlInsert(node->right, value);
    }
    return avlBalance(node);
}

avl_node_t *avlSearch(avl_node_t *node, int value) {
    if (!node) {
        return NULL;
    }
    if (value == node->data) {
        return node;
    } else if (value > node->data) {
        return avlSearch(node->right, value);
    } else {
        return avlSearch(node->left, value);
    }
}
avl_node_t *avlFindMin(avl_node_t *node) {
    if (node->left) {
        return avlFindMin(node->left);
    }
    return node;
}
avl_node_t *avlRemoveMin(avl_node_t *node) {
    if (node->left == NULL)
        return node->right;
    node->left = avlRemoveMin(node->left);

    return avlBalance(node);
}
avl_node_t *avlRemove(avl_node_t *node, int value) {
    if (!node) {
        return NULL;
    }
    if (value > node->data) {
        node->right = avlRemove(node->right, value);
    } else if (value < node->data) {
        node->left = avlRemove(node->left, value);
    } else {
        avl_node_t *del_l = node->left;
        avl_node_t *del_r = node->right;
        free(node);
        if (!del_r && !del_l) {
            return NULL;
        }
        if (!del_r) {
            return del_l;
        }
        avl_node_t *min = avlFindMin(del_r);
        min->right = avlRemoveMin(del_r);
        min->left = del_l;

        return avlBalance(min);
    }
    return avlBalance(node);
}

void avlInOrder(avl_node_t *node, void (*visit)(avl_node_t *, void *), void *arg) {
    if (node == NULL) {
        return;
    }
    avlInOrder(node->left, visit, arg);
    visit(node, arg);
    avlInOrder(node->right, visit, arg);
}

void *avlPrintNode(avl_node_t *node, void *arg) {
    printf("%d\n", node->data);
}
void freeAvlNode(avl_node_t *node, void *arg) {
    free(node);
}
void freeAvl(avl_node_t *tree) {
    avlInOrder(tree, freeAvlNode, NULL);
}
