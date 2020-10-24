#include "matrix.h"
#include "types.h"
#include "retcodes.h"
#include <time.h>
#include <sys/time.h>
#include <inttypes.h>
#include <string.h>

vector_t *create_vector()
{
    vector_t *arr = NULL;
    arr = calloc(1, sizeof(vector_t));
    if (!arr)
    {
        return NULL;
    }
    return arr;
}
matrix_t *create_normalize_matrix()
{
    matrix_t *arr = NULL;
    arr = calloc(1, sizeof(matrix_t));
    if (!arr)
    {
        return NULL;
    }
    return arr;
}
sparse_matrix_t *create_sparse_matrix()
{
    sparse_matrix_t *arr = NULL;
    arr = calloc(1, sizeof(sparse_matrix_t));
    if (!arr)
    {
        return NULL;
    }
    return arr;
}
int initialize_vector(vector_t *vector, int len)
{
    my_num_t *data = calloc(len * 2, sizeof(my_num_t));
    if (!data)
    {
        return ALLOCATION_ERR;
    }
    vector->data = data;
    vector->len = len;
    vector->capacity = len * 2;
    return EXIT_SUCCESS;
}
int initialize_normalize_matrix(matrix_t *src, int rows, int cols)
{
    my_num_t **arr = NULL;
    arr = calloc(rows, sizeof(my_num_t *));
    if (!arr)
    {
        return ALLOCATION_ERR;
    }
    for (int i = 0; i < rows; i++)
    {
        arr[i] = (my_num_t *)calloc(cols, sizeof(my_num_t));
        if (!arr[i])
        {
            free_arr(arr, rows);
            return ALLOCATION_ERR;
        }
    }
    src->rows = rows;
    src->cols = cols;
    src->matrix = arr;
    return EXIT_SUCCESS;
}
void free_vector(vector_t *vector)
{
    free(vector->data);
    free(vector);
}
void free_sparse_matrix(sparse_matrix_t *sparse_matrix)
{
    free(sparse_matrix->values);
    free(sparse_matrix->cols_index);
    free(sparse_matrix->rows_ind);
    free(sparse_matrix);
}
void free_matrix(matrix_t *matrix)
{
    free_arr(matrix->matrix, matrix->rows);
    free(matrix);
}
void free_arr(my_num_t **arr, int rows)
{
    for (int i = 0; i < rows; i++)
    {
        free(arr[i]);
    }
}
int write_num(matrix_t *matrix, my_num_t value, int row, int col)
{
    if (matrix->matrix[row][col] != 0)
    {
        return -1;
    }
    matrix->matrix[row][col] = value;
    return 0;
}

int initialize_sparse_matrix(sparse_matrix_t *src, int rows, int cols, int count_not_null)
{
    if (!rows || !cols)
    {
        return ARG_ERR;
    }
    my_num_t *data = (my_num_t *)calloc(count_not_null, sizeof(my_num_t));
    if (!data)
    {
        return ALLOCATION_ERR;
    }
    int *arr_rows = (int *)calloc(count_not_null, sizeof(int));
    if (!arr_rows)
    {
        free(data);
        return ALLOCATION_ERR;
    }
    int *values_ptr = (int *)calloc(cols + 1, sizeof(int));
    if (!values_ptr)
    {
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
void normalize_to_sparse(matrix_t *normalize_matrix, sparse_matrix_t *sparse_matrix)
{
    int len = 0;
    int flag = FALSE;
    for (int col = 0; col < normalize_matrix->cols; col++)
    {
        flag = FALSE;
        for (int row = 0; row < normalize_matrix->rows; row++)
        {
            if (normalize_matrix->matrix[row][col] != 0)
            {
                sparse_matrix->values[len] = normalize_matrix->matrix[row][col];
                sparse_matrix->rows_ind[len] = row;
                if (!flag)
                {
                    sparse_matrix->cols_index[col] = len;
                    flag = TRUE;
                }
                len++;
            }
            if (!flag)
            {
                sparse_matrix->cols_index[col] = len;
            }
        }
    }
    // for (int i = 0; i < sparse_matrix->count; i++)
    // {
    //     printf("%lf ", sparse_matrix->values[i]);
    // }
    sparse_matrix->cols_index[sparse_matrix->cols] = len;
}
void sparse_to_normalize(sparse_matrix_t *sparse_matrix, matrix_t *normalize_matrix)
{
    for (int col = 0; col < sparse_matrix->cols; col++)
    {
        if (sparse_matrix->cols_index[col] == sparse_matrix->cols_index[col + 1])
        {
            for (int row = 0; row < sparse_matrix->rows; row++)
            {
                normalize_matrix->matrix[row][col] = 0;
            }
        }
        else
        {
            for (int i = sparse_matrix->cols_index[col]; i < sparse_matrix->cols_index[col + 1]; i++)
            {
                normalize_matrix->matrix[sparse_matrix->rows_ind[i]][col] = sparse_matrix->values[i];
            }
        }
    }
}

void multiply_normalize_on_vector(matrix_t *matrix, matrix_t *vector, matrix_t *res)
{
    for (int i = 0; i < matrix->rows; i++)
    {
        for (int j = 0; j < 1; j++)
        {
            for (int k = 0; k < matrix->cols; k++)
            {
                res->matrix[i][0] += matrix->matrix[i][k] * vector->matrix[k][0];
            }
        }
    }
}
void print_matrix(matrix_t *matrix)
{
    for (int i = 0; i < matrix->rows; i++)
    {
        for (int j = 0; j < matrix->cols; j++)
        {
            printf(PrMyNum " ", matrix->matrix[i][j]);
        }
        printf("\n");
    }
}
void print_vector(vector_t *vector)
{
    for (int i = 0; i < vector->len; i++)
    {
        printf(PrMyNum " ", vector->data[i]);
    }
    printf("\n");
}
int fill_matrix(matrix_t *normalize_matrix, double percent_matrix)
{
    int count_not_null = percent_matrix * normalize_matrix->rows * normalize_matrix->cols;
    int rand_row;
    int rand_col;
    int num;
    srand(time(NULL));
    while (count_not_null)
    {
        while (TRUE)
        {
            rand_row = rand() % normalize_matrix->rows;
            rand_col = rand() % normalize_matrix->cols;
            if (normalize_matrix->matrix[rand_row][rand_col] == 0)
            {
                num = (rand() % MAX_VAL) - MAX_VAL / 2;
                write_num(normalize_matrix, num, rand_row, rand_col);
                count_not_null--;
                break;
            }
        }
    }
    return EXIT_SUCCESS;
}
int fill_vector(vector_t *vector, double percent_vector)
{
    int count_not_null = percent_vector * vector->len;
    int rand_ind;
    int num;
    srand(time(NULL));
    while (count_not_null)
    {
        while (TRUE)
        {
            rand_ind = rand() % vector->len;
            if (vector->data[rand_ind] == 0)
            {
                num = (rand() % MAX_VAL) - MAX_VAL / 2;
                vector->data[rand_ind] = num;
                count_not_null--;
                break;
            }
        }
    }
    return EXIT_SUCCESS;
}
void print_sparse(sparse_matrix_t *sparse) {
    int count = 0;
    my_num_t *values_out = calloc(sparse->rows, sizeof(my_num_t));
    int *rows_out = calloc(sparse->rows, sizeof(int));


    for (int i = 0; i < sparse->rows; i++) {
        if (sparse->values[i] != (my_num_t)0) {
            values_out[count] = sparse->values[i];
            rows_out[count] = sparse->rows_ind[i];
            count++;
        }
    }
    free(sparse->values);
    free(sparse->rows_ind);

    sparse->values = values_out;
    sparse->rows_ind = rows_out;
    sparse->count = count; // спорно
    sparse->cols_index[1] = count;
    sparse->cols_index[0] = 0;
    int elems = sparse->count;
    if (!elems) {
        elems = sparse->rows;
        sparse->cols_index[1] = sparse->rows;
    }
    for (int i = 0; i < elems; i++) {
            printf(PrMyNum" ", sparse->values[i]);
    }
    printf("\n");
    for (int i = 0; i < elems; i++) {
        printf("%d ", sparse->rows_ind[i]);
    }
    printf("\n");
    for (int i = 0; i < sparse->cols + 1; i++) {
        printf("%d ", sparse->cols_index[i]);
    }
    printf("\n");

}
void get_multiply_time(matrix_t *normalize_matrix, matrix_t *vector_normalize, matrix_t * normalize_res, sparse_matrix_t *sparse_matrix, sparse_matrix_t *sparse_vector, sparse_matrix_t *sparse_res)
{
    // printf("Исходная матрица - множимое\n");
    // print_matrix(normalize_matrix);
    // printf("Вектор-столбец - множитель\n");
    // print_matrix(vector_normalize);

    struct timeval tv_start, tv_stop;
    int64_t time_n = 0, time_s = 0;

    for (int i = 0; i < COUNT; i++)
    {
        gettimeofday(&tv_start, NULL);
        multiply_normalize_on_vector(normalize_matrix, vector_normalize, normalize_res);
        gettimeofday(&tv_stop, NULL);
        time_n += (tv_stop.tv_sec - tv_start.tv_sec) * 1000000LL + (tv_stop.tv_usec - tv_start.tv_usec);
    }
    // printf("Результат умножения в количестве %d раз.\n", COUNT);
    // print_matrix(normalize_res);
    // printf("\n\n");
    
    for (int i = 0; i < COUNT; i++)
    {
        gettimeofday(&tv_start, NULL);
        multiply_sparse_on_sparse(sparse_matrix, sparse_vector, sparse_res);
        gettimeofday(&tv_stop, NULL);
        time_s += (tv_stop.tv_sec - tv_start.tv_sec) * 1000000LL + (tv_stop.tv_usec - tv_start.tv_usec);
    }
    // printf("Результат умножения в количестве %d раз.\n", COUNT);
    // print_sparse(sparse_res);
    // printf("\n\n");
    double res_n, res_s;
    res_n = time_n / (double)COUNT;
    res_s = time_s / (double)COUNT;
    size_t size_norm_matrix = 0;
    size_t size_norm_vector = 0;
    size_t size_sparse_matrix = 0;
    size_t size_sparse_vector = 0;


    get_size_normalize(&size_norm_matrix, normalize_matrix);
    get_size_normalize(&size_norm_vector, vector_normalize);
    get_size_sparse(&size_sparse_matrix, sparse_matrix);
    get_size_sparse(&size_sparse_vector, sparse_vector);


    printf("Размерность матрицы %d x %d.\n", normalize_matrix->rows, normalize_matrix->cols);
    printf("Умножение нормализованной матрицы на вектор выполнено за %lf.\n", res_n);
    printf("Размер обычной матрицы = %zu, обычного вектора = %zu, Сумма = %zu\n\n", size_norm_matrix, size_norm_vector, size_norm_vector + size_norm_matrix);
    printf("Умножение разреженной матрицы на вектор выполнено за %lf.\n", res_s);
    printf("Размер разреженной матрицы = %zu, разреженного вектора = %zu, Сумма = %zu\n", size_sparse_matrix, size_sparse_vector, size_sparse_matrix + size_sparse_vector);
    // printf("Нормальная форма результата:\n");
    // print_matrix(normalize_res);
    // printf("\n\nРазреженная форма:\n");
    // print_sparse(sparse_res);
}
void get_size_normalize(size_t *size, matrix_t *matrix) {
    size_t tmp = 0;
    tmp = tmp + sizeof(my_num_t) * matrix->rows * matrix->cols;
    tmp += sizeof(matrix->matrix);
    tmp += sizeof(matrix->rows);
    tmp += sizeof(matrix->cols);
    tmp += sizeof(matrix_t *);
    *size = tmp;
}
void get_size_sparse(size_t *size, sparse_matrix_t *sparse) {
    size_t tmp = 0;
    tmp += sizeof(sparse->count);
    tmp += sizeof(sparse->rows);
    tmp += sizeof(sparse->cols);
    tmp += sizeof(sparse->values);
    tmp += sizeof(sparse->rows_ind);
    tmp += sizeof(sparse->cols_index);
    tmp += sizeof(sparse_matrix_t *);
    tmp += sizeof(my_num_t) * sparse->count;
    tmp += sizeof(int) * sparse->count;
    tmp += sizeof(int) * (sparse->cols + 1);


    *size = tmp;
}
/**
*@brief Умножение разреженной матрицы на нормальный вектор
*
*@param sparse_matrix разреженная матрица
*@param vector вектор
*@param res результирующий массив длины vector
*@warning res - изначально заполнен нулями 
 */
void multiply_sparse_on_vector(sparse_matrix_t *sparse_matrix, vector_t *vector, vector_t *res)
{
    for (int i = 0; i < sparse_matrix->cols; i++)
    {
        for (int j = sparse_matrix->cols_index[i]; j < sparse_matrix->cols_index[i + 1]; j++)
        {
            res->data[sparse_matrix->rows_ind[j]] = res->data[sparse_matrix->rows_ind[j]] + sparse_matrix->values[j] * vector->data[i];
        }
    }
}
// void multiply_sparse_on_sparse(sparse_matrix_t *sparse_matrix, sparse_matrix_t *sparse_vector, sparse_matrix_t *sparse_res)
// {
//     for (int i = 0; i < sparse_matrix->cols; i++)
//     {
//         for (int j = sparse_matrix->cols_index[i]; j < sparse_matrix->cols_index[i + 1]; j++)
//         {
//             for (int k = sparse_vector->cols_index[0]; k < sparse_vector->cols_index[1]; k++)
//             {
//                 if (i == sparse_vector->rows_ind[k])
//                 {
//                     sparse_res->values[sparse_matrix->rows_ind[j]] = sparse_res->values[sparse_matrix->rows_ind[j]] + sparse_matrix->values[j] * sparse_vector->values[k];
//                     sparse_res->rows_ind[sparse_matrix->rows_ind[j]] = sparse_matrix->rows_ind[j];
//                 }
//             }
//         }
//     }
void multiply_sparse_on_sparse(sparse_matrix_t *sparse_matrix, sparse_matrix_t *sparse_vector, sparse_matrix_t *sparse_res)
{
    for (int i = 0; i < sparse_matrix->cols; i++)
    {
        for (int j = sparse_matrix->cols_index[i]; j < sparse_matrix->cols_index[i + 1]; j++)
        {
            for (int k = sparse_vector->cols_index[0]; k < sparse_vector->cols_index[1]; k++)
            {
                if (i == sparse_vector->rows_ind[k])
                {
                    sparse_res->values[sparse_matrix->rows_ind[j]] = sparse_res->values[sparse_matrix->rows_ind[j]] + sparse_matrix->values[j] * sparse_vector->values[k];
                    sparse_res->rows_ind[sparse_matrix->rows_ind[j]] = sparse_matrix->rows_ind[j];
                }
            }
        }
    }
    // printf("START res\n");
    // for (int i = 0; i < sparse_res->rows; i++)
    // {
    //     printf(PrMyNum " ", sparse_res->values[i]);
    // }
    // printf("\n");
    // for (int i = 0; i < sparse_res->rows; i++)
    // {
    //     printf("%d ", sparse_res->rows_ind[i]);
    // }
    // printf("\n");
    // for (int i = 0; i < 2; i++)
    // {
    //     printf("%d ", sparse_res->cols_index[i]);
    // }
    // printf("\n");
    // printf("FINISH res\n");

    // int count = 0;
    // my_num_t *values_out = calloc( sparse_res->rows, sizeof(my_num_t));
    // int *rows_out = calloc(sparse_res->rows, sizeof(int));

    // // my_num_t *val_inc = values_out;
    // // int *rows_inc = rows_out;

    // for (int i = 0; i < sparse_res->rows; i++) {
    //     if (sparse_res->values[i] != (my_num_t)0) {
    //         values_out[count] = sparse_res->values[i];
    //         rows_out[count] = sparse_res->rows_ind[i];
    //         count++;
    //     }
    // }
    // free(sparse_res->values);
    // free(sparse_res->rows_ind);

    // sparse_res->values = values_out;
    // sparse_res->rows_ind = rows_out;
    // sparse_res->count = count; // спорно
    // sparse_res->cols_index[1] = count;
    // printf("AFTER:\n");
    // for (int i = 0; i < sparse_res->rows; i++)
    // {
    //     printf(PrMyNum " ", sparse_res->values[i]);
    // }
    // printf("\n");
    // for (int i = 0; i < sparse_res->rows; i++)
    // {
    //     printf("%d ", sparse_res->rows_ind[i]);
    // }
    // printf("\n");
    // for (int i = 0; i < 2; i++)
    // {
    //     printf("%d ", sparse_res->cols_index[i]);
    // }
    // printf("\n");
}
int get_matrix_from_file(FILE *in, matrix_t **normalize_matrix, matrix_t **normalize_vector, sparse_matrix_t **sparse_matrix, sparse_matrix_t **sparse_vector) {
    int rows_matrix = 0;
    int cols_matrix = 0;
    int rows_vector = 0;

    if (fscanf(in, "%d %d", &rows_matrix, &cols_matrix) != 2) {
        printf("Не удалось считать размер матрицы.\n");
        return READ_ERR;
    }
    matrix_t *matrix = create_normalize_matrix();

    if (!matrix || initialize_normalize_matrix(matrix, rows_matrix, cols_matrix)) {
        printf("Не удалось создать матрицу\n");
        free(matrix);
        return ALLOCATION_ERR;
    }
    int matrix_not_null = 0;
    for (int i = 0; i < rows_matrix; i++) {
        for (int j = 0; j < cols_matrix; j++) {
            if (fscanf(in, ScMyNum, &matrix->matrix[i][j]) != 1) {
                printf("Не удалось считать число из файла.\n");
                free_matrix(matrix);
                return ALLOCATION_ERR;
            }
            if (matrix->matrix[i][j] != (my_num_t)0) {
                matrix_not_null++;
            }
        }
    }
    if (fscanf(in, "%d", &rows_vector) != 1) {
        printf("Не удалось считать размер вектора.\n");
        free_matrix(matrix);
        return ALLOCATION_ERR;
    }
    if (cols_matrix != rows_vector) {
        printf("Неверный размер вектора. Количество строк должно равняться кол-ву столбцов матрицы.\n");
        free_matrix(matrix);
        return ALLOCATION_ERR;
    }
    matrix_t *vector = create_normalize_matrix();
    
    if (!vector || initialize_normalize_matrix(vector, rows_vector, 1)) {
        printf("Не удалось создать вектор\n");
        free_matrix(matrix);
        free(vector);
        return ALLOCATION_ERR;
    }
    int vector_not_null = 0;
    for (int i = 0; i < rows_vector; i++) {
        if (fscanf(in, ScMyNum, &vector->matrix[i][0]) != 1) {
            printf("Не удалось считать элемент вектора.\n");
            free_matrix(matrix);
            free_matrix(vector);
            return ALLOCATION_ERR;
        }
        if (vector->matrix[i][0] != (my_num_t)0) {
            vector_not_null++;
        }
    }
    sparse_matrix_t *sparse_m = create_sparse_matrix();
    if (!sparse_m || initialize_sparse_matrix(sparse_m, rows_matrix, cols_matrix, matrix_not_null)) {
        printf("Не удалось создать разреженную матрицу.\n");
        free_matrix(matrix);
        free_matrix(vector);
        free(sparse_m);
        return ALLOCATION_ERR;
    }
    sparse_matrix_t *sparse_v = create_sparse_matrix();
    if (!sparse_v || initialize_sparse_matrix(sparse_v, rows_vector, 1, vector_not_null)) {
        printf("Не удалось создать разреженный вектор.\n");
        free_matrix(matrix);
        free_matrix(vector);
        free_sparse_matrix(sparse_m);
        free(sparse_v);
        return ALLOCATION_ERR;
    }

    normalize_to_sparse(matrix, sparse_m);
    normalize_to_sparse(vector, sparse_v);

    *normalize_matrix = matrix;
    *normalize_vector = vector;
    *sparse_matrix = sparse_m;
    *sparse_vector = sparse_v;

    return EXIT_SUCCESS;
}
