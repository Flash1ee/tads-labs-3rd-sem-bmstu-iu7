#include <stdio.h>
#include <string.h>
#include "utils.h"
#include "types.h"
#include "retcodes.h"
#include "matrix.h"

int main()
{

    int mode = 0;

    while (TRUE)
    {
        welcome();
        if (scanf("%d", &mode) != 1 || (mode != EXIT && mode != MULTIPLY && mode != INPUT_FILE && mode != TEST))
        {
            printf("Такого режима нет в программе, повторите попытку...\n");
            return ARG_ERR;
        }
        switch (mode)
        {
        case EXIT:
        {
            printf("Спасибо за использования.\nДо скорых встреч.");
            return EXIT_SUCCESS;
        }
        case MULTIPLY:
        {
            int rows, cols;
            printf("Введите количество строк и столбцов матрицы");
            if (scanf("%d %d", &rows, &cols) != 2 || rows < 0 || cols < 0)
            {
                printf("Некорректный ввод\n");
                return READ_ERR;
            }
            printf("Введите количество ненулевых элементов: ");
            int count = 0;
            if (scanf("%d", &count) != 1 || count < 0 || count > cols * rows)
            {
                printf("Некорректный ввод\n");
                return READ_ERR;
            }

            matrix_t *normalize_matrix = create_normalize_matrix();
            if (!normalize_matrix)
            {
                printf("Не удалось создать обычную матрицу");
                return ALLOCATION_ERR;
            }
            int rc;
            rc = initialize_normalize_matrix(normalize_matrix, rows, cols);
            if (rc)
            {
                printf("Не удалось инициализировать матрицу\n");
                free(normalize_matrix);
                return ALLOCATION_ERR;
            }
            int count_unik = count;
            for (int i = 0; i < count; i++)
            {
                int r, c;
                printf("Введите номер строки (индексация с 1): ");
                rc = (scanf("%d", &r));
                printf("Введите номер столбца (индексация с 1): ");
                rc += (scanf("%d", &c));
                if (rc != 2 || r <= 0 || c <= 0 || r > rows || c > cols)
                {
                    printf("Введена несуществующая(ий) строка / столбец.\n");
                    free_matrix(normalize_matrix);
                    return READ_ERR;
                }
                printf("Введите значение: ");
                my_num_t value;
                rc = (scanf(ScMyNum, &value));
                if (rc != 1)
                {
                    printf("Неверный ввод числа.\n");
                    free_matrix(normalize_matrix);
                    return READ_ERR;
                }
                if (write_num(normalize_matrix, value, r - 1, c - 1) == -1)
                {
                    count_unik -= 1;
                }
                print_matrix(normalize_matrix);
            }
            // Ввёл обычную матрицу
            sparse_matrix_t *sparse_matrix = create_sparse_matrix();
            if (!sparse_matrix)
            {
                printf("Не удалось создать разреженную матрицу\n");
                free_matrix(normalize_matrix);
                return ALLOCATION_ERR;
            }

            rc = initialize_sparse_matrix(sparse_matrix, rows, cols, count_unik);
            if (rc)
            {
                printf("Не удалось создать разреженную матрицу.\n");
                free_matrix(normalize_matrix);
                free(sparse_matrix);
                return rc;
            }
            normalize_to_sparse(normalize_matrix, sparse_matrix);

            matrix_t *vector = create_normalize_matrix();
            if (!vector)
            {
                printf("Не удалось создать вектор\n");
                free_matrix(normalize_matrix);
                free_sparse_matrix(sparse_matrix);
                return ALLOCATION_ERR;
            }
            int vector_len = cols;
            rc = initialize_normalize_matrix(vector, vector_len, 1);
            if (rc)
            {
                free_matrix(normalize_matrix);
                free_sparse_matrix(sparse_matrix);
                free(vector);
                return ALLOCATION_ERR;
            }
            printf("По заданной матрице будет сформирован вектор из %d элементов\n", cols);
            count_unik = 0;
            count = 0;
            printf("Введите кол - во ненулеввых элементов вектора\n");
            printf("Допустимые значения: [0 : %d]\n", cols);
            if (scanf("%d", &count) != 1)
            {
                printf("Некорректное значение.\n");
                free_matrix(normalize_matrix);
                free_sparse_matrix(sparse_matrix);
                free_matrix(vector);
                break;
            }
            if (count < 0 || count > cols)
            {
                printf("Кол-во элементов не входит в диапазон [0 : %d].\n", cols);
                free_matrix(normalize_matrix);
                free_sparse_matrix(sparse_matrix);
                free_matrix(vector);
                break;
            }
            count_unik = count;
            for (int i = 0; i < count; i++)
            {
                int r;
                printf("Введите номер строки (индексация с 1): ");
                rc = (scanf("%d", &r));
                if (rc != 1 || r <= 0 || r > rows)
                {
                    printf("Введена несуществующая строка.\n");
                    free_matrix(normalize_matrix);
                    free_sparse_matrix(sparse_matrix);
                    free_matrix(vector);
                    return READ_ERR;
                }
                printf("Введите значение: ");
                my_num_t value;
                rc = (scanf(ScMyNum, &value));
                if (rc != 1)
                {
                    printf("Неверный ввод числа.\n");
                    free_matrix(normalize_matrix);
                    free_sparse_matrix(sparse_matrix);
                    free_matrix(vector);
                    return READ_ERR;
                }
                if (write_num(vector, value, r - 1, 0) == -1)
                {
                    count_unik -= 1;
                }
                print_matrix(vector);
            }
            // for (int i = 0; i < cols; i++)
            // {
            //     my_num_t tmp;
            //     if (scanf(ScMyNum, &tmp) != 1)
            //     {
            //         printf("Неверный ввод числа.\n");
            //         free_matrix(normalize_matrix);
            //         free_sparse_matrix(sparse_matrix);
            //         free_matrix(vector);
            //         return ALLOCATION_ERR;
            //     }
            //     vector->matrix[i][0] = tmp;
            //     if (tmp != 0)
            //     {
            //         count_unik++;
            //     }
            // }
            printf("Вектор заполнен.\n");
            if (!count_unik)
            {
                printf("Результат умножения.\n");
                for (int i = 0; i < rows; i++)
                {
                    printf("%lf ", (my_num_t)ZERO);
                }
                printf("\n");
                free_matrix(normalize_matrix);
                free_sparse_matrix(sparse_matrix);
                free_matrix(vector);
                break;
            }
            sparse_matrix_t *sparse_vector = create_sparse_matrix();
            if (!sparse_vector)
            {
                printf("Не удалось создать вектор\n");
                free_matrix(normalize_matrix);
                free_sparse_matrix(sparse_matrix);
                free_matrix(vector);
                return ALLOCATION_ERR;
            }
            if (initialize_sparse_matrix(sparse_vector, rows, 1, count_unik))
            {
                free_matrix(normalize_matrix);
                free_sparse_matrix(sparse_matrix);
                free_matrix(vector);
                free(sparse_vector);
                return ALLOCATION_ERR;
            }
            normalize_to_sparse(vector, sparse_vector);
            sparse_matrix_t *sparse_res = create_sparse_matrix();
            if (!sparse_res)
            {
                printf("Не удалось создать вектор\n");
                free_matrix(normalize_matrix);
                free_sparse_matrix(sparse_matrix);
                free_matrix(vector);
                free_sparse_matrix(sparse_vector);
                return ALLOCATION_ERR;
            }
            rc = initialize_sparse_matrix(sparse_res, cols, 1, cols);
            if (rc)
            {
                free_matrix(normalize_matrix);
                free_matrix(normalize_matrix);
                free_sparse_matrix(sparse_matrix);
                free_matrix(vector);
                free_sparse_matrix(sparse_vector);
                free(sparse_res);
                return ALLOCATION_ERR;
            }
            print_matrix(normalize_matrix);
            printf("\n");
            print_matrix(vector);
            printf("\n");
            multiply_sparse_on_sparse(sparse_matrix, sparse_vector, sparse_res);
            // multiply_sparse_on_vector(sparse_matrix, vector, res);
            printf("Результат умножения разреженной матрицы на вектор-столбец\n");
            print_sparse(sparse_res);
            // print_vector(res);
            // for (int i = 0; i < res->len; i++)
            // {
            //     res->data[i] = 0;
            // }
            // multiply_normalize_on_vector(normalize_matrix, vector, res);
            // printf("\nРезультат умножения нормализованной матрицы на вектор-столбец\n");
            // print_vector(res);

            free_matrix(normalize_matrix);
            free_sparse_matrix(sparse_matrix);
            free_matrix(vector);
            free_sparse_matrix(sparse_vector);
            free_sparse_matrix(sparse_res);

            // free_vector(res);
            break;
        }
        case INPUT_FILE:
        {
            int c;
            while ((c = getchar()) != '\n')
            {
            }
            printf("Формат файла:\nПервая строка - размер матрицы в виде двух чисел (кол-во строк кол-во столбцов).\n");
            printf("Далее сама матрица, затем кол-во элементов вектор-столбца в одной строке.\n");
            printf("Начиная со следующей строки - элементы вектор-столбца.\n");
            printf("Введите название файла.(Не более 32 символов)\n");
            char filename[32 + 3] = {'\0'};
            if (!fgets(filename, 32 + 3, stdin) || strlen(filename) > 32 + 1)
            {
                printf("Введено много символов. Ошибка.\n");
                break;
            }
            filename[strlen(filename) - 1] = '\0';
            FILE *file = fopen(filename, "r");
            if (!file)
            {
                printf("Не удалось открыть файл.\n");
                break;
            }
            matrix_t *normalize_matrix = NULL;
            matrix_t *normalize_vector = NULL;
            sparse_matrix_t *sparse_matrix = NULL;
            sparse_matrix_t *sparse_vector = NULL;

            sparse_matrix_t *sparse_res = NULL;

            int rc = get_matrix_from_file(file, &normalize_matrix, &normalize_vector, &sparse_matrix, &sparse_vector);
            if (rc)
            {
                fclose(file);
                break;
            }
            sparse_res = create_sparse_matrix();
            if (!sparse_res)
            {
                printf("Не удалось создать результирующий вектор.\n");
                free_matrix(normalize_matrix);
                free_matrix(normalize_vector);
                free_sparse_matrix(sparse_matrix);
                free(sparse_res);
                fclose(file);
                break;
            }
            initialize_sparse_matrix(sparse_res, normalize_matrix->rows, 1, normalize_matrix->rows);
            print_sparse(sparse_res);
            multiply_sparse_on_sparse(sparse_matrix, sparse_vector, sparse_res);
            printf("Результат умножения:\n");
            print_sparse(sparse_res);
            printf("Разреженность матрицы в процентах = %lf\n", ((double)sparse_matrix->count / (normalize_matrix->rows * normalize_matrix->cols) * 100.));
            printf("Разреженность вектор-столбца в процентах = %lf\n", ((double)sparse_vector->count / (normalize_vector->cols * normalize_vector->rows) * 100));
            free_matrix(normalize_matrix);
            free_matrix(normalize_vector);
            free_sparse_matrix(sparse_matrix);
            free_sparse_matrix(sparse_vector);
            free_sparse_matrix(sparse_res);
            fclose(file);
            break;
        }
        case TEST:
        {
            int rows, cols;
            printf("Введите количество строк и столбцов матрицы: ");
            if (scanf("%d %d", &rows, &cols) != 2 || rows < 0 || cols < 0)
            {
                printf("Некорректный ввод\n");
                return READ_ERR;
            }
            int percent_matrix, percent_vector;
            printf("Введите процент разреженности матрицы в виде целого числа [0:100]\n");
            if (scanf("%d", &percent_matrix) != 1 || percent_matrix < 0 || percent_matrix > 100)
            {
                printf("Введен некорректный процент.\n");
                break;
            }
            printf("Введите процент разреженности вектор-столбца в виде целого числа [0:100]\n");
            if (scanf("%d", &percent_vector) != 1 || percent_vector < 0 || percent_vector > 100)
            {
                printf("Введен некорректный процент.\n");
                break;
            }
            double rarefaction_matrix = percent_matrix / 100.;
            double rarefaction_vector = percent_vector / 100.;

            matrix_t *normalize_matrix = create_normalize_matrix();
            if (!normalize_matrix)
            {
                printf("Не удалось создать нормализованную матрицу\n");
                return ALLOCATION_ERR;
            }
            if (initialize_normalize_matrix(normalize_matrix, rows, cols))
            {
                printf("Не удалось инициализировать нормализованную матрицу.\n");
                free(normalize_matrix);
                return ALLOCATION_ERR;
            }
            matrix_t *normalize_vector = create_normalize_matrix();
            if (!normalize_vector)
            {
                printf("Не удалось создать нормализованный вектор\n");
                free_matrix(normalize_matrix);
                return ALLOCATION_ERR;
            }
            if (initialize_normalize_matrix(normalize_vector, cols, 1))
            {
                printf("Не удалось инициализировать нормализованный вектор\n");
                free_matrix(normalize_matrix);
                free(normalize_vector);
                return ALLOCATION_ERR;
            }
            fill_matrix(normalize_matrix, rarefaction_matrix);
            fill_matrix(normalize_vector, rarefaction_vector);

            // print_matrix(normalize_matrix);
            // print_matrix(normalize_vector);

            sparse_matrix_t *sparse_matrix = create_sparse_matrix();
            if (!sparse_matrix)
            {
                printf("Не удалось создать разреженную матрицу\n");
                free_matrix(normalize_matrix);
                free_matrix(normalize_vector);
                return ALLOCATION_ERR;
            }
            int not_null = rarefaction_matrix * rows * cols;
            if (initialize_sparse_matrix(sparse_matrix, rows, cols, not_null))
            {
                printf("Не удалось инициализировать разреженную матрицу.\n");
                free_matrix(normalize_matrix);
                free_matrix(normalize_vector);
                free(sparse_matrix);
                return ALLOCATION_ERR;
            }
            normalize_to_sparse(normalize_matrix, sparse_matrix);
            sparse_matrix_t *sparse_vector = create_sparse_matrix();
            if (!sparse_vector)
            {
                printf("Не удалось создать разреженный вектор\n");
                free_matrix(normalize_matrix);
                free_matrix(normalize_vector);
                free_sparse_matrix(sparse_matrix);
                return ALLOCATION_ERR;
            }
            int not_null_vector = rarefaction_vector * cols;
            if (initialize_sparse_matrix(sparse_vector, cols, 1, not_null_vector))
            {
                printf("Не удалось инициализировать разреженный вектор.\n");
                free_matrix(normalize_matrix);
                free_sparse_matrix(sparse_matrix);
                free_matrix(normalize_vector);
                free(sparse_vector);
                return ALLOCATION_ERR;
            }
            normalize_to_sparse(normalize_vector, sparse_vector);

            sparse_matrix_t *sparse_res = create_sparse_matrix();
            if (!sparse_res)
            {
                printf("Не удалось создать вектор\n");
                free_matrix(normalize_matrix);
                free_sparse_matrix(sparse_matrix);
                free_matrix(normalize_vector);
                free(sparse_vector);
                return ALLOCATION_ERR;
            }
            matrix_t *normalize_res = create_normalize_matrix();
            if (!normalize_res)
            {
                printf("Не удалось создать вектор\n");
                free_matrix(normalize_matrix);
                free_sparse_matrix(sparse_matrix);
                free_matrix(normalize_vector);
                free(sparse_vector);
                free(sparse_res);
                return ALLOCATION_ERR;
            }
            if (initialize_sparse_matrix(sparse_res, rows, 1, rows))
            {
                free_matrix(normalize_matrix);
                free_sparse_matrix(sparse_matrix);
                free_matrix(normalize_vector);
                free(sparse_vector);
                free(sparse_res);
                return ALLOCATION_ERR;
            }
            if (initialize_normalize_matrix(normalize_res, rows, 1))
            {
                printf("Не удалось инициализировать вектор");
                free_matrix(normalize_matrix);
                free_sparse_matrix(sparse_matrix);
                free_matrix(normalize_vector);
                free(sparse_vector);
                free_sparse_matrix(sparse_res);
                return ALLOCATION_ERR;
            }
            // printf("SPARSE\n");
            // print_sparse(sparse_matrix);
            // printf("\n");
            // print_sparse(sparse_vector);
            // printf("\n");
            get_multiply_time(normalize_matrix, normalize_vector, normalize_res, sparse_matrix, sparse_vector, sparse_res);
            // printf("RESULT:\n");
            // print_matrix(normalize_res);
            // print_sparse(sparse_res);
            printf("Разреженность множителя %d, вектор - столбца %d\n", percent_matrix, percent_vector);
            break;
        }
        }
    }

    return EXIT_SUCCESS;
}