#!/bin/bash
# This script is used to test our algorithm to compute betweenness centrality
# (bwc/bwc_exec), on multiple randomly generated test cases.
# For model output we use Brandes algorithm (bwc/bwc_exec -b)
# NB: __don't__ use this in the directory testing in the repo root, instead
# use the copy created by make in Release/testing.

sp="/-\|"
sc=0
spin() {
   printf "\b${sp:sc++:1}"
   ((sc==${#sp})) && sc=0
}
endspin() {
   printf "\r%s\n" "$@"
}

if [ "$#" -ne 3 ]; then 
    echo USAGE: num_tests num_vertices p
    exit 1
fi

TRUE="../bwc/bwc_exec -b"
CHECK="../bwc/bwc_exec"
TESTS=$1
N=$2
p=$3

for ((i=1; i<=$TESTS; i++)); do
    spin
    ./generator $N $p > graph.txt
    ./l2cc < graph.txt > graph.l2cc
    $TRUE < graph.l2cc > true 2> /dev/null
    $CHECK < graph.l2cc > check 2> /dev/null
    if ./rcmp6 graph.l2cc check true 2> diffout ; then
        # test passed
        :
    else
        endspin
        echo "Test [$i] FAILED, check graph.txt, diffout"
        exit 1
    fi
done

endspin
echo "all tests OK"
