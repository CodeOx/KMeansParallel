#!/bin/bash
./seq 4 dataset/dataset_50000_4.txt out1.txt out2.txt
for f in ./dataset/dataset_*.txt; do 
	base_name=${f%.txt}
	id1=${base_name#*_}
	k=${id1#*_}
	./omp $k 2 ${f} out1.txt out2.txt
        ./omp $k 4 ${f} out1.txt out2.txt
	./omp $k 8 ${f} out1.txt out2.txt
	./omp $k 16 ${f} out1.txt out2.txt	
done
