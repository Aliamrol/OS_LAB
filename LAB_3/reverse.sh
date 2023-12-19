#!/bin/bash


read -p "enter the string: " number

reversed=""
length=${#number}
sum=0

for ((i = length-1; i >= 0; i--)); do
    el=${number:i:1}
    reversed="$reversed$el"
    sum=$(($el + $sum))
done

echo "reversed: $reversed"
echo "sum: $sum"