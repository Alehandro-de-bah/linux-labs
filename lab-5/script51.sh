#!/bin/bash

levels=(
        "-O3 -fipa-pta -flto"
        "-O3 -fipa-bit-cp -flto"
        "-O3 -fipa-vrp -flto"
        "-O3 -fipa-icf -flto"
	"-O3 -fprofile-generate"
	"-O3 -fprofile-use"
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

