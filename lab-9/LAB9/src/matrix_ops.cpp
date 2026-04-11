#include "../include/matrix_ops.h"
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <cstring>
#include <iostream>

#ifdef _OPENMP
#include <omp.h>
#endif

// Создание матрицы с выравниванием
Matrix* create_matrix(int rows, int cols)
{
    Matrix* m = (Matrix*)malloc(sizeof(Matrix));
    if (!m)
    {
        return nullptr;
    }

    m->rows = rows;
    m->cols = cols;
    
    // Выравнивание на 64 байта (для AVX-512)
    int ret = posix_memalign((void**)&m->data, 64, rows * cols * sizeof(float));
    if (ret != 0)
    {
        free(m);
        return nullptr;
    }

    memset(m->data, 0, rows * cols * sizeof(float));

    return m;
}

void free_matrix(Matrix* m)
{
    if (m)
    {
        free(m->data); // Сначала освобождаем данные
        free(m); // Затем освобождаем структуру
    }
}

// Инициализация случайными числами
void init_random(Matrix* m)
{
    srand(time(nullptr));
    for(int i = 0; i < m->rows * m->cols; i++)
    {
        m->data[i] = (float)(rand() % 100) / 10.0f;
    }
}

double get_time_ms()
{
    auto now = std::chrono::high_resolution_clock::now();
    auto duration = now.time_since_epoch();
    return std::chrono::duration<double, std::milli>(duration).count();
}

// 1. НАИВНАЯ РЕАЛИЗАЦИЯ (без оптимизаций)
void matrix_multiply_naive(const Matrix* A, const Matrix* B, Matrix* C)
{
    int n = A->rows;
    int m = A->cols;
    int p = B->cols;
    
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < p; j++)
        {
            float sum = 0;
            for(int k = 0; k < m; k++)
            {
                sum += A->data[i * m + k] * B->data[k * p + j];
            }
            C->data[i * p + j] = sum;
        }
    }
}

// 2. ОПТИМИЗИРОВАННАЯ ВЕРСИЯ (смена порядка циклов для кэша)
void matrix_multiply_optimized(const Matrix* A, const Matrix* B, Matrix* C)
{
    int n = A->rows;
    int m = A->cols;
    int p = B->cols;
    
    // Обнуляем результат
    memset(C->data, 0, n * p * sizeof(float));
    
    // ikj порядок для лучшего использования кэша
    for(int i = 0; i < n; i++)
    {
        for(int k = 0; k < m; k++)
        {
            float aik = A->data[i * m + k];
            for(int j = 0; j < p; j++)
            {
                C->data[i * p + j] += aik * B->data[k * p + j];
            }
        }
    }
}

// 3. ВЕКТОРИЗОВАННАЯ ВЕРСИЯ (с подсказками компилятору)
void matrix_multiply_vectorized(const Matrix* A, const Matrix* B, Matrix* C)
{
    int n = A->rows;
    int m = A->cols;
    int p = B->cols;
    
    memset(C->data, 0, n * p * sizeof(float));
    
    for(int i = 0; i < n; i++)
    {
        for(int k = 0; k < m; k++)
        {
            float aik = A->data[i * m + k];
            const float* b_row = &B->data[k * p];
            float* c_row = &C->data[i * p];
            
            #pragma omp simd  // явно требуем векторизацию (OpenMP)
            for(int j = 0; j < p; j++)
            {
                c_row[j] += aik * b_row[j];
            }
        }
    }
}

// 4. ВЕКТОРИЗАЦИЯ + ВЫРАВНИВАНИЕ
void matrix_multiply_aligned(const Matrix* A, const Matrix* B, Matrix* C)
{
    int n = A->rows;
    int m = A->cols;
    int p = B->cols;
    
    memset(C->data, 0, n * p * sizeof(float));
    
    for(int i = 0; i < n; i++) {
        for(int k = 0; k < m; k++) {
            float aik = A->data[i * m + k];
            const float* b_row = &B->data[k * p];
            float* c_row = &C->data[i * p];
            
            #pragma omp simd aligned(b_row, c_row: 64)
            for(int j = 0; j < p; j++)
            {
                c_row[j] += aik * b_row[j];
            }
        }
    }
}

// 5. МНОГОПОТОЧНАЯ ВЕРСИЯ (OpenMP)
void matrix_multiply_parallel(const Matrix* A, const Matrix* B, Matrix* C)
{
    int n = A->rows;
    int m = A->cols;
    int p = B->cols;
    
    #pragma omp parallel for
    for (int i = 0; i < n; i++)
    {
        for (int k = 0; k < m; k++)
        {
            float aik = A->data[i * m + k];
            #pragma omp simd
            for (int j = 0; j < p; j++)
            {
                C->data[i * p + j] += aik * B->data[k * p + j];
            }
        }
    }
}
