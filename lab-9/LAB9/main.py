#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import ctypes
import tkinter as tk
from tkinter import ttk, messagebox
import threading
import time
import numpy as np
from matplotlib.figure import Figure
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg

# Загрузка динамической библиотеки
lib = ctypes.CDLL('./lib/libmatrix.so')

# Определение структур и функций C++ для Python
class Matrix(ctypes.Structure):
    _fields_ = [
        ("data", ctypes.POINTER(ctypes.c_float)),
        ("rows", ctypes.c_int),
        ("cols", ctypes.c_int)
    ]

# Настройка типов аргументов
lib.create_matrix.argtypes = [ctypes.c_int, ctypes.c_int]
lib.create_matrix.restype = ctypes.POINTER(Matrix)

lib.free_matrix.argtypes = [ctypes.POINTER(Matrix)]

lib.init_random.argtypes = [ctypes.POINTER(Matrix)]

lib.get_time_ms.argtypes = []
lib.get_time_ms.restype = ctypes.c_double

# Функции умножения
lib.matrix_multiply_naive.argtypes = [ctypes.POINTER(Matrix), ctypes.POINTER(Matrix), ctypes.POINTER(Matrix)]
lib.matrix_multiply_optimized.argtypes = [ctypes.POINTER(Matrix), ctypes.POINTER(Matrix), ctypes.POINTER(Matrix)]
lib.matrix_multiply_vectorized.argtypes = [ctypes.POINTER(Matrix), ctypes.POINTER(Matrix), ctypes.POINTER(Matrix)]
lib.matrix_multiply_aligned.argtypes = [ctypes.POINTER(Matrix), ctypes.POINTER(Matrix), ctypes.POINTER(Matrix)]
lib.matrix_multiply_parallel.argtypes = [ctypes.POINTER(Matrix), ctypes.POINTER(Matrix), ctypes.POINTER(Matrix)]

class MatrixMultiplicationApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Matrix Multiplication Benchmark - C++ Library + Python GUI")
        self.root.geometry("900x700")
        
        self.setup_ui()
        
    def setup_ui(self):
        # Основной фрейм
        main_frame = ttk.Frame(self.root, padding="10")
        main_frame.grid(row=0, column=0, sticky=(tk.W, tk.E, tk.N, tk.S))
        
        # Параметры матрицы
        ttk.Label(main_frame, text="Размер матрицы:").grid(row=0, column=0, sticky=tk.W)
        self.matrix_size = tk.StringVar(value="1024")
        size_combo = ttk.Combobox(main_frame, textvariable=self.matrix_size, 
                                   values=["512", "1024", "1536", "2048"])
        size_combo.grid(row=0, column=1, padx=5)
        
        # Кнопка запуска
        self.run_button = ttk.Button(main_frame, text="Запустить бенчмарк", 
                                     command=self.run_benchmark)
        self.run_button.grid(row=0, column=2, padx=20)
        
        # Прогресс-бар
        self.progress = ttk.Progressbar(main_frame, mode='indeterminate')
        self.progress.grid(row=1, column=0, columnspan=3, sticky=(tk.W, tk.E), pady=10)
        
        # Текстовый вывод
        self.text_output = tk.Text(main_frame, height=15, width=80)
        self.text_output.grid(row=2, column=0, columnspan=3, pady=10)
        
        # Скроллбар
        scrollbar = ttk.Scrollbar(main_frame, command=self.text_output.yview)
        scrollbar.grid(row=2, column=3, sticky=(tk.N, tk.S))
        self.text_output.config(yscrollcommand=scrollbar.set)
        
        # График
        self.figure = Figure(figsize=(8, 4), dpi=100)
        self.ax = self.figure.add_subplot(111)
        self.canvas = FigureCanvasTkAgg(self.figure, master=main_frame)
        self.canvas.get_tk_widget().grid(row=3, column=0, columnspan=3, pady=10)
        
    def log(self, message):
        """Вывод сообщения в текстовое поле"""
        self.text_output.insert(tk.END, message + "\n")
        self.text_output.see(tk.END)
        self.root.update()
        
    def run_benchmark(self):
        """Запуск бенчмарка в отдельном потоке"""
        self.run_button.config(state='disabled')
        self.progress.start()
        self.text_output.delete(1.0, tk.END)
        
        thread = threading.Thread(target=self.benchmark_thread)
        thread.start()
        
    def benchmark_thread(self):
        """Поток выполнения бенчмарка"""
        try:
            size = int(self.matrix_size.get())
            self.log(f"Запуск бенчмарка для матриц {size}×{size}")
            self.log("=" * 60)
            
            # Создание матриц через C++ библиотеку
            self.log("Создание матриц...")
            A = lib.create_matrix(size, size)
            B = lib.create_matrix(size, size)
            C = lib.create_matrix(size, size)
            
            self.log("Инициализация случайными числами...")
            lib.init_random(A)
            lib.init_random(B)
            
            results = []
            implementations = [
                ("Наивная (без оптимизаций)", lib.matrix_multiply_naive),
                ("Оптимизированная (кэш)", lib.matrix_multiply_optimized),
                ("Векторизованная", lib.matrix_multiply_vectorized),
                ("Векторизация + выравнивание", lib.matrix_multiply_aligned),
                ("Многопоточная (OpenMP)", lib.matrix_multiply_parallel)
            ]
            
            for name, func in implementations:
                self.log(f"\nТестирование: {name}")
                start_time = time.time()
                
                # Вызов C++ функции
                func(A, B, C)
                
                end_time = time.time()
                elapsed_ms = (end_time - start_time) * 1000
                results.append((name, elapsed_ms))
                self.log(f"  Время: {elapsed_ms:.2f} мс")
                
            # Очистка
            lib.free_matrix(A)
            lib.free_matrix(B)
            lib.free_matrix(C)
            
            # Вывод сводной таблицы
            self.log("\n" + "=" * 60)
            self.log("СВОДНАЯ ТАБЛИЦА РЕЗУЛЬТАТОВ")
            self.log("-" * 60)
            self.log(f"{'Метод':<35} {'Время (мс)':<15} {'Ускорение':<10}")
            self.log("-" * 60)
            
            baseline = results[0][1]  # наивная реализация
            for name, time_ms in results:
                speedup = baseline / time_ms
                self.log(f"{name:<35} {time_ms:<15.2f} {speedup:<10.2f}x")
            
            # Построение графика
            self.plot_results(results)
            
        except Exception as e:
            self.root.after(0, lambda: messagebox.showerror("Ошибка", str(e)))
        finally:
            self.root.after(0, self.benchmark_finished)
            
    def plot_results(self, results):
        """Построение графика результатов"""
        self.ax.clear()
        names = [r[0] for r in results]
        times = [r[1] for r in results]
        
        colors = ['#ff6b6b', '#4ecdc4', '#45b7d1', '#96ceb4', '#ffeaa7']
        bars = self.ax.bar(names, times, color=colors)
        self.ax.set_ylabel('Время выполнения (мс)')
        self.ax.set_title('Сравнение производительности методов умножения матриц')
        self.ax.set_xticklabels(names, rotation=45, ha='right')
        
        # Добавление значений на столбцы
        for bar, t in zip(bars, times):
            height = bar.get_height()
            self.ax.text(bar.get_x() + bar.get_width()/2., height,
                        f'{t:.0f}ms', ha='center', va='bottom')
        
        self.figure.tight_layout()
        self.canvas.draw()
        
    def benchmark_finished(self):
        """Завершение бенчмарка"""
        self.progress.stop()
        self.run_button.config(state='normal')
        self.log("\n✅ Бенчмарк завершён!")

if __name__ == "__main__":
    root = tk.Tk()
    app = MatrixMultiplicationApp(root)
    root.mainloop()
