CXX = g++
CXXFLAGS = -Wall -O2 -march=native -fopenmp -fPIC
INCLUDES = -I./include

# Флаги для диагностики векторизации (только для .cpp файла)
VECFLAGS = -fopt-info-vec-optimized -fopt-info-vec-missed

all: lib/libmatrix.so tests/test_matrix

# Динамическая библиотека
lib/libmatrix.so: src/matrix_ops.cpp | lib
	$(CXX) $(CXXFLAGS) -shared $(INCLUDES) $< -o $@

# Версия с диагностикой векторизации (для анализа)
lib/libmatrix_vecinfo.so: src/matrix_ops.cpp | lib
	$(CXX) $(CXXFLAGS) $(VECFLAGS) -shared $(INCLUDES) $< -o $@

# Тестовая программа
tests/test_matrix: tests/test_matrix.cpp src/matrix_ops.cpp | tests
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@

# Очистка
clean:
	rm -rf lib/*.so tests/test_matrix

# Запуск тестов
run: all
	LD_LIBRARY_PATH=./lib:$$LD_LIBRARY_PATH ./tests/test_matrix

.PHONY: all clean run
