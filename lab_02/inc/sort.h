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
int in_sort(theatre *src, size_t len, int (*cmp_by_min_price) (theatre a, theatre b));
/**
*@brief Сравнение двух структур по полю min_price
*
*@param a Структура theatre
*@param b Структура theatre
*@return 0, если элементы равны;
*           x > 0, если a > b;
*           x < 0, если a < b.
 */
int cmp_by_min_price(theatre a, theatre b);
/**
*@brief Функция пузырьковой сортировки массива структур по минимальной стоимости билета - поле min_price
*
*@param src Массив структур
*@param len Длина массива
*@param cmp_by_min_price Указатель на функцию сортировки структур по ключу min_price
*@return int Код ошибки
 */
int bubble_sort(theatre *src, size_t len, int (*cmp_by_min_price) (theatre a, theatre b));

#endif //_SORT_H_