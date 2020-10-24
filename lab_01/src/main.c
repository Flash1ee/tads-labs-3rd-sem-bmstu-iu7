#include <stdio.h>
#include "../inc/long_nums.h"

void err_output(int rc)
{
    switch (rc)
    {
        case INPUT_ERROR:
            fprintf(stdout, "Ошибка ввода\n");
            break;
        case EXPONENT_ERROR:
            fprintf(stdout, "Ошибка переполнения порядка\n");
            break;
        case ZERO_DIVISION_ERR:
            fprintf(stdout, "Деление на ноль\n");
            break;
    }
}
/**
 * @brief 
 * 
 * @return int error code
 */
int main()
{
    int err = 0;
    char number_buffer[255] = { 0 };
    printf("                      123456789012345678901234567890e99999\n");
    printf("Введите вещественное: ");
    scanf("%s", number_buffer);

    long_real_t number_1 = { 0 };
    long_real_t number_2 = { 0 };

    err = input_long_real(number_buffer, &number_1);
    
    if (!err)
    {
        printf("                      123456789012345678901234567890e99999\n");
        printf("Введите вещественное: ");
        scanf("%s", number_buffer);
        err = input_long_real(number_buffer, &number_2);
    }
    
    long_real_t result = { 0 };
    
    if (!err)
        err = div_long_real(&number_1, &number_2, &result);

    if (!err)
    {
        printf("Результат деления: ");
        err = fprint_long_real(&result, stdout);
        printf("\n");
    }    

    if (err)
        err_output(err);

    return err;
}
