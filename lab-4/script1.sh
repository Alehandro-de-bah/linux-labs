#!/bin/sh
if [ $# -ne 3 ]; then
	echo "Ошибка: неверное количество аргументов"
	exit 1
elif [ "$2" = "/" ] && [ $3 -eq 0 ]; then
	echo "Ошибка: деление на ноль"
	exit 1
fi

case $2 in
	+) echo $(($1 + $3));;
	-) echo $(($1 - $3));;
	/) echo $(($1 / $3));;
	x|X) echo $(($1 * $3));;
esac

