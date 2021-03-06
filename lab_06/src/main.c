#include <stdio.h>
#include <string.h>

#include "avl_tree.h"
#include "my_menu.h"
#include "retcodes.h"
#include "tree.h"
#include "view.h"
#include "utils.h"
#include "hash.h"
#include "hash_oper.h"

#define EXIT 0
#define TREE 1
#define HASH_TABLE 2
#define FILE_ADD 3
#define TIME_TEST 4

#define file_storage "./data/"
int main() {
    tree_node_t *bst_tree = NULL;
    avl_node_t *avl_tree = NULL;

    hash_table *ht = NULL;
    int cmp_k = 0;

    char f_name[50] = { 0 };
    char f_hash[50] = { 0 };

    while (1) {
        welcome();
        int choice = getUserInput();
        if (choice == -1) {
            puts("Выбран неверный режим, повторите попытку.");
        } else {
            switch (choice) {
                case EXIT:
                    puts("Спасибо за использование, до свидания");
                    if (bst_tree) {
                        freeTree(bst_tree);
                    }
                    if (avl_tree) {
                        freeAvl(avl_tree);
                    }
                    return EXIT_SUCCESS;
                case TREE:
                    treeOperations(&bst_tree, &avl_tree, f_name);
                    break;
                case HASH_TABLE:
                    hashOperations(&ht, f_hash, &cmp_k);
                    break;
                case FILE_ADD: {
                    puts("Введите целое число для добавления в файл.");
                    int x;
                    if (scanf("%d", &x) != 1) {
                        puts("Неверный ввод, переводим в меню");
                        clean_buf();
                        break;
                    }
                    clean_buf();
                    char file[16];
                    puts("Введите имя файла из папки data");
                    if (!fgets(file, 16, stdin) || strlen(file) < 2) {
                        puts("Неверный ввод, переводим в меню");
                    }
                    file[strlen(file) - 1] = '\0';
                    char full_name[32] = {0};
                    strcpy(full_name, file_storage);
                    strcat(full_name, file);
                    printf("Число будет добавлено в файл %s\n", full_name);
                    FILE *out = fopen(full_name, "a");
                    if (!out) {
                        puts("Ошибка открытия файла");
                        break;
                    }
                    fprintf(out, " %d ", x);
                    fclose(out);
                    puts("Число успешно добавлено в файл");
                    break;
                }
                case TIME_TEST:
                    puts("Режим замеры времени добавления в ДДП/АВЛ/Хеш-таблицу/файл");
                    timeOperations();
                    break;
            }
        }
    }
}