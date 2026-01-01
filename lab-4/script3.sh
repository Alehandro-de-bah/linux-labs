#!/bin/bash
isNumber() {
	echo "$1" | grep -Eq '^[0-9]+\.?[0-9]*$'
}

if [ $(( $# % 2 )) -eq 0 ] || [ $# -lt 3 ]; then
	echo "Недостаточное количество аргументов"; exit 1
fi

if ! isNumber "$1"; then
	echo "Аргумент не является числом"; exit 1
fi

res=$1
shift 1
while [ $# -gt 1 ]; do
	if ! isNumber "$2"; then
		echo "Аргумент не является числом"; exit 1
	fi

	case $1 in
		+) res=$(echo "scale=6; $res + $2" | bc);;
		-) res=$(echo "scale=6; $res - $2" | bc);;
        	/) if [ "$(echo "$2 == 0" | bc)" = "1" ]; then
		   	echo "Ошибка: деление на ноль"
			exit 1
		   fi
		   res=$(echo "scale=6; $res / $2" | bc);;
		x|X) res=$(echo "scale=6; $res * $2" | bc);;
		*)
			echo "Неизвестная операция"
			exit 1
			;;
	esac
	shift 2
done
echo $res
