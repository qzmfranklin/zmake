#! /usr/bin/env sh

make gen2dpts.exe graham_scan.exe test_convex_hull.exe
./gen2dpts.exe 0 100 0 100 15 > 1.txt
./graham_scan.exe < 1.txt > 2.txt
./test_convex_hull.exe < 2.txt

./plot_hull.py 2.txt
#./test_convex_hull.exe < 3.txt # test convex_hull.cpp
