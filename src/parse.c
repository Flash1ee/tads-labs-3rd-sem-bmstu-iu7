#include "../inc/tools.h"
#include "../inc/data_struct.h"
#include "../inc/parse.h"

#include <stdio.h>

int parse_string(pre_num num, my_float_t *res)
{
    int rc = 0;
    rc = parse_sign(num, res);
    if (rc != EXIT_SUCCESS)
        return rc;
    printf("sign parse true\n");
    rc = parse_mantissa(num, res);
    if (rc != EXIT_SUCCESS)
        return rc;

    if (res->exp_pos != NO)
    {
        rc = parse_order(num, res);
        if (rc != EXIT_SUCCESS)
            return rc;
    }
    correct_order(res);
    return rc;
}

int parse_sign(pre_num num, my_float_t *res)
{
    if (!num)
        return EXIT_FAILURE;
    if (*num == MINUS)
        res->num_sign = MINUS;
    else if (*num == PLUS)
        res->num_sign = PLUS;
    else if (*num != DOT && !isdigit(*num))
        return SIGN_ERR;
    return EXIT_SUCCESS;
}

int parse_mantissa(pre_num num, my_float_t *res)
{
    size_t i = 0;
    size_t j = 0;
    if (res->num_sign == PLUS || res->num_sign == MINUS)
        i = 1;
    char tmp[MANTISSA_LEN * 2] = {'\0'};

    while (isdigit(num[i]))
        tmp[j++] = num[i++];

    if (num[i] == DOT)
    {
        res->dot_pos = j;
        i++;
    }
    else if (tolower(num[i]) == EXP)
    {
        res->exp_pos = i;
        tmp[j] = '\0';
        if (strlen(tmp) > MANTISSA_LEN - 1)
            return LONG_MANTISSA_ERR;
        else
            strcpy(res->mantissa, tmp);
        return EXIT_SUCCESS;
    }
    else if (num[i] != '\0')
        return READ_ERR;

    while (isdigit(num[i]))
        tmp[j++] = num[i++];

    if (tolower(num[i]) == EXP)
        res->exp_pos = i++;
    else if (num[i] != '\0')
        return SYMBOL_ERR;
    tmp[j] = '\0';

    if (strlen(tmp) > MANTISSA_LEN - 1)
        return LONG_MANTISSA_ERR;
    else
        strcpy(res->mantissa, tmp);
    return EXIT_SUCCESS;
}

int parse_order(pre_num num, my_float_t *res)
{
    size_t i = (res->exp_pos + 1);
    printf("exp_pos = %d\n", i);

    if (num[i] == MINUS)
        res->ord_sign = MINUS;
    else if (num[i] == PLUS)
        res->ord_sign = PLUS;
    else if (isdigit(num[i]))
        res->ord_sign = PLUS;
    else
        return SIGN_ERR;
    if ((isdigit(num[i]) && strlen(num + i) > ORDER_LEN) || 
    (!isdigit(num[i]) && strlen(num + i + 1) > ORDER_LEN))
    {
        printf("i=%zu, strlen(num+i) = %zu\n,", i, strlen(num + i));
        return ORD_ERR;
    }

    char *check = NULL;
    long order = 0;
    order = strtol(&num[i], &check, 10);
    printf("order = %ld\n", order);
    if (!check)
        return ORD_ERR;
    else
        res->order_int = order;
    return EXIT_SUCCESS;
}
void correct_order(my_float_t *res)
{
    if (res->dot_pos != NO && res->mantissa[0] == '0')
        res->order_int -= (strlen(res->mantissa) - res->dot_pos);
    else if (res->dot_pos != NO && res->mantissa[0] > '0' && strlen(res->mantissa) != (size_t)res->dot_pos)
        res->order_int -= res->dot_pos;
}
void del_zeros(my_float_t *res)
{
    size_t cnt = 0;
    if (*(res->mantissa) == '0')
    {
        for (size_t i = 0; res->mantissa[i++] == '0'; cnt++)
            ;
        printf("%zu\n", cnt);
        size_t i = cnt;
        for (; i < strlen(res->mantissa); i++)
            res->mantissa[i - cnt] = res->mantissa[i];
        res->mantissa[i - cnt] = '\0';
    }
}
