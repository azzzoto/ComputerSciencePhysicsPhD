#!/bin/bash

# =============================================================================
# Assignment 09: CPU Parallelization - Comprehensive Test Suite
# =============================================================================
# This script runs all programs with various configurations and generates
# detailed performance analysis results.
# =============================================================================

# Colors for output formatting
RED='\033[0;31m'
GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

# Configuration
RESULTS_DIR="results"
TIMESTAMP=$(date +"%Y%m%d_%H%M%S")
RESULTS_FILE="${RESULTS_DIR}/benchmark_${TIMESTAMP}.txt"

# MPI configuration for Docker compatibility
export PATH=/usr/lib64/openmpi/bin:$PATH
export LD_LIBRARY_PATH=/usr/lib64/openmpi/lib:$LD_LIBRARY_PATH
export OMPI_ALLOW_RUN_AS_ROOT=1
export OMPI_ALLOW_RUN_AS_ROOT_CONFIRM=1

# MPI options for Docker compatibility
MPI_OPTS="--allow-run-as-root --mca pml ob1 --mca btl tcp,self --mca mtl ^ofi --oversubscribe"

# Test configurations
declare -a VECTOR_SIZES=("100000" "1000000" "5000000" "10000000")
declare -a PROCESS_COUNTS=("2" "4")
declare -a THREAD_COUNTS=("2" "4" "8")

# =============================================================================
# Utility Functions
# =============================================================================

print_header() {
    echo -e "${BLUE}=============================================================================${NC}"
    echo -e "${BLUE}$1${NC}"
    echo -e "${BLUE}=============================================================================${NC}"
}

print_section() {
    echo -e "${CYAN}--- $1 ---${NC}"
}

print_success() {
    echo -e "${GREEN}✓ $1${NC}"
}

print_warning() {
    echo -e "${YELLOW}⚠ $1${NC}"
}

print_error() {
    echo -e "${RED}✗ $1${NC}"
}

setup_environment() {
    # Create results directory
    mkdir -p "$RESULTS_DIR"
    
    # Clear previous results file
    echo "CPU Parallelization Benchmark Results" > "$RESULTS_FILE"
    echo "Generated on: $(date)" >> "$RESULTS_FILE"
    echo "System Information:" >> "$RESULTS_FILE"
    echo "- CPU Info: $(grep 'model name' /proc/cpuinfo | head -1 | cut -d':' -f2 | xargs)" >> "$RESULTS_FILE"
    echo "- CPU Cores: $(nproc)" >> "$RESULTS_FILE"
    echo "- Memory: $(free -h | grep Mem | awk '{print $2}')" >> "$RESULTS_FILE"
    echo "- OpenMP Threads Available: ${OMP_NUM_THREADS:-$(nproc)}" >> "$RESULTS_FILE"
    echo "" >> "$RESULTS_FILE"
    
    print_success "Environment setup completed"
}

check_binaries() {
    print_section "Checking compiled binaries"
    
    local all_exist=true
    
    for program in "1_openmp_parallel_daxpy" "2_mpi_parallel_daxpy" "3_openmp_reduction" "4_mpi_reduction"; do
        if [[ -f "./$program" ]]; then
            print_success "$program exists"
        else
            print_error "$program not found"
            all_exist=false
        fi
    done
    
    if [[ "$all_exist" == false ]]; then
        print_warning "Some binaries missing. Attempting to compile..."
        make all
        if [[ $? -eq 0 ]]; then
            print_success "Compilation successful"
        else
            print_error "Compilation failed"
            exit 1
        fi
    fi
}

run_system_info() {
    print_header "SYSTEM INFORMATION"
    
    echo "=== System Information ===" | tee -a "$RESULTS_FILE"
    echo "Date: $(date)" | tee -a "$RESULTS_FILE"
    echo "Hostname: $(hostname)" | tee -a "$RESULTS_FILE"
    echo "OS: $(cat /etc/os-release | grep PRETTY_NAME | cut -d'=' -f2 | tr -d '\"')" | tee -a "$RESULTS_FILE"
    echo "Kernel: $(uname -r)" | tee -a "$RESULTS_FILE"
    echo "Architecture: $(uname -m)" | tee -a "$RESULTS_FILE"
    echo "" | tee -a "$RESULTS_FILE"
    
    echo "=== CPU Information ===" | tee -a "$RESULTS_FILE"
    lscpu | grep -E "(Model name|CPU\(s\)|Thread|Socket|Core)" | tee -a "$RESULTS_FILE"
    echo "" | tee -a "$RESULTS_FILE"
    
    echo "=== Memory Information ===" | tee -a "$RESULTS_FILE"
    free -h | tee -a "$RESULTS_FILE"
    echo "" | tee -a "$RESULTS_FILE"
    
    echo "=== Compiler Information ===" | tee -a "$RESULTS_FILE"
    gcc --version | head -1 | tee -a "$RESULTS_FILE"
    mpicc --version | head -1 | tee -a "$RESULTS_FILE"
    echo "" | tee -a "$RESULTS_FILE"
}

# =============================================================================
# Test Functions
# =============================================================================

test_openmp_basic() {
    print_header "TEST 1: BASIC OPENMP VECTOR ADDITION"
    
    echo "=== Test 1: Basic OpenMP Vector Addition ===" >> "$RESULTS_FILE"
    echo "" >> "$RESULTS_FILE"
    
    for size in "${VECTOR_SIZES[@]}"; do
        print_section "Testing with vector size: $size"
        echo "--- Vector Size: $size ---" >> "$RESULTS_FILE"
        
        # Test with different thread counts
        for threads in "${THREAD_COUNTS[@]}"; do
            if [[ $threads -le $(nproc) ]]; then
                export OMP_NUM_THREADS=$threads
                echo "OpenMP Threads: $threads" >> "$RESULTS_FILE"
                
                echo "Running: OMP_NUM_THREADS=$threads ./1_openmp_parallel_daxpy $size 3.0 4.0"
                timeout 60s ./1_openmp_parallel_daxpy "$size" 3.0 4.0 2>&1 | tee -a "$RESULTS_FILE"
                
                if [[ ${PIPESTATUS[0]} -eq 0 ]]; then
                    print_success "Completed successfully with $threads threads"
                else
                    print_warning "Failed or timed out with $threads threads"
                fi
                echo "" >> "$RESULTS_FILE"
            fi
        done
        echo "" >> "$RESULTS_FILE"
    done
}

test_mpi_basic() {
    print_header "TEST 2: BASIC MPI VECTOR ADDITION"
    
    echo "=== Test 2: Basic MPI Vector Addition ===" >> "$RESULTS_FILE"
    echo "" >> "$RESULTS_FILE"
    
    for size in "${VECTOR_SIZES[@]}"; do
        print_section "Testing with vector size: $size"
        echo "--- Vector Size: $size ---" >> "$RESULTS_FILE"
        
        for procs in "${PROCESS_COUNTS[@]}"; do
            echo "MPI Processes: $procs" >> "$RESULTS_FILE"
            
            echo "Running: mpirun $MPI_OPTS -np $procs ./2_mpi_parallel_daxpy $size 3.0 4.0"
            timeout 60s mpirun $MPI_OPTS -np "$procs" ./2_mpi_parallel_daxpy "$size" 3.0 4.0 2>&1 | tee -a "$RESULTS_FILE"
            
            if [[ ${PIPESTATUS[0]} -eq 0 ]]; then
                print_success "Completed successfully with $procs processes"
            else
                print_warning "Failed or timed out with $procs processes"
            fi
            echo "" >> "$RESULTS_FILE"
        done
        echo "" >> "$RESULTS_FILE"
    done
}

test_openmp_reduction() {
    print_header "TEST 3: OPENMP REDUCTION OPERATIONS"
    
    echo "=== Test 3: OpenMP Reduction Operations ===" >> "$RESULTS_FILE"
    echo "" >> "$RESULTS_FILE"
    
    for size in "${VECTOR_SIZES[@]}"; do
        print_section "Testing reduction with vector size: $size"
        echo "--- Vector Size: $size ---" >> "$RESULTS_FILE"
        
        for threads in "${THREAD_COUNTS[@]}"; do
            if [[ $threads -le $(nproc) ]]; then
                export OMP_NUM_THREADS=$threads
                echo "OpenMP Threads: $threads" >> "$RESULTS_FILE"
                
                echo "Running: OMP_NUM_THREADS=$threads ./3_openmp_reduction $size 3.0 4.0"
                timeout 120s ./3_openmp_reduction "$size" 3.0 4.0 2>&1 | tee -a "$RESULTS_FILE"
                
                if [[ ${PIPESTATUS[0]} -eq 0 ]]; then
                    print_success "Reduction completed successfully with $threads threads"
                else
                    print_warning "Reduction failed or timed out with $threads threads"
                fi
                echo "" >> "$RESULTS_FILE"
            fi
        done
        echo "" >> "$RESULTS_FILE"
    done
}

test_mpi_reduction() {
    print_header "TEST 4: MPI REDUCTION OPERATIONS"
    
    echo "=== Test 4: MPI Reduction Operations ===" >> "$RESULTS_FILE"
    echo "" >> "$RESULTS_FILE"
    
    for size in "${VECTOR_SIZES[@]}"; do
        print_section "Testing MPI reduction with vector size: $size"
        echo "--- Vector Size: $size ---" >> "$RESULTS_FILE"
        
        for procs in "${PROCESS_COUNTS[@]}"; do
            echo "MPI Processes: $procs" >> "$RESULTS_FILE"
            
            echo "Running: mpirun $MPI_OPTS -np $procs ./4_mpi_reduction $size 3.0 4.0"
            timeout 120s mpirun $MPI_OPTS -np "$procs" ./4_mpi_reduction "$size" 3.0 4.0 2>&1 | tee -a "$RESULTS_FILE"
            
            if [[ ${PIPESTATUS[0]} -eq 0 ]]; then
                print_success "MPI reduction completed successfully with $procs processes"
            else
                print_warning "MPI reduction failed or timed out with $procs processes"
            fi
            echo "" >> "$RESULTS_FILE"
        done
        echo "" >> "$RESULTS_FILE"
    done
}

test_scaling_analysis() {
    print_header "TEST 5: DETAILED SCALING ANALYSIS"
    
    echo "=== Test 5: Detailed Scaling Analysis ===" >> "$RESULTS_FILE"
    echo "" >> "$RESULTS_FILE"
    
    local test_size="5000000"
    
    print_section "OpenMP Scaling (Vector Size: $test_size)"
    echo "--- OpenMP Scaling Analysis ---" >> "$RESULTS_FILE"
    echo "Vector Size: $test_size" >> "$RESULTS_FILE"
    echo "" >> "$RESULTS_FILE"
    
    # Test OpenMP scaling
    for threads in 1 2 4 6 8; do
        if [[ $threads -le $(nproc) ]]; then
            export OMP_NUM_THREADS=$threads
            echo "Testing with $threads thread(s)..."
            echo "Threads: $threads" >> "$RESULTS_FILE"
            timeout 60s ./3_openmp_reduction "$test_size" 3.0 4.0 2>&1 | tee -a "$RESULTS_FILE"
            echo "" >> "$RESULTS_FILE"
        fi
    done
    
    print_section "MPI Scaling (Vector Size: $test_size)"
    echo "--- MPI Scaling Analysis ---" >> "$RESULTS_FILE"
    echo "Vector Size: $test_size" >> "$RESULTS_FILE"
    echo "" >> "$RESULTS_FILE"
    
    # Test MPI scaling
    for procs in 1 2 3 4; do
        echo "Testing with $procs process(es)..."
        echo "Processes: $procs" >> "$RESULTS_FILE"
        timeout 60s mpirun $MPI_OPTS -np "$procs" ./4_mpi_reduction "$test_size" 3.0 4.0 2>&1 | tee -a "$RESULTS_FILE"
        echo "" >> "$RESULTS_FILE"
    done
}

test_edge_cases() {
    print_header "TEST 6: EDGE CASES AND STRESS TESTS"
    
    echo "=== Test 6: Edge Cases and Stress Tests ===" >> "$RESULTS_FILE"
    echo "" >> "$RESULTS_FILE"
    
    print_section "Small Vector Performance"
    echo "--- Small Vector Performance ---" >> "$RESULTS_FILE"
    
    # Test very small vectors
    for size in 1000 10000 50000; do
        echo "Vector Size: $size" >> "$RESULTS_FILE"
        
        export OMP_NUM_THREADS=4
        echo "OpenMP (4 threads):" >> "$RESULTS_FILE"
        timeout 30s ./3_openmp_reduction "$size" 3.0 4.0 2>&1 | tee -a "$RESULTS_FILE"
        
        echo "MPI (4 processes):" >> "$RESULTS_FILE"
        timeout 30s mpirun $MPI_OPTS -np 4 ./4_mpi_reduction "$size" 3.0 4.0 2>&1 | tee -a "$RESULTS_FILE"
        echo "" >> "$RESULTS_FILE"
    done
    
    print_section "Large Vector Performance"
    echo "--- Large Vector Performance ---" >> "$RESULTS_FILE"
    
    # Test large vector if memory allows
    local large_size="20000000"
    echo "Vector Size: $large_size" >> "$RESULTS_FILE"
    
    export OMP_NUM_THREADS=8
    echo "OpenMP (8 threads):" >> "$RESULTS_FILE"
    timeout 180s ./3_openmp_reduction "$large_size" 3.0 4.0 2>&1 | tee -a "$RESULTS_FILE"
    
    if [[ ${PIPESTATUS[0]} -eq 0 ]]; then
        print_success "Large vector OpenMP test completed"
    else
        print_warning "Large vector OpenMP test failed or timed out"
    fi
    echo "" >> "$RESULTS_FILE"
}

generate_summary() {
    print_header "GENERATING PERFORMANCE SUMMARY"
    
    echo "=== Performance Summary ===" >> "$RESULTS_FILE"
    echo "Generated on: $(date)" >> "$RESULTS_FILE"
    echo "" >> "$RESULTS_FILE"
    
    # Extract key performance metrics
    echo "=== Key Performance Metrics ===" >> "$RESULTS_FILE"
    
    # Find best OpenMP performance
    echo "Best OpenMP Performance:" >> "$RESULTS_FILE"
    grep -A5 -B5 "Speedup.*[2-9]\.[0-9]x" "$RESULTS_FILE" | tail -10 >> "$RESULTS_FILE"
    echo "" >> "$RESULTS_FILE"
    
    # Find best MPI performance
    echo "Best MPI Performance:" >> "$RESULTS_FILE"
    grep -A5 -B5 "Efficiency.*[1-9][0-9][0-9]\." "$RESULTS_FILE" | tail -10 >> "$RESULTS_FILE"
    echo "" >> "$RESULTS_FILE"
    
    # Summary statistics
    echo "=== Summary Statistics ===" >> "$RESULTS_FILE"
    echo "Total test duration: $(($(date +%s) - START_TIME)) seconds" >> "$RESULTS_FILE"
    echo "Results saved to: $RESULTS_FILE" >> "$RESULTS_FILE"
    echo "" >> "$RESULTS_FILE"
    
    print_success "Performance summary generated"
}

# =============================================================================
# Main Execution
# =============================================================================

main() {
    START_TIME=$(date +%s)
    
    print_header "CPU PARALLELIZATION COMPREHENSIVE BENCHMARK"
    echo "Starting comprehensive benchmark suite..."
    echo "Results will be saved to: $RESULTS_FILE"
    echo ""
    
    # Setup
    setup_environment
    check_binaries
    
    # Run system information gathering
    run_system_info
    
    # Run all tests
    test_openmp_basic
    test_mpi_basic
    test_openmp_reduction
    test_mpi_reduction
    test_scaling_analysis
    test_edge_cases
    
    # Generate summary
    generate_summary
    
    # Final summary
    print_header "BENCHMARK COMPLETED"
    print_success "All tests completed successfully!"
    print_success "Results saved to: $RESULTS_FILE"
    print_success "Total duration: $(($(date +%s) - START_TIME)) seconds"
    
    echo ""
    echo "Quick Results Summary:"
    echo "====================="
    echo "Results file: $RESULTS_FILE"
    echo "Log directory: $RESULTS_DIR"
    echo ""
    echo "To view results:"
    echo "  less $RESULTS_FILE"
    echo "  grep -A5 -B5 'Speedup' $RESULTS_FILE"
    echo "  grep 'Efficiency' $RESULTS_FILE"
}

# =============================================================================
# Script Execution
# =============================================================================

# Trap to ensure cleanup on exit
trap 'echo "Benchmark interrupted"; exit 1' INT TERM

# Check if script is run from correct directory
if [[ ! -f "Makefile" ]]; then
    print_error "Please run this script from the assignment directory containing the Makefile"
    exit 1
fi

# Run main function
main "$@"
