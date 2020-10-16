#ifndef _TYPES_H_
#define _TYPES_H_


#define TRUE 1
#define FALSE 0

#define ScMyNum "%lf"
#define PrMyNum "%lf"

typedef double my_num_t;

typedef struct 
{
    int rows;
    int cols;
    int count;
    my_num_t *values;
    int *rows_ind;
    int *cols_index;
} sparse_matrix_t;

typedef struct 
{
    int rows;
    int cols;
    my_num_t **matrix;
} matrix_t;

typedef struct types
{
    int len;
    int capacity;
    my_num_t *data;
} vector_t;



/**
*@brief Создание разреженной матрицы
*
*@param matrix разреженная матрица
*@param rows количество строк
*@param cols количество столбцов
*@return int код ошибки
 */
vector_t *create_vector();
matrix_t *create_normalize_matrix();
sparse_matrix_t *create_sparse_matrix();
int initialize_vector(vector_t *vector, int len);
int initialize_sparse_matrix(sparse_matrix_t *src, int rows, int cols, int count_not_null);
int initialize_normalize_matrix(matrix_t *src, int rows, int cols);
void normalize_to_sparse(matrix_t *normalize_matrix, sparse_matrix_t *sparse_matrix);
void sparse_to_normalize(sparse_matrix_t *sparse_matrix, matrix_t *normalize_matrix);
void multiply_sparse_on_vector(sparse_matrix_t *sparse_matrix, vector_t *vector, vector_t *res);
void free_arr(my_num_t **arr, int rows);
void free_matrix(matrix_t *matrix);
void free_sparse_matrix(sparse_matrix_t *sparse_matrix);
void free_vector(vector_t *vector);
int write_num(matrix_t *matrix, my_num_t value, int row, int col);
void print_matrix(matrix_t *matrix);
void print_vector(vector_t *vector);


#endif  //  _TYPES_H_