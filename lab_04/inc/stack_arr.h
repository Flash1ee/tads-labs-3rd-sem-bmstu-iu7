#ifndef _STACK_ARR_H_
#define _STACK_ARR_H_


typedef struct
{
    char **data;
    int top;
    int tail;
    int capacity;
} stack_arr_t;


stack_arr_t *create_s_arr();
int free_s_arr(stack_arr_t **arr);
int clean_s_arr(stack_arr_t *arr);
int push_s_arr(stack_arr_t **arr, char *str);
int pop_s_arr(stack_arr_t *arr);
int peek_s_arr(stack_arr_t *arr);
int print_s_arr(stack_arr_t *arr);



#endif  // _STACK_ARR_H_