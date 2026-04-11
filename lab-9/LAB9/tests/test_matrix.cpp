#include "../include/matrix_ops.h"
#include <iostream>
#include <iomanip>

int main()
{
    const int N = 1024;  // Размер матрицы 1024×1024
    std::cout << "=== Тестирование умножения матриц " << N << "x" << N << " ===\n\n";
    
    // Создаём матрицы
    Matrix* A = create_matrix(N, N);
    Matrix* B = create_matrix(N, N);
    Matrix* C = create_matrix(N, N);
    
	// Инициализируем матрицы случайными числами
    init_random(A);
    init_random(B);
    
    double start = 0.0;
	double end = 0.0;
    
    std::cout << std::fixed << std::setprecision(3);

    // 1. Наивная реализация (без оптимизаций)
    start = get_time_ms();
    matrix_multiply_naive(A, B, C);
    end = get_time_ms();
    double time_naive = end - start;
    std::cout << "1. Наивная (без оптимизаций):\t" << time_naive << " ms\n";
    
    // 2. Оптимизированная версия (смена порядков циклов)
    start = get_time_ms();
    matrix_multiply_optimized(A, B, C);
    end = get_time_ms();
    double time_opt = end - start;
    std::cout << "2. Оптимизированная (кэш):\t" << time_opt << " ms\n";
    
    // 3. Векторизованная версия
    start = get_time_ms();
    matrix_multiply_vectorized(A, B, C);
    end = get_time_ms();
    double time_vec = end - start;
    std::cout << "3. Векторизованная:\t\t" << time_vec << " ms\n";
    
    // 4. Векторизованная версия с выравниванием
    start = get_time_ms();
    matrix_multiply_aligned(A, B, C);
    end = get_time_ms();
    double time_align = end - start;
    std::cout << "4. Векторизация + выравнивание:\t" << time_align << " ms\n";
    
    // 5. Многопоточная версия
    start = get_time_ms();
    matrix_multiply_parallel(A, B, C);
    end = get_time_ms();
    double time_par = end - start;
    std::cout << "5. Многопоточная (OpenMP):\t" << time_par << " ms\n";
    
    // Очистка
    free_matrix(A);
    free_matrix(B);
    free_matrix(C);
    
    return 0;
}
