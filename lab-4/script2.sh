#!/bin/sh
if [ $# -ne 3 ]; then
        echo "Ошибка: неверное количество аргументов"
        exit 1
elif [ "$2" = "/" ] && [ $(echo "$3 == 0" | bc) -eq 1 ]; then
        echo "Ошибка: деление на ноль"
        exit 1
fi

case $2 in
	+) res=`echo "scale=6; $1 + $3" | bc`;;
        -) res=`echo "scale=6; $1 - $3" | bc`;;
        /) res=`echo "scale=6; $1 / $3" | bc`;;
        x|X) res=`echo "scale=6; $1 * $3" | bc`;;
esac
echo $res

