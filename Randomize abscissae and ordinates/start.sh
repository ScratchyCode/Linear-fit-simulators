#!/bin/bash
DIR=$(dirname "$0")
cd $DIR

gcc Gen.c -lm -o gen.out
gcc Fit.c -lm -o fit.out
./gen.out
./fit.out
gnuplot script.gnuplot -p

sleep 1
rm fit.dat
rm data.dat
rm gen.out
rm fit.out

exit
