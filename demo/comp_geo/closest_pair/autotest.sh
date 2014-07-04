#!/bin/sh

cd ..
make gen2dpts.exe
cd -
../gen2dpts.exe 0 100 0 100 37 > 1.txt
make test_closest_pair.exe
./test_closest_pair.exe < 1.txt


echo "If the n^2 method and the nlgn method produced the same answer,"
echo "you have passed the test."
