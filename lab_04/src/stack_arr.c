#include "stack_arr.h"

stack_arr_t *create_s_arr()
{
    stack_arr_t *arr = malloc(sizeof(stack_arr_t));
    if (!arr)
    {
        return NULL;
    }
    char **arr_data = calloc(INIT_SIZE, sizeof(char*));
    if (!arr_data)
    {
        free(arr);
        return NULL;
    }
    arr->data = arr_data;
    arr->capacity = INIT_SIZE;
    arr->top = 0;
    return arr;
}
void free_s_arr(stack_arr_t *arr)
{
    clean_s_arr(arr);
    arr->capacity = 0;
    free(arr);
}
int clean_s_arr(stack_arr_t *arr)
{
    if (!arr)
    {
        return EMPTY;
    }
    for (int i = arr->top - 1; i >= 0; i--)
    {
        free(arr->data[i]);
    }
    arr->top = 0;
    return EXIT_SUCCESS;
}
int push_s_arr(stack_arr_t **arr, char *str)
{
    if (!(*arr))
    {
        *arr = create_s_arr();
        if (!(*arr))
        {
            return ALLOCATION_ERR;
        }
        printf("CREATE\n");
        printf("top = %d\n", (*arr)->top);

    }
    if ((*arr)->top == (*arr)->capacity)
    {
        char **tmp = realloc((*arr)->data, (INIT_SIZE / 2) * sizeof(char*));
        if (!tmp)
        {
            return ALLOCATION_ERR;
        }
        (*arr)->data = tmp;
        (*arr)->capacity += INIT_SIZE / 2;
    }
    int ind = (*arr)->top;
    (*arr)->data[ind] = str;
    (*arr)->top +=1;

    return EXIT_SUCCESS;
}
int pop_s_arr(stack_arr_t *arr)
{
    if (!arr || arr->top == 0)
    {
        return EMPTY;
    }
    arr->top -= 1;
    free(arr->data[arr->top]);
    return EXIT_SUCCESS;
}
int peek_s_arr(stack_arr_t *arr)
{
    if (!arr || arr->top == 0)
    {
        return EMPTY;
    }
    printf("%s\n", arr->data[arr->top - 1]);
    return EXIT_SUCCESS;
}
int print_s_arr(stack_arr_t *arr)
{
    if (!arr || arr->top == 0)
    {
        return EMPTY;
    }
    for (int i = arr->top - 1; i >= 0; i--)
    {
        printf("%s\n", arr->data[i]);
    }
    return EXIT_SUCCESS;
}