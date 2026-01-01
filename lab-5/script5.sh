#!/bin/bash

echo "Тестирование оптимизаций для файла: $1"

levels=(
	"-O0"
	"-Os"
	"-O1"
	"-O2"
	"-O3"
	"-O2 -march=native"
	"-O3 -march=native"
	"-O2 -march=native -funroll-loops"
	"-O3 -march=native -funroll-loops"
)

for level in "${levels[@]}"; do
	echo "================================="
	echo "Текущая опция оптимизации: $level"

	gcc $level $1 -lstdc++

	echo "Время выполнения:"
	time ./a.out

	echo "Размер исполняемого файла: "
	du -b a.out | cut -f1

	rm a.out
done

