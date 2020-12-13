#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "my_menu.h"

void welcome() {
    puts("Добро пожаловать в программу по работе с деревьями и хеш таблицей");
    puts("Чтобы закончить выполнение введите 0");
    puts("Для работы с деревьями, введите 1");
    puts("Для работы с хеш-таблицей введите 2");
    puts("Чтобы добавить число в файл, введите 3");
}


int getUserInput() {
    int choice = -1;
    char buf[4];
    if (!fgets(buf,4, stdin) || buf[1] != '\n' || !isdigit(*buf)) {
        return -1;
    }
    choice = atoi(buf);
    for (int i = 0; i < 4; ++i) {
        if (i == choice) {
            return choice;
        }
    }
    return -1;
}

void clean_buf() {
    int c;
    while ((c = getchar() != '\n') && c != EOF) {
    }
}
