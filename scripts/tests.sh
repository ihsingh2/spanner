#!/bin/bash

if [ $# -lt 1 ]; then
    echo "usage: $0 <algorithm> [<k>]"
    exit 0
fi

algorithm=$1

if [ $algorithm -gt 0 ]; then
    if [ $# -lt 2 ]; then
        echo "usage: $0 <algorithm> [<k>]"
        exit 0
    fi
    k=$2
fi

input=(
    "USAir97"
    "semeion_10NN"
    "optdigits_10NN"
    "geom"
    "usps_norm_5NN"
    "worms20_10NN"
    "cond-mat-2003"
    "cond-mat-2005"
)

for i in {1..5}; do
    echo --------------- ITERATION $i ---------------
    for file in ${input[@]}; do
        echo $file
        if [ $algorithm -gt 0 ]; then
            ./build/spanner input/$file.mtx $algorithm $k
        else
            ./build/spanner input/$file.mtx $algorithm
        fi;
        echo
    done
done
