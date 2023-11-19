#!/bin/bash -l

# commen avaliable
DIR=myOutPut

C9OUT=cout_9_my.dat
C16OUT=cout_16_my.dat
C4096OUT=cout_4096_my.dat

F9OUT=fout_9_my.dat
F16OUT=fout_16_my.dat
F4096OUT=fout_4096_my.dat

# create directory and file to store output files
# create directory
mkdir $DIR

# create output data files
touch $DIR/$C9OUT
touch $DIR/$C16OUT
touch $DIR/$C4096OUT

touch $DIR/$F9OUT
touch $DIR/$F16OUT
touch $DIR/$F4096OUT


# load modules intel compiler
module load compilers/intel/2019u5 

# run Makefile
make

