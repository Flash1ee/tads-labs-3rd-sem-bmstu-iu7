#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "io.h"
#include "utils.h"

int main()
{
    int state = 1;
    int rc = 0;
    int mode;
    char buf[NUM + 2];
    theatre *arr = NULL;
    theatre *tmp_realloc = NULL;
    char filename[NAME + 3];

    size_t arr_len = 0;
    size_t arr_capacity = 0;
    size_t count = 0;
    int file_state = 0;

    FILE *in = NULL;

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
            memset(buf, 0, NUM + 2);
            switch (mode)
            {
            case EXIT:
                printf("Спасибо за использования, до скорых встреч.\n");
                state = 0;
                output(arr, arr_len, arr_len);
                if (arr)
                {
                    free(arr);
                }
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
                file_state = 1;
                rc = cnt_structs(in, &count);
                if (rc)
                {
                    fclose(in);
                    file_state = 0;
                    printf("Ошибка подсчёта структур в файле\n");
                    return rc;
                }
                if (!arr)
                {
                    arr = (theatre *)malloc(count * sizeof(theatre));
                    if (!arr)
                    {
                        printf("Ошибка выделения памяти, повторите позже...\n");
                        fclose(in);
                        file_state = 0;
                        break;
                    }
                    arr_capacity += count;
                }
                if (arr_capacity < arr_len + count)
                {
                    tmp_realloc = (theatre *)realloc(arr, count + SIZE_INC);
                    if (!tmp_realloc)
                    {
                        printf("Ошибка выделения памяти, повторите позже...\n");
                        fclose(in);
                        file_state = 0;
                        break;
                    }
                    arr = tmp_realloc;
                    tmp_realloc = NULL;
                    arr_capacity = arr_capacity + SIZE_INC + count;
                }
                fseek(in, 0, SEEK_SET);
                rc = read(arr, arr_len, in, count);
                if (rc)
                {
                    fclose(in);
                    file_state = 0;
                    printf("Ошибка считывания файла\n");
                    free(arr);
                    return rc;
                }
                arr_len += count;
                count = 0;
                printf("Данные файла записаны в массив\n");
                if (file_state)
                {
                    fclose(in);
                    file_state = 0;
                }
                break;
            case ADD_STRUCT:
            {
                char file[NAME];
                printf("Введите имя файла для записи.\nЕсли вы хотите добавить запись в массив, наберите 0\n");
                if (!fgets(file, NAME, stdin))
                {
                    printf("Вы ничего не ввели...\n");
                    break;
                }
                file[strlen(file) - 1] = '\0';
                if (!strcmp(file, "0"))
                {
                    printf("Введите количество добавляемых записей\n");
                    size_t cnt_add = 0;

                    if (!(fgets(buf, NUM + 2, stdin)) || input_num((int *)&cnt_add, buf))
                    {
                        printf("Вы ничего не ввели...\n");
                        break;
                    }
                    memset(buf, 0, NUM + 2);
                    if (!arr)
                    {
                        arr = (theatre *)malloc(sizeof(theatre) * SIZE_INC);
                        if (!arr)
                        {
                            printf("Ошибка выделения памяти, повторите позже...\n");
                            break;
                        }
                        arr_len = 0;
                        arr_capacity = SIZE_INC;
                    }
                    theatre *tmp = arr;
                    rc = add(arr, &tmp, &arr_len, &arr_capacity, cnt_add);
                    if (rc)
                    {
                        free(arr);
                        printf("Ошибка добавления\n");
                        return rc;
                    }
                    printf("Вы успешно добавили %zu структур", cnt_add);
                    break;
                }
                else
                {
                    printf("Неверный ключ\n");
                    break;
                }
                /*
                else
                {
                    if (!arr)
                    {
                        arr = (theatre *)malloc(sizeof(theatre) * SIZE_INC);
                        if (!arr)
                        {
                            printf("Ошибка выделения памяти, повторите позже\n");
                            break;
                        }
                        arr_capacity = SIZE_INC;
                    }
                    rc = read(arr, arr_len, stdin, 1);
                    if (rc)
                    {
                        printf("Ошибка ввода, повторите попытку\n");
                        free(arr);
                        return rc;
                    }
                    arr_len++;
                }
                */
            }
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