#!/bin/bash
# This script is used to test Brandes++ to compute betweenness centrality
# (bwc/bwc_exec -p)
# For model output we use Brandes algorithm (bwc/bwc_exec -b)
# NB: __don't__ use this in the directory testing in the repo root, instead
# use the copy created by make in Release/testing.

# Requirements:
# gpmetis, FastCommunityMH, and graclus must be installed and visible.

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
CHECK="../bwc/bwc_exec -p"
TESTS=$1
N=$2
p=$3

runone() {
    TEST=$1
    PART=$2
    TI=$3
    cat $TEST > temp
    cat $PART >> temp

    $CHECK < temp > check 2> /dev/null
    if ./rcmp6 graph.txt check true 2> diffout ; then
        # test passed
        :
    else
        endspin
        echo "Test [$TI] FAILED, check $TEST, $PART, diffout"
        exit 1
    fi
}

for ((i=1; i<=$TESTS; i++)); do
    spin
    # generates G(n, p), write in edge-list format to graph.txt
    ./generator $N $p > graph.txt

    # reads edge list form from graph.txt, writes adjacency list to graph.adj1
    ./pe2a 1 < graph.txt > graph.adj1

    $TRUE < graph.txt > true 2> /dev/null

    gpmetis graph.adj1 10 > /dev/null 2> /dev/null
    # gpmetis graph.adj1 100
    runone graph.txt graph.adj1.part.10 $i
    # runone graph.txt graph.adj1.part.100 $i

    # input format to Mod partition program does not need number of vertices
    # or edges.
    tail -n +2 graph.txt > graph.pairs

    # on first run, the program writes to graph-fc_firstRun.info some
    # diagnostics, including the "best time" for partition.
    FastCommunityMH -f graph.pairs -l firstRun > /dev/null 2> /dev/null
    STEP=$(cat graph-fc_firstRun.info | grep STEP | sed 's/[^0-9]*//g')

    # run the program again, this time write the actual partition
    FastCommunityMH -f graph.pairs -l storePart -c $STEP > /dev/null 2> /dev/null

    # because format for writing partition is different, port it to our
    # supported format, before (finally!) running brandes++ on it.
    ./ppart < graph-fc_storePart.groups > graph.part
    runone graph.txt graph.part $i

    graclus graph.adj1 10 > /dev/null 2> /dev/null
    # graclus graph.adj1 100
    runone graph.txt graph.adj1.part.10 $i
    # runone graph.txt graph.adj1.part.100 $i
done

endspin
rm graph*
echo "all tests OK"
