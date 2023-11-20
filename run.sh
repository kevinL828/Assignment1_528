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
c_time=`date '+%H:%M:%S %Y-%m-%d'`
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
echo "run: ./ci.exe 9_coords.coord $DIR/$C9OUT"
echo ${c_time} >> $LOG
echo "run: ./ci.exe 9_coords.coord $DIR/$C9OUT" >> $LOG
./ci.exe 9_coords.coord $DIR/$C9OUT >> $LOG
echo "" >> $LOG

# run compare.exe
echo "run compare.exe to compare with the standard answer:" >> $LOG
./compare.exe cout_9.dat $DIR/$C9OUT >> $LOG
echo "" >> $LOG
echo "Finished!"

# same for cout_16 and cout_4096 data
echo "run: ./ci.exe 16_coords.coord $DIR/$C16OUT"
echo ${c_time} >> $LOG
echo "run: ./ci.exe 16_coords.coord $DIR/$C16OUT" >> $LOG
./ci.exe 16_coords.coord $DIR/$C16OUT >> $LOG
echo "" >> $LOG
echo "run compare.exe to compare with the standard answer:" >> $LOG
./compare.exe cout_16.dat $DIR/$C16OUT >> $LOG
echo "" >> $LOG
echo "Finished!"

echo "run: ./ci.exe 4096_coords.coord $DIR/$C4096OUT"
echo ${c_time} >> $LOG
echo "run: ./ci.exe 4096_coords.coord $DIR/$C4096OUT" >> $LOG
./ci.exe 4096_coords.coord $DIR/$C4096OUT >> $LOG
echo "" >> $LOG
echo "run compare.exe to compare with the standard answer:" >> $LOG
./compare.exe cout_4096.dat $DIR/$C4096OUT >> $LOG
echo "" >> $LOG
echo "Finished!"


# same operation for farthest insetion
# fInsertion in single
echo "run: ./fi.exe 9_coords.coord $DIR/$F9OUT"
echo ${c_time} >> $LOG
echo "run: ./fi.exe 9_coords.coord $DIR/$F9OUT" >> $LOG
./fi.exe 9_coords.coord $DIR/$F9OUT >> $LOG
echo "" >> $LOG
echo "run compare.exe to compare with the standard answer:" >> $LOG
./compare.exe cout_9.dat $DIR/$F9OUT >> $LOG
echo "" >> $LOG
echo "Finished!"

echo "run: ./fi.exe 16_coords.coord $DIR/$F16OUT"
echo ${c_time} >> $LOG
echo "run: ./fi.exe 16_coords.coord $DIR/$F16OUT" >> $LOG
./fi.exe 16_coords.coord $DIR/$F16OUT >> $LOG
echo "" >> $LOG
echo "run compare.exe to compare with the standard answer:" >> $LOG
./compare.exe cout_16.dat $DIR/$F16OUT >> $LOG
echo "" >> $LOG
echo "Finished!"

echo "run: ./fi.exe 4096_coords.coord $DIR/$F4096OUT"
echo ${c_time} >> $LOG
echo "run: ./fi.exe 4096_coords.coord $DIR/$F4096OUT" >> $LOG
./fi.exe 4096_coords.coord $DIR/$F4096OUT >> $LOG
echo "" >> $LOG
echo "run compare.exe to compare with the standard answer:" >> $LOG
./compare.exe cout_4096.dat $DIR/$F4096OUT >> $LOG
echo "" >> $LOG
echo "Finished!"