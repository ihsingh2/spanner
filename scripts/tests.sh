#!/bin/bash

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
        ./build/spanner input/$file.mtx
        echo
    done
done
