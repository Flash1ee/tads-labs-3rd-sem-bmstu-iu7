#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "io.h"

void menu()
{
    printf("Приветствуем вас. Это программа для измерения времени сортировки таблицы репертуара театров\n\n");
    printf("Введите 0 для завершения выполнения\n\n\n");
    printf("Введите 1 для загрузки данных из файла\n");
    printf("Введите 2 для добавления записи в таблицу с клавиатуры\n");
    printf("Введите 3 для удаления записи из таблицы\n\n\n");
    printf("Введите 4 для сортировки массива ключей O(n^2)\n");
    printf("Введите 5 для сортировки таблицы O(n^2)\n");
    printf("Введите 6 для сортировки массива ключей O(N*log(N))\n");
    printf("Введите 7 для сортировки таблицы O(N*log(N)\n\n\n");
    printf("Введите 8 для вывода таблицы\n");
    printf("Введите 9 для вывода таблицы ключей\n");
    printf("Введите 10 для вывода списка музыкальных спектаклей для детей указанного возраста\nс ");
    printf("продолжительностью меньше указанной.\n\n\n");
    printf("Введите 11 для перемешивания таблиц\n");
    printf("Введите 12 для сравнения сортировок скорости О(n*log(N))\n");
    printf("Введите 13 для сравнения сортировок скорости О(n^2)\n");

}
int input_num(int *num, char string[])
{
    if (!num || (strlen(string) < 2 && *string == '\n'))
        return 1;
    int res;
    char *key = NULL;

    res = strtol(string, &key, 10);
    if (key != NULL && *key == '\n')
    {
        *num = res;
        return 0;
    }
    return 1;
}
int validation_mode(int8_t mode)
{
    for (size_t i = 0; i < COUNT_MODE; i++)
        if (mode == i)
            return EXIT_SUCCESS;
    return EXIT_FAILURE;
}

int output(theatre_t src[], size_t cnt, size_t pos)
{
    if (!src || (pos > cnt && pos != cnt))
    {
        return PRINT_ERR;
    }
    printf("┃━━━━━┃━━━━┃━━━━━━━━━━━━━━━━━━━━━━━━━━━━┃"
    "━━━━━━━━━━━━━━━━━━━━━━━━━━━━┃━━━━━━━━━━━━━━━━━━━━━━━━━━━━┃"
    "━━━━━━━━┃━━━━━━━━┃━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┃\n");
    printf("┃%5s┃%4s┃%-28s┃%-28s┃%-28s┃%-8s┃%-8s┃%-51s┃\n", "#N","id","Theatre", "Show", "Producer", "Min", "Max", "Variable part");
    printf("┃━━━━━┃━━━━┃━━━━━━━━━━━━━━━━━━━━━━━━━━━━┃━━━━━━━━━━━━━━━━━━━━━━━━━━━━┃━━━━━━━━━━━━━━━━━━━━━━━━━━━━┃"
    "━━━━━━━━┃━━━━━━━━┃━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┃\n");
    if (pos == cnt)
    {
        for (size_t i = 0; i < cnt; i++)
        {
            print_pos(src, i);
        }
    }
    else
    {
        print_pos(src, pos);
    }
     printf("┃━━━━━┃━━━━┃━━━━━━━━━━━━━━━━━━━━━━━━━━━━┃"
    "━━━━━━━━━━━━━━━━━━━━━━━━━━━━┃━━━━━━━━━━━━━━━━━━━━━━━━━━━━┃"
    "━━━━━━━━┃━━━━━━━━┃━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┃\n");
    return EXIT_SUCCESS;
}
void print_pos(theatre_t src[], size_t pos)
{

    theatre_t cur = src[pos];
    printf("┃%5zu┃%4d┃%28s┃%28s┃%28s┃%8d┃%8d┃", pos + 1, cur.id, cur.title, cur.show, cur.producer, cur.min_price, cur.max_price);
    switch (cur.category)
    {
    case KIDS:
        printf("%16d┃%16s┃        ┃        ┃\n", cur.choice.minor.min_age, cur.choice.minor.genre);
        break;
    case ADULTS:
        printf("%16s┃                ┃        ┃        ┃\n", cur.choice.major.genre);
        break;
    case MUSIC:
        printf("%16s┃%16s┃%8d┃%8d┃\n", cur.choice.sound.composer, cur.choice.sound.country, cur.choice.sound.min_age,cur.choice.sound.time);
        break;
    }
}
void print_key_table(theatre_key_t res[], size_t len)
{
    printf("┃━━━━━┃━━━━━━━┃━━━━━━━━┃\n");
    printf("┃%-5s┃%-7s┃%-8s┃\n", "N_key", "N_table", "min");
    printf("┃━━━━━┃━━━━━━━┃━━━━━━━━┃\n");
    for (size_t i = 0; i < len; i++)
    {
        printf("┃%5zu┃%7d┃%8d┃\n", i + 1, res[i].id, res[i].min_price);
    }
}
int get_music_list(theatre_t src[], size_t len, int age, int time_lim)
{
    int rc = 1;
    char key = 0;
    for (size_t i = 0; i < len; i++)
    {
        if (src[i].category == MUSIC)
        {
            if (src[i].choice.sound.time < time_lim && src[i].choice.sound.min_age == age)
            {
                if (!key)
                {
                    printf("┃%32s┃%4s┃%16s┃\n", "Show", "age", "time");
                    key = 1;
                }
                rc = 0;
                printf("┃%32s┃%4"PRId8"┃%16d┃\n", src[i].show, age, src[i].choice.sound.time);
            }
        }
    }
    return rc;
}