#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <assert.h>
#include "graph.h"
#include "menu.h"
#include "print.h"

#define N 10
#define M 3
#define V 6

#define DATA "./data/graph"
#define OUT_PATH "./data/graph.gv"


#define EXIT 0
#define FILE_LOAD 1
#define TASK 2
#define HELP 3
#define OUTPUT 4
#define COUNT 5


typedef enum {
    IN_ERR,
    OPEN_ERR,
    F_READ_ERR,
    EMPTY,
    NO_MATRIX
} err_types;

const char *errors[] = {
    "Неверный ввод, повторите попытку",
    "Не удалось открыть файл",
    "Ошибка чтения из файла",
    "Матрица смежности пуста",
    "Матрица не заполнена"

};
int main() {

    int **matrix = NULL;
    int size = 0;

    while (1) {
        welcome();
        int choice = get_user_action(COUNT);
        if (choice == -1) {
            puts(errors[IN_ERR]);
        } else {
            switch (choice) {
                case EXIT:
                    puts("Спасибо за использование, до свидания");
                    return EXIT_SUCCESS;
                case HELP:
                    help_info();
                    break;
                case FILE_LOAD: {
                    char file[32] = {0};
                    puts("Введите имя файла из папки data");
                    if (!fgets(file, 32, stdin) || file[strlen(file) - 1] != '\n') {
                        puts(errors[IN_ERR]);
                        break;
                    }
                    char path[32];
                    file[strlen(file) - 1] = '\0';
                    strcpy(path, DATA);
                    strcat(path, file);
                    FILE *in = fopen(path, "r");
                    if (!in) {
                        puts(errors[OPEN_ERR]);
                        break;
                    }
                    matrix = read_from_file(in, &size);
                    if (!matrix) {
                        puts(errors[F_READ_ERR]);
                        fclose(in);
                        break;
                    }
                    fclose(in);
                    puts("Матрица смежности заполнена");
                    break;
                }
                case TASK: {
                    if (!matrix) {
                        puts(errors[EMPTY]);
                        break;
                    }
                    puts("Введите вершину, для которой хотите найти максимальный путь");
                    printf("Допустимый диапазон v [0 : %d]\n", size - 1);
                    int vertex = -1;
                    if (scanf("%d", &vertex) != 1 || vertex < 0 || vertex > size - 1) {
                        puts(errors[IN_ERR]);
                        clean_buf();
                        break;
                    }
                    clean_buf();
                    puts("Максимальные пути для выбранной вершины");
                    longest_path(matrix, size, vertex);
                    break;
                }
                case OUTPUT: {
                    if (!matrix) {
                        puts(errors[NO_MATRIX]);
                        break;
                    }
                    FILE *out = fopen(OUT_PATH, "w");
                    assert(out);
                    export_graph_to_dot(out, "graph_res", matrix, size);
                    fclose(out);
                    puts("Матрица экспортирована. Чтобы посмотреть запустить ./show_graph");
                    break;
                }
            }
        }
    }
    // int data[N][M] = {
    //     {0,1,5},
    //     {0,2,3},
    //     {1,3,6},
    //     {1,2,2},
    //     {2,4,4},
    //     {2,5,2},
    //     {2,3,7},
    //     {3,5,1},
    //     {3,4,-1},
    //     {4,5,-2},
    // };
    // int **arr = create_matrix_adjacency(V, V);

    // for (int i = 0; i < N; i++) {
    //     int u = data[i][0];
    //     int v = data[i][1];
    //     int weight = data[i][2];
    //     add_edge(arr, u, v, weight);
    // }
    // for (int i = 0; i < V; i++) {
    //     for (int j = 0; j < V; j++) {
    //         printf("%d ", arr[i][j]);
    //     }
    //     puts("");
    // }

    // int s = 2;
    // longest_path(arr, V, s);


    return 0;
}