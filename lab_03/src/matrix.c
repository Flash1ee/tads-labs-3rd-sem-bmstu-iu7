#include "matrix.h"
#include "types.h"
#include "retcodes.h"
#include <time.h>
#include <sys/time.h>
#include <inttypes.h>
#include <string.h>



vector_t *create_vector() {
    vector_t *arr = NULL;
    arr = calloc(1, sizeof(vector_t));
    if (!arr) {
        return NULL;
    }
    return arr;
}
matrix_t *create_normalize_matrix() {
    matrix_t *arr = NULL;
    arr = calloc(1, sizeof(matrix_t));
    if (!arr) {
        return NULL;
    }
    return arr;
}
sparse_matrix_t *create_sparse_matrix() {
    sparse_matrix_t *arr = NULL;
    arr = calloc(1, sizeof(sparse_matrix_t));
    if (!arr) {
        return NULL;
    }
    return arr;
}
int initialize_vector(vector_t *vector, int len) {
    my_num_t *data = calloc(len * 2, sizeof(my_num_t));
    if (!data) {
        return ALLOCATION_ERR;
    }
    vector->data = data;
    vector->len = len;
    vector->capacity = len * 2;
    return EXIT_SUCCESS;
}
int initialize_normalize_matrix(matrix_t *src, int rows, int cols) {
    my_num_t **arr = NULL;
    arr = malloc(rows * sizeof(my_num_t *));
    if (!arr) {
        return ALLOCATION_ERR;
    }
    for (int i = 0; i < rows; i++) {
        arr[i] = (my_num_t*) calloc(cols, sizeof(my_num_t));
        if (!arr[i]) {
            free_arr(arr, rows);
            return ALLOCATION_ERR;
        }
    }
    src->rows = rows;
    src->cols = cols;
    src->matrix = arr;
    return EXIT_SUCCESS;
}
void free_vector(vector_t *vector) {
    free(vector->data);
    free(vector);
}
void free_sparse_matrix(sparse_matrix_t *sparse_matrix) {
    free(sparse_matrix->values);
    free(sparse_matrix->cols_index);
    free(sparse_matrix->rows_ind);
    free(sparse_matrix);
}
void free_matrix(matrix_t *matrix) {
    free_arr(matrix->matrix, matrix->rows);
    free(matrix);
}
void free_arr(my_num_t **arr, int rows) {
    for (int i = 0; i < rows; i++) {
        free(arr[i]);
    }
}
int write_num(matrix_t *matrix, my_num_t value, int row, int col) {
    if (matrix->matrix[row][col] != 0) {
        return -1;
    }
    matrix->matrix[row][col] = value;
    return 0;
}

int initialize_sparse_matrix(sparse_matrix_t *src, int rows, int cols, int count_not_null) {
    if (!rows || !cols) {
        return ARG_ERR;
    }
    my_num_t *data = (my_num_t*) malloc(sizeof(my_num_t) * count_not_null);
    if (!data) {
        return ALLOCATION_ERR;
    }
    int *arr_rows = (int*) malloc(sizeof(int) * count_not_null);
    if (!arr_rows) {
        free(data);
        return ALLOCATION_ERR;
    }
    int *values_ptr = (int*) malloc(sizeof(int) * (cols + 1));
    if (!values_ptr) {
        free(data);
        free(arr_rows);
        return ALLOCATION_ERR;
    }
    src->rows = rows;
    src->cols = cols;
    src->count = count_not_null;
    src->values = data;
    src->rows_ind = arr_rows;
    src->cols_index = values_ptr;
    return EXIT_SUCCESS;
}
void normalize_to_sparse(matrix_t *normalize_matrix, sparse_matrix_t *sparse_matrix) {
    int len = 0;
    int flag = FALSE;
    for (int col = 0; col < normalize_matrix->cols; col++) {
        flag = FALSE;
        for (int row = 0; row < normalize_matrix->rows; row++) {
            if (normalize_matrix->matrix[row][col] != 0) {
                sparse_matrix->values[len] = normalize_matrix->matrix[row][col];
                sparse_matrix->rows_ind[len] = row;
                if (!flag) {
                    sparse_matrix->cols_index[col] = len;
                    flag = TRUE;
                }
                len++;
            }
        if (!flag) {
            sparse_matrix->cols_index[col] = len;
        }
        }
    }
    for (int i = 0; i < sparse_matrix->count; i++) {
        printf("%lf ", sparse_matrix->values[i]);
    }
    sparse_matrix->cols_index[sparse_matrix->cols] = len;
}
void sparse_to_normalize(sparse_matrix_t *sparse_matrix, matrix_t *normalize_matrix) {
    for (int col = 0; col < sparse_matrix->cols; col++) {
        if (sparse_matrix->cols_index[col] == sparse_matrix->cols_index[col + 1]) {
            for (int row = 0; row < sparse_matrix->rows; row++) {
                normalize_matrix->matrix[row][col] = 0;
            }
        } else {
            for (int i = sparse_matrix->cols_index[col]; i < sparse_matrix->cols_index[col + 1]; i++) {
                normalize_matrix->matrix[sparse_matrix->rows_ind[i]][col] = sparse_matrix->values[i];
            }
        }
    }
}
/**
*@brief Умножение разреженной матрицы на нормальный вектор
*
*@param sparse_matrix разреженная матрица
*@param vector вектор
*@param res результирующий массив длины vector
*@warning res - изначально заполнен нулями 
 */
void multiply_sparse_on_vector(sparse_matrix_t *sparse_matrix, vector_t *vector, vector_t *res) {
    for (int i = 0; i < sparse_matrix->cols; i++) {
        for (int j = sparse_matrix->cols_index[i]; j < sparse_matrix->cols_index[i + 1]; j++) {
            res->data[sparse_matrix->rows_ind[j]] = res->data[sparse_matrix->rows_ind[j]] + sparse_matrix->values[j] * vector->data[i];
        }
    }
}
void multiply_normalize_on_vector(matrix_t *matrix, vector_t *vector, vector_t *res) {
    for (int i = 0; i < matrix->rows; i++) {
        for (int j = 0; j < 1; j++) {
            for (int k = 0; k < matrix->cols; k++) {
                res->data[i] += matrix->matrix[i][k] * vector->data[k];
            }
        }
    }
}
void print_matrix(matrix_t *matrix) {
    for (int i = 0; i < matrix->rows; i++) {
        for (int j = 0; j < matrix->cols; j++) {
            printf(PrMyNum" ", matrix->matrix[i][j]);
        }
        printf("\n");
    }
}
void print_vector(vector_t *vector) {
    for (int i = 0; i < vector->len; i++) {
        printf(PrMyNum" ", vector->data[i]);
    }
    printf("\n");
}
int fill_matrix(matrix_t *normalize_matrix, double percent_matrix) {
    int count_not_null = percent_matrix * normalize_matrix->rows * normalize_matrix->cols;
    int rand_row;
    int rand_col;
    int num;
    srand(time(NULL));
    while (count_not_null) {
        while (TRUE) {
            rand_row = rand() % normalize_matrix->rows;
            rand_col = rand() % normalize_matrix->cols;
            if (normalize_matrix->matrix[rand_row][rand_col] == 0) {
                num = (rand() % MAX_VAL) - MAX_VAL / 2; 
                write_num(normalize_matrix, num, rand_row, rand_col);
                count_not_null--;
                break;
            }
        }
    }
    return EXIT_SUCCESS;
}
int fill_vector(vector_t *vector, double percent_vector) {
    int count_not_null = percent_vector * vector->len;
    int rand_ind;
    int num;
    srand(time(NULL));
    while (count_not_null) {
        while (TRUE) {
            rand_ind = rand() % vector->len;
            if (vector->data[rand_ind] == 0) {
                num = (rand() % MAX_VAL) - MAX_VAL / 2; 
                vector->data[rand_ind] = num;
                count_not_null--;
                break;
            }
        }
    }
    return EXIT_SUCCESS;
}
void get_multiply_time(sparse_matrix_t *sparse_matrix, matrix_t *normalize_matrix, vector_t *vector, vector_t *res) {
    printf("Исходная матрица - множимое\n");
    print_matrix(normalize_matrix);
    printf("Вектор-столбец - множитель\n");
    print_vector(vector);

    struct timeval tv_start, tv_stop;
    int64_t time_n = 0, time_s = 0;

    for (int i = 0; i < 1; i++) {
        gettimeofday(&tv_start, NULL);
        multiply_normalize_on_vector(normalize_matrix, vector, res);
        gettimeofday(&tv_stop, NULL);
        time_n += (tv_stop.tv_sec - tv_start.tv_sec) * 1000000LL + (tv_stop.tv_usec - tv_start.tv_usec);
    }
    printf("Результат умножения в количестве %d раз.\n", COUNT);
    print_vector(res);
    printf("\n\n");
    for (int i = 0; i < res->len; i++) {
        res->data[i] = 0;
    }
    for (int i = 0; i < 1; i++) {
        gettimeofday(&tv_start, NULL);
        multiply_sparse_on_vector(sparse_matrix, vector, res);
        gettimeofday(&tv_stop, NULL);
        time_s += (tv_stop.tv_sec - tv_start.tv_sec) * 1000000LL + (tv_stop.tv_usec - tv_start.tv_usec);
    }
    printf("Результат умножения в количестве %d раз.\n", COUNT);
    print_vector(res);
    printf("\n\n");
    double res_n, res_s;
    res_n = time_n / (double)COUNT;
    res_s = time_s / (double)COUNT;
    printf("Размерность матрицы %d x %d.\n", normalize_matrix->rows, normalize_matrix->cols);
    printf("Умножение нормализованной матрицы на вектор выполнено за %lf.\n", res_n);
    printf("Умножение разреженной матрицы на вектор выполнено за %lf.\n", res_s);

}