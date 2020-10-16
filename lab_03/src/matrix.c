#include "matrix.h"
#include "types.h"
#include "retcodes.h"


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
    if (matrix->matrix[col][row] != 0) {
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