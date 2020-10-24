#ifndef TYPES_H_
#define TYPES_H_

#include <inttypes.h>
#define IND 5
/**
*@brief Длина числа
 */
#define NUM 16
/**
*@brief Название театра
 */
#define TITLE 32
/**
*@brief Название представления
 */
#define SHOW 32
/**
*@brief ФИО композитора
 */
#define NAME 32
/**
*@brief Название страны
 */
#define COUNTRY 16
/**
*@brief Размер жанра представления
 */
#define TYPE 16
/**
*@brief Перечисление жанров спектаклей
 */
typedef enum
{
    FAIRY_TALE,
    PIECE,
    DRAMA,
    COMEDY
} GENRE;
/**
*@brief Перечисление контингента театра
 */
typedef enum
{
    KIDS,
    ADULTS,
    MUSIC
} CATEGORY;
/**
*@brief Структура, описывающая спектакли для детей
*@param min_age - Минимальный возраст
*@param type - переменная выбора жанра спектакля 
*@param genre - жанр спектакля
*
 */
typedef struct
{
    int8_t min_age;
    int8_t type;
    char genre[TYPE];
} child;
/**
*@brief Структура, описывающая спектакли для взрослых
*@param type - переменная выбора жанра спектакля 
*@param genre - жанр спектакля
 */
typedef struct
{
    int8_t type;
    char genre[TYPE];
} adult;
/**
*@brief Структура для хранения данных музыкального представления
*@param composer - ФИО композитора длиной NAME
*@param country - страна исполнителя длины COUNTRY
*@param min_age - ограничение по возрасту снизу
*@param time - продолжительность выступления в минутах
 */
typedef struct
{
    char composer[NAME + 3];
    char country[COUNTRY + 3];
    int min_age;
    int time;
} music;
/**
*@brief Структура для описания репертуара театра
*@param title Название театра
*@param show Нвание спектакля
*@param producer ФИО режиссёра
*@param min_price Минимальная цена билета
*@param max_price Максимальная цена билета
*@param category Переменная выбора вариативной части (тип спектакля)
*@param choice Объединение вариативной части - типа спектакля
 */
typedef struct
{
    int id;
    char title[TITLE + 3];
    char show[SHOW + 3];
    char producer[NAME + 3];
    int min_price;
    int max_price;
    int8_t category;
    union
    {
        child minor;
        adult major;
        music sound;
    } choice;
} theatre_t;

typedef struct 
{
    int min_price;
    int id;
} theatre_key_t;


#endif //TYPES_H_