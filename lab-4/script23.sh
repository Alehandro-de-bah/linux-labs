#!/bin/bash
echo -n "Введите число N: "
read N

middle=$((N / 2))
for ((i = 0; i < N; i++)); do
	for ((j = 0; j < N; j++)); do
		if [ $j -eq $((middle - i + 1)) ] || [ $j -eq $((middle + i - 1)) ] \
		|| [ $j -eq $((i - middle + 1)) ] || [ $j -eq $((3*middle - i - 1)) ] \
		|| [ $j -eq 0 ] || [ $j -eq $((N - 1)) ] \
		|| [ $i -eq 0 ] || [ $i -eq $((N - 1)) ]; then
			echo -n "* "
		else
			echo -n "  "
		fi
	done
	echo
done


