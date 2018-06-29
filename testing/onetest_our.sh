#!/bin/bash
# This script is used to test all algorithm to compute betweenness centrality
# (bwc/bwc_exec, bwc/bwc_exec -p), on a single test case.
# For model output we use Brandes algorithm (bwc/bwc_exec -b)
# NB: __don't__ use this in the directory testing in the repo root, instead
# use the copy created by make in Release/testing.

if [ "$#" -ne 1 ]; then 
    echo USAGE: input_file
    exit 1
fi

TRUE="../bwc/bwc_exec -b"
CHECK="../bwc/bwc_exec"
TEST=$1

# rip out largest biconnected component from input file
./l2cc < $TEST > $TEST.l2cc

T2CC=$TEST.l2cc

rm true.log
rm our.log
for ((i=1; i<=5; i++)) do
    $TRUE < $T2CC > true 2>> true.log
    $CHECK < $T2CC > check 2>> our.log

    if ./rcmp6 $T2CC check true 2> diffout ; then
        # test passed
        :
    else
        echo "Wrong output, check diffout"
        exit 1
    fi

done
