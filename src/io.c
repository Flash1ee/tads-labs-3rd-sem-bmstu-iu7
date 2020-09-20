#include <string.h>
#include <stdio.h>


#include "../inc/tools.h"
#include "../inc/data_struct.h"

void hello()
{
    ;
}

int input(pre_num a, pre_num b)
{
    printf("Введите первое число в заданном формате: ");
    if ((scanf("%" MAX "s",a)!= 1))
        return READ_ERR;
    printf("\n");

    printf("Введите второе число в заданном формате: ");
    if ((scanf("%" MAX "s",b)!= 1))
        return READ_ERR;
    

    return EXIT_SUCCESS;
}
