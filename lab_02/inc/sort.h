#ifndef _SORT_H_
#define _SORT_H_

#include <stdlib.h>
#include "types.h"
/**
*@brief Функция сортровки вставками массива структур по минимальной стоимости билета - поле min_price
*
*@param src Массив структур
*@param len Длина массива
*@param cmp_by_min_price Указатель на функцию сортировки структур по ключу min_price
*@return int Код ошибки
 */
int in_sort(theatre_t *src, size_t len, int (*cmp_by_min_price) (theatre_t a, theatre_t b));

#endif //_SORT_H_
