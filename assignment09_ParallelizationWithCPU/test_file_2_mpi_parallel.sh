#!/bin/bash

# Test script for MPI DAXPY performance
export PATH=/usr/lib64/openmpi/bin:$PATH
export LD_LIBRARY_PATH=/usr/lib64/openmpi/lib:$LD_LIBRARY_PATH
export OMPI_ALLOW_RUN_AS_ROOT=1
export OMPI_ALLOW_RUN_AS_ROOT_CONFIRM=1

# MPI options for Docker compatibility
MPI_OPTS="--allow-run-as-root --mca pml ob1 --mca btl tcp,self --mca mtl ^ofi --oversubscribe"

echo "=== MPI DAXPY Performance Test ==="
echo "Testing different configurations..."
echo ""

echo "1. Small vector (1M elements, 2 processes):"
mpirun $MPI_OPTS -np 2 ./2_mpi_parallel_daxpy 1000000 3.0 4.0
echo ""

echo "2. Medium vector (5M elements, 2 processes):"
mpirun $MPI_OPTS -np 2 ./2_mpi_parallel_daxpy 5000000 3.0 4.0
echo ""

echo "3. Medium vector (5M elements, 4 processes):"
mpirun $MPI_OPTS -np 4 ./2_mpi_parallel_daxpy 5000000 3.0 4.0
echo ""

echo "4. Large vector (10M elements, 2 processes):"
mpirun $MPI_OPTS -np 2 ./2_mpi_parallel_daxpy 10000000 3.0 4.0
echo ""

echo "5. Large vector (10M elements, 4 processes):"
mpirun $MPI_OPTS -np 4 ./2_mpi_parallel_daxpy 10000000 3.0 4.0
echo ""

echo "=== Test completed ==="
