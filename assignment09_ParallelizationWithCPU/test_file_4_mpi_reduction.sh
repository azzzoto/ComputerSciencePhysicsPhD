#!/bin/bash

# Test script for MPI DAXPY + Reduction performance
export PATH=/usr/lib64/openmpi/bin:$PATH
export LD_LIBRARY_PATH=/usr/lib64/openmpi/lib:$LD_LIBRARY_PATH
export OMPI_ALLOW_RUN_AS_ROOT=1
export OMPI_ALLOW_RUN_AS_ROOT_CONFIRM=1

# MPI options for Docker compatibility
MPI_OPTS="--allow-run-as-root --mca pml ob1 --mca btl tcp,self --mca mtl ^ofi --oversubscribe"

echo "======================================================================="
echo "              MPI DAXPY + REDUCTION PERFORMANCE TEST"
echo "======================================================================="
echo "Testing program: 4_mpi_reduction"
echo "This program tests vector addition (d = x + y) with reduction (sum)"
echo "and compares different MPI reduction strategies:"
echo "  - Combined: DAXPY + reduction in single loop"
echo "  - Separate: DAXPY then reduction in separate steps"
echo "  - Allreduce: All processes get the final result"
echo ""

# Check if program exists
if [[ ! -f "./4_mpi_reduction" ]]; then
    echo "Program 4_mpi_reduction not found. Compiling..."
    make 4_mpi_reduction
    echo ""
fi

echo "1. Small vector test (100K elements, 2 processes):"
echo "   Command: mpirun -np 2 ./4_mpi_reduction 100000 2.0 3.0"
mpirun $MPI_OPTS -np 2 ./4_mpi_reduction 100000 2.0 3.0
echo ""
echo "-------------------------------------------------------------------"

echo "2. Medium vector test (1M elements, 2 processes):"
echo "   Command: mpirun -np 2 ./4_mpi_reduction 1000000 3.0 4.0"
mpirun $MPI_OPTS -np 2 ./4_mpi_reduction 1000000 3.0 4.0
echo ""
echo "-------------------------------------------------------------------"

echo "3. Medium vector test (1M elements, 4 processes):"
echo "   Command: mpirun -np 4 ./4_mpi_reduction 1000000 3.0 4.0"
mpirun $MPI_OPTS -np 4 ./4_mpi_reduction 1000000 3.0 4.0
echo ""
echo "-------------------------------------------------------------------"

echo "4. Large vector test (5M elements, 2 processes):"
echo "   Command: mpirun -np 2 ./4_mpi_reduction 5000000 2.5 3.5"
mpirun $MPI_OPTS -np 2 ./4_mpi_reduction 5000000 2.5 3.5
echo ""
echo "-------------------------------------------------------------------"

echo "5. Large vector test (5M elements, 4 processes):"
echo "   Command: mpirun -np 4 ./4_mpi_reduction 5000000 2.5 3.5"
mpirun $MPI_OPTS -np 4 ./4_mpi_reduction 5000000 2.5 3.5
echo ""
echo "-------------------------------------------------------------------"

echo "6. Very large vector test (10M elements, 4 processes):"
echo "   Command: mpirun -np 4 ./4_mpi_reduction 10000000 1.5 2.5"
mpirun $MPI_OPTS -np 4 ./4_mpi_reduction 10000000 1.5 2.5
echo ""
echo "-------------------------------------------------------------------"

echo "7. Scaling test (10M elements, 8 processes):"
echo "   Command: mpirun -np 8 ./4_mpi_reduction 10000000 1.0 2.0"
mpirun $MPI_OPTS -np 8 ./4_mpi_reduction 10000000 1.0 2.0
echo ""
echo "-------------------------------------------------------------------"

echo "8. Default parameters test (no arguments, 4 processes):"
echo "   Command: mpirun -np 4 ./4_mpi_reduction"
mpirun $MPI_OPTS -np 4 ./4_mpi_reduction
echo ""

echo "======================================================================="
echo "                        TEST COMPLETED"
echo "======================================================================="
echo ""
echo "ANALYSIS SUMMARY:"
echo "• This program demonstrates three MPI reduction strategies:"
echo "  1. Combined approach: Vector addition and reduction in single loop"
echo "  2. Separate approach: Vector addition, then separate reduction"
echo "  3. Allreduce approach: All processes receive the final sum"
echo ""
echo "• Key metrics to observe:"
echo "  - Speedup vs serial computation"
echo "  - Efficiency (speedup/number of processes)"
echo "  - Best performing approach for different vector sizes"
echo "  - Scaling behavior with increasing process count"
echo ""
echo "• Expected behavior:"
echo "  - Small vectors: Serial may be faster due to MPI overhead"
echo "  - Large vectors: MPI should show better performance"
echo "  - Separate approach often performs best due to better cache usage"
echo "  - Allreduce has communication overhead but all processes get result"
echo ""
echo "For comprehensive benchmarks of all programs, run: ./run_benchmarks.sh"
echo "For quick functionality test, run: ./quick_test.sh"
