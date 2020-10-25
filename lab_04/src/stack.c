#include "stack.h"

int peek()
{
    if (top == NULL)
    {
        return EMPTY;
    }
    printf("%s\n", top->data);
    return EXIT_SUCCESS;
}
int pop()
{
    if (top == NULL)
    {
        return EMPTY;
    }
    node_t *tmp = top;
    top = top->next;

    free(tmp->data);
    free(tmp);
    return EXIT_SUCCESS;
}

int push(char *data)
{
    node_t *new_node = create_node(data);
    if (!new_node)
    {
        return ALLOCATION_ERR;
    }
    if (!top)
    {
        top = new_node;
        new_node->next = NULL;
    }
    else
    {
        new_node->next = top;
        top = new_node;
    }
    return EXIT_SUCCESS;
}

node_t *create_node(char *data)
{
    if (!data)
    {
        return NULL;
    }
    node_t *node = malloc(sizeof(node_t));
    if (!node)
    {
        return NULL;
    }
    char *str = strdup(data);
    if (!str)
    {
        free(node);
        return NULL;
    }

    node->data = str;
    return node;
}
int print_all()
{
    if (!top)
    {
        return EMPTY;
    }
    for (node_t *cur = top; cur != NULL; cur = cur->next)
    {
        printf("%s\n", cur->data);
    }
    return EXIT_SUCCESS;
}
int clean_stack_list()
{
    if (!top)
    {
        return EMPTY;
    }
    int rc = 0;
    while (!rc)
    {
        rc = pop();
    }
    return EXIT_SUCCESS;
}
