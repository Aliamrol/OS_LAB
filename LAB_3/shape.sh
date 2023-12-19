#!/bin/bash

read -p "enter an integer from 1 to 3" shape


case $shape in
    1)
        for ((i = 0; i <= 5; i++)); do
            for ((j = 0; j < i; j++)); do
                echo -n "$i "
            done
            echo ""
        done
        exit 0
    ;;
    2)
        for ((i = 0; i < 5; i++)); do
            for ((j = 5-$i; j >= 0; j--)); do
                echo -n " "
            done
            for ((k = $i; k >=0; k--)); do
                if [ k != 0 ]; then
                    echo -n ". " 
                else
                    echo -n "."
                fi
            done
            for ((j = 5-i; j >= 0; j--)); do
                echo -n " "
            done
            echo ""
        done
        for ((i = 5; i >= 0; i--)); do
            for ((j = 5-$i; j >= 0; j--)); do
                echo -n " "
            done
            for ((k = $i; k >=0; k--)); do
                if [ k != 0 ]; then
                    echo -n ". " 
                else
                    echo -n "."
                fi
            done
            for ((j = 5-i; j >= 0; j--)); do
                echo -n " "
            done
            echo ""
        done
    ;;
    3)
        for ((i = 0; i < 5; i++)); do
            for ((j = 0; j <= i-1; j++)); do
                echo -n "| "
            done
            echo "|_"
        done
    ;;

esac
