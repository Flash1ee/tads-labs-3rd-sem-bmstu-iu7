#define _GNU_SOURCE
#include <stdio.h>
#include "retcodes.h"
#include "utils.h"
#include "stack.h"
#include "stack_arr.h"


const char *messages[] = {
    "Спасибо за использование, до скорых встреч.\n",
    "Неверный режим, повторите попытку...\n",
    "Ошибка считывания строки, переводим в меню...\n",
    "Ошибка выделения памяти, переводим в меню...\n",
    "Стек пуст.\n"};

int main(void)
{
    stack_arr_t *arr = NULL;
    free_addr_t *arr_free_addr = NULL;
    int mode = 1;
    while (mode)
    {
        menu();
        if (scanf("%d", &mode) != 1 || valid_mode(mode))
        {
            char c;
            while ((c = getchar()) != '\n' && c != EOF)
                ;
            printf("%s", messages[MODE_ER]);
            continue;
        }
        char c;
        while ((c = getchar()) != '\n' && c != EOF)
            ;
        switch (mode)
        {
        case EXIT:
            printf("%s", messages[EXIT_M]);
            mode = 0;
            clean_stack_list(arr_free_addr);
            if (arr)
            {
                free_s_arr(arr);
            }
            break;
        case ADD_L:
        {
            printf("Введите строку для добавление в стек-список\n");
            char *data = NULL;
            size_t len = 0;

            if (getline(&data, &len, stdin) == -1)
            {
                printf("%s", messages[READ_ER]);
                continue;
            }
            data[strlen(data) - 1] = '\0';

            if (!arr_free_addr)
            {
                arr_free_addr = creatre_free_addr_arr();
                if (!arr_free_addr)
                {
                    printf("%s", messages[ALLOC_ER]);
                    free(data);
                    continue;
                }
            }
            print_free_addr(arr_free_addr);
            if (push(data, arr_free_addr))
            {
                free(data);
                printf("%s", messages[ALLOC_ER]);
                continue;
            }
            printf("Строка добавлена в стек-список\n");
            print_free_addr(arr_free_addr);
            break;
        }
        case ADD_A:
        {
            printf("Введите строку для добавление в стек-массив\n");
            char *data = NULL;
            size_t len = 0;
            if (getline(&data, &len, stdin) == -1)
            {
                printf("%s", messages[READ_ER]);
                continue;
            }
            data[strlen(data) - 1] = '\0';
            if (push_s_arr(&arr, data))
            {
                free(data);
                printf("%s", messages[ALLOC_ER]);
                continue;
            }
            printf("Строка добавлена в стек-массив\n");
            break;
        }
        case DEL_L:
            printf("С вершины стека-списка будет удален элемент\n");
            if (pop(arr_free_addr))
            {
                printf("%s", messages[EMPTY_ER]);
            }
            break;
        case DEL_A:
            printf("С вершины стека-массива будет удален элемент\n");
            if (pop_s_arr(arr))
            {
                printf("%s", messages[EMPTY_ER]);
            }
            break;
        case CHECK_TOP_L:
            printf("На вершине стека-списка:\n");
            if (peek())
            {
                printf("%s", messages[EMPTY_ER]);
            }
            break;
        case CHECK_TOP_A:
            printf("На вершине стека-массива:\n");
            if (peek_s_arr(arr))
            {
                printf("%s", messages[EMPTY_ER]);
            }
            break;
        case PRINT_L:
            printf("Будут выведены все элемента из стека-списка.\n");
            if (print_all())
            {
                printf("%s", messages[EMPTY_ER]);
            }
            break;
        case PRINT_A:
            printf("Будут выведены все элемента из стека-массива.\n");
            if (print_s_arr(arr))
            {
                printf("%s", messages[EMPTY_ER]);
            }
            break;
        case CLEAN_L:
            if (clean_stack_list(arr_free_addr))
            {
                printf("%s", messages[EMPTY_ER]);
            }
            else
            {
                printf("Стек-список очищен.\n");
            }
            break;
        case CLEAN_A:
            if (free_s_arr(arr))
            {
                printf("%s", messages[EMPTY_ER]);
            }
            else
            {
                printf("Стек-массив очищен.\n");
            }
            break;
        case ADDR_L:
            print_free_addr(arr_free_addr);
            break;
        }
    }
}
