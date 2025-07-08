#!/bin/bash

# =============================================================================
# MPI Environment Setup Script
# =============================================================================
# This script configures the environment to use OpenMPI commands directly
# 
# Usage:
#   source setup_mpi_env.sh    # Configure current shell
#   . setup_mpi_env.sh         # Alternative sourcing syntax
#
# After sourcing, you can use MPI commands directly:
#   mpirun -np 4 ./2_mpi_parallel_daxpy 1000000 3.0 4.0
#   mpicc -o program program.c
# =============================================================================

# Colors for output
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
NC='\033[0m' # No Color

echo -e "${GREEN}=== MPI Environment Setup ===${NC}"

# Configure OpenMPI paths
export PATH=/usr/lib64/openmpi/bin:$PATH
export LD_LIBRARY_PATH=/usr/lib64/openmpi/lib:$LD_LIBRARY_PATH

# Configure OpenMPI for Docker/root usage
export OMPI_ALLOW_RUN_AS_ROOT=1
export OMPI_ALLOW_RUN_AS_ROOT_CONFIRM=1

# MPI options for Docker compatibility
export MPI_OPTS="--allow-run-as-root --mca pml ob1 --mca btl tcp,self --mca mtl ^ofi --oversubscribe"

echo -e "${YELLOW}Environment configured with:${NC}"
echo "• OpenMPI binary path: /usr/lib64/openmpi/bin"
echo "• OpenMPI library path: /usr/lib64/openmpi/lib"
echo "• Docker/root permissions: enabled"
echo "• MPI compatibility options: configured"
echo ""

# Check if MPI commands are available
if command -v mpirun &> /dev/null; then
    echo -e "${GREEN}✓ mpirun command is now available${NC}"
    echo -e "${GREEN}✓ mpicc command is now available${NC}"
    echo ""
    
    # Display MPI information
    echo -e "${YELLOW}MPI Information:${NC}"
    echo "• MPI Version: $(mpirun --version 2>/dev/null | head -1)"
    echo "• Available processors: $(nproc)"
    echo ""
    
    echo -e "${YELLOW}Usage examples:${NC}"
    echo "# Direct MPI commands (now available):"
    echo "mpirun -np 4 ./2_mpi_parallel_daxpy 1000000 3.0 4.0"
    echo "mpirun -np 4 ./4_mpi_reduction 1000000 3.0 4.0"
    echo ""
    echo "# With Docker compatibility options:"
    echo "mpirun \$MPI_OPTS -np 4 ./2_mpi_parallel_daxpy 1000000 3.0 4.0"
    echo ""
    echo -e "${GREEN}Environment ready! You can now use MPI commands directly.${NC}"
else
    echo -e "${RED}✗ Error: mpirun command not found${NC}"
    echo "Please check OpenMPI installation:"
    echo "• On AlmaLinux/CentOS/RHEL: sudo dnf install openmpi-devel"
    echo "• On Ubuntu/Debian: sudo apt-get install openmpi-bin libopenmpi-dev"
    echo "• On other systems: check your package manager"
fi
