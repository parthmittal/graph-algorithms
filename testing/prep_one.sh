#!/bin/bash

if [ "$#" -ne 2 ]; then 
    echo USAGE: input_file output_dir
    exit 1
fi

writeone() {
    PART=$1
    FILE=$2

    cat graph.l2cc > $FILE
    cat $PART >> $FILE
}

INPUT=$1
DIR=$2

mkdir -p $DIR

cp $INPUT $DIR/graph.txt
cp rem_multi $DIR
cp l2cc $DIR
cp pe2a $DIR
cp ppart $DIR

cd $DIR

./rem_multi < graph.txt > graph.simple
./l2cc < graph.simple > graph.l2cc
./pe2a 1 < graph.l2cc > graph.adj1

gpmetis graph.adj1 100 > /dev/null 2> /dev/null
writeone graph.adj1.part.100 metis-100.in
rm graph.adj1.part.100

gpmetis graph.adj1 1000 > /dev/null 2> /dev/null
writeone graph.adj1.part.1000 metis-1000.in
rm graph.adj1.part.1000

tail -n +2 graph.l2cc > graph.pairs

# on first run, the program writes to graph-fc_firstRun.info some
# diagnostics, including the "best time" for partition.
FastCommunityMH -f graph.pairs -l firstRun > /dev/null 2> /dev/null
STEP=$(cat graph-fc_firstRun.info | grep STEP | sed 's/[^0-9]*//g')

# run the program again, this time write the actual partition
FastCommunityMH -f graph.pairs -l storePart -c $STEP > /dev/null 2> /dev/null

# because format for writing partition is different, port it to our
# supported format.
./ppart < graph-fc_storePart.groups > graph.part
writeone graph.part mod.in

rm graph-fc*

graclus graph.adj1 100 > /dev/null 2> /dev/null
writeone graph.adj1.part.100 gc-100.in
rm graph.adj1.part.100

graclus graph.adj1 1000 > /dev/null 2> /dev/null
writeone graph.adj1.part.1000 gc-1000.in
rm graph.adj1.part.1000
