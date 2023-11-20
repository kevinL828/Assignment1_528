#!/bin/bash -l

# commen avaliable
DIR=myOutPut

C9OUT=cout_9_my.dat
C16OUT=cout_16_my.dat
C4096OUT=cout_4096_my.dat
C4096OUT_OMP=cout_4096_my_omp.dat

F9OUT=fout_9_my.dat
F16OUT=fout_16_my.dat
F4096OUT=fout_4096_my.dat
F4096OUT_OMP=fout_4096_my_omp.dat

today=`date +%Y%m%d`
LOG=$DIR/log_${today}


# create directory and file to store output files
# create directory
mkdir $DIR

# load modules intel compiler
module load compilers/intel/2019u5

# run Makefile
make

# run to get the output data, wirte output to a log file
# cInsertion in single
echo "run: ./ci.exe 9_coords.coord -o $DIR/$C9OUT" >> $LOG
./ci.exe 9_coords.coord -o $DIR/$C9OUT >> $LOG
echo "" >> $LOG

