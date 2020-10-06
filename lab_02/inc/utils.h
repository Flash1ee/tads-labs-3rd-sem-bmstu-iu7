#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "err.h"
#include "io.h"


#define SIZE_INC 20
/**
*@brief Функция для заполнения массива указателей на структуры
*
*@param src Массив структур
*@param dst Массив указателей на структуры
*@param cnt Количество элементов
*@return int Код ошибки
 */
int fill_arr_p(theatre src[], theatre *dst[], size_t cnt);
/**
*@brief Удаление структуры из массива по позиции ind
*
*@param src Массив структур
*@param dst Массив указателей на структуры
*@param ind Позиция удаляемой структуры
*@return int Код ошибки
 */
int del(theatre src[], theatre *dst[]);
/**
*@brief Добавление структуры в массив
*
*@param src Массив структур
*@param new Адрес указателя на новый массив в случае расширения
*@param len_src Длина массива
*@param dst Массив указателей на структуры
*@return int Код ошибки
 */
int add(theatre src[], theatre **new, size_t *len_src, size_t *capacity_src, size_t cnt);


#endif // _UTILS_H_