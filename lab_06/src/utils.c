#include "utils.h"

#include <ctype.h>
#include <string.h>

#include "avl_tree.h"
#include "hash.h"
#include "my_menu.h"
#include "retcodes.h"
#include "view.h"

#define graph_data "./data/tree"
#define output_bst_beg "./data/graph_bst_beg.gv"
#define output_bst_end "./data/graph_bst_end.gv"

#define output_avl "./data/graph_avl.gv"

#define BUF 3

enum {
    EXIT,
    INPUT_BST,
    ADD_BST,
    DEL_BST,
    PRINT_BST,
    BST_TO_AVL,
    ADD_AVL,
    DEL_AVL,
    PRINT_AVL,
    COUNT
} treeOperation;

const char *errors[] = {
    "Такого файла нет",
    "Спасибо за использование, до свидания.",
    "Не удалось считать БДД",
    "Не удалось открыть файл",
    "Дерево пусто",
    "Некорректный ввод числа",
    "Значение не найдено"

};

void treeInterface() {
    puts("Чтобы ввести ДДП из файла, введите 1");
    puts("Чтобы добавить число в ДДП, введите 2");
    puts("Чтобы удалить число из ДДП введите 3");
    puts("Чтобы вывести ДДП, в файл введите 4");
    puts("");
    puts("Чтобы сбалансировать ДДП введите 5");
    puts("Чтобы добавить число в АВЛ дерево введите 6");
    puts("Чтобы удалить число из АВЛ дерева введите 7");
    puts("Чтобы вывести АВЛ дерево введите 8");
    puts("");
    puts("Чтобы выйти в главное меню введите 0");
    puts("");
    puts("");
}

int getUserTreeOperation() {
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
avl_node_t *preTreeLook(tree_node_t *tree, void *(*act)(avl_node_t *avl, int value), avl_node_t *avl) {
    if (tree == NULL) {
        return NULL;
    }
    avl = act(avl, tree->data);
    avl->left = preTreeLook(tree->left, act, avl->left);
    avl->right = preTreeLook(tree->right, act, avl->right);
    return avlBalance(avl);
}

avl_node_t *bstToAvl(tree_node_t *bst, avl_node_t *avl) {
    return preTreeLook(bst, avlInsert, avl);
}
uint64_t tick(void) {
    uint32_t high, low;
    __asm__ __volatile__(
        "rdtsc\n"
        "movl %%edx, %0\n"
        "movl %%eax, %1\n"
        : "=r"(high), "=r"(low)::"%rax", "%rbx", "%rcx", "%rdx");

    uint64_t ticks = ((uint64_t)high << 32) | low;

    return ticks;
}

void timeOperations() {
    char buf[32] = "./data/tree";
    // puts("Введите номер файла");
    // char num[3];
    // if (!fgets(num, 4, stdin) || num[strlen(num) - 1] != '\n') {
    //     puts("Неверный ввод номера файла");
    //     clean_buf();
    //     return;
    // }
    // num[strlen(num) - 1] = '\0';
    // strcat(buf, num);
    // strcat(buf, ".txt");
    int choice = 0;
    puts("Выберите количество добавляемых чисел");
    puts("10 - 1");
    puts("50 - 2");
    puts("100 - 3");
    puts("250 - 4");

    if (scanf("%d", &choice) != 1 || (choice != 1 && choice != 2 && choice != 3 && choice != 4)) {
        puts("Неверный ввод");
        clean_buf();
        return;
    }
    clean_buf();
    char f_num[5] = {0};
    snprintf(f_num, 3, "%d", choice);
    strcat(buf, f_num);
    strcat(buf, ".txt");
    int cnt = 10;
    if (choice == 2) {
        cnt = 50;
    } else if (choice == 3) {
        cnt = 100;
    } else if (choice == 4) {
        cnt = 250;
    }
    int arr[cnt];
    int i = 0;
    FILE *in = fopen(buf, "r");
    while (fscanf(in, "%d", &arr[i]) == 1) {
        i++;
    }
    fclose(in);

    tree_node_t *bst = NULL;
    avl_node_t *avl = NULL;
    hash_table *hash = NULL;

    uint64_t t_btree = 0;
    uint64_t t_avl = 0;
    uint64_t t_hash = 0;
    uint64_t t_file = 0;

    for (int i = 0; i < 10; i++) {
        uint64_t t_btree_tmp = tick();
        for (int i = 0; i < cnt; i++) {
            tree_node_t *node = createNode(arr[i]);
            bst = bTreeInsert(bst, node);
        }
        // bst = readTreeFromFile(buf);
        t_btree_tmp = tick() - t_btree_tmp;
        t_btree += t_btree_tmp;
        freeTree(bst);
        bst = NULL;
    }
    t_btree /= 10;

    for (int i = 0; i < 10; i++) {
        uint64_t t_avl_tmp = tick();
        for (int i = 0; i < cnt; i++) {
            avl = avlInsert(avl, arr[i]);
        }
        // avl = readAvlFromFile(buf);
        t_avl_tmp = tick() - t_avl_tmp;
        freeAvl(avl);
        avl = NULL;
        t_avl += t_avl_tmp;
    }
    t_avl /= 10;
    for (int i = 0; i < 10; i++) {
        hash = htHashTableCreate(cnt * 2);
        int cmp_k = cnt;
        uint64_t t_hash_tmp = tick();
        for (int i = 0; i < cnt; i++) {
            htInsert(hash, arr[i], cmp_k);
        }
        // readHashFromFile(&hash, buf, 100);
        t_hash_tmp = tick() - t_hash_tmp;
        t_hash += t_hash_tmp;
        htHashTableDestroy(hash);
        hash = NULL;
    }
    t_hash /= 10;
    for (int i = 0; i < 10; i++) {
        FILE *out = fopen("./out/test_write.txt", "w");
        uint64_t t_file_tmp = tick();
        for (int i = 0; i < cnt; i++) {
            fprintf(out, "%ld ", tick() % 1000);
        }
        t_file_tmp = tick() - t_file_tmp;
        fclose(out);
        t_file += t_file_tmp;
    }
    t_file /= 10;

    size_t mem_bst = (sizeof(tree_node_t) * cnt);
    size_t mem_avl = (sizeof(avl_node_t) * cnt);
    size_t mem_hash = (sizeof(hash_table) + (sizeof(int) * cnt));
    printf("Результаты замеров записи %d чисел, память - оперативная.\n", cnt);
    printf("┃ БДД   ┃ АВЛ   ┃ ХЕШ   ┃ ФАЙЛ  ┃\n");
    printf("┃━━━━━━━┃━━━━━━━┃━━━━━━━┃━━━━━━━┃\n");
    printf("┃%7ld┃%7ld┃%7ld┃%7ld┃ время\n", t_btree, t_avl, t_hash, t_file);
    printf("┃%7ld┃%7ld┃%7ld┃%7d┃ память\n", mem_bst, mem_avl, mem_hash, 0);

    printf("Время записи в ДДП %ld, занимаемая память %zu\n", t_btree, mem_bst);
    printf("Время записи в АВЛ %ld чисел, занимаемая память %zu\n", t_avl, mem_avl);
    printf("Время записи в Хеш таблицу %ld чисел, занимаемая память %zu\n", t_hash, mem_hash);
    printf("Время записи в файл %ld чисел, занимаемая память %d\n", t_file, 0);
    return;
}
void treeOperations(tree_node_t **bTree, avl_node_t **avlTree, char *f_name) {
    char buf[BUF + 1];
    strcpy(f_name, graph_data);

    while (1) {
        treeInterface();
        int choice = getUserTreeOperation();
        if (choice == -1) {
            puts("Выбран неверный режим, повторите попытку.");
        } else {
            switch (choice) {
                case EXIT:
                    puts("Переводим в главное меню.");
                    return;
                case INPUT_BST:
                    if (*bTree) {
                        puts("Дерево уже существует.");
                        break;
                    }
                    puts("Введите номер файла с деревом.");
                    if (!fgets(buf, BUF + 1, stdin) || buf[BUF - 1] != '\0') {
                        puts(errors[FILE_NOT_FOUND - 1]);
                        break;
                    }  // TODO добавить кол-во файлов..
                    buf[BUF - 2] = '\0';

                    strcat(f_name, buf);
                    strcat(f_name, ".txt");
                    *bTree = readTreeFromFile(f_name);

                    if (!*bTree) {
                        puts(errors[BST_READ_ERR - 1]);
                        break;
                    }
                    puts("Дерево успешно считано из файла.");
                    FILE *out = fopen(output_bst_beg, "w");
                    if (!out) {
                        puts(errors[F_READ_ERR - 1]);
                        break;
                    }
                    treeExportToDot(out, "bst_tree_beg", *bTree, BST);
                    fclose(out);
                    break;
                case ADD_BST: {
                    puts("Введите целое число для добавления в БДД.");
                    int x;
                    if (scanf("%d", &x) != 1) {
                        puts("Неверный ввод, переводим в меню");
                        clean_buf();
                        break;
                    }
                    tree_node_t *new = createNode(x);
                    tree_node_t *tmp = bTreeInsert(*bTree, new);
                    if (!tmp) {
                        printf("Значение должно быть уникальным, %d уже есть в дереве", x);
                        clean_buf();
                        freeNode(new, NULL);
                        break;
                    }
                    *bTree = tmp;
                    puts("Значение успешно добавлено в БДД");
                    clean_buf();
                    break;
                }
                case DEL_BST: {
                    if (!*bTree) {
                        puts(errors[EMPTY_TREE]);
                    }
                    int x = 0;
                    puts("Введите удаляемое число");
                    if (scanf("%d", &x) != 1) {
                        puts(errors[NUM_READ_ERR]);
                        break;
                    }
                    if (!bTreeSearch(*bTree, x)) {
                        puts(errors[NOT_FOUND]);
                        clean_buf();
                        break;
                    }
                    *bTree = bTreeRemove(*bTree, x);
                    printf("Число %d удалено из дерева", x);
                    clean_buf();
                    break;
                }
                case PRINT_BST:
                    if (!*bTree) {
                        puts("Сначала нужно ввести БДД");
                        break;
                    }
                    out = fopen(output_bst_end, "w");
                    if (!out) {
                        puts(errors[F_READ_ERR - 1]);
                        break;
                    }
                    treeExportToDot(out, "bst_tree_end", *bTree, BST);
                    fclose(out);
                    puts("Дерево экспортировано в файл");
                    puts("для просмотра запустите завершите выполнение программы");
                    puts("И запустите ./show_graph.sh");
                    break;
                case BST_TO_AVL: {
                    if (!*bTree) {
                        puts("Введите ДДП, балансировать нечего.\n");
                        break;
                    }
                    if (!*avlTree) {
                        *avlTree = bstToAvl(*bTree, *avlTree);
                    } else {
                        *avlTree = avlBalance(*avlTree);
                    }
                    puts("Дерево сбалансировано.");
                    break;
                }
                case ADD_AVL: {
                    puts("Введите целое число для добавления в БДД.");
                    int x;
                    if (scanf("%d", &x) != 1) {
                        puts("Неверный ввод, переводим в меню");
                        clean_buf();
                        break;
                    }
                    avl_node_t *tmp = avlInsert(*avlTree, x);
                    if (!tmp) {
                        printf("Значение должно быть уникальным, %d уже есть в дереве", x);
                        clean_buf();
                        break;
                    }
                    *avlTree = tmp;
                    puts("Значение успешно добавлено в АВЛ дерево");
                    clean_buf();
                    break;
                }
                case DEL_AVL: {
                    if (!*avlTree) {
                        puts(errors[EMPTY_TREE]);
                    }
                    int x = 0;
                    puts("Введите удаляемое число");
                    if (scanf("%d", &x) != 1) {
                        puts(errors[NUM_READ_ERR]);
                        break;
                    }
                    if (!avlSearch(*avlTree, x)) {
                        puts(errors[NOT_FOUND]);
                        clean_buf();
                        break;
                    }
                    *avlTree = avlInsert(*avlTree, x);
                    printf("Число %d удалено из дерева", x);
                    clean_buf();
                    break;
                }
                case PRINT_AVL: {
                    if (!*avlTree) {
                        puts("АВЛ дерево не задано");
                    }
                    out = fopen(output_avl, "w");
                    if (!out) {
                        puts(errors[F_READ_ERR]);
                    }
                    treeExportToDot(out, "avl_tree", *avlTree, AVL);
                    fclose(out);
                    puts("Дерево экспортировано в файл");
                    puts("для просмотра запустите завершите выполнение программы");
                    puts("И запустите ./show_graph.sh");
                    break;
                }
            }
        }
    }
}