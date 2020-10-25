#ifndef _UTILS_H_
#define _UTILS_H_

#include "stack.h"
#include "retcodes.h"

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
    COUNT
} MODE;

typedef enum {
    EXIT_M,
    MODE_ER,
    READ_ER,
    ALLOC_ER,
    EMPTY_ER,
} ERROR;

int valid_mode(int mode);
void menu();


#endif  // _UTILS_H_