#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "types.h"
#include "io.h"
#include "utils.h"

#define MAX 10000

int main()
{
    int state = 1;
    int rc = 0;
    int mode;
    char buf[NUM + 2];

    char filename[NAME + 3];
    FILE *in = NULL;
    theatre_t arr[MAX];
    theatre_key_t arr_keys[MAX];
    size_t arr_len = 0;
    size_t count = 0;
    size_t id_count = 0;

    while (state)
    {
        menu();
        // if (scanf("%d", &mode) != 1)
        // {
        //     char c;
        //     while (c != '\n') {
        //         if (scanf("%c", &c) != 1)
        //         printf("Ошибка чтения, повторите попытку\n");
        //         break;
        //     }
        //     printf("Ошибка чтения, повторите попытку\n");
        //     continue;
        // }
        if (!fgets(buf, NUM + 2, stdin) || !strlen(buf) || (input_num(&mode, buf)))
        {
            // int c;
            // while ((c = getchar()) != '\n' && c != EOF) {}
            if (!strlen(buf))
            {
                printf("Спасибо за использования, до скорых встреч.\n");
                break;
            }
            printf("Ошибка чтения, повторите попытку\n");
            continue;
        }
        else
        {
            switch (mode)
            {
            case EXIT:
                printf("Спасибо за использования, до скорых встреч.\n");
                state = 0;
                break;
            case LOAD_FILE:
                printf("Выбран режим ввода данных из файла\n");
                printf("Введите имя файла (Не более 32 символов): ");
                if (!fgets(filename, NAME + 3, stdin) || (strlen(filename) - 1) > NAME)
                {
                    printf("Длинное имя файла\nПереводим обратно в меню.");
                    break;
                }

                filename[strlen(filename) - 1] = '\0';
                in = fopen(filename, "r");
                if (!in)
                {
                    printf("Не удалось открыть файл\n");
                    break;
                }
                rc = cnt_structs(in, &count);
                if (rc)
                {
                    fclose(in);
                    printf("Ошибка подсчёта структур в файле\n");
                    return rc;
                }
                if (count + arr_len > MAX)
                {
                    fclose(in);
                    printf("Невозможно записать в таблицу данные,размер превышает MAX\n");
                    break;
                }
                fseek(in, 0, SEEK_SET);
                rc = read(arr, arr_len, in, count, &id_count);
                if (rc)
                {
                    fclose(in);
                    printf("Ошибка считывания файла\n");
                    return rc;
                }
                if (fill_arr_keys(arr, arr_keys, arr_len, count))
                {
                    fclose(in);
                    printf("Ошибка считывания файла\n");
                    return rc;
                }
                arr_len += count;
                count = 0;
                printf("Данные файла записаны в массив\n");
                fclose(in);
                break;
            case ADD_STRUCT:
            {
                printf("Введите количество добавляемых записей\n");
                size_t cnt_add = 0;

                if (!(fgets(buf, NUM + 2, stdin)) || input_num((int *)&cnt_add, buf))
                {
                    printf("Вы ничего не ввели...\n");
                    break;
                }
                memset(buf, 0, NUM + 2);
                if (cnt_add + arr_len > MAX)
                {
                    printf("Невозможно записать в таблицу данные,размер превышает MAX\n");
                    break;
                }
                rc = add(arr, &arr_len, cnt_add, arr_keys, &id_count);
                if (rc)
                {
                    printf("Ошибка добавления\n");
                    return rc;
                }
                printf("Вы успешно добавили %zu структур\n", cnt_add);
                break;
            }
            case DEL_STRUCT:
                printf("Введите id записи таблицы для удаления");
                int id = -1;
                if (!fgets(buf, IND, stdin) || input_num((int *)&id, buf))
                {
                    printf("Введена некорректная позиция\nПереводим в меню\n");
                    break;
                }
                if (!arr_len)
                {
                    printf("Таблица пуста\n");
                    break;
                }
                rc = del(arr, &arr_len, arr_keys, id);
                if (rc)
                {
                    if (rc == ID_NOT_FOUND)
                    {
                        printf("Введённый id не найден в таблице\n");
                        break;
                    }
                    printf("Некорректные данные для удаления\nПереводим в меню\n");
                    break;
                }
                printf("Удаление успешно выполнено\n");
                break;
            case SORT_KEYS:
                printf("Сортировка массива ключей без сортировки исходной таблицы за О(n^2)\n");
                if (!arr_len)
                {
                    printf("Массив пуст\n");
                    break;
                }
                rc = bubble_sort_keys(arr_keys, arr_len);
                if (rc)
                {
                    printf("Ошибка сортировки\n");
                    return rc;
                }
                break;
            case SORT_TABLE:
                if (!arr_len)
                {
                    printf("Массив пуст\n");
                    break;
                }
                printf("Сортировка таблицы без сортировки ключей за О(n^2)\n");
                rc = bubble_sort(arr, arr_len);
                if (rc)
                {
                    printf("Ошибка сортировки\n");
                    return rc;
                }
                break;
            case SORT_KEYS_QS:
                if (!arr_len)
                {
                    printf("Массив пуст\n");
                    break;
                }
                printf("Сортировка массива ключей без сортировки исходной таблицы за О(N*log(N))\n");
                quick_sort_keys(arr_keys, 0, arr_len - 1);
                break;
            case SORT_TABLE_QS:
                if (!arr_len)
                {
                    printf("Массив пуст\n");
                    break;
                }
                printf("Сортировка таблицы без сортировки ключей за О(N*log(N)\n");
                quick_sort_table(arr, 0, arr_len - 1);
                break;
            case PRINT_KEYS:
                if (!arr_len)
                {
                    printf("Массив пуст\n");
                    break;
                }
                print_key_table(arr_keys, arr_len);
                break;
            case PRINT_TABLE:
                if (!arr_len)
                {
                    printf("Массив пуст\n");
                    break;
                }
                output(arr, arr_len, arr_len);
                break;
            case LIST_MUSIC_FOR_KIDS:
            {
                if (!arr_len)
                {
                    printf("Массив пуст\n");
                    break;
                }
                char tmp[NUM + 3] = {0};
                int age;
                int time = 0;
                printf("Введите возраст детей: ");
                if (!fgets(tmp, NUM + 3, stdin) || sscanf(tmp, "%d", &age) != 1 || age < 0)
                {
                    printf("Неверно указан возраст\nПереводим в меню\n");
                    break;
                }
                memset(tmp, 0, sizeof(tmp));
                printf("Введите максимальную продолжительность спектакля: ");
                if (!fgets(tmp, NUM + 3, stdin) || sscanf(tmp, "%d", &time) != 1 || time < 0)
                {
                    printf("Неверно указано время\nПереводим в меню\n");
                    break;
                }
                if (get_music_list(arr, arr_len, age, time))
                {
                    printf("В таблице совпадения не найдены\n");
                    break;
                }
                break;
            }
            case SHUFFLE:
            {
                if (!arr_len)
                {
                    printf("Массив пуст\n");
                    break;
                }
                printf("Функция для перемешивания массива\n");
                printf("Введите 1, чтобы перемешать таблицу\nВведите 2, чтобы перемешать таблицу ключей\n");
                int choice = 0;
                char tmp[NUM];
                if (!fgets(tmp, NUM, stdin) || input_num(&choice, tmp) || (choice != 1 && choice != 2))
                {
                    printf("Число введено неверно\n");
                    break;
                }
                if (choice == 1)
                {
                    shuffle(arr, arr_len, sizeof(theatre_t));
                }
                else if (choice == 2)
                {
                    shuffle(arr_keys, arr_len, sizeof(theatre_key_t));
                }
                break;
            }
            case TIME_SLOU:
            {
                if (!arr_len)
                {
                    printf("Массив пуст\n");
                    break;
                }
                struct timeval tv_start, tv_stop;
                int64_t time_k, time_t;

                time_k = time_t = 0;

                theatre_key_t tmp_keys[arr_len];
                theatre_t tmp_table[arr_len];

                for (size_t i = 0; i < 10; i++)
                {
                    memcpy(tmp_keys, arr_keys, sizeof(theatre_key_t) * arr_len);
                    gettimeofday(&tv_start, NULL);
                    bubble_sort_keys(tmp_keys, arr_len);
                    gettimeofday(&tv_stop, NULL);
                    time_k += (tv_stop.tv_sec - tv_start.tv_sec) * 1000000LL + (tv_stop.tv_usec - tv_start.tv_usec);
                }
                memset(&tv_start, 0, sizeof(tv_start));
                memset(&tv_stop, 0, sizeof(tv_stop));


                for (size_t i = 0; i < 10; i++)
                {
                    memcpy(tmp_table, arr, sizeof(theatre_t) * arr_len);
                    gettimeofday(&tv_start, NULL);
                    bubble_sort(tmp_table, arr_len);
                    gettimeofday(&tv_stop, NULL);
                    time_t += (tv_stop.tv_sec - tv_start.tv_sec) * 1000000LL + (tv_stop.tv_usec - tv_start.tv_usec);
                }
                double res_k, res_t;
                res_k = time_k / 10.;
                res_t = time_t / 10.;
                time_k = time_t = 0;

                printf("Сортировка ключей О(n^2) выполнена за %lf\n", res_k);
                printf("Сортировка таблицы О(n^2) выполнена за %lf\n", res_t);
                printf("Размер таблицы ключей = %zu, таблицы = %zu\n", sizeof(tmp_keys), sizeof(tmp_table));
                break;
            }
            case TIME_FAST:
            {
                if (!arr_len)
                {
                    printf("Массив пуст\n");
                    break;
                }
                struct timeval tv_start, tv_stop;
                int64_t time_k, time_t;
                time_k = time_t = 0;

                theatre_key_t tmp_keys[arr_len];
                theatre_t tmp_table[arr_len];
                
                for (size_t i = 0; i < 10; i++)
                { 
                    memcpy(tmp_keys, arr_keys, sizeof(theatre_key_t) * arr_len);
                    gettimeofday(&tv_start, NULL);
                    quick_sort_keys(tmp_keys, 0, arr_len - 1);
                    gettimeofday(&tv_stop, NULL);
                    time_k += (tv_stop.tv_sec - tv_start.tv_sec) * 1000000LL + (tv_stop.tv_usec - tv_start.tv_usec);
                }
                
                for (size_t i = 0; i < 10; i++)
                {
                    memcpy(tmp_table, arr, sizeof(theatre_t) * arr_len);
                    gettimeofday(&tv_start, NULL);
                    quick_sort_table(tmp_table, 0, arr_len - 1);
                    gettimeofday(&tv_stop, NULL);
                    time_t += (tv_stop.tv_sec - tv_start.tv_sec) * 1000000LL + (tv_stop.tv_usec - tv_start.tv_usec);
                }
                double res_k, res_t;
                res_k = time_k / 10.;
                res_t = time_t / 10.;
                time_k = time_t = 0;
                printf("Сортировка ключей О(n*(log(N)) выполнена за %lf\n", res_k);
                printf("Сортировка таблицы О(n*(log(N)) выполнена за %lf\n", res_t);
                printf("Размер таблицы ключей = %zu, таблицы = %zu\n", sizeof(tmp_keys), sizeof(tmp_table));
                break;
            }
            break;
            default:
                printf("Такого режима нет в программе, повторите попытку...\n\n\n");
                continue;
            }
        }
    }
    return EXIT_SUCCESS;
}