#!/bin/bash


inputStart="inputs/input_group"
outputStart="solutions/solution_group"
suffix=".txt"

while true; do
    for i in `seq 1 28`; do
        ./ab $inputStart$i$suffix $outputStart$i$suffix 
    done
done
