#ifndef _UTILS_H_
#define _UTILS_H_

#define LIST 0
#define ARRAY 1
#define ASCII_START 97
#define ASCII_END 122
#define MAX_STR 128

#include "stack.h"
#include "stack_arr.h"
typedef enum {
    EXIT,
    ADD_L,
    ADD_A,
    DEL_L,
    DEL_A,
    CHECK_TOP_L,
    CHECK_TOP_A,
    PRINT_L,
    PRINT_A,
    CLEAN_L,
    CLEAN_A,
    ADDR_L,
    RAND_L,
    RAND_A,
    DEL_N_L,
    DEL_N_A,
    COUNT
} MODE;

typedef enum {
    EXIT_M,
    MODE_ER,
    READ_ER,
    ALLOC_ER,
    RAND_ER,
    EMPTY_ER,
    FULL_ER,
    ARG_ER
} ERROR;

int valid_mode(int mode);
size_t generate_random_len();
int rand_push(stack_arr_t **arr, free_addr_t **addresses, int flag);
int multi_pop(stack_arr_t *arr, free_addr_t *arr_free_addr, int flag);

void menu();


#endif  // _UTILS_H_