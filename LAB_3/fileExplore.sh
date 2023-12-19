#!/bin/bash

read -p "enter the path for the file: " filePath
fileContent=""

if [ -f "$filePath" ]; then
    fileContent=$(cat "$filePath")

else
    echo "the file does not exist"
    exit 0
fi

read -p "enter the starting index: " x
read -p "enter the ending index: " y

sed -n "$x,$y p" "./$filePath"