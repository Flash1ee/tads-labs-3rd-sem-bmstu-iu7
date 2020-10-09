#ifndef _IO_H_
#define _IO_H_

#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "err.h"

#define COUNT_MODE 4
typedef enum
{
    EXIT,
    LOAD_FILE,
    ADD_STRUCT,
    DEL_STRUCT,
    SORT_KEYS,
    SORT_TABLE,
    SORT_KEYS_QS,
    SORT_TABLE_QS,
    PRINT_TABLE,
    PRINT_KEYS,
    LIST_MUSIC_FOR_KIDS
} MODE;
/**
*@brief Разбор строки, считанной fgets
*@param num указатель на число-результат
*@param string строка, которая разбирается
*@return int Код ошибки
*
 */
int input_num(int *num, char string[]);
int validation_mode(int8_t mode);
void menu();

/**
*@brief Считывание структур из файла в массив
*
*@param f Файловая переменная
*@param src Массив структур
*@param cnt Количество считываемых структур
*@return int Кош ошибки
 */
int input(FILE *f, theatre_t src[], size_t cnt);
/**
*@brief Вывод информации из массива
*
*@param src Массив структур
*@param cnt Длинна массива
*@param pos Позиция выводимой структуры, pos == cnt вывести все.
*@return int Код ошибки
 */
int output(theatre_t src[], size_t cnt, size_t pos);
/**
*@brief Вывод структуры по позиции
*
*@param src массив структур
*@param pos позиция
*@return int код ошибки
*@warning корректность позиции на вызывающей стороне
 */
void print_pos(theatre_t src[], size_t pos);
/**
*@brief Вывод списка всех
музыкальных спектаклей для детей возраста age с
продолжительностью меньше time_lim
*@param age Возраст ребенка
*@param time_lim Верхняя граница времени продолжительности спектакля
*@return int Код ошибки
 */
int print_res(theatre_t src[], int8_t age, int8_t time_lim);


void print_key_table(theatre_key_t res[], size_t len);

#endif //_IO_H_