#ifndef MATRIX_OPS_H
#define MATRIX_OPS_H

#ifdef __cplusplus
extern "C" {
#endif

// Структура для хранения матрицы
typedef struct
{
    float* data;
    int rows;
    int cols;
} Matrix;

// Функции для работы с матрицами
Matrix* create_matrix(int rows, int cols);
void free_matrix(Matrix* m);

// Различные версии умножения матриц
void matrix_multiply_naive(const Matrix* A, const Matrix* B, Matrix* C);
void matrix_multiply_optimized(const Matrix* A, const Matrix* B, Matrix* C);
void matrix_multiply_vectorized(const Matrix* A, const Matrix* B, Matrix* C);
void matrix_multiply_aligned(const Matrix* A, const Matrix* B, Matrix* C);
void matrix_multiply_parallel(const Matrix* A, const Matrix* B, Matrix* C);

// Вспомогательные функции
void init_random(Matrix* m);
double get_time_ms();

#ifdef __cplusplus
}
#endif

#endif
