#!/bin/bash
read -p "enter the first number" first
read -p "enter the second number" second

# check inputs using regex
if ! [[ $first =~ ^[0-9]+$ ]]; then
	echo "incorrect input for first"
	exit 0
fi

if ! [[ $second =~ ^[0-9]+$ ]]; then
	echo "incorrect input for second"
	exit 0
fi


echo $(expr $first + $second)

if [ $first -gt $second ]; then
	echo 'first number is greater'
else
	echo 'second number is greater'
fi



