#include <stdio.h>
#include <stdlib.h>

#include "../inc/tools.h"
#include "../inc/data_struct.h"
#include "../inc/parse.h"


int main()
{
    my_float_t num_frst, num_sec;
    pre_num frst, sec;

    num_frst.num_sign = NO;
    num_sec.num_sign = NO;

    num_frst.exp_pos = NO;
    num_sec.exp_pos = NO;

    num_frst.dot_pos = NO;
    num_sec.dot_pos = NO;

    num_frst.ord_sign = NO;
    num_sec.ord_sign = NO;
    num_frst.order_int = 0;
    num_sec.order_int = 0;


    int rc = EXIT_SUCCESS;

    if (input(frst, sec) != EXIT_SUCCESS)
    {
        printf("Ошибка ввода\n");
        return READ_ERR;
    }

    if ((rc = parse_string((char*)frst, &num_frst)) != EXIT_SUCCESS)
    {
        printf("Ошибка считывания, повторите попытку в соответствии с шаблоном\n");
        return rc;
    }
    if ((rc = parse_string((char*)sec, &num_sec)) != EXIT_SUCCESS)
    {
        printf("Ошибка считывания, повторите попытку в соответствии с шаблоном\n");
        return rc;
    }
    del_zeros(&num_frst);
    del_zeros(&num_sec);


    printf("знак = %c\nмантисса = %s\ndot_pos = %d\n порядок = %ld\n mantissa_len = %zu", num_frst.num_sign,num_frst.mantissa, num_frst.dot_pos,num_frst.order_int,strlen(num_frst.mantissa));
    printf("знак = %c\nмантисса = %s\ndot_pos = %d\n порядок = %ld\n mantissa_len = %zu", num_sec.num_sign,num_sec.mantissa, num_sec.dot_pos,num_sec.order_int,strlen(num_sec.mantissa));

    return EXIT_SUCCESS;
}