CC = gcc
FLAGS = -Wall -Wextra -O2 -fopenmp
	
CC_MPI = mpicc
FLAGS_MPI = -Wall -Wextra -O2

.PHONY: all clean help test

all: 1_openmp_parallel_daxpy 2_mpi_parallel_daxpy 3_openmp_reduction 4_mpi_reduction

1_openmp_parallel_daxpy: 1_openmp_parallel_daxpy.c
	$(CC) $(FLAGS) -o 1_openmp_parallel_daxpy 1_openmp_parallel_daxpy.c

2_mpi_parallel_daxpy: 2_mpi_parallel_daxpy.c
	$(CC_MPI) $(FLAGS_MPI) -o 2_mpi_parallel_daxpy 2_mpi_parallel_daxpy.c

3_openmp_reduction: 3_openmp_reduction.c
	$(CC) $(FLAGS) -o 3_openmp_reduction 3_openmp_reduction.c

4_mpi_reduction: 4_mpi_reduction.c
	$(CC_MPI) $(FLAGS_MPI) -o 4_mpi_reduction 4_mpi_reduction.c

clean:
	rm -f 1_openmp_parallel_daxpy 2_mpi_parallel_daxpy 3_openmp_reduction 4_mpi_reduction *.o *.out
	rm -r results

test: all
	@echo "Running quick functionality test..."
	@./quick_test.sh

help:
	@echo "Assignment 09: CPU Parallelization"
	@echo "=================================="
	@echo "Available targets:"
	@echo "  all     - Compile all programs"
	@echo "  clean   - Remove compiled files"
	@echo "  test    - Run quick functionality test"
	@echo "  help    - Show this help message"
	@echo ""
	@echo "Programs:"
	@echo "  1_openmp_parallel_daxpy - OpenMP vector addition"
	@echo "  2_mpi_parallel_daxpy    - MPI vector addition"
	@echo "  3_openmp_reduction      - OpenMP reduction operations"
	@echo "  4_mpi_reduction         - MPI reduction operations"
	@echo ""
	@echo "Usage:"
	@echo "  source mpi_env.sh              # Setup MPI environment"
	@echo "  make all && ./quick_test.sh    # Compile and test"
	@echo "  ./run_benchmarks.sh            # Full performance analysis"
	@echo "  ./test_file_2_mpi_parallel.sh                  # MPI vector addition tests"
	@echo "  ./test_file_4_mpi_reduction.sh        # MPI reduction tests"
	@echo "  less README.md                 # Comprehensive documentation"