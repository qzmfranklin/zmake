#!/bin/sh

cd ..
make gen2dpts.exe
cd -
../gen2dpts.exe 0 100 0 100 15 > 1.txt
make graham_scan.exe test_convex_hull.exe
./graham_scan.exe < 1.txt > 2.txt
./test_convex_hull.exe < 1.txt
../plot_hull.py 2.txt

echo "If you see a plot of a convex hull poping up, you have passed the test."
