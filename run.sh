#!/bin/bash
for f in /dataset/dataset_*.txt; do 
	base_name=${f%.txt}
    id=${base_name#*_}
    k=${id#*_}
    ./seq "k" "$f" "out1_${f}" "out2_${f}" 
done