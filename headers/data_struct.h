#ifndef DATA_STRUCT_H
#define DATA_STRUCT_H


#define MAX_MY_FLOAT_T_LEN 40
#define MANTISSA_LEN 32
#define ORDER_LEN 7

typedef struct {
    char num_sign;
    char mantissa[MANTISSA_LEN];
    char order[ORDER_LEN];
    int dot_pos;
    int exp_pos;
    int order_int;
} my_float_t;

typedef char pre_num[MAX_MY_FLOAT_T_LEN];

#endif //DATA_STRUCT_H