#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "io.h"
#include "utils.h"


#define MAX 1000

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
        if (!fgets(buf, NUM + 2, stdin) || (input_num(&mode, buf)))
        {
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
                char tmp[NAME];
                printf("Если вы хотите добавить запись в массив, наберите 0\n");
                if (!fgets(tmp, NAME, stdin))
                {
                    printf("Вы ничего не ввели...\n");
                    break;
                }
                tmp[strlen(tmp) - 1] = '\0';
                if (!strcmp(tmp, "0"))
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
                else
                {
                    printf("Неверный ключ\n");
                    break;
                }
            }
            case DEL_STRUCT:
            printf("Введите id записи таблицы для удаления");
            int id = -1;
            if (!fgets(buf, IND, stdin) || input_num((int *)&id , buf))
            {
                printf("Введена некорректная позиция\nПереводим в меню\n");
                break;
            }
            if (!arr_len)
            {
                printf("Массив пуст\nПереводим в меню\n");
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
            rc = bubble_sort_keys(arr_keys, arr_len);
            if (rc)
            {
                printf("Ошибка сортировки\n");
                return rc;
            }
            break;
            case SORT_TABLE:
            printf("Сортировка таблицы без сортировки ключей за О(n^2)\n");
            rc = bubble_sort(arr, arr_len);
             if (rc)
            {
                printf("Ошибка сортировки\n");
                return rc;
            }
            break;
            case PRINT_KEYS:
            print_key_table(arr_keys, arr_len);
            break;
            case PRINT_TABLE:
            output(arr, arr_len, arr_len);
            case LIST_MUSIC_FOR_KIDS:
                break;
            default:
                printf("Такого режима нет в программе, повторите попытку...\n\n\n");
                continue;
            }
        }
    }
    return EXIT_SUCCESS;
}