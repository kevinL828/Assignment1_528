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


get_c_time()
{
  c_time=`date '+%H:%M:%S %Y-%m-%d'`
}

# create directory and file to store output files
# create directory
mkdir $DIR

# load modules intel compiler
module load compilers/intel/2019u5

# run Makefile
make

# run to get the output data, wirte output to a log file
# cInsertion in single
get_c_time
echo "[${c_time}] run: ./ci.exe 9_coords.coord $DIR/$C9OUT"
echo "[${c_time}] run: ./ci.exe 9_coords.coord $DIR/$C9OUT" >> $LOG
./ci.exe 9_coords.coord $DIR/$C9OUT >> $LOG
echo "" >> $LOG

# run compare.exe
echo "run compare.exe to compare with the standard answer:" >> $LOG
./compare.exe cout_9.dat $DIR/$C9OUT >> $LOG
echo "" >> $LOG
echo "Finished!"

# same for cout_16 and cout_4096 data
get_c_time
echo "[${c_time}] run: ./ci.exe 16_coords.coord $DIR/$C16OUT"
echo "[${c_time}] run: ./ci.exe 16_coords.coord $DIR/$C16OUT" >> $LOG
./ci.exe 16_coords.coord $DIR/$C16OUT >> $LOG
echo "" >> $LOG
echo "run compare.exe to compare with the standard answer:" >> $LOG
./compare.exe cout_16.dat $DIR/$C16OUT >> $LOG
echo "" >> $LOG
echo "Finished!"

get_c_time
echo "[${c_time}] run: ./ci.exe 4096_coords.coord $DIR/$C4096OUT"
echo "[${c_time}] run: ./ci.exe 4096_coords.coord $DIR/$C4096OUT" >> $LOG
./ci.exe 4096_coords.coord $DIR/$C4096OUT >> $LOG
echo "" >> $LOG
echo "run compare.exe to compare with the standard answer:" >> $LOG
./compare.exe cout_4096.dat $DIR/$C4096OUT >> $LOG
echo "" >> $LOG
echo "Finished!"


# same operation for farthest insetion
# fInsertion in single
get_c_time
echo "[${c_time}] run: ./fi.exe 9_coords.coord $DIR/$F9OUT"
echo "[${c_time}] run: ./fi.exe 9_coords.coord $DIR/$F9OUT" >> $LOG
./fi.exe 9_coords.coord $DIR/$F9OUT >> $LOG
echo "" >> $LOG
echo "run compare.exe to compare with the standard answer:" >> $LOG
./compare.exe fout_9.dat $DIR/$F9OUT >> $LOG
echo "" >> $LOG
echo "Finished!"
echo "" >> $LOG
echo "" >> $LOG

get_c_time
echo "[${c_time}] run: ./fi.exe 16_coords.coord $DIR/$F16OUT"
echo "[${c_time}] run: ./fi.exe 16_coords.coord $DIR/$F16OUT" >> $LOG
./fi.exe 16_coords.coord $DIR/$F16OUT >> $LOG
echo "" >> $LOG
echo "run compare.exe to compare with the standard answer:" >> $LOG
./compare.exe fout_16.dat $DIR/$F16OUT >> $LOG
echo "" >> $LOG
echo "Finished!"
echo "" >> $LOG
echo "" >> $LOG


get_c_time
echo "[${c_time}] run: ./fi.exe 4096_coords.coord $DIR/$F4096OUT"
echo "[${c_time}] run: ./fi.exe 4096_coords.coord $DIR/$F4096OUT" >> $LOG
./fi.exe 4096_coords.coord $DIR/$F4096OUT >> $LOG
echo "" >> $LOG
echo "run compare.exe to compare with the standard answer:" >> $LOG
./compare.exe fout_4096.dat $DIR/$F4096OUT >> $LOG
echo "" >> $LOG
echo "Finished!"
echo "" >> $LOG
echo "" >> $LOG


# run with omp parallel method by GCC
for x in 1 2 4 8 16 32
do
  get_c_time
  echo "[${c_time}] run: sbatch -c $x openmp.sh comp.exe 4096_coords.coord ${C4096OUT_OMP}_${x}"
  echo "[${c_time}] run: sbatch -c $x openmp.sh comp.exe 4096_coords.coord ${C4096OUT_OMP}_${x}" >> $LOG
  sbatch -c $x openmp.sh comp.exe 4096_coords.coord ${C4096OUT_OMP}_${x} >> $LOG
  echo "" >> $LOG
  echo "run compare.exe to compare with the standard answer:" >> $LOG
  ./compare.exe cout_4096.dat $DIR/${C4096OUT_OMP}_${x} >> $LOG
  echo "" >> $LOG
  echo "Finished!"
  echo "" >> $LOG
  echo "" >> $LOG
  
done
#
# get_c_time
# sbatch -c 4 comp.exe 4096_coords.coord ./myOutPut/cout_4096_omp.dat