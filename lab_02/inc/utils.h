#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "err.h"
#include "io.h"

/**
*@brief Удаление структуры из таблицы по позиции ind 
*
*@param src Массив структур
*@param len Указатель на длину массива
*@param dst Массив указателей на структуры
*@param id id записи таблицы
*@return int Код ошибки
 */
int del(theatre_t src[], size_t *len, theatre_key_t dst[], size_t id);
/**
*@brief Добавление структуры в массив
*
*@param src Массив структур
*@param len_src Длина массива
*@param arr_key Массив указателей на структуры
*@param cnt Количество записываемых структур
*@param id_count Указатель на счётчик id элемента таблицы
*@return int Код ошибки
 */
int add(theatre_t src[], size_t *len_src, size_t cnt, theatre_key_t arr_key[], size_t *id_count);
/**
*@brief Функция создания записи театра
*
*@param tmp указатель на запись
*@param stream Указатель на поток ввода
 */
int entry(theatre_t *tmp, FILE *stream);
/**
*@brief Подсчёт количества структур в фалйе
*
*@param f Файловая переменная
*@param cnt Указатель на переменную количества подсчитанных структур
*@return int Код ошибки
 */
int cnt_structs(FILE *f, size_t *cnt);

int read(theatre_t src[], size_t len, FILE *in, size_t cnt, size_t *id_count);
int fill_arr_keys(theatre_t src[], theatre_key_t dst[], size_t len, size_t cnt);

/**
*@brief Функция пузырьковой сортировки массива структур по минимальной стоимости билета - поле min_price
*
*@param src Массив структур
*@param len Длина массива
*@return int Код ошибки
 */
int bubble_sort(theatre_t *src, size_t len);
/**
*@brief Сравнение двух структур по полю min_price
*
*@param a Структура theatre_t
*@param b Структура theatre_t
*@return 0, если элементы равны;
*           x > 0, если a > b;
*           x < 0, если a < b.
 */
int cmp_by_min_price(theatre_t a, theatre_t b);
void swap_table(theatre_t frst[], theatre_t sec[]);
void swap_keys(theatre_key_t frst[], theatre_key_t sec[]);

int bubble_sort_keys(theatre_key_t *keys, size_t len);

#endif // _UTILS_H_