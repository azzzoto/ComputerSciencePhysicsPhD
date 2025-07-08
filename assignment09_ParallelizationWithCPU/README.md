# Assignment 09: CPU Parallelization with OpenMP and MPI

This assignment implements and compares vector addition operations and reductions using serial, OpenMP, and MPI approaches. The code demonstrates different parallelization strategies and their performance characteristics in various scenarios.

## Table of Contents

1. [Quick Start Guide](#quick-start-guide)
2. [Overview](#overview)
3. [Project Structure](#project-structure)
4. [Programs Description](#programs-description)
5. [Prerequisites](#prerequisites)
6. [Compilation](#compilation)
7. [Usage](#usage)
8. [Docker Environment Issues](#docker-environment-issues)
9. [Performance Analysis](#performance-analysis)
10. [Results Summary](#results-summary)
11. [Tips for Best Results](#tips-for-best-results)
12. [Troubleshooting](#troubleshooting)

---

## Quick Start Guide

### Prerequisites

Install OpenMPI and configure environment:
```bash
# Install required packages
dnf install -y gcc openmpi openmpi-devel

# Configure MPI environment
source mpi_env.sh
```

### Compile and Test

```bash
# 1. Compile all programs
make all

# 2. Run basic functionality test (recommended first)
./quick_test.sh

# 3. For comprehensive performance analysis
./run_benchmarks.sh
```

**Current system:** 8 CPU cores available

### Individual Program Quick Examples

```bash
# 1. OpenMP Vector Addition
./1_openmp_parallel_daxpy 1000000 3.0 4.0

# 2. MPI Vector Addition  
mpirun -np 4 ./2_mpi_parallel_daxpy 1000000 3.0 4.0

# 3. OpenMP Reduction
./3_openmp_reduction 1000000 3.0 4.0

# 4. MPI Reduction
mpirun -np 4 ./4_mpi_reduction 1000000 3.0 4.0
```

### Docker Compatibility
If running in Docker and encountering 'Bus error', use these MPI flags 

```bash
--allow-run-as-root --mca pml ob1 --mca btl tcp,self --mca mtl ^ofi --oversubscribe
```

> if command `source mpi_env` was entered (as described in [Quick Start Guide](#quick-start-guide)) this should be automatic

### Specialized Test Scripts

For more detailed testing of specific programs - MPI only because it was harder writing coding those files ;) 

```bash
# Test MPI vector addition with various configurations
./test_file_2_mpi_parallel.sh

# Test MPI reduction with detailed performance analysis
./test_file_4_mpi_reduction.sh
```

---

## Overview

This project contains four main programs that implement vector addition (`d = x + y`) and reduction operations (computing the sum of all elements in the result vector):

1. **1_openmp_parallel_daxpy**: Basic OpenMP parallelization of vector addition
2. **2_mpi_parallel_daxpy**: MPI parallelization of vector addition with performance comparison
3. **3_openmp_reduction**: OpenMP implementation with vector addition and reduction operations
4. **4_mpi_reduction**: MPI implementation with vector addition and reduction operations

### What Each Program Demonstrates

- **Program 1**: Basic OpenMP parallelization vs serial execution
- **Program 2**: MPI distributed computing with performance metrics
- **Program 3**: OpenMP reduction operations (combined vs separate phases)
- **Program 4**: MPI reduction strategies (Reduce vs Allreduce)

### Performance Metrics Measured

- Execution time (serial vs parallel)
- Speedup ratio
- Parallel efficiency
- Correctness validation

## Project Structure

```
Assignment 09: CPU Parallelization
├── 1_openmp_parallel_daxpy.c    # Basic OpenMP vector addition
├── 2_mpi_parallel_daxpy.c       # MPI vector addition with performance analysis
├── 3_openmp_reduction.c         # OpenMP vector addition + reduction
├── 4_mpi_reduction.c            # MPI vector addition + reduction
├── Makefile                     # Compilation configuration
├── README.md                    # This comprehensive documentation
├── mpi_env.sh                   # MPI environment configuration
├── run_benchmarks.sh            # Full benchmark suite
├── quick_test.sh                # Quick functionality test
├── test_file_2_mpi_parallel.sh                  # MPI vector addition tests
└── test_file_4_mpi_reduction.sh        # MPI reduction performance tests
```



## Programs Description

### 1. OpenMP Parallel Vector Addition (`1_openmp_parallel_daxpy.c`)

**Purpose**: Compares serial and OpenMP parallel implementations of vector addition.

**Features**:
- Serial baseline implementation
- OpenMP parallel loop with `#pragma omp parallel for`
- Performance comparison between serial and parallel approaches
- Memory allocation and validation
- Configurable vector size and values

**Key Implementation Details**:
- Uses `clock()` for serial timing and `omp_get_wtime()` for OpenMP timing
- Simple parallel loop without reduction
- Direct comparison of execution times

### 2. MPI Parallel Vector Addition (`2_mpi_parallel_daxpy.c`)

**Purpose**: Implements distributed memory parallelization using MPI.

**Features**:
- Work distribution across multiple MPI processes
- Load balancing for uneven work distribution
- Result gathering using `MPI_Gatherv`
- Comprehensive performance analysis
- Error checking and validation

**Key Implementation Details**:
- Dynamic work distribution: `local_n = N / size` with remainder handling
- Each process handles `local_n` elements
- Process 0 performs serial comparison and result validation
- Uses `MPI_Barrier` for synchronized timing
- Detailed speedup and efficiency calculations

### 3. OpenMP Reduction (`3_openmp_reduction.c`)

**Purpose**: Demonstrates OpenMP reduction operations combined with vector addition.

**Features**:
- Combined vector addition and reduction in single parallel region
- Separate vector addition and reduction phases
- Performance comparison between approaches
- Comprehensive verification of results
- Efficiency analysis with multiple threads

**Key Implementation Details**:
- `#pragma omp parallel for reduction(+:sum)` for combined approach
- Two separate OpenMP regions for comparison
- Numerical accuracy verification with tolerance checking
- Thread efficiency analysis

### 4. MPI Reduction (`4_mpi_reduction.c`)

**Purpose**: Implements distributed reduction operations using MPI collective communications.

**Features**:
- Three different MPI reduction strategies:
  - Combined local computation and `MPI_Reduce`
  - Separate computation phases with `MPI_Reduce`
  - `MPI_Allreduce` for broadcasting results to all processes
- Performance comparison of different MPI reduction approaches
- Comprehensive error checking and validation

**Key Implementation Details**:
- Local reduction on each process
- `MPI_Reduce` to combine results at root process
- `MPI_Allreduce` for distributing results to all processes
- Comparison of communication overhead

## Prerequisites

### System Requirements
- Linux environment (AlmaLinux 9)
- GCC compiler with OpenMP support
- OpenMPI library and development files

### MPI Environment Setup

**Important**: To use MPI commands directly (like `mpirun`), you need to configure your environment first.

#### Quick Setup (Recommended)
```bash
# Configure MPI environment for current session
source mpi_env.sh

# Now you can use MPI commands directly:
mpirun -np 4 ./2_mpi_parallel_daxpy 1000000 3.0 4.0
mpirun -np 4 ./4_mpi_reduction 1000000 3.0 4.0
```

#### Without Environment Setup
If you prefer not to configure the environment, you can use the provided test scripts:
```bash
./test_file_2_mpi_parallel.sh              # MPI vector addition tests
./test_file_4_mpi_reduction.sh    # MPI reduction tests
```

### Installation on AlmaLinux
```bash
# Install required packages
dnf install -y gcc openmpi openmpi-devel

# Quick MPI setup (use our provided script)
source mpi_env.sh
```

## Compilation

### Build System

Use the provided Makefile to compile all programs:

```bash
# Compile all programs
make all

# Run quick functionality test
make test

# Show available targets and usage
make help

# Clean compiled files
make clean
```

### Individual Program Compilation

```bash
# Compile specific programs
make 1_openmp_parallel_daxpy
make 2_mpi_parallel_daxpy
make 3_openmp_reduction
make 4_mpi_reduction
```

### Compiler Configuration

**Compiler flags used**:
- `-Wall -Wextra`: Enable comprehensive warnings
- `-O2`: Optimization level 2 for performance
- `-fopenmp`: Enable OpenMP support (for OpenMP programs)

**Compilers**:
- `gcc`: Used for OpenMP programs
- `mpicc`: Used for MPI programs (wraps gcc with MPI libraries)

## Usage

**Note**: Before using MPI commands directly, make sure to configure your environment:
```bash
source mpi_env.sh    # Quick setup
```

### 1. OpenMP Parallel Vector Addition
```bash
# Default parameters (DIM=10000000, x=2.5, y=3.7)
./1_openmp_parallel_daxpy

# Custom parameters
./1_openmp_parallel_daxpy <DIM> <x_modulus> <y_modulus>
./1_openmp_parallel_daxpy 5000000 2.0 3.0
```

### 2. MPI Parallel Vector Addition
```bash
# Default parameters (N=1000000, x=3.0, y=4.0)
mpirun -np 4 ./2_mpi_parallel_daxpy

# Custom parameters
mpirun -np 4 ./2_mpi_parallel_daxpy <N> <x_val> <y_val>
mpirun -np 4 ./2_mpi_parallel_daxpy 5000000 2.0 3.0
```

### 3. OpenMP Reduction
```bash
# Default parameters (N=10000000, x=3.0, y=4.0)
./3_openmp_reduction

# Custom parameters
./3_openmp_reduction <N> <x_val> <y_val>
./3_openmp_reduction 5000000 2.0 3.0
```

### 4. MPI Reduction
```bash
# Default parameters (N=1000000, x=3.0, y=4.0)
mpirun -np 4 ./4_mpi_reduction

# Custom parameters
mpirun -np 4 ./4_mpi_reduction <N> <x_val> <y_val>
mpirun -np 4 ./4_mpi_reduction 5000000 2.0 3.0
```

## Docker Environment Issues

### Problem Description
When running MPI programs in Docker containers, you may encounter "Bus error" crashes due to OpenFabrics Interface (OFI) compatibility issues. This is a common problem when high-performance networking interfaces are not available in containerized environments.

### Error Symptoms
```
[container:PID] Caught signal 7 (Bus error: nonexistent physical address)
==== backtrace ====
libfabric.so.1(+0x314c6a)
mca_mtl_ofi.so(+0xce98)
...
```

### Solution
Use these MPI runtime parameters to force TCP transport and disable problematic interfaces:

```bash
# Required MPI options for Docker compatibility
MPI_OPTS="--allow-run-as-root --mca pml ob1 --mca btl tcp,self --mca mtl ^ofi --oversubscribe"

# Example usage
mpirun $MPI_OPTS -np 4 ./2_mpi_parallel_daxpy
```

**Parameter explanations**:
- `--allow-run-as-root`: Permits MPI execution as root user
- `--mca pml ob1`: Forces use of OB1 point-to-point messaging layer
- `--mca btl tcp,self`: Uses only TCP and loopback transports
- `--mca mtl ^ofi`: Explicitly disables OFI transport
- `--oversubscribe`: Allows more MPI processes than CPU cores

## Performance Analysis

### Metrics Calculated

1. **Execution Time**: Wall-clock time for computation
2. **Speedup**: `S = T_serial / T_parallel`
3. **Efficiency**: `E = S / P` (where P is number of processes/threads)
4. **Relative Performance**: Percentage improvement/degradation

### Expected Performance Characteristics

**OpenMP Performance**:
- Good scalability for large vectors (>10M elements)
- Thread overhead visible with small vectors
- Combined operations generally faster than separate phases
- Typical efficiency: 20-30% on 8 cores for memory-bound operations

**MPI Performance**:
- Higher overhead due to process communication
- Better scalability potential for very large problems

**Vector Size Impact**:
- Small vectors (<1M elements): Serial often fastest due to overhead
- Medium vectors (1-10M elements): Moderate parallelization benefits
- Large vectors (>10M elements): Best parallelization efficiency

## Results Summary

### Typical Performance Results

**OpenMP Results (8 threads, 10M elements)**:
```
Serial time:     0.152799 seconds
OpenMP time:     0.070612 seconds
Speedup:         2.16x
Efficiency:      27.05%
```

**MPI Results (4 processes, 1M elements)**:
```
Serial time:     0.017405 seconds
MPI time:        0.003389 seconds
Speedup:         5.14x
Efficiency:      128.40%
```

### Key Observations

1. **Memory-bound Nature**: Vector addition is limited by memory bandwidth, not computation
2. **Overhead Impact**: Small vectors suffer from parallelization overhead
3. **Scaling Characteristics**: Efficiency decreases with more threads/processes
4. **Docker Performance**: MPI performance affected by containerization overhead

## Tips for Best Results

### Performance Optimization Guidelines

1. **Vector Size Considerations**:
   - Use vector sizes ≥ 1M elements for meaningful parallelization benefits
   - Small vectors (<100K elements) often perform better in serial due to overhead
   - Large vectors (>10M elements) show the best parallelization efficiency

2. **Thread/Process Configuration**:
   - Test with different thread/process counts to find optimal configuration
   - For OpenMP: Start with thread count = number of CPU cores
   - For MPI: Consider 2-4 processes for small to medium vectors
   - Monitor CPU usage: `htop` or `top` to verify resource utilization

3. **System Resources**:
   - Monitor memory usage with large vectors (`free -h`, `htop`)
   - Check available CPU cores: `nproc` (Current system: 8 cores available)
   - Ensure sufficient RAM for vector allocation

4. **Environment Variables**:
   ```bash
   # OpenMP configuration
   export OMP_NUM_THREADS=4           # Set thread count
   export OMP_SCHEDULE=static         # Use static scheduling
   export OMP_PROC_BIND=true          # Enable thread binding
   
   # MPI configuration (for Docker)
   export OMPI_ALLOW_RUN_AS_ROOT=1
   export OMPI_ALLOW_RUN_AS_ROOT_CONFIRM=1
   ```

5. **Measurement Best Practices**:
   - Run multiple iterations for stable timing measurements
   - Use appropriate timing functions (`omp_get_wtime()` vs `clock()`)
   - Consider warm-up runs to eliminate cache effects
   - Verify results correctness before analyzing performance

6. **Testing Strategy**:
   - Start with `./quick_test.sh` to verify functionality
   - Use `./run_benchmarks.sh` for comprehensive analysis
   - Test scaling with incremental thread/process counts
   - Compare different vector sizes to understand overhead impact

## Troubleshooting

### Common Issues

1. **MPI Bus Errors in Docker**
   - **Solution**: Use Docker-compatible MPI flags (see Docker section)
   - **Cause**: OFI interface incompatibility

2. **Poor OpenMP Performance**
   - **Check**: `OMP_NUM_THREADS` environment variable
   - **Solution**: Set appropriate thread count: `export OMP_NUM_THREADS=4`

3. **Compilation Errors**
   - **Missing OpenMP**: Install GCC with OpenMP support
   - **Missing MPI**: Install OpenMPI development packages

4. **Memory Allocation Failures**
   - **Cause**: Insufficient memory for large vectors
   - **Solution**: Reduce vector size or increase available memory

5. **Incorrect Results**
   - **Check**: Numerical precision issues with large sums
   - **Solution**: Use appropriate tolerance in comparisons

### Debugging Commands

```bash
# Check OpenMP configuration
echo $OMP_NUM_THREADS
./your_program | grep "Number of threads"

# Check MPI configuration
mpirun --version
mpirun -np 1 hostname

# Memory usage monitoring
top -p $(pgrep your_program)
```

### Performance Tuning

1. **OpenMP Optimization**:
   ```bash
   export OMP_NUM_THREADS=4              # Optimal thread count
   export OMP_SCHEDULE=static            # Static scheduling
   export OMP_PROC_BIND=true             # Thread binding
   ```

2. **MPI Optimization**:
   ```bash
   # Process binding
   mpirun --bind-to core -np 4 ./program
   
   # NUMA awareness
   mpirun --map-by node:PE=2 -np 4 ./program
   ```

3. **System Configuration**:
   ```bash
   # Disable CPU frequency scaling
   echo performance | sudo tee /sys/devices/system/cpu/cpu*/cpufreq/scaling_governor
   
   # Increase process limits
   ulimit -s unlimited
   ```

---

## Ready to Start?

**Quick start command:** `./quick_test.sh`

This will run basic functionality tests for all programs and verify that your environment is correctly configured.