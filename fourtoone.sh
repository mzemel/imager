#!/bin/bash

i=$( ls $1 | wc -l )
k=1
for (( j=1; j<=i; j++)) do
	number=$(( $(($j + 3)) % 4))
	if [ $number -eq 0 ]; then
		#echo 1
		mv "$1/image$j.jpg" "$1/image$k.jpg"
		((k++))
	else
		#echo 0
		rm "$1/image$j.jpg"
	fi
done
