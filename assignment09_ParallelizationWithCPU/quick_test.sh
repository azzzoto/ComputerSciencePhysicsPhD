#!/bin/bash

# =============================================================================
# Quick Test Script for Assignment 09: CPU Parallelization
# =============================================================================
# This script performs a quick test of all programs with basic configurations
# =============================================================================

# Colors for output
GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
NC='\033[0m'

# MPI configuration for Docker compatibility
export PATH=/usr/lib64/openmpi/bin:$PATH
export LD_LIBRARY_PATH=/usr/lib64/openmpi/lib:$LD_LIBRARY_PATH
export OMPI_ALLOW_RUN_AS_ROOT=1
export OMPI_ALLOW_RUN_AS_ROOT_CONFIRM=1

MPI_OPTS="--allow-run-as-root --mca pml ob1 --mca btl tcp,self --mca mtl ^ofi --oversubscribe"

echo -e "${BLUE}=============================================================================${NC}"
echo -e "${BLUE}                    QUICK TEST - CPU PARALLELIZATION                        ${NC}"
echo -e "${BLUE}=============================================================================${NC}"
echo ""

# Check if programs exist, compile if needed
if [[ ! -f "./1_openmp_parallel_daxpy" ]] || [[ ! -f "./4_mpi_reduction" ]]; then
    echo -e "${YELLOW}Compiling programs...${NC}"
    make all
    echo ""
fi

# Test 1: OpenMP Basic
echo -e "${GREEN}TEST 1: OpenMP Vector Addition${NC}"
echo "Running: ./1_openmp_parallel_daxpy 1000000 3.0 4.0"
./1_openmp_parallel_daxpy 1000000 3.0 4.0
echo ""

# Test 2: MPI Basic
echo -e "${GREEN}TEST 2: MPI Vector Addition${NC}"
echo "Running: mpirun -np 4 ./2_mpi_parallel_daxpy 1000000 3.0 4.0"
mpirun $MPI_OPTS -np 4 ./2_mpi_parallel_daxpy 1000000 3.0 4.0
echo ""

# Test 3: OpenMP Reduction
echo -e "${GREEN}TEST 3: OpenMP Reduction${NC}"
echo "Running: ./3_openmp_reduction 1000000 3.0 4.0"
export OMP_NUM_THREADS=4
./3_openmp_reduction 1000000 3.0 4.0
echo ""

# Test 4: MPI Reduction
echo -e "${GREEN}TEST 4: MPI Reduction${NC}"
echo "Running: mpirun -np 4 ./4_mpi_reduction 1000000 3.0 4.0"
mpirun $MPI_OPTS -np 4 ./4_mpi_reduction 1000000 3.0 4.0
echo ""

echo -e "${BLUE}=============================================================================${NC}"
echo -e "${GREEN}                              TESTS COMPLETED                               ${NC}"
echo -e "${BLUE}=============================================================================${NC}"
echo ""
echo "For comprehensive benchmarks, run: ./run_benchmarks.sh"
echo "For detailed documentation, see: README.md"
