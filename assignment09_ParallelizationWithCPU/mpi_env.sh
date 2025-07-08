#!/bin/bash

# Simple MPI environment configuration
# Usage: source mpi_env.sh

export PATH=/usr/lib64/openmpi/bin:$PATH
export LD_LIBRARY_PATH=/usr/lib64/openmpi/lib:$LD_LIBRARY_PATH
export OMPI_ALLOW_RUN_AS_ROOT=1
export OMPI_ALLOW_RUN_AS_ROOT_CONFIRM=1
export MPI_OPTS="--allow-run-as-root --mca pml ob1 --mca btl tcp,self --mca mtl ^ofi --oversubscribe"

echo "MPI environment configured. You can now use 'mpirun' and 'mpicc' commands directly."
