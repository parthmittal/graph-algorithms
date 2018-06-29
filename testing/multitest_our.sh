#!/bin/bash
# This script is used to test our algorithm to compute betweenness centrality
# (bwc/bwc_exec);
# For model output we use Brandes algorithm (bwc/bwc_exec -b)
# NB: __don't__ use this in the directory testing in the repo root, instead
# use the copy created by make in Release/testing.

echo USAGE: num_tests num_vertices p

TRUE="bwc/bwc_exec -b"
CHECK="bwc/bwc_exec"
TESTS=$1
N=$2
p=$3

for i in {1..$TESTS}
do
    ./generator $N $p > graph.txt
    $TRUE < graph.txt > true
    $CHECK < graph.txt > check
    DIFF=$(diff true check)
    if [ "$DIFF" != "" ]
        echo "Failed Test #$i, failing test written to graph.txt"
        exit 1
    fi
done
