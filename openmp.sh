#!/bin/bash -l

# Specific course queue and max wallclock time
#SBATCH -p course -t 2

# Defaults on Barkla (but set to be safe)
## Specify the current working directory as the location for executables/files
#SBATCH -D ./
## Export the current environment to the compute node
#SBATCH --export=ALL
#SBATCH --time=1:00:00

# load modules intel compiler
module load compilers/intel/2019u5 

## SLURM terms
## nodes            relates to number of nodes
## ntasks-per-node  relates to MPI processes per node
## cpus-per-task    relates to OpenMP threads (per MPI process)

echo "Node list                    : $SLURM_JOB_NODELIST"
echo "Number of nodes allocated    : $SLURM_JOB_NUM_NODES or $SLURM_NNODES"
echo "Number of threads or processes          : $SLURM_NTASKS"
echo "Number of processes per node : $SLURM_TASKS_PER_NODE"
echo "Requested tasks per node     : $SLURM_NTASKS_PER_NODE"
echo "Requested CPUs per task      : $SLURM_CPUS_PER_TASK"
echo "Scheduling priority          : $SLURM_PRIO_PROCESS"

# parallel using OpenMP
# SRC = $1 is name of the compiled code
EXE=$1
DAT=$2
OUT=$3

echo $EXE
echo $DAT
echo $OUT
#compilation using intel compiler of sourcecode to exectuable.
echo ./$EXE $DAT $OUT
./$EXE $DAT $OUT
echo ------------------------------------
