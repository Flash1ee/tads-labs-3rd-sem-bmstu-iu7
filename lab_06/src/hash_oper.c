#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "my_menu.h"
#include "hash_oper.h"
#include "hash.h"

void hashInterface() {
    puts("Чтобы заполнить хеш-таблицу числами из файла, введите 1");
    puts("Чтобы добавить число в таблицу, введите 2");
    puts("Чтобы удалить число из таблицы введите 3");
    puts("Чтобы вывести таблицу, введите 4");
    puts("");
    puts("Чтобы выйти в главное меню введите 0");
    puts("");
    puts("");
}
static enum {
    EXIT,
    FILL_HASH_FROM_FILE,
    ADD_HASH,
    DEL_HASH,
    PRINT_HASH,
    PERCENT_FILL,
    COUNT
} hashOperation;

static const char *errors[] = {
    "Некорректный ввод, переводим в меню..",
    "Не удалось считать данные в таблицу",
    "Элемент уже в таблице",
    "Превышено количество сравнений при коллизиях",
    "Таблица пуста",
    "Значение не найдено"

};

static enum {
    ERR_INPUT,
    HASH_READ_ERR,
    ELEM_FOUND,
    MAX_CMP,
    EMPTY_TABLE
} types_err;

static int getUserHashOperation() {
    int choice = -1;
    char buf[4];

    if (!fgets(buf, 4, stdin) || buf[1] != '\n' || !isdigit(*buf)) {
        return -1;
    }
    choice = atoi(buf);
    for (int i = 0; i < COUNT; ++i) {
        if (i == choice) {
            return choice;
        }
    }
    return -1;
}

#define graph_data "./data/tree"
#define BUF 3
#define INIT_SIZE 53

void hashOperations(hash_table **ht, char *f_name, int *cmp_k) {
    strcpy(f_name, graph_data);
    char f_name_cur[20];

    while (1) {
        hashInterface();
        int choice = getUserHashOperation();
        if (choice == -1) {
            puts("Выбран неверный режим, повторите попытку.");
        } else {
            switch (choice) {
                case EXIT: {
                    puts("Переводим в главное меню");
                    return;
                }
                case FILL_HASH_FROM_FILE: {
                    if (*ht) {
                        puts("Таблица уже существует.");
                        break;
                    }
                    puts("Введите номер файла для выгрузки в таблицу.");
                    int n = 0;
                    if (scanf("%d", &n) != 1 || n < 0 || n > MAX_FILE) {
                        puts(errors[ERR_INPUT]);
                        clean_buf();
                        break;
                    }
                    clean_buf();
                    puts("Введите количество макс. кол-во сравнений при возникновении коллизий");
                    if (scanf("%d", cmp_k) != 1) {
                        puts(errors[ERR_INPUT]);
                        clean_buf();
                        break;
                    }
                    char buf[3];
                    snprintf(buf, 3, "%d", n);

                    strcpy(f_name_cur, f_name);
                    strcat(f_name_cur, buf);
                    strcat(f_name_cur, ".txt");
                    int rc = readHashFromFile(ht, f_name_cur, *cmp_k);
                    if (rc || !*ht) {
                        puts(errors[HASH_READ_ERR]);
                        *ht = NULL;
                        clean_buf();
                        break;
                    }
                    puts("Таблица успешно считана из файла.");
                    clean_buf();
                    break;
                }
                case ADD_HASH: {
                    puts("Введите число для добавления в таблицу");
                    int x = 0;
                    if (scanf("%d", &x) != 1) {
                        puts(errors[ERR_INPUT]);
                        clean_buf();
                        break;
                    }
                    if (!*ht) {
                        *ht = htHashTableCreate(INIT_SIZE);
                    }
                    if (*cmp_k == 0) {
                        puts("Введите количество макс. кол-во сравнений при возникновении коллизий");
                        if (scanf("%d", cmp_k) != 1) {
                            puts(errors[ERR_INPUT]);
                            clean_buf();
                            break;
                        }
                    }
                    if (htSearch(*ht, x) != NULL) {
                        puts(errors[ELEM_FOUND]);
                        clean_buf();
                        break;
                    }
                    int rc = htInsert(*ht, x, *cmp_k);
                    if (rc) {
                        puts(errors[MAX_CMP]);
                        clean_buf();
                        break;
                    }
                    puts("Число добавлено в таблицу");
                    break;
                }
                case DEL_HASH: {
                    if (!*ht) {
                        puts(errors[EMPTY_TABLE]);
                        break;
                    }
                    puts("Введите значение для удаления");
                    int x = 0;
                    if (scanf("%d", &x) != 1) {
                        puts(errors[ERR_INPUT]);
                        clean_buf();
                        break;
                    }
                    if (htSearch(*ht, x) == NULL) {
                        puts(errors[ELEM_FOUND]);
                        clean_buf();
                        break;
                    }
                    htDelete(*ht, x);
                    puts("Число удалено из таблицы");
                    clean_buf();
                    break;
                }
                case PRINT_HASH: {
                    puts("Хеш таблица:");
                    if (!*ht) {
                        puts(errors[EMPTY_TABLE]);
                        break;
                    }
                    printTable(*ht);
                    break;
                }
                case PERCENT_FILL:
                    if (!*ht) {
                        puts(errors[EMPTY_TABLE]);
                    }
                    puts("Процент заполненности таблицы:");
                    printf("%% = %d\n", htFillPercent(*ht));
                    break;
            }
        }
    }
}