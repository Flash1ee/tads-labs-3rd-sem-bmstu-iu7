#ifndef PARSE_H
#define PARSE_H

#include "data_struct.h"
#include <string.h>
#include <ctype.h>

#define MINUS '-'
#define PLUS '+'

int parse_string(pre_num num, my_float_t *res);
int parse_sign(pre_num num, my_float_t *res);
int parse_mantissa(pre_num num, my_float_t *res);
int parse_exp(pre_num num, my_float_t *res);
int parse_order(pre_num num, my_float_t *res);
void correct_order(my_float_t *res);
void del_zeros(my_float_t *res);


#endif //PARSE_H