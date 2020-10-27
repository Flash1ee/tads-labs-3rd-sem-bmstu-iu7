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
int pop(free_addr_t *list_addr)
{
    if (top == NULL || !list_addr)
    {
        return EMPTY;
    }
    node_t *tmp = top;
    top = top->next;
    // printf("Удалена строка %s\n", tmp->data);
    free(tmp->data);
    if (list_addr->len == list_addr->capacity)
    {
        node_t **temp_addr = realloc(list_addr->free_ptr, (list_addr->capacity * COEFFICIENT) * sizeof(node_t *));
        if (!temp_addr)
        {
            free(tmp);
            return ALLOCATION_ERR;
        }
        list_addr->free_ptr = temp_addr;
        list_addr->capacity *= COEFFICIENT;
    }
    tmp->next = NULL;
    list_addr->free_ptr[list_addr->len++] = tmp;
    return EXIT_SUCCESS;
}
void print_free_addr(free_addr_t *list_addr)
{
    printf("Список свободных адресов: [");
    if (!list_addr || !list_addr->len)
    {
        printf("свободных адресов нет ]\n");
    }
    else
    {
        for (size_t i = 0; i < list_addr->len; i++)
        {
            printf("%p ", (void *)list_addr->free_ptr[i]);
        }
        printf("]\n");
    }
}
int push(char *data, free_addr_t *list_addr)
{
    node_t *new_node = NULL;
    if (list_addr->len)
    {
        new_node = list_addr->free_ptr[list_addr->len - 1];
        new_node->data = strdup(data);
        if (!new_node->data)
        {
            free(new_node);
            return ALLOCATION_ERR;
        }
        list_addr->len--;
        list_addr->free_ptr[list_addr->len] = NULL;
    }
    else
    {
        new_node = create_node(data);
        if (!new_node)
        {
            return ALLOCATION_ERR;
        }
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
free_addr_t *creatre_free_addr_arr()
{
    free_addr_t *arr = malloc(sizeof(free_addr_t));
    if (!arr)
    {
        return NULL;
    }
    node_t **tmp = NULL;
    tmp = calloc(INIT_SIZE, sizeof(node_t *));
    if (!tmp)
    {
        free(arr);
        return NULL;
    }
    arr->free_ptr = tmp;
    arr->len = 0;
    arr->capacity = INIT_SIZE;
    return arr;
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
int clean_stack_list(free_addr_t *list_addr)
{
    if (!top)
    {
        return EMPTY;
    }
    int rc = 0;
    while (!rc)
    {
        rc = pop(list_addr);
        if (rc)
        {
            return ALLOCATION_ERR;
        }
    }
    return EXIT_SUCCESS;
}
void clean_free_addr(free_addr_t *list_addr)
{
    for (size_t i = 0; i < list_addr->len; i++)
    {
        free(list_addr->free_ptr[i]);
    }
    free(list_addr->free_ptr);
    list_addr->len = 0;
    list_addr->capacity = 0;
    free(list_addr);
}