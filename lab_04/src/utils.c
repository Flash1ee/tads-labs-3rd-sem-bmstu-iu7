#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <inttypes.h>

#include "retcodes.h"
#include "utils.h"
void menu()
{
    printf("Добро пожаловать в программу по работе со стеком.\n");
    printf("Введите 1, чтобы добавить строку в стек-список.\n");
    printf("Введите 2, чтобы добавить строку в стек-массив.\n\n");
    printf("Введите 3, чтобы удалить элемент с вершины стека-списка.\n");
    printf("Введите 4, чтобы удалить элемент с вершины стека-массива.\n\n");
    printf("Введите 5, чтобы посмотреть строку на вершине стека-списка\n");
    printf("Введите 6, чтобы посмотреть строку на вершине стека-массива\n\n");
    printf("Введите 7, чтобы вывести стек-список.\n");
    printf("Введите 8, чтобы вывести стек-массив.\n\n");
    printf("Введите 9, чтобы очистить стек-список.\n");
    printf("Введите 10, чтобы очистить стек-массив.\n\n");
    printf("Введите 11, чтобы вывести список свободных адресов для стека-списка.\n\n");
    printf("Введите 12, чтобы заполнить стек-список N случайными элементами.\n");
    printf("Введите 13, чтобы заполнить стек-массив N случайными элементами.\n\n");
    printf("Введите 14, чтобы удалить из стека-списка N элементов.\n");
    printf("Введите 15, чтобы удалить из стека-массива N элементов.\n\n");
    printf("Введите 0, чтобы закончить выполнение.\n");
}
const char *place[] = {
    "STACK_LIST",
    "STACK_ARRAY"};
int valid_mode(int mode)
{
    for (int i = 0; i < COUNT; i++)
    {
        if (mode == i)
        {
            return EXIT_SUCCESS;
        }
    }
    return EXIT_FAILURE;
}
char *generate_random_str(size_t len)
{
    char *str = malloc(len + 1);
    if (!str)
    {
        return NULL;
    }
    for (size_t i = 0; i < len; i++)
    {
        str[i] = (char)((rand() % (ASCII_END - ASCII_START)) + ASCII_START);
    }
    str[len] = '\0';
    return str;
}
size_t generate_random_len()
{
    return MAX_STR;
    // return rand() % MAX_STR;
}
int rand_push(stack_arr_t **arr, free_addr_t **addresses, int flag)
{
    if (flag != LIST && flag != ARRAY)
    {
        return ARG_ERR;
    }
    if (!arr || !addresses)
    {
        return ARG_ERR;
    }
    if (!(*addresses))
    {
        *addresses = creatre_free_addr_arr();
        if (!(*addresses))
        {
            return ALLOCATION_ERR;
        }
    }
    int count = 0;
    printf("Введите кол-во добавляемых строк.\n");
    if (scanf("%d", &count) != 1 || count < 0)
    {
        return ARG_ERR;
    }
    int rc = 0;
    srand(time(NULL));
    struct timeval tv_start, tv_stop;
    int64_t time_add = 0;
    int64_t memory = 0;
    for (int i = 0; i < count; i++)
    {
        size_t len = generate_random_len();
        char *tmp = generate_random_str(len);
        memory += (strlen(tmp) + 1);
        if (!tmp)
        {
            return ALLOCATION_ERR;
        }
        if (flag == LIST)
        {
            if (node_cnt + count > STACK_LIM)
            {
                return FULL_ER;
            }
            gettimeofday(&tv_start, NULL);
            rc = push(tmp, *addresses);
            gettimeofday(&tv_stop, NULL);
            memory += sizeof(node_t);
            if (rc)
            {
                free(tmp);
                return ALLOCATION_ERR;
            }
            time_add += (tv_stop.tv_sec - tv_start.tv_sec) * 1000000LL + (tv_stop.tv_usec - tv_start.tv_usec);
        }
        else if (flag == ARRAY)
        {
            gettimeofday(&tv_start, NULL);
            rc = push_s_arr(arr, tmp);
            gettimeofday(&tv_stop, NULL);
            if (rc)
            {
                free(tmp);
                return rc;
            }
            time_add += (tv_stop.tv_sec - tv_start.tv_sec) * 1000000LL + (tv_stop.tv_usec - tv_start.tv_usec);
        }
    }
    if (flag == ARRAY)
    {
        memory += sizeof(stack_arr_t);
    }
    else
    {
        memory += sizeof(top);
    }
    
    printf("%d строк успешно добавлено в %s.\n", count, place[flag]);
    printf("Время, затраченное на добавление %d строк = %ld микросекунд\n", count, time_add);
    printf("Занимаемая память : %ld\n", memory);
    return EXIT_SUCCESS;
}
int multi_pop(stack_arr_t *arr, free_addr_t *arr_free_addr, int flag)
{
    if (flag != LIST && flag != ARRAY)
    {
        return MODE_ER;
    }
    if (flag == ARRAY)
    {
        if (!arr || !arr->top)
        {
            return EMPTY_ER;
        }
        
    }
    if (flag == LIST)
    {
        if (!arr_free_addr)
        {
            return EMPTY_ER;
        }
        if (!top)
        {
            return EMPTY_ER;
        }
    }
    int count = 0;
    printf("Введите количество удаляемых элементов.\n");
    if (scanf("%d", &count) != 1 || count < 0)
    {
        return READ_ER;
    }
    int rc = 0;
    int i;
    struct timeval tv_start, tv_stop;
    int64_t res = 0;
    if (flag == LIST)
    {
        for (i = 0; i < count; i++)
        {
            gettimeofday(&tv_start, NULL);
            rc = pop(arr_free_addr);
            gettimeofday(&tv_stop, NULL);
            if (rc)
            {
                if (rc == EMPTY)
                {
                    break;
                }
                return rc;
            }
            res += (tv_stop.tv_sec - tv_start.tv_sec) * 1000000LL + (tv_stop.tv_usec - tv_start.tv_usec);
        }
    }
    if (flag == ARRAY)
    {
        for (i = 0; i < count; i++)
        {
            gettimeofday(&tv_start, NULL);
            rc = pop_s_arr(arr);
            gettimeofday(&tv_stop, NULL);
            if (rc)
            {
                if (rc == EMPTY)
                {
                    break;
                }
                return rc;
            }
            res += (tv_stop.tv_sec - tv_start.tv_sec) * 1000000LL + (tv_stop.tv_usec - tv_start.tv_usec);
        }
    }
    if (rc == EMPTY)
    {
        printf("Количество строк в списке меньше, чем вы хотите удалить. Удалены все строки списка...\n");
    }
    printf("Время затраченное на удаление %d в %s строк = %ld\n", i, place[flag], res);
    return EXIT_SUCCESS;
}