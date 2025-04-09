#!/bin/bash

# Clean and compile the C program
echo "============================="
echo "Cleaning and compiling C program..."
echo "============================="
make clean
make

echo "Running C Program..."
./vector_components_sum

# Get arguments
N=$1
a=${2:3.0}  # Default to 3.0 if not provided

# Check if Julia is installed
if ! command -v julia &> /dev/null; then
    echo "Error: Julia is not installed. Please install Julia first."
    exit 1
fi

# run the julia program
printf "\n"
echo "============================="
echo "Running Julia program..."
echo "============================="
if [ $# -eq 0 ]; then
    julia gaussian_daxpy.jl
else
    julia gaussian_daxpy.jl $N $a
fi

printf "\nC and Julia programs completed successfully!"
printf "\nResults of the Julia program are saved in daxpy_result.dat and results.dat\n" 