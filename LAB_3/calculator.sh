#!/bin/bash

read -p "Number of operands: " operand_numbers

# Initialize arrays for operands and operators
operands=()
operators=()

echo "Enter operands:"

# Read operands
for ((i = 0; i < operand_numbers; i++)); do
    read element
    operands+=("$element")
done

echo "Enter operators:" 
# Read operators
for ((i = 1; i < operand_numbers; i++)); do
    read element
    operators+=("$element")
done

# Initialize the result with the first operand
eval_="${operands[0]}"

# Perform the calculations based on the operators
for ((i = 1; i < operand_numbers; i++)); do
    operator="${operators[i-1]}"
    operand="${operands[i]}"
    
    case "$operator" in 
        "+")
            eval_=$(($eval_ + $operand))
            ;;
        "-")
            eval_=$(($eval_ - $operand))
            ;;
        "*")
            eval_=$(($eval_ * $operand))
            ;;
        "/")
            eval_=$(($eval_ / $operand))
            ;;
    esac
done

echo "Result: $eval_"
