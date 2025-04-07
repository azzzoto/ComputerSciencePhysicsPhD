#!/bin/bash

# Clean and compile the C program
echo "-----------------------------"
echo "Cleaning and compiling C program..."
echo "-----------------------------"
make clean
make

# Check if Julia is installed
if ! command -v julia &> /dev/null; then
    echo "Error: Julia is not installed. Please install Julia first."
    exit 1
fi

# Check if DataFrames package is installed, if not install it
echo "Checking for required Julia packages..."
julia -e 'using Pkg; if !("DataFrames" in keys(Pkg.project().dependencies)) Pkg.add("DataFrames") end'
julia -e 'using Pkg; if !("Printf" in keys(Pkg.project().dependencies)) Pkg.add("Printf") end'

# Check number of arguments
if [ $# -lt 1 ]; then
    echo "Usage: $0 <N> [x_inf x_sup]"
    echo "  N: number of points"
    echo "  x_inf: lower integration limit (optional)"
    echo "  x_sup: upper integration limit (optional)"
    exit 1
fi

# Get arguments
N=$1
x_inf=${2:-0}  # Default to 0 if not provided
x_sup=${3:-1.5707963267948966}  # Default to pi/2 if not provided

# Compile and run C program
echo "-----------------------------"
echo "Running C program..."
echo "-----------------------------"
if [ $# -eq 1 ]; then
    ./compute_integral $N
else
    ./compute_integral $N $x_inf $x_sup
fi

# Check if C program ran successfully
if [ $? -ne 0 ]; then
    echo "-----------------------------"
    echo "Error: C program failed to run"
    echo "-----------------------------"
    exit 1
fi

# Run Julia script
echo "-----------------------------"
echo "Running Julia script..."
echo "-----------------------------"
julia compare_integrals.jl

# Check if Julia script ran successfully
if [ $? -ne 0 ]; then
    echo "Error: Julia script failed to run"
    exit 1
fi

echo "Integration completed successfully!"
echo "Results are saved in c_integration_results.dat and julia_integration_results.dat" 