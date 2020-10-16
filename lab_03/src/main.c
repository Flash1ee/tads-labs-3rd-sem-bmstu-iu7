#include <stdio.h>
#include "utils.h"
#include "types.h"
#include "retcodes.h"
#include "matrix.h"


int main(void) {
    welcome();
    int rows, cols;
    if (scanf("%d %d", &rows, &cols) != 2 || rows < 0 || cols < 0) {
        printf("Некорректный ввод\n");
        return READ_ERR;
    }
    printf("Введите количество ненулевых элементов: ");
    int count = 0;
    if (scanf("%d", &count) != 1 || count < 0 || count > cols * rows) {
        printf("Некорректный ввод\n");
        return READ_ERR;
    }
    
    matrix_t *normalize_matrix = create_normalize_matrix();
    if (!normalize_matrix) {
        printf("Не удалось создать обычную матрицу");
        return ALLOCATION_ERR;
    }
    int rc;
    rc = initialize_normalize_matrix(normalize_matrix, rows, cols);
    if (rc) {
        printf("Не удалось инициализировать матрицу\n");
        free(normalize_matrix);
        return ALLOCATION_ERR;
    }
    int count_unik = count;
    for (int i = 0; i < count; i++) {
        int r, c;
        printf("Введите номер строки (индексация с 1): ");
        rc = (scanf("%d", &r));
        printf("Введите номер столбца (индексация с 1): ");
        rc += (scanf("%d", &c));
        if (rc != 2 || r < 0 || c < 0 || r - 1 > rows || c - 1 > cols) {
            printf("Некорректный ввод\n");
            free_matrix(normalize_matrix);
            return READ_ERR;
        }
        printf("Введите значение: ");
        my_num_t value;
        rc = (scanf(ScMyNum, &value));
        if (rc != 1) {
            printf("Некорректный ввод\n");
            free_matrix(normalize_matrix);
            return READ_ERR;
        }
        if (write_num(normalize_matrix, value, r - 1, c - 1) == -1) {
            count_unik -= 1;
        }
    }
    // Ввёл обычную матрицу
    sparse_matrix_t *sparse_matrix = create_sparse_matrix();
    if (!sparse_matrix) {
        printf("Не удалось создать разреженную матрицу\n");
        free_matrix(normalize_matrix);
        return ALLOCATION_ERR;
    }

    rc = initialize_sparse_matrix(sparse_matrix, rows, cols, count);
    if (rc) {
        printf("Не удалось создать разреженную матрицу.\n");
        free_matrix(normalize_matrix);
        free(sparse_matrix);
        return rc;
    }
    normalize_to_sparse(normalize_matrix, sparse_matrix);


    vector_t *vector = create_vector();
    if (!vector) {
        printf("Не удалось создать вектор\n");
        free_matrix(normalize_matrix);
        free_sparse_matrix(sparse_matrix);
        return ALLOCATION_ERR;
    }
    int vector_len = cols;
    rc = initialize_vector(vector, vector_len);
    if (rc) {
        free_matrix(normalize_matrix);
        free_sparse_matrix(sparse_matrix);
        free(vector);;
        return ALLOCATION_ERR;
    }
    printf("По заданной матрице будет сформирован вектор из %d элементов\n", cols);
    printf("Введите элементы вектора\n");
    for (int i = 0; i < cols; i++) {
        my_num_t tmp;
        if (scanf(ScMyNum, &tmp) != 1) {
            printf("Введён некорректный элемент.\n");
            free_matrix(normalize_matrix);
            free_sparse_matrix(sparse_matrix);
            free_vector(vector);
            return ALLOCATION_ERR;
        }
        vector->data[i] = tmp;
    }
    printf("Вектор заполнен.\n");

    vector_t *res = create_vector();
    if (!res) {
        printf("Не удалось создать вектор\n");
        free_matrix(normalize_matrix);
        free_sparse_matrix(sparse_matrix);
        free_vector(vector);
        return ALLOCATION_ERR;
    }
    rc = initialize_vector(res, vector_len);
    if (rc) {
        free_matrix(normalize_matrix);
        free_sparse_matrix(sparse_matrix);
        free_vector(vector);
        free(res);
        return ALLOCATION_ERR;
    }
    print_matrix(normalize_matrix);
    print_vector(vector);
    multiply_sparse_on_vector(sparse_matrix, vector, res);
    printf("Результат умножения матрицы на вектор-столбец\n");
    print_vector(res);

    free_matrix(normalize_matrix);
    free_sparse_matrix(sparse_matrix);
    free_vector(vector);
    free_vector(res);
    return EXIT_SUCCESS;
}