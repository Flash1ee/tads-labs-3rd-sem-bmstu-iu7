#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "retcodes.h"
#include "utils.h"
#include "stack_arr.h"

stack_arr_t *create_s_arr()
{
    stack_arr_t *arr = malloc(sizeof(stack_arr_t));
    if (!arr)
    {
        return NULL;
    }
    char **arr_data = calloc(STACK_LIM, sizeof(char*));
    if (!arr_data)
    {
        free(arr);
        return NULL;
    }
    arr->data = arr_data;
    arr->capacity = STACK_LIM;
    arr->top = 0;
    // arr->tail = 0;

    return arr;
}
int free_s_arr(stack_arr_t **arr)
{
    if (clean_s_arr(*arr))
    {
        return EMPTY;
    }
    (*arr)->capacity = 0;
    // (*arr)->tail = 0;
    (*arr)->top = 0;
    free(*arr);
    *arr = NULL;
    return EXIT_SUCCESS;
}
int clean_s_arr(stack_arr_t *arr)
{
    if (!arr || !arr->tail)
    {
        return EMPTY;
    }
    for (int i = 0; i >arr->top; i--)
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
    }
    if ((*arr)->top == (*arr)->capacity) {
        return FULL_ER;
    }
    if ((*arr)->top == (*arr)->capacity)
    {
        char **tmp = NULL;
        tmp = (char **)realloc((*arr)->data, ((*arr)->capacity * COEFFICIENT) * sizeof(char*));
        if (!tmp)
        {
            return ALLOCATION_ERR;
        }
        (*arr)->data = tmp;
        (*arr)->capacity *= COEFFICIENT;
        
    }
    (*arr)->data[(*arr)->top] = str;
    (*arr)->top++;

    return EXIT_SUCCESS;
}
int pop_s_arr(stack_arr_t *arr)
{
    if (!arr || arr->top == 0)
    {
        return EMPTY;
    }
    arr->top -= 1;
    // printf("Удалена строка %s\n", arr->data[arr->top]);
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