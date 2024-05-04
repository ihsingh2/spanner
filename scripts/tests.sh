#!/bin/bash

if [ "$#" -ne 1 ]; then
    echo "usage: tests.sh <t>"
    exit 0
fi

t=$1

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
        ./build/spanner input/$file.mtx $t
        echo
    done
done
