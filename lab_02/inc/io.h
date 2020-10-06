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
*@brief Подсчёт количества структур в фалйе
*
*@param f Файловая переменная
*@param cnt Указатель на переменную количества подсчитанных структур
*@return int Код ошибки
 */
int cnt_structs(FILE *f, size_t *cnt);
/**
*@brief Считывание структур из файла в массив
*
*@param f Файловая переменная
*@param src Массив структур
*@param cnt Количество считываемых структур
*@return int Кош ошибки
 */
int input(FILE *f, theatre src[], size_t cnt);
/**
*@brief Вывод информации из массива
*
*@param src Массив структур
*@param cnt Длинна массива
*@param pos Позиция выводимой структуры, pos == cnt вывести все.
*@return int Код ошибки
 */
int output(theatre src[], size_t cnt, size_t pos);
/**
*@brief Вывод структуры по позиции
*
*@param src массив структур
*@param pos позиция
*@return int код ошибки
*@warning корректность позиции на вызывающей стороне
 */
void print_pos(theatre src[], size_t pos);
/**
*@brief Функция создания записи театра
*
*@param tmp указатель на запись
*@param stream Указатель на поток ввода
 */
int entry(theatre *tmp, FILE *stream);
/**
*@brief Вывод списка всех
музыкальных спектаклей для детей возраста age с
продолжительностью меньше time_lim
*@param age Возраст ребенка
*@param time_lim Верхняя граница времени продолжительности спектакля
*@return int Код ошибки
 */
int print_res(theatre src[], int8_t age, int8_t time_lim);

int read(theatre src[], size_t len, FILE *in, size_t cnt);

#endif //_IO_H_