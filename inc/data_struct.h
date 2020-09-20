#ifndef DATA_STRUCT_H
#define DATA_STRUCT_H


#define MAX_MY_FLOAT_T_LEN 39 // without \0
#define MAX "40"
#define MANTISSA_LEN 31 //with dot
#define ORDER_LEN 5


#define NO -1


typedef struct {
    char num_sign;
    char mantissa[MANTISSA_LEN];
    char order[ORDER_LEN];
    int dot_pos;
    int exp_pos;
    int ord_sign;
    long order_int;
} my_float_t;

typedef char pre_num[MAX_MY_FLOAT_T_LEN];

#endif //DATA_STRUCT_H