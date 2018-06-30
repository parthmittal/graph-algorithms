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
OUR="../bwc/bwc_exec"
BPP="../bwc/bwc_exec -p"
TEST=$1
T2CC=graph.l2cc

runone_bpp() {
    PART=$1
    PNAME=$2

    cat $TEST > temp
    cat $PART >> temp

    $BPP < temp > check 2>> bpp.$PNAME.log
    if ./rcmp6 temp check true 2> diffout ; then
        # ok, test passed
        :
    else
        echo BPP FAILED, partition scheme $PNAME.
        exit 1
    fi

    rm temp
}

# rip out largest biconnected component from input file
./l2cc < $TEST > $T2CC

# reads edge list form from graph.l2cc, writes adjacency list to graph.adj1
./pe2a 1 < $T2CC > graph.adj1

\rm -f true.log
\rm -f our.log
\rm -f bpp.metis-10.log
\rm -f bpp.metis-100.log
\rm -f bpp.mod.log
\rm -f bpp.gc-10.log
\rm -f bpp.gc-100.log

for ((i=1; i<=5; i++)) do
    $TRUE < $T2CC > true 2>> true.log
    $OUR < $T2CC > check 2>> our.log

    if ./rcmp6 $T2CC check true 2> diffout ; then
        # test passed
        :
    else
        echo "OUR: Wrong output, check diffout"
        exit 1
    fi

    gpmetis graph.adj1 10 > /dev/null 2> /dev/null
    # gpmetis graph.adj1 100 > /dev/null 2> /dev/null

    runone_bpp graph.adj1.part.10 "metis-10"
    # runone_bpp graph.adj1.part.100 "metis-100"

    # input format to Mod partition program does not need number of vertices
    # or edges.
    tail -n +2 $T2CC > graph.pairs

    # on first run, the program writes to graph-fc_firstRun.info some
    # diagnostics, including the "best time" for partition.
    FastCommunityMH -f graph.pairs -l firstRun > /dev/null 2> /dev/null
    STEP=$(cat graph-fc_firstRun.info | grep STEP | sed 's/[^0-9]*//g')

    # run the program again, this time write the actual partition
    FastCommunityMH -f graph.pairs -l storePart -c $STEP > /dev/null 2> /dev/null

    # because format for writing partition is different, port it to our
    # supported format, before (finally!) running brandes++ on it.
    ./ppart < graph-fc_storePart.groups > graph.part
    runone_bpp graph.part "mod"

    graclus graph.adj1 10 > /dev/null 2> /dev/null
    # graclus graph.adj1 100
    runone_bpp graph.adj1.part.10 "gc-10"
    # runone_bpp graph.adj1.part.100 "gc-100"
done

echo "[All times in milliseconds]"
echo "min    max    median    mean    stdev"

echo "=================================================="

echo "Brandes Algorithm:"
datamash min 1 max 1 median 1 mean 1 sstdev 1 < true.log

echo "=================================================="

echo "Our Algorithm:"
datamash min 1 max 1 median 1 mean 1 sstdev 1 < our.log

echo "=================================================="

echo "Brandes++:"

echo "metis-10:"
datamash min 1 max 1 median 1 mean 1 sstdev 1 < bpp.metis-10.log

# echo "--------------------------------------------------"
# 
# echo "metis-100:"
# datamash min 1 max 1 median 1 mean 1 sstdev 1 < bpp.metis-100.log

echo "--------------------------------------------------"

echo "mod:"
datamash min 1 max 1 median 1 mean 1 sstdev 1 < bpp.mod.log

echo "--------------------------------------------------"

echo "gc-10:"
datamash min 1 max 1 median 1 mean 1 sstdev 1 < bpp.gc-10.log

# echo "--------------------------------------------------"
# 
# echo "gc-100:"
# datamash min 1 max 1 median 1 mean 1 sstdev 1 < bpp.gc-100.log

echo "=================================================="
